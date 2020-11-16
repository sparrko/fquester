#include "fgui.h"
#include "SystemConsole.h"
#include "fgui_RWGlobalVars.h"

int FRECUP_MG = 80;

struct MediaGuide {
	FGUI::Window* fwin;

	sf::Clock cl1;

	// Up
	FGUI::Elements::Panel* p1;
	FGUI::Elements::Text* t_time;	FGUI::Elements::Text* t_rtime;
	FGUI::Elements::Text* t_frame;	FGUI::Elements::Text* t_rframe;

	// Down
	FGUI::Elements::Panel* p2;
	FGUI::Elements::Button *b_back, *b_plst, *b_forw, *b_step;
	FGUI::Elements::CheckBox* cb_repeat, * cb_track; bool tcb_repeat, tcb_track;
	FGUI::Elements::Button* b_skip, * b_tost;
	FGUI::Elements::Text* t_pitch; FGUI::Elements::Button* b_pp, * b_pm; FGUI::Elements::Text* t_rpitch;
	FGUI::Elements::Text* t_volume; FGUI::Elements::Button* b_vp, * b_vm; FGUI::Elements::Text* t_rvolume;

	void init() {

		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"Media Guide");

		fwin->setIcon(FGUI::WinCh::DefIcons::WINDOW_DEF_ICON_EXPLORER);
		fwin->setSize(FGUI::Vector2u(210, 60 + 120 + 5 + 15 + 30 + 10));
		fwin->setBlockSize(true);
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_NONE);
		fwin->setButtonsBar(FGUI::WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE);

		// Up
		
		p1 = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 5), FGUI::Vector2u(200, 50)); fwin->tieElement(p1);		
		
		t_time = FGUI::Elements::CreateText(FGUI::Vector2i(10, 10), L"Time: "); t_time->setClearColor(p1->getColor()); fwin->tieElement(t_time);
		t_rtime = FGUI::Elements::CreateText(FGUI::Vector2i(70, 10), L"1000.28s / 1128.69s"); t_rtime->setClearColor(p1->getColor()); fwin->tieElement(t_rtime);
		
		t_frame = FGUI::Elements::CreateText(FGUI::Vector2i(10, 30), L"Frame: "); t_frame->setClearColor(p1->getColor()); fwin->tieElement(t_frame);
		t_rframe = FGUI::Elements::CreateText(FGUI::Vector2i(70, 30), L"24000 / 24000"); t_rframe->setClearColor(p1->getColor()); fwin->tieElement(t_rframe);

		// Down

		p2 = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 60), FGUI::Vector2u(200, 175)); fwin->tieElement(p2);

		b_back = FGUI::Elements::CreateButton(FGUI::Vector2i(10, 65), L"<"); b_back->setClearColor(p2->getColor()); fwin->tieElement(b_back);
		b_plst = FGUI::Elements::CreateButton(FGUI::Vector2i(43, 65), L"Stop"); b_plst->setClearColor(p2->getColor()); b_plst->setSize(FGUI::Vector2u(52, b_plst->getSize().y)); fwin->tieElement(b_plst);
		b_forw = FGUI::Elements::CreateButton(FGUI::Vector2i(100, 65), L">"); b_forw->setClearColor(p2->getColor()); fwin->tieElement(b_forw);
		b_step = FGUI::Elements::CreateButton(FGUI::Vector2i(150, 65), L"x3"); b_step->setClearColor(p2->getColor()); fwin->tieElement(b_step);

		cb_repeat = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(10, 118), L"Повтор"); cb_repeat->setClearColor(p2->getColor()); fwin->tieElement(cb_repeat); tcb_repeat = false;
		cb_track = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(10, 145), L"Прогресс"); cb_track->setClearColor(p2->getColor()); cb_track->setChecked(true); fwin->tieElement(cb_track); tcb_track = true;

		b_tost = FGUI::Elements::CreateButton(FGUI::Vector2i(115, 142), L"В начало"); b_tost->setClearColor(p2->getColor()); fwin->tieElement(b_tost);
		b_skip = FGUI::Elements::CreateButton(FGUI::Vector2i(115, 115), L"Пропуск"); b_skip->setClearColor(p2->getColor()); b_skip->setSize(b_tost->getSize()); fwin->tieElement(b_skip);
		

		t_pitch = FGUI::Elements::CreateText(FGUI::Vector2i(10, 175), L"Питча: "); t_pitch->setClearColor(p2->getColor()); fwin->tieElement(t_pitch);
		b_pm = FGUI::Elements::CreateButton(FGUI::Vector2i(98, 173), L"-"); b_pm->setClearColor(p2->getColor()); fwin->tieElement(b_pm);
		b_pp = FGUI::Elements::CreateButton(FGUI::Vector2i(175, 173), L"+"); b_pp->setClearColor(p2->getColor()); b_pp->setSize(b_pm->getSize()); fwin->tieElement(b_pp);
		t_rpitch = FGUI::Elements::CreateText(FGUI::Vector2i(135, 175), L"1.25"); t_rpitch->setClearColor(p2->getColor()); fwin->tieElement(t_rpitch);

		t_volume = FGUI::Elements::CreateText(FGUI::Vector2i(10, 205), L"Громкость: "); t_volume->setClearColor(p2->getColor()); fwin->tieElement(t_volume);
		b_vm = FGUI::Elements::CreateButton(FGUI::Vector2i(98, 198 + 5), L"-"); b_vm->setClearColor(p2->getColor()); fwin->tieElement(b_vm);
		b_vp = FGUI::Elements::CreateButton(FGUI::Vector2i(175, 198 + 5), L"+"); b_vp->setClearColor(p2->getColor()); b_vp->setSize(b_vm->getSize()); fwin->tieElement(b_vp);
		t_rvolume = FGUI::Elements::CreateText(FGUI::Vector2i(130, 205), L"100%"); t_rvolume->setClearColor(p2->getColor()); fwin->tieElement(t_rvolume);

		fwin->hide();
	}
	void checkFguiEvents() {
		if (b_step->getEventPressing()) {
			if (FQuester::moveTick == 1) FQuester::moveTick = 3;
			else if (FQuester::moveTick == 3) FQuester::moveTick = 10;
			else if (FQuester::moveTick == 10) FQuester::moveTick = 30;
			else if (FQuester::moveTick == 30) FQuester::moveTick = 1;

			b_step->setText(L"x" + std::to_wstring((long long)FQuester::moveTick));

			b_step->reset();
		}

		if (b_back->getEventPressing()) {
			if (FQuester::s_sound.getStatus() == sf::Sound::Status::Stopped) { FQuester::s_sound.play(); FQuester::s_sound.setPlayingOffset(sf::milliseconds(FQuester::sb_sound.getDuration().asMilliseconds() - SHOWERASK_MUSIC_DIVMOVE)); }
			else {
				float move = FQuester::s_sound.getPlayingOffset().asMilliseconds() - (1000. * FQuester::moveTick);
				if (move < 0) move = 0;
				FQuester::s_sound.setPlayingOffset(sf::milliseconds(move));
			}
			if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());

			b_back->reset();
		}
		if (b_forw->getEventPressing()) {
			float move = FQuester::s_sound.getPlayingOffset().asMilliseconds() + (1000. * FQuester::moveTick);
			if (move > FQuester::sb_sound.getDuration().asMilliseconds() - (1000. * FQuester::moveTick)) move = FQuester::sb_sound.getDuration().asMilliseconds() - 5;
			FQuester::s_sound.setPlayingOffset(sf::milliseconds(move));
			if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());

			b_forw->reset();
		}

		if (b_vp->getEventPressing()) {
			float move = FQuester::s_sound.getVolume() + (1 * FQuester::moveTick); if (move > 100.) move = 100.;
			FQuester::s_sound.setVolume(move);

			b_vp->reset();
		}
		if (b_vm->getEventPressing()) {
			float move = FQuester::s_sound.getVolume() - (1 * FQuester::moveTick); if (move < 0) move = 0;
			FQuester::s_sound.setVolume(move);

			b_vm->reset();
		}

		if (b_pp->getEventPressing()) {
			float move = FQuester::s_sound.getPitch() + (0.01 * FQuester::moveTick); if (move > 3.) move = 3.;
			FQuester::s_sound.setPitch(move);
			if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());

			b_pp->reset();
		}
		if (b_pm->getEventPressing()) {
			FQuester::s_sound.setPitch(FQuester::s_sound.getPitch() - (0.01 * FQuester::moveTick));
			if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());

			b_pm->reset();
		}


		if (b_plst->getPressed()) {
			if (FQuester::s_sound.getStatus() == sf::Sound::Status::Paused) FQuester::s_sound.play();
			else if (FQuester::s_sound.getStatus() == sf::Sound::Status::Stopped) FQuester::s_sound.play();
			else if (FQuester::s_sound.getStatus() == sf::Sound::Status::Playing) FQuester::s_sound.pause();
			if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());

			if (FQuester::s_sound.getStatus() == sf::Sound::Status::Paused) { b_plst->setText(L"Play"); b_plst->setSize(FGUI::Vector2u(52, b_plst->getSize().y)); }
			else { b_plst->setText(L"Stop"); }

			b_plst->reset();
		}
		if (b_skip->getPressed()) {
			FQuester::s_sound.stop();

			fwin->hide();
			b_skip->reset();
		}
		if (b_tost->getPressed()) {
			if (FQuester::sa_part != 4) FQuester::sa_part--;

			if (FQuester::type == 'A') {
				if (settings::s_contentTypeMode == 0 && FQuester::sa_part < 2) { FQuester::s_sound.play(); }
				else if (settings::s_contentTypeMode == 1 && FQuester::sa_part < 1) { FQuester::s_sound.play(); }
			}
			if (FQuester::type == 'V') {
				if (settings::s_contentTypeModeV == 0 && FQuester::sa_part < 2) { FQuester::cap.set(CV_CAP_PROP_POS_MSEC, 0.); FQuester::s_sound.play(); }
				else if (settings::s_contentTypeModeV == 1 && FQuester::sa_part < 1) { FQuester::cap.set(CV_CAP_PROP_POS_MSEC, 0.); FQuester::s_sound.play(); }
			}

			b_tost->reset();
		}

		
	}
	void checkEvents() {
		if (activateMediaGuide) { 
			cb_repeat->setChecked(FQuester::s_sound.getLoop());
			cb_track->setChecked(FQuester::showPosMedia);

			if (!fwin->getShowed()) { fwin->activate(); } activateMediaGuide = false; 
		}
		if (event.type == SDL_KEYDOWN && FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_TAB && FQuester::s_sound.getStatus() == sf::Sound::Status::Playing) {
			cb_repeat->setChecked(FQuester::s_sound.getLoop());
			cb_track->setChecked(FQuester::showPosMedia);

			if (!fwin->getShowed()) { fwin->activate(); } else fwin->hide();
		}
		if (fwin->getShowed()) {
			if (event.type == SDL_KEYDOWN) {
				if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
					if (FQuester::s_sound.getStatus() == sf::Sound::Status::Paused) FQuester::s_sound.play();
					else if (FQuester::s_sound.getStatus() == sf::Sound::Status::Stopped) FQuester::s_sound.play();
					else if (FQuester::s_sound.getStatus() == sf::Sound::Status::Playing) FQuester::s_sound.pause();
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}

				if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_UP) {
					float move = FQuester::s_sound.getVolume() + 5; if (move > 100.) move = 100.;
					FQuester::s_sound.setVolume(move);
				}
				else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_DOWN) {
					float move = FQuester::s_sound.getVolume() - 5; if (move < 0) move = 0;
					FQuester::s_sound.setVolume(move);
				}

				if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_O) {
					FQuester::s_sound.setPitch(FQuester::s_sound.getPitch() - 0.1);
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}
				else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_P) {
					float move = FQuester::s_sound.getPitch() + 0.1; if (move > 3.) move = 3.;
					FQuester::s_sound.setPitch(move);
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}
				else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_K) {
					FQuester::s_sound.setPitch(FQuester::s_sound.getPitch() - 0.01);
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}
				else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_L) {
					float move = FQuester::s_sound.getPitch() + 0.01; if (move > 3.) move = 3.;
					FQuester::s_sound.setPitch(move);
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}
				else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_M) {
					FQuester::s_sound.setPitch(1.);
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}

				if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_LEFT) {
					if (FQuester::s_sound.getStatus() == sf::Sound::Status::Stopped) { FQuester::s_sound.play(); FQuester::s_sound.setPlayingOffset(sf::milliseconds(FQuester::sb_sound.getDuration().asMilliseconds() - SHOWERASK_MUSIC_DIVMOVE)); }
					else {
						float move = FQuester::s_sound.getPlayingOffset().asMilliseconds() - SHOWERASK_MUSIC_DIVMOVE;
						if (move < 0) move = 0;
						FQuester::s_sound.setPlayingOffset(sf::milliseconds(move));
					}
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}
				if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					float move = FQuester::s_sound.getPlayingOffset().asMilliseconds() + SHOWERASK_MUSIC_DIVMOVE;
					if (move > FQuester::sb_sound.getDuration().asMilliseconds() - 5) move = FQuester::sb_sound.getDuration().asMilliseconds() - 5;
					FQuester::s_sound.setPlayingOffset(sf::milliseconds(move));
					if (FQuester::type == 'V') FQuester::cap.set(CV_CAP_PROP_POS_MSEC, FQuester::s_sound.getPlayingOffset().asMilliseconds());
				}

				if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_R) {
					FQuester::s_sound.setLoop(!FQuester::s_sound.getLoop());
				}

				if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_T) {
					FQuester::showPosMedia = !FQuester::showPosMedia;
				}
			}
		}
	}
	void update() {
		if (fwin->getShowed()) {
			if (cl1.getElapsedTime().asMilliseconds() > 1000. / FRECUP_MG || settings::s_vsync) {
				cl1.restart();

				try {
					std::string bufs;

					std::string num = std::to_string((long double)((int)(FQuester::s_sound.getPlayingOffset().asSeconds() * 100)) / 100.);
					num = num.substr(0, num.find(',') + 3); ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					bufs = num + "s";
					bufs += " / ";
					num = std::to_string((long double)((int)(FQuester::sb_sound.getDuration().asSeconds() * 100)) / 100.);
					num = num.substr(0, num.find(',') + 3);
					bufs += num + "s";
					t_rtime->setText(bufs);

					bufs = std::to_string((long long)FQuester::cap.get(CV_CAP_PROP_POS_FRAMES));
					bufs += " / ";
					bufs += std::to_string((long long)FQuester::cap.get(CV_CAP_PROP_FRAME_COUNT));
					t_rframe->setText(bufs);

					num = std::to_string((long double)FQuester::s_sound.getPitch());
					num = num.substr(0, num.find(',') + 3);
					bufs = num;
					t_rpitch->setText(bufs);

					num = std::to_string((long double)FQuester::s_sound.getVolume());
					num = num.substr(0, num.find(','));
					bufs = num;
					t_rvolume->setText(bufs);

					if (tcb_repeat != cb_repeat->getChecked()) {
						tcb_repeat = cb_repeat->getChecked();
						FQuester::s_sound.setLoop(cb_repeat->getChecked());
					}

					if (tcb_track != cb_track->getChecked()) {
						tcb_track = cb_track->getChecked();
						FQuester::showPosMedia = (cb_track->getChecked());
					}

					if (FQuester::s_sound.getStatus() == sf::Sound::Status::Stopped && b_plst->getText() == L"Stop") { b_plst->setText(L"Play"); b_plst->setSize(FGUI::Vector2u(52, b_plst->getSize().y)); }
					else if (FQuester::s_sound.getStatus() == sf::Sound::Status::Paused && b_plst->getText() == L"Stop") { b_plst->setText(L"Play"); b_plst->setSize(FGUI::Vector2u(52, b_plst->getSize().y)); }
					else if (FQuester::s_sound.getStatus() == sf::Sound::Status::Playing && b_plst->getText() == L"Play") { b_plst->setText(L"Stop"); }
				}
				catch (exception e) {
					
				}
			}
		}
	}
} fw_mediaGuide;