#include "fgui.h"
#include "SystemConsole.h"
#include "fgui_RWGlobalVars.h"

struct ExitAsk {
	FGUI::Window* fwin;

	FGUI::Elements::Button* fbuttons[2];
	FGUI::Elements::Text* text;
	FGUI::Elements::ConsoleList* clsList;
	int cursor;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"                     FQuester");

		fwin->deleteIcon();
		fwin->setSize(FGUI::Vector2u(294, 80));
		fwin->setBlockMove(true); fwin->setBlockSize(true);
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		fwin->setButtonsBar(NULL);

		text = FGUI::Elements::CreateText(FGUI::Vector2i(30, 15), L"Вы действительно хотите выйти?");
		fwin->tieElement(text);

		fbuttons[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(210, 46), L"    Да    ");
		fwin->tieElement(fbuttons[1]);

		fbuttons[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(20, 46), L"    Нет    ");
		fwin->tieElement(fbuttons[0]);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (fbuttons[1]->getPressed()) {
			sc.sendMessage("exit");
		}
		if (fbuttons[0]->getPressed()) {
			fwin->hide();
		}
	}
	void checkEvents() {
		if (activateExitAsk) { if (!fwin->getShowed()) { fwin->activate(); fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER); } activateExitAsk = false; }
	}
	void update() {
		if (fwin->getWasResized()) {
		}
	}
} fw_exitAsk;