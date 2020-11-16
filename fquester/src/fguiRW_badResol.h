#include "fgui.h"
#include "SystemConsole.h"
#include "fgui_RWGlobalVars.h"

struct BadResol {
	FGUI::Window* fwin;

	FGUI::Elements::Button* button;
	FGUI::Elements::Text* text;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"                                          FQuester");

		fwin->deleteIcon();
		fwin->setSize(FGUI::Vector2u(505, 80));
		fwin->setBlockMove(true); fwin->setBlockSize(true);
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		fwin->setButtonsBar(NULL);

		text = FGUI::Elements::CreateText(FGUI::Vector2i(15, 15), L"Невозможно открыть редактор при разрешении меньше 1024x768!");
		fwin->tieElement(text);

		button = FGUI::Elements::CreateButton(FGUI::Vector2i(220, 46), L"    Ок    ");
		fwin->tieElement(button);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (button->getPressed()) {
			fwin->hide();
		}
	}
	void checkEvents() {
		if (activateBadResol) { if (!fwin->getShowed()) { fwin->activate(); fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER); } activateBadResol = false; }
	}
	void update() {
		if (fwin->getWasResized()) {
		}
	}
} fw_badResol;