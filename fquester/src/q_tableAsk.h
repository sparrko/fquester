#pragma once

#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"
#include "QAL_Reader.h"
#include "textOpt.h"
#include "q_showerAsk.h"
#include "settings.h"

int TABLEASK_FPS = 80;

int TABLEASK_HEIGHT = 177;
int TABLEASK_WIDTH = 324;

int TABLEASK_RETREATX = 5;
int TABLEASK_RETREATY = TABLEASK_RETREATX;

float TABLEASK_DIV = 60;

namespace FQuester {
	class TableAsk {
		SDL_Texture* t_table;
		SDL_Rect src, dst, dstSelected;
		bool in;
		float down;
		float left, div;
		sf::Clock c_fps;
		int lines;
		int sx, sy;
		bool selected;
		uint16_t send_nTopic; uint16_t send_nAsk;
		bool goodRendered;
	public:
		void render(int nTopic) {
			if (FQuester::QM_Reader::topics[nTopic].asks.size() == 0) goodRendered = false;
			else {
				goodRendered = true;
				down = 0.;

				left = 1000;
				div = TABLEASK_DIV;

				lines = ceil(FQuester::QM_Reader::topics[nTopic].asks.size() / 4.f);

				if (t_table) SDL_DestroyTexture(t_table);
				t_table = SDL_CreateTexture(renderer, NULL, SDL_TEXTUREACCESS_TARGET, 1276 + TABLEASK_RETREATX, lines * TABLEASK_HEIGHT);
				SDL_SetTextureBlendMode(t_table, SDL_BLENDMODE_BLEND);
				SDL_SetRenderTarget(renderer, t_table);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
				SDL_RenderFillRect(renderer, NULL);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_Color c; c.r = 222; c.g = 215; c.b = 143; c.a = 255;
				SDL_Rect dstFrame, dstText;

				for (int i = 0; i < FQuester::QM_Reader::topics[nTopic].asks.size(); i++) {
					dstFrame.x = i % 4 * (TABLEASK_WIDTH - TABLEASK_RETREATX);
					dstFrame.y = floor(i / 4.f) * (TABLEASK_HEIGHT - TABLEASK_RETREATY);
					dstFrame.w = TABLEASK_WIDTH;
					dstFrame.h = TABLEASK_HEIGHT;
					SDL_RenderCopy(renderer, Res::txtr_g_askFrame, NULL, &dstFrame);

					///////////////

					auto bufserf = TTF_RenderText_Blended(Res::ttf_askFrame, std::to_string((long long)FQuester::QM_Reader::topics[nTopic].asks[i].points).c_str(), c);
					auto buftxtr = SDL_CreateTextureFromSurface(renderer, bufserf);

					dstText.w = bufserf->w; dstText.h = bufserf->h;
					dstText.x = dstFrame.x + TABLEASK_WIDTH / 2.f - dstText.w / 2.f;
					dstText.y = dstFrame.y + TABLEASK_HEIGHT / 2.f - dstText.h / 2.f;
					SDL_RenderCopy(renderer, buftxtr, NULL, &dstText);

					SDL_FreeSurface(bufserf);
					SDL_DestroyTexture(buftxtr);
				}
				SDL_SetRenderTarget(renderer, NULL);
			}
		}
		void init() {
			dstSelected.x = dstSelected.y = dstSelected.w = dstSelected.h = 0;

			selected = false;
			goodRendered = false;

			in = true;
			down = 0.;
			sx = -1; sy = -1;

			left = 1000;
			div = 0;
			lines = 0;

			send_nTopic = send_nAsk = 0;

			if (t_table) SDL_DestroyTexture(t_table);
		}
		void checkEvents() {
			if (goodRendered) {
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

						if (lines * TABLEASK_HEIGHT - 900.f < 0) {
							down = 0;
						}
						else {
							if (down > lines * TABLEASK_HEIGHT - 900)
								down = lines * TABLEASK_HEIGHT - 900;
						}
					}
				}
				if (in && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && sx > -1) {
					if (!selected) {
						///////////////////////////////////////////////////////////////////////////////////////////////////////// go to ask
						shadow.set(0, DIR_SHAD);
						selected = true;
						send_nTopic = nTopic;
						send_nAsk = sx + sy * 4;
					}
					else {
						showerAsk.loadAsk(send_nTopic, send_nAsk);
						selected = false;
						part_q_game = 2;
						shadow.set(255, -DIR_SHAD);
					}
				}
			}
		}
		void display() {
			// Update
			if ((!settings::s_vsync && c_fps.getElapsedTime().asMilliseconds() > (float)1000. / TABLEASK_FPS) || settings::s_vsync) {
				c_fps.restart();
				
				left -= div;
				if (left < 0) { left = 0; div = 0; }

				if (CheckRect(dst, cursor.getPosition())) in = true; else in = false;
				if (in && goodRendered) {
					sx = (cursor.getPosition().x - dst.x) / ((FGUI::sizeDisplay.x * (TABLEASK_WIDTH - TABLEASK_RETREATX)) / 1920.f);
					sy = (cursor.getPosition().y - dst.y + (FGUI::sizeDisplay.y * down) / 1080.f) / ((FGUI::sizeDisplay.y * (TABLEASK_HEIGHT - TABLEASK_RETREATY)) / 1080.f);
					if (sx + sy * 4 >= QM_Reader::topics[nTopic].asks.size()) sx = -1;
				}

				if (selected && shadow.shadowPer == 255) {
					showerAsk.loadAsk(send_nTopic, send_nAsk);
					selected = false;
					part_q_game = 2;
					shadow.set(255, -DIR_SHAD);
				}
			}
			// Go end if
			if (start_num_ask == sum_ask) {
				// part_q_game = 3;
				// shadow.set(255, -DIR_SHAD);
				Res::aude_part_one.setDiv(AUDE_DIV_MINUS);
				Res::aude_part_two.setDiv(AUDE_DIV_MINUS);
				Res::aude_part_three.setDiv(AUDE_DIV_MINUS);
				Res::aude_part_four.setDiv(AUDE_DIV_MINUS);

				// Res::aud_end.play();
				// callResultLogo = 2;
				// activateMenu = true;
			}
			// Render
			if (mRenderAskTable) {
				render(nTopic);
				mRenderAskTable = false;
			}
			// Display
			if (goodRendered) {
				SDL_SetRenderTarget(renderer, NULL);

				src.x = 0;
				src.w = 1276 + TABLEASK_RETREATX;
				src.h = lines * TABLEASK_HEIGHT - down;
				src.y = down;

				dst.x = (FGUI::sizeDisplay.x * (644 - TABLEASK_RETREATX + left)) / 1920.f;
				dst.y = (FGUI::sizeDisplay.y * (212 - TABLEASK_RETREATY)) / 1080.f;
				dst.w = (FGUI::sizeDisplay.x * (src.w + 1)) / 1920.f;
				dst.h = (FGUI::sizeDisplay.y * src.h) / 1080.f;

				if (in && sx > -1) {
					dstSelected.w = (FGUI::sizeDisplay.x * (TABLEASK_WIDTH - TABLEASK_RETREATX)) / 1920.f;
					dstSelected.h = (FGUI::sizeDisplay.y * TABLEASK_HEIGHT) / 1080.f;
					dstSelected.x = dst.x + (dstSelected.w + (FGUI::sizeDisplay.x * (TABLEASK_RETREATX - 2.5f)) / 1920.f) * sx;
					dstSelected.y = dst.y + (dstSelected.h - (FGUI::sizeDisplay.y * (TABLEASK_RETREATY - 1.2)) / 1080.f) * sy - (FGUI::sizeDisplay.y * down) / 1080.f;

					if (dstSelected.y < (FGUI::sizeDisplay.y * (212 - TABLEASK_RETREATY)) / 1080.f) {
						dstSelected.h = dstSelected.h - ((FGUI::sizeDisplay.y * (212 - TABLEASK_RETREATY)) / 1080.f - dstSelected.y);
						dstSelected.y = (FGUI::sizeDisplay.y * (212 - TABLEASK_RETREATY)) / 1080.f;
						if (dstSelected.h < 0) dstSelected.h = 0;
					}

					SDL_SetRenderDrawColor(renderer, 50, 50, 255, 200);
					SDL_RenderFillRect(renderer, &dstSelected);
				}

				SDL_RenderCopy(renderer, t_table, &src, &dst);
			}
		}
	} tableAsk;
}