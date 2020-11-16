#pragma once

#include <string>
#include "fgui_element.h"
#include "fgui_objects.h"
#include "fgui_resources.h"
#include "fgui_window_iabwnd.h"
#include "fgui_render.h"

namespace FGUI {
	namespace Elements {
		class Panel : public Element {
			Color cbg;
			bool firstSize;
			bool convex;
		public:
			/////////////////////////////
			void setConvex(bool _convex) {
				convex = _convex;
			}

			Vector2i getPosition() { return position; }

			void setColor(Color _color) {
				cbg = _color;
				if (!hide) { mr = mc = true; }
			}
			Color getColor() { return cbg; }

			void setSize(Vector2u _size) {
				if (firstSize) { lLast.w = _size.x; lLast.h = _size.y; firstSize = false; }
				else { lLast.w = size.x; lLast.h = size.y; }
				size = _size;

				if (!hide) mr = mc = true;
			}

			///////////////////

			void init(FGUI::Vector2i _position, Vector2u _size) {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true;
				hide = false; //////////////////////////////////////////////////////////////////

				convex = false;

				setPosition(_position);
				setSize(_size);
				setColor(Vars::Panel::c_defCenter);
			}
			void init() {
				firstPos = true; //////////////////////////////////////////////////////////////////
				firstSize = true;
				hide = false; //////////////////////////////////////////////////////////////////

				convex = false;

				setColor(Vars::Panel::c_defCenter);
			}
			void checkEvents(FGUI::IAbWnd& iaw) override {
				
			}
			void display(FGUI::Render::RendererPack& rp) override {
				if (mc) {
					rp.clearLast(lLast, c_clear);
					mc = false;
				}
				if (mr) {
					rp.renderPanel(position, size, cbg, convex);
					mr = false;
				}
			}

			//////////////////
		};

		Panel* CreatePanel() {
			Panel* bufElement = new Panel;
			bufElement->init();
			return bufElement;
		}

		Panel* CreatePanel(FGUI::Vector2i _position, FGUI::Vector2u _size) {
			Panel* bufElement = new Panel;
			bufElement->init(_position, _size);
			return bufElement;
		}
	}
}