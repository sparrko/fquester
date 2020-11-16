#pragma once

#include <string>
#include "fgui_element.h"
#include "fgui_objects.h"
#include "fgui_resources.h"
#include "fgui_window_iabwnd.h"
#include "fgui_render.h"

namespace FGUI {
	namespace Elements {
		class Serf : public Element {
			Render::FTexture image;
			bool firstSize;
		public:
			/////////////////////////////
			
			Vector2i getPosition() { return position; }

			void setSize(Vector2u _size) {
				if (firstSize) { lLast.w = _size.x; lLast.h = _size.y; firstSize = false; }
				else { lLast.w = size.x; lLast.h = size.y; }
				size = _size;

				if (!hide) mr = mc = true;
			}
			void setImage(SDL_Surface* serf) {
				image.init(serf);
				if (!hide) mr = mc = true;
			}

			///////////////////

			void init(FGUI::Vector2i _position, Vector2u _size, SDL_Surface* _serf) {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true;
				hide = false; //////////////////////////////////////////////////////////////////

				setPosition(_position);
				setSize(_size);
				setImage(_serf);
			}
			void init() {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true;
				hide = false; //////////////////////////////////////////////////////////////////
			}
			void checkEvents(FGUI::IAbWnd& iaw) override {
				
			}
			void display(FGUI::Render::RendererPack& rp) override {
				if (mc) {
					rp.clearLast(lLast, c_clear);
					mc = false;
				}
				if (mr) {
					rp.renderSerf(position, size, image);
					mr = false;
				}
			}

			//////////////////
		};

		Serf* CreateSerf() {
			Serf* bufElement = new Serf;
			bufElement->init();
			return bufElement;
		}

		Serf* CreateSerf(FGUI::Vector2i _position, FGUI::Vector2u _size, SDL_Surface* _serf) {
			Serf* bufElement = new Serf;
			bufElement->init(_position, _size, _serf);
			return bufElement;
		}
	}
}