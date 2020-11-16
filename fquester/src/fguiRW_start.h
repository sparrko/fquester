#include "fgui.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "fgui_RWGlobalVars.h"
#include "qal_unpacking.h"
#include "loadBG.h"

struct StartWindow {
	
	FGUI::Window* fwin;
	FGUI::Elements::Panel* p;
	FGUI::Elements::Button* b_con;
	FGUI::Elements::Button* b_open;
	FGUI::Elements::Button* b_no;
	FGUI::Elements::Text* t_nAutor;
	FGUI::Elements::Text* t_nPack;
	FGUI::Elements::TextBox* tb_nAutor;
	FGUI::Elements::TextBox* tb_nPack;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"                          Инициализация");
		fwin->deleteIcon(); fwin->setButtonsBar(0);
		fwin->setBlockMove(true); fwin->setBlockSize(true);
		fwin->setSize(FGUI::Vector2u(400, 115));
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);

		p = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 5), FGUI::Vector2u(390, 105));
		fwin->tieElement(p);

		b_con = FGUI::Elements::CreateButton(FGUI::Vector2i(280, 80), L"Продолжить");
		fwin->tieElement(b_con); 

		b_open = FGUI::Elements::CreateButton(FGUI::Vector2i(190, 80), L"Открыть");
		fwin->tieElement(b_open);

		b_no = FGUI::Elements::CreateButton(FGUI::Vector2i(10, 80), L"Вернуться"); 
		fwin->tieElement(b_no);

		tb_nPack = FGUI::Elements::CreateTextBox(FGUI::Vector2i(150, 7 + 3), 240);
		fwin->tieElement(tb_nPack);

		tb_nAutor = FGUI::Elements::CreateTextBox(FGUI::Vector2i(150, 37 + 3), 240);
		fwin->tieElement(tb_nAutor);

		t_nAutor = FGUI::Elements::CreateText(FGUI::Vector2i(10, 40 + 3), L"Имя автора:"); t_nAutor->setClearColor(p->getColor());
		fwin->tieElement(t_nAutor);

		t_nPack = FGUI::Elements::CreateText(FGUI::Vector2i(10, 10 + 3), L"Название пака:"); t_nPack->setClearColor(p->getColor());
		fwin->tieElement(t_nPack);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (activateStart) { if (!fwin->getShowed()) { fwin->activate(); } activateStart = false; }
		if (fwin->getShowed()) {
			if (b_no->getPressed()) {
				fwin->hide();
				mInitMenu = true;
				b_no->reset();
			}

			bool callPressButton = false;

			if (tb_nPack->getActive()) { if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_TAB) tb_nAutor->setCursor(); }
			if (tb_nAutor->getPressedEnter()) { callPressButton = true; }

			fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
			if (b_con->getPressed() || callPressButton) {
				if (!tb_nAutor->getText().empty() && !tb_nPack->getText().empty()) {
					qal.setName(tb_nPack->getText());
					qal.setAutor(tb_nAutor->getText());
					activateQuesterRedactor = true;
					fwin->hide();
				}
				else {
					FQuester::Res::aud_warning.play();
				}
				b_con->reset();
			}

			if (b_open->getPressed()) {
				std::wstring path = AppdataLocal;
				path += L"/FQuester";

				_wmkdir(path.c_str());

				setFalseFullscreenCheckBox = true;
				if (CreateWindowAndRenderer(settings::getWindowSize(), false)) FGUI::NewSizeWindow();

				auto buf = FQuester::OpenFile('Q');
				if (!buf.empty() && buf.size() != 255) {
					RenderLoadBG(L"Загружается пак. Ждите...");

					FQuester::UnpackQAL(buf, path);
					tb_nAutor->setText(qal.autor);
					tb_nPack->setText(qal.name);

					FQuester::Res::aud_shutdown.play();
					SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Builder", "QAL file was unpacked.", FGUI::p_window);
				}
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
} fw_startWindow;