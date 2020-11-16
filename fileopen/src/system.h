#pragma once

#include <stdint.h>
#include <string>
#include <vector>
#include <ctime>
#include <cassert>

#define COPY_BUFFER_SIZE 1024 * 4

namespace e {
	// Write bytes with decoding

	void writeNum8(uint8_t _num8, FILE* _file) {
		fputc(_num8, _file);
	}

	uint8_t readNum8(FILE* _file) { // Take 4 chars
		return fgetc(_file);
	}

	void writeNum16(uint16_t _num16, FILE* _file) {
		uint8_t num8[2];

		num8[0] = (_num16 >> 8) & 0x000000ff;
		num8[1] = _num16 & 0x000000ff;

		fwrite(num8, sizeof(char), 2, _file);
	}

	uint16_t readNum16(FILE* _file) { // Take 4 chars
		uint8_t num8[2];
		fread(num8, sizeof(char), 2, _file);

		uint16_t num16 = 0;

		num16 += num8[0] << 8;
		num16 += num8[1];

		return num16;
	}

	void writeNum32(uint32_t _num32, FILE* _file) {
		uint8_t num8[4];

		num8[0] = _num32 >> 24;
		num8[1] = (_num32 >> 16) & 0x000000ff;
		num8[2] = (_num32 >> 8) & 0x000000ff;
		num8[3] = _num32 & 0x000000ff;

		fwrite(num8, sizeof(char), 4, _file);
	}

	uint32_t readNum32(FILE* _file) { // Take 4 chars
		uint8_t num8[4];
		fread(num8, sizeof(char), 4, _file);

		uint32_t num32 = 0;

		num32 += num8[0] << 24;
		num32 += num8[1] << 16;
		num32 += num8[2] << 8;
		num32 += num8[3];

		return num32;
	}

	// Write and read for wchar_t

	void wfwrite(std::wstring _text, FILE* _file) {
		char bufc[2];
		wchar_t bufwc;
		for (uint16_t i = 0; i < _text.size(); i++) {
			bufwc = _text[i];
			bufc[0] = bufwc >> 8;
			bufc[1] = bufwc & 0x000000ff;
			fwrite(bufc, sizeof(char), 2, _file);
		}
	}

	std::wstring wfread(uint16_t _size, FILE* _file) {
		std::wstring bufws = L"";
		for (uint16_t i = 0; i < _size; i++) {
			char* bufc = new char[2];
			wchar_t bufwc = 0;
			fread(bufc, sizeof(char), 2, _file);
			bufwc += (int)bufc[0] * 256;
			bufwc += bufc[1];
			bufws += bufwc;
			delete bufc;
		}
		return bufws;
	}

	// Full copy <_src> in <_dst>

	void fcopy(FILE* _src, FILE* _dst) {
		uint16_t nread;
		char bufc[COPY_BUFFER_SIZE];
		while (nread = fread(bufc, sizeof(char), COPY_BUFFER_SIZE, _src)) {
			fwrite(bufc, sizeof(char), nread, _dst);
		}
	}

	void fcopyFile(std::wstring _srcPath, FILE* _dst) {
		FILE* src;
		src = _wfopen(_srcPath.c_str(), L"rb");

		fcopy(src, _dst);

		fclose(src);
	}

	// Time mark(4 bytes)

	void fwriteTime(FILE* _file) {
		time_t seconds = time(NULL);
		writeNum32(seconds, _file);
	}

	tm* freadTime(FILE* _file) {
		time_t seconds = readNum32(_file);
		struct tm* t_m;
		t_m = localtime(&seconds);
		return t_m;
	}

}