#include "fgui.h"
#include "SystemConsole.h"
#include "fgui_RWGlobalVars.h"
#include "fprUnit.h"

struct ReallyDelete {
	FGUI::Window* fwin;

	FGUI::Elements::Button* fbuttons[2];
	FGUI::Elements::Text* text;
	FGUI::Elements::ConsoleList* clsList;
	int cursor;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"                                  FRP");

		fwin->deleteIcon();
		fwin->setSize(FGUI::Vector2u(370, 80));
		fwin->setBlockMove(true); fwin->setBlockSize(true);
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		fwin->setButtonsBar(NULL);

		text = FGUI::Elements::CreateText(FGUI::Vector2i(30, 15), L"Вы действительно хотите удалить этот пак?");
		fwin->tieElement(text);

		fbuttons[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(290, 46), L"    Да    ");
		fwin->tieElement(fbuttons[1]);

		fbuttons[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(20, 46), L"    Нет    ");
		fwin->tieElement(fbuttons[0]);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (fbuttons[1]->getPressed()) {
			FQuester::FRP::Delete(FQuester::frpSelected);
			FQuester::mChangeSelect = true;
			fwin->hide();
			fbuttons[1]->reset();
		}
		if (fbuttons[0]->getPressed()) {
			fwin->hide();
		}
	}
	void checkEvents() {
		if (activateReallyDelete) { if (!fwin->getShowed()) { fwin->activate(); fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER); } activateReallyDelete = false; }
	}
	void update() {
		if (fwin->getWasResized()) {
		}
	}
} fw_reallyDelete;