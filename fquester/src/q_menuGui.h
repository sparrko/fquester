#pragma once

#include "q_globalVars.h"

#include "fgui_system.h"
#include <SDL.h>
#include "objects.h"
#include "res.h"
#include "cursor.h"
#include "fprUnit.h"
#include <iostream>
#include "shadow.h"


namespace FQuester{
	const int button_main_sizeX = 484;
	const int button_main_sizeY = 109;
	const int button_main_sizeYin = 100;

	const int button_main_transMin = 20;
	const int button_main_transStart = 255;
	const int button_main_transDivP = 30;
	const int button_main_transDivM = 6;
	const int button_main_transSpeed = 14;

	struct QMGMain {
		//SDL_Rect dst;
		SDL_Texture* tt;
		FGUI::Timer tm;
		int trans[5];
		QMGMain() {
			tt = NULL;
		}
		void init() {
			if (!tt) {
				tt = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 484, 510);
				SDL_SetTextureBlendMode(tt, SDL_BLENDMODE_BLEND);

				for (int a = 0; a < 5; a++) {
					trans[a] = button_main_transStart;
				}
			}	
		}
		void checkEvents() {
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				sc.sendMessage("fwin exitAsk");
			}
			if (event.button.button == SDL_BUTTON_LEFT) {
				SDL_Rect dstgui;
				dstgui.x = 0; dstgui.y = (float)(FGUI::sizeDisplay.y * 452) / 1080; dstgui.w = (float)(FGUI::sizeDisplay.y * 484) / 1080; dstgui.h = (float)(FGUI::sizeDisplay.y * 109) / 1080;
				int bufrx = FGUI::sizeDisplay.x / 2 - dstgui.w / 2;
				if (cursor.getPosition().x > bufrx && cursor.getPosition().x < bufrx + dstgui.w) {
					for (int a = 0; a < 5; a++) {
						if (cursor.getPosition().y > dstgui.y && cursor.getPosition().y < dstgui.y + (float)(FGUI::sizeDisplay.y * button_main_sizeYin) / 1080) {
							switch (a) {
								case 0: 
									if (event.type == SDL_MOUSEBUTTONDOWN) {
										q_menu_part = 1; FRP::mustRenderListFrp = true;
									}
									break;
								case 1: 
									if (event.type == SDL_MOUSEBUTTONUP) {
										int WinX; SDL_GetWindowSize(window, &WinX, NULL);
										if (WinX < 1024) { activateBadResol = true; }
										else { mInitRedactor = true; }
									}
									break;
								case 2: if (event.type == SDL_MOUSEBUTTONUP) { activateSettings = true; } break;
								case 3: 
									if (event.type == SDL_MOUSEBUTTONDOWN) {
										if (CheckFullscreen()) { ChangeFullscreen(); }
										fw_settings.cbv_fullscreen->setChecked(false);
										SDL_MinimizeWindow(window);
										system("start https://fquester.000webhostapp.com/index.html");
									}
								break;
								case 4: if (event.type == SDL_MOUSEBUTTONDOWN) { activateExitAsk = true; } break;
							}
						}
						dstgui.y += (float)(FGUI::sizeDisplay.y * button_main_sizeYin) / 1080;
					}
				}
			}
		}
		void render() {
			SDL_Rect dstgui;

			if (tm.getMilliseconds() > button_main_transSpeed) {
				dstgui.x = 0; dstgui.y = (float)(FGUI::sizeDisplay.y * 452) / 1080; dstgui.w = (float)(FGUI::sizeDisplay.y * 484) / 1080; dstgui.h = (float)(FGUI::sizeDisplay.y * 109) / 1080;
				int bufrx = FGUI::sizeDisplay.x / 2 - dstgui.w / 2;
				if (cursor.getPosition().x > bufrx && cursor.getPosition().x < bufrx + dstgui.w) {
					for (int a = 0; a < 5; a++) {
						if (cursor.getPosition().y > dstgui.y && cursor.getPosition().y < dstgui.y + (float)(FGUI::sizeDisplay.y * button_main_sizeYin) / 1080) trans[a] += button_main_transDivP;
						else trans[a] -= button_main_transDivM;

						if (trans[a] < button_main_transMin) trans[a] = button_main_transMin;
						else if (trans[a] > 255) trans[a] = 255;

						dstgui.y += (float)(FGUI::sizeDisplay.y * button_main_sizeYin) / 1080;
					}
				}
				else {
					for (int a = 0; a < 5; a++) {
						trans[a] -= button_main_transDivM;

						if (trans[a] < button_main_transMin) trans[a] = button_main_transMin;
						else if (trans[a] > 255) trans[a] = 255;
					}
				}
				tm.restart();
			}

			////////////////////////////////////////////////////

			SDL_SetRenderTarget(renderer, tt);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderFillRect(renderer, NULL);

			dstgui.x = 0; dstgui.y = 5; dstgui.w = 484; dstgui.h = 109;
			for (int a = 0; a < 5; a++) {
				SDL_SetRenderDrawColor(renderer, 137, 178, 255, trans[a]); //(renderer, 204, 189, 66, trans[a]);
				SDL_RenderFillRect(renderer, &dstgui);
				dstgui.y += button_main_sizeYin;

			}
			SDL_RenderCopy(renderer, Res::txtr_m_gui_menu, NULL, NULL);

			SDL_SetRenderTarget(renderer, NULL);
		}
		void display() {
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_Rect dstgui; 
			dstgui.w = (float)(FGUI::sizeDisplay.y * 484) / 1080;
			dstgui.h = (float)(FGUI::sizeDisplay.y * 510) / 1080;
			dstgui.x = FGUI::sizeDisplay.x / 2 - dstgui.w / 2;
			dstgui.y = (float)(FGUI::sizeDisplay.y * 452) / 1080;
			SDL_RenderCopy(renderer, tt, NULL, &dstgui);
		}
	} qmgMain;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool CheckRect(SDL_Rect rect, FGUI::Vector2i mousePos) {
		if (mousePos.x > rect.x && mousePos.y > rect.y && mousePos.x < rect.x + rect.w && mousePos.y < rect.y + rect.h) return true;
		else return false;
	}

	const int select_down_sizeX = 23;
	
	struct QMGFRPList{
		SDL_Texture* t_display;
		SDL_Rect dst, src;
		SDL_Rect trans;
		int sizeY;
		float down;
		SDL_Color c_main;
		int selected;
		bool lPresg; bool callT;
		QMGFRPList() {
			down = 0;
			c_main.r = 222; c_main.g = 215; c_main.b = 143; c_main.a = 255;
			selected = -1;
			lPresg = callT = false;
		}
		void checkEvents() {
			SDL_Rect cr;
			cr.w = (float)(FGUI::sizeDisplay.y * 455) / 1080;
			cr.x = FGUI::sizeDisplay.x / 2 - cr.w / 2;
			cr.y = (float)(FGUI::sizeDisplay.y * 132) / 1080;
			cr.h = (float)(FGUI::sizeDisplay.y * 560) / 1080;
			cr.w -= (float)(FGUI::sizeDisplay.y * 30) / 1080;

			SDL_Rect downB = cr;
			downB.x = cr.x + cr.w;
			downB.w = 30;

			if (CheckRect(downB, cursor.getPosition())) {
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) lPresg = callT = true;
			}
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) lPresg = false;


			if (lPresg) {
				if (event.type == SDL_MOUSEMOTION || callT) {
					if (FRP::frpList.size() > 2) {
						down = (float)(cursor.getPosition().y - downB.y) / downB.h * (sizeY - 570);

						if (down > sizeY - 570) down = sizeY - 570;
						if (down < 0) down = 0;
					}
					callT = false;
				}
			}
			

			if (CheckRect(cr, cursor.getPosition())) {
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
					int my = cursor.getPosition().y - (float)(FGUI::sizeDisplay.y * 142) / 1080;
					int bufselected = (my + (float)(FGUI::sizeDisplay.y * down) / 1080) / ((float)(FGUI::sizeDisplay.y * 182) / 1080);
					if (!(bufselected > FRP::frpList.size() - 1)) {
						selected = bufselected;
					}
				}
				if (event.type == SDL_MOUSEWHEEL)
				{
					if (FRP::frpList.size() > 3) {
						if (event.wheel.y > 0) // scroll up
						{
							down -= mouseWhellSpeed;
							if (down < 0) down = 0;
						}
						else if (event.wheel.y < 0) // scroll down
						{
							down += mouseWhellSpeed;
							if (down > sizeY - 570) down = sizeY - 570;
							if (down < 0) down = 0;
						}
					}
					else down = 0;
				}
			}
		}
		void display() {
			// Render
			if (FRP::mustRenderListFrp) {
				if (t_display) SDL_DestroyTexture(t_display);
				sizeY = 20 + 182 * FRP::frpList.size() + 15;
				t_display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 455, sizeY);
				SDL_SetTextureBlendMode(t_display, SDL_BLENDMODE_BLEND);
				
				SDL_SetRenderTarget(renderer, t_display); 
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE); SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderFillRect(renderer, NULL);

				SDL_Surface* serf; SDL_Texture* txtr; SDL_Rect dste;
				for (int a = 0; a < FRP::frpList.size(); a++) {
					// Frame
					SDL_Rect dstn; dstn.x = 0; dstn.y = 10 + a * 182; dstn.w = 425; dstn.h = 190;
					SDL_Rect src; src.x = 0; src.y = 0;
					SDL_RenderCopy(renderer, Res::txtr_m_gui_select_select, NULL, &dstn);

					// Name
					serf = TTF_RenderUNICODE_Blended(Res::ttf_select_name, (Uint16*)FRP::frpList[a].name.c_str(), c_main);
					if (serf) {
						txtr = SDL_CreateTextureFromSurface(renderer, serf);
						dste.x = 15;
						dste.y = dstn.y - 3;
						dste.w = serf->w;
						dste.h = serf->h;
						
						SDL_QueryTexture(txtr, NULL, NULL, &src.w, &src.h); if (src.w > 395) { src.w = dste.w = 395; }

						SDL_RenderCopy(renderer, txtr, &src, &dste);
						SDL_FreeSurface(serf); SDL_DestroyTexture(txtr);
					}

					// Autor
					serf = TTF_RenderUNICODE_Blended(Res::ttf_select_other, (Uint16*)FRP::frpList[a].autor.c_str(), c_main);
					if (serf) {
						txtr = SDL_CreateTextureFromSurface(renderer, serf);
						dste.x = 15;
						dste.y = dstn.y + 75;
						dste.w = serf->w;
						dste.h = serf->h;

						SDL_QueryTexture(txtr, NULL, NULL, &src.w, &src.h); if (src.w > 395) { src.w = dste.w = 395; }

						SDL_RenderCopy(renderer, txtr, &src, &dste);
						SDL_FreeSurface(serf); SDL_DestroyTexture(txtr);
					}

					// Date
					serf = TTF_RenderUNICODE_Blended(Res::ttf_select_other, (Uint16*)FRP::frpList[a].date.c_str(), c_main);
					if (serf) {
						txtr = SDL_CreateTextureFromSurface(renderer, serf);
						dste.x = 15;
						dste.y = dstn.y + 125;
						dste.w = serf->w;
						dste.h = serf->h;

						SDL_RenderCopy(renderer, txtr, NULL, &dste);
						SDL_FreeSurface(serf); SDL_DestroyTexture(txtr);
					}

					// AskTopics
					serf = TTF_RenderUNICODE_Blended(Res::ttf_select_other, (Uint16*)FRP::frpList[a].topicsAsks.c_str(), c_main);
					if (serf) {
						txtr = SDL_CreateTextureFromSurface(renderer, serf);
						dste.y = dstn.y + 125;
						dste.w = serf->w;
						dste.x = 410 - dste.w;
						dste.h = serf->h;

						SDL_RenderCopy(renderer, txtr, NULL, &dste);
						SDL_FreeSurface(serf); SDL_DestroyTexture(txtr);
					}
				}

				FRP::mustRenderListFrp = false; SDL_SetRenderTarget(renderer, NULL);
			}

			// Check
			if (qmgFPRList.down + 570 > qmgFPRList.sizeY) {
				qmgFPRList.down = qmgFPRList.sizeY - 570;
				if (qmgFPRList.down < 0) qmgFPRList.down = 0;
			}

			// Display
			src.x = 0;
			src.y = down;
			src.w = 455;
			src.h = sizeY; if (src.h > 560) { src.h = 560; }
			dst.w = (float)(FGUI::sizeDisplay.y * 455) / 1080;
			dst.x = FGUI::sizeDisplay.x / 2 - dst.w / 2;
			dst.y = (float)(FGUI::sizeDisplay.y * 132) / 1080;						
			dst.h = sizeY; if (dst.h > 560) { dst.h = 560; }
			dst.h = (float)(FGUI::sizeDisplay.y * dst.h) / 1080;

			if (selected > -1) {
				trans.x = dst.x;
				trans.y = dst.y + (float)(FGUI::sizeDisplay.y * (16 + 182 * (selected) - down)) / 1080;
				trans.w = dst.w - (float)(FGUI::sizeDisplay.y * 35) / 1080;
				trans.h = (float)(FGUI::sizeDisplay.y * 182) / 1080;

				SDL_SetRenderDrawColor(renderer, c_main.r, c_main.g, c_main.b, 80);

				if (trans.y < (float)(FGUI::sizeDisplay.y * 132) / 1080) {
					trans.h = trans.y + trans.h - (float)(FGUI::sizeDisplay.y * 132) / 1080;
					trans.y = (float)(FGUI::sizeDisplay.y * 132) / 1080;
					if (trans.h > 0) SDL_RenderFillRect(renderer, &trans);
				}
				else if (trans.y + trans.h > (float)(FGUI::sizeDisplay.y * 686) / 1080) {
					int bufy = (trans.y - (float)(FGUI::sizeDisplay.y * 132) / 1080) + trans.h - (float)(FGUI::sizeDisplay.y * 558) / 1080;
					trans.h = trans.h - bufy;
					if (trans.h > 0) SDL_RenderFillRect(renderer, &trans);
				}
				else {
					SDL_RenderFillRect(renderer, &trans);
				}
				
			}

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(renderer, t_display, &src, &dst);

			// Down
			if (FRP::frpList.size() > 2) {
				dst.x += (dst.w - (float)(FGUI::sizeDisplay.y * select_down_sizeX) / 1080);
				dst.h = (float)(FGUI::sizeDisplay.y * ((float)560 / sizeY * 560 - 11)) / 1080;
				dst.y = (float)(FGUI::sizeDisplay.y * (140 + (float)560 / sizeY * down)) / 1080;
				dst.w = (float)(FGUI::sizeDisplay.y * select_down_sizeX) / 1080;
				SDL_SetRenderDrawColor(renderer, c_main.r, c_main.g, c_main.b, c_main.a);
				SDL_RenderFillRect(renderer, &dst);
			}
		}
	} qmgFPRList;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	struct QMGSelect {
		SDL_Rect rtrans[4];
		int itrans[4];
		FGUI::Timer tm;
		FGUI::Vector2i posBt;
		QMGSelect() {
			FRP::Update();
			rtrans[0].x = 0; rtrans[0].y = 0; rtrans[0].w = 103; rtrans[0].h = 100;
			rtrans[1].x = 102; rtrans[1].y = 0; rtrans[1].w = 372; rtrans[1].h = 100;
			rtrans[2].x = 0; rtrans[2].y = 100; rtrans[2].w = 238; rtrans[2].h = 100;
			rtrans[3].x = 238; rtrans[3].y = 100; rtrans[3].w = 238; rtrans[3].h = 100;

			for (int a = 0; a < 5; a++) {
				itrans[a] = 0;
			}

			posBt.x = 0;
			posBt.y = 0;
		}
		void init() {

		}
		void checkEvents() {
			
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) q_menu_part = 0;

			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
				SDL_Rect dstgui;

				for (int a = 0; a < 4; a++) {
					dstgui.x = (float)(FGUI::sizeDisplay.y * rtrans[a].x) / 1080 + posBt.x;
					dstgui.y = (float)(FGUI::sizeDisplay.y * rtrans[a].y) / 1080 + posBt.y;
					dstgui.w = (float)(FGUI::sizeDisplay.y * rtrans[a].w) / 1080;
					dstgui.h = (float)(FGUI::sizeDisplay.y * rtrans[a].h) / 1080;
					if (CheckRect(dstgui, cursor.getPosition())) {
						switch (a) {
							case 0: q_menu_part = 0; break;
							case 1: if (qmgFPRList.selected > -1) { callStart = true; shadow.set(0, 15); } else { Res::aud_warning.play(); menu_quake = menu_quakeMax; } break; // Go game
							case 2: 
								if (qmgFPRList.selected > -1) {
									activateReallyDelete = true;
									frpSelected = qmgFPRList.selected;
									
								}
								else { Res::aud_warning.play(); menu_quake = menu_quakeMax; }
								//qmgFPRList.down = 0;
								break;
							case 3: 
								if (CheckFullscreen()) { ChangeFullscreen(); }
								FQuester::FRP::Add();
								/////////////////////////////////////////////////////////////////////////////////////////////////////
								FGUI::NewSizeWindow();
								break;
						}
					}
				}
			}

			qmgFPRList.checkEvents();
		}
		void render() {
			if (FQuester::callStart && shadow.shadowPer == 255) {
				mInitGame = true;
			}

			if (mChangeSelect) { mChangeSelect = false; qmgFPRList.selected = -1; }

			posBt.x = (float)FGUI::sizeDisplay.x / 2 - ((float)(FGUI::sizeDisplay.y * rtrans[3].w) / 1080);
			posBt.y = (float)743 / 1080 * FGUI::sizeDisplay.y;

			SDL_Rect dstgui;

			if (tm.getMilliseconds() > button_main_transSpeed) {
				for (int a = 0; a < 4; a++) {
					dstgui.x = (float)(FGUI::sizeDisplay.y * rtrans[a].x) / 1080 + posBt.x;
					dstgui.y = (float)(FGUI::sizeDisplay.y * rtrans[a].y) / 1080 + posBt.y;
					dstgui.w = (float)(FGUI::sizeDisplay.y * rtrans[a].w) / 1080;
					dstgui.h = (float)(FGUI::sizeDisplay.y * rtrans[a].h) / 1080;
					if (CheckRect(dstgui, cursor.getPosition())) itrans[a] += button_main_transDivP;
					else itrans[a] -= button_main_transDivM;
					if (itrans[a] < button_main_transMin) itrans[a] = button_main_transMin;
					else if (itrans[a] > 255) itrans[a] = 255;
				}

				tm.restart();
			}
		}
		void display() {
			SDL_Rect dstgui;

			// trans
			for (int a = 0; a < 4; a++) {
				dstgui.x = (float)(FGUI::sizeDisplay.y * rtrans[a].x) / 1080 + posBt.x;
				dstgui.y = (float)(FGUI::sizeDisplay.y * rtrans[a].y) / 1080 + posBt.y;
				dstgui.w = (float)(FGUI::sizeDisplay.y * rtrans[a].w) / 1080;
				dstgui.h = (float)(FGUI::sizeDisplay.y * rtrans[a].h) / 1080; 
				SDL_SetRenderDrawColor(renderer, 137, 178, 255, itrans[a]); //(renderer, 204, 189, 66, trans[a]);
				SDL_RenderFillRect(renderer, &dstgui);
			}

			// dinamic
			qmgFPRList.display();

			// static
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			dstgui.w = (float)(FGUI::sizeDisplay.y * 484) / 1080;
			dstgui.h = (float)(FGUI::sizeDisplay.y * 818) / 1080;
			dstgui.x = FGUI::sizeDisplay.x / 2 - dstgui.w / 2;
			dstgui.y = (float)(FGUI::sizeDisplay.y * 128) / 1080;
			SDL_RenderCopy(renderer, Res::txtr_m_gui_select_static, NULL, &dstgui);
		}
	} qmgSelect;
};