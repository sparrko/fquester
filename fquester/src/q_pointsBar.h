#pragma once

#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"
#include "q_cmdPoints.h"
#include "settings.h"

float POINTSBAR_FPS = 80;
float POINTSBAR_DIV = 11;
float POINTSBAR_HEIGHT = 188;
float POINTSBAR_TIMEWAIT = 1000;



namespace FQuester {
	enum CMD {
		RED = 0, GREEN = 1, BLUE = 2
	};
	class PointsBar {
		SDL_Color c_points;
		bool onFloatDown;
		bool onFloatBar;
		sf::Clock c_fps;
		sf::Clock c_outBar;
		
		bool show;
		SDL_Rect dstfloatBar;

		SDL_Rect srcr_points[3];
		SDL_Texture* t_points[3];

		uint8_t posBar; SDL_Rect dstSelectBar;

		bool callDontUpdateOther;
	public:
		long long getPoints(uint8_t _cmd) {
			if (_cmd == CMD::RED) return points[0];
			else if (_cmd == CMD::GREEN) return points[1];
			else if (_cmd == CMD::BLUE) return points[2];
		}
		void setPoints(uint8_t _cmd, long long _points) {
			auto bufserf = TTF_RenderText_Blended(Res::ttf_points, std::to_string(_points).c_str(), c_points);

			if (_cmd == CMD::RED) { 
				srcr_points[0].w = bufserf->w; srcr_points[0].h = bufserf->h;
				if (t_points[0]) SDL_DestroyTexture(t_points[0]);
				points[0] = _points;
				t_points[0] = SDL_CreateTextureFromSurface(renderer, bufserf);
			}
			else if (_cmd == CMD::GREEN) {
				srcr_points[1].w = bufserf->w; srcr_points[1].h = bufserf->h;
				if (t_points[1]) SDL_DestroyTexture(t_points[1]);
				points[1] = _points;
				t_points[1] = SDL_CreateTextureFromSurface(renderer, bufserf);
			}
			else if (_cmd == CMD::BLUE) {
				srcr_points[2].w = bufserf->w; srcr_points[2].h = bufserf->h;
				if (t_points[2]) SDL_DestroyTexture(t_points[2]);
				points[2] = _points;
				t_points[2] = SDL_CreateTextureFromSurface(renderer, bufserf);
			}

			SDL_FreeSurface(bufserf);
		}
		void updatePointsTextures() {
			auto bufserf = TTF_RenderText_Blended(Res::ttf_points, std::to_string(points[0]).c_str(), c_points);

			srcr_points[0].w = bufserf->w; srcr_points[0].h = bufserf->h;
			if (t_points[0]) SDL_DestroyTexture(t_points[0]);
			t_points[0] = SDL_CreateTextureFromSurface(renderer, bufserf);

			SDL_FreeSurface(bufserf);

			bufserf = TTF_RenderText_Blended(Res::ttf_points, std::to_string(points[1]).c_str(), c_points);

			srcr_points[1].w = bufserf->w; srcr_points[1].h = bufserf->h;
			if (t_points[1]) SDL_DestroyTexture(t_points[1]);
			t_points[1] = SDL_CreateTextureFromSurface(renderer, bufserf);

			SDL_FreeSurface(bufserf);

			bufserf = TTF_RenderText_Blended(Res::ttf_points, std::to_string(points[2]).c_str(), c_points);

			srcr_points[2].w = bufserf->w; srcr_points[2].h = bufserf->h;
			if (t_points[2]) SDL_DestroyTexture(t_points[2]);
			t_points[2] = SDL_CreateTextureFromSurface(renderer, bufserf);

			SDL_FreeSurface(bufserf);
		}
		void addPoints(uint8_t _cmd, long long _points) {
			if (_cmd == CMD::RED) { setPoints(CMD::RED, getPoints(CMD::RED) + _points); }
			else if (_cmd == CMD::GREEN) { setPoints(CMD::GREEN, getPoints(CMD::GREEN) + _points); }
			else if (_cmd == CMD::BLUE) { setPoints(CMD::BLUE, getPoints(CMD::BLUE) + _points); }
		}
		/////////////////////
		void init() {
			callDontUpdateOther = false;
			posBar = 3;
			srcr_points[0].x = srcr_points[0].y = srcr_points[1].x = srcr_points[1].y = srcr_points[2].x = srcr_points[2].y = 0;
			c_points.r = 222; c_points.g = 215; c_points.b = 143; c_points.a = 255;
			setPoints(CMD::RED, 0); setPoints(CMD::GREEN, 0); setPoints(CMD::BLUE, 0);
			plusPoints = 0L;

			show = false;
			onFloatDown = onFloatBar = false;
			locY = 0;
			
			cmdPoints.init();
		}
		void checkEvents() {
			if (onFloatDown && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) { show = !show; }
			else if (!onFloatBar && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) { if(!ignorPressForPointsBar) show = false; }
			else if (onFloatBar && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
				cmdPoints.setClick(cursor.getPosition(), posBar, plusPoints);
			}
			cmdPoints.checkEvents();
		}
		bool checkCol() {
			if (onFloatDown || (onFloatBar && show) || callDontUpdateOther) return true;
			else return false;
		}
		void display() {
			// Update
			if (mChangePoints) {
				mChangePoints = false;
				setPoints(cForPoints, newPoints);
			}
			if (mAddPoints) {
				mAddPoints = false;
				addPoints(cForPoints, newPoints);
			}
			if (updatePoints) {
				updatePoints = false;
				updatePointsTextures();
			}

			if ((!settings::s_vsync && c_fps.getElapsedTime().asMilliseconds() > (float)1000. / POINTSBAR_FPS) || settings::s_vsync) {
				c_fps.restart();

				SDL_Rect dstfloatButton;
				dstfloatButton.w = (float)(FGUI::sizeDisplay.y * 97) / 1080; dstfloatButton.h = (float)(FGUI::sizeDisplay.y * 42) / 1080;
				dstfloatButton.x = FGUI::sizeDisplay.x / 2 - dstfloatButton.w / 2; dstfloatButton.y = FGUI::sizeDisplay.y - dstfloatButton.h;
				if (CheckRect(dstfloatButton, cursor.getPosition())) onFloatDown = true; else onFloatDown = false;

				if (show) { locY += POINTSBAR_DIV; if (locY > POINTSBAR_HEIGHT) locY = POINTSBAR_HEIGHT; }
				else { locY -= POINTSBAR_DIV; if (locY < 0) locY = 0; }

				
				dstfloatBar.w = (float)(FGUI::sizeDisplay.x * 1920) / 1920; dstfloatBar.h = (float)(FGUI::sizeDisplay.y * 188) / 1080;
				dstfloatBar.x = FGUI::sizeDisplay.x / 2 - dstfloatBar.w / 2; dstfloatBar.y = FGUI::sizeDisplay.y - dstfloatBar.h;
				if (!CheckRect(dstfloatBar, cursor.getPosition())) { if (onFloatBar) { onFloatBar = false; c_outBar.restart(); if (!ignorPressForPointsBar) posBar = 3; } }
				else {
					if (!ignorPressForPointsBar) {
						if (show) {
							dstfloatBar.w = (float)(FGUI::sizeDisplay.x * (1920 / 3.)) / 1920; dstfloatBar.h = (float)(FGUI::sizeDisplay.y * 188) / 1080;
							dstfloatBar.x = 0; dstfloatBar.y = FGUI::sizeDisplay.y - dstfloatBar.h;
							if (CheckRect(dstfloatBar, cursor.getPosition())) { posBar = 0; dstSelectBar = dstfloatBar; }
							dstfloatBar.w = (float)(FGUI::sizeDisplay.x * (1920 / 3.)) / 1920; dstfloatBar.h = (float)(FGUI::sizeDisplay.y * 188) / 1080;
							dstfloatBar.x = dstfloatBar.w; dstfloatBar.y = FGUI::sizeDisplay.y - dstfloatBar.h;
							if (CheckRect(dstfloatBar, cursor.getPosition())) { posBar = 1; dstSelectBar = dstfloatBar; }
							dstfloatBar.w = (float)(FGUI::sizeDisplay.x * (1920 / 3.)) / 1920; dstfloatBar.h = (float)(FGUI::sizeDisplay.y * 188) / 1080;
							dstfloatBar.x = dstfloatBar.w * 2; dstfloatBar.y = FGUI::sizeDisplay.y - dstfloatBar.h;
							if (CheckRect(dstfloatBar, cursor.getPosition())) { posBar = 2; dstSelectBar = dstfloatBar; }
						}
						else posBar = 3;
					}
					onFloatBar = true;
				}

				if (c_outBar.getElapsedTime().asMilliseconds() > POINTSBAR_TIMEWAIT && !onFloatBar && show) { show = false; }
			}

			// Render
			if (locY == 0) {
				if (!onFloatDown) SDL_SetTextureAlphaMod(Res::txtr_g_floatButton, 160); else SDL_SetTextureAlphaMod(Res::txtr_g_floatButton, 255);
				SDL_Rect dstfloatButton;
				dstfloatButton.w = (float)(FGUI::sizeDisplay.y * 97) / 1080; dstfloatButton.h = (float)(FGUI::sizeDisplay.y * 42) / 1080;
				dstfloatButton.x = FGUI::sizeDisplay.x / 2 - dstfloatButton.w / 2; dstfloatButton.y = FGUI::sizeDisplay.y - dstfloatButton.h;
				SDL_RenderCopy(renderer, Res::txtr_g_floatButton, NULL, &dstfloatButton);
			}
			else {
				// Select
				if (posBar != 3) {
					dstSelectBar.y = FGUI::sizeDisplay.y - dstSelectBar.h + POINTSBAR_HEIGHT - locY;
					SDL_SetRenderDrawColor(renderer, 40, 70, 200, 160);
					SDL_RenderFillRect(renderer, &dstSelectBar);
				}

				// Frame
				dstfloatBar.w = (float)(FGUI::sizeDisplay.x * 1920) / 1920; dstfloatBar.h = (float)(FGUI::sizeDisplay.y * 188) / 1080;
				dstfloatBar.x = FGUI::sizeDisplay.x / 2 - dstfloatBar.w / 2; dstfloatBar.y = FGUI::sizeDisplay.y - dstfloatBar.h + POINTSBAR_HEIGHT - locY;
				SDL_RenderCopy(renderer, Res::txtr_g_floatBar, NULL, &dstfloatBar);

				// Points
				SDL_Rect dstPoints;
				
				dstPoints.w = (float)(FGUI::sizeDisplay.x * srcr_points[0].w) / 1920; dstPoints.h = (float)(FGUI::sizeDisplay.y * srcr_points[0].h) / 1080;
				dstPoints.x = (float)(FGUI::sizeDisplay.x * (137 + 235)) / 1920 - dstPoints.w / 2; dstPoints.y = FGUI::sizeDisplay.y - dstPoints.h + POINTSBAR_HEIGHT - locY + (float)(FGUI::sizeDisplay.y * 23) / 1080;
				SDL_RenderCopy(renderer, t_points[0], &srcr_points[0], &dstPoints);

				dstPoints.w = (float)(FGUI::sizeDisplay.x * srcr_points[1].w) / 1920; dstPoints.h = (float)(FGUI::sizeDisplay.y * srcr_points[1].h) / 1080;
				dstPoints.x = (float)(FGUI::sizeDisplay.x * (777 + 235)) / 1920 - dstPoints.w / 2; dstPoints.y = FGUI::sizeDisplay.y - dstPoints.h + POINTSBAR_HEIGHT - locY + (float)(FGUI::sizeDisplay.y * 23) / 1080;
				SDL_RenderCopy(renderer, t_points[1], &srcr_points[1], &dstPoints);

				dstPoints.w = (float)(FGUI::sizeDisplay.x * srcr_points[2].w) / 1920; dstPoints.h = (float)(FGUI::sizeDisplay.y * srcr_points[2].h) / 1080;
				dstPoints.x = (float)(FGUI::sizeDisplay.x * (1417 + 235)) / 1920 - dstPoints.w / 2; dstPoints.y = FGUI::sizeDisplay.y - dstPoints.h + POINTSBAR_HEIGHT - locY + (float)(FGUI::sizeDisplay.y * 23) / 1080;
				SDL_RenderCopy(renderer, t_points[2], &srcr_points[2], &dstPoints);

			}

			// Other
			ignorPressForPointsBar = false;
			if (cmdPoints.display()) { c_outBar.restart(); callDontUpdateOther = true; }
			else callDontUpdateOther = false;
		}
	} pointsBar;
}