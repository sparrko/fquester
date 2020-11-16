#pragma once

#include "QAL.h"
#include "QAL_Reader.h"


namespace FQuester {
	// Help
	bool wfexists(std::wstring fname) { return _waccess(fname.c_str(), 0) != -1; }

	// Delete all buffer files
	void ClearUnpackFiles(std::wstring _timeSpace) {
		std::wstring vidloc = _timeSpace; vidloc += L"/"; vidloc += L"buffervid.mp4";
		std::wstring audloc = _timeSpace; audloc += L"/"; audloc += L"bufferaud.ogg";
		std::wstring bufres = _timeSpace; bufres += L"/"; bufres += L"bufferresultvideo.mp4";

		_wremove(vidloc.c_str()); _wremove(audloc.c_str()); _wremove(bufres.c_str());
		
		std::wstring path;
		int miss = 0;
		for (int i = 0;; i++) { // Topics move
			for (int j = 0;; j++) { // Asks move
				path = _timeSpace;
				path += L"/";
				path += std::to_wstring((long long)i);
				path += L"_";
				path += std::to_wstring((long long)j);
				path += L".qap";

				if (wfexists(path)) {
					miss = 0;
					_wremove(path.c_str());
				}
				else {
					miss++;
					break;
				}
			}

			if (miss == 2) break;
		}
	}

	// На этом и заканчивается перспективность проекта 26.09.20
	
	// 1_1 - IdTopic_IdAsk
	// Windows/temp/FQuester/1_1.qup (qal un packing)
	void UnpackQAL(std::wstring _path, std::wstring _timeSpace) {
		// Read
		QM_Reader::Clear();
		try { QM_Reader::Init(_path); }
		catch (exception e) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Ошибка чтения", e.what(), window);
		};
		
		
		// Init
		ClearUnpackFiles(_timeSpace);
		qal.topics.clear();
		qal.name = QM_Reader::name;
		qal.autor = QM_Reader::autor;
		
		// Write data and create project in game
		uint8_t topicSize = QM_Reader::topics.size();
		
		std::wstring path;
		
		for (uint16_t i = 0; i < topicSize; i++) {
			qal.addTopic(QM_Reader::topics[i].name);
			uint8_t asksSize = QM_Reader::topics[i].asks.size();
			
			for (uint16_t j = 0; j < asksSize; j++) {
				path = _timeSpace;
				path += L"/";

				path += std::to_wstring((long long)i);
				path += L"_";
				path += std::to_wstring((long long)j);
				path += L".qap";

				FILE* fresult = _wfopen(path.c_str(), L"wb");

				switch (QM_Reader::topics[i].asks[j].type) {
					case 'T':
						e::writeNum8(0, fresult);
						qal.addAskText(QM_Reader::topics[i].name, QM_Reader::topics[i].asks[j].ask, QM_Reader::topics[i].asks[j].answer, QM_Reader::topics[i].asks[j].points);
						break;
					case 'I':
						fwrite(QM_Reader::topics[i].asks[j].res, 1, QM_Reader::topics[i].asks[j].size_res, fresult);
						qal.addAskImage(QM_Reader::topics[i].name, QM_Reader::topics[i].asks[j].ask, QM_Reader::topics[i].asks[j].answer, QM_Reader::topics[i].asks[j].points, path);
						break;
					case 'A':
						fwrite(QM_Reader::topics[i].asks[j].res, 1, QM_Reader::topics[i].asks[j].size_res, fresult);
						qal.addAskAudio(QM_Reader::topics[i].name, QM_Reader::topics[i].asks[j].ask, QM_Reader::topics[i].asks[j].answer, QM_Reader::topics[i].asks[j].points, path, L"", L"");
						break;
					case 'V':
						// Create full video
						std::wstring vidloc = _timeSpace; vidloc += L"/"; vidloc += L"buffervid.mp4";
						std::wstring audloc = _timeSpace; audloc += L"/"; audloc += L"bufferaud.ogg";
						std::wstring bufres = _timeSpace; bufres += L"/"; bufres += L"bufferresultvideo.mp4";

						_wremove(vidloc.c_str()); _wremove(audloc.c_str()); _wremove(bufres.c_str());

						FILE* fres = _wfopen(audloc.c_str(), L"wb"); fwrite(QM_Reader::topics[i].asks[j].res, 1, QM_Reader::topics[i].asks[j].size_res, fres); fclose(fres);
						FILE* fvid = _wfopen(vidloc.c_str(), L"wb"); fwrite(QM_Reader::topics[i].asks[j].res_v, 1, QM_Reader::topics[i].asks[j].size_res_v, fvid); fclose(fvid);

						// -i video.mp4 -i audio.wav -c copy output.mkv
						std::wstring command;

						command = L"ffmpegn.exe -i \""; command += vidloc; command += L"\""; command += L" -i "; command += L" \""; command += audloc; command += L"\"";
						command += L" -y "; command += L" \""; command += bufres; command += L"\"";

						_wsystem(command.c_str());
						
						FILE* frvb = _wfopen(bufres.c_str(), L"rb");
						e::fcopy(frvb, fresult);
						fclose(frvb);

						qal.addAskVideo(QM_Reader::topics[i].name, QM_Reader::topics[i].asks[j].ask, QM_Reader::topics[i].asks[j].answer, QM_Reader::topics[i].asks[j].points, path, L"", L"");

						_wremove(vidloc.c_str()); _wremove(audloc.c_str()); _wremove(bufres.c_str());

						break;
				}
				
				fclose(fresult);
			}
		}


		// Call update form
		newValuesQuesterRedactor = true;
	}
}