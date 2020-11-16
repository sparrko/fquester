#pragma once

#include <Windows.h>
#include <string>
#include "system.h"
#include "fgui.h"

namespace FQuester {
	std::wstring OpenFile(char mode) {
		FILE* f;
		std::wstring savepath = AppdataLocal; 
		savepath += L"\\FQuester\\";
		_wmkdir(savepath.c_str());
		savepath += L"openfilename.rlt";
		_wremove(savepath.c_str());
		f = _wfopen(savepath.c_str(), L"wb");
		fputc(mode, f);
		fclose(f);
		
		_wsystem(L"fileopen.exe -");

		f = _wfopen(savepath.c_str(), L"rb");
		uint8_t size = e::readNum8(f);

		std::wstring bufws = L"";
		for (int i = 0; i < size; i++) {
			char bufc[2]; wchar_t bufwc = 0;
			bufc[0] = fgetc(f);
			bufc[1] = fgetc(f);
			bufwc += (int)bufc[0] << 8;
			bufwc += bufc[1];
			bufws += bufwc;
		}

		fclose(f);

		return bufws;
	}
}