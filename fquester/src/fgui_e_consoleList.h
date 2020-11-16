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
		class ConsoleList : public Element {
			bool firstSize;
			int down;

			std::vector <FGUI::Elements::ClsText> texts;
		public:
			/////////////////////////////
			void cls() {
				texts.clear();
				down = 0;
			}
			void setDown(double _down) {
				if (_down > getLimitDown()) down = getLimitDown();
				else if (_down < 0) _down = 0;
				else down = _down;
			}
			void setSize(Vector2u _size) {
				if (firstSize) { lLast.w = _size.x; lLast.h = _size.y; firstSize = false; }
				else { lLast.w = size.x; lLast.h = size.y; }
				size = _size;

				if (!hide) mr = mc = true;
			}
			void addText(std::wstring _text) {
				if (texts.size() < Vars::ConsoleList::maxLines) {
					texts.resize(texts.size() + 1);
					texts[texts.size() - 1].setText(_text);
				}
				else {
					for (int i = 1; i < Vars::ConsoleList::maxLines; i++) {
						std::swap(texts[i - 1], texts[i]);
					}
					texts[Vars::ConsoleList::maxLines - 1].setText(_text);
				}
				if (!hide) mr = true;

				down = getLimitDown();
			}
			void addText(std::string text) {
				std::wstring _text(text.begin(), text.end());
				
				if (texts.size() < Vars::ConsoleList::maxLines) {
					texts.resize(texts.size() + 1);
					texts[texts.size() - 1].setText(_text);
				}
				else {
					for (int i = 1; i < Vars::ConsoleList::maxLines; i++) {
						std::swap(texts[i - 1], texts[i]);
					}
					texts[Vars::ConsoleList::maxLines - 1].setText(_text);
				}
				if (!hide) mr = true;

				down = getLimitDown();
			}
			int getLimitDown() {
				int a = texts.size() - int(size.y / Vars::ConsoleList::heightLine) + Vars::ConsoleList::retreatDownLines;
				if (a < 0) a = 0;
				return a;
			}
			std::wstring getText(uint16_t _num) {
				return texts[_num].text;
			}
			std::vector <FGUI::Elements::ClsText>* getTexts() {
				return &texts;
			}
			///////////////////
			void init(FGUI::Vector2i _position, FGUI::Vector2u _size) {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true; //////////////////////////////////////////////////////////////////
				hide = false; //////////////////////////////////////////////////////////////////

				texts.clear();
				setPosition(_position);
				setSize(_size);

				mr = mc = true;
				down = 0;
			}
			void checkEvents(FGUI::IAbWnd & iaw) override {
				if (p_event->type == SDL_MOUSEWHEEL)
				{
					if (p_event->wheel.y > 0) // scroll up
					{
						down -= Vars::mouseWhellSpeed;
						if (down < 0) down = 0;
					}
					else if (p_event->wheel.y < 0) // scroll down
					{
						if (!(texts.size() <= (int)((float)size.y / Vars::ConsoleList::heightLine))) {
							down += Vars::mouseWhellSpeed;
							if (down > getLimitDown()) down = getLimitDown();
						}
					}
					draw();
				}
			}
			void display(FGUI::Render::RendererPack & rp) override {
				if (mc) {
					rp.clearLast(lLast, c_clear);
					mc = false;
				}
				if (mr) {
					rp.renderConsoleList(position, size, texts, down);
					mr = false;
				}
			}

			//////////////////
		};

		ConsoleList* CreateConsoleList(FGUI::Vector2i _position, FGUI::Vector2u _size) {
			ConsoleList* bufElement = new ConsoleList;
			bufElement->init(_position, _size);
			return bufElement;
		}
	}
}