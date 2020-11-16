#pragma once

#include "system.h"
#include <iostream>

#define QMR_VERSION '1'

namespace FQuester {
	// Only one
	namespace QM_Reader {
		bool inited = false;
		std::wstring error;
		std::wstring autor;
		std::wstring name;
		tm* time_mark;

		struct QM_Ask {
			// Main
			char type; // T I A V
			std::wstring ask;
			std::wstring answer;
			uint16_t points;
			// Res
			char* res; uint32_t size_res;
			char* res_v; uint32_t size_res_v;
		};

		struct QM_Topic {
			std::wstring name;
			std::vector <QM_Ask> asks;
		};

		std::vector <QM_Topic> topics;

		bool Init(std::wstring _path) {
			FILE* f;
			f = _wfopen(_path.c_str(), L"rb");

			// Read head
			std::string bufs = ""; std::string bufs2 = "QAL"; bufs2 += QMR_VERSION;
			for (uint16_t i = 0; i < 4; i++) bufs += fgetc(f);
			if (bufs != bufs2) { error = L"Данный фаил не является игровым модулем FQuester."; return false; }
			time_mark = e::freadTime(f);

			name = e::wfread(e::readNum8(f), f);
			autor = e::wfread(e::readNum8(f), f);


			// Read data
			uint8_t topicSize = e::readNum8(f);
			for (uint16_t i = 0; i < topicSize; i++) { // Topic move
				topics.resize(topics.size() + 1);
				topics[i].name = e::wfread(e::readNum8(f), f);

				uint8_t asksSize = e::readNum8(f);
				for (uint16_t j = 0; j < asksSize; j++) { // Topic move
					topics[i].asks.resize(topics[i].asks.size() + 1);
					topics[i].asks[j].type = fgetc(f);
					topics[i].asks[j].ask = e::wfread(e::readNum8(f), f);
					topics[i].asks[j].answer = e::wfread(e::readNum8(f), f);
					topics[i].asks[j].points = e::readNum16(f);

					switch (topics[i].asks[j].type) {
					case 'I':
						topics[i].asks[j].size_res = e::readNum32(f);
						topics[i].asks[j].res = new char[topics[i].asks[j].size_res];
						fread(topics[i].asks[j].res, sizeof(char), topics[i].asks[j].size_res, f);
						break;
					case 'A':
						topics[i].asks[j].size_res = e::readNum32(f);
						topics[i].asks[j].res = new char[topics[i].asks[j].size_res];
						fread(topics[i].asks[j].res, sizeof(char), topics[i].asks[j].size_res, f);
						break;
					case 'V':
						topics[i].asks[j].size_res = e::readNum32(f);
						topics[i].asks[j].res = new char[topics[i].asks[j].size_res];
						fread(topics[i].asks[j].res, sizeof(char), topics[i].asks[j].size_res, f);

						topics[i].asks[j].size_res_v = e::readNum32(f);
						topics[i].asks[j].res_v = new char[topics[i].asks[j].size_res_v];
						fread(topics[i].asks[j].res_v, sizeof(char), topics[i].asks[j].size_res_v, f);
						break;
					}

				}
			}

			// Close
			fclose(f);
			inited = true;

			return true;
		}

		void Clear() {
			for (uint16_t i = 0; i < topics.size(); i++) {
				for (uint16_t j = 0; j < topics[i].asks.size(); j++) {
					switch (topics[i].asks[j].type) {
					case 'I':
						free(topics[i].asks[j].res);
						break;
					case 'A':
						free(topics[i].asks[j].res);
						break;
					case 'V':
						free(topics[i].asks[j].res);
						free(topics[i].asks[j].res_v);
						break;
					}
				}
			}
			topics.clear();
			std::vector<QM_Topic>(topics).swap(topics);
		}

		void ShowFullInfo() {
			std::wcout << L"Версия: " << L"QAL" << QMR_VERSION << std::endl;
			std::wcout << L"Время создания: " << time_mark->tm_year + 1900 << L"." << time_mark->tm_mon + 1 << L"." << time_mark->tm_mday << L" " << time_mark->tm_hour << L":" << time_mark->tm_min << L":" << time_mark->tm_sec << std::endl;
			std::wcout << L"Автор: " << autor << L"; Название пака: " << name << std::endl;

			for (uint16_t i = 0; i < topics.size(); i++) {
				std::wcout << L"Тема <" << topics[i].name << L">:" << std::endl;
				for (uint16_t j = 0; j < topics[i].asks.size(); j++) {
					std::wcout << L"\tВопрос: " << topics[i].asks[j].ask << L"; Ответ: " << topics[i].asks[j].answer << L"; Баллы: " << topics[i].asks[j].points;
					switch (topics[i].asks[j].type) {
					case 'T':
						std::wcout << L"; Тип: Текст" << std::endl;
						break;
					case 'I':
						std::wcout << L"; Тип: Картинка(" << topics[i].asks[j].size_res << L" байт)" << std::endl;
						break;
					case 'A':
						std::wcout << L"; Тип: Аудио(" << topics[i].asks[j].size_res << L" байт)" << std::endl;
						break;
					case 'V':
						std::wcout << L"; Тип: Видео(" << topics[i].asks[j].size_res << L" байт аудио и " << topics[i].asks[j].size_res_v << L" байт видео)" << std::endl;
						break;
					}
				}
			}
		}
	}
}