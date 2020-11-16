#pragma once

#include "res.h"
#include "fgui_cursor.h"
#include "fgui_main.h"

namespace FQuester {
	struct Redactor {
		sf::Clock wait;
		sf::Clock time; SDL_Texture* t;

		SDL_Texture* t_f8; SDL_Rect r_t_f8;

		int lastn;
		Redactor() {
		}
		void Init() {
			fw_startWindow.fwin->activate();
			Res::aude_menu.setDiv(AUDE_DIV_MINUS);
			FGUI::NewSizeWindow();
			if (devMode) Res::aude_serf.setDiv(AUDE_DIV_PLUS);
			wait.restart();
			lastn = 0;

			if (!t_f8) {
				SDL_Color c; c.r = c.g = c.b = c.a = 255;
				std::wstring outputTxt = L"Для смены полноэкранного режима: F8";
				auto serf = TTF_RenderUNICODE_Blended(FQuester::Res::ttf_u_menu_footer, (Uint16*)outputTxt.c_str(), c);
				t_f8 = SDL_CreateTextureFromSurface(renderer, serf);
				
				r_t_f8.w = serf->w;
				r_t_f8.h = serf->h;
				r_t_f8.x = 5; r_t_f8.y = 5;
				
				SDL_FreeSurface(serf);
			}
		}
		void PreUpdate() {

		}
		void CheckEvents() {
			
		}
		void PostUpdate() {
		}
		void Display() {
			
			time_t seconds = std::time(NULL);
			struct tm* t_m;
			t_m = localtime(&seconds);
			

			std::string outputTxt = "Time now: ";
			if (t_m->tm_hour == 0) outputTxt += "00";
			else if (t_m->tm_hour < 10) { outputTxt += "0"; outputTxt += std::to_string((long long)t_m->tm_hour); }
			else outputTxt += std::to_string((long long)t_m->tm_hour);
			outputTxt += ":";
			if (t_m->tm_min == 0) outputTxt += "00";
			else if (t_m->tm_min < 10) { outputTxt += "0"; outputTxt += std::to_string((long long)t_m->tm_min); }
			else outputTxt += std::to_string((long long)t_m->tm_min);
			outputTxt += ":";
			if (t_m->tm_sec == 0) outputTxt += "00";
			else if (t_m->tm_sec < 10) { outputTxt += "0"; outputTxt += std::to_string((long long)t_m->tm_sec); }
			else outputTxt += std::to_string((long long)t_m->tm_sec);
			outputTxt += ".";

			uint64_t buft = time.getElapsedTime().asMicroseconds() % 1000000;

			if (buft < 10) { outputTxt += "00000"; outputTxt += std::to_string((long long)buft); }
			else if (buft < 100) { outputTxt += "0000"; outputTxt += std::to_string((long long)buft); }
			else if (buft < 1000) { outputTxt += "000"; outputTxt += std::to_string((long long)buft); }
			else if (buft < 10000) { outputTxt += "00"; outputTxt += std::to_string((long long)buft); }
			else if (buft < 100000) { outputTxt += "0"; outputTxt += std::to_string((long long)buft); }
			else if (buft < 1000000) { outputTxt += std::to_string((long long)buft); }
			else outputTxt += "999999";

			if (lastn != t_m->tm_sec) { lastn = t_m->tm_sec; time.restart(); }

			SDL_Color c; c.r = c.g = c.b = c.a = 255;
			auto serf = TTF_RenderText_Blended(FQuester::Res::ttf_u_menu_footer, outputTxt.c_str(), c);
			if (t) SDL_DestroyTexture(t);
			t = SDL_CreateTextureFromSurface(renderer, serf);
			SDL_FreeSurface(serf);
			SDL_Rect dst; 
			dst.w = serf->w; dst.h = serf->h;
			dst.x = FGUI::sizeDisplay.x - dst.w - 5; dst.y = FGUI::sizeDisplay.y - dst.h - 5;

			SDL_SetRenderTarget(renderer, NULL); SDL_SetRenderDrawColor(renderer, 30, 0, 30, 255); SDL_RenderFillRect(renderer, NULL); SDL_RenderCopy(renderer, t, NULL, &dst);

			SDL_RenderCopy(renderer, t_f8, NULL, &r_t_f8);
		}
	} q_redactor;
}
