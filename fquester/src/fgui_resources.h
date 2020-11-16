#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <io.h>
#include "fgui_ftexture.h"
#include "fgui_vars.h"

// Cursor
#include "fgui_res_click.xpm"
#include "fgui_res_move.xpm"
#include "fgui_res_redactor.xpm"
#include "fgui_res_simple.xpm"
#include "fgui_res_size.xpm"
#include "fgui_res_sizeLeft.xpm"
#include "fgui_res_sizeTop.xpm"

#include "fgui_res_icons.xpm"

#include "fgui_res_bar.xpm"

#include "fgui_res_checkBox.XPM"

///////////////////////////////////////////////// MACROS
#define CFGUIX_FAST
////////////////////////////////////////////////////////

// Fonts
#ifdef CFGUIX_RELEASE
#include "fgui_ubuntu_ttf.h"
#endif

namespace FGUI {
	namespace Res {
		// Cursors
		FGUI::Render::FTexture ft_c_click;
		FGUI::Render::FTexture ft_c_move;
		FGUI::Render::FTexture ft_c_redactor;
		FGUI::Render::FTexture ft_c_simple;
		FGUI::Render::FTexture ft_c_size;
		FGUI::Render::FTexture ft_c_sizeLeft;
		FGUI::Render::FTexture ft_c_sizeTop;
		// Def icons
		FGUI::Render::FTexture ft_i[4];
		// Buttons bar
		FGUI::Render::FTexture ft_b[6];
		// CheckBox
		FGUI::Render::FTexture ft_cb[2];

		TTF_Font* ttf_ubuntu_caption;
		TTF_Font* ttf_ubuntu_button;
		TTF_Font* ttf_ubuntu_text;
		TTF_Font* ttf_ubuntu_textBox;
		TTF_Font* ttf_ubuntu_checkBox;
		TTF_Font* ttf_ubuntu_clsTxt;
		TTF_Font* ttf_ubuntu_stsTxt;

		void Init() {
			// Def icons
			SDL_Surface* buf = IMG_ReadXPMFromArray((char**)fgui_icons_xpm);
			SDL_Rect srcr; srcr.w = buf->w / 4; srcr.h = srcr.w; srcr.y = 0;
			SDL_Surface* buf2 = SDL_CreateRGBSurface(0, srcr.w, srcr.h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
			srcr.x = srcr.w * 0; SDL_BlitSurface(buf, &srcr, buf2, NULL); 
			ft_i[0].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 1; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_i[1].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 2; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_i[2].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 3; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_i[3].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			// Buttons bar
			buf = IMG_ReadXPMFromArray((char**)fgui_bar_xpm);
			srcr.w = srcr.h = buf->h;
			SDL_FreeSurface(buf2); buf2 = SDL_CreateRGBSurface(0, srcr.w, srcr.h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
			srcr.x = srcr.w * 0; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_b[0].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 1; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_b[1].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 2; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_b[2].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 3; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_b[3].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 4; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_b[4].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 5; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_b[5].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			// CheckBox
			buf = IMG_ReadXPMFromArray((char**)fgui_checkbox_xpm);
			srcr.w = srcr.h = buf->h;
			SDL_FreeSurface(buf2); buf2 = SDL_CreateRGBSurface(0, srcr.w, srcr.h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
			srcr.x = srcr.w * 0; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_cb[0].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			srcr.x = srcr.w * 1; SDL_BlitSurface(buf, &srcr, buf2, NULL);
			ft_cb[1].init(buf2); SDL_FillRect(buf2, NULL, SDL_MapRGBA(buf2->format, 0, 0, 0, 0));
			// Fonts
			#ifdef CFGUIX_RELEASE
				SDL_RWops* bufrw = SDL_RWFromMem((void*)fgui_ubuntu_ttf, fgui_ubuntu_ttf_size);
			#else ifdef CFGUIX_FAST
				if (_access("resources/ubuntu.ttf", 0) == -1) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Can't open resource: ubuntu.ttf", FGUI::p_window); exit(0); }
				SDL_RWops* bufrw = SDL_RWFromFile("resources/ubuntu.ttf", "rb");
			#endif
			ttf_ubuntu_caption = TTF_OpenFontRW(bufrw, 1, FGUI::Vars::Window::caption_fsize);
			ttf_ubuntu_button = TTF_OpenFontRW(bufrw, 1, FGUI::Vars::Button::button_text_size);
			ttf_ubuntu_text = TTF_OpenFontRW(bufrw, 1, FGUI::Vars::Text::fontSize);
			ttf_ubuntu_textBox = TTF_OpenFontRW(bufrw, 1, FGUI::Vars::TextBox::fontSize);
			ttf_ubuntu_checkBox = TTF_OpenFontRW(bufrw, 1, FGUI::Vars::CheckBox::fontSize);
			ttf_ubuntu_clsTxt = TTF_OpenFontRW(bufrw, 1, FGUI::Vars::ConsoleList::fontSize);
			ttf_ubuntu_stsTxt = TTF_OpenFontRW(bufrw, 1, FGUI::Vars::SettingsList::fontSize);
			// Cursor
			SDL_FreeSurface(buf); buf = IMG_ReadXPMFromArray((char**)fgui_c_click_xpm);
			ft_c_click.init(buf);
			SDL_FreeSurface(buf); buf = IMG_ReadXPMFromArray((char**)fgui_c_move_xpm);
			ft_c_move.init(buf);
			SDL_FreeSurface(buf); buf = IMG_ReadXPMFromArray((char**)fgui_c_redactor_xpm);
			ft_c_redactor.init(buf);
			SDL_FreeSurface(buf); buf = IMG_ReadXPMFromArray((char**)fgui_c_simple_xpm);
			ft_c_simple.init(buf);
			SDL_FreeSurface(buf); buf = IMG_ReadXPMFromArray((char**)fgui_c_size_xpm);
			ft_c_size.init(buf);
			SDL_FreeSurface(buf); buf = IMG_ReadXPMFromArray((char**)fgui_c_sizeLeft_xpm);
			ft_c_sizeLeft.init(buf);
			SDL_FreeSurface(buf); buf = IMG_ReadXPMFromArray((char**)fgui_c_sizeTop_xpm);
			ft_c_sizeTop.init(buf);
			// Clear
			SDL_FreeSurface(buf);
		}
		void Clear() {
			ft_c_sizeTop.clear(); ft_c_sizeLeft.clear(); ft_c_size.clear(); ft_c_simple.clear(); ft_c_redactor.clear(); ft_c_move.clear(); ft_c_click.clear();

			ft_i[0].clear(); ft_i[1].clear(); ft_i[2].clear(); ft_i[3].clear();
			ft_b[0].clear(); ft_b[1].clear(); ft_b[2].clear(); ft_b[3].clear(); ft_b[4].clear(); ft_b[5].clear();
			ft_cb[0].clear(); ft_cb[1].clear();

			TTF_CloseFont(ttf_ubuntu_caption);
			TTF_CloseFont(ttf_ubuntu_button);
			TTF_CloseFont(ttf_ubuntu_text);
			TTF_CloseFont(ttf_ubuntu_textBox);
			TTF_CloseFont(ttf_ubuntu_checkBox);
			TTF_CloseFont(ttf_ubuntu_clsTxt);
		}
	}
}