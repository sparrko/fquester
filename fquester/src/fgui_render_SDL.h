// -------------------------------------- //
// FGUI - simple render on SDL
// (28.01.20) w1 l1
// -------------------------------------- //
#pragma once

#include <vector>
#include "fgui_window_tpdfs.h"
#include "fgui_window_iabwnd.h"
#include "fgui_objects.h"
#include "fgui_vars.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include "fgui_ftexture_SDL.h"
#include "fgui_resources.h"
#include "fgui_clsTxt.h"
#include "fgui_stsTxt.h"
#include "fgui_c_clear.h"

namespace FGUI {
	namespace Render {
		// -------------------------------------- //
		// Main
		bool Init() {
			SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_BLEND);

			return true;
		}
		void PresentFGUI() {
			SDL_SetRenderTarget(p_renderer, NULL);
			SDL_RenderCopy(p_renderer, t_display, NULL, NULL);
		}

		SDL_Surface* Text_Blended(TTF_Font* font, std::wstring text, FGUI::Color color) {
			SDL_Color bufc; bufc.r = color.r; bufc.g = color.g; bufc.b = color.b; bufc.a = color.a;
			return TTF_RenderUNICODE_Blended(font, (Uint16*)text.c_str(), bufc);
		}
		SDL_Surface* Text_Blended(TTF_Font* font, std::string text, FGUI::Color color) {
			SDL_Color bufc; bufc.r = color.r; bufc.g = color.g; bufc.b = color.b; bufc.a = color.a;
			return TTF_RenderText_Blended(font, text.c_str(), bufc);
		}
		// -------------------------------------- //
		// Inside render functions
		void SetColor(FGUI::Color color) { SDL_SetRenderDrawColor(p_renderer, color.r, color.g, color.b, color.a); }
		// -------------------------------------- //
		// For render
		struct RendererPack {
			IAbWnd* iaw;
			SDL_Texture* t_frame;
			SDL_Texture* t_center;

			void init(IAbWnd* _iaw) {
				iaw = _iaw;

				t_frame = SDL_CreateTexture(p_renderer, NULL, SDL_TEXTUREACCESS_TARGET, iaw->getSize().x + Vars::Window::borderSize * 2, iaw->getSize().y + Vars::Window::borderSize + Vars::Window::hatSize);
				SDL_SetTextureBlendMode(t_frame, SDL_BLENDMODE_BLEND);

				t_center = SDL_CreateTexture(p_renderer, NULL, SDL_TEXTUREACCESS_TARGET, iaw->getSize().x, iaw->getSize().y);
				SDL_SetTextureBlendMode(t_center, SDL_BLENDMODE_BLEND);
			}
			void deinit() {
				SDL_DestroyTexture(t_frame);
				SDL_DestroyTexture(t_center);
			}
			///////////////////////////////////////
			void updateTFrame() {
				if (t_frame) SDL_DestroyTexture(t_frame);
				t_frame = SDL_CreateTexture(p_renderer, NULL, SDL_TEXTUREACCESS_TARGET, iaw->getSize().x + Vars::Window::borderSize * 2, iaw->getSize().y + Vars::Window::borderSize + Vars::Window::hatSize);
			}
			void updateTCenter() {
				if (t_center) SDL_DestroyTexture(t_center);
				t_center = SDL_CreateTexture(p_renderer, NULL, SDL_TEXTUREACCESS_TARGET, iaw->getSize().x, iaw->getSize().y);
			}
			void renderFrame(FGUI::Render::FTexture& ft_caption, uint8_t& status_icon, FGUI::Render::FTexture& ft_icon, FGUI::Render::FTexture ft_def_icons[3]) {
				if (iaw->drawFrame())
				{
					// Create texture
					updateTFrame();
					SDL_SetRenderTarget(p_renderer, t_frame);		

					// Frame
					SDL_Rect wdr[4];			// Window draw rects
					SetColor(FGUI::Vars::Window::c_rects);
					wdr[0].x = 1; wdr[0].y = 1; wdr[0].w = iaw->getSize().x + FGUI::Vars::Window::borderSize * 2 - 2; wdr[0].h = FGUI::Vars::Window::hatSize - 2;
					wdr[1].x = iaw->getSize().x + FGUI::Vars::Window::borderSize + 1; wdr[1].y = FGUI::Vars::Window::hatSize - 1; wdr[1].w = FGUI::Vars::Window::borderSize - 2; wdr[1].h = iaw->getSize().y + FGUI::Vars::Window::borderSize;
					wdr[2].x = FGUI::Vars::Window::borderSize - 1; wdr[2].y = iaw->getSize().y + FGUI::Vars::Window::hatSize + 1; wdr[2].w = iaw->getSize().x + 2; wdr[2].h = FGUI::Vars::Window::borderSize - 2;
					wdr[3].x = 1; wdr[3].y = FGUI::Vars::Window::hatSize - 1; wdr[3].w = FGUI::Vars::Window::borderSize - 2; wdr[3].h = iaw->getSize().y + FGUI::Vars::Window::borderSize;
					SDL_RenderFillRects(p_renderer, wdr, 4);
					
					// Lines ambient
					SDL_Point wdl[3];		// Window draw lines
					SetColor(FGUI::Vars::Window::c_lightLn);
					wdl[0].x = 0; wdl[0].y = iaw->getSize().y + FGUI::Vars::Window::hatSize + FGUI::Vars::Window::borderSize - 1;
					wdl[1].x = 0; wdl[1].y = 0;
					wdl[2].x = iaw->getSize().x + FGUI::Vars::Window::borderSize * 2 - 1; wdl[2].y = 0;
					SDL_RenderDrawLines(p_renderer, wdl, 3); // light 1
					wdl[0].x = iaw->getSize().x + FGUI::Vars::Window::borderSize; wdl[0].y = FGUI::Vars::Window::hatSize - 1;
					wdl[1].x = iaw->getSize().x + FGUI::Vars::Window::borderSize; wdl[1].y = FGUI::Vars::Window::hatSize + iaw->getSize().y;
					wdl[2].x = FGUI::Vars::Window::borderSize - 1; wdl[2].y = FGUI::Vars::Window::hatSize + iaw->getSize().y;
					SDL_RenderDrawLines(p_renderer, wdl, 3); // light 2
					SetColor(FGUI::Vars::Window::c_darkLn);
					wdl[0].x = iaw->getSize().x + FGUI::Vars::Window::borderSize * 2 - 1; wdl[0].y = 1;
					wdl[1].x = iaw->getSize().x + FGUI::Vars::Window::borderSize * 2 - 1; wdl[1].y = FGUI::Vars::Window::hatSize + iaw->getSize().y + FGUI::Vars::Window::borderSize - 1;
					wdl[2].x = 1; wdl[2].y = FGUI::Vars::Window::hatSize + iaw->getSize().y + FGUI::Vars::Window::borderSize - 1;
					SDL_RenderDrawLines(p_renderer, wdl, 3); // dark 1
					wdl[0].x = FGUI::Vars::Window::borderSize - 1; wdl[0].y = FGUI::Vars::Window::hatSize + iaw->getSize().y - 1;
					wdl[1].x = FGUI::Vars::Window::borderSize - 1; wdl[1].y = FGUI::Vars::Window::hatSize - 1;
					wdl[2].x = iaw->getSize().x + FGUI::Vars::Window::borderSize - 1; wdl[2].y = FGUI::Vars::Window::hatSize - 1;
					SDL_RenderDrawLines(p_renderer, wdl, 3); // dark 2

					// Icon
					if (status_icon != 0) {
						SDL_Rect dst; dst.x = Vars::Window::borderSize; dst.y = Vars::Window::hatSize / 2 - Vars::Window::iconSize / 2; dst.w = Vars::Window::iconSize; dst.h = Vars::Window::iconSize;
						switch (status_icon) {
							case 1:
								SDL_RenderCopy(p_renderer, ft_icon.texture, NULL, &dst);
							break;
							case 2:
								SDL_RenderCopy(p_renderer, ft_def_icons[0].texture, NULL, &dst);
							break;
							case 3:
								SDL_RenderCopy(p_renderer, ft_def_icons[1].texture, NULL, &dst);
							break;
							case 4:
								SDL_RenderCopy(p_renderer, ft_def_icons[2].texture, NULL, &dst);
							break;
							case 5:
								SDL_RenderCopy(p_renderer, ft_def_icons[3].texture, NULL, &dst);
							break;
						}
					}

					// Caption
					if (ft_caption.exists()) {	
						int sizeX;

						if (status_icon != 0) { // exists icon
							if (iaw->getSize().x - (Vars::Window::iconSize + Vars::Window::buttonRetreat) < ft_caption.size.x) { // small window
								sizeX = iaw->getSize().x - (Vars::Window::iconSize + Vars::Window::buttonRetreat);
							}
							else { // else
								sizeX = ft_caption.size.x;
							}
						} 
						else { // no exisist icon
							if (iaw->getSize().x < ft_caption.size.x) sizeX = iaw->getSize().x; // 
							else sizeX = ft_caption.size.x;
						}

						SDL_Rect sr; sr.x = FGUI::Vars::Window::borderSize; if (status_icon != 0) sr.x += Vars::Window::iconSize + Vars::Window::buttonRetreat;
						sr.y = FGUI::Vars::Window::hatSize / 2 - ft_caption.size.y / 2; sr.w = sizeX; sr.h = ft_caption.size.y;

						SDL_Rect src; src.x = 0; src.y = 0; src.w = sizeX; src.h = ft_caption.size.y;
						SDL_RenderCopy(p_renderer, ft_caption.texture, &src, &sr);
					}

					// End draw
					iaw->drawFrame(false);
				}
			}
			void renderCenter() {
				if (iaw->updateCenter()) {
					updateTCenter();
					SDL_SetRenderTarget(p_renderer, t_center);
					SetColor(iaw->centerColor());
					SDL_RenderFillRect(p_renderer, NULL);
					iaw->updateCenter(false);
				}
			}
			void renderButtonsBar(uint16_t& barButtons, uint8_t& stFB, uint8_t& stSB, FGUI::Render::FTexture ft_bar[6]) {
				if (iaw->drawBar()) {
					SDL_SetRenderTarget(p_renderer, t_frame);
					SDL_Rect dst; 
					dst.x = Vars::Window::borderSize + iaw->getSize().x - Vars::Window::buttonSize; dst.y = Vars::Window::hatSize / 2 - Vars::Window::buttonSize / 2; dst.w = Vars::Window::buttonSize; dst.h = Vars::Window::buttonSize;
					switch (barButtons) {
					case WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE | WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN:
						if (stFB == 0) SDL_RenderCopy(p_renderer, ft_bar[0].texture, NULL, &dst);
						else if (stFB == 1) SDL_RenderCopy(p_renderer, ft_bar[1].texture, NULL, &dst);
						else if (stFB == 2) SDL_RenderCopy(p_renderer, ft_bar[2].texture, NULL, &dst);
						dst.x = Vars::Window::borderSize + iaw->getSize().x - Vars::Window::buttonSize * 2 - Vars::Window::buttonRetreat; dst.y = Vars::Window::hatSize / 2 - Vars::Window::buttonSize / 2; dst.w = Vars::Window::buttonSize; dst.h = Vars::Window::buttonSize;
						if (stSB == 0) SDL_RenderCopy(p_renderer, ft_bar[3].texture, NULL, &dst);
						else if (stSB == 1) SDL_RenderCopy(p_renderer, ft_bar[4].texture, NULL, &dst);
						else if (stSB == 2) SDL_RenderCopy(p_renderer, ft_bar[5].texture, NULL, &dst);
						break;
					case WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE:
						if (stFB == 0) SDL_RenderCopy(p_renderer, ft_bar[0].texture, NULL, &dst);
						else if (stFB == 1) SDL_RenderCopy(p_renderer, ft_bar[1].texture, NULL, &dst);
						else if (stFB == 2) SDL_RenderCopy(p_renderer, ft_bar[2].texture, NULL, &dst);
						break;
					case WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN:
						if (stSB == 0) SDL_RenderCopy(p_renderer, ft_bar[3].texture, NULL, &dst);
						else if (stSB == 1) SDL_RenderCopy(p_renderer, ft_bar[4].texture, NULL, &dst);
						else if (stSB == 2) SDL_RenderCopy(p_renderer, ft_bar[5].texture, NULL, &dst);
						break;
					}

					iaw->drawBar(false);
				}
			}
			///////////////////////////////////////
			// Elements
			void clearLast(Rect& lLast, FGUI::Elements::C_Clear& c_clear) {
				SDL_SetRenderTarget(p_renderer, t_center);
				SDL_Rect bufr;
				if(c_clear.getUpdated()) SetColor(c_clear.getClearColor());
				else SetColor(iaw->centerColor());
				bufr.x = lLast.x; bufr.y = lLast.y; bufr.w = lLast.w + 1; bufr.h = lLast.h + 1;
				SDL_RenderFillRect(p_renderer, &bufr);
			}

			void renderButton(Vector2i& position, Vector2u& size, FTexture& tex_text, uint8_t& status_render) {
				// Init
				SDL_SetRenderTarget(p_renderer, t_center);
				SDL_Rect bufr;
				SDL_Point bdl[3];

				// Center
				SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_NONE);
				if (status_render == 1) SetColor(FGUI::Vars::Button::c_cOver);
				else if (status_render == 2) SetColor(FGUI::Vars::Button::c_cPressed);
				else SetColor(FGUI::Vars::Button::c_cNothink);

				bufr.x = position.x; bufr.y = position.y; bufr.w = size.x; bufr.h = size.y;
				SDL_RenderFillRect(p_renderer, &bufr);

				// Shadow lines
				SDL_SetRenderDrawBlendMode(p_renderer, SDL_BLENDMODE_BLEND);
				if (status_render == 0 || status_render == 1) {
					SetColor(FGUI::Vars::Button::c_lightLn);
					bdl[0].x = position.x; bdl[0].y = position.y + size.y;
					bdl[1].x = position.x; bdl[1].y = position.y;
					bdl[2].x = position.x + size.x; bdl[2].y = position.y;
					SDL_RenderDrawLines(p_renderer, bdl, 3); // light

					SetColor(FGUI::Vars::Button::c_darkLn);
					bdl[0].x = position.x + 1; bdl[0].y = position.y + size.y;
					bdl[1].x = position.x + size.x; bdl[1].y = position.y + size.y;
					bdl[2].x = position.x + size.x; bdl[2].y = position.y + 1;
					SDL_RenderDrawLines(p_renderer, bdl, 3); // dark
				}
				else {
					SetColor(FGUI::Vars::Button::c_darkLn);
					bdl[0].x = position.x; bdl[0].y = position.y + size.y;
					bdl[1].x = position.x; bdl[1].y = position.y;
					bdl[2].x = position.x + size.x; bdl[2].y = position.y;
					SDL_RenderDrawLines(p_renderer, bdl, 3); // dark

					SetColor(FGUI::Vars::Button::c_lightLn);
					bdl[0].x = position.x + 1; bdl[0].y = position.y + size.y;
					bdl[1].x = position.x + size.x; bdl[1].y = position.y + size.y;
					bdl[2].x = position.x + size.x; bdl[2].y = position.y + 1;
					SDL_RenderDrawLines(p_renderer, bdl, 3); // light
				}

				// Text
				SDL_Rect dstSurf = { position.x + size.x / 2 - tex_text.size.x / 2, position.y + size.y / 2 - tex_text.size.y / 2, tex_text.size.x, tex_text.size.y };
				SDL_RenderCopy(p_renderer, tex_text.texture, NULL, &dstSurf);
			}

			void renderText(FTexture& tex_text, Vector2i& position) {
				// Init
				SDL_SetRenderTarget(p_renderer, t_center);
				SDL_Rect bufr;
					
				// Show text
				SDL_Rect dstSurf = { position.x, position.y, tex_text.size.x, tex_text.size.y };
				SDL_RenderCopy(p_renderer, tex_text.texture, NULL, &dstSurf);
			}

			uint16_t renderTextBox(FTexture& txt, Vector2i& position, Vector2u& size) {
				SDL_SetRenderTarget(p_renderer, t_center);
				SDL_Rect dst, src;

				SetColor(Vars::TextBox::c_center);
				dst.x = position.x;
				dst.y = position.y;
				dst.w = size.x;
				dst.h = size.y;

				SDL_RenderFillRect(p_renderer, &dst);

				SetColor(Vars::TextBox::c_darkLn);
				SDL_RenderDrawRect(p_renderer, &dst);

				if (txt.size.x < size.x - Vars::TextBox::retreatX * 2 - Vars::TextBox::retreatRight) src.x = 0; else src.x = txt.size.x - (size.x - Vars::TextBox::retreatX * 2 - Vars::TextBox::retreatRight);
				src.y = 0;
				if (txt.size.x < size.x - Vars::TextBox::retreatX * 2 - Vars::TextBox::retreatRight) src.w = txt.size.x; else src.w = size.x - Vars::TextBox::retreatX * 2 - Vars::TextBox::retreatRight;
				src.h = txt.size.y;

				dst.x = position.x + Vars::TextBox::retreatX; 
				dst.y = position.y + size.y / 2 - txt.size.y / 2 + Vars::TextBox::retreatY; 
				dst.w = src.w;
				dst.h = txt.size.y;

				SDL_RenderCopy(p_renderer, txt.texture, &src, &dst);

				return dst.w;
			}

			void renderCheckBox(FTexture& txt, Vector2i& position, bool& checked) {
				SDL_SetRenderTarget(p_renderer, t_center);
				
				SDL_Rect dst;

				dst.x = position.x;
				dst.y = position.y;
				dst.w = Vars::CheckBox::iconSize;
				dst.h = Vars::CheckBox::iconSize;

				if (!checked) SDL_RenderCopy(p_renderer, FGUI::Res::ft_cb[0].texture, NULL, &dst);
				else SDL_RenderCopy(p_renderer, FGUI::Res::ft_cb[1].texture, NULL, &dst);
				
				dst.x = position.x + Vars::CheckBox::retreatIcon + Vars::CheckBox::iconSize;
				dst.y = position.y + (Vars::CheckBox::iconSize / 2 - txt.size.y / 2);
				dst.w = txt.size.x;
				dst.h = txt.size.y;

				SDL_RenderCopy(p_renderer, txt.texture, NULL, &dst);
			}

			void renderConsoleList(Vector2i& position, Vector2u& size, std::vector <FGUI::Elements::ClsText>& texts, int &down) {
				SDL_SetRenderTarget(p_renderer, t_center);

				// Center

				SDL_Rect dst;

				dst.x = position.x; dst.y = position.y;
				dst.w = size.x; dst.h = size.y;

				SetColor(Vars::ConsoleList::c_center);
				SDL_RenderFillRect(p_renderer, &dst);

				// Ambient

				SDL_Point bdl[3];

				SetColor(FGUI::Vars::ConsoleList::c_darkLn);
				bdl[0].x = position.x; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x; bdl[1].y = position.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // dark

				SetColor(FGUI::Vars::ConsoleList::c_lightLn);
				bdl[0].x = position.x + 1; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x + size.x; bdl[1].y = position.y + size.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y + 1;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // light

				// Texts
				uint16_t mxLnUp = ceil((float)size.y / Vars::ConsoleList::heightLine);
				uint16_t upLines = 0;

				// Texts
				dst.y = position.y + Vars::List::retreatDownY + ((float)(size.y - Vars::List::retreatDownY) / texts.size() * down);
				dst.h = (int)((float)size.y / ( texts.size() + Vars::ConsoleList::retreatDownLines ) * (size.y / Vars::List::heightLine) - (Vars::List::retreatDownY*2) - 1);
				if (dst.h + dst.y > size.y - Vars::List::retreatDownY) { dst.h = size.y - dst.y; }
				if (dst.h < Vars::ConsoleList::minCursorY) dst.h = Vars::ConsoleList::minCursorY;
				if (!(dst.h > size.y - Vars::List::retreatDownY * 2)) {
					dst.x = position.x + size.x - Vars::List::retreatDownX;
					dst.y = position.y + Vars::List::retreatDownY + ((float)(size.y - Vars::List::retreatDownY) / texts.size() * down);
					dst.w = Vars::List::downSizeX;

					SetColor(Vars::List::c_down);
					SDL_RenderFillRect(p_renderer, &dst);
				}

				if (!texts.empty()) {
					SDL_Rect src;


					src.x = src.y = 0;
					dst.x = position.x + Vars::List::retreatXText;

					for (int i = 0; i < mxLnUp; i++) {
						if (i + down >= texts.size()) break;

						if (!texts[i + down].rendered) texts[i + down].render();

						dst.y = position.y + Vars::ConsoleList::retreatYText + (Vars::List::heightLine * i);
						src.h = dst.h = texts[i + down].texture.size.y;
						if (i == mxLnUp - 1) dst.h = src.h = size.y - Vars::List::retreatYText - Vars::List::heightLine * (int)((float)size.y / Vars::List::heightLine);
						if (texts[i + down].texture.size.x > (size.x - Vars::List::retreatXText - Vars::List::retreatRight)) src.w = dst.w = size.x - Vars::List::retreatXText - Vars::List::retreatRight;
						else src.w = dst.w = texts[i + down].texture.size.x;

						SDL_RenderCopy(p_renderer, texts[i + down].texture.texture, &src, &dst);
					}
				}
			}

			void renderList(Vector2i& position, Vector2u& size, uint16_t down, int& tcur, std::vector <FGUI::Elements::ClsText>& texts) {
				SDL_SetRenderTarget(p_renderer, t_center);

				// Center

				SDL_Rect dst;

				dst.x = position.x; dst.y = position.y;
				dst.w = size.x; dst.h = size.y;

				SetColor(Vars::List::c_center);
				SDL_RenderFillRect(p_renderer, &dst);

				// Ambient

				SDL_Point bdl[3];

				SetColor(FGUI::Vars::List::c_darkLn);
				bdl[0].x = position.x; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x; bdl[1].y = position.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // dark

				SetColor(FGUI::Vars::List::c_lightLn);
				bdl[0].x = position.x + 1; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x + size.x; bdl[1].y = position.y + size.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y + 1;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // light


				// Cursor
				uint16_t mxLnUp = ceil((float)size.y / Vars::List::heightLine);
				if (tcur >= 0) {
					if (tcur >= down && tcur < down + mxLnUp) {
						dst.x = position.x + 1;
						dst.w = size.x - Vars::List::retreatRight + Vars::List::retreatXText - 1;
						dst.h = Vars::List::heightLine;

						dst.y = position.y + Vars::ConsoleList::retreatYText + (Vars::List::heightLine * (tcur - down));

						if (dst.y > size.y - Vars::List::heightLine) dst.h = dst.y - size.y;

						SetColor(Vars::List::c_cursor);
						SDL_RenderFillRect(p_renderer, &dst);
					}
				}

				// Texts
				dst.h = (int)((float)size.y / texts.size() * (size.y / Vars::List::heightLine) - Vars::List::retreatDownY - 1);
				if (!(dst.h > size.y - Vars::List::retreatDownY * 2)) {
					dst.x = position.x + size.x - Vars::List::retreatDownX;
					dst.y = position.y + Vars::List::retreatDownY + ((float)(size.y - Vars::List::retreatDownY * 2) / texts.size() * down);
					dst.w = Vars::List::downSizeX;

					SetColor(Vars::List::c_down);
					SDL_RenderFillRect(p_renderer, &dst);
				}
				
				if (!texts.empty()) {
					SDL_Rect src;
					

					src.x = src.y = 0;
					dst.x = position.x + Vars::List::retreatXText;

					for (int i = 0; i < mxLnUp; i++) {
						if (i + down >= texts.size()) break;

						if (!texts[i + down].rendered) texts[i + down].render();

						dst.y = position.y + Vars::ConsoleList::retreatYText + (Vars::List::heightLine * i);
						src.h = dst.h = texts[i + down].texture.size.y;
						if (i == mxLnUp - 1) dst.h = src.h = size.y - Vars::List::retreatYText - Vars::List::heightLine*(int)((float)size.y / Vars::List::heightLine);
						if (texts[i + down].texture.size.x > (size.x - Vars::List::retreatXText - Vars::List::retreatRight)) src.w = dst.w = size.x - Vars::List::retreatXText - Vars::List::retreatRight;
						else src.w = dst.w = texts[i + down].texture.size.x;

						SDL_RenderCopy(p_renderer, texts[i + down].texture.texture, &src, &dst);
					}
				}
			}

			void renderSettingsList(Vector2i& position, Vector2u& size, uint16_t down, int& tcur, std::vector <FGUI::Elements::StsText>& texts) {
				SDL_SetRenderTarget(p_renderer, t_center);

				// Center

				SDL_Rect dst;

				dst.x = position.x; dst.y = position.y;
				dst.w = size.x; dst.h = size.y;

				SetColor(Vars::List::c_center);
				SDL_RenderFillRect(p_renderer, &dst);

				// Ambient

				SDL_Point bdl[3];

				SetColor(FGUI::Vars::SettingsList::c_darkLn);
				bdl[0].x = position.x; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x; bdl[1].y = position.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // dark

				SetColor(FGUI::Vars::SettingsList::c_lightLn);
				bdl[0].x = position.x + 1; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x + size.x; bdl[1].y = position.y + size.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y + 1;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // light


				// Cursor
				uint16_t mxLnUp = ceil((float)size.y / Vars::SettingsList::heightLine);
				if (tcur >= 0) {
					if (tcur >= down && tcur < down + mxLnUp) {
						dst.x = position.x + 1;
						dst.w = size.x - Vars::SettingsList::retreatRight + Vars::SettingsList::retreatXText - 1;
						dst.h = Vars::SettingsList::heightLine;

						dst.y = position.y + Vars::SettingsList::retreatYText + (Vars::SettingsList::heightLine * (tcur - down));

						if (dst.y > size.y - Vars::SettingsList::heightLine) dst.h = dst.y - size.y;

						SetColor(Vars::SettingsList::c_cursor);
						SDL_RenderFillRect(p_renderer, &dst);
					}
				}

				// Texts
				dst.h = (int)((float)size.y / texts.size() * (size.y / Vars::SettingsList::heightLine) - Vars::SettingsList::retreatDownY - 1);
				if (!(dst.h > size.y - Vars::SettingsList::retreatDownY * 2)) {
					dst.x = position.x + size.x - Vars::SettingsList::retreatDownX;
					dst.y = position.y + Vars::SettingsList::retreatDownY + ((float)(size.y - Vars::SettingsList::retreatDownY * 2) / texts.size() * down);
					dst.w = Vars::SettingsList::downSizeX;

					SetColor(Vars::List::c_down);
					SDL_RenderFillRect(p_renderer, &dst);
				}

				if (!texts.empty()) {
					SDL_Rect src;


					src.x = src.y = 0;
					dst.x = position.x + Vars::SettingsList::retreatXText;

					for (int i = 0; i < mxLnUp; i++) {
						if (i + down >= texts.size()) break;

						if (!texts[i + down].rendered) texts[i + down].render();

						dst.y = position.y + Vars::SettingsList::retreatYText + (Vars::SettingsList::heightLine * i);
						src.h = dst.h = texts[i + down].texture.size.y;
						if (i == mxLnUp - 1) dst.h = src.h = size.y - Vars::SettingsList::retreatYText - Vars::SettingsList::heightLine * (int)((float)size.y / Vars::SettingsList::heightLine);
						if (texts[i + down].texture.size.x > (size.x - Vars::SettingsList::retreatXText - Vars::SettingsList::retreatRight)) src.w = dst.w = size.x - Vars::SettingsList::retreatXText - Vars::SettingsList::retreatRight;
						else src.w = dst.w = texts[i + down].texture.size.x;

						SDL_RenderCopy(p_renderer, texts[i + down].texture.texture, &src, &dst);
					}
				}
			}

			void renderPanel(Vector2i &position, Vector2u &size, FGUI::Color &color, bool& convex) {
				SDL_SetRenderTarget(p_renderer, t_center);

				// Center

				SDL_Rect dst;

				dst.x = position.x; dst.y = position.y;
				dst.w = size.x; dst.h = size.y;

				SetColor(color);
				SDL_RenderFillRect(p_renderer, &dst);

				// Ambient

				SDL_Point bdl[3];

				if (convex) SetColor(FGUI::Vars::Panel::c_darkLn); else SetColor(FGUI::Vars::Panel::c_lightLn);
				bdl[0].x = position.x; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x; bdl[1].y = position.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // dark

				if (convex) SetColor(FGUI::Vars::Panel::c_lightLn); else SetColor(FGUI::Vars::Panel::c_darkLn);
				bdl[0].x = position.x + 1; bdl[0].y = position.y + size.y;
				bdl[1].x = position.x + size.x; bdl[1].y = position.y + size.y;
				bdl[2].x = position.x + size.x; bdl[2].y = position.y + 1;
				SDL_RenderDrawLines(p_renderer, bdl, 3); // light
			}

			void renderSerf(Vector2i& position, Vector2u& size, FTexture& image) {
				SDL_SetRenderTarget(p_renderer, t_center);
				
				SDL_Rect dst;
				dst.x = position.x; dst.y = position.y;
				dst.w = size.x; dst.h = size.y;

				SDL_RenderCopy(p_renderer, image.texture, NULL, &dst);
			}
		};

		void DrawFTexture(FGUI::Rect dstrect, FTexture* ftex, RendererPack* renderPack) {
			SDL_SetRenderTarget(p_renderer, renderPack->t_frame);
			SDL_Rect sr; sr.x = dstrect.x; sr.y = dstrect.y; sr.w = dstrect.w; sr.h = dstrect.h;
			SDL_RenderCopy(p_renderer, ftex->texture, NULL, &sr);
		}
		void DrawFTexture(FGUI::Rect srcrect, FGUI::Rect dstrect, FTexture* ftex, RendererPack* renderPack) {
			SDL_SetRenderTarget(p_renderer, renderPack->t_frame);
			SDL_Rect src; src.x = srcrect.x; src.y = srcrect.y; src.w = srcrect.w; src.h = srcrect.h;
			SDL_Rect sr; sr.x = dstrect.x; sr.y = dstrect.y; sr.w = dstrect.w; sr.h = dstrect.h;
			SDL_RenderCopy(p_renderer, ftex->texture, &src, &sr);
		}
		void DrawFTexture(FGUI::Rect srcrect, FGUI::Rect dstrect, FTexture* ftex) // Will draw on t_display
		{
			SDL_SetRenderTarget(p_renderer, t_display);
			SDL_Rect src; src.x = srcrect.x; src.y = srcrect.y; src.w = srcrect.w; src.h = srcrect.h;
			SDL_Rect sr; sr.x = dstrect.x; sr.y = dstrect.y; sr.w = dstrect.w; sr.h = dstrect.h;
			SDL_RenderCopy(p_renderer, ftex->texture, &src, &sr);
		}
		void DrawFTexture(Vector2i position, FTexture* ftex) // Will draw on t_display
		{
			SDL_SetRenderTarget(p_renderer, t_display);
			SDL_Rect dst;
			dst.x = position.x; dst.y = position.y; dst.w = ftex->size.x; dst.h = ftex->size.y;
			SDL_RenderCopy(p_renderer, ftex->texture, NULL, &dst);
		}

		void PresentWindow(FGUI::Render::RendererPack* rp, FGUI::Vector2i position) {
			SDL_SetRenderTarget(p_renderer, t_display);

			SDL_Rect bufr;
			bufr.x = position.x; bufr.y = position.y; bufr.w = rp->iaw->getSize().x + Vars::Window::borderSize * 2; bufr.h = rp->iaw->getSize().y + Vars::Window::borderSize + Vars::Window::hatSize;
			SDL_RenderCopy(p_renderer, rp->t_frame, NULL, &bufr);

			bufr.x = position.x + Vars::Window::borderSize; bufr.y = position.y + Vars::Window::hatSize; bufr.w = rp->iaw->getSize().x; bufr.h = rp->iaw->getSize().y;
			SDL_RenderCopy(p_renderer, rp->t_center, NULL, &bufr);
		}
		void Clear(FGUI::Color _colorClear = FGUI::Colors::Transparent) {
			SDL_SetRenderTarget(p_renderer, t_display);
			SetColor(_colorClear);
			SDL_RenderClear(p_renderer);
		}


		
	}
}