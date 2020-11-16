#pragma once

#include "fdpReader.h"
#include <SDL.h>
#include <SDL_image.h>
#include "objects.h"
#include "res.h"
#include "audioUnit.h"

namespace FQuester {
	void vPars(SDL_Texture* texture, cv::Mat const& mat)
	{
		IplImage* img = &(IplImage)mat;

		unsigned char* texture_data = NULL;
		int texture_pitch = 0;

		SDL_LockTexture(texture, 0, (void**)& texture_data, &texture_pitch);
		memcpy(texture_data, (void*)img->imageData, img->width * img->height * img->nChannels);
		SDL_UnlockTexture(texture);
	}



	bool FileExists(std::string fname) { return access(fname.c_str(), 0) != -1; }
	bool WFileExists(std::wstring fname) { return _waccess(fname.c_str(), 0) != -1; }


	// First
	FDP::Reader fdpfp;
	std::vector <SDL_Texture*> load_anim;
	bool loadFirstFDP() {
		if (!FileExists("resources/res1.fdp")) { return false; }
		if (!fdpfp.open("resources/res1.fdp")) { return false; }
		for (int i = 0; i < 62; i++) {
			std::string path = "load_anim/"; path += std::to_string((long long)i); path += ".png";

			SDL_RWops* rw;
			rw = SDL_RWFromMem(fdpfp.getRes(path), fdpfp.getSize(path));
			SDL_Surface* image = IMG_LoadPNG_RW(rw);
			
			load_anim.resize(load_anim.size() + 1);
			load_anim[load_anim.size() - 1] = SDL_CreateTextureFromSurface(renderer, image);

			SDL_FreeSurface(image);
			SDL_FreeRW(rw);
		}

		FQuester::Res::sb_startup.loadFromMemory(fdpfp.getRes("audio/startup.ogg"), fdpfp.getSize("audio/startup.ogg"));
		FQuester::Res::aud_startup.setBuffer(FQuester::Res::sb_startup);
		FQuester::Res::sb_shutdown.loadFromMemory(fdpfp.getRes("audio/shutdown.ogg"), fdpfp.getSize("audio/shutdown.ogg"));
		FQuester::Res::aud_shutdown.setBuffer(FQuester::Res::sb_shutdown);
		FQuester::Res::sb_warning.loadFromMemory(fdpfp.getRes("audio/warning.ogg"), fdpfp.getSize("audio/warning.ogg"));
		FQuester::Res::aud_warning.setBuffer(FQuester::Res::sb_warning); 
		FQuester::Res::sb_cant.loadFromMemory(fdpfp.getRes("audio/limitTextBox.ogg"), fdpfp.getSize("audio/limitTextBox.ogg"));
		FQuester::Res::aud_cant.setBuffer(FQuester::Res::sb_cant);
		FQuester::Res::sb_screenshot.loadFromMemory(fdpfp.getRes("audio/screenshot.ogg"), fdpfp.getSize("audio/screenshot.ogg"));
		FQuester::Res::aud_screenshot.setBuffer(FQuester::Res::sb_screenshot);
		
		if (access("resources/futurac.ttf", 0) == -1) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Can't open resource: futurac.ttf", FGUI::p_window); exit(0); }

		SDL_RWops* bufrw_ttf_u_menu_footer = SDL_RWFromFile("resources/ubuntu.ttf", "rb"); Res::ttf_u_menu_footer = TTF_OpenFontRW(bufrw_ttf_u_menu_footer, 0, Res::ttf_u_menu_footer_size);
		SDL_RWops* bufrw_ttf_select_name = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_select_name = TTF_OpenFontRW(bufrw_ttf_select_name, 0, Res::ttf_select_name_size);
		SDL_RWops* bufrw_ttf_select_other = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_select_other = TTF_OpenFontRW(bufrw_ttf_select_other, 0, Res::ttf_select_other_size);
		SDL_RWops* bufrw_ttf_infoDev = SDL_RWFromFile("resources/ubuntu.ttf", "rb"); Res::ttf_infoDev = TTF_OpenFontRW(bufrw_ttf_infoDev, 0, Res::ttf_infoDev_size);
		SDL_RWops* bufrw_ttf_ask = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_ask = TTF_OpenFontRW(bufrw_ttf_ask, 0, Res::ttf_ask_size);
		SDL_RWops* bufrw_ttf_topic = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_topic = TTF_OpenFontRW(bufrw_ttf_topic, 0, Res::ttf_topic_size);
		SDL_RWops* bufrw_ttf_points = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_points = TTF_OpenFontRW(bufrw_ttf_points, 0, Res::ttf_points_size);
		SDL_RWops* bufrw_ttf_topicsBar = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_topicsBar = TTF_OpenFontRW(bufrw_ttf_topicsBar, 0, Res::ttf_topicsBar_size);
		SDL_RWops* bufrw_ttf_askFrame = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_askFrame = TTF_OpenFontRW(bufrw_ttf_askFrame, 0, Res::ttf_askFrame_size);
		SDL_RWops* bufrw_ttf_timer = SDL_RWFromFile("resources/ubuntu.ttf", "rb"); Res::ttf_timer = TTF_OpenFontRW(bufrw_ttf_timer, 0, Res::ttf_timer_size);
		SDL_RWops* bufrw_ttf_timer_out = SDL_RWFromFile("resources/ubuntu.ttf", "rb"); Res::ttf_timer_out = TTF_OpenFontRW(bufrw_ttf_timer_out, 0, Res::ttf_timer_size_out); 

		SDL_RWops* bufrw_ttf_tpoints = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_tpoints = TTF_OpenFontRW(bufrw_ttf_tpoints, 0, Res::ttf_tpoints_size);
		SDL_RWops* bufrw_ttf_tpoints_out = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_tpoints_out = TTF_OpenFontRW(bufrw_ttf_tpoints_out, 0, Res::ttf_tpoints_size_out);

		SDL_RWops* bufrw_ttf_endButtons = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_endButtons = TTF_OpenFontRW(bufrw_ttf_endButtons, 0, Res::ttf_endButtons_size);
		SDL_RWops* bufrw_ttf_endButtons_out = SDL_RWFromFile("resources/futurac.ttf", "rb"); Res::ttf_endButtons_out = TTF_OpenFontRW(bufrw_ttf_endButtons_out, 0, Res::ttf_endButtons_size_out);

		SDL_RWops* bufrw_ttf_loadBg = SDL_RWFromFile("resources/ubuntu.ttf", "rb"); Res::ttf_loadBg = TTF_OpenFontRW(bufrw_ttf_loadBg, 0, Res::ttf_loadBg_size);
		
		fdpfp.clear();
		return true;
	}

	// Second
	FDP::Reader fdpsp;
	bool loadSecondFDP() {
		if (!FileExists("resources/res2.fdp")) { return false; }
		if (!fdpsp.open("resources/res2.fdp")) { return false; }

		// Audio (SFML)
		FQuester::Res::sb_auction.loadFromMemory(fdpsp.getRes("audio/auction.ogg"), fdpsp.getSize("audio/auction.ogg")); FQuester::Res::aud_auction.setBuffer(FQuester::Res::sb_auction);
		FQuester::Res::sb_end.loadFromMemory(fdpsp.getRes("audio/end.ogg"), fdpsp.getSize("audio/end.ogg")); FQuester::Res::aud_end.setBuffer(FQuester::Res::sb_end);
		FQuester::Res::sb_haha.loadFromMemory(fdpsp.getRes("audio/haha.ogg"), fdpsp.getSize("audio/haha.ogg")); FQuester::Res::aud_haha.setBuffer(FQuester::Res::sb_haha);
		FQuester::Res::sb_time.loadFromMemory(fdpsp.getRes("audio/time.ogg"), fdpsp.getSize("audio/time.ogg")); FQuester::Res::aud_time.setBuffer(FQuester::Res::sb_time);
		FQuester::Res::sb_press.loadFromMemory(fdpsp.getRes("audio/sb_press.ogg"), fdpsp.getSize("audio/sb_press.ogg")); FQuester::Res::aud_press.setBuffer(FQuester::Res::sb_press);
		FQuester::Res::sb_tick.loadFromMemory(fdpsp.getRes("audio/tick.ogg"), fdpsp.getSize("audio/tick.ogg")); FQuester::Res::aud_tick.setBuffer(FQuester::Res::sb_tick); FQuester::Res::aud_tick.setVolume(50);
		FQuester::Res::sb_tack.loadFromMemory(fdpsp.getRes("audio/tack.ogg"), fdpsp.getSize("audio/tack.ogg")); FQuester::Res::aud_tack.setBuffer(FQuester::Res::sb_tack); FQuester::Res::aud_tack.setVolume(50);

		FQuester::Res::sb_high.loadFromMemory(fdpsp.getRes("audio/high.ogg"), fdpsp.getSize("audio/high.ogg")); FQuester::Res::aud_high.setBuffer(FQuester::Res::sb_high);
		FQuester::Res::sb_result.loadFromMemory(fdpsp.getRes("audio/result.ogg"), fdpsp.getSize("audio/result.ogg")); FQuester::Res::aud_result.setBuffer(FQuester::Res::sb_result);
		FQuester::Res::sb_plus.loadFromMemory(fdpsp.getRes("audio/plus.ogg"), fdpsp.getSize("audio/plus.ogg")); FQuester::Res::aud_plus.setBuffer(FQuester::Res::sb_plus); FQuester::Res::aud_plus.setLoop(true);

		// Music AudEngine
		
		FQuester::Res::aude_categories.openFromMem(fdpsp.getRes("audio/categories.ogg"), fdpsp.getSize("audio/categories.ogg"), true); audengine.addInArray(&FQuester::Res::aude_categories);
		FQuester::Res::aude_menu.openFromMem(fdpsp.getRes("audio/menu.ogg"), fdpsp.getSize("audio/menu.ogg"), true); audengine.addInArray(&FQuester::Res::aude_menu);
		FQuester::Res::aude_part_one.openFromMem(fdpsp.getRes("audio/part_one.ogg"), fdpsp.getSize("audio/part_one.ogg"), true); audengine.addInArray(&FQuester::Res::aude_part_one);
		FQuester::Res::aude_part_two.openFromMem(fdpsp.getRes("audio/part_two.ogg"), fdpsp.getSize("audio/part_two.ogg"), true); audengine.addInArray(&FQuester::Res::aude_part_two);
		FQuester::Res::aude_part_three.openFromMem(fdpsp.getRes("audio/part_three.ogg"), fdpsp.getSize("audio/part_three.ogg"), true); audengine.addInArray(&FQuester::Res::aude_part_three);
		FQuester::Res::aude_part_four.openFromMem(fdpsp.getRes("audio/path_four.ogg"), fdpsp.getSize("audio/path_four.ogg"), true); audengine.addInArray(&FQuester::Res::aude_part_four);
		FQuester::Res::aude_serf.openFromMem(fdpsp.getRes("audio/serf.ogg"), fdpsp.getSize("audio/serf.ogg"), true); audengine.addInArray(&FQuester::Res::aude_serf); int mrand = std::rand() % 3; if (mrand == 0) { if (std::rand() % 2 == 0) Res::aude_serf.setPitch(1.02); else Res::aude_serf.setPitch(1.00); } else if (mrand == 1) Res::aude_serf.setPitch(0.6); else Res::aude_serf.setPitch(0.8);

		// Textures (SDL)

		for (int i = 0; i < 80; i++) {
			std::string path = "anim_music/"; path += std::to_string((long long)i); path += ".png";

			SDL_RWops* rw;
			rw = SDL_RWFromMem(fdpsp.getRes(path), fdpsp.getSize(path));
			SDL_Surface* image = IMG_LoadPNG_RW(rw);

			Res::music_anim.resize(Res::music_anim.size() + 1);
			Res::music_anim[Res::music_anim.size() - 1] = SDL_CreateTextureFromSurface(renderer, image);

			SDL_FreeSurface(image);
			SDL_FreeRW(rw);
		}

		

		SDL_RWops* bufrw; SDL_Surface* bufserf; // buf res


		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/bg_menu.png"), fdpsp.getSize("texture/bg_menu.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_m_bg_menu = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf); 
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/light.png"), fdpsp.getSize("texture/light.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_m_light = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf); 
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/frame.png"), fdpsp.getSize("texture/frame.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_m_frame = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf); 
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/logo.png"), fdpsp.getSize("texture/logo.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_m_logo = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf); 
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/cursor.png"), fdpsp.getSize("texture/cursor.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_cursor = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/gui_menu.png"), fdpsp.getSize("texture/gui_menu.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_m_gui_menu = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/select_select.png"), fdpsp.getSize("texture/select_select.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_m_gui_select_select = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/select_static.png"), fdpsp.getSize("texture/select_static.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_m_gui_select_static = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/floatButton.png"), fdpsp.getSize("texture/floatButton.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_g_floatButton = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/floatBar.png"), fdpsp.getSize("texture/floatBar.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_g_floatBar = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/cmdPointsFrame.png"), fdpsp.getSize("texture/cmdPointsFrame.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_g_cmdPointsFrame = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/cmdPointsBg.png"), fdpsp.getSize("texture/cmdPointsBg.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_g_cmdPointsBg = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/bg_game.png"), fdpsp.getSize("texture/bg_game.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_g_bg = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/ask_frame.png"), fdpsp.getSize("texture/ask_frame.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_g_askFrame = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		bufrw = SDL_RWFromMem(fdpsp.getRes("texture/scmd.png"), fdpsp.getSize("texture/scmd.png")); bufserf = IMG_Load_RW(bufrw, 1); SDL_RWclose(bufrw);
		Res::txtr_g_scmd = SDL_CreateTextureFromSurface(renderer, bufserf); SDL_FreeSurface(bufserf);
		
		Res::cap.open("resources/bg_game.fvp");

		fdpsp.clear();
		return true;
	}



}