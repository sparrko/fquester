#pragma once

#include "fgui_www.h"
#include <limits>
#include "fgui_cursor.h"
#include "fgui_info.h"
#include "fgui_cursorTextBox.h"

namespace FGUI{
	bool GUIActive() {
		for (int a = 0; a < numWindows; a++) {
			if(ExistsWindow[a]->getShowed()) return true;
		}
		return false;
	}
	// -------------------------------------- //
	// Initialise
	// // Initialise system with SDL
	#ifdef FGUI_RENDER_SDL
	bool Init(SDL_Window* window, SDL_Renderer* renderer, SDL_Event* event) {
		p_window = window;
		p_renderer = renderer;
		p_event = event;
		
		SDL_DisplayMode displayMode; SDL_GetWindowDisplayMode(p_window, &displayMode);
		sizeDisplay.x = displayMode.w; sizeDisplay.y = displayMode.h;
		t_display = SDL_CreateTexture(p_renderer, NULL, SDL_TEXTUREACCESS_TARGET, sizeDisplay.x, sizeDisplay.y); if (!t_display) { return false; }
		else SDL_SetTextureBlendMode(t_display, SDL_BLENDMODE_BLEND);

		FGUI::Render::Init();
		FGUI::Res::Init();
		
		cursorText.init();

		/////////////////////////////////////////////////////////

		return true;
	}
	void NewSizeWindow() {
		SDL_DisplayMode displayMode; SDL_GetWindowDisplayMode(p_window, &displayMode);
		sizeDisplay.x = displayMode.w; sizeDisplay.y = displayMode.h;
		SDL_DestroyTexture(t_display);
		t_display = SDL_CreateTexture(p_renderer, NULL, SDL_TEXTUREACCESS_TARGET, sizeDisplay.x, sizeDisplay.y);
		SDL_SetTextureBlendMode(t_display, SDL_BLENDMODE_BLEND);

		for (int a = 0; a < numWindows; a++) {
			ExistsWindow[a]->fullUpdateRender();
		}
	}
	#endif
	// // Initialise system with OpenGL
	#ifdef FGUI_RENDER_OPENGL
	bool Init() { return true; }
	#endif
	// -Init--------------------------------- //


	// -------------------------------------- //
	// Update logic window
	void Update() {
		// Update iduList
		iduList.resize(numWindows);
		for (int a = 0; a < numWindows; a++) {
			for (int b = 0; b < numWindows; b++) {
				if (WindowsList[b]->getIdu() == a) {
					iduList[a] = WindowsList[b];
				}
			}
		}
		// Select window
		for (int a = 0; a < callUpdateIdu.size(); a++) {
			for (int b = *callUpdateIdu[a] - 1; b > -1; b--) {
				iduList[b]->setIdu(iduList[b]->getIdu() + 1);
			}
			*callUpdateIdu[a] = 0;

			if (a == 0) Elements::p_ac_textBox = NULL;
		}
		callUpdateIdu.clear();
		// Update iduList
		iduList.resize(numWindows);
		for (int a = 0; a < numWindows; a++) {
			for (int b = 0; b < numWindows; b++) {
				if (WindowsList[b]->getIdu() == a) {
					iduList[a] = WindowsList[b];
				}
			}
		}
		// Hide window
		for (int a = 0; a < callHideIdu.size(); a++) {
			for (int b = *callHideIdu[a] + 1; b < getCountsWindow(); b++) {
				iduList[b]->setIdu(iduList[b]->getIdu() - 1);
			}
			*callHideIdu[a] = FGUI::getCountsWindow() - 1;
		}
		callHideIdu.clear();
		// Delete window
		for (int a = 0; a < numWindows; a++) {
			if (iduList[a]->mustDelete()) {
				DeleteWindow(iduList[a]);
				for (int b = a; b < numWindows - 1; b++) {
					iduList[b] = iduList[b + 1];
				}
				iduList.resize(iduList.size() - 1);
				break;
			}
		}
		// Update windows
		checkBoxFinded = false; centerFinded = false; windowCheckFinded = false; 
		
		if (iduList.size() > 0) { if (iduList[0]->getLife()) { iduList[0]->check(); iduList[0]->update(); } }
		if (!windowCheckFinded) {
			for (int a = 1; a < numWindows; a++) {
				if (iduList[a]->check()) {
					iduList[a]->update();
					windowCheckFinded = true;
				}
			}
		}
		// Cancel move mouse whell
		if (p_event->type == 1027) p_event->type = 1024; // replacement to mouse move
	}
	// -Update------------------------------- //


	// -------------------------------------- //
	// Check events from gui
	void CheckEvents() {
		// Check events for update render if was minimized
		if (p_event->type == SDL_WINDOWEVENT && p_event->window.event == SDL_WINDOWEVENT_MINIMIZED) {
			callCheckInputFocus = true;
		}
		if (callCheckInputFocus && p_event->type == SDL_WINDOWEVENT && p_event->window.event == SDL_WINDOWEVENT_ENTER) {
			callCheckInputFocus = false;
			callWaitFirstRender = true;
		}
		// Close only 1 window
		if (p_event->type == SDL_MOUSEBUTTONUP && p_event->button.button == SDL_BUTTON_LEFT) {
			windowWasPressedClose = false;
		}
		// Check input text
		inputText.clear();
		if (FGUI::p_event->type == SDL_KEYDOWN && p_event->key.keysym.sym == SDLK_BACKSPACE && SDL_GetModState() & KMOD_CTRL) { inputText = L"$/cdel"; }
		else if (FGUI::p_event->type == SDL_KEYDOWN && p_event->key.keysym.sym == SDLK_BACKSPACE) { inputText = L"$/del"; }
		else if (FGUI::p_event->type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_RETURN) { inputText = L"$/enter"; }
		else if (FGUI::p_event->type == SDL_KEYDOWN && FGUI::p_event->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
			wchar_t* text = NULL;
			if (OpenClipboard(0))
			{
				HANDLE hData = GetClipboardData(CF_UNICODETEXT);
				text = (wchar_t*)GlobalLock(hData);
				GlobalUnlock(hData);
				CloseClipboard();
			}

			inputText += text;
		}
		else if (p_event->type == SDL_TEXTINPUT) {
			auto bufcheck = p_event->text.text;
			if (strlen(bufcheck) == 1) {
				inputText += (wchar_t)bufcheck[0];
			}
			else if (strlen(bufcheck) == 2) {
				if (bufcheck[0] == -48) {
					inputText += (wchar_t)(1152 + bufcheck[1]);
				}
				else if (bufcheck[0] == -47) {
					inputText += (wchar_t)(1216 + bufcheck[1]);
				}
			}
		}
		// Cursor
		cursor.checkEvent();
	}
	// -CheckEvents-------------------------- //


	// -------------------------------------- //
	// Render and display
	void Display() {
		if (callUpdateRender) {
			FGUI::NewSizeWindow();
			callUpdateRender = false;
		}

		FGUI::Render::Clear();

		for (int a = numWindows - 1; a > -1; a--) {
			for (int b = 0; b < numWindows; b++) {
				if (WindowsList[b]->getIdu() == a) {
					WindowsList[b]->display();
					break;
				}
			}
		}

		cursorText.display();
		cursor.display();

		FGUI::Render::PresentFGUI();

		if (callWaitFirstRender) {
			callWaitFirstRender = false;
			callUpdateRender = true;
		}
	}
	// -Display------------------------------ //


	// -------------------------------------- //
	// Clear
	void Quit() {
		clearUnits();
		Update();
		Res::Clear();
	}
	// -Quit--------------------------------- //
}