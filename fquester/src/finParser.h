#pragma once

#include <vector>
#include <stdint.h>
#include <string>

namespace FIN {
	std::wstring adept(std::wstring data) {
		while (true) {
			if (data[0] == 32 || data[0] == 9) data.erase(0, 1);
			else break;
		}
		while (true) {
			if (data[data.size() - 1] == 32 || data[data.size() - 1] == 9) data.erase(data.size() - 1, 1);
			else break;
		}
		return data;
	}

	struct Unit {
		std::wstring name;
		std::wstring value;
	};

	class Parser {
		std::vector <Unit> data;
	public:
		bool open(std::wstring path) {
			FILE* f = _wfopen(path.c_str(), L"rt, ccs=UTF-8");

			if (!f) return false;

			_fseeki64(f, 0, SEEK_END); uint32_t size = _ftelli64(f);
			_fseeki64(f, 0, SEEK_SET);

			bool hfile = true; uint32_t oldftell = -1;
			while (hfile) {
				// Copy line
				std::wstring line = L"";
				bool bline = true;
				while (bline) {
					wchar_t bufwc = fgetwc(f);

					if (bufwc == L'\n') bline = false;
					else line += bufwc;

					if (_ftelli64(f) == oldftell) { hfile = bline = false; }
					else oldftell = _ftelli64(f);
				}

				// Delete comments
				for (int i = 0; i < line.size(); i++) {
					if (line[i] == L'#') line.erase(line.begin() + i, line.end());
				}

				// Work
				if (line.size() != 0) {
					uint16_t del = 0;
					for (int i = 0; i < line.size(); i++) {
						if (line[i] == L'=') {
							data.resize(data.size() + 1);
							data[data.size() - 1].name = adept(line.substr(0, i));
							data[data.size() - 1].value = adept(line.substr(i + 1, line.size() - i));
							break;
						}
					}
				}
			}

			fclose(f);

			return true;
		}

		std::wstring getText(std::wstring name) {
			for (int i = 0; i < data.size(); i++)
				if (data[i].name == name) { return data[i].value; }
			return L"error";
		}

		long int getInt(std::wstring name) {
			for (int i = 0; i < data.size(); i++)
				if (data[i].name == name) { return std::stol(data[i].value.c_str()); }
			return 0;
		}

		bool getBool(std::wstring name) {
			for (int i = 0; i < data.size(); i++)
				if (data[i].name == name) {
					if (data[i].value[0] == L'1') return true;
					if (data[i].value[0] == L'0') return false;
				}
			return false;
		}

		void showAll() {
			for (int i = 0; i < data.size(); i++) {
				std::wcout << data[i].name << L" = " << data[i].value << std::endl;
			}
		}
	};
}