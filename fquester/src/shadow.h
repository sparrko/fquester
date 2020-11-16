#pragma once

#include "fgui_timer.h"
#include "objects.h"
#include <SDL.h>
#include "settings.h"

#define DIR_SHAD 14.
#define LONGDIR_SHAD 5.

namespace FQuester {
	struct Shadow {
		FGUI::Timer tt;
		float dir;
		float shadowPer;
		bool move;
		Shadow() {
			dir = -1;
			shadowPer = 0;
			move = false;
		}
		void set(float shadow, float dr) {
			shadowPer = shadow;
			dir = dr;
		}
		bool getMove() {
			return move;
		}
		void changeDir() {
			if (dir < 0) dir = DIR_SHAD;
			else if (dir > 0) dir = -(DIR_SHAD);
			move = true;
		}
		void display() {
			if ((!settings::s_vsync && tt.getMilliseconds() > (float)1000. / 80) || settings::s_vsync) {
				shadowPer += dir;
				if (shadowPer > 255.) { shadowPer = 255.; move = false; }
				else if (shadowPer < 0.) { shadowPer = 0.; move = false; }
				tt.restart();
			}
			if (shadowPer > 0) {
				SDL_SetRenderTarget(renderer, NULL);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, (int)shadowPer);
				SDL_RenderFillRect(renderer, NULL);
			}
		}
	} shadow;
}