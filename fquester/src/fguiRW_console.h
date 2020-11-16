#include "fgui.h"
#include "SystemConsole.h"
#include "fgui_RWGlobalVars.h"
#include "settings.h"
#include "game_globalVars.h"

struct Console {
	FGUI::Window* fwin;

	FGUI::Elements::Button* fbInput;
	FGUI::Elements::TextBox* textBox;
	FGUI::Elements::ConsoleList* clsList;
	int cursor;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"Console");

		fwin->setIcon(FGUI::WinCh::DefIcons::WINDOW_DEF_ICON_CONSOLE);
		fwin->setSize(FGUI::Vector2u(500, 380));
		fwin->setMinSize(FGUI::Vector2u(200, 120));
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_NONE);
		fwin->setButtonsBar(FGUI::WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE | FGUI::WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN);
		
		SDL_RendererInfo info = { 0 };
		SDL_GetRendererInfo(renderer, &info);
		if (std::string(info.name).substr(0, 6) == "opengl") fwin->setBlockSize(true);

		fbInput = FGUI::Elements::CreateButton(FGUI::Vector2i(1, 1), L" Enter ");
		fwin->tieElement(fbInput);

		textBox = FGUI::Elements::CreateTextBox(FGUI::Vector2i(1, 1), 1);
		textBox->setMaxChars(100);
		fwin->tieElement(textBox);

		cursor = 0;

		clsList = FGUI::Elements::CreateConsoleList(FGUI::Vector2i(5, 5), FGUI::Vector2u(10, 10));
		fwin->tieElement(clsList);

		fwin->hide();
 
	}
	void checkFguiEvents() {
		if (activateConsole) { if (!fwin->getShowed()) { fwin->activate(); } activateConsole = false; }
		if (fbInput->getPressed() || textBox->getPressedEnter()) {
			sc.sendMessage(textBox->getText());
			textBox->clear();

			if (textBox->getPressedEnter()) {
				cursor = clsList->getTexts()->size() - 1;
			}
		}

	}
	void checkEvents() {
		if (m_part == 2) {
			if (FGUI::p_event->type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_F2) { if (!fw_console.fwin->getLife()) { fw_console.fwin->activate(); clsList->setDown(clsList->getLimitDown()); clsList->draw(); textBox->setCursor(); } else fw_console.fwin->hide(); }
		}
		else {
			if (FGUI::p_event->type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_GRAVE) { if (!fw_console.fwin->getLife()) { fw_console.fwin->activate(); clsList->setDown(clsList->getLimitDown()); clsList->draw(); textBox->setCursor(); } else fw_console.fwin->hide(); }
		}

		if (FGUI::p_event->type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_UP) {
			if (fw_console.fwin->getLife()) {
				cursor--;
				if (cursor < 0) cursor = clsList->getTexts()->size() - 1;
				textBox->setText(clsList->getText(cursor).substr(2, clsList->getText(cursor).size() - 2));
			}
		}
		
		if (FGUI::p_event->type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_DOWN) {
			if (fw_console.fwin->getLife()) {
				cursor++;
				if (cursor > clsList->getTexts()->size() - 1) cursor = 0;
				textBox->setText(clsList->getText(cursor).substr(2, clsList->getText(cursor).size() - 2));
			}
		}
	}
	void update() {
		while (true) {
			auto lnFDw = sc.getLineForDraw();
			if (lnFDw == L"") break;
			clsList->addText(lnFDw);
		}

		if (sc.getNeedCls()) {
			clsList->cls();
		}

		if (fwin->getWasResized()) {
			fbInput->setPosition(FGUI::Vector2i(fwin->getSize().x - 70, fwin->getSize().y - 30));
			fbInput->noClear();

			textBox->setSize(fwin->getSize().x - 80);
			textBox->setPosition(FGUI::Vector2i(5, fwin->getSize().y - 30));
			textBox->noClear();

			clsList->setSize(FGUI::Vector2u(fwin->getSize().x - 10, fwin->getSize().y - 40));
			clsList->noClear();
		}
	}
} fw_console;