#pragma once

#include <string>
#include "fgui_element.h"
#include "fgui_objects.h"
#include "fgui_resources.h"
#include "fgui_window_iabwnd.h"
#include "fgui_render.h"
#include "fgui_clsTxt.h"



namespace FGUI {
	namespace Elements {
		class List : public Element {
			bool firstSize;

			int down;
			std::vector <FGUI::Elements::ClsText> texts;

			int tcur; bool pressed;
		public:
			
			/////////////////////////////
			void changeText(uint16_t pos, std::wstring name) {
				texts[pos].setText(name);
				draw();
			}
			int getCursor() {
				if (tcur >= 0) return tcur;
				else return 0;
			}
			int getCursorD() {
				return tcur;
			}
			void setCursor(int pos) {
				if (pos < 0) pos = 0;
				else if (pos > texts.size() - 1) pos = texts.size() - 1;
				tcur = pos;
				draw();
			}
			void setCursorD(int pos) {
				tcur = pos;
				draw();
			}
			std::vector <FGUI::Elements::ClsText> *getTexts() {
				return &texts;
			}
			double getLimitDown() {
				if ((texts.size() - int(size.y / Vars::List::heightLine)) < .0) return .0;
				else return (texts.size() - int(size.y / Vars::List::heightLine));
			}
			std::wstring getText(uint16_t _num) {
				return texts[_num].text;
			}
			void setDown(double _down) {
				if (_down > getLimitDown()) down = getLimitDown();
				else if (_down < 0) _down = 0;
				else down = _down;
			}
			void clear() {
				texts.clear();
				setDown(0);
				draw();
			}
			void deleteLine(uint16_t pos) {
				texts.erase(texts.begin() + pos);
				
				if (texts.size() > 0) {
					setDown(0);
					if (getCursor() == texts.size()) setCursor(getCursor() - 1);
					if (getCursor() == 0 && texts.size() == 0) setCursorD(-1);
				}
				else {
					down = 0;
				}

				draw();
			}
			void setSize(Vector2u _size) {
				if (firstSize) { lLast.w = _size.x; lLast.h = _size.y; firstSize = false; }
				else { lLast.w = size.x; lLast.h = size.y; }
				size = _size;

				if (!hide) mr = mc = true;
			}
			void addText(std::wstring _text) {
				if (texts.size() < Vars::List::maxLines) {
					texts.resize(texts.size() + 1);
					texts[texts.size() - 1].setText(_text);
				}
				else {
					for (int i = 1; i < Vars::List::maxLines; i++) {
						std::swap(texts[i - 1], texts[i]);
					}
					texts[Vars::List::maxLines - 1].setText(_text);
				}
				if (!hide) mr = true;
			}
			///////////////////
			void init(FGUI::Vector2i _position, FGUI::Vector2u _size) {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true; //////////////////////////////////////////////////////////////////
				hide = false; //////////////////////////////////////////////////////////////////

				tcur = -1; down = 0;
				texts.clear();
				setPosition(_position);
				setSize(_size);

				mr = mc = true;
			}
			void checkEvents(FGUI::IAbWnd & iaw) override {
				FGUI::Rect rch = Rect(iaw.getPosition().x + Vars::Window::borderSize + position.x, iaw.getPosition().y + Vars::Window::hatSize + position.y, size.x, size.y);

				if (limitRectCheck(rch, iaw)) {
					if (FGUI::CheckRect(rch, cursor.getPosition())) {
						if (p_event->type == SDL_MOUSEBUTTONDOWN && p_event->button.button == SDL_BUTTON_LEFT) {
							if (texts.size() > 0) {
								int temp = int((float)(cursor.getPosition().y - iaw.getPosition().y - position.y + Vars::List::retreatYText) / Vars::List::heightLine);
								temp = down + temp - 2;
								if (temp >= texts.size()) temp = texts.size() - 1;
								if (temp < 0) temp = 0;
								setCursor(temp);
								draw();
							}
						}
						if (p_event->type == SDL_MOUSEWHEEL)
						{
							if (p_event->wheel.y > 0) // scroll up
							{
								down -= Vars::mouseWhellSpeed;
								if (down < 0) down = 0;
							}
							else if (p_event->wheel.y < 0) // scroll down
							{
								if (!(texts.size() <= (int)((float)size.y / Vars::List::heightLine))) {
									down += Vars::mouseWhellSpeed;
									if (down > getLimitDown()) down = getLimitDown();
								}
							}
							draw();
						}
						if (p_event->type == SDL_KEYDOWN && !pressed) {
							if (FGUI::p_event->key.keysym.sym == SDLK_DOWN) {
								if (tcur < 0) tcur = texts.size() - 1;
								else {
									tcur++;
									if (tcur >= texts.size()) tcur = texts.size() - 1;
								}
							}
							else if (FGUI::p_event->key.keysym.sym == SDLK_UP) {
								if (tcur < 0) tcur = 0;
								else {
									tcur--;
									if (tcur < 0) tcur = 0;
								}
							}
							draw();
							pressed = true;
						}
						if (p_event->type == SDL_KEYUP) {
							if (FGUI::p_event->key.keysym.sym == SDLK_DOWN || FGUI::p_event->key.keysym.sym == SDLK_UP) {
								pressed = false;
							}
						}
					}
				}
			}
			void display(FGUI::Render::RendererPack & rp) override {
				if (mc) {
					rp.clearLast(lLast, c_clear);
					mc = false;
				}
				if (mr) {
					rp.renderList(position, size, down, tcur, texts);
					mr = false;
				}
			}

			//////////////////
		};

		List* CreateList(FGUI::Vector2i _position, FGUI::Vector2u _size) {
			List* bufElement = new List;
			bufElement->init(_position, _size);
			return bufElement;
		}
	}
}