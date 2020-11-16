#include <Windows.h>
#include "system.h"

#include <iostream>
#include <string>

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

std::wstring savepath = _wgetenv(L"APPDATA");

std::wstring OpenImage() {
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH];
	*fileName = 0;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"Image files (*.png; *.jpg; *.bmp)\0*.png;*.jpg;*.bmp\0All files\0*.*\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = L"”кажите путь к изображению";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	std::wstring buf(ofn.lpstrFile);
	return buf;
}
std::wstring OpenVideo() {
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH];
	*fileName = 0;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"Video files (*.mp4; *.avi)\0*.mp4;*.avi\0All files\0*.*\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = L"”кажите путь к видео";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	std::wstring buf(ofn.lpstrFile);
	return buf;
}
std::wstring OpenAudio() {
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH];
	*fileName = 0;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"Audio files (*.mp3; *.wav; *.ogg)\0*.mp3;*.wav;*.ogg\0All files\0*.*\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = L"”кажите путь к аудио";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	std::wstring buf(ofn.lpstrFile);
	return buf;
}
std::wstring OpenPack() {
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH];
	*fileName = 0;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"Quester Ask List (*.qal)\0*.qal\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = L"”кажите путь к паку";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetOpenFileName(&ofn);
	std::wstring buf(ofn.lpstrFile);
	return buf;
}
std::wstring SavePack() {
	OPENFILENAME ofn;
	TCHAR fileName[MAX_PATH];
	*fileName = 0;

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"Quester Ask List (*.qal)\0*.qal\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = L"”кажите путь";
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	GetSaveFileName(&ofn);
	std::wstring buf(ofn.lpstrFile);
	if (buf.size() > 0 && buf.substr(buf.size() - 4, 4) != L".qal") {
		buf += L".qal";
	}
	return buf;
}

int main(int argc, char** argv) {
	// Check key
	if (argc < 1) std::exit(0);

	// Init
	savepath += L"\\FQuester\\";
	_wmkdir(savepath.c_str());
	savepath += L"openfilename.rlt";

	// Read mode
	char mode;
	FILE* f = _wfopen(savepath.c_str(), L"rb");
	mode = e::readNum8(f);
	fclose(f);

	// Get path
	std::wstring mpath;
	if (mode == 'I') mpath = OpenImage();
	else if (mode == 'V') mpath = OpenVideo();
	else if (mode == 'A') mpath = OpenAudio();
	else if (mode == 'Q') mpath = OpenPack();
	else if (mode == 'S') mpath = SavePack();

	// Save
	f = _wfopen(savepath.c_str(), L"wb");
	if (mpath.size() > 0) {
		e::writeNum8(mpath.size(), f);
		e::wfwrite(mpath.c_str(), f);
	}
	fclose(f);
}