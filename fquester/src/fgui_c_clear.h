#pragma once

#include "fgui_system.h"

namespace FGUI {
	namespace Elements {
		class C_Clear {
			bool updated;
			FGUI::Color c_clear;
			public:
			C_Clear() {
				updated = false;
			}
			void setClearColor(FGUI::Color _clear_color) {
				c_clear = _clear_color;
				updated = true;
			}
			FGUI::Color getClearColor() {
				return c_clear;
			}
			bool getUpdated() {
				return updated;
			}
		};
	}
}