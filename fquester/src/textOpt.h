#pragma once

#include <string>
#include <stdint.h>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include "res.h"
#include "objects.h"
#include "fix.h"

uint16_t WIDTH_TEXTOUT_RENDER_0 = 1920 - 100;
uint16_t WIDTH_TEXTOUT_RENDER_1 = 1280;
uint16_t SPACE_WIDTH_TEXTOUT_RENDER = 16;
uint16_t HEIGHTLINEASK_TEXTOUT_RENDER = 90;
uint16_t HEIGHTLINETOPIC_TEXTOUT_RENDER = 190;

void reTextextr(std::wstring &in) {
	for (uint16_t i = 0; i < in.size(); i++) {
		if ((int)in[i] == 65451) in[i] = L'"';
		else if ((int)in[i] == 65467) in[i] = L'"';
		else if ((int)in[i] == 65211) in[i] = L'"';
		else if ((int)in[i] == 65195) in[i] = L'"';
	}
}

namespace FQuester {
	std::vector <std::wstring> getWords(std::wstring line) {
		std::vector <std::wstring> words;
		uint16_t mark = 0;
		reTextextr(line);
		for (uint16_t i = 0; i < line.size(); i++) {
			if (line[i] == L' ') {
				if (mark == 0) words.push_back(line.substr(0, i));
				else words.push_back(line.substr(mark, i - mark));
				mark = i + 1;
			}
			else if (i == line.size() - 1) {
				if (mark == 0) words.push_back(line.substr(0, i + 1));
				else words.push_back(line.substr(mark, i - mark + 1));
			}
		}

		return words;
	}
	std::vector <SDL_Surface*> getWordsSerf(TTF_Font* font, std::vector <std::wstring> wordsTxt) {

		std::vector <SDL_Surface*> wordsSerf;
		SDL_Color c; c.r = 222; c.g = 215; c.b = 143; c.a = 255;
		
		for (uint16_t i = 0; i < wordsTxt.size(); i++) {
			wordsSerf.push_back(TTF_RenderUNICODE_Blended(font, (Uint16*)wordsTxt[i].c_str(), c));
		}
		
		
		return wordsSerf;
	}
	std::vector <SDL_Texture*> textArr(TTF_Font* font, std::wstring line, uint16_t heightLine, uint16_t placeX) {

		auto wordsSerf = getWordsSerf(font, getWords(line));
		
		std::vector <SDL_Texture*> tLines; uint16_t numLine = 0; uint16_t retreat = 0; uint16_t mark = 0;
		SDL_Rect dst;

		for (uint16_t i = 0; i < wordsSerf.size(); i++) {
			int wid = placeX;

			int msizetec = GetWidthTexture(wordsSerf[i], renderer);
			if (SPACE_WIDTH_TEXTOUT_RENDER + msizetec + retreat > wid) {
				tLines.push_back(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, retreat, heightLine));
				SDL_SetTextureBlendMode(tLines[tLines.size() - 1], SDL_BLENDMODE_BLEND);
				SDL_SetRenderTarget(renderer, tLines[tLines.size() - 1]);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);  
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderFillRect(renderer, NULL);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

				dst.x = 0; dst.y = 0;
				for (int a = mark; a < i; a++) {
					SDL_Texture* ab;
					ab = SDL_CreateTextureFromSurface(renderer, wordsSerf[a]);
					SDL_QueryTexture(ab, NULL, NULL, &dst.w, &dst.h);
					SDL_RenderCopy(renderer, ab, NULL, &dst);
					SDL_DestroyTexture(ab);
					dst.x += dst.w + SPACE_WIDTH_TEXTOUT_RENDER;
				}
				mark = i;
				retreat = SPACE_WIDTH_TEXTOUT_RENDER + msizetec;
			}
			else {
				retreat += SPACE_WIDTH_TEXTOUT_RENDER + msizetec;///////////////////////////////////////
			}

			if (i == wordsSerf.size() - 1) {
				tLines.resize(tLines.size() + 1);
				tLines[tLines.size() - 1] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, retreat - SPACE_WIDTH_TEXTOUT_RENDER, heightLine);
				SDL_SetTextureBlendMode(tLines[tLines.size() - 1], SDL_BLENDMODE_BLEND);
				SDL_SetRenderTarget(renderer, tLines[tLines.size() - 1]);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderFillRect(renderer, NULL);
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

				dst.x = 0; dst.y = 0;
				for (int a = mark; a < wordsSerf.size(); a++) {
					SDL_Texture* ab;
					ab = SDL_CreateTextureFromSurface(renderer, wordsSerf[a]);
					dst.w = wordsSerf[a]->w;
					dst.h = wordsSerf[a]->h;
					SDL_RenderCopy(renderer, ab, NULL, &dst);
					SDL_DestroyTexture(ab);
					dst.x += wordsSerf[a]->w + SPACE_WIDTH_TEXTOUT_RENDER;
				}
				mark = i;
				retreat = SPACE_WIDTH_TEXTOUT_RENDER + wordsSerf[i]->w;
			}
		}

		SDL_SetRenderTarget(renderer, NULL);

		// Clear serf buffer
		for (int a = 0; a < wordsSerf.size(); a++) {
			SDL_FreeSurface(wordsSerf[a]);
		}
		wordsSerf.clear();


		return tLines;
	}

	// Useble
	SDL_Texture* FRenderAsk(std::wstring line, uint16_t placeX) {
		// Init
		auto tLines = textArr(Res::ttf_ask, line, HEIGHTLINEASK_TEXTOUT_RENDER, placeX);

		// Count size texture
		uint16_t tWidth = 0;
		uint16_t tHeight = 0;
		for (int i = 0; i < tLines.size(); i++) {
			int buf_width; SDL_QueryTexture(tLines[i], NULL, NULL, &buf_width, NULL);
			if (buf_width > tWidth) tWidth = buf_width;
		}
		tHeight = HEIGHTLINEASK_TEXTOUT_RENDER * tLines.size();

		// Create texture
		SDL_Texture* t_display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tWidth, tHeight);
		SDL_SetTextureBlendMode(t_display, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer, t_display);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		// Output
		SDL_Rect dst; dst.y = 0;
		for (int i = 0; i < tLines.size(); i++) {
			int buf_width, buf_height; SDL_QueryTexture(tLines[i], NULL, NULL, &buf_width, &buf_height);
			dst.x = tWidth / 2.f - buf_width / 2.f;
			dst.y = HEIGHTLINEASK_TEXTOUT_RENDER * i;
			dst.w = buf_width; dst.h = buf_height;

			SDL_RenderCopy(renderer, tLines[i], NULL, &dst);
		}

		// End
		for (int a = 0; a < tLines.size(); a++) {
			SDL_DestroyTexture(tLines[a]);
		}
		tLines.clear();

		SDL_SetRenderTarget(renderer, NULL);


		return t_display;
	}

	SDL_Texture* FRenderTopic(std::wstring topic, uint16_t placeX) {
		// Init
		auto tLines = textArr(Res::ttf_topic, topic, HEIGHTLINETOPIC_TEXTOUT_RENDER, placeX);

		// Count size texture
		uint16_t tWidth = 0;
		uint16_t tHeight = 0;
		for (int i = 0; i < tLines.size(); i++) {
			int buf_width; SDL_QueryTexture(tLines[i], NULL, NULL, &buf_width, NULL);
			if (buf_width > tWidth) tWidth = buf_width;
		}
		tHeight = HEIGHTLINETOPIC_TEXTOUT_RENDER * tLines.size();

		// Create texture
		SDL_Texture* t_display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, tWidth, tHeight);
		SDL_SetTextureBlendMode(t_display, SDL_BLENDMODE_BLEND);
		SDL_SetRenderTarget(renderer, t_display);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); SDL_RenderFillRect(renderer, NULL);
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		// Output
		SDL_Rect dst; dst.y = 0;
		for (int i = 0; i < tLines.size(); i++) {
			int buf_width, buf_height; SDL_QueryTexture(tLines[i], NULL, NULL, &buf_width, &buf_height);
			dst.x = tWidth / 2.f - buf_width / 2.f;
			dst.y = HEIGHTLINETOPIC_TEXTOUT_RENDER * i;
			dst.w = buf_width; dst.h = buf_height;

			SDL_RenderCopy(renderer, tLines[i], NULL, &dst);
		}

		// End
		for (int a = 0; a < tLines.size(); a++) {
			SDL_DestroyTexture(tLines[a]);
		}
		tLines.clear();

		SDL_SetRenderTarget(renderer, NULL);
		return t_display;
	}
}