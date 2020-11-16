#pragma once

#include <SDL.h>

int GetWidthTexture(SDL_Texture* texture) {
	int size;
	SDL_QueryTexture(texture, NULL, NULL, &size, NULL);
	return size;
}
int GetWidthTexture(SDL_Surface* serf, SDL_Renderer* renderer) {
	auto t = SDL_CreateTextureFromSurface(renderer, serf);
	int size = GetWidthTexture(t);
	SDL_DestroyTexture(t);
	return size;
}