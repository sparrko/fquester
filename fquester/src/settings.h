#pragma once

// Количество модов
#define N_RESOLS 22
#define N_RENDERER 3


#include <fstream>
#include <sstream>
#include "system.h"
#include "fgui_system.h"
#include "finParser.h"
#include "frameLimit.h"

// resol(15) - 1440x900
// resol(19) - 1920x1080

// ------------------------------------- Системное ------------------------------------- // 

	// Что пишется в конфиге если его надо создавать
	#define CS_RESOL			0
	#define CS_RENDERER			2 // 0 - software, 1 - direct9, 2 - opengl(default)
	#define CS_FULLSCREEN		false
	#define CS_VSYNC			false
	#define CS_NOISE			true
	#define CS_FILTER			1
	#define CS_BRIGHT			130
	#define CS_FRAMELIMIT		0

	#define CS_VOLUME			100
	#define CS_BMUSIC			true
	#define CS_BTICKS			true

	#define CS_CATPER			20
	#define CS_EXCHANGE			false
	#define CS_TRANSMISSION		false
	#define CS_AUCTION			true
	#define CS_NAMES			true
	#define CS_DELETEAFTER		false
	#define CS_TIMER			15

	#define CS_CONTENTTYPEMODE	0
	#define CS_CONTENTTYPEMODEV	1
	#define CS_SCALINGCONTENT	1

	// Куда
	// #define CS_PATH			L"settings.fin"

	// Теперь(эпоха Beta) в Documents создается папка FQuester в которой как раз будет
	// валяться этот конфиг. Все это необходимо делать ради стабильности (больше не 
	// требуются права админа)

// ------------------------------------------------------------------------------------- // 



namespace settings {

	uint8_t			s_resol;
	uint8_t			s_renderer;
	bool			s_fullscreen;
	bool			s_vsync;
	bool			s_noise;
	uint8_t			s_filter;
	uint8_t			s_bright;
	uint16_t		s_frameLimit;

	uint8_t			s_volume;
	bool			s_bmusic;
	bool			s_bticks;

	uint8_t			s_catper;
	bool			s_exchange;
	bool			s_transmission;
	bool			s_auction;
	bool			s_deleteafter;
	bool			s_names;
	uint8_t			s_timer;

	uint8_t			s_contentTypeMode;
	uint8_t			s_contentTypeModeV;
	bool			s_scalingContent;

	//////////////////////////////////////////

	int resolMax;

	void save() {
		FILE* f;
		
		_wmkdir((DocumentsLocal + L"/FQuester").c_str());
		f = _wfopen((DocumentsLocal + L"/FQuester/settings.fin").c_str(), L"wb");

		std::string bufs;
		bufs = "# This is config file of settings for FQuester"; bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "# Window"; bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tresol = "; bufs += std::to_string((long long)s_resol); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tfullscreen = "; bufs += std::to_string((long long)s_fullscreen); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tbright = "; bufs += std::to_string((long long)s_bright); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "# Render"; bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\trender = "; bufs += std::to_string((long long)s_renderer); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tfilter = "; bufs += std::to_string((long long)s_filter); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tvsync = "; bufs += std::to_string((long long)s_vsync); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tnoise = "; bufs += std::to_string((long long)s_noise); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tframeLimit = "; bufs += std::to_string((long long)s_frameLimit); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		
		bufs = "# Volume"; bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tvolume = "; bufs += std::to_string((long long)s_volume); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tbmusic = "; bufs += std::to_string((long long)s_bmusic); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tbticks = "; bufs += std::to_string((long long)s_bticks); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);

		bufs = "# Cat"; bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tcatper = "; bufs += std::to_string((long long)s_catper); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\texchange = "; bufs += std::to_string((long long)s_exchange); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\ttransmission = "; bufs += std::to_string((long long)s_transmission); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tauction = "; bufs += std::to_string((long long)s_auction); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tnames = "; bufs += std::to_string((long long)s_names); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tdeleteAfter = "; bufs += std::to_string((long long)s_deleteafter); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\ttimer = "; bufs += std::to_string((long long)s_timer); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);

		bufs = "# Other"; bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tcontentTypeMode = "; bufs += std::to_string((long long)s_contentTypeMode); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tcontentTypeModeV = "; bufs += std::to_string((long long)s_contentTypeModeV); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		bufs = "\tscalingContent = "; bufs += std::to_string((long long)s_scalingContent); bufs += "\n"; fwrite(bufs.c_str(), sizeof(char), bufs.size(), f);
		fclose(f);
	}
	void read() {
		if (!(_waccess((DocumentsLocal + L"/FQuester/settings.fin").c_str(), 0) != -1)) {
			s_resol = CS_RESOL;
			s_renderer = CS_RENDERER;
			s_filter = CS_FILTER;
			s_fullscreen = CS_FULLSCREEN;
			s_vsync = CS_VSYNC;
			s_noise = CS_NOISE;
			s_volume = CS_VOLUME;
			s_bright = CS_BRIGHT;
			s_contentTypeMode = CS_CONTENTTYPEMODE;
			s_contentTypeModeV = CS_CONTENTTYPEMODEV;
			s_scalingContent = CS_SCALINGCONTENT;
			s_bmusic = CS_BMUSIC;
			s_catper = CS_CATPER;
			s_exchange = CS_EXCHANGE;
			s_transmission = CS_TRANSMISSION;
			s_auction = CS_AUCTION;
			s_deleteafter = CS_DELETEAFTER;
			s_timer = CS_TIMER;
			s_bticks = CS_BTICKS;
			s_names = CS_NAMES;
			s_frameLimit = CS_FRAMELIMIT; setFrameLimit(s_frameLimit);

			save();
		}
		FIN::Parser p;
		p.open(DocumentsLocal + L"/FQuester/settings.fin");
		settings::s_volume = p.getInt(L"volume");
		settings::s_resol = p.getInt(L"resol");
		settings::s_renderer = p.getInt(L"render");
		settings::s_filter = p.getInt(L"filter");
		settings::s_fullscreen = p.getBool(L"fullscreen");
		settings::s_vsync = p.getBool(L"vsync");
		settings::s_noise = p.getBool(L"noise");
		settings::s_bright = p.getInt(L"bright");
		settings::s_contentTypeModeV = p.getInt(L"contentTypeModeV");
		settings::s_contentTypeMode = p.getInt(L"contentTypeMode");
		settings::s_scalingContent = p.getInt(L"scalingContent");
		settings::s_catper = p.getInt(L"catper");
		settings::s_bmusic = p.getInt(L"bmusic");
		settings::s_exchange = p.getInt(L"exchange");
		settings::s_transmission = p.getInt(L"transmission");
		settings::s_auction = p.getInt(L"auction");
		settings::s_deleteafter = p.getInt(L"deleteAfter");
		settings::s_timer = p.getInt(L"timer");
		settings::s_bticks = p.getInt(L"bticks");
		settings::s_names = p.getInt(L"names");
		settings::s_frameLimit = p.getInt(L"frameLimit"); setFrameLimit(s_frameLimit);
	}
	
	FGUI::Vector2u getWindowSize(int resol = s_resol) {
		switch (resol) {
		case 0: { return FGUI::Vector2u(640, 480); }
		case 1: { return FGUI::Vector2u(720, 480); }
		case 2: { return FGUI::Vector2u(720, 576); }
		case 3: { return FGUI::Vector2u(800, 600); }
		case 4: { return FGUI::Vector2u(1024, 768); }
		case 5: { return FGUI::Vector2u(1152, 864); }
		case 6: { return FGUI::Vector2u(1280, 720); }
		case 7: { return FGUI::Vector2u(1280, 768); }
		case 8: { return FGUI::Vector2u(1280, 800); }
		case 9: { return FGUI::Vector2u(1280, 960); }
		case 10: { return FGUI::Vector2u(1280, 1024); }
		case 11: { return FGUI::Vector2u(1360, 768); }
		case 12: { return FGUI::Vector2u(1360, 1024); }
		case 13: { return FGUI::Vector2u(1366, 768); }
		case 14: { return FGUI::Vector2u(1440, 720); }
		case 15: { return FGUI::Vector2u(1400, 900); }
		case 16: { return FGUI::Vector2u(1600, 900); }
		case 17: { return FGUI::Vector2u(1600, 1200); }
		case 18: { return FGUI::Vector2u(1680, 1050); }
		case 19: { return FGUI::Vector2u(1920, 1080); }
		case 20: { return FGUI::Vector2u(2048, 1536); }
		case 21: { return FGUI::Vector2u(2560, 1600); }
		case 22: { return FGUI::Vector2u(3840, 2160); }
		}
	}

	void checkMaxResol(FGUI::Vector2i maxSize) {
		for (int i = 0; i < N_RESOLS; i++) {
			if (getWindowSize(i).x > maxSize.x) {
				resolMax = i;
				break;
			}
		}
	}
};
// ------------------------------------------------------------------------------------- // 
