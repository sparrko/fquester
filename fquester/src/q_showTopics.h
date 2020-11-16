#pragma once

#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"
#include "QAL_Reader.h"
#include "textOpt.h"
#include "settings.h"

float SHOWTOPICS_DIV = 6;
float SHOWTOPICS_FPS = 60.;
float SHOWTOPICS_SLEEP = 1000;

namespace FQuester {
	class ShowTopics {
		std::vector <SDL_Texture*> tTopics;
		uint16_t nTopic; float scale, div; 
		int buf_width, buf_height;
		sf::Clock c_fps, c_sleep;
		public:
		void init() {
			// Delete last
			for (uint16_t i = 0; i < tTopics.size(); i++) {
				SDL_DestroyTexture(tTopics[i]);
			}
			tTopics.clear();
			
			// Create
			for (uint16_t i = 0; i < FQuester::QM_Reader::topics.size(); i++) {
				tTopics.resize(tTopics.size() + 1);
				tTopics[tTopics.size() - 1] = FRenderTopic(FQuester::QM_Reader::topics[i].name, WIDTH_TEXTOUT_RENDER_1);
			}

			// Init vars
			nTopic = 0;
			scale = 0.;
			div = SHOWTOPICS_DIV;

			// Play music
			Res::aude_categories.setDiv(AUDE_DIV_PLUS);
		}
		void checkEvents() {
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) { nTopic = FQuester::QM_Reader::topics.size(); }
		}
		bool display() {
			// Update
			if (nTopic == FQuester::QM_Reader::topics.size()) return false;

			if ((!settings::s_vsync && c_fps.getElapsedTime().asMilliseconds() > (float)1000. / SHOWTOPICS_FPS) || settings::s_vsync) {
				if (c_sleep.getElapsedTime().asMilliseconds() > 1000) {
					scale += div;
					if (scale > 100) {
						scale = 100; div = -SHOWTOPICS_DIV; c_sleep.restart();
					}
					else if (scale < 0) { scale = 0; div = SHOWTOPICS_DIV; nTopic++; if (nTopic == FQuester::QM_Reader::topics.size()) return false; }
				}

				c_fps.restart();
			}

			// Display
			SDL_QueryTexture(tTopics[nTopic], NULL, NULL, &buf_width, &buf_height);
			SDL_Rect dst;
			dst.w = (float)(FGUI::sizeDisplay.y * (buf_width)) / 1080. * (scale / 100.);
			dst.h = (float)(FGUI::sizeDisplay.y * (buf_height)) / 1080. * (scale / 100.);
			dst.x = FGUI::sizeDisplay.x / 2.f - dst.w / 2.f;
			dst.y = FGUI::sizeDisplay.y / 2.f - dst.h / 2.f;
			SDL_RenderCopy(renderer, tTopics[nTopic], NULL, &dst);

			return true;
		}
	} showTopics;
}