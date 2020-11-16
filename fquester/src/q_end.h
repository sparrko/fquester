#pragma once

#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"

int SCEND_RETREAT = 100;
int SCEND_WIDTH = 200;
int SCEND_HEIGHT = 700;
int SCEND_FPS = 80;
float SCEND_SPEED = 0.6;
int SCEND_OUTLINE = 3;

int SCEND_BUTTONRETREAT_X = -20;
int SCEND_BUTTONRETREAT_Y = -10;

int SCEND_LINESPEED = 3;
int SCEND_LINEMAX = 1500;

namespace FQuester {
	class ScEnd {
	public:
		sf::Clock c_fps;

		bool start;
		int mode;
		int part;

		SDL_Texture* t_button;
		SDL_Rect r_button;

		SDL_Texture* t_points[3]; SDL_Rect r_points[3];

		int sizeLine;
		
		SDL_Rect r_f; SDL_Rect r_s; SDL_Rect r_t;
		SDL_Color cl_f; SDL_Color cl_s; SDL_Color cl_t;
		int ppoints[3];
		int nRect;
		float height;

		bool pressed;
		bool onButton;

		void init() {
			start = true;
			mode = part = 0;

			nRect = height = 0;

			onButton = false;
			pressed = false;
			

		}
		void checkEvents() {
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				mInitMenu = true;
				FQuester::QM_Reader::Clear();
				Res::aud_result.stop();
				Res::aud_high.stop();
				Res::aud_plus.stop();
			}
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
				pressed = true;
			}
		}
		void display() {
			if (mode == 0) {
				if (start) { 

					ppoints[0] = ppoints[1] = ppoints[2] = 0;

					sizeLine = 0;

					SDL_Color tcl; tcl.r = tcl.g = tcl.b = 0;
					cl_f = cl_s = cl_t = tcl;
					SDL_Rect trt; trt.x = trt.y = trt.w = trt.h = 0;
					r_f = r_s = r_t = trt;

					bool inited[3]; inited[0] = false; inited[1] = false; inited[2] = false;

					if (points[0] > points[1] && points[0] > points[2]) { cl_f.r = 255; cl_f.g = 0; cl_f.b = 0; inited[0] = true; ppoints[1] = points[0]; }
					else if (points[1] > points[0] && points[1] > points[2]) { cl_f.r = 0; cl_f.g = 255; cl_f.b = 0; inited[1] = true; ppoints[1] = points[1]; }
					else if (points[2] > points[0] && points[2] > points[1]) { cl_f.r = 0; cl_f.g = 0; cl_f.b = 255; inited[2] = true; ppoints[1] = points[2]; }

					if (points[0] < points[1] && points[0] < points[2]) { cl_t.r = 255; cl_t.g = 0; cl_t.b = 0; inited[0] = true; ppoints[2] = points[0]; }
					else if (points[1] < points[0] && points[1] < points[2]) { cl_t.r = 0; cl_t.g = 255; cl_t.b = 0; inited[1] = true; ppoints[2] = points[1]; }
					else if (points[2] < points[0] && points[2] < points[1]) { cl_t.r = 0; cl_t.g = 0; cl_t.b = 255; inited[2] = true; ppoints[2] = points[2]; }

					if (!inited[0]) { cl_s.r = 255; cl_s.g = 0; cl_s.b = 0; ppoints[0] = points[0]; }
					else if (!inited[1]) { cl_s.r = 0; cl_s.g = 255; cl_s.b = 0; ppoints[0] = points[1]; }
					else if (!inited[2]) { cl_s.r = 0; cl_s.g = 0; cl_s.b = 255; ppoints[0] = points[2]; }

					sc.addDraw(ppoints[1]);
					sc.addDraw(ppoints[0]);
					sc.addDraw(ppoints[2]);

					shadow.set(0, 3); 
					start = !start; 

					// Render
					TTF_SetFontOutline(Res::ttf_tpoints_out, SCEND_OUTLINE);
					SDL_Surface* bufserf;
					SDL_Color white; white.r = 222; white.g = 215; white.b = 143; white.a = 255;
					SDL_Color black; black.r = black.g = black.b = 40; black.a = 255;
					for (int a = 0; a < 3; a++) {
						int str;

						if (a == 0) str = ppoints[1];
						else if (a == 1) str = ppoints[0];
						else if (a == 2) str = ppoints[2];

						SDL_Surface* bg_surface = TTF_RenderText_Blended(Res::ttf_tpoints_out, std::to_string((long long)str).c_str(), black);
						SDL_Surface* fg_surface = TTF_RenderText_Blended(Res::ttf_tpoints, std::to_string((long long)str).c_str(), white);
						SDL_Rect rect; rect.x = SCEND_OUTLINE;
						rect.y = SCEND_OUTLINE;
						rect.w = fg_surface->w;
						rect.h = fg_surface->h;

						SDL_SetSurfaceBlendMode(fg_surface, SDL_BLENDMODE_BLEND);
						SDL_BlitSurface(fg_surface, NULL, bg_surface, &rect);
						SDL_FreeSurface(fg_surface);

						if (t_points[a]) SDL_DestroyTexture(t_points[a]);

						t_points[a] = SDL_CreateTextureFromSurface(renderer, bg_surface);
						r_points[a].w = bg_surface->w;
						r_points[a].h = bg_surface->h;
						SDL_FreeSurface(bg_surface);
					}

					TTF_SetFontOutline(Res::ttf_endButtons_out, SCEND_OUTLINE);
					SDL_Surface* bg_surface = TTF_RenderUNICODE_Blended(Res::ttf_endButtons_out, (Uint16*)L"Выход", black);
					SDL_Surface* fg_surface = TTF_RenderUNICODE_Blended(Res::ttf_endButtons, (Uint16*)L"Выход", white);
					SDL_Rect rect;
					rect.x = SCEND_OUTLINE;
					rect.y = SCEND_OUTLINE;
					rect.w = fg_surface->w;
					rect.h = fg_surface->h;

					SDL_SetSurfaceBlendMode(fg_surface, SDL_BLENDMODE_BLEND);
					SDL_BlitSurface(fg_surface, NULL, bg_surface, &rect);
					SDL_FreeSurface(fg_surface);

					if (t_button) SDL_DestroyTexture(t_button);
					t_button = SDL_CreateTextureFromSurface(renderer, bg_surface);
					r_button.w = bg_surface->w;
					r_button.h = bg_surface->h;
					r_button.x = FGUI::sizeDisplay.x - r_button.w + SCEND_BUTTONRETREAT_X;
					r_button.y = FGUI::sizeDisplay.y - r_button.h + SCEND_BUTTONRETREAT_Y;
					SDL_FreeSurface(bg_surface);

					Res::aud_result.play();
				}
				if (shadow.shadowPer == 255 && mode < 1) { 
					mode = 1;
					fullIgnoreUpdateBg = true;
					shadow.set(0, -3);
					SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(renderer, NULL);
				}
			}
			else if (mode == 1) {
				// Check button
				if (CheckRect(r_button, cursor.getPosition())) {
					onButton = true;
					if (pressed) {
						Res::aud_result.stop();
						Res::aud_high.stop();
						Res::aud_plus.stop();

						pressed = false;
						mInitMenu = true;
						FQuester::QM_Reader::Clear();
					}
				}
				else {
					onButton = false;
					pressed = false;
				}


				// Draw
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, NULL);

				if ((!settings::s_vsync && c_fps.getElapsedTime().asMilliseconds() > (float)1000. / SCEND_FPS) || settings::s_vsync) {
					c_fps.restart();

					sizeLine += SCEND_LINESPEED; if (sizeLine > SCEND_LINEMAX) sizeLine = SCEND_LINEMAX;

					if (nRect != 3) { 
						height += SCEND_SPEED; 
						if (Res::aud_plus.getStatus() != sf::Sound::Status::Playing) Res::aud_plus.play(); 
					}
					else {
						Res::aud_plus.stop();
					}

					if (height > 255) { nRect++; height = 0; Res::aud_high.play(); }

					if (nRect == 0) {
						if (height > 255) { height = 255; }
						r_f.w = FGUI::sizeDisplay.x / 1920. * SCEND_WIDTH;
						r_f.h = FGUI::sizeDisplay.x / 1920. * SCEND_HEIGHT * (height / 255.);
						r_f.x = FGUI::sizeDisplay.x / 2 - r_f.w / 2;
						r_f.y = FGUI::sizeDisplay.y / 2 - r_f.h / 2;

						r_points[0].x = FGUI::sizeDisplay.x / 2 - r_points[0].w / 2;
						r_points[0].y = r_f.y + r_f.h;
					}
					else if (nRect == 1) {
						if (height > 255) { height = 255; }
						r_s.w = FGUI::sizeDisplay.x / 1920. * SCEND_WIDTH;
						r_s.h = (FGUI::sizeDisplay.x / 1920. * SCEND_HEIGHT) * ((float)(height / 255.) * ((float)ppoints[0] / ppoints[1]));
						r_s.x = FGUI::sizeDisplay.x / 2 - r_s.w / 2 - r_s.w - SCEND_RETREAT;
						r_s.y = r_f.y + r_f.h - r_s.h;

						r_points[1].x = FGUI::sizeDisplay.x / 2 - r_s.w - SCEND_RETREAT - r_points[1].w / 2;
						r_points[1].y = r_f.y + r_f.h;
					}
					else if (nRect == 2) {
						if (height > 255) { height = 255; }
						r_t.w = FGUI::sizeDisplay.x / 1920. * SCEND_WIDTH;
						r_t.h = (FGUI::sizeDisplay.x / 1920. * SCEND_HEIGHT) * ((float)(height / 255.) * ((float)ppoints[2] / ppoints[1]));
						r_t.x = FGUI::sizeDisplay.x / 2 + r_t.w / 2 + SCEND_RETREAT;
						r_t.y = r_f.y + r_f.h - r_t.h;

						r_points[2].x = FGUI::sizeDisplay.x / 2 + r_s.w + SCEND_RETREAT - r_points[2].w / 2;
						r_points[2].y = r_f.y + r_f.h;
					}
				}


				SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
				int width = (FGUI::sizeDisplay.x / 1920. * sizeLine);
				SDL_RenderDrawLine(renderer, (r_points[0].x + r_points[0].w / 2) - width / 2, r_points[0].y, (r_points[0].x + r_points[0].w / 2) + width / 2, r_points[0].y);

				SDL_SetRenderDrawColor(renderer, cl_f.r, cl_f.g, cl_f.b, 255);
				SDL_RenderFillRect(renderer, &r_f);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(renderer, &r_f);
				if (nRect > 0) { SDL_RenderCopy(renderer, t_points[0], NULL, &r_points[0]); }

				SDL_SetRenderDrawColor(renderer, cl_s.r, cl_s.g, cl_s.b, 255);
				SDL_RenderFillRect(renderer, &r_s);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(renderer, &r_s);
				if (nRect > 1) { SDL_RenderCopy(renderer, t_points[1], NULL, &r_points[1]); }

				SDL_SetRenderDrawColor(renderer, cl_t.r, cl_t.g, cl_t.b, 255);
				SDL_RenderFillRect(renderer, &r_t);
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderDrawRect(renderer, &r_t);
				if (nRect > 2) { SDL_RenderCopy(renderer, t_points[2], NULL, &r_points[2]); }

				if (onButton) SDL_SetTextureColorMod(t_button, 255, 255, 255);
				else SDL_SetTextureColorMod(t_button, 180, 180, 180);
				if (nRect > 2) { SDL_RenderCopy(renderer, t_button, NULL, &r_button); }
			}
		}
	} scEnd;
}