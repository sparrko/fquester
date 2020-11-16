#include <iostream>
#include <stdint.h>
#include <string>
#include <cwchar>
#include <time.h>
#include <vector>
#include <algorithm>
#include <io.h>

#pragma once

#include "system.h"
#include <algorithm>
#include <iostream>

#define QM_VERSION '1'
bool qm_compress = true;

#include <io.h>

bool FileExists(std::wstring fname)
{
	return _waccess(fname.c_str(), 0) != -1;
}

namespace FQuester {
	bool QAL_FileExists(std::string fname) { return access(fname.c_str(), 0) != -1; }
	bool QAL_WFileExists(std::wstring fname) { return _waccess(fname.c_str(), 0) != -1; }

	struct QM_Ask {
		// Main
		char type; // T I A V
		std::wstring ask;
		std::wstring answer;
		uint16_t points;
		// Res
		std::wstring path;
		std::wstring startTime;
		std::wstring durTime;
	};

	struct QM_Topic {
		std::wstring name;
		std::vector <QM_Ask> asks;
	};

	struct QM_Sort {
		bool operator () (const QM_Ask& _a, const QM_Ask& _b) {
			return _a.points < _b.points;
		}
	};

	struct QM_Save {
		std::wstring name;
		std::wstring autor;
		std::vector <QM_Topic> topics;
		std::wstring error;

		QM_Save() {
		}

		void sort(uint16_t posTopic) {
			std::sort(topics[posTopic].asks.begin(), topics[posTopic].asks.end(), QM_Sort());
		}

		uint16_t getAsksLenght(std::wstring topicName) {
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == topicName) {
					return topics[i].asks.size();
				}
			}
		}

		std::wstring getShortAsk(std::wstring topicName, uint16_t pos) {
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == topicName) {
					std::wstring buf = L"";
					buf += std::to_wstring((long long)topics[i].asks[pos].points);
					buf += L"$ - ";
					buf += topics[i].asks[pos].ask;
					buf += L"(";
					buf += topics[i].asks[pos].answer;
					buf += L")";
					return buf;
				}
			}
		}

		void setAutor(std::wstring _autor) { autor = _autor; }
		std::wstring getAutor() { return autor; }

		void setName(std::wstring _name) { name = _name; }
		std::wstring getName() { return name; }

		void addTopic(std::wstring _nameTopic) {
			topics.resize(topics.size() + 1);
			topics[topics.size() - 1].name = _nameTopic;
		}
		void deleteTopic(std::wstring _nameTopic) {
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == _nameTopic) {
					topics.erase(topics.begin() + i);
					break;
				}
			}
		}

		void addAskText(std::wstring _topic,
			std::wstring _ask,
			std::wstring _answer,
			uint16_t _points)
		{
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == _topic) {
					topics[i].asks.resize(topics[i].asks.size() + 1);
					topics[i].asks[topics[i].asks.size() - 1].type = 'T';
					topics[i].asks[topics[i].asks.size() - 1].ask = _ask;
					topics[i].asks[topics[i].asks.size() - 1].answer = _answer;
					if (_points > 10000) _points = 10000;
					topics[i].asks[topics[i].asks.size() - 1].points = _points;
					sort(i);
					break;
				}
			}
		}

		bool addAskImage(std::wstring _topic,
			std::wstring _ask,
			std::wstring _answer,
			uint16_t _points,
			std::wstring _path)
		{
			if (!QAL_WFileExists(_path)) return false;

			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == _topic) {
					topics[i].asks.resize(topics[i].asks.size() + 1);
					topics[i].asks[topics[i].asks.size() - 1].type = 'I';
					topics[i].asks[topics[i].asks.size() - 1].ask = _ask;
					topics[i].asks[topics[i].asks.size() - 1].answer = _answer;
					if (_points > 10000) _points = 10000;
					topics[i].asks[topics[i].asks.size() - 1].points = _points;
					topics[i].asks[topics[i].asks.size() - 1].path = _path;
					sort(i);
					break;
				}
			}

			return true;
		}

		void addAskAudio(std::wstring _topic,
			std::wstring _ask,
			std::wstring _answer,
			uint16_t _points,
			std::wstring _path,
			std::wstring _startTime,
			std::wstring _durTime
		)
		{
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == _topic) {
					topics[i].asks.resize(topics[i].asks.size() + 1);
					topics[i].asks[topics[i].asks.size() - 1].type = 'A';
					topics[i].asks[topics[i].asks.size() - 1].ask = _ask;
					topics[i].asks[topics[i].asks.size() - 1].answer = _answer;
					if (_points > 10000) _points = 10000;
					topics[i].asks[topics[i].asks.size() - 1].points = _points;
					topics[i].asks[topics[i].asks.size() - 1].path = _path;
					topics[i].asks[topics[i].asks.size() - 1].startTime = _startTime;
					topics[i].asks[topics[i].asks.size() - 1].durTime = _durTime;
					sort(i);
					break;
				}
			}
		}

		void addAskVideo(std::wstring _topic,
			std::wstring _ask,
			std::wstring _answer,
			uint16_t _points,
			std::wstring _path,
			std::wstring _startTime,
			std::wstring _durTime
		)
		{
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == _topic) {
					topics[i].asks.resize(topics[i].asks.size() + 1);
					topics[i].asks[topics[i].asks.size() - 1].type = 'V';
					topics[i].asks[topics[i].asks.size() - 1].ask = _ask;
					topics[i].asks[topics[i].asks.size() - 1].answer = _answer;
					if (_points > 10000) _points = 10000;
					topics[i].asks[topics[i].asks.size() - 1].points = _points;
					topics[i].asks[topics[i].asks.size() - 1].path = _path;
					topics[i].asks[topics[i].asks.size() - 1].startTime = _startTime;
					topics[i].asks[topics[i].asks.size() - 1].durTime = _durTime;
					sort(i);
					break;
				}
			}
		}
		
		void deleteAsk(std::wstring _topic, uint16_t _pos) {
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == _topic) {
					topics[i].asks.erase(topics[i].asks.begin() + _pos);
					break;
				}
			}
		}
		QM_Ask getTempAsk(std::wstring _topic, uint16_t _pos) {
			for (uint16_t i = 0; i < topics.size(); i++) {
				if (topics[i].name == _topic) {
					return topics[i].asks[_pos];
				}
			}
		}

		std::wstring getError() { return error; }

		///////////////////////////////////////////////////////

		bool canBuild() {
			// Проверки
			if (topics.size() == 0) { error = L"Нету тем."; return false; }
			uint16_t asks_num = 0;
			for (int i = 0; i < topics.size(); i++) {
				asks_num += topics[i].asks.size();
			}
			if (asks_num == 0) { error = L"Нету вопросов."; return false; }

			return true;
		}

		bool build(std::wstring local) {
			// Проверки
			if (!canBuild()) return false;

			FILE* f;
			f = _wfopen(local.c_str(), L"wb");
			
			// Write head
			fwrite("QAL", sizeof(char), 3, f);
			fputc(QM_VERSION, f);
			e::fwriteTime(f);
			e::writeNum8(getName().size(), f); e::wfwrite(getName(), f);
			e::writeNum8(getAutor().size(), f); e::wfwrite(getAutor(), f);

			try {
				// Info QM
				e::writeNum8(topics.size(), f);
				for (uint16_t i = 0; i < topics.size(); i++) {
					e::writeNum8(topics[i].name.size(), f); e::wfwrite(topics[i].name, f);
					e::writeNum8(topics[i].asks.size(), f);
					for (uint16_t j = 0; j < topics[i].asks.size(); j++) {
						// Write head of ask
						fputc(topics[i].asks[j].type, f);
						e::writeNum8(topics[i].asks[j].ask.size(), f); e::wfwrite(topics[i].asks[j].ask, f);
						e::writeNum8(topics[i].asks[j].answer.size(), f); e::wfwrite(topics[i].asks[j].answer, f);
						e::writeNum16(topics[i].asks[j].points, f);
						// Other
						uint32_t res_size;
						FILE* res;
						std::wstring command; std::wstring pathz;
						switch (topics[i].asks[j].type) {
						case 'I':
							res = _wfopen(topics[i].asks[j].path.c_str(), L"rb");
							_fseeki64(res, 0, SEEK_END); res_size = _ftelli64(res); e::writeNum32(res_size, f);
							_fseeki64(res, 0, SEEK_SET); e::fcopy(res, f);
							fclose(res);
							break;
						case 'A':
							command = L"ffmpegn.exe -i \"";
							command += topics[i].asks[j].path;
							command += L"\" ";
							if (!topics[i].asks[j].startTime.empty() && !topics[i].asks[j].durTime.empty()) {
								command += L" -ss "; command += topics[i].asks[j].startTime;
								command += L" -t "; command += topics[i].asks[j].durTime;
							}

							pathz = std::wstring(TempLocal.begin(), TempLocal.end());
							pathz += L"/result.ogg";
							command += L" \"";
							command += pathz;
							command += L"\"";

							_wsystem(command.c_str());

							res = _wfopen(pathz.c_str(), L"rb");
							_fseeki64(res, 0, SEEK_END); res_size = _ftelli64(res); e::writeNum32(res_size, f);
							_fseeki64(res, 0, SEEK_SET); e::fcopy(res, f);
							fclose(res);

							_wremove(pathz.c_str());

							break;
						case 'V':
							// Create audio
							command = L"ffmpegn.exe -i \"";
							command += topics[i].asks[j].path;
							command += L"\" -vn";
							if (!topics[i].asks[j].startTime.empty() && !topics[i].asks[j].durTime.empty()) {
								command += L" -ss "; command += topics[i].asks[j].startTime;
								command += L" -t "; command += topics[i].asks[j].durTime;
							}
							std::wstring pathz;
							pathz = std::wstring(TempLocal.begin(), TempLocal.end());
							pathz += L"/result.ogg";
							command += L" \"";
							command += pathz;
							command += L"\"";
							_wsystem(command.c_str());

							if (QAL_WFileExists(pathz)) {
								res = _wfopen(pathz.c_str(), L"rb");
								_fseeki64(res, 0, SEEK_END); res_size = _ftelli64(res); e::writeNum32(res_size, f);
								_fseeki64(res, 0, SEEK_SET); e::fcopy(res, f);
								fclose(res);

								_wremove(pathz.c_str());
							}
							else {
								e::writeNum32(0, f);
							}

							// Create video
							pathz = std::wstring(TempLocal.begin(), TempLocal.end());
							pathz += L"/stepvideo.mp4";

							std::wstring pathd = std::wstring(TempLocal.begin(), TempLocal.end());
							pathd += L"/result.mp4";

							command = L"ffmpegn.exe -i \"";
							command += topics[i].asks[j].path;
							command += L"\" -vf scale=640:-1 -r 24 -an";
							if (!topics[i].asks[j].startTime.empty() && !topics[i].asks[j].durTime.empty()) {
								command += L" -ss "; command += topics[i].asks[j].startTime;
								command += L" -t "; command += topics[i].asks[j].durTime;
							}
							command += L" \"";
							command += pathz;
							command += L"\"";
							_wsystem(command.c_str());

							if (qm_compress) { command = L"ffmpego.exe -i "; command += L" \""; command += pathz; command += L"\""; command += L" \""; command += pathd; command += L"\""; }
							else { command = L"ffmpego.exe -i "; command += L" \""; command += pathz; command += L"\""; command += L" -vb 1M"; command += L" \""; command += pathd; command += L"\""; }
							_wsystem(command.c_str());

							_wremove(pathz.c_str());

							res = _wfopen(pathd.c_str(), L"rb");
							_fseeki64(res, 0, SEEK_END); res_size = _ftelli64(res); e::writeNum32(res_size, f);
							_fseeki64(res, 0, SEEK_SET); e::fcopy(res, f);
							fclose(res);

							_wremove(pathd.c_str());

							break;
						}
					}
				}
			}
			catch (exception e) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Ошибка сборки", e.what(), NULL);
			}
			// Close
			fclose(f);

			return true;
		}
	};
}

FQuester::QM_Save qal;
