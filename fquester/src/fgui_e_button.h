#pragma once

#include <string>
#include "fgui_element.h"
#include "fgui_objects.h"
#include "fgui_resources.h"
#include "fgui_window_iabwnd.h"
#include "fgui_render.h"
#include "fgui_timer.h"

namespace FGUI {
	namespace Elements {
		class Button : public Element {
			bool firstSize;
			FGUI::Render::FTexture tex_text;
			std::wstring text;

			bool cursorOver;

			bool pressing;
			bool pressTick;
			bool evPress; FGUI::Timer t1; FGUI::Timer t2;
			

			uint8_t status_render; // 0 - nothink, 1 - over, 2 - pressing
			uint8_t old_status_render;
		public:
			/////////////////////////////
			void setSize(FGUI::Vector2u _size) {
				size = _size;
				draw();
			}
			Vector2u getSize() { return size; }
			std::wstring getText() {
				return text;
			}
			void setText(std::wstring _text) {
				if (_text == L"") {
					if (!hide) { lLast.x = position.x; lLast.y = position.y; lLast.w = size.x; lLast.h = size.y;}

					text = _text;

					tex_text.init(FGUI::Render::Text_Blended(FGUI::Res::ttf_ubuntu_button, L" ", FGUI::Vars::Button::c_text));

					size.x = tex_text.size.x = Vars::Button::minSize.x;
					size.y = tex_text.size.y = Vars::Button::minSize.y;

					if (firstSize) { firstSize = false; lLast.w = size.x; lLast.h = size.y; }
					
					if (!hide) mr = mc = true;
				}
				else if (text != _text) {
					if (!hide) { lLast.x = position.x; lLast.y = position.y; lLast.w = size.x; lLast.h = size.y; }

					text = _text;
					tex_text.init(FGUI::Render::Text_Blended(FGUI::Res::ttf_ubuntu_button, text, FGUI::Vars::Button::c_text));

					size.x = tex_text.size.x + Vars::Button::retreatX * 2;
					size.y = tex_text.size.y + Vars::Button::retreatY * 2;

					if (size.x < Vars::Button::minSize.x) size.x = Vars::Button::minSize.x;
					if (size.y < Vars::Button::minSize.y) size.y = Vars::Button::minSize.y;

					if (firstSize) { firstSize = false; lLast.w = size.x; lLast.h = size.y; }

					if(!hide)mr = mc = true;
				}
			}
			std::wstring getString() { return text; }
			Vector2i getPosition() { return position; }
			///////////////////
			void init(FGUI::Vector2i _position, FGUI::Vector2u _size, std::wstring _text) {
				firstSize = true; //////////////////////////////////////////////////////////////////
				firstPos = true; //////////////////////////////////////////////////////////////////
				hide = false; //////////////////////////////////////////////////////////////////

				setPosition(_position); 
				size = _size;
				setText(_text);

				cursorOver = false; pressing = false; pressTick = false; evPress = false;
				status_render = 0;

				mr = mc = true;
			}
			void init() {
				firstSize = true; //////////////////////////////////////////////////////////////////
				firstPos = true; //////////////////////////////////////////////////////////////////
				hide = false; //////////////////////////////////////////////////////////////////

				cursorOver = false; pressing = false; pressTick = false; evPress = false;
				status_render = 0;

				mr = mc = true;
			}
			void checkEvents(FGUI::IAbWnd& iaw) override {
				if (!hide) {
					pressTick = false; evPress = false;
					FGUI::Rect rch = Rect(iaw.getPosition().x + Vars::Window::borderSize + position.x, iaw.getPosition().y + Vars::Window::hatSize + position.y, size.x, size.y);
					if (limitRectCheck(rch, iaw)) {
						if (FGUI::CheckRect(rch, cursor.getPosition())) {
							cursorOver = true; status_render = 1; mr = true;
						}
						else {
							if (cursorOver != false) {
								cursorOver = false; status_render = 0; mr = true;
							}
							if (pressing != false) {
								pressing = false;
								mr = true;
							}
						}

						if (cursorOver) {
							if (p_event->type == SDL_MOUSEBUTTONDOWN && p_event->button.button == SDL_BUTTON_LEFT) {
								if(!pressing) { t2.restart(); } pressing = true; status_render = 2; mr = true;
							}
							else if (pressing) {
								if (p_event->type == SDL_MOUSEBUTTONUP && p_event->button.button == SDL_BUTTON_LEFT) {
									pressing = false;
									pressTick = true;
								}
							}
						}

						if (pressing == true && status_render != 3) {
							status_render = 3; mr = true;
						}
						
						if (pressing) {
							if (t2.getMilliseconds() > Vars::Button::timeStartTick2) { if (t1.getMilliseconds() > Vars::Button::speedTick2) { t1.restart(); evPress = true; } }
							else { if (t1.getMilliseconds() > Vars::Button::speedTick1) { t1.restart(); evPress = true; } }
						}
					}

					if (!FGUI::CheckRect(rch, cursor.getPosition()) && pressing == true) {
						status_render = 0; mr = true;
					}
				}
			}
			void display(FGUI::Render::RendererPack& rp) override {
				if (mc) {
					rp.clearLast(lLast, c_clear);
					mc = false;
				}
				if (!hide) if (mr) {
					rp.renderButton(position, size, tex_text, status_render); 
					mr = false;
				}
			}
			///////////////////
			bool getEventPressing() {
				return evPress;
			}
			bool getPressed() {
				return pressTick;
			}
			bool getPressing() {
				return pressing;
			}
			bool getCursorOver() {
				return cursorOver;
			}
			//////////////////
			void reset() {
				pressTick = pressing = evPress = cursorOver = false;
			}
		};

		Button* CreateButton(FGUI::Vector2i _position, std::wstring _text = L"Button") {
			Button* bufElement = new Button;
			bufElement->init(_position, Vector2u(1, 1), _text);
			return bufElement;
		}
		Button* CreateButton() {
			Button* bufElement = new Button;
			bufElement->init();
			return bufElement;
		}
	}
}