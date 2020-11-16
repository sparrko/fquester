#pragma once


#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"

uint16_t CMDPOINTS_RETREATBORDER = 10;
uint16_t CMDPOINTS_RADSTART = 50;

float locY;

namespace FQuester {
	class CmdPoints{
		SDL_Rect startCheck;
		SDL_Rect dstBg;
		SDL_Rect dstFrame;
		SDL_Rect dstSelect;
		uint8_t select;
		bool show;
		int sendc; long long points;
		bool wasPressed;
	public:
		void setClick(FGUI::Vector2i pos, int _sendc, long long _points) {
			dstSelect.x = dstSelect.y = dstSelect.w = dstSelect.h = 0;

			if (pos.x > FGUI::sizeDisplay.x - dstFrame.w) {
				startCheck.w = CMDPOINTS_RADSTART * 2; startCheck.h = CMDPOINTS_RADSTART * 2;
				startCheck.x = pos.x - CMDPOINTS_RADSTART; startCheck.y = pos.y - CMDPOINTS_RADSTART;
				
				dstFrame.x = pos.x - dstFrame.w; dstFrame.y = pos.y - dstFrame.h;
				dstBg.x = pos.x - (CMDPOINTS_RETREATBORDER * FGUI::sizeDisplay.x) / 1920 - dstBg.w; dstBg.y = pos.y - dstBg.h - (CMDPOINTS_RETREATBORDER * FGUI::sizeDisplay.y) / 1080;
			}
			else {
				startCheck.x = pos.x - CMDPOINTS_RADSTART; startCheck.y = pos.y - CMDPOINTS_RADSTART;
				startCheck.w = CMDPOINTS_RADSTART * 2; startCheck.h = CMDPOINTS_RADSTART * 2;

				dstFrame.x = pos.x; dstFrame.y = pos.y - dstFrame.h;
				dstBg.x = pos.x + (CMDPOINTS_RETREATBORDER * FGUI::sizeDisplay.x) / 1920; dstBg.y = pos.y - dstBg.h - (CMDPOINTS_RETREATBORDER * FGUI::sizeDisplay.y) / 1080;
			}
			show = true;

			sendc = _sendc;
			points = _points;
		}
		/////////////////////////
		void init() {
			sendc = 3;
			select = 0;
			show = false;
			wasPressed = false;

			dstFrame.w = (float)(FGUI::sizeDisplay.y * 655) / 1080;
			dstFrame.h = (float)(FGUI::sizeDisplay.y * 400) / 1080;
			dstBg.w = (float)(FGUI::sizeDisplay.y * 639) / 1080;
			dstBg.h = (float)(FGUI::sizeDisplay.y * 383) / 1080;
		}
		void checkEvents() {
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) { wasPressed = true; }
		}
		bool display() {
			if (show) {
				// Update
				ignorPressForPointsBar = true;
				if (!CheckRect(dstFrame, cursor.getPosition()) && !CheckRect(startCheck, cursor.getPosition())) show = false;
				else {
					SDL_Rect checkrect;
					for (int i = 0; i < 5; i++) {
						checkrect.x = dstFrame.x;
						checkrect.h = dstFrame.h / 5.f;
						checkrect.y = dstFrame.y + checkrect.h * i;
						checkrect.w = dstFrame.w;

						if (CheckRect(checkrect, cursor.getPosition())) {
							if			(i == 0) { dstSelect = checkrect; if (wasPressed) { mAddPoints = true; cForPoints = sendc; newPoints = points; show = false; } }
							else if		(i == 1) { dstSelect = checkrect; if (wasPressed) { mAddPoints = true; cForPoints = sendc; newPoints = points / 2.f; show = false; } }
							else if		(i == 2) { dstSelect.x = dstSelect.y = dstSelect.w = dstSelect.h = 0; }
							else if		(i == 3) { dstSelect = checkrect; if (wasPressed) { mAddPoints = true; cForPoints = sendc; newPoints = -points; show = false; fiasko = true; } }
							else if		(i == 4) { dstSelect = checkrect; if (wasPressed) { mAddPoints = true; cForPoints = sendc; newPoints = -points / 2.f; show = false; fiasko = true; } }
							break;
						}
					}
				}
				if (locY == 0) show = false;
			
				// Render
				dstFrame.w = (float)(FGUI::sizeDisplay.y * 655) / 1080;
				dstFrame.h = (float)(FGUI::sizeDisplay.y * 400) / 1080;
				dstBg.w = (float)(FGUI::sizeDisplay.y * 639) / 1080;
				dstBg.h = (float)(FGUI::sizeDisplay.y * 383) / 1080;

				if (locY != 0) SDL_RenderCopy(renderer, Res::txtr_g_cmdPointsBg, NULL, &dstBg);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 120);
				SDL_RenderFillRect(renderer, &dstSelect);
				if (locY != 0) SDL_RenderCopy(renderer, Res::txtr_g_cmdPointsFrame, NULL, &dstFrame);
			}
			wasPressed = false;

			// ret
			if (show) return true; else return false;
		}
	} cmdPoints;
}