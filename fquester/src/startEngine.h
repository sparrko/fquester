
bool vsyncWas = false;

const int NOISE_FPS = 20;



#include <SFML/System.hpp>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_opengl.h>

#include "fgui.h"
#include "fguiRW_questerRedactor.h"
#include "fguiRW_console.h"
#include "fguiRW_start.h"
#include "fguiRW_settings.h"
#include "fguiRW_exitAsk.h"
#include "fguiRW_auction.h"
#include "fguiRW_menu.h"
#include "fguiRW_cpuTest.h"
#include "fguiRW_reallyDelete.h"
#include "fguiRW_mediaGuide.h"
#include "fguiRW_askReset.h"
#include "fguiRW_namesList.h"
#include "fguiRW_badResol.h"

#include <iostream>



#include "screenshot.h"

#include "settings.h"

#include "SystemConsole.h"
#include "shadow.h"
#include "q_load.h"
#include "readPack.h"
#include "game.h"

#include "frameLimit.h"

#include "infoDev.h"

#include "textOpt.h"

#include "startLogo.h"

short int shadowMax = 200;
float shadowDiv = 20;
short int shadowFPS = 80;
float shadow = 0; 
FGUI::Timer shadowT;






struct Engine {
	sf::Clock fps_limit_timer;
	SDL_Texture* a;
	SDL_Rect adst;
	Engine() { isRunning = true; }
	void Init() {
		// Init system
		FQuester::StartLogo::ShowLogo(); // init sdl
		if (TTF_Init() != 0) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Can't iniitialise SDL_ttf", TTF_GetError(), window); exit(0); }
		
		// Clear buffer files for unpacking
		std::wstring path = AppdataLocal;
		path += L"/FQuester";
		FQuester::ClearUnpackFiles(path);

		// Create VARS
		SDL_DisplayMode DM; SDL_GetCurrentDisplayMode(0, &DM); DesktopX = DM.w; DesktopY = DM.h; FGUI::Vector2i maxSize(DesktopX, DesktopY); settings::checkMaxResol(maxSize); 
		settings::read();

		// Creaеte window
		window = SDL_CreateWindow("FQuester",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			settings::getWindowSize().x, settings::getWindowSize().y,
			SDL_WINDOW_HIDDEN);

		// Create render
		Uint32 rendererFlags = SDL_RENDERER_SOFTWARE;
		if (settings::s_renderer == 0) SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "software", SDL_HintPriority::SDL_HINT_OVERRIDE);
		else if (settings::s_renderer == 1) SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "direct3d", SDL_HintPriority::SDL_HINT_OVERRIDE);
		else if (settings::s_renderer == 2) SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengl", SDL_HintPriority::SDL_HINT_OVERRIDE);
		if (settings::s_renderer != 0) {
			if (settings::s_vsync == true) { rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC; vsyncWas = true; }
			else { rendererFlags = SDL_RENDERER_ACCELERATED; }
		}
		if (settings::s_filter == 0) { SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "0", SDL_HintPriority::SDL_HINT_OVERRIDE); }
		else if (settings::s_filter == 1) { SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "1", SDL_HintPriority::SDL_HINT_OVERRIDE); }
		else if (settings::s_filter == 2) { SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "2", SDL_HintPriority::SDL_HINT_OVERRIDE); }		
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | rendererFlags);
		
		// Read first resources
		if (!FQuester::loadFirstFDP()) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Can't open resource: res1.fdp", window); exit(0); };

		// Init FGUI
		FGUI::Init(window, renderer, &event);
		fw_CPUTest.init(); fw_startWindow.init(); fw_settings.init(); fw_console.init(); fw_questerRedactor.init(); fw_exitAsk.init(); fw_auctionWindow.init(); fw_menu.init(); fw_reallyDelete.init(); fw_mediaGuide.init(); fw_askReset.init(); fw_namesList.init(); fw_badResol.init();

		// Apply config settings
		SDL_SetWindowBrightness(FGUI::p_window, (float)settings::s_bright / 100);
		sf::Listener::setGlobalVolume(settings::s_volume);

		// Read second resources
		srand(random_srand.getElapsedTime().asMicroseconds());
		if (!FQuester::loadSecondFDP()) { SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Can't open resource: res2.fdp", window); exit(0); };

		// Initialise game
		FQuester::Init();

		// Show real window
		FQuester::StartLogo::CloseLogo();
		SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN); SDL_ShowWindow(window);
		if (settings::s_fullscreen == true) { SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); } else { SDL_SetWindowFullscreen(window, SDL_WINDOW_SHOWN); }
		SDL_ShowCursor(false);

		// Set cursor center
		SDL_WarpMouseInWindow(window, settings::getWindowSize().x / 2, settings::getWindowSize().y / 2);

		// Init noise
		n_inited = false;
		if (!n_inited) {
			SDL_Surface* s_n = SDL_CreateRGBSurface(0, 800, 600, 32, NULL, NULL, NULL, NULL);

			for (int a = 0; a < 12; a++) {
				for (int x = 0; x < 800; x++) {
					for (int y = 0; y < 600; y++) {
						Uint32* target_pixel = (Uint32*)s_n->pixels + (y * s_n->w) + x;
						uint8_t rand = std::rand() % 100;
						uint32_t a = (int)(255 << 24 | rand << 16 | rand << 8 | rand);
						*target_pixel = a;
					}
				}

				t_n[a] = SDL_CreateTextureFromSurface(renderer, s_n);
				SDL_SetTextureBlendMode(t_n[a], SDL_BLENDMODE_BLEND);
			}
			SDL_FreeSurface(s_n);
			t_n_frame = 0;

			n_inited = true;
		}
	}
	void CheckEvents() {
		// Console update
		sc.update();

		// FGUI events
		fw_CPUTest.checkFguiEvents(); fw_startWindow.checkFguiEvents(); fw_questerRedactor.checkFguiEvents(); fw_console.checkFguiEvents(); fw_settings.checkFguiEvents(); fw_exitAsk.checkFguiEvents(); fw_auctionWindow.checkFguiEvents(); fw_menu.checkFguiEvents(); fw_reallyDelete.checkFguiEvents(); fw_mediaGuide.checkFguiEvents(); fw_askReset.checkFguiEvents(); fw_namesList.checkFguiEvents(); fw_badResol.checkFguiEvents();

		// Update game before CheckEvents
		FQuester::PreUpdate();

		// Game 
		while (SDL_PollEvent(&event)) {
			// Game CheckEvents or FGUI
			fw_CPUTest.checkEvents(); fw_startWindow.checkEvents(); fw_questerRedactor.checkEvents(); fw_console.checkEvents(); fw_settings.checkEvents(); fw_exitAsk.checkEvents(); fw_auctionWindow.checkEvents(); fw_menu.checkEvents(); fw_reallyDelete.checkEvents(); fw_mediaGuide.checkEvents(); fw_askReset.checkEvents(); fw_namesList.checkEvents(); fw_badResol.checkEvents();
			if (FGUI::GUIActive()) FGUI::CheckEvents();
			else FQuester::CheckEvents();
			
			if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_F10) { isRunning = false; break; }
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_F12) FGUI::NewSizeWindow();
			if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F11) {
				time_t seconds = time(NULL);
				_wmkdir(L"screenshots");
				std::string file; file = "screenshots/fq"; file += std::to_string((long long)seconds); file += ".png";
				saveScreenshot(file.c_str(), renderer);
				sc.addDraw("Screenshot saved: " + file);
				FQuester::Res::aud_screenshot.play();
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F8) {
				ChangeFullscreen();
				FQuester::Res::aud_startup.play();
			}
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_F4) activateSettings = true;

			// Check InfoDev struct
			infoDev.checkEvents();
		}
		
		// Update game after CheckEvents
		FQuester::PostUpdate();
	}
	void Update() {
		// Updating logic outside the interface engine
		fw_console.update(); fw_mediaGuide.update(); fw_questerRedactor.update();
		
		// Crash
		if (crash1b) { m_part = 2; crash1(); } else if (crash2b) { m_part = 2; crash2(); if (FGUI::getCountsWindow() == 255) { crash2b = false; } } else if (crash3b) { m_part = 2; crash3(); }

		FGUI::Update();
	}
	void Display() {
		// Main rendering
		FQuester::Display();

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		// TV effect
		if (settings::s_noise && m_part == 1) {

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(renderer, t_n[t_n_frame], NULL, NULL);
			if (t_n_time.getElapsedTime().asMilliseconds() > 1000. / NOISE_FPS) {
				t_n_time.restart();
				t_n_frame++;
				if (t_n_frame >= 12) t_n_frame = 0;
				SDL_SetTextureAlphaMod(t_n[t_n_frame], 35);
			}
		}

		// Shadow gui
		if (shadowT.getMilliseconds() > 1000. / shadowFPS) {
			shadowT.restart();
			if (FGUI::GUIActive() && !fw_mediaGuide.fwin->getShowed()) {
				shadow += shadowDiv;
				if (shadow > shadowMax) shadow = shadowMax;
			}
			else {
				shadow -= shadowDiv;
				if (shadow < 0) shadow = 0;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, shadow); SDL_RenderFillRect(renderer, NULL);

		// System
		if (FGUI::GUIActive()) FGUI::Display();
		infoDev.display();

		// Push display
		SDL_RenderPresent(renderer);
	}
	void WhileWork() {
		while (isRunning) {
			if ((fps_limit_timer.getElapsedTime().asMicroseconds() > 1000000. / n_frameLimit) || n_frameLimit == 0) {
				fps_limit_timer.restart();
				CheckEvents();
				Update();
				Display();
			}
		}
	}
	void Quit() {
		std::wstring path = AppdataLocal;
		path += L"/FQuester";
		FQuester::ClearUnpackFiles(path);

		/*FGUI::Quit();
		
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);*/

		remove((TempLocal + "videobuffer.bin").c_str());

		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}
};