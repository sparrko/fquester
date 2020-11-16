#pragma once

#include "objects.h"
#include "readPack.h"
#include "fgui.h"
#include "audioUnit.h"
#include "infoDev.h"

#define FPS_SPEED 45.
#define EVT_SPEED 140.
#define GL_DELAY (1000 / EVT_SPEED)

namespace FQuester {
	bool loadingRes;
	void showLoadingBG() {
		SDL_RendererInfo info = { 0 };
		SDL_GetRendererInfo(renderer, &info);
		
		if (std::string(info.name).substr(0, 6) != "opengl") {
			SDL_ShowCursor(true);
			loadingRes = true;
			int num = 0;
			uint32_t mTime = 0;
			uint32_t mTime2 = 0;
			SDL_Rect dst;
			dst.w = (float)FGUI::sizeDisplay.x / 100 * (8);
			dst.h = dst.w;
			dst.x = (float)FGUI::sizeDisplay.x / 100 * 85;
			dst.y = (float)FGUI::sizeDisplay.y / 100 * 78;
			SDL_RenderClear(renderer); SDL_RenderCopy(renderer, load_anim[0], NULL, &dst);
			while (loadingRes) {
				audengine.update();
				if (SDL_GetTicks() > mTime + (1000.f / FPS_SPEED)) {
					mTime = SDL_GetTicks();
					num++; if (num > 61) num = 0;

					infoDev.display();
					SDL_RenderCopy(renderer, load_anim[num], NULL, &dst);
					SDL_RenderPresent(renderer);
				}

				else if (SDL_GetTicks() > mTime2 + (1000.f / EVT_SPEED)) {
					mTime2 = SDL_GetTicks();
					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_F10) { exit(0); }
					}
				}
				else {
					SDL_Delay((int)GL_DELAY);
				}
			}
			SDL_ShowCursor(false);
		}
		else {
			SDL_ShowCursor(true);
			loadingRes = true;
			uint32_t mTime2 = 0;
			while (loadingRes) {
				audengine.update();

				if (SDL_GetTicks() > mTime2 + (1000.f / EVT_SPEED)) {
					mTime2 = SDL_GetTicks();
					while (SDL_PollEvent(&event)) {
						if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_F10) { exit(0); }
					}
				}
				else {
					SDL_Delay((int)GL_DELAY);
				}
			}
			SDL_ShowCursor(false);
		}
	}
	void stopLoadingBG() {
		loadingRes = false;
	}
}