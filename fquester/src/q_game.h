#pragma once


#include <SDL_net.h>

#include "res.h"
#include "cursor.h"
#include "q_menuGui.h"
#include "infoDev.h"
#include "QAL_Reader.h"
#include "q_pointsBar.h"
#include "q_showTopics.h"
#include "q_topicsBar.h"
#include "q_tableAsk.h"
#include "game_globalVars.h"
#include "q_end.h"

namespace FQuester {
	struct Game {
		cv::Mat v_framebg;
		SDL_Texture* v_texturebg;
		sf::Clock clock;
		sf::Clock clock2;

		float modeColor;

		// Topics show
		Game() {
			modeColor = 255;
		}
		void Init() {
			part_q_game = 0;

			Res::aude_menu.setDiv(AUDE_DIV_MINUS);

			if (!v_texturebg) {
				v_texturebg = SDL_CreateTexture(
					renderer,
					SDL_PIXELFORMAT_BGR24,
					SDL_TEXTUREACCESS_STREAMING,
					(int)Res::cap.get(CV_CAP_PROP_FRAME_WIDTH),
					(int)Res::cap.get(CV_CAP_PROP_FRAME_HEIGHT)
				);
			}
			fullIgnoreUpdateBg = ignoreUpdateBg = false;

			/////////////////////
			showTopics.init();
			//////////////////////
			start_num_ask = 0;
			topicsBar.init();
			pointsBar.init();
			showerAsk.init();
			tableAsk.init(); 
			scEnd.init();

			cursor.videoMode(true);
		}
		void PreUpdate() {
		}
		void CheckEvents() {
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_ESCAPE && (part_q_game != 4)) {
				showerAsk.shutDown();
				
				fullIgnoreUpdateBg = ignoreUpdateBg = false;
				cursor.videoMode(false);
				n_frameLimit = FRAME_LIMIT;

				part_q_game = 3;
				shadow.set(255, -DIR_SHAD);
				Res::aude_part_one.setDiv(AUDE_DIV_MINUS);
				Res::aude_part_two.setDiv(AUDE_DIV_MINUS);
				Res::aude_part_three.setDiv(AUDE_DIV_MINUS);
				Res::aude_part_four.setDiv(AUDE_DIV_MINUS);

				Res::aud_end.play();
				activateMenu = true;
			}
			cursor.checkEvents();
			// Topics
			if (part_q_game == 0) showTopics.checkEvents();
			// Table
			else if (part_q_game == 1) {
				if (cursor.getVideoMode()) cursor.videoMode(false);
				pointsBar.checkEvents();
				if (!pointsBar.checkCol()) { topicsBar.checkEvents(); tableAsk.checkEvents(); }
			}
			// Ask
			else if (part_q_game == 2) {
				showerAsk.checkEvents();
			}
			// Menu
			else if (part_q_game == 3) {

			}
			// End
			else if (part_q_game == 4) {
				scEnd.checkEvents();
			}
		}
		void PostUpdate() {
		}
		void Display() {
			// Clear
			if (clock.getElapsedTime().asMicroseconds() > 1000000. / 30. && !ignoreUpdateBg && !fullIgnoreUpdateBg) {
				clock.restart();
				Res::cap >> v_framebg;
				if (v_framebg.empty())
				{
					Res::cap.set(CV_CAP_PROP_POS_MSEC, 0.9f);
					Res::cap >> v_framebg;
				}
				FQuester::vPars(v_texturebg, v_framebg);
			}
			if (clock2.getElapsedTime().asMicroseconds() > 1000000. / 80. || settings::s_vsync) {
				clock2.restart();
				if (fiasko && plusPoints != 0) {
					fiasko = false;
					modeColor = 60;
				}
				modeColor += 4; if (modeColor > 255) modeColor = 255;
				SDL_SetTextureColorMod(v_texturebg, 255, modeColor, modeColor);
			}
			if (!fullIgnoreUpdateBg) { 
				SDL_SetRenderTarget(renderer, NULL); 
				SDL_RenderCopy(renderer, v_texturebg, NULL, NULL); 
			}

			// Topics
			if (part_q_game == 0) { 
				if (!showTopics.display()) { 
					part_q_game = 1; 
					Res::aude_categories.setDiv(AUDE_DIV_MINUS); 
					if(settings::s_bmusic) Res::aude_part_one.setDiv(AUDE_DIV_PLUS);
				} 
			}
			// Table
			else if (part_q_game == 1) { 
				topicsBar.display();
				tableAsk.display();
				SDL_RenderCopy(renderer, Res::txtr_g_bg, NULL, NULL);
				pointsBar.display(); 
			}
			// Ask
			else if (part_q_game == 2) {
				if (shadow.shadowPer == 255) shadow.set(254, -DIR_SHAD);
				showerAsk.display();
			}
			// Menu
			else if (part_q_game == 3) {
				
			}
			// End
			else if (part_q_game == 4) {
				scEnd.display();
			}

			FQuester::shadow.display();
			cursor.display();
		}
	} q_game;
}
