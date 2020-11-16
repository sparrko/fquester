#pragma once

namespace FGUI {
	namespace WinCh {
		typedef enum {
			WINDOW_DEF_ICON_APP = 1,
			WINDOW_DEF_ICON_CONSOLE = 2,
			WINDOW_DEF_ICON_EXPLORER = 3,
			WINDOW_DEF_ICON_SETTINGS = 4
		} DefIcons;
		typedef enum {
			WINDOW_BAR_BUTTON_CLOSE = 0x00000001,
			WINDOW_BAR_BUTTON_FULLSCREEN = 0x00000002
		} BarButtons;
		typedef enum {
			WINDOW_NONE = 0,
			WINDOW_CENTER = 1,
		} Position;
	}
}