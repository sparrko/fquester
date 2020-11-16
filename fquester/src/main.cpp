#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <cassert>
#include <string>

std::wstring MainExeLocal;
std::string TempLocal;
std::wstring AppdataLocal = _wgetenv(L"APPDATA");
std::wstring DocumentsLocal = AppdataLocal;

#include "StartEngine.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:wmainCRTStartup")


// For initialise main pathes

void InitPathces(wchar_t** argv) {
	CHAR wline[MAX_PATH];
	int s;
	int pos;
	
	MainExeLocal = argv[0];

	DocumentsLocal = AppdataLocal;
	DocumentsLocal.erase(DocumentsLocal.length() - 15, 15);
	DocumentsLocal += L"Documents\\";

	GetSystemDirectory(wline, MAX_PATH);
	TempLocal = wline;
	s = 0;
	pos = TempLocal.length() - 1;
	while (s != 1) {
		if (TempLocal[pos] != '\\') pos--;
		else { 
			s++;
			TempLocal.erase(pos + 1, TempLocal.length() - pos - 1);
		}
	}

	TempLocal += "temp\\";
}


// Main

int wmain(int argc, wchar_t** argv) {
	 setlocale(LC_ALL, "Russian");
	
	InitPathces(argv);
	
	Engine engine;
	
	engine.Init();
	engine.WhileWork();
	engine.Quit();
	
	return EXIT_SUCCESS;
}