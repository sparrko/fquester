#pragma once

#include "res.h"
#include "fgui.h"


void RenderLoadBG(std::wstring message) {

	SDL_SetRenderDrawBlendMode(FGUI::p_renderer, SDL_BLENDMODE_BLEND);

	//

	SDL_SetRenderDrawColor(FGUI::p_renderer, 200, 200, 200, 100);
	SDL_RenderFillRect(FGUI::p_renderer, NULL);

	//

	SDL_Rect rectplp;
	SDL_SetRenderDrawColor(FGUI::p_renderer, 200, 200, 200, 255);

	rectplp.x = -1;
	rectplp.w = FGUI::sizeDisplay.x + 2;
	rectplp.h = 70;
	rectplp.y = FGUI::sizeDisplay.y / 2 - rectplp.h / 2;

	SDL_RenderFillRect(FGUI::p_renderer, &rectplp);

	SDL_SetRenderDrawColor(FGUI::p_renderer, 0, 0, 0, 255);
	SDL_RenderDrawRect(FGUI::p_renderer, &rectplp);

	//

	SDL_Color black; 
	black.r = 0; black.g = 0; black.b = 0; black.a = 255;
	SDL_Rect recttxt;

	SDL_Surface* bg_surface = TTF_RenderUNICODE_Blended(FQuester::Res::ttf_loadBg, (Uint16*)message.c_str(), black);

	SDL_Texture* t = SDL_CreateTextureFromSurface(FGUI::p_renderer, bg_surface);
	recttxt.w = bg_surface->w;
	recttxt.h = bg_surface->h;
	recttxt.x = FGUI::sizeDisplay.x / 2 - bg_surface->w / 2;
	recttxt.y = FGUI::sizeDisplay.y / 2 - bg_surface->h / 2;
	SDL_FreeSurface(bg_surface);
	
	SDL_RenderCopy(FGUI::p_renderer, t, NULL, &recttxt);
	SDL_DestroyTexture(t);

	//

	for (int i = 0; i < 5; i++) {
		SDL_PollEvent(NULL);
		SDL_RenderPresent(FGUI::p_renderer);
	}
}