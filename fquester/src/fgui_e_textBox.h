#pragma once

#include <string>
#include "fgui_element.h"
#include "fgui_objects.h"
#include "fgui_resources.h"
#include "fgui_window_iabwnd.h"
#include "fgui_render.h"
#include "fgui_timer.h"
#include "fgui_cursorTextBox.h"

#include "fgui_window_call.h"
#include "fgui_objects.h"

#include "res.h"

namespace FGUI {
	namespace Elements {
		Element* p_ac_textBox;

		class TextBox : public Element {
			bool firstSize;
			bool onlyNum;
			uint16_t textLenghtMax;
			uint16_t moveCursor;
			FGUI::Render::FTexture t_text; 
			std::wstring text; Color cText;
			bool enterPressed;
			bool callSetCursor;
			void drawText() {
				SDL_Surface* text_serf = FGUI::Render::Text_Blended(Res::ttf_ubuntu_textBox, text, cText);
				t_text.init(text_serf);
				SDL_FreeSurface(text_serf);
				mr = true;
			}
		public:
			TextBox() { moveCursor = 0; textLenghtMax = 0; }
			~TextBox() {  }
			void setColor(FGUI::Color color) {
				cText = color;
			}
			void setText(std::wstring _text) {
				if (text != _text) {
					text = _text;
					SDL_Surface* text_serf = FGUI::Render::Text_Blended(Res::ttf_ubuntu_textBox, text, cText);
					t_text.init(text_serf);
					SDL_FreeSurface(text_serf);
					mr = true;
				}
			}
			std::wstring getText() {
				return text;
			}
			void setSize(uint32_t _sizeX) {
				if (!hide) {
					lLast.x = position.x; lLast.y = position.y;
					if (firstSize) { lLast.w = size.x = _sizeX; lLast.h = size.y = Vars::TextBox::sizeY; firstSize = false; }
					else { lLast.w = size.x; size.x = _sizeX; }
					mr = mc = true;
				}
				size.x = _sizeX;
			}
			void clear() {
				if (!text.empty()) {
					text = L"";
					SDL_Surface* text_serf = FGUI::Render::Text_Blended(Res::ttf_ubuntu_textBox, L" ", cText);
					t_text.init(text_serf);
					SDL_FreeSurface(text_serf);
					mr = true;
				}
			}
			void setMaxChars(uint16_t _max) {
				textLenghtMax = _max + 1;
			}
			void setInputOnlyNum(bool _type) {
				onlyNum = _type;
			}
			void setCursor() {
				callSetCursor = true;
			}
			bool getActive() {
				if (p_ac_textBox == this) return true;
				else return false;
			}

			///////////////////

			void init(FGUI::Vector2i _position, uint32_t _sizeX) {
				firstSize = true; //////////////////////////////////////////////////////////////////////////////
				firstPos = true; //////////////////////////////////////////////////////////////////////////////
				hide = false; //////////////////////////////////////////////////////////////

				setText(L"");
				setPosition(_position);
				setSize(_sizeX);
				setColor(FGUI::Vars::TextBox::c_text);

				onlyNum = false;

				callSetCursor = false;
				mr = mc = true; 
			}
			void init() {
				firstSize = true; //////////////////////////////////////////////////////////////////////////////
				firstPos = true; //////////////////////////////////////////////////////////////////////////////
				hide = false; //////////////////////////////////////////////////////////////

				setColor(FGUI::Vars::TextBox::c_text);

				onlyNum = false;

				callSetCursor = false;
				mr = mc = true;
			}
			void checkEvents(FGUI::IAbWnd& iaw) override {
				if (!hide) {
					if (!checkBoxFinded) {

						enterPressed = false;

						FGUI::Rect rch = Rect(iaw.getPosition().x + Vars::Window::borderSize + position.x, iaw.getPosition().y + Vars::Window::hatSize + position.y, size.x, size.y);

						if (callSetCursor) {
							p_ac_textBox = this;
							checkBoxFinded = true;
							callSetCursor = false;
						}
						// 48 - 57
						if (limitRectCheck(rch, iaw)) {
							if (FGUI::CheckRect(rch, cursor.getPosition())) {
								if (p_event->type == SDL_MOUSEBUTTONDOWN && p_event->button.button == SDL_BUTTON_LEFT) {
									p_ac_textBox = this;
									checkBoxFinded = true;
								}
								cursor.setCondition(FGUI::CURSOR_REDACTOR);
							}
							else {
								if (p_event->type == SDL_MOUSEBUTTONDOWN && p_event->button.button == SDL_BUTTON_LEFT) {
									p_ac_textBox = NULL;
								}
							}

							if (p_ac_textBox == this) {
								if (!inputText.empty()) {
									if (inputText == L"$/del") { if (text.size() != 0) text.resize(text.size() - 1); }
									else if (inputText == L"$/cdel") {
										if (text.size() != 0) {
											Uint16 from = 0;
											if (text[text.length() - 1] == ' ') {
												for (int a = text.length() - 1; a > -1; a--) {
													if (text[a] != ' ') break;
													else from = a;
												}
											}
											else {
												for (int a = 0; a < text.length(); ++a) {
													if (text[a] == ' ') from = a;
												}
											}
											text.erase(from, text.size());
										}
									}
									else if (inputText == L"$/enter") {
										enterPressed = true;
									}
									//if (onlyNum && text.length() == 1) { if (text[0] >= 48 && text[0] <= 57) text += inputText; else cursorText.wrongInput(); }
									//else if (!onlyNum)
									else if ((onlyNum && inputText.length() == 1) || !onlyNum) {
										if (text.length() + FGUI::inputText.length() < textLenghtMax) if (onlyNum) { if (inputText[0] >= 48 && inputText[0] <= 57) text += inputText; else cursorText.wrongInput(); } else text += inputText;
										else if (textLenghtMax == 0 || !onlyNum) { if (text.length() + FGUI::inputText.length() < Vars::TextBox::maxChars) text += inputText; else cursorText.wrongInput(); }
										else cursorText.wrongInput();
									}
									else cursorText.wrongInput();
									FGUI::inputText.clear();
									drawText();
								}
								if (!text.empty()) cursorText.setPosition(FGUI::Vector2i(iaw.getPosition().x + Vars::Window::borderSize + position.x + moveCursor + Vars::TextBox::retreatCursor, iaw.getPosition().y + Vars::Window::hatSize + position.y));
								else cursorText.setPosition(FGUI::Vector2i(iaw.getPosition().x + Vars::Window::borderSize + position.x + Vars::TextBox::retreatCursor, iaw.getPosition().y + Vars::Window::hatSize + position.y));
							}

						}
					}
				}
			}
			void display(FGUI::Render::RendererPack& rp) override {
				if (mc) {
					rp.clearLast(lLast, c_clear);
					mc = false;
				}
				if (mr) {
					moveCursor = rp.renderTextBox(t_text, position, size);
					mr = false;
				}
			}
			bool getPressedEnter() {
				return enterPressed;
			}

			//////////////////
		};

		TextBox* CreateTextBox(FGUI::Vector2i _position, uint32_t _sizeX) {
			TextBox* bufElement = new TextBox;
			bufElement->init(_position, _sizeX);
			return bufElement;
		}
		TextBox* CreateTextBox() {
			TextBox* bufElement = new TextBox;
			bufElement->init();
			return bufElement;
		}
	}
}