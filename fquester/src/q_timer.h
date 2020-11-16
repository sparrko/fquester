#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "q_globalVars.h"
#include "objects.h"
#include <vector>
#include "settings.h"
#include "res.h"
#include "fgui.h"


float QTIMER_MSIZE = 1.5;
float QTIMER_CSIZE = 0.03;
float QTIMER_MOVE = 10;
FGUI::Vector2u QTIMER_RETREAT = FGUI::Vector2u(25, 0);
float QTIMER_FPS = 80;
float QTIMER_OUTLINE = 7;

int QTIMER_TR_MAX = 220;
int QTIMER_TR_MIN = 130;
float QTIMER_TR_DIV = 3;

namespace FQuester {
	class QTimer {
		private:
			sf::Clock fpst;

			int max;
			std::vector <SDL_Texture*> nums;
			std::vector <SDL_Rect> nums_rects;
			sf::Clock cl2; float move;
			sf::Clock cl; int fake;
			bool mPLayS;
			float size;
			float trans;
			bool mRestartTime;
			bool ignoreSound;
		public:
		QTimer() {
			size = 1;
			max = settings::s_timer + 1;
			mPLayS = false;
			fake = 0;
			trans = QTIMER_TR_MAX;
			mRestartTime = false;
			ignoreSound = false;
		}
		void init() {
			
			max = settings::s_timer + 1;

			if (nums.size() != max) {
				// Clear
				for (int a = 0; a < nums.size(); a++) {
					SDL_DestroyTexture(nums[a]);
				}

				nums.resize(max);
				nums_rects.resize(max);

				// Render
				TTF_SetFontOutline(Res::ttf_timer_out, QTIMER_OUTLINE);
				SDL_Surface* bufserf; 
				SDL_Color white; white.r = 222; white.g = 215; white.b = 143; white.a = 255;
				SDL_Color black; black.r = black.g = black.b = 0; black.a = 255;
				for (int a = 0; a < max; a++) {
					SDL_Surface* bg_surface = TTF_RenderText_Blended(Res::ttf_timer_out, std::to_string((long long)a).c_str(), black);
					SDL_Surface* fg_surface = TTF_RenderText_Blended(Res::ttf_timer, std::to_string((long long)a).c_str(), white);
					SDL_Rect rect; rect.x = QTIMER_OUTLINE;
					rect.y = QTIMER_OUTLINE;
					rect.w = fg_surface->w;
					rect.h = fg_surface->h;

					SDL_SetSurfaceBlendMode(fg_surface, SDL_BLENDMODE_BLEND);
					SDL_BlitSurface(fg_surface, NULL, bg_surface, &rect);
					SDL_FreeSurface(fg_surface);

					nums[a] = SDL_CreateTextureFromSurface(renderer, bg_surface);
					nums_rects[a].w = bg_surface->w;
					nums_rects[a].h = bg_surface->h;
					SDL_FreeSurface(bg_surface);
				}
			}

		}
		bool mustInit() {
			return (nums.size() != settings::s_timer + 1);
		}
		void start() {
			if (settings::s_timer != 0) {
				cl.restart();
				mPLayS = true;
				fake = 0;
				size = QTIMER_MSIZE;
				move = 0;
				trans = QTIMER_TR_MAX;
				if (settings::s_bticks) { if (nums.size() % 2 != 0) Res::aud_tick.play(); else Res::aud_tack.play(); }
			}
		}
		void startLong() {
			if (settings::s_timer != 0) {
				cl.restart();
				mPLayS = true;
				fake = 0;
				size = QTIMER_MSIZE;
				move = 0;
				trans = QTIMER_TR_MAX;
				mRestartTime = true;
			}
		}
		void end() {
			fake = 99;
			mPLayS = false;
			move = 301;
			ignoreSound = true;
		}
		void display() {
			if (settings::s_timer != 0 && (!fullIgnoreUpdateBg && !ignoreUpdateBg)) {
				if (mRestartTime) { mRestartTime = false; cl.restart(); if (settings::s_bticks) { if (nums.size() % 2 != 0) Res::aud_tick.play(); else Res::aud_tack.play(); } }
				if (cl.getElapsedTime().asSeconds() >= 1) {
					cl.restart();
					fake++;
					if (!(fake > nums.size() - 1)) {
						size = QTIMER_MSIZE;
						trans = QTIMER_TR_MAX;
						if (settings::s_bticks) { if (fake % 2 != 0) Res::aud_tick.play(); else Res::aud_tack.play(); }
					}
				}
				if (fpst.getElapsedTime().asMilliseconds() > 1000. / QTIMER_FPS || settings::s_vsync) {
					fpst.restart();
					size -= QTIMER_CSIZE;
					if (size < 1.) size = 1.;
					if ((fake > nums.size() - 1) && cl2.getElapsedTime().asSeconds() > 1.5) {
						move += QTIMER_MOVE;
						if (move > 301) move = 301;
					}

					trans -= QTIMER_TR_DIV;
					if (trans < QTIMER_TR_MIN) trans = QTIMER_TR_MIN;
				}
				

			
				int z = fake;
				if (z > nums.size() - 1) z = nums.size() - 1;

				SDL_Rect dst;
				dst.w = (nums_rects[nums.size() - z - 1].w * size) * FGUI::sizeDisplay.x / 1920.;
				dst.h = (nums_rects[nums.size() - z - 1].h * size) * FGUI::sizeDisplay.y / 1080.;
				dst.x = QTIMER_RETREAT.x * (FGUI::sizeDisplay.x / 1920.) - (nums_rects[nums.size() - z - 1].w * size - nums_rects[nums.size() - z - 1].w) / 2. * (FGUI::sizeDisplay.x / 1920.) - move * (FGUI::sizeDisplay.x / 1920.);
				dst.y = QTIMER_RETREAT.y * (FGUI::sizeDisplay.y / 1080.) - (nums_rects[nums.size() - z - 1].h * size - nums_rects[nums.size() - z - 1].h) / 2. * (FGUI::sizeDisplay.y / 1080.);
				
				if (move < 300) {
					SDL_SetTextureAlphaMod(nums[nums.size() - z - 1], trans);
					SDL_RenderCopy(renderer, nums[nums.size() - z - 1], NULL, &dst);
				}
				if (!ignoreSound) {
					if (mPLayS && (nums.size() - z - 1) == 0) {
						mPLayS = false;
						Res::aud_time.play();
						cl2.restart();
					}
				}
				else {
					ignoreSound = !ignoreSound;
				}
			}
		}
	} qTimer;
}