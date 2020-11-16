#pragma once

#include <stdint.h>

float SHOWERASK_AUDIOHEIGHTPERSIZE = 1;
float SHOWERASK_ANIMAUDIO_FPS = 45;
float SHOWERASK_MUSIC_DIVMOVE = 2000.;

namespace FQuester {
	bool helpTab = true;
	bool callStart = false;
	uint16_t start_num_ask = 0;
	uint16_t sum_ask = 0;

	uint8_t q_menu_part = 0;
	int part_q_game = 0;
	int sa_part = 0;

	bool mAddPoints = false; bool mChangePoints = false; uint8_t cForPoints = 3; long long newPoints; bool updatePoints = false;

	bool inCmdPoints = false;
	bool ignorPressForPointsBar = false;

	long long plusPoints = 0; long long points[3];

	bool mRenderAskTable = false; uint16_t nTopic = 0;

	bool ignoreUpdateBg = false; bool fullIgnoreUpdateBg = false;

	bool goPlayContent = false;

	bool fiasko = false;

	sf::SoundBuffer sb_sound; sf::Sound s_sound; SDL_Rect dstPlayed; sf::Clock c_animMusic; SDL_Rect dstAnimMusic; uint16_t n_animMusic;
	cv::VideoCapture cap; cv::Mat frame; SDL_Texture* v_txtr; double v_fps; sf::Clock c1; sf::Clock c2; sf::Clock c3;
	bool showPosMedia = true; char type;
	int moveTick = 3;

	bool trigDeleteFrp = false; int frpSelected; bool mChangeSelect = false;
}