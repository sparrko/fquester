#include "fgui.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "fgui_RWGlobalVars.h"
#include "q_globalVars.h"

#define NOMINMAX

struct AuctionWindow {
	FGUI::Window* fwin;

	FGUI::Elements::Text* p_text; 
	FGUI::Elements::Text* p_prnow; int check;
	FGUI::Elements::Text* text[2];
	FGUI::Elements::Button* no;
	FGUI::Elements::Button* button;
	FGUI::Elements::Button* max[3];
	FGUI::Elements::TextBox* tb;

	void init() {
		check = 0;

		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"                                 Аукцион");
		fwin->deleteIcon(); fwin->setButtonsBar(0);
		fwin->setBlockMove(true); fwin->setBlockSize(true);
		fwin->setSize(FGUI::Vector2u(420, 130));
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);

		
		text[0] = FGUI::Elements::CreateText(FGUI::Vector2i(10, 10), L"      Команда, открывшая этот вопрос, предлагает свою"); fwin->tieElement(text[0]);
		text[1] = FGUI::Elements::CreateText(FGUI::Vector2i(10, 30), L"цену. Цена не больше имеющихся баллов этой команды!"); fwin->tieElement(text[1]);
		
		p_text = FGUI::Elements::CreateText(FGUI::Vector2i(10, 68), L"Цена: "); fwin->tieElement(p_text);

		p_prnow = FGUI::Elements::CreateText(FGUI::Vector2i(10, 103), L"Стоимость без: "); fwin->tieElement(p_prnow);

		tb = FGUI::Elements::CreateTextBox(FGUI::Vector2i(55, 65), 240);
		tb->setInputOnlyNum(true);
		fwin->tieElement(tb);

		max[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(305 - 3, 65), L"R"); fwin->tieElement(max[0]);
		max[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(345 - 3, 65), L"G"); fwin->tieElement(max[1]);
		max[2] = FGUI::Elements::CreateButton(FGUI::Vector2i(385 - 3, 65), L"B"); fwin->tieElement(max[2]);

		button = FGUI::Elements::CreateButton(FGUI::Vector2i(305 - 3, 100), L"Продолжить");
		fwin->tieElement(button);

		no = FGUI::Elements::CreateButton(FGUI::Vector2i(200 - 3 - 3, 100), L"Отказаться");
		fwin->tieElement(no);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (activateAuctionWindow) { 
			if (!fwin->getShowed()) { fwin->activate(); } activateAuctionWindow = false; 
			if (check != FQuester::plusPoints) {
				check = FQuester::plusPoints;
				p_prnow->setText(L"Стоимость без: " + std::to_wstring((long long)check));
			}
		}
		if (fwin->getShowed()) {
			fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
			if (button->getPressed()) {
				if (!tb->getText().empty()) {
					long n = std::stol(tb->getText());
					if (n <= std::max(std::max(FQuester::points[0], FQuester::points[1]), FQuester::points[2]) && n > 0 || n == 0) {
						FQuester::goPlayContent = true;
						if (n != 0) FQuester::plusPoints = n;
						tb->clear();
						fwin->hide();
						button->reset();
					}
					else {
						FQuester::Res::aud_warning.play();
					}
				}
			}
			for (int i = 0; i < 3; i++) {
				if (max[i]->getPressed()) if (FQuester::points[i] < 0) tb->setText(L"0"); else tb->setText(std::to_wstring(FQuester::points[i]));
			}
			if (no->getPressed()) {
				FQuester::goPlayContent = true;
				FQuester::sa_part = 0;
				tb->clear();
				fwin->hide();
				no->reset();
			}
		}
	}
	void checkEvents() {
		if (fwin->getShowed()) {
			if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
				FQuester::goPlayContent = true;
				FQuester::sa_part = 0;
				tb->clear();
				fwin->hide();
				no->reset();
			}
		}
	}
	void update() {
		if (fwin->getShowed()) {
			if (fwin->getWasResized()) {

			}
		}
	}
} fw_auctionWindow;