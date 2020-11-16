#pragma once

#include "res.h"
#include "cursor.h"

float menu_quake;
float menu_quakeMax = 0.08;
float menu_quakeDiv = 0.002;
float menu_quakeFPS = 80;

#include "q_menuGui.h"

#include "INFO.h"

const int ANIMFPS_MENU = 80;

namespace FQuester {


	struct Menu {
		SDL_Rect dstlogo; bool doublePressed; sf::Clock doubleTime;
		float logoScale; sf::Clock t;

		 sf::Clock t2;

		int trans_light_dir_def;
		int trans_light; int trans_light_dir; FGUI::Timer trans_light_time;
		SDL_Texture* t_footer; SDL_Rect t_footer_dstrect;

		int emup;

		Menu() {
			trans_light_dir_def = 4;
			doublePressed = false;
		}
		void Init() {
			emup = -140;

			logoScale = 10;
			menu_quake = 0;

			q_menu_part = 0;

			Res::aude_part_one.setDiv(AUDE_DIV_MINUS); Res::aude_part_two.setDiv(AUDE_DIV_MINUS); Res::aude_part_three.setDiv(AUDE_DIV_MINUS); Res::aude_part_four.setDiv(AUDE_DIV_MINUS);
			Res::aude_serf.setDiv(AUDE_DIV_MINUS); Res::aude_categories.setDiv(AUDE_DIV_MINUS);
			Res::aude_menu.setDiv(AUDE_DIV_PLUS);
			
			trans_light = 0; trans_light_dir = trans_light_dir_def;

			shadow.set(255, -(LONGDIR_SHAD));

			if (!t_footer) {
				SDL_Color c; c.r = c.g = c.b = c.a = 255;
				std::wstring t_t = FQuester::INFO_NAME; t_t += L" ";
				t_t += FQuester::INFO_VERSION; t_t += L" (";
				t_t += FQuester::INFO_DATE; t_t += L")";
				auto bufserf = TTF_RenderUNICODE_Blended(Res::ttf_u_menu_footer, (Uint16*)t_t.c_str(), c);
				t_footer_dstrect.w = bufserf->w; t_footer_dstrect.h = bufserf->h;
				t_footer = SDL_CreateTextureFromSurface(renderer, bufserf);
				SDL_FreeSurface(bufserf);
			}	
		}
		void PreUpdate() {
			if (q_menu_part == 0) qmgMain.init();
			else qmgSelect.init();
		}
		void CheckEvents() {
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_F1) {
				if (CreateWindowAndRenderer(settings::getWindowSize(), false)) FGUI::NewSizeWindow();
				fw_settings.cbv_fullscreen->setChecked(false);
				SDL_MinimizeWindow(window);
				system("start https://fquester.000webhostapp.com/index.html");
			}

			if (FQuester::CheckRect(dstlogo, FQuester::cursor.getPosition()) && event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT && logoScale == 1)
			{
				if (doublePressed) { devMode = !devMode; Res::aud_shutdown.play(); }
				else {
					doublePressed = true;
					doubleTime.restart();
				}
			}

			if (q_menu_part == 0) qmgMain.checkEvents();
			else qmgSelect.checkEvents();

			cursor.checkEvents();
		}
		void PostUpdate() {

		}
		void Display() {
			if (doubleTime.getElapsedTime().asMilliseconds() > 300) doublePressed = false;

			// move
			if ((!settings::s_vsync && t.getElapsedTime().asMilliseconds() > (float)1000. / ANIMFPS_MENU) || settings::s_vsync) {
				t.restart();
				if (logoScale > 1) logoScale -= 0.3;
				else logoScale = 1;
			}
			if ((!settings::s_vsync && t2.getElapsedTime().asMilliseconds() > (float)1000. / menu_quakeFPS) || settings::s_vsync) {
				t2.restart();
				menu_quake -= menu_quakeDiv;
				if (menu_quake < 0) { menu_quake = 0; }
			}

			// bg
			SDL_Rect srcbg;
			srcbg.w = 1080 * ((float)FGUI::sizeDisplay.x / FGUI::sizeDisplay.y);
			srcbg.h = 1080;
			srcbg.x = (float)1920 / 2 - (float)srcbg.w / 2; srcbg.y = 0;
			SDL_RenderCopy(renderer, Res::txtr_m_bg_menu, &srcbg, NULL);

			// Emup
			SDL_Rect emup_r;
			if (emup < 1100) {
				emup_r.x = 0; emup_r.w = FGUI::sizeDisplay.x;
				if (emup > 0) {
					emup_r.y = (emup + rand() % 5) * FGUI::sizeDisplay.y / 1080.; emup_r.h = (140 + rand() % 5) * FGUI::sizeDisplay.y / 1080.;
				}
				else {
					emup_r.y = 0; emup_r.h = ((140 + rand() % 5) + (emup + rand() % 5)) * FGUI::sizeDisplay.y / 1080.;
				}
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 20 + rand() % 5);
				SDL_RenderFillRect(renderer, &emup_r);
			}

			// TV
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(renderer, t_n[t_n_frame], NULL, NULL);
			if (t_n_time.getElapsedTime().asMilliseconds() > 1000. / NOISE_FPS) {
				t_n_time.restart();
				t_n_frame++;
				if (t_n_frame >= 12) t_n_frame = 0;
				SDL_SetTextureAlphaMod(t_n[t_n_frame], 25);
			}

			// light
			SDL_Rect dstlight;
			dstlight.x = (float)(1920 - 1239) / 1920 * FGUI::sizeDisplay.x; dstlight.y = 0;
			dstlight.w = (float)1239 / 1920 * FGUI::sizeDisplay.x; dstlight.h = FGUI::sizeDisplay.y;

			if (trans_light_time.getMilliseconds() > 33) { // 30 fps
				emup+=8; if (emup > 4080) emup = -140;


				trans_light += trans_light_dir; if (trans_light > 255) { trans_light_dir = -trans_light_dir_def / 2; trans_light = 255; }
				else if (trans_light < 0) { trans_light_dir = trans_light_dir_def; trans_light = 0; }
				trans_light_time.restart();
			}
			SDL_SetTextureAlphaMod(Res::txtr_m_light, trans_light);
			SDL_RenderCopy(renderer, Res::txtr_m_light, NULL, &dstlight);

			// frame
			SDL_Rect dstframe;
			dstframe.h = (float)(FGUI::sizeDisplay.y * 978) / 1080;
			dstframe.w = (float)FGUI::sizeDisplay.y / 1080 * 634;
			float r1 = 0; if (menu_quake != 0) { r1 = (std::rand() % 100) * menu_quake; r1 = r1 - r1 / 2; } dstframe.x = FGUI::sizeDisplay.x / 2 - dstframe.w / 2 + r1;
			float r2 = 0; if (menu_quake != 0) { r2 = (std::rand() % 100) * menu_quake; r2 = r2 - r2 / 2; } dstframe.y = FGUI::sizeDisplay.y / 2 - dstframe.h / 2 + r2;
			SDL_RenderCopy(renderer, Res::txtr_m_frame, NULL, &dstframe);

			// gui
			if (q_menu_part == 0) {
				// gui
				qmgMain.render();
				qmgMain.display();

				// logo
				
				
				dstlogo.h = logoScale * (float)(FGUI::sizeDisplay.y * 254) / 1080;
				dstlogo.w = logoScale * (float)FGUI::sizeDisplay.y / 1080 * 513;
				dstlogo.x = FGUI::sizeDisplay.x / 2 - dstlogo.w / 2;
				dstlogo.y = FGUI::sizeDisplay.y / 9;
				if (logoScale != 1) SDL_SetTextureAlphaMod(Res::txtr_m_logo, 255 * ((10 - logoScale) / 10));
				else SDL_SetTextureAlphaMod(Res::txtr_m_logo, 255);
				SDL_RenderCopy(renderer, Res::txtr_m_logo, NULL, &dstlogo);
			}
			else {
				qmgSelect.render();
				qmgSelect.display();
			}

			// footer
			t_footer_dstrect.x = FGUI::sizeDisplay.x - t_footer_dstrect.w - 10;
			t_footer_dstrect.y = FGUI::sizeDisplay.y - t_footer_dstrect.h - 5;
			SDL_RenderCopy(renderer, t_footer, NULL, &t_footer_dstrect);

			// shadow
			FQuester::shadow.display();

			// cursor
			cursor.display();
		}
	} q_menu;
}