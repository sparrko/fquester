#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "fdpReader.h"

namespace FQuester {
	namespace StartLogo {

		bool FileExists(std::string fname) { return access(fname.c_str(), 0) != -1; }

		SDL_Window* window;
		SDL_Renderer* renderer;

		void ShowLogo() {
			if (SDL_Init(SDL_INIT_VIDEO) != 0) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Can't iniitialise SDL", SDL_GetError(), window); exit(0); }
			int* imgFlags = new int; *imgFlags = IMG_INIT_JPG | IMG_INIT_PNG; if (!(IMG_Init(*imgFlags) & *imgFlags)) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Can't iniitialise SDL_image", IMG_GetError(), window); exit(0); } delete imgFlags;
			window = SDL_CreateWindow("FQuester loading...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 268, SDL_WINDOW_BORDERLESS);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

			FDP::Reader fdp;
			if (!FileExists("resources/res0.fdp")) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Can't open resource: res0.fdp", window); exit(0); }
			if (!fdp.open("resources/res0.fdp")) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Can't open resource: res0.fdp", window); exit(0); }

			SDL_RWops* bufrw; SDL_Surface* bufserf; 
			
			bufrw = SDL_RWFromMem(fdp.getRes("frame.png"), fdp.getSize("frame.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
			SDL_Texture* frame = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
			
			struct tm* local;
			time_t t;
			t = time(NULL);
			local = localtime(&t);
			
			SDL_Texture* center;

			if (local->tm_mday == 14 && local->tm_mon + 1 == 2) {
				bufrw = SDL_RWFromMem(fdp.getRes("1402.png"), fdp.getSize("1402.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
				center = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
			}
			else {
				bufrw = SDL_RWFromMem(fdp.getRes("default.png"), fdp.getSize("default.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
				center = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
			}

			fdp.clear();

			SDL_RenderCopy(renderer, center, NULL, NULL);
			SDL_RenderCopy(renderer, frame, NULL, NULL);
			
			SDL_RenderPresent(renderer);

			SDL_DestroyTexture(frame);
			SDL_DestroyTexture(center);
			SDL_DestroyRenderer(renderer);
		}
		void CloseLogo() {
			SDL_DestroyWindow(window);
		}
	}
}