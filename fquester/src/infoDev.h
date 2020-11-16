#pragma once

#include <SDL.h>
#include "objects.h"
#include <SFML/System.hpp>
#include <SDL_ttf.h>z
#include "res.h"
#include "settings.h"

#define INFODEV_TICK 12 // fps

class InfoDev {
	bool show;
	sf::Clock clock, clock2;
	SDL_Texture* lines[2]; SDL_Rect dst[2];
	SDL_Color c;
	SDL_Color c_bg;
	
	uint32_t framesPer;
	int frames;

	sf::Clock t_c;
	public:
	InfoDev() { 
		show = false; 
		dst[0].x = dst[0].y = 0; framesPer = 0; 
		c.r = 255; c.g = 0; c.b = 255; c.a = 255;
		c_bg.r = 0; c_bg.g = 0; c_bg.b = 0; c_bg.a = 255;
	}
	void checkEvents() {
		if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F9) { show = !show; }
	}
	void display() {
		std::string outputTxt;
		if (show && clock.getElapsedTime().asMicroseconds() > 1000000) {
			clock.restart();

			outputTxt = "FPS: "; outputTxt += std::to_string((long long)framesPer);
			if (vsyncWas) { outputTxt += " (VSYNC: on)"; } else if (n_frameLimit > 0) { outputTxt += " (Limit: "; outputTxt += std::to_string((long long)n_frameLimit); outputTxt += ")"; }
			auto serf1 = TTF_RenderText(FQuester::Res::ttf_infoDev, outputTxt.c_str(), c, c_bg);
			if (lines[0]) SDL_DestroyTexture(lines[0]);
			lines[0] = SDL_CreateTextureFromSurface(renderer, serf1);
			SDL_FreeSurface(serf1);
			dst[0].w = serf1->w; dst[0].h = serf1->h;
			framesPer = 0;
		}
		

		if (show && clock2.getElapsedTime().asMicroseconds() > 1000000. / INFODEV_TICK) {
			clock2.restart();

			outputTxt = "Frame time: "; outputTxt += std::to_string((long long)t_c.getElapsedTime().asMilliseconds()); outputTxt += " ms ";
			outputTxt += std::to_string((long long)t_c.getElapsedTime().asMicroseconds() % 10000); outputTxt += " mcs";
			auto serf2 = TTF_RenderText(FQuester::Res::ttf_infoDev, outputTxt.c_str(), c, c_bg);
			if (lines[1]) SDL_DestroyTexture(lines[1]);
			lines[1] = SDL_CreateTextureFromSurface(renderer, serf2);
			SDL_FreeSurface(serf2);
			dst[1].w = serf2->w; dst[1].h = serf2->h;
			dst[1].y = dst[0].h;
		}
		t_c.restart();

		if (show) {
			SDL_SetRenderTarget(renderer, NULL);
			SDL_RenderCopy(renderer, lines[0], NULL, &dst[0]);
			SDL_RenderCopy(renderer, lines[1], NULL, &dst[1]);
			framesPer++;
		}
	}
} infoDev;