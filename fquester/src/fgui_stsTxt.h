#pragma once

#ifndef FGUI_RENDER_SDL && FGUI_RENDER_OPENGL
#define FGUI_RENDER_SDL
#endif

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "fgui_vars.h"
#include "fgui_resources.h"
#include "fgui_ftexture.h"

namespace FGUI {
	namespace Elements {
		struct StsText {
			StsText() {
				rendered = false;
				text = L"";
			}
			bool rendered;
			std::wstring text;
			Render::FTexture texture;
			void render(FGUI::Color color = Vars::SettingsList::defFontColor) {
				if (texture.exists()) texture.clear();
				SDL_Color bufc; bufc.r = color.r; bufc.g = color.g; bufc.b = color.b; bufc.a = color.a;
				auto surf = TTF_RenderUNICODE_Blended(Res::ttf_ubuntu_stsTxt, (Uint16*)text.c_str(), bufc);
				texture.init(surf);
				SDL_FreeSurface(surf);
				rendered = true;
			}
			void setText(std::wstring _text) {
				text = _text;
				rendered = false;
			}
			~StsText() {
				//texture.clear();
			}
		};
	}
}