#pragma once

#include "fgui_render.h"
#include "fgui_resources.h"
#include "fgui_timer.h"

namespace FGUI {
	class CursorTextBox {
	private:
		FGUI::Vector2i position;
		FGUI::Timer tm;
		FGUI::Timer tmw;
		bool show;
		FGUI::Render::FTexture texture;
		FGUI::Render::FTexture texture2;
		bool wrong;
	public:
		CursorTextBox() { wrong = false; show = false; position.x = position.y = -1; }
		void init() {
			texture.init(FGUI::Render::Text_Blended(Res::ttf_ubuntu_textBox, Vars::TextBox::redactorChar, Vars::TextBox::c_text));
			texture2.init(FGUI::Render::Text_Blended(Res::ttf_ubuntu_textBox, Vars::TextBox::redactorChar, Vars::TextBox::c_textWrong));
		}
		void display() {
			if (tmw.getMilliseconds() > Vars::TextBox::tickWrong && wrong) {
				wrong = false;
			}

			if (tm.getMilliseconds() > Vars::TextBox::tickCursor) { show = !show; tm.restart(); }

			if (position.x > 0 && position.y > 0 && show) {
				if (!wrong) {
					FGUI::Render::DrawFTexture(position, &texture);
				}
				else {
					FGUI::Render::DrawFTexture(position, &texture2);
				}
			}

			position.x = position.y = -1;
		}
		void wrongInput() {
			tmw.restart();
			wrong = true;
		}
		Vector2i getPosition() {
			return position;
		}
		void setPosition(FGUI::Vector2i _position) {
			position = _position;
		}
	} cursorText;
}