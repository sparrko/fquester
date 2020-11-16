#include "fgui.h"
#include "SystemConsole.h"
#include "fgui_RWGlobalVars.h"

struct NamesList {
	FGUI::Window* fwin;

	FGUI::Elements::List* list;
	FGUI::Elements::TextBox* tb;
	FGUI::Elements::Button* del;
	FGUI::Elements::Button* add;

	bool first;

	void updateList() {
		list->clear();
		for (unsigned int i = 0; i < FQuester::FNM::names.size(); i++) {
			list->addText(FQuester::FNM::names[i]);
		}
	}
	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"Список совпадений");

		fwin->setIcon(FGUI::WinCh::DefIcons::WINDOW_DEF_ICON_SETTINGS);
		fwin->setSize(FGUI::Vector2u(310, 350));
		fwin->setBlockSize(true);
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		fwin->setButtonsBar(FGUI::WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE);


		list = FGUI::Elements::CreateList(FGUI::Vector2i(5, 5), FGUI::Vector2u(300, 280));
		fwin->tieElement(list);
		first = true;

		tb = FGUI::Elements::CreateTextBox(FGUI::Vector2i(5, 290), 300);
		fwin->tieElement(tb);

		del = FGUI::Elements::CreateButton(FGUI::Vector2i(5, 320), L"Удалить");
		fwin->tieElement(del);

		add = FGUI::Elements::CreateButton(FGUI::Vector2i(216, 320), L"Добавить");
		fwin->tieElement(add);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (del->getEventPressing()) {
			if (list->getCursorD() >= 0 && !list->getTexts()->empty()) {
				FQuester::FNM::Delete(list->getCursor());
				FQuester::FNM::Update();
				updateList();
			}
			else {
				FQuester::Res::aud_warning.play();
			}
		}
		if (add->getEventPressing()) {
			if (!tb->getText().empty()) {
				FQuester::FNM::Add(tb->getText());
				tb->clear();
				FQuester::FNM::Update();
				updateList();
			}
			else {
				FQuester::Res::aud_warning.play();
			}
		}
	}
	void checkEvents() {
		if (activateNameList) { if (!fwin->getShowed()) { fwin->activate(); fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER); } activateNameList = false; }
		if (first) { first = false; updateList(); }
	}
	void update() {
		if (fwin->getWasResized()) {
		}
	}
} fw_namesList;