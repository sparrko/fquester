#pragma once

#include "fgui_vars.h"

namespace FGUI {
	class IAbWnd {
		private:
			bool resized;
			bool mrBar;
			bool mrFrame; // must draw
			bool uCenter; // update
			Vector2u size;
			Vector2i position;
			FGUI::Color c_center;
		public:
		void init() {	
			size = FGUI::Vars::Window::defSize;
			c_center = FGUI::Vars::Window::c_defCenter;
			mrFrame = uCenter = mrBar = true;
		}
		////////////////////////////////////////////////////////////
		void start() {
			resized = false;
		}

		void fullUpdate() {
			mrFrame = true; mrBar = true; uCenter = true; resized = true;
		}

		void setSize(FGUI::Vector2u _size) { size = _size; mrFrame = true; mrBar = true; uCenter = true; resized = true; }
		Vector2u getSize() { return size; }
		bool wasResized() { return resized; }

		void setPosition(FGUI::Vector2i _position) { 
			if (_position.x < Vars::Window::adhesionBorders.x || _position.x > (-1) * Vars::Window::adhesionBorders.x) position.x = 0; else position.x = _position.x;
			if (_position.y < Vars::Window::adhesionBorders.y || _position.y > (-1) * Vars::Window::adhesionBorders.y) position.y = 0; else position.y = _position.y;
		}
		Vector2i getPosition() { return position; }

		void drawFrame(bool _mrFrame) { mrFrame = _mrFrame; }
		bool drawFrame() { return mrFrame; }

		void updateCenter(bool _uCenter) { uCenter = _uCenter; }
		bool updateCenter() { return uCenter; }
		

		void drawBar(bool _mrBar) { mrBar = _mrBar; }
		bool drawBar() { return mrBar; }

		void centerColor(FGUI::Color _c_center) { c_center = _c_center; }
		FGUI::Color centerColor() { return c_center; }
	};
}
