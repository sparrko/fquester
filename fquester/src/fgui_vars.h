#pragma once

#include "fgui_system.h"

namespace FGUI {
	namespace Vars {


		float globalSize = 1.;

		unsigned int short text_fsize = globalSize * 15;
		int mouseWhellSpeed = 1;

		namespace SettingsList {
			float fontSize = globalSize * 30;
			float widthDDown = globalSize * 12;
			// const uint16_t retreatXDDown = 2;
			// const uint16_t retreatYDDown = 4;

			float retreatXText = globalSize * 3;
			float retreatYText = globalSize * 1;
			float retreatRight = globalSize * 3;
			float retreatDownX = globalSize * 8;
			float retreatDownY = globalSize * 4;
			float downSizeX = globalSize * 4;

			float heightLine = globalSize * 36;

			const FGUI::Color c_down(200, 200, 200);
			const FGUI::Color c_cursor(180, 180, 180);
			const FGUI::Color defFontColor(255, 255, 255);
			const FGUI::Color c_center(80, 80, 80);
			const FGUI::Color c_lightLn(255, 255, 255, 255);
			const FGUI::Color c_darkLn(0, 0, 0, 255);

			// Double down
			//const FGUI::Color c_dDownI(220, 220, 220, 255); // inside
			//const FGUI::Color c_dDownO(160, 160, 160, 255); // outside

			int maxLines = 128;
		}

		namespace Panel {
			const FGUI::Color c_defCenter(200, 200, 200);
			const FGUI::Color c_lightLn(255, 255, 255, 180);
			const FGUI::Color c_darkLn(0, 0, 0, 180);
		}

		namespace List {
			float fontSize = globalSize * 14;
			float widthDDown = globalSize * 12;
			// const uint16_t retreatXDDown = 2;
			// const uint16_t retreatYDDown = 4;

			float retreatXText = globalSize * 3;
			float retreatYText = globalSize * 1;
			float retreatRight = globalSize * 15;
			float retreatDownX = globalSize * 8;
			float retreatDownY = globalSize * 6;
			float downSizeX = globalSize * 4;

			float heightLine = globalSize * 16;

			const FGUI::Color c_down(200, 200, 200);
			const FGUI::Color c_cursor(80, 80, 180);
			const FGUI::Color defFontColor(255, 255, 255);
			const FGUI::Color c_center(80, 80, 80);
			const FGUI::Color c_lightLn(255, 255, 255, 255);
			const FGUI::Color c_darkLn(0, 0, 0, 255);

			// Double down
			//const FGUI::Color c_dDownI(220, 220, 220, 255); // inside
			//const FGUI::Color c_dDownO(160, 160, 160, 255); // outside

			int maxLines = 128;
			float retreatDown = globalSize * 30;
		}

		namespace ConsoleList {
			float minCursorY = globalSize * 5;

			float fontSize = globalSize * 14;
			float widthDDown = 12;
			// const uint16_t retreatXDDown = 2;
			// const uint16_t retreatYDDown = 4;

			float retreatXText = globalSize * 3;
			float retreatYText = globalSize * 1;
			float retreatRight = globalSize * 15;

			float heightLine = globalSize * 16;

			const FGUI::Color defFontColor(255, 255, 255);
			const FGUI::Color c_center(80, 80, 80);
			const FGUI::Color c_lightLn(255, 255, 255, 255);
			const FGUI::Color c_darkLn(0, 0, 0, 255);

			// Double down
			//const FGUI::Color c_dDownI(220, 220, 220, 255); // inside
			//const FGUI::Color c_dDownO(160, 160, 160, 255); // outside

			int maxLines = 128;
			float retreatDownLines = globalSize * 2;
		}

		namespace CheckBox {
			float fontSize = globalSize * text_fsize;
			const FGUI::Color defFontColor(0, 0, 0);

			float iconSize = globalSize * 16;
			float retreatIcon = globalSize * 4;
		}

		namespace TextBox {
			const FGUI::Color c_text(0, 0, 0);
			const FGUI::Color c_textWrong(255, 0, 0);
			const FGUI::Color c_center(255, 255, 255);
			const FGUI::Color c_darkLn(50, 50, 50);

			float sizeY = globalSize * 24;

			float retreatRight = globalSize * 25;
			float retreatX = globalSize * 4;
			float retreatY = globalSize * -1;
			float fontSize = globalSize * 18;

			const char* redactorChar = "|";

			float retreatCursor = globalSize * 3;

			const short int tickCursor = 300;
			const short int tickWrong = 800;

			const short int maxChars = 256;
		}

		namespace Text {
			float fontSize = globalSize * text_fsize;
			const FGUI::Color defFontColor(0, 0, 0);
		}

		namespace Button {
			float button_text_size = text_fsize;

			const FGUI::Color c_text(0, 0, 0);
			const FGUI::Color c_rects(80, 80, 80);

			const FGUI::Color c_cPressed(180, 180, 230); // center
			const FGUI::Color c_cNothink(220, 220, 220);
			const FGUI::Color c_cOver(250, 250, 255);

			const FGUI::Color c_lightLn(255, 255, 255, 255);
			const FGUI::Color c_darkLn(30, 30, 30, 255);

			float borderSize = globalSize * (2 + 2);
			const FGUI::Vector2u minSize(10, 22);
			float retreatX = globalSize * 10;
			float retreatY = globalSize * 3;

			float speedTick1 = 250;
			float speedTick2 = 75;
			float timeStartTick2 = 1500;
		}

		namespace Window {
			// Position
			const Vector2u adhesionBorders(20, 15); // Прилипание к краям; (0, 0) - значит отключено

			// Bar
			float buttonSize = globalSize * 22;
			float buttonRetreat = globalSize * 2;

			// Frame
			FGUI::Color c_rects(100, 100, 100, 255);
			const FGUI::Color c_lightLn(144, 144, 144);
			const FGUI::Color c_darkLn(72, 72, 72);
			const FGUI::Color c_defCenter(180, 180, 180);
			float borderSize = globalSize * 7;
			float hatSize = globalSize * 27;
			const FGUI::Vector2u minSize(buttonSize * 2 + buttonRetreat * 2, 1); // Inside(x - 3 button)
			const FGUI::Vector2u defSize(300, 300); // Inside
			const FGUI::Vector2i defPos(80, 50); // Inside
			// const bool GrUp = true; // gradient up
			const FGUI::Color c_rects_grup(150, 150, 150, 255);
			float caption_fsize = globalSize * 21;
			// Caption
			const FGUI::Color c_caption(255, 255, 255);

			// Icon
			float iconSize = globalSize * 20;
			float iconRetreatAfter = globalSize * 2;

			// Elements
			const uint16_t maxElements = 256;
		}
	}
}