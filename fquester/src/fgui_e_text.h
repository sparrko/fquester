#pragma once

#include <string>
#include "fgui_element.h"
#include "fgui_objects.h"
#include "fgui_resources.h"
#include "fgui_window_iabwnd.h"
#include "fgui_render.h"

namespace FGUI {
	namespace Elements {
		class Text : public Element {
			bool firstSize;
			Render::FTexture tex_text;
			std::wstring text;
			Color cText;
			Vector2i position;
		public:
			/////////////////////////////
			void setPosition(Vector2i _position)  {
				if (!hide) {
					if (firstPos) {
						lLast.x = _position.x; lLast.y = _position.y;
						firstPos = false;
					}
					else {
						lLast.x = position.x; lLast.y = position.y;
					}
					mr = mc = true;
				}
				position = _position;
			}
			Vector2i getPosition() { return position; }

			void setText(std::wstring _text) {
				if (_text != text) {
					if(!firstSize) {
						lLast.w = tex_text.size.x; lLast.h = tex_text.size.y;
					}
					text = _text;
					SDL_Surface* text_serf = FGUI::Render::Text_Blended(Res::ttf_ubuntu_text, text, cText);
					tex_text.init(text_serf);

					size.x = tex_text.size.x; size.y = tex_text.size.y;

					SDL_FreeSurface(text_serf);

					if (firstSize) {
						lLast.w = tex_text.size.x; lLast.h = tex_text.size.y;
						firstSize = false;
					}

					if (!hide) mr = mc = true;
				}
			}
			void setText(std::string _text) {
				std::wstring buf(_text.begin(), _text.end());
				setText(buf);
			}
			std::wstring getText() { return text; }

			void setColor(Color _color) {
				cText = _color;
				SDL_Surface* text_serf = FGUI::Render::Text_Blended(Res::ttf_ubuntu_text, text, cText);
				tex_text.init(text_serf);
				SDL_FreeSurface(text_serf);
				if (!hide) { mr = mc = true; }
			}
			Color getColor() { return cText; }


			///////////////////
			
			void init(FGUI::Vector2i _position, std::wstring _text) {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true; //////////////////////////////////////////////////////////////////
				hide = false;

				setText(_text);
				setPosition(_position);
				setColor(Vars::Text::defFontColor);
			}
			void init() {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true; //////////////////////////////////////////////////////////////////
				hide = false;

				text = L"";
				setColor(Vars::Text::defFontColor);
			}
			void checkEvents(FGUI::IAbWnd& iaw) override {
			}
			void display(FGUI::Render::RendererPack& rp) override {
				if (mc) {
					rp.clearLast(lLast, c_clear);
					mc = false;
				}
				if(!hide) if (mr) {
					rp.renderText(tex_text, position);
					mr = false;
				}
			}
			
			//////////////////
		};

		Text* CreateText(FGUI::Vector2i _position, std::wstring _text = L"Simple Text") {
			Text* bufElement = new Text;
			bufElement->init(_position, _text);
			return bufElement;
		}
		Text* CreateText() {
			Text* bufElement = new Text;
			bufElement->init();
			return bufElement;
		}
	}
}