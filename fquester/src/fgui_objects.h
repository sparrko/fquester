#pragma once

#include "fgui_system.h"
#include "fgui_timer.h"

#ifndef FGUI_RENDER_SDL && FGUI_RENDER_OPENGL
	#define FGUI_RENDER_SDL
#endif

#ifdef FGUI_RENDER_SDL
namespace FGUI {
	SDL_Window* p_window = nullptr; FGUI::Vector2u sizeDisplay;
	SDL_Renderer* p_renderer = nullptr;
	SDL_Event* p_event = nullptr; 
	
	SDL_Texture* t_display = nullptr;

	std::wstring inputText = L"";

	bool centerFinded = false;
	bool windowCheckFinded = false;
	bool checkBoxFinded = false;
	bool windowWasPressedClose = false;

	bool callCheckInputFocus = false;
	bool callUpdateRender = false;
	bool callWaitFirstRender = false;
}
#endif