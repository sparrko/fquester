#pragma once

#include <SFML/Audio.hpp>
#include <SDL.h>
#include "audioUnit.h"
#include <opencv2/opencv.hpp>

namespace FQuester {
	namespace Res {
		// Res 1
		sf::SoundBuffer sb_startup; sf::Sound aud_startup;
		sf::SoundBuffer sb_shutdown; sf::Sound aud_shutdown;
		sf::SoundBuffer sb_warning; sf::Sound aud_warning;
		sf::SoundBuffer sb_cant; sf::Sound aud_cant;

		// TTF
		uint16_t ttf_u_menu_footer_size = 14;
		TTF_Font* ttf_u_menu_footer;
		uint16_t ttf_select_name_size = 78;
		TTF_Font* ttf_select_name;
		uint16_t ttf_select_other_size = 58;
		TTF_Font* ttf_select_other;
		uint16_t ttf_infoDev_size = 18;
		TTF_Font* ttf_infoDev;

		uint16_t ttf_ask_size = 80;
		TTF_Font* ttf_ask;
		uint16_t ttf_topic_size = 180;
		TTF_Font* ttf_topic;
		uint16_t ttf_points_size = 200;
		TTF_Font* ttf_points;
		uint16_t ttf_topicsBar_size = 120;
		TTF_Font* ttf_topicsBar;
		uint16_t ttf_askFrame_size = 150;
		TTF_Font* ttf_askFrame;


		uint16_t ttf_timer_size = 150;
		TTF_Font* ttf_timer;
		uint16_t ttf_timer_size_out = 150;
		TTF_Font* ttf_timer_out;


		uint16_t ttf_tpoints_size = 50;
		TTF_Font* ttf_tpoints;
		uint16_t ttf_tpoints_size_out = 50;
		TTF_Font* ttf_tpoints_out;

		uint16_t ttf_endButtons_size = 50;
		TTF_Font* ttf_endButtons;
		uint16_t ttf_endButtons_size_out = 50;
		TTF_Font* ttf_endButtons_out;

		uint16_t ttf_loadBg_size = 30;
		TTF_Font* ttf_loadBg;

		// Res 2
		std::vector <SDL_Texture*> music_anim;

		sf::SoundBuffer sb_auction; sf::Sound aud_auction;
		sf::SoundBuffer sb_end; sf::Sound aud_end;
		sf::SoundBuffer sb_haha; sf::Sound aud_haha;
		sf::SoundBuffer sb_time; sf::Sound aud_time;
		sf::SoundBuffer sb_press; sf::Sound aud_press;
		sf::SoundBuffer sb_tick; sf::Sound aud_tick;
		sf::SoundBuffer sb_tack; sf::Sound aud_tack;
		sf::SoundBuffer sb_screenshot; sf::Sound aud_screenshot;

		sf::SoundBuffer sb_high; sf::Sound aud_high;
		sf::SoundBuffer sb_result; sf::Sound aud_result;
		sf::SoundBuffer sb_plus; sf::Sound aud_plus;

		Aud aude_menu;
		Aud aude_part_one;
		Aud aude_part_three;
		Aud aude_part_two;
		Aud aude_part_four;
		Aud aude_serf;
		Aud aude_categories;

		SDL_Texture* txtr_cursor;

		SDL_Texture* txtr_m_bg_menu;
		SDL_Texture* txtr_m_light;
		SDL_Texture* txtr_m_frame;
		SDL_Texture* txtr_m_logo;
		SDL_Texture* txtr_m_gui_menu;
		SDL_Texture* txtr_m_gui_select_select;
		SDL_Texture* txtr_m_gui_select_static;
		SDL_Texture* txtr_g_floatButton;
		SDL_Texture* txtr_g_floatBar;
		SDL_Texture* txtr_g_cmdPointsFrame;
		SDL_Texture* txtr_g_cmdPointsBg;
		SDL_Texture* txtr_g_bg;
		SDL_Texture* txtr_g_askFrame;
		SDL_Texture* txtr_g_scmd;

		cv::VideoCapture cap;
	}
}