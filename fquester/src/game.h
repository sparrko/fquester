#pragma once

#include "game_globalVars.h"

#include "res.h"
#include "cursor.h"

#include "q_menu.h"
#include "q_game.h"
#include "q_redactor.h"
#include "q_menuGui.h"

#include "audioUnit.h"

namespace FQuester {
	void Init() {
		// First init
		FRP::InitFRP();
		mInitMenu = true;
		Res::aude_menu.setVolume(100);
	}
	void PreUpdate() {
		// Init for all
		if (mInitMenu) {
			// Clear QAL
			Res::aud_result.stop();
			Res::aud_high.stop();
			Res::aud_plus.stop();

			FQuester::QM_Reader::Clear();

			m_part = 0;
			q_menu.Init();
			mInitMenu = false;

			showerAsk.shutDown();
		}
		if (mInitGame) {
			Res::aude_menu.setDiv(AUDE_DIV_MINUS);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); SDL_RenderFillRect(renderer, NULL); SDL_RenderPresent(renderer);
			
			// Load QAL
			/*sf::Thread openPackThread([]() {
				FQuester::showLoadingBG();
			});
			openPackThread.launch();*/
			if (!WFileExists(FRP_LIST_PATH + FQuester::FRP::frpList[qmgFPRList.selected].path)) { sc.addDraw(L"Can't open pack. Unknown error."); mInitGame = false; mInitMenu = true; }
			try {
				if (!FQuester::QM_Reader::Init(FRP_LIST_PATH + FQuester::FRP::frpList[qmgFPRList.selected].path)) { FQuester::FRP::Update(); mInitMenu = true; Res::aud_cant.play(); sc.addDraw(FQuester::QM_Reader::error); }
			}
			catch (exception e) {
				SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Ошибка чтения", e.what(), window);
			}
			sum_ask = 0;
			for (int i = 0; i < FQuester::QM_Reader::topics.size(); i++)
				sum_ask += FQuester::QM_Reader::topics[i].asks.size();

						SDL_Delay(200);
						//FQuester::stopLoadingBG();

			// Start
						//openPackThread.terminate();
			shadow.set(255, -3);
			m_part = 1;
			q_game.Init();
			mInitGame = false;
			SDL_ShowCursor(false);
		}
		if (mInitRedactor) {
			m_part = 2;
			q_redactor.Init();
			mInitRedactor = false;
		}

		if (m_part == 0) { q_menu.PreUpdate(); }
		else if (m_part == 1) { q_game.PreUpdate(); }
		else if (m_part == 2) { q_redactor.PreUpdate(); }

		audengine.update();
	}
	void CheckEvents() {

		if (m_part == 0) { q_menu.CheckEvents(); }
		else if (m_part == 1) { q_game.CheckEvents(); }
		else if (m_part == 2) { q_redactor.CheckEvents(); }
		
	}
	void PostUpdate() {
		if (m_part == 0) { q_menu.PostUpdate(); }
		else if (m_part == 1) { q_game.PostUpdate(); }
		else if (m_part == 2) { q_redactor.PostUpdate(); }	
		
	}
	void Display() {
		if (m_part == 0) { q_menu.Display(); }
		else if (m_part == 1) { q_game.Display(); }
		else if (m_part == 2) { q_redactor.Display(); }

		
	}
}