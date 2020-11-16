#pragma once

#include <string>
#include <SDL.h>
#include "fgui_objects.h"
#include "objects.h"
#include "fgui_RWGlobalVars.h"
#include "q_globalVars.h"
#include "frameLimit.h"
#include "crash.h"
#include "names.h"
#include "INFO.h"
#include <vector>

#ifdef _WIN32 || _WIN64
#include <Windows.h>
#include <cassert>
#endif

std::vector <std::string> listCommands;
bool hcv = false;
void openHelpConsoleVector() {
	listCommands.push_back("add (adds points to the team, look \"cheats\")");
	listCommands.push_back("autor (shows the list of authors)");
	listCommands.push_back("cheats (shows how to manipulate team points)");
	listCommands.push_back("cls (clear console)");
	listCommands.push_back("exit (ends the game loop)");
	listCommands.push_back("fastExit (ends the process)");
	listCommands.push_back("fguiInfo (shows fgui version)");
	listCommands.push_back("FGUIcrash1 (crashs fgui, test 1)");
	listCommands.push_back("FGUIcrash2 (crashs fgui, test 2)");
	listCommands.push_back("FGUIcrash3 (crashs fgui, test 3)");
	listCommands.push_back("frameLimit (sets frame limit)");
	listCommands.push_back("fwindows (shows list of all register FGUI-windows)");
	listCommands.push_back("fwin (calls window, look \"fwindows\")");
	listCommands.push_back("help (shows a list of basic commands)");
	listCommands.push_back("names (updates and shows the list of names)");
	listCommands.push_back("patches (shows the main patches)");
	listCommands.push_back("renderInfo (shows the name of render api)");
	listCommands.push_back("set (changes points to the team, look \"cheats\")");
	listCommands.push_back("serf (activates dev mode)");
	listCommands.push_back("sysInfo (shows the main system characteristic)");
	listCommands.push_back("uprender gui (redraws fgui completely)");
}

std::vector <std::string> listWindows;
void openWindowsConsoleVector() {
	listWindows.push_back("auctionWindow");
	listWindows.push_back("console");
	listWindows.push_back("cpuTest");
	listWindows.push_back("exitAsk");
	listWindows.push_back("mediaGuide");
	listWindows.push_back("menu");
	listWindows.push_back("nameList");
	listWindows.push_back("questerRedactor");
	listWindows.push_back("reallyDelete");
	listWindows.push_back("settings");
	listWindows.push_back("start");
}

class SystemConsole {
private:
	std::vector <std::wstring> linesForDraw;
	std::vector <std::wstring> buffer;
	bool mustCls;
public:
	void addDraw(std::string msg) {
		auto buf = std::wstring(msg.begin(), msg.end());
		linesForDraw.push_back(L"] " + buf);
	}
	void addDraw(std::wstring msg) {
		std::wstring buf = L"] "; buf += msg;
		linesForDraw.push_back(buf);
	}
	void addDraw(int msg) {
		std::wstring buf = L"] "; buf += std::to_wstring((long long)msg);
		linesForDraw.push_back(buf);
	}
	void addDraw(float msg) {
		std::wstring buf = L"] "; buf += std::to_wstring((long double)msg);
		linesForDraw.push_back(buf);
	}
	void sendMessage(std::wstring msg) {
		if (!msg.empty()) {
			buffer.push_back(msg);
		}
	}
	void sendMessage(std::string msg) {
		if (!msg.empty()) {
			auto buf = std::wstring(msg.begin(), msg.end());
			buffer.push_back(buf);
		}
	}
	SystemConsole() {
		std::wstring bufws = FQuester::INFO_NAME;
		bufws += L" ";
		bufws += FQuester::INFO_VERSION;
		bufws += L" ";
		bufws += FQuester::INFO_DATE;
		bufws += L" from a Demin Egor.";
		addDraw(bufws);
		addDraw(L"______________________________________________");
		addDraw(L"");

		sendMessage("patches");
		sendMessage("sysInfo");
		sendMessage("renderInfo");
		sendMessage("names");
		sendMessage("autor");
		sendMessage("What new?");
		
		mustCls = false;
	}
	void update() {
		if (!hcv) { openHelpConsoleVector(); openWindowsConsoleVector(); hcv = true; }
		for (int i = 0; i < buffer.size(); i++) {
			auto msg = buffer[i];
			addDraw(msg);
			std::string buf;

			if (msg == L"fastExit") { exit(0); }
			else if (msg == L"exit") { isRunning = false; }
			else if (msg == L"What new?") { addDraw(L" - Now Egor has become kind and decided to add a normal command \'help\'."); addDraw(L""); }
			#ifdef _WIN32 || _WIN64
			else if (msg == L"sysInfo") {
				// Threads architecture
				SYSTEM_INFO sysinfo;
				GetNativeSystemInfo(&sysinfo);
				// Memory
				MEMORYSTATUSEX statex;
				statex.dwLength = sizeof(statex);
				GlobalMemoryStatusEx(&statex);
				long double RAMSIZE = (double)statex.ullTotalPhys / (1024 * 1024 * 1024);

				addDraw(L"______________________________________________");
				addDraw(L"System information:");
				buf = "    - Platform: "; buf += SDL_GetPlatform(); addDraw(buf);

				OSVERSIONINFO osVersion = { 0 }; osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO); const BOOL succeed = GetVersionEx(&osVersion); assert(succeed);
				const unsigned majorVersion = osVersion.dwMajorVersion; const unsigned minorVersion = osVersion.dwMinorVersion;
				buf = "    - Operating System Version(WinApi): "; buf += std::to_string((long long)majorVersion); buf += "."; buf += std::to_string((long long)minorVersion); addDraw(buf);

				buf = "    - CPU Threads: "; buf += std::to_string((long long)sysinfo.dwNumberOfProcessors); if (sysinfo.dwNumberOfProcessors > 1) buf += " (good)"; else buf += " (bad)"; addDraw(buf);
				buf = "    - CPU Architecture: "; if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) buf += "x64"; else if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) buf += "x86"; else buf += "Unknown"; addDraw(buf);
				buf = "    - Public RAM: "; buf += std::to_string(RAMSIZE); buf += " gb"; if (RAMSIZE >= 0.256) buf += " (good)"; else buf += " (bad)"; addDraw(buf);
				addDraw(L" ");
			}
			#endif
			else if (msg == L"fguiInfo") {
				addDraw(L"______________________________________________");
				addDraw(L"FGUI information:");
				buf = "    - Name: "; buf += FGUI_INFO_NAMEENGINE; addDraw(buf);
				buf = "    - Version: "; buf += FGUI_INFO_VERSIONENGINE; addDraw(buf);
				addDraw(L" ");
			}
			else if (msg == L"autors" || msg == L"autor") {
				addDraw(L"______________________________________________");
				addDraw(L"Autors:");
				buf = "    - Demin Egor - project lead, programming and content making"; addDraw(buf);
				buf = "    - Suhanov Oleg - qal maker <HelloWorld.qal>"; addDraw(buf);
				buf = "    - Iksanov Andrey - game designer and web programming"; addDraw(buf);
				buf = "    - Acsenov Albert - original idea developer"; addDraw(buf);
				buf = "    - Ferapontov Vitaly - project customer"; addDraw(buf);
				
				addDraw(L" ");
			}
			else if (msg == L"renderInfo") {
				SDL_RendererInfo info = { 0 };
				SDL_GetRendererInfo(renderer, &info);

				addDraw(L"______________________________________________");
				addDraw(L"Render info: ");
				buf = "    - Api name: "; buf += info.name; addDraw(buf);
				addDraw(L" ");
			}
			else if (msg == L"patches") {
				addDraw(L"______________________________________________");
				addDraw(L"Patches: ");
				addDraw(L"    - AppData: " + AppdataLocal); 
				std::string a = "    - Temp: "; a += TempLocal;
				addDraw(a);
				addDraw(L"    - Main Exe: " + MainExeLocal);
				addDraw(L"    - Documents: " + DocumentsLocal);
				addDraw(L" ");
			}
			else if (msg == L"cls") {
				mustCls = true;
			}
			else if (msg == L"cheats") {
				addDraw(L"______________________________________________");
				addDraw(L"How change points of commands:");
				addDraw(L"    1. \"set\" or \"add\"");
				addDraw(L"    2. \"c1\" or \"c2\" or \"c3\"");
				addDraw(L"    3. <points>");
				addDraw(L"");
			}
			else if (msg.substr(0, 10) == L"frameLimit") {
				std::string bufs(msg.begin(), msg.end());
				n_frameLimit = FRAME_LIMIT = std::atol(bufs.substr(10, msg.size() - 10).c_str());
			}
			else if (msg == L"names") {
				bool ok = FQuester::FNM::Update();

				if (ok) {
					addDraw(L"______________________________________________");
					addDraw(L"List of names(" + std::to_wstring((long long)FQuester::FNM::names.size()) + L"):");

					for (unsigned int i = 0; i < FQuester::FNM::names.size(); i++) {
						sc.addDraw(L"    - " + FQuester::FNM::names[i]);
					}
				}
				else {
					addDraw(L"______________________________________________");
					addDraw(L"FNM: names not found!");
				}
				addDraw(L"");
			}
			else if (msg.substr(0, 10) == L"names add ") {
				FQuester::FNM::Add(msg.substr(10, msg.size() - 10));

				FQuester::FNM::Update();

				addDraw(L"______________________________________________");
				addDraw(L"List of names(" + std::to_wstring((long long)FQuester::FNM::names.size()) + L"):");

				for (unsigned int i = 0; i < FQuester::FNM::names.size(); i++) {
					sc.addDraw(L"    - " + FQuester::FNM::names[i]);
				}
				addDraw(L"");
			}
			else if (msg.substr(0, 10) == L"names del ") {

				FQuester::FNM::Delete(_wtoi(msg.substr(10, msg.size() - 10).c_str()));

				FQuester::FNM::Update();

				addDraw(L"______________________________________________");
				addDraw(L"List of names(" + std::to_wstring((long long)FQuester::FNM::names.size()) + L"):");

				for (unsigned int i = 0; i < FQuester::FNM::names.size(); i++) {
					sc.addDraw(L"    - " + FQuester::FNM::names[i]);
				}
				addDraw(L"");
			}
			else if (msg.substr(0, 3) == L"set") {
				if (msg.substr(4, 2) == L"c1") {
					FQuester::mChangePoints = true;
					FQuester::cForPoints = 0;
					std::string bufs(msg.begin(), msg.end());
					FQuester::newPoints = std::atol(bufs.substr(6, msg.size() - 6).c_str());
				}
				else if (msg.substr(4, 2) == L"c2") {
					FQuester::mChangePoints = true;
					FQuester::cForPoints = 1;
					std::string bufs(msg.begin(), msg.end());
					FQuester::newPoints = std::atol(bufs.substr(6, msg.size() - 6).c_str());
				}
				else if (msg.substr(4, 2) == L"c3") {
					FQuester::mChangePoints = true;
					FQuester::cForPoints = 2;
					std::string bufs(msg.begin(), msg.end());
					FQuester::newPoints = std::atol(bufs.substr(6, msg.size() - 6).c_str());
				}
				else {
					FQuester::mChangePoints = false;
					FQuester::cForPoints = 3;
					FQuester::newPoints = 0;
				}

				if (msg.substr(4, 2) == L"pp") {
					FQuester::mAddPoints = true;
					std::string bufs(msg.begin(), msg.end());
					FQuester::plusPoints = std::atol(bufs.substr(6, msg.size() - 6).c_str());
				}
			}
			else if (msg.substr(0, 3) == L"add") {
				if (msg.substr(4, 2) == L"c1") {
					FQuester::mAddPoints = true;
					FQuester::cForPoints = 0;
					std::string bufs(msg.begin(), msg.end());
					FQuester::newPoints = std::atol(bufs.substr(6, msg.size() - 6).c_str());
				}
				else if (msg.substr(4, 2) == L"c2") {
					FQuester::mAddPoints = true;
					FQuester::cForPoints = 1;
					std::string bufs(msg.begin(), msg.end());
					FQuester::newPoints = std::atol(bufs.substr(6, msg.size() - 6).c_str());
				}
				else if (msg.substr(4, 2) == L"c3") {
					FQuester::mAddPoints = true;
					FQuester::cForPoints = 2;
					std::string bufs(msg.begin(), msg.end());
					FQuester::newPoints = std::atol(bufs.substr(6, msg.size() - 6).c_str());
				}
				else {
					FQuester::mAddPoints = false;
					FQuester::cForPoints = 3;
					FQuester::newPoints = 0;
				}
			}
			else if (msg == L"update render GUI" || msg == L"uprender gui") {
				FGUI::NewSizeWindow();
			}
			else if (msg == L"serf" || msg == L"egor" || msg == L"Egor") {
				if (!devMode) {
					devMode = true;
					addDraw(L"    - Dev mode: on");
				}
				else {
					devMode = false;
					addDraw(L"    - Dev mode: off");
				}
			}
			else if (msg == L"fwin mediaGuide") { activateMediaGuide = true; }
			else if (msg == L"fwin reallyDelete") { activateReallyDelete = true; }
			else if (msg == L"fwin questerRedactor") { activateQuesterRedactor = true; }
			else if (msg == L"fwin settings") { activateSettings = true; }
			else if (msg == L"fwin start") { activateStart = true; }
			else if (msg == L"fwin console") { activateConsole = true; }
			else if (msg == L"fwin exitAsk") { activateExitAsk = true; }
			else if (msg == L"fwin auctionWindow") { activateAuctionWindow = true; }
			else if (msg == L"fwin cpuTest") { activateCPUTest = true; }
			else if (msg == L"fwin nameList") { activateNameList = true; }
			else if (msg == L"fwin menu") { activateMenu = true; }
			else if (msg == L"fwin") { addDraw(L"fwin <system name window>(start fwindow)"); }
			else if (msg == L"FGUIcrash1") { crash1b = !crash1b; if (crash1b) addDraw(L"*** SAY BYE ***"); }
			else if (msg == L"FGUIcrash2") { crash2b = !crash2b; if (crash2b) addDraw(L"*** Wait ***"); }
			else if (msg == L"FGUIcrash3") { crash3b = !crash3b; if (crash3b) addDraw(L"*** SAY BYE ***"); }
			else if (msg == L"help") { 
				addDraw(L"______________________________________________");
				addDraw(L"All commands: ");
				std::string buffer;
				for (int a = 0; a < listCommands.size(); a++) {
					buffer = "    - ";
					buffer += listCommands[a];
					addDraw(buffer);
				}
				addDraw(L"");
			}
			else if (msg == L"fwindows") {
				addDraw(L"______________________________________________");
				addDraw(L"All fwindows(fwin <name window>): ");
				std::string buffer;
				for (int a = 0; a < listWindows.size(); a++) {
					buffer = "    - ";
					buffer += listWindows[a];
					addDraw(buffer);
				}
				addDraw(L"");
			}
			else { addDraw(L"Unknown command!"); }
		}

		buffer.clear();
	}
	std::wstring getLineForDraw() {
		if (!linesForDraw.empty()) {
			auto ret = linesForDraw[0];
			linesForDraw.erase(linesForDraw.begin());
			return ret;
		}
		else {
			return L"";
		}
	}
	bool getNeedCls() {
		if (mustCls) { mustCls = false; return true; }
		else return false;
	}
} sc;