#include "fgui.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "fgui_RWGlobalVars.h"
#include "settings.h"
#include "objects.h"
#include "QAL_Reader.h"

struct Menu {
	FGUI::Window* fwin;

	FGUI::Elements::Button* b_menu;
	FGUI::Elements::Button* b_result;
	FGUI::Elements::Button* b_back;

	FGUI::Elements::Text* t_r; FGUI::Elements::Text* t_rr;
	FGUI::Elements::Text* t_g; FGUI::Elements::Text* t_rg;
	FGUI::Elements::Text* t_b; FGUI::Elements::Text* t_rb;

	FGUI::Elements::Text* t_logo;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"Пауза");
		fwin->deleteIcon(); fwin->setButtonsBar(0);
		fwin->setBlockMove(true); fwin->setBlockSize(true);
		fwin->setSize(FGUI::Vector2u(400, 170));
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);

		t_logo = FGUI::Elements::CreateText(); t_logo->setPosition(FGUI::Vector2i(140, 10));
		t_logo->setText(L"Меню"); fwin->tieElement(t_logo);

		t_r = FGUI::Elements::CreateText(FGUI::Vector2i(10 + 65, 50), L"Красная команда:"); fwin->tieElement(t_r);
		t_g = FGUI::Elements::CreateText(FGUI::Vector2i(10 + 65, 70), L"Зеленая команда:"); fwin->tieElement(t_g);
		t_b = FGUI::Elements::CreateText(FGUI::Vector2i(10 + 65, 90), L"Синяя команда:"); fwin->tieElement(t_b);

		t_rr = FGUI::Elements::CreateText(); t_rr->setPosition(FGUI::Vector2i(250 + 65, 50)); fwin->tieElement(t_rr); t_rr->setColor(FGUI::Color(220, 30, 30));
		t_rg = FGUI::Elements::CreateText(); t_rg->setPosition(FGUI::Vector2i(250 + 65, 70)); fwin->tieElement(t_rg); t_rg->setColor(FGUI::Color(10, 150, 10));
		t_rb = FGUI::Elements::CreateText(); t_rb->setPosition(FGUI::Vector2i(250 + 65, 90)); fwin->tieElement(t_rb); t_rb->setColor(FGUI::Color(30, 30, 220));

		b_back = FGUI::Elements::CreateButton(FGUI::Vector2i(10, 140), L"Вернуться в игру"); fwin->tieElement(b_back);
		b_result = FGUI::Elements::CreateButton(FGUI::Vector2i(155, 140), L"Подвести итог"); fwin->tieElement(b_result);
		b_menu = FGUI::Elements::CreateButton(FGUI::Vector2i(330, 140), L"Меню"); fwin->tieElement(b_menu);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (((FQuester::points[0] == 0) && (FQuester::points[1] == 0) && (FQuester::points[2] == 0)) ||
			((FQuester::points[0] == FQuester::points[1]) || (FQuester::points[0] == FQuester::points[2]) || (FQuester::points[2] == FQuester::points[1])) ||
			((FQuester::points[0] + FQuester::points[1] + FQuester::points[0]) < 0)
			) b_result->setHide(true);
		else b_result->setHide(false);
		b_result->draw();
		
		if (activateMenu) {
			if (!fwin->getShowed()) {
				SDL_WarpMouseInWindow(window, settings::getWindowSize().x / 2, settings::getWindowSize().y / 2);

				fwin->activate();
				activateMenu = false;

				t_rr->setText(std::to_wstring(FQuester::points[0]));
				t_rg->setText(std::to_wstring(FQuester::points[1]));
				t_rb->setText(std::to_wstring(FQuester::points[2]));

			}
		}
		if (fwin->getShowed()) {



			fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
			if (b_result->getPressed()) {
				b_result->reset();
				fwin->hide();
				FQuester::Res::aud_end.stop();
				FQuester::part_q_game = 4;
			}
			if (b_menu->getPressed()) {
				b_menu->reset();
				fwin->hide();
				mInitMenu = true;
				FQuester::Res::aud_end.stop();
				FQuester::QM_Reader::Clear();
			}
			if (b_back->getPressed()) {
				b_back->reset();
				fwin->hide();
				FQuester::part_q_game = 1;
				FQuester::Res::aud_end.stop();


				int musicPer = FQuester::start_num_ask * 100. / FQuester::sum_ask;
				int musicPart;
				if (musicPer >= 80) musicPart = 4;
				else if (musicPer >= 46) musicPart = 3;
				else if (musicPer >= 12) musicPart = 2;
				else musicPart = 1;

				if (musicPart == 1 && !FQuester::Res::aude_part_one.getPlaying()) { FQuester::Res::aude_part_one.setDiv(AUDE_DIV_PLUS); }
				else if (musicPart == 2 && !FQuester::Res::aude_part_two.getPlaying()) { FQuester::Res::aude_part_one.setDiv(AUDE_DIV_MINUS); FQuester::Res::aude_part_two.setDiv(AUDE_DIV_PLUS); }
				else if (musicPart == 3 && !FQuester::Res::aude_part_three.getPlaying()) { FQuester::Res::aude_part_one.setDiv(AUDE_DIV_MINUS); FQuester::Res::aude_part_two.setDiv(AUDE_DIV_MINUS); FQuester::Res::aude_part_three.setDiv(AUDE_DIV_PLUS); }
				else if (musicPart == 4 && !FQuester::Res::aude_part_four.getPlaying()) { FQuester::Res::aude_part_one.setDiv(AUDE_DIV_MINUS); FQuester::Res::aude_part_two.setDiv(AUDE_DIV_MINUS); FQuester::Res::aude_part_three.setDiv(AUDE_DIV_MINUS); FQuester::Res::aude_part_four.setDiv(AUDE_DIV_PLUS); }

			}
		}
	}
	void checkEvents() {
		if (fwin->getShowed()) {
		}
	}
	void update() {
		if (fwin->getShowed()) {
			if (fwin->getWasResized()) {

			}
		}
	}
} fw_menu;