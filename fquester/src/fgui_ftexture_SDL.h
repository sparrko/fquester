#pragma once

#define FGUI_RENDER_SDL

#include <SDL.h>
#include "fgui_objects.h"

namespace FGUI {
	namespace Render {
		//////////////////////////////////////////////////////////////
		// res
		struct FTexture {
			SDL_Texture* texture;
			FGUI::Vector2i size;
			FTexture() { texture = nullptr; }
			bool exists() {
				if (texture) return true;
				else return false;
			}
			bool init(SDL_Surface* surface) {
				if (texture) SDL_DestroyTexture(texture);
				texture = SDL_CreateTextureFromSurface(FGUI::p_renderer, surface);
				if (!texture) return false;
				size.x = surface->w; size.y = surface->h;
				return true;
			}
			void clear() {
				if (texture) SDL_DestroyTexture(texture);
			}
		};
	}
}