#include "fgui.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "fgui_RWGlobalVars.h"
#include "q_globalVars.h"
#include "3D.h"
#include "objPars.h"

struct CPUTest {
	FGUI::Window* fwin;
	FGUI::Elements::Text* t_result;
	FGUI::Elements::Button* b_go;

	FGUI::Elements::Serf* serf;
	SDL_Surface* sdlf;

	sf::Clock t;
	sf::Clock t2;
	uint16_t frame;

	fs::Model* test_mdl;
	sf::Vector3f rw;
	sf::Vector3f pw;
	
	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"CPU Test");
		fwin->deleteIcon(); fwin->setButtonsBar(FGUI::WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE);
		fwin->setBlockSize(true);
		fwin->setSize(FGUI::Vector2u(512, 384 + 30));
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		
		t_result = FGUI::Elements::CreateText(FGUI::Vector2i(5, 389), L"Result: 0ms");
		fwin->tieElement(t_result);

		b_go = FGUI::Elements::CreateButton(FGUI::Vector2i(420, 387), L"Start Test");
		fwin->tieElement(b_go);

		sdlf = SDL_CreateRGBSurface(0, 512, 384, 32, NULL, NULL, NULL, NULL);
		
		sr::serf = sdlf;

		test_mdl = fs::LoadModel("resources/berserker.obj");

		serf = FGUI::Elements::CreateSerf(FGUI::Vector2i(0, 0), FGUI::Vector2u(512, 384), sdlf);
		fwin->tieElement(serf);
		
		rw = sf::Vector3f(90, 540, 0);
		pw = sf::Vector3f(-1, -2, -10);

		frame = 0;

		fwin->hide();
	}
	void checkFguiEvents() {
		if (activateCPUTest) { if (!fwin->getShowed()) { fwin->activate(); } activateCPUTest = false; }
		if (fwin->getShowed()) {
			if (b_go->getPressed()) { rw.y = 180; t2.restart(); n_frameLimit = 0; }
			if (rw.y < 540) {
				if (t.getElapsedTime().asMilliseconds() > 1000) {
					t.restart();

					frame = 0;
				}
				else {
					frame++;
				}
				
				sr::SetPrimary();

				sr::Rotatef(rw);
				sr::Translatef(pw);

				SDL_FillRect(sdlf, NULL, NULL);
				sf::Vector3f* tr = new sf::Vector3f[3]; fs::Triangle t;

				rw.y++;

				for (int a = 0; a < test_mdl->GetCountTriangles(); a++) {
					tr = test_mdl->GetTriangle(a).v;
					sr::DrawLineOfTriangle(tr);
				}

				serf->setImage(sdlf);

				if (rw.y == 539) {
					t_result->setText("Result: " + std::to_string((long long)t2.getElapsedTime().asMilliseconds()) + "ms");
					n_frameLimit = FRAME_LIMIT;
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
} fw_CPUTest;