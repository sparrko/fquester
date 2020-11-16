
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:wmainCRTStartup")

#include <iostream>
#include <stdint.h>
#include <ctime>
#include <Windows.h>
#include <string>
#include "system.h"

#define QMR_VERSION 1

std::wstring DocumentsLocal = _wgetenv(L"APPDATA");

void InitPathces() {
	DocumentsLocal.erase(DocumentsLocal.length() - 15, 15);
	DocumentsLocal += L"Documents\\";
}

bool FileExists(std::wstring fname)
{
	return _waccess(fname.c_str(), 0) != -1;
}

int wmain(int argc, wchar_t* argv[])
{
	//setlocale(LC_ALL, "Russian");



	if (argc > 1) // только один аргумент
	{
		InitPathces();

		FILE* f = _wfopen(argv[1], L"rb");

		if (!f) {
			MessageBoxW(NULL, L"Файл не существует.", L"FQuester", MB_OK);
			return 0; 
		}////////////////////////////////////////////   new

		std::string bufs = "";
		for (uint16_t i = 0; i < 4; i++) bufs += fgetc(f);

		if (bufs != "QAL1") { fclose(f); MessageBoxW(NULL, L"Данный фаил не является игровым модулем FQuester или устарел.", L"FQuester", MB_OK); }
		else {
			// Info
			std::wstring name;
			std::wstring autor;
			uint32_t time_mark;
			uint8_t topics;
			uint16_t asks = 0;

			std::wstring file_name = L"";

			// Get name of file
			std::wstring sargv(argv[1]); 
			for (int i = sargv.size() - 1; i > -1; i--) {
				if (sargv[i] == L'\\') { file_name += sargv.substr(i + 1, sargv.size() - i); break; }
			}
			
			// Get time ДД:ММ:ГГ ЧЧ:ММ
			time_mark = e::readNum32(f);
			/*if (time_mark->tm_mday < 10) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_mday); }
			else date += std::to_wstring((long long)time_mark->tm_mday); date += L".";
			if (time_mark->tm_mon < 10) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_mon + 1); }
			else date += std::to_wstring((long long)time_mark->tm_mon + 1); date += L".";
			date += std::to_wstring((long long)time_mark->tm_year + 1900).substr(2, 2); date += L" ";
			if (time_mark->tm_hour < 10) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_hour); }
			else date += std::to_wstring((long long)time_mark->tm_hour); date += L":";
			if (time_mark->tm_min < 10) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_min); }
			else date += std::to_wstring((long long)time_mark->tm_min);*/

			// Get name pack
			name = e::wfread(e::readNum8(f), f);

			// Get name autor
			autor = e::wfread(e::readNum8(f), f);

			// Get topics
			topics = e::readNum8(f);
			for (uint16_t i = 0; i < topics; i++) { // Topic move
				e::wfread(e::readNum8(f), f);
				uint8_t asksSize = e::readNum8(f);
				for (uint16_t j = 0; j < asksSize; j++) { // Topic move
					char type = fgetc(f);
					e::wfread(e::readNum8(f), f);
					e::wfread(e::readNum8(f), f);
					e::readNum16(f);

					switch (type) {
					case 'I':
						fseek(f, e::readNum32(f), SEEK_CUR);
						break;
					case 'A':
						fseek(f, e::readNum32(f), SEEK_CUR);
						break;
					case 'V':
						fseek(f, e::readNum32(f), SEEK_CUR);
						fseek(f, e::readNum32(f), SEEK_CUR);
						break;
					}
				}
				asks += asksSize;
			}

			// Locale frp.exe
			std::wstring locale = argv[0];
			std::wstring bufwstr = DocumentsLocal; bufwstr += L"\\FQuester";
			_wmkdir(bufwstr.c_str());
			bufwstr += L"\\packs";
			_wmkdir(bufwstr.c_str());
			bufwstr += L"\\";

			std::wstring path;

			// Check config
			path = bufwstr;
			path += L'\\';
			path += L"list.frp";
			FILE* frp;
			int filesNum = 0;
			if (FileExists(path)) {
				frp = _wfopen(path.c_str(), L"rb");
				fseek(frp, 0, SEEK_END); uint32_t cf_size = ftell(frp); fseek(frp, 0, SEEK_SET);

				int ll = 0; char last = 1;
				for (int a = 0; a < cf_size; a++) {
					char bufc = fgetc(frp);
					if (last == bufc) { ll++; }
					else ll = 0;

					if (ll > 3) { filesNum++;  }

					last = bufc;
				}
				fseek(frp, 0, SEEK_SET);
				std::wstring bufName, bufAutor;
				uint32_t bufMarkTime;

				for (int a = 0; a < filesNum; a++) {
					e::wfread(e::readNum8(frp), frp);
					bufName = e::wfread(e::readNum8(frp), frp);
					bufAutor = e::wfread(e::readNum8(frp), frp);
					bufMarkTime = e::readNum32(frp);
					e::readNum16(frp);
					e::readNum8(frp);
					for (int a = 0; a < 5; a++) e::readNum8(frp);

					if (bufMarkTime == time_mark) {
						MessageBoxW(NULL, L"Такой пак уже добавлен в игре.", L"FQuester", MB_OK);
						return 0;
					}
				}

				// Copy file
				fseek(f, 0, SEEK_SET);
				std::wstring path = bufwstr;
				path += L'\\'; path += file_name;
				FILE* out = _wfopen(path.c_str(), L"wb");
				e::fcopy(f, out);
				fclose(f); fclose(out);

				fclose(frp);
			}
			else{
				filesNum = 0;

				// Copy file
				fseek(f, 0, SEEK_SET);
				std::wstring path = bufwstr;
				path += L'\\'; path += file_name;
				FILE* out = _wfopen(path.c_str(), L"wb");
				e::fcopy(f, out);
				fclose(f); fclose(out);
			}
			// Write info in config
			frp = _wfopen(path.c_str(), L"a");

			e::writeNum8(file_name.size(), frp); e::wfwrite(file_name, frp);
			e::writeNum8(name.size(), frp); e::wfwrite(name, frp);
			e::writeNum8(autor.size(), frp); e::wfwrite(autor, frp);
			e::writeNum32(time_mark, frp);
			e::writeNum16(asks, frp);
			e::writeNum8(topics, frp);
			for (int a = 0; a < 5; a++) e::writeNum8(0, frp);

			fclose(frp);

			MessageBoxW(NULL, L"Пак успешно добавлен в игру.", L"FQuester", MB_OK);
		}
	}
	/*else
	{
		cout << "Not arguments" << endl;
	}
	system("pause");*/

	

	return 0;
}