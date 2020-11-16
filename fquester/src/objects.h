#pragma once

#include <SDL.h>
#include "settings.h"
#include "fgui_system.h"
#include "fgui.h"

SDL_Texture* t_n[12]; int t_n_frame; sf::Clock t_n_time; bool n_inited;

bool isRunning;
int DesktopX, DesktopY;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event event;

sf::Clock random_srand;

bool filter = false;
float mouseWhellSpeed = 40;

bool devMode = false; 

bool changeFullscreenCheckBox = false;

bool CreateWindowAndRenderer(FGUI::Vector2u size, bool fullscreen) {
	bool ret = false;

	int WinX, WinY; SDL_GetWindowSize(window, &WinX, &WinY);
	if (size.x != WinX || size.y != WinY) {
		SDL_SetWindowSize(window, size.x, size.y);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		ret = true;
		SDL_RenderPresent(renderer);
	}

	if ((SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) && fullscreen == false) {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN); ret = true; SDL_RenderPresent(renderer);
	}
	else if (!(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN) && fullscreen == true) {
		if (fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); ret = true; SDL_RenderPresent(renderer);
	}

	return ret;
}

void ChangeFullscreen() {
	settings::s_fullscreen = !settings::s_fullscreen;
	settings::save();
	settings::read();
	changeFullscreenCheckBox = true;
	CreateWindowAndRenderer(settings::getWindowSize(), settings::s_fullscreen);
	FGUI::NewSizeWindow();
}

bool CheckFullscreen() {
	if ((SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)) {
		return true;
	}
	if (!(SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN)) {
		return false;
	}
}