#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "SystemConsole.h"

// Uni-method
bool saveScreenshot(const std::string& file, SDL_Renderer* renderer) {
	SDL_Rect _viewport;
	SDL_Surface* _surface = NULL;
	SDL_RenderGetViewport(renderer, &_viewport);
	_surface = SDL_CreateRGBSurface(0, _viewport.w, _viewport.h, 32, 0, 0, 0, 0);
	if (_surface == NULL) {
		std::string msg = "Cannot create SDL_Surface: "; msg += SDL_GetError();
		sc.addDraw(msg.c_str());
		return false;
	}
	if (SDL_RenderReadPixels(renderer, NULL, _surface->format->format, _surface->pixels, _surface->pitch) != 0) {
		std::string msg = "Cannot read data from SDL_Renderer: "; msg += SDL_GetError();
		sc.addDraw(msg.c_str());
		SDL_FreeSurface(_surface);
		return false;
	}
	if (IMG_SavePNG(_surface, file.c_str()) != 0) {
		std::string msg = "Cannot save PNG file: "; msg += SDL_GetError();
		sc.addDraw(msg.c_str());
		SDL_FreeSurface(_surface);
		return false;
	}
	SDL_FreeSurface(_surface);
	return true;
}