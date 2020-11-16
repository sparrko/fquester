#pragma once

#include <string>
#include "fgui_element.h"
#include "fgui_objects.h"
#include "fgui_resources.h"
#include "fgui_window_iabwnd.h"
#include "fgui_render.h"

namespace FGUI {
	namespace Elements {
		class CheckBox : public Element {
				Render::FTexture tex_text;
				std::wstring text;

				bool checked;
				bool mUp;
			public:
			void setText(std::wstring _text){
				if (text != _text) {
					if (!hide) { lLast.x = position.x; lLast.y = position.y; lLast.w = size.x; lLast.h = size.y; }

					text = _text;
					tex_text.init(FGUI::Render::Text_Blended(FGUI::Res::ttf_ubuntu_checkBox, text, FGUI::Vars::CheckBox::defFontColor));

					size.x = tex_text.size.x + Vars::CheckBox::retreatIcon + Vars::CheckBox::iconSize;
					size.y = Vars::CheckBox::iconSize;

					if (!hide) mr = mc = true;
				}
			}
			void setChecked(bool _checked) {
				checked = _checked;
				if (!hide) mc = mr = true;
			}
			void setChecked() {
				checked = !checked;
				if (!hide) mc = mr = true;
			}
			bool getChecked() {
				return checked;
			}
			////////////////////////////////////////////////
			void init(FGUI::Vector2i _position, std::wstring _text){
				firstPos = true; //////////////////////////////////////////////////////////////////
				checked = false;
				hide = false;

				mUp = false;
				setText(_text);
				setPosition(_position);

				mr = mc = true;
			}
			void checkEvents(FGUI::IAbWnd& iaw) override {
				if (!hide) {
					FGUI::Rect rch = Rect(iaw.getPosition().x + Vars::Window::borderSize + position.x, iaw.getPosition().y + Vars::Window::hatSize + position.y, size.x, size.y);
					if (limitRectCheck(rch, iaw)) {
						if (FGUI::CheckRect(rch, cursor.getPosition())) {
							if (p_event->type == SDL_MOUSEBUTTONDOWN && p_event->button.button == SDL_BUTTON_LEFT) {
								if (!mUp) {
									setChecked();
									mUp = true;
								}
							}
							if (p_event->type == SDL_MOUSEBUTTONUP && p_event->button.button == SDL_BUTTON_LEFT) {
								mUp = false;
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
				if (!hide) if (mr) {
					rp.clearLast(lLast, c_clear);
					mc = false;
					rp.renderCheckBox(tex_text, position, checked);
					mr = false;
				}
			}
			
		};
		/////////////////////////////////////////

		CheckBox* CreateCheckBox(FGUI::Vector2i _position, std::wstring _text) {
			CheckBox* bufElement = new CheckBox;
			bufElement->init(_position, _text);
			return bufElement;
		}
	}
}