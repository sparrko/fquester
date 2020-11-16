#pragma once

#include "fgui.h"
#include "res.h"

// Crash test
sf::Clock cl_ch; int t_ch = 600; int t_chm1 = 4; int t_chm2 = 1; 

bool crash1b = false;
bool crash2b = false;
bool crash3b = false;

void crash1() {
	if (cl_ch.getElapsedTime().asMilliseconds() > t_ch) {
		
		cl_ch.restart(); if (t_ch > 140) t_ch -= t_chm1; 
		else { t_ch -= t_chm2; FGUI::Vars::Window::hatSize = rand() % 100; FGUI::Vars::Window::borderSize = rand() % 50; FGUI::Vars::Window::c_rects.r = rand() % 255; FGUI::Vars::Window::c_rects.g = rand() % 255; FGUI::Vars::Window::c_rects.b = rand() % 255; }
		if (t_ch < 10) t_ch = 10;
		FGUI::Window* fwin;
		fwin = FGUI::FCreateWindow();
		fwin->init();
		std::wstring ws = L"CrashTest: " + std::to_wstring((long long)FGUI::getCountsWindow()); fwin->setCaption(ws);
		FGUI::Vector2u v; v.x = rand() % (FGUI::sizeDisplay.x - 100); v.y = rand() % (FGUI::sizeDisplay.y - 100); fwin->setSize(v);
		FGUI::Color cl; cl.r = rand() % 255; cl.g = rand() % 255; cl.b = rand() % 255; cl.a = 255; fwin->setCenterColor(cl);
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);

		for (int a = 0; a < 255; a++) {
			int r = rand() % 6;
			switch (r) {
			case 0:
				FGUI::Elements::Text * st;
				st = FGUI::Elements::CreateText(FGUI::Vector2i(rand() % fwin->getSize().x, rand() % fwin->getSize().y), L"Simple Text");
				fwin->tieElement(st);
				break;
			case 1:
				FGUI::Elements::Button * sb;
				sb = FGUI::Elements::CreateButton(FGUI::Vector2i(rand() % fwin->getSize().x, rand() % fwin->getSize().y), L"Simple Button");
				fwin->tieElement(sb);
				break;
			case 2:
				FGUI::Elements::Panel * sp;
				sp = FGUI::Elements::CreatePanel(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), FGUI::Vector2u(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2));
				fwin->tieElement(sp);
				break;
			case 3:
				FGUI::Elements::TextBox * stb;
				stb = FGUI::Elements::CreateTextBox(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), rand() % 100);
				fwin->tieElement(stb);
				break;
			case 4:
				FGUI::Elements::CheckBox * scb;
				scb = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), L"Simple CheckBox");
				fwin->tieElement(scb);
				break;
			case 5:
				FGUI::Elements::ConsoleList * sl;
				sl = FGUI::Elements::CreateConsoleList(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), FGUI::Vector2u(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2));
				sl->addText(L"dead inside"); sl->addText(L"dead inside"); sl->addText(L"dead inside");
				fwin->tieElement(sl);
				break;
			}
		}

		FGUI::Elements::Panel* p;
		p = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 5), FGUI::Vector2u(100, 50));
		fwin->tieElement(p);

		FGUI::Elements::Text* t;
		t = FGUI::Elements::CreateText(FGUI::Vector2i(10, 10), L"winint: 255");
		fwin->tieElement(t);

		FGUI::Elements::Text* t2;
		t2 = FGUI::Elements::CreateText(FGUI::Vector2i(10, 30), L"t_ch: " + std::to_wstring((long long)t_ch));
		fwin->tieElement(t2);

		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		fwin->activate();
		FQuester::Res::aud_press.play();
	}
};

bool crash2() {
	if (FGUI::getCountsWindow() < 255) {

		if (cl_ch.getElapsedTime().asMilliseconds() > t_ch) {

			cl_ch.restart(); if (t_ch > 140) t_ch -= t_chm1;
			else { t_ch -= t_chm2; }
			if (t_ch < 10) t_ch = 10;
			FGUI::Window* fwin;
			fwin = FGUI::FCreateWindow();
			fwin->init();
			std::wstring ws = L"CrashTest: " + std::to_wstring((long long)FGUI::getCountsWindow()); fwin->setCaption(ws);
			FGUI::Vector2u v; v.x = rand() % (FGUI::sizeDisplay.x - 100); v.y = rand() % (FGUI::sizeDisplay.y - 100); fwin->setSize(v);
			FGUI::Color cl; cl.r = rand() % 255; cl.g = rand() % 255; cl.b = rand() % 255; cl.a = 255; fwin->setCenterColor(cl);
			fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);

			for (int a = 0; a < 255; a++) {
				int r = rand() % 6;
				switch (r) {
				case 0:
					FGUI::Elements::Text * st;
					st = FGUI::Elements::CreateText(FGUI::Vector2i(rand() % fwin->getSize().x, rand() % fwin->getSize().y), L"Simple Text");
					fwin->tieElement(st);
					break;
				case 1:
					FGUI::Elements::Button * sb;
					sb = FGUI::Elements::CreateButton(FGUI::Vector2i(rand() % fwin->getSize().x, rand() % fwin->getSize().y), L"Simple Button");
					fwin->tieElement(sb);
					break;
				case 2:
					FGUI::Elements::Panel * sp;
					sp = FGUI::Elements::CreatePanel(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), FGUI::Vector2u(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2));
					fwin->tieElement(sp);
					break;
				case 3:
					FGUI::Elements::TextBox * stb;
					stb = FGUI::Elements::CreateTextBox(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), rand() % 100);
					fwin->tieElement(stb);
					break;
				case 4:
					FGUI::Elements::CheckBox * scb;
					scb = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), L"Simple CheckBox");
					fwin->tieElement(scb);
					break;
				case 5:
					FGUI::Elements::ConsoleList * sl;
					sl = FGUI::Elements::CreateConsoleList(FGUI::Vector2i(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2), FGUI::Vector2u(rand() % fwin->getSize().x / 2, rand() % fwin->getSize().y / 2));
					sl->addText(L"dead inside"); sl->addText(L"dead inside"); sl->addText(L"dead inside");
					fwin->tieElement(sl);
					break;
				}
			}

			FGUI::Elements::Panel* p;
			p = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 5), FGUI::Vector2u(100, 50));
			fwin->tieElement(p);

			FGUI::Elements::Text* t;
			t = FGUI::Elements::CreateText(FGUI::Vector2i(10, 10), L"winint: 255");
			fwin->tieElement(t);

			FGUI::Elements::Text* t2;
			t2 = FGUI::Elements::CreateText(FGUI::Vector2i(10, 30), L"t_ch: " + std::to_wstring((long long)t_ch));
			fwin->tieElement(t2);

			fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
			fwin->activate();
			FQuester::Res::aud_time.play();

			return true;
		}

	}
	else { return false; }
};

void crash3() {
if (cl_ch.getElapsedTime().asMilliseconds() > t_ch) {

			cl_ch.restart(); if (t_ch > 140) t_ch -= t_chm1;
			else { t_ch -= t_chm2; }
			if (t_ch < 10) t_ch = 10;
			FGUI::Window* fwin;
			fwin = FGUI::FCreateWindow();
			fwin->init();
			std::wstring ws = L"CrashTest: " + std::to_wstring((long long)FGUI::getCountsWindow()); fwin->setCaption(ws);
			FGUI::Vector2u v; v.x = rand() % (FGUI::sizeDisplay.x - 100); v.y = rand() % (FGUI::sizeDisplay.y - 100); fwin->setSize(v);
			FGUI::Color cl; cl.r = rand() % 255; cl.g = rand() % 255; cl.b = rand() % 255; cl.a = 255; fwin->setCenterColor(cl);
			fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);

			FGUI::Elements::Panel* p;
			p = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 5), FGUI::Vector2u(100, 50));
			fwin->tieElement(p);

			FGUI::Elements::Text* t;
			t = FGUI::Elements::CreateText(FGUI::Vector2i(10, 10), L"winint: 255");
			fwin->tieElement(t);

			FGUI::Elements::Text* t2;
			t2 = FGUI::Elements::CreateText(FGUI::Vector2i(10, 30), L"t_ch: " + std::to_wstring((long long)t_ch));
			fwin->tieElement(t2);

			fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
			fwin->activate();
			FQuester::Res::aud_press.play();
	}
};