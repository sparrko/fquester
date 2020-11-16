#pragma once

#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"
#include "q_cmdPoints.h"
#include "QAL_Reader.h"
#include "settings.h"

int TOPICSBAR_FPS = 80;
int TOPICSBAR_STARTDIR = 20;
int TOPICSBAR_RETREATX = 15;
int TOPICSBAR_HEIGHT = 120;
int TOPICSBAR_RETREATINTEX = 23;

namespace FQuester {

	class TopicsBar {
		SDL_Texture* t_topics;
		float down;
		SDL_Rect dsttop;
		SDL_Rect srctop;
		SDL_Rect dstSelected;
		uint16_t selected;
		sf::Clock c_fps;
		int moveX, dir;
		bool in;
	public:
		TopicsBar() {
			t_topics = NULL;
		}
		void init() {
			selected = 256;
			down = 0;
			in = false;
			moveX = -638; dir = TOPICSBAR_STARTDIR;

			if (t_topics) SDL_DestroyTexture(t_topics);
			t_topics = SDL_CreateTexture(renderer, NULL, SDL_TEXTUREACCESS_TARGET, 638, FQuester::QM_Reader::topics.size() * TOPICSBAR_HEIGHT + TOPICSBAR_RETREATINTEX);
			SDL_SetTextureBlendMode(t_topics, SDL_BLENDMODE_BLEND);
			SDL_SetRenderTarget(renderer, t_topics);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
			SDL_RenderFillRect(renderer, NULL);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_Color c; c.r = 222; c.g = 215; c.b = 143; c.a = 255;
			for (uint16_t i = 0; i < FQuester::QM_Reader::topics.size(); i++) {
				SDL_Rect dst;

				auto bufserf = TTF_RenderUNICODE_Blended(Res::ttf_topicsBar, (Uint16*)FQuester::QM_Reader::topics[i].name.c_str(), c);
				auto buft = SDL_CreateTextureFromSurface(renderer, bufserf);

				dst.x = TOPICSBAR_RETREATX;
				dst.y = TOPICSBAR_HEIGHT * i;
				dst.w = bufserf->w;
				dst.h = bufserf->h;

				SDL_RenderCopy(renderer, buft, NULL, &dst);

				SDL_FreeSurface(bufserf);
				SDL_DestroyTexture(buft);
			}
			SDL_SetRenderTarget(renderer, NULL);
		}
		void checkEvents() {
			if (event.type == SDL_MOUSEWHEEL && in)
			{
				if (event.wheel.y > 0) // scroll up
				{
					down -= mouseWhellSpeed;
					if (down < 0) down = 0;
				}
				else if (event.wheel.y < 0) // scroll down
				{
					down += mouseWhellSpeed;

					if (FQuester::QM_Reader::topics.size() * TOPICSBAR_HEIGHT - 870.f < 0) {
						down = 0;
					}
					else {
						if (down > FQuester::QM_Reader::topics.size() * TOPICSBAR_HEIGHT - 870)
							down = FQuester::QM_Reader::topics.size() * TOPICSBAR_HEIGHT - 870;
					}
				}
			}
			if (in && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
				int bufselected = (cursor.getPosition().y - (FGUI::sizeDisplay.y * (214 - down)) / 1080.f) / ((FGUI::sizeDisplay.y * TOPICSBAR_HEIGHT) / 1080.f);

				if (bufselected < QM_Reader::topics.size()) {
					if (selected != bufselected) {
						selected = bufselected;
						mRenderAskTable = true;
						nTopic = selected;
					}
				}

			}
		}
		void display() {
			// Update

			if ((!settings::s_vsync && c_fps.getElapsedTime().asMilliseconds() > (float)1000. / TOPICSBAR_FPS) || settings::s_vsync) {
				c_fps.restart();

				SDL_Rect checkrect;
				checkrect.x = 0; checkrect.w = (float)(FGUI::sizeDisplay.x * 638) / 1920;
				checkrect.y = (float)(FGUI::sizeDisplay.y * 214) / 1080; checkrect.h = (float)(FGUI::sizeDisplay.y * (846)) / 1080;
				if (CheckRect(checkrect, cursor.getPosition())) { in = true; }
				else { in = false; }

				moveX += dir;
				if (moveX > 0) { moveX = 0; dir = 0; }
			}

			// Display

			srctop.x = 0;
			srctop.y = down + TOPICSBAR_RETREATINTEX;
			srctop.w = 638;
			srctop.h = FQuester::QM_Reader::topics.size() * TOPICSBAR_HEIGHT - down;

			dsttop.x = ((float)FGUI::sizeDisplay.x * moveX) / 1920.f;
			dsttop.y = (FGUI::sizeDisplay.y * 214) / 1080.f;
			dsttop.h = (FGUI::sizeDisplay.y * srctop.h) / 1080.f;
			dsttop.w = (FGUI::sizeDisplay.x * srctop.w) / 1920.f;

			dstSelected.x = 0;
			dstSelected.w = (FGUI::sizeDisplay.x * srctop.w) / 1920.f;
			dstSelected.h = (FGUI::sizeDisplay.y * TOPICSBAR_HEIGHT) / 1080.f;
			dstSelected.y = (FGUI::sizeDisplay.y * (TOPICSBAR_HEIGHT * selected + 214 - down)) / 1080.f;

			if (dstSelected.y < (FGUI::sizeDisplay.y * 214) / 1080.f) {
				dstSelected.h = dstSelected.h - ((FGUI::sizeDisplay.y * 214) / 1080.f - dstSelected.y);
				dstSelected.y = (FGUI::sizeDisplay.y * 214) / 1080.f;
				if (dstSelected.h < 0) dstSelected.h = 0;
			}

			SDL_SetRenderDrawColor(renderer, 120, 120, 255, 180);
			if (selected != 256) SDL_RenderFillRect(renderer, &dstSelected);
			SDL_RenderCopy(renderer, t_topics, &srctop, &dsttop);
		}
	} topicsBar;
}