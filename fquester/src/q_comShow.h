#pragma once

#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"
#include "QAL_Reader.h"
#include "q_globalVars.h"

int COMSHOW_TPOS1 = 0;
int COMSHOW_TPOS2 = 111;
int COMSHOW_TPOS3 = 221;

int COMSHOW_WIDTH = 110;
int COMSHOW_HEIGHT = 188;

int COMSHOW_PERPOSY = 15;

int COMSHOW_SHOWTIME = 3000;
int COMSHOW_DIV = 5;
int COMSHOW_FPS = 80;

namespace FQuester {
	class ComShow {
		SDL_Rect src, dst;
		sf::Clock c_fps;
		FGUI::Timer t;
		float left;
		uint8_t ncmd;
	public:
		// //////////////////////////////
		void init() {
			t.setMilliseconds(COMSHOW_SHOWTIME);
			ncmd = 100;
			left = 0;
		}
		void push(uint8_t cmd) {
			t.restart();
			ncmd = cmd;
		}
		void display() {
			if (c_fps.getElapsedTime().asMilliseconds() > 1000. / COMSHOW_FPS) {
				c_fps.restart();

				if (t.getMilliseconds() < COMSHOW_SHOWTIME) left += COMSHOW_DIV;
				else left -= COMSHOW_DIV;

				if (left < 0) left = 0;
				else if (left > COMSHOW_WIDTH) left = COMSHOW_WIDTH;

				if (left != 0) {
					if (ncmd == 0) { src.x = COMSHOW_TPOS3; } else if (ncmd == 1) { src.x = COMSHOW_TPOS1; } else if (ncmd == 2) { src.x = COMSHOW_TPOS2; }
					src.y = 0;
					src.w = COMSHOW_WIDTH;
					src.h = COMSHOW_HEIGHT;

					dst.x = FGUI::sizeDisplay.x - FGUI::sizeDisplay.x * left / 1920.;
					dst.y = FGUI::sizeDisplay.y / 100. * COMSHOW_PERPOSY;
					dst.w = src.w * FGUI::sizeDisplay.x / 1920.;
					dst.h = src.h * FGUI::sizeDisplay.y / 1080.;
				}
			}
			if (left != 0) {
				SDL_RenderCopy(renderer, Res::txtr_g_scmd, &src, &dst);
			}
		}

	} comShow;
}