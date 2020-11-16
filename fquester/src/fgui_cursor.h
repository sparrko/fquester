#pragma once

#include "fgui_render.h"
#include "fgui_resources.h"
#include <SDL.h>

namespace FGUI {
	typedef enum {
		CURSOR_CLICK = 0, CURSOR_MOVE, CURSOR_REDACTOR, CURSOR_SIMPLE, CURSOR_SIZE, CURSOR_SIZELEFT, CURSOR_SIZETOP
	} CursorCondition;
	class Cursor {
		private:
			FGUI::Vector2i position;
			CursorCondition condition; 
		public:
			Cursor() { condition = FGUI::CursorCondition::CURSOR_SIMPLE; }
			void checkEvent() {
				if (p_event->type == SDL_MOUSEMOTION) { position.x = p_event->motion.x; position.y = p_event->motion.y; }
			}
			void display() {
				switch (condition) {
					case FGUI::CursorCondition::CURSOR_SIMPLE:
						FGUI::Render::DrawFTexture(position, &FGUI::Res::ft_c_simple);
						break;
					case FGUI::CursorCondition::CURSOR_MOVE:
						FGUI::Render::DrawFTexture(Vector2i(position.x - (int)FGUI::Res::ft_c_move.size.x / 2, position.y - (int)FGUI::Res::ft_c_move.size.y / 2), &FGUI::Res::ft_c_move);
						break;
					case FGUI::CursorCondition::CURSOR_REDACTOR:
						FGUI::Render::DrawFTexture(Vector2i(position.x - (int)FGUI::Res::ft_c_redactor.size.x / 2, position.y - (int)FGUI::Res::ft_c_redactor.size.y / 2), &FGUI::Res::ft_c_redactor);
						break;
					case FGUI::CursorCondition::CURSOR_SIZE:
						FGUI::Render::DrawFTexture(Vector2i(position.x - (int)FGUI::Res::ft_c_size.size.x / 2, position.y - (int)FGUI::Res::ft_c_size.size.y / 2), &FGUI::Res::ft_c_size);
						break;
					case FGUI::CursorCondition::CURSOR_SIZELEFT:
						FGUI::Render::DrawFTexture(Vector2i(position.x - (int)FGUI::Res::ft_c_sizeLeft.size.x / 2, position.y - (int)FGUI::Res::ft_c_sizeLeft.size.y / 2), &FGUI::Res::ft_c_sizeLeft);
						break;
					case FGUI::CursorCondition::CURSOR_SIZETOP:
						FGUI::Render::DrawFTexture(Vector2i(position.x - (int)FGUI::Res::ft_c_sizeTop.size.x / 2, position.y - (int)FGUI::Res::ft_c_sizeTop.size.y / 2), &FGUI::Res::ft_c_sizeTop);
						break;
					case FGUI::CursorCondition::CURSOR_CLICK:
						FGUI::Render::DrawFTexture(Vector2i(position.x - (int)FGUI::Res::ft_c_sizeTop.size.x / 2, position.y), &FGUI::Res::ft_c_click);
						break;
				}

				condition = FGUI::CursorCondition::CURSOR_SIMPLE;
			}
			CursorCondition getCondition() { return condition; }
			void setCondition(CursorCondition _condition) { condition = _condition; }
			Vector2i getPosition() { return position; }
			void setPosition(Vector2i _position) { position = _position; }
	} cursor;
}