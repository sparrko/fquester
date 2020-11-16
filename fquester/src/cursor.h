#pragma once

#include "fgui_system.h"
#include <SDL.h>
#include "objects.h"
#include "res.h"
#include "q_globalVars.h"

const int fquester_cur_sizeX = 40;
const int fquester_cur_sizeY = 40;

namespace FQuester{
	class Cursor{
		sf::Clock cl;
		sf::Clock cl2;
		int trans;
		bool pressed;

		bool video;
		FGUI::Vector2i position;
		SDL_Texture* tcur; 
		SDL_Rect dst;
		public:
		Cursor() {
			pressed = false;
			video = false; trans = 255;
			dst.x = dst.y = 0;
			dst.w = fquester_cur_sizeX;
			dst.h = fquester_cur_sizeY;
			SDL_SetTextureBlendMode(Res::txtr_cursor, SDL_BLENDMODE_BLEND);
		}
		void checkEvents() {
			if (event.type == SDL_MOUSEMOTION) { 
				position.x = event.motion.x; 
				position.y = event.motion.y; 
				cl.restart();
				trans = 255;
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				cl.restart();
				trans = 255;
				
			}

			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) pressed = false;
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) pressed = true;
		}
		void display() {
			if (video || fullIgnoreUpdateBg) {
				if (cl.getElapsedTime().asMilliseconds() > 1000) {
					if (cl2.getElapsedTime().asMilliseconds() > 1000. / 80. && trans > 0) {
						cl2.restart();
						trans-=10;
					}
				}
			}
			else {
				trans = 255;
			}
			dst.x = position.x; dst.y = position.y;
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			if (trans < 0) trans = 0;
			SDL_SetTextureAlphaMod(Res::txtr_cursor, trans);
			// if (!pressed) SDL_SetTextureColorMod(Res::txtr_cursor, 220, 220, 220);
			// else SDL_SetTextureColorMod(Res::txtr_cursor, 255, 255, 255);
			SDL_RenderCopy(renderer, Res::txtr_cursor, NULL, &dst);
		}
		FGUI::Vector2i getPosition() {
			return position;
		}
		void videoMode(bool mode) {
			video = mode;
		}
		bool getVideoMode() {
			return video;
		}
	} cursor;
}