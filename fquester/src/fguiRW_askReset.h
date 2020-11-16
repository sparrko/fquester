#include "fgui.h"
#include "SystemConsole.h"
#include "fgui_RWGlobalVars.h"

struct AskReset {
	FGUI::Window* fwin;

	FGUI::Elements::Button* fbuttons[2];
	FGUI::Elements::Text* text;
	FGUI::Elements::Text* text2;
	FGUI::Elements::ConsoleList* clsList;
	int cursor;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"                               FQuester");

		fwin->deleteIcon();
		fwin->setSize(FGUI::Vector2u(400, 98));
		fwin->setBlockMove(true); fwin->setBlockSize(true);
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		fwin->setButtonsBar(NULL);

		text = FGUI::Elements::CreateText(FGUI::Vector2i(15, 15), L"Вы действительно хотите удалить все данные игры?"); fwin->tieElement(text);
		text2 = FGUI::Elements::CreateText(FGUI::Vector2i(30, 35), L"Сотрутся все игровые паки и конфиги настроек!"); fwin->tieElement(text2);

		fbuttons[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(318, 65), L"    Да    ");
		fwin->tieElement(fbuttons[1]);

		fbuttons[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(20, 65), L"    Нет    ");
		fwin->tieElement(fbuttons[0]);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (fbuttons[1]->getEventPressing()) {
			// ResetInfo();
			sc.sendMessage("exit");
		}
		if (fbuttons[0]->getPressed()) {
			fwin->hide();
		}
	}
	void checkEvents() {
		if (activateAskReset) { if (!fwin->getShowed()) { fwin->activate(); fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER); } activateAskReset = false; }
	}
	void update() {
		if (fwin->getWasResized()) {
		}
	}
} fw_askReset;