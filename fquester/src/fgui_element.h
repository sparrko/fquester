#pragma once

#include "fgui_c_clear.h"

namespace FGUI {
	namespace Elements {
		class Element {
			// Vector type
		protected:
			bool firstPos; bool mr; bool mc; bool hide; FGUI::Rect lLast;

			FGUI::Vector2i position;
			FGUI::Vector2u size;
			C_Clear c_clear;

			FGUI::Vector2i hidePos;
		public:
			virtual void checkEvents(FGUI::IAbWnd& iaw) = 0;
			virtual void display(FGUI::Render::RendererPack& rp) = 0;
			virtual void setPosition(Vector2i _position) {
				if (!hide) {
					if (firstPos) {
						lLast.x = position.x = _position.x;
						lLast.y = position.y = _position.y;
						lLast.w = size.x;
						lLast.h = size.y;

						firstPos = false;
					}
					else {
						lLast.x = position.x;
						lLast.y = position.y;
						lLast.w = size.x;
						lLast.h = size.y;

						position = _position;
					}
					mr = mc = true;
				}
			}

			void setClearColor(FGUI::Color _clear_color) {
				c_clear.setClearColor(_clear_color);
				if (!hide) { mr = mc = true; }
			}
			FGUI::Color getClearColor() {
				return c_clear.getClearColor();
			}

			void draw() {
				if (!hide) mr = true;
			}
			void noClear() {
				mc = false;
			}

			Vector2i getPosition() { return position; }
			Vector2u getSize() { return size; }

			bool limitRectCheck(FGUI::Rect& rch, FGUI::IAbWnd& iaw) {
				if (rch.x + rch.w < iaw.getPosition().x + Vars::Window::borderSize) return false;
				if (rch.y + rch.h < iaw.getPosition().y + Vars::Window::hatSize) return false;

				if (rch.x < iaw.getPosition().x + Vars::Window::borderSize) { rch.w = rch.x + rch.w - (iaw.getPosition().x + Vars::Window::borderSize); rch.x = iaw.getPosition().x + Vars::Window::borderSize; }
				if (rch.y < iaw.getPosition().y + Vars::Window::hatSize) { rch.h = rch.y + rch.h - (iaw.getPosition().y + Vars::Window::hatSize); rch.y = iaw.getPosition().y + Vars::Window::hatSize; }

				return true;
			}

			void setHide() {
				hide = !hide;
				if (hide) {
					mc = true;
				}
				else {
					mr = true;
				}
			}
			void setHide(bool _hide) {
				if (_hide == hide) return;

				hide = _hide;
				if (hide) {
					mc = true;
				}
				else {
					mr = true;
				}
			}
			bool getHide() {
				return hide;
			}
		};
	}
}