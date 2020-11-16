#pragma once

#include <string>
#include <stdint.h>
#include <vector>
#include "system.h"
#include "SystemConsole.h"
#include "fileDialogs.h"

std::wstring FRP_LIST_PATH;

namespace FQuester {
	namespace FRP {
		bool mustRenderListFrp = true;
		bool FileExists(std::wstring fname)
		{
			return _waccess(fname.c_str(), 0) != -1;
		}
		struct FRPUnit {
			std::wstring path;
			std::wstring name;
			std::wstring autor;
			std::wstring date;
			std::wstring topicsAsks;
		};
		std::vector <FRPUnit> frpList;
		void Show() {
			sc.addDraw("______________________________________________");
			sc.addDraw(L"list.frp:");
			for (int a = 0; a < frpList.size(); a++) {
				std::wstring bufws = L"    - "; bufws += std::to_wstring((long long)a + 1); bufws += L". "; 
				bufws += frpList[a].path; bufws += L"; ";
				bufws += frpList[a].name; bufws += L"; ";
				bufws += frpList[a].autor; bufws += L"; ";
				bufws += frpList[a].date; bufws += L"; ";
				bufws += frpList[a].topicsAsks; bufws += L". ";
				sc.addDraw(bufws);
			}
			sc.addDraw("");
		}
		void Update() {
			if (FileExists(FRP_LIST_PATH + L"list.frp")) {
				FILE* frp = _wfopen((FRP_LIST_PATH + L"list.frp").c_str(), L"rb");
				_fseeki64(frp, 0, SEEK_END); uint32_t cf_size = _ftelli64(frp); _fseeki64(frp, 0, SEEK_SET);

				int ll = 0; char last = '1';
				uint16_t filesNum = 0;
				for (int a = 0; a < cf_size; a++) {
					char bufc = fgetc(frp);
					if (last == bufc && bufc == 0) { ll++; }
					else ll = 0;

					if (ll == 4) { filesNum++; }

					last = bufc;
				}
				_fseeki64(frp, 0, SEEK_SET);

				frpList.clear();
				frpList.resize(filesNum);

				for (int a = 0; a < filesNum; a++) {
					frpList[a].path = e::wfread(e::readNum8(frp), frp);

					frpList[a].name = e::wfread(e::readNum8(frp), frp);
					frpList[a].autor = e::wfread(e::readNum8(frp), frp);

					auto time_mark = e::freadTime(frp); std::wstring date = L"";
					if (time_mark->tm_mday < 10) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_mday); }
					else date += std::to_wstring((long long)time_mark->tm_mday); date += L".";
					if (time_mark->tm_mon < 9) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_mon + 1); }
					else date += std::to_wstring((long long)time_mark->tm_mon + 1); date += L".";
					date += std::to_wstring((long long)time_mark->tm_year + 1900).substr(2, 2); date += L" ";
					if (time_mark->tm_hour < 10) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_hour); }
					else date += std::to_wstring((long long)time_mark->tm_hour); date += L":";
					if (time_mark->tm_min < 10) { date += L"0"; date += std::to_wstring((long long)time_mark->tm_min); }
					else date += std::to_wstring((long long)time_mark->tm_min);
					frpList[a].date = date;

					uint16_t asks = e::readNum16(frp);
					uint16_t topics = e::readNum8(frp);

					frpList[a].topicsAsks = L"";
					frpList[a].topicsAsks += std::to_wstring((long long)asks);
					frpList[a].topicsAsks += L"/";
					frpList[a].topicsAsks += std::to_wstring((long long)topics);

					for (int a = 0; a < 5; a++) e::readNum8(frp);
				}

				fclose(frp);

				mustRenderListFrp = true;
				Show();
			}
		}
		void Delete(uint16_t num) {
			if (FileExists(FRP_LIST_PATH + L"list.frp")) {
				long long start = -1, end = -1; // for cut
				FILE* frp = _wfopen((FRP_LIST_PATH + L"list.frp").c_str(), L"rb");
				_fseeki64(frp, 0, SEEK_END); uint32_t cf_size = _ftelli64(frp); _fseeki64(frp, 0, SEEK_SET);
				int ll = 0; char lastChar = '1'; uint16_t posList = 0;
				for (int a = 0; a < cf_size; a++) {
					char bufc = fgetc(frp);
					if (lastChar == bufc) { ll++; }
					else ll = 0;

					if (ll > 3) { posList++; }
					if (posList == num && start < 0) { if (posList == 0) start = a - 1; else start = a + 1; }
					if (posList - 1 == num && end < 0) { if (posList == 1) end = a; else end = a; break; }

					lastChar = bufc;
				}
				sc.addDraw("FRP must delete: " + std::to_string(start) + " - " + std::to_string(end));

				_fseeki64(frp, 0, SEEK_SET);
				FILE* temp = _wfopen((FRP_LIST_PATH + L"temp.frp").c_str(), L"wb");
				for (int a = 0; a < cf_size; a++) {
					char bufc = fgetc(frp);
					if (a < start || a > end) {
						fputc(bufc, temp);
					}
				}
				fclose(frp); fclose(temp);
				_wremove((FRP_LIST_PATH + L"list.frp").c_str());

				temp = _wfopen((FRP_LIST_PATH + L"temp.frp").c_str(), L"rb");
				frp = _wfopen((FRP_LIST_PATH + L"list.frp").c_str(), L"wb");
				e::fcopy(temp, frp);
				fclose(temp); fclose(frp);
				_wremove((FRP_LIST_PATH + L"temp.frp").c_str());

				std::wstring fileDelete = FRP_LIST_PATH; fileDelete += frpList[num].path;
				_wremove(fileDelete.c_str());

				Update();
			}			
		}
		void Add() {
			auto path = FQuester::OpenFile('Q');
			if(!path.empty()){
				if (FileExists(path)) {
					sc.addDraw(L"FRP try add: " + path);
					FILE * f = _wfopen(path.c_str(), L"rb");
					std::string bufs = ""; for (uint16_t i = 0; i < 4; i++) bufs += fgetc(f);
					if (bufs != "QAL1") { sc.addDraw("    - Error: File is not qal pack!"); Res::aud_warning.play(); return; }
					else {
						std::wstring name; std::wstring autor;
						std::wstring time_mark_ws = L"";
						uint8_t topics; uint16_t asks = 0;

						std::wstring file_name = L"";

						// Read main
						time_t seconds = e::readNum32(f);
						struct tm* time_mark;
						time_mark = localtime(&seconds);

						if (time_mark->tm_mday < 10) { time_mark_ws += L"0"; time_mark_ws += std::to_wstring((long long)time_mark->tm_mday); }
						else time_mark_ws += std::to_wstring((long long)time_mark->tm_mday); time_mark_ws += L".";
						if (time_mark->tm_mon < 9) { time_mark_ws += L"0"; time_mark_ws += std::to_wstring((long long)time_mark->tm_mon + 1); }
						else time_mark_ws += std::to_wstring((long long)time_mark->tm_mon + 1); time_mark_ws += L".";
						time_mark_ws += std::to_wstring((long long)time_mark->tm_year + 1900).substr(2, 2); time_mark_ws += L" ";
						if (time_mark->tm_hour < 10) { time_mark_ws += L"0"; time_mark_ws += std::to_wstring((long long)time_mark->tm_hour); }
						else time_mark_ws += std::to_wstring((long long)time_mark->tm_hour); time_mark_ws += L":";
						if (time_mark->tm_min < 10) { time_mark_ws += L"0"; time_mark_ws += std::to_wstring((long long)time_mark->tm_min); }
						else time_mark_ws += std::to_wstring((long long)time_mark->tm_min);

						name = e::wfread(e::readNum8(f), f);
						autor = e::wfread(e::readNum8(f), f);

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
									_fseeki64(f, e::readNum32(f), SEEK_CUR);
									break;
								case 'A':
									_fseeki64(f, e::readNum32(f), SEEK_CUR);
									break;
								case 'V':
									_fseeki64(f, e::readNum32(f), SEEK_CUR);
									_fseeki64(f, e::readNum32(f), SEEK_CUR);
									break;
								}
							}
							asks += asksSize;
						}

						// Check copies
						for (int a = 0; a < frpList.size(); a++) {
							if (frpList[a].date == time_mark_ws) { sc.addDraw("    - Error: This pack has already been added!"); Res::aud_warning.play(); return; }
						}

						// Get name of file
						std::wstring sargv(path);
						for (int i = sargv.size() - 1; i > -1; i--) {
							if (sargv[i] == L'\\') { file_name += sargv.substr(i + 1, sargv.size() - i); sc.addDraw(L"    - File name: " + file_name); break; }
						}

						// Copy
						std::wstring newCopy = FRP_LIST_PATH; newCopy += file_name;
						FILE* out = _wfopen(newCopy.c_str(), L"wb");
						_fseeki64(f, 0, SEEK_SET);
						e::fcopy(f, out);
						fclose(f); fclose(out);

						// Add new unit in list
						f = _wfopen((FRP_LIST_PATH + L"list.frp").c_str(), L"ab");

						e::writeNum8(file_name.size(), f); e::wfwrite(file_name, f);
						e::writeNum8(name.size(), f); e::wfwrite(name, f); sc.addDraw(L"    - Pack name: " + name);
						e::writeNum8(autor.size(), f); e::wfwrite(autor, f); sc.addDraw(L"    - Autor: " + autor);
						e::writeNum32(seconds, f); sc.addDraw(L"    - Time of create: " + time_mark_ws);
						e::writeNum16(asks, f);
						e::writeNum8(topics, f);
						for (int a = 0; a < 5; a++) e::writeNum8(0, f);

						fclose(f);

						Update(); ////////////////////////////////////////////////////////////////////
					}
				}
				else { sc.addDraw("    - FRP Error: Can't find file!"); Res::aud_warning.play(); }
			}
			else { sc.addDraw("    - FRP Error: File not selected!"); Res::aud_warning.play(); }

		}
		void InitFRP() {
			_wmkdir((DocumentsLocal + L"FQuester").c_str());
			_wmkdir((DocumentsLocal + L"FQuester\\packs").c_str());
			FRP_LIST_PATH = DocumentsLocal + L"FQuester\\packs\\";
			FQuester::FRP::Update();
		}
	}
}