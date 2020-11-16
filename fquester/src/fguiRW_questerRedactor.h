#include "fgui.h"
#include "SystemConsole.h"
#include "QAL.h"
#include "fileDialogs.h"
#include "fgui_RWGlobalVars.h"
#include "res.h"
#include "settings.h"
#include "game_globalVars.h"
#include "loadBG.h"

long long a = 0;

std::string bufap;
std::string bufvp;

struct QuesterRedactor {
	bool www, cnsl;

	FGUI::Window* fwin;
	
	bool firstRender;

	// Left
	FGUI::Elements::Text* t_tema;
	FGUI::Elements::TextBox* tb_tema;
	FGUI::Elements::Button* b_addTema;
	FGUI::Elements::Button* b_renameTema;
	FGUI::Elements::Button* b_deleteTema;
	FGUI::Elements::List* l_temi; int lastCursor;
	FGUI::Elements::Panel* p_l;

	// Left Down
	FGUI::Elements::Button* b_exit;
	FGUI::Elements::Button* b_comp;
	FGUI::Elements::CheckBox* cb_compres;
	FGUI::Elements::Panel* p_ld;

	// Right
	FGUI::Elements::Panel* p_r;
	FGUI::Elements::List* l_asks;
	FGUI::Elements::Button* b_reAsk;
	FGUI::Elements::Button* b_deleteAsk;

	// Center
	FGUI::Elements::Panel* p_c;

	int i_task; // 0 - text, 1 - image, 2 - audio, 3 - video
	FGUI::Elements::Text* t_task;
	FGUI::Elements::Button* b_sizetask[2];
	FGUI::Elements::Text* t_taskv;

	FGUI::Elements::Text* t_ask; FGUI::Elements::TextBox* tb_ask;
	FGUI::Elements::Text* t_answer; FGUI::Elements::TextBox* tb_answer;

	FGUI::Elements::Text* t_file;
	FGUI::Elements::Button* b_file;
	FGUI::Elements::TextBox* tb_file;

	FGUI::Elements::Text* t_resize; FGUI::Elements::Text* t_rule;
	FGUI::Elements::Text* t_with; FGUI::Elements::TextBox* tb_with;
	FGUI::Elements::Text* t_und; FGUI::Elements::TextBox* tb_und;


	FGUI::Elements::Text* t_counts;
	FGUI::Elements::TextBox* tb_counts;
	FGUI::Elements::Button* b_sizecounts[2]; int i_counts;
	FGUI::Elements::Button* b_addAsk;

	// Down
	FGUI::Elements::Text* t_help;
	FGUI::Elements::Text* t_version;


	void init() {
		firstRender = true;

		fwin = FGUI::FCreateWindow();
		fwin->init(); 
		fwin->setCaption(L"Редактор пака для викторины");

		fwin->setIcon(FGUI::WinCh::DefIcons::WINDOW_DEF_ICON_EXPLORER);
		fwin->setSize(FGUI::Vector2u(976, 335 + 24));
		fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER);
		fwin->setButtonsBar(0);
		fwin->setBlockSize(true);
		fwin->setCenterColor(FGUI::Color(94, 136, 183));

		// Down
		t_version = FGUI::Elements::CreateText(FGUI::Vector2i(928, 335), L"QAL1");					fwin->tieElement(t_version);
		t_help = FGUI::Elements::CreateText(FGUI::Vector2i(10, 335), L"Инструкция по редактору находится на официальном сайте. Звук сигнализирует об ошибке (смотрите в консоле \"F2\").");					t_help->setColor(FGUI::Color(30, 30, 200)); fwin->tieElement(t_help);
		
		// Right
		p_r = FGUI::Elements::CreatePanel(FGUI::Vector2i(653, 5), FGUI::Vector2u(318, 325)); p_r->setColor(FGUI::Color(153, 180, 209));	p_r->setConvex(false);	fwin->tieElement(p_r);
		b_deleteAsk = FGUI::Elements::CreateButton(FGUI::Vector2i(658, 303), L"Удалить");		fwin->tieElement(b_deleteAsk);
		b_reAsk = FGUI::Elements::CreateButton(FGUI::Vector2i(740, 303), L"Копировать");		fwin->tieElement(b_reAsk);	
		l_asks = FGUI::Elements::CreateList(FGUI::Vector2i(658, 10), FGUI::Vector2u(308, 289));		fwin->tieElement(l_asks);
		lastCursor = -1;

		// Center
		p_c = FGUI::Elements::CreatePanel(FGUI::Vector2i(329, 5), FGUI::Vector2u(318, 325)); p_c->setColor(FGUI::Color(153, 180, 209));	p_c->setConvex(false);	fwin->tieElement(p_c);
		t_task = FGUI::Elements::CreateText(); t_task->setPosition(FGUI::Vector2i(334 + 3, 14)); t_task->setText(L"Тип вопроса: "); t_task->setClearColor(p_c->getColor());					fwin->tieElement(t_task);
		t_taskv = FGUI::Elements::CreateText(); t_taskv->setPosition(FGUI::Vector2i(475, 14)); t_taskv->setClearColor(p_c->getColor());	fwin->tieElement(t_taskv);
		b_sizetask[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(437, 10), L"<");			fwin->tieElement(b_sizetask[0]);
		b_sizetask[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(612, 10), L">");			fwin->tieElement(b_sizetask[1]);
		i_task = 3;
		t_ask = FGUI::Elements::CreateText(); t_ask->setPosition(FGUI::Vector2i(337, 50)); t_ask->setText(L"Вопрос: "); t_ask->setClearColor(p_c->getColor()); fwin->tieElement(t_ask);
		tb_ask = FGUI::Elements::CreateTextBox(FGUI::Vector2i(400, 45), 240); tb_ask->setClearColor(p_c->getColor());; fwin->tieElement(tb_ask);
		t_answer = FGUI::Elements::CreateText(); t_answer->setPosition(FGUI::Vector2i(337, 77)); t_answer->setText(L"Ответ: "); t_answer->setClearColor(p_c->getColor()); fwin->tieElement(t_answer);
		tb_answer = FGUI::Elements::CreateTextBox(FGUI::Vector2i(400, 72), 240); tb_answer->setClearColor(p_c->getColor()); fwin->tieElement(tb_answer);
		t_file = FGUI::Elements::CreateText(); t_file->setPosition(FGUI::Vector2i(337, 110)); t_file->setText(L"Фаил: "); t_file->setClearColor(p_c->getColor()); fwin->tieElement(t_file);
		tb_file = FGUI::Elements::CreateTextBox(FGUI::Vector2i(400, 105), 180);	tb_file->setClearColor(p_c->getColor()); fwin->tieElement(tb_file);
		b_file = FGUI::Elements::CreateButton(FGUI::Vector2i(581, 105), L"Open"); b_file->setClearColor(p_c->getColor()); fwin->tieElement(b_file);

		t_resize = FGUI::Elements::CreateText(); t_resize->setPosition(FGUI::Vector2i(337, 148)); t_resize->setText(L"Обрезка: "); t_resize->setClearColor(p_c->getColor()); fwin->tieElement(t_resize);
		t_rule = FGUI::Elements::CreateText(); t_rule->setColor(FGUI::Color(80, 80, 80)); t_rule->setText(L"(ЧЧ:ММ:СС) || (С) || (void)"); t_rule->setClearColor(p_c->getColor()); t_rule->setPosition(FGUI::Vector2i(642 - t_rule->getSize().x, 148)); fwin->tieElement(t_rule);
		t_with = FGUI::Elements::CreateText(); t_with->setPosition(FGUI::Vector2i(393, 180)); t_with->setText(L"с"); t_with->setClearColor(p_c->getColor()); fwin->tieElement(t_with);
		tb_with = FGUI::Elements::CreateTextBox(FGUI::Vector2i(410, 175), 140); tb_with->setClearColor(p_c->getColor()); fwin->tieElement(tb_with);
		t_und = FGUI::Elements::CreateText(); t_und->setPosition(FGUI::Vector2i(364, 210)); t_und->setText(L"длит."); t_und->setClearColor(p_c->getColor()); fwin->tieElement(t_und);
		tb_und = FGUI::Elements::CreateTextBox(FGUI::Vector2i(410, 205), 140); tb_und->setClearColor(p_c->getColor()); fwin->tieElement(tb_und);

		i_counts = 0;
		t_counts = FGUI::Elements::CreateText(); t_counts->setPosition(FGUI::Vector2i(337, 279)); t_counts->setText(L"Очки: "); t_counts->setClearColor(p_c->getColor()); fwin->tieElement(t_counts);
		tb_counts = FGUI::Elements::CreateTextBox(FGUI::Vector2i(400, 274), 185); fwin->tieElement(tb_counts); tb_counts->setMaxChars(4); tb_counts->setInputOnlyNum(true);
		b_sizecounts[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(586, 274), L"-");	fwin->tieElement(b_sizecounts[0]);
		b_sizecounts[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(612, 274), L"+");	fwin->tieElement(b_sizecounts[1]);
		b_addAsk = FGUI::Elements::CreateButton(FGUI::Vector2i(497, 303), L"Добавить вопрос");			fwin->tieElement(b_addAsk);

		// Left down
		p_ld = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 275), FGUI::Vector2u(318, 55)); p_ld->setColor(FGUI::Color(153, 180, 209));	p_ld->setConvex(false);	fwin->tieElement(p_ld);
		b_exit = FGUI::Elements::CreateButton(FGUI::Vector2i(10, 303), L"Выйти из редактора");		fwin->tieElement(b_exit);
		b_comp = FGUI::Elements::CreateButton(FGUI::Vector2i(186, 303), L"Собрать проект");			fwin->tieElement(b_comp);
		cb_compres = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(10, 280), L"Сжатие (сильно ухудшает видео)"); cb_compres->setClearColor(p_ld->getColor()); cb_compres->setChecked(false);	fwin->tieElement(cb_compres);

		// Left
		p_l = FGUI::Elements::CreatePanel(FGUI::Vector2i(5, 5), FGUI::Vector2u(318, 265)); p_l->setColor(FGUI::Color(153, 180, 209));	p_l->setConvex(false);	fwin->tieElement(p_l);
		t_tema = FGUI::Elements::CreateText(); t_tema->setPosition(FGUI::Vector2i(12, 219)); t_tema->setClearColor(p_l->getColor()); t_tema->setText(L"Тема: ");					fwin->tieElement(t_tema);
		tb_tema = FGUI::Elements::CreateTextBox(FGUI::Vector2i(57, 213), 260);						fwin->tieElement(tb_tema);
		b_addTema = FGUI::Elements::CreateButton(FGUI::Vector2i(228, 242), L"Добавить");			fwin->tieElement(b_addTema);
		b_renameTema = FGUI::Elements::CreateButton(FGUI::Vector2i(92, 242), L"Переименовать");		fwin->tieElement(b_renameTema);
		b_deleteTema = FGUI::Elements::CreateButton(FGUI::Vector2i(10, 242), L"Удалить");			fwin->tieElement(b_deleteTema);
		l_temi = FGUI::Elements::CreateList(FGUI::Vector2i(10, 10), FGUI::Vector2u(308, 197));		fwin->tieElement(l_temi);

		fwin->hide();
	}
	void checkFguiEvents() {
		www = cnsl = false;
		if (activateQuesterRedactor) { if (!fwin->getShowed()) { fwin->activate(); fwin->setPosition(FGUI::WinCh::Position::WINDOW_CENTER); } activateQuesterRedactor = false; }
		if (fwin->getShowed()) {
			// Удаление вопроса
			if (b_deleteAsk->getPressed()) {
				if (l_asks->getTexts()->size() != 0) {
					if (l_asks->getCursor() >= 0) {
						qal.deleteAsk(l_temi->getText(l_temi->getCursor()), l_asks->getCursor());
						l_asks->deleteLine(l_asks->getCursor());
					}
					else { sc.addDraw(L"QR: Не выделен вопрос."); FQuester::Res::aud_warning.play(); }
				}
				else { sc.addDraw(L"QR: Нету вопросов для удаления."); FQuester::Res::aud_warning.play(); }
			}
			// Копирование вопроса
			if (b_reAsk->getPressed()) {
				if (l_asks->getTexts()->size() != 0) {
					if (l_asks->getCursor() >= 0) {
						auto temp = qal.getTempAsk(l_temi->getText(l_temi->getCursor()), l_asks->getCursor());
					switch (temp.type) { case 'T': i_task = 0; break; case 'I': i_task = 1; break; case 'A': i_task = 2; break; case 'V': i_task = 3; break; }
					switch (i_task) {
					case 0:
						t_taskv->setText(L"Текст");
						t_file->setHide(true); tb_file->setHide(true); b_file->setHide(true); t_resize->setHide(true); t_rule->setHide(true);
						t_with->setHide(true); tb_with->setHide(true); t_und->setHide(true); tb_und->setHide(true);

						tb_ask->setText(temp.ask);
						tb_answer->setText(temp.answer);
						tb_counts->setText(std::to_wstring((long long)temp.points));
						break;
					case 1:
						t_taskv->setText(L"Изображение");
						t_file->setHide(false); tb_file->setHide(false); b_file->setHide(false); t_resize->setHide(true); t_rule->setHide(true);
						t_with->setHide(true); tb_with->setHide(true); t_und->setHide(true); tb_und->setHide(true);

						tb_ask->setText(temp.ask);
						tb_answer->setText(temp.answer);
						tb_counts->setText(std::to_wstring((long long)temp.points));
						tb_file->setText(temp.path);
						break;
					case 2:
						t_taskv->setText(L"Аудио");
						t_file->setHide(false); tb_file->setHide(false); b_file->setHide(false); t_resize->setHide(false); t_rule->setHide(false);
						t_with->setHide(false); tb_with->setHide(false); t_und->setHide(false); tb_und->setHide(false);

						tb_ask->setText(temp.ask);
						tb_answer->setText(temp.answer);
						tb_counts->setText(std::to_wstring((long long)temp.points));
						tb_file->setText(temp.path);
						tb_with->setText(temp.startTime);
						tb_und->setText(temp.durTime);
						break;
					case 3:
						t_taskv->setText(L"Видео");
						t_file->setHide(false); tb_file->setHide(false); b_file->setHide(false); t_resize->setHide(false); t_rule->setHide(false);
						t_with->setHide(false); tb_with->setHide(false); t_und->setHide(false); tb_und->setHide(false);

						tb_ask->setText(temp.ask);
						tb_answer->setText(temp.answer);
						tb_counts->setText(std::to_wstring((long long)temp.points));
						tb_file->setText(temp.path);
						tb_with->setText(temp.startTime);
						tb_und->setText(temp.durTime);
						break;
					}
					}
					else { sc.addDraw(L"QR: Не выделен вопрос."); FQuester::Res::aud_warning.play(); }
				}
				else { sc.addDraw(L"QR: Нету вопросов для копирования."); FQuester::Res::aud_warning.play(); }
			}
			// Если сменили курсор в темах то меняем вопросы слева
			if (l_temi->getCursorD() != lastCursor) {
				l_asks->clear();
				for (uint16_t i = 0; i < qal.getAsksLenght(l_temi->getText(l_temi->getCursor())); i++) {
					l_asks->addText(qal.getShortAsk(l_temi->getText(l_temi->getCursor()), i));
				}
				l_asks->setCursorD(-1);
				lastCursor = l_temi->getCursorD();
			}
			// Очки
			if (b_sizecounts[0]->getPressed() || b_sizecounts[1]->getPressed()) {
				if (b_sizecounts[0]->getPressed()) i_counts -= 50; else i_counts += 50;
				if (i_counts < 0) i_counts = 0; else if (i_counts > 10000) i_counts = 10000;
				tb_counts->setText(std::to_wstring((long long)i_counts));
			}
			if (tb_counts->getPressedEnter()) {
				if (tb_counts->getText().size() > 0) {
					auto bufws = tb_counts->getText();
					std::string buf(bufws.begin(), bufws.end());
					i_counts = std::atoi(buf.c_str());
					if (i_counts < 0) i_counts = 0; else if (i_counts > 10000) i_counts = 10000;
					tb_counts->setText(std::to_wstring((long long)i_counts));
				}
				else { sc.addDraw(L"QR: Не заполнено поле очков."); FQuester::Res::aud_warning.play(); }
			}
			// Тип вопроса
			if (b_sizetask[0]->getPressed() || b_sizetask[1]->getPressed() || firstRender) {
				firstRender = false;
				if (b_sizetask[0]->getPressed()) i_task--;
				if (b_sizetask[1]->getPressed()) i_task++;
				if (i_task < 0) i_task = 3; else if (i_task > 3) i_task = 0;
				switch (i_task) {
				case 0:
					t_taskv->setText(L"Текст");
					t_file->setHide(true); tb_file->setHide(true); b_file->setHide(true); t_resize->setHide(true); t_rule->setHide(true);
					t_with->setHide(true); tb_with->setHide(true); t_und->setHide(true); tb_und->setHide(true);
					break;
				case 1:
					t_taskv->setText(L"Изображение");
					t_file->setHide(false); tb_file->setHide(false); b_file->setHide(false); t_resize->setHide(true); t_rule->setHide(true);
					t_with->setHide(true); tb_with->setHide(true); t_und->setHide(true); tb_und->setHide(true);
					break;
				case 2:
					t_taskv->setText(L"Аудио");
					t_file->setHide(false); tb_file->setHide(false); b_file->setHide(false); t_resize->setHide(false); t_rule->setHide(false);
					t_with->setHide(false); tb_with->setHide(false); t_und->setHide(false); tb_und->setHide(false);
					break;
				case 3:
					t_taskv->setText(L"Видео");
					t_file->setHide(false); tb_file->setHide(false); b_file->setHide(false); t_resize->setHide(false); t_rule->setHide(false);
					t_with->setHide(false); tb_with->setHide(false); t_und->setHide(false); tb_und->setHide(false);
					break;
				}
			}
			// Кнопка выхода
			if (b_exit->getPressed()) {
				fwin->hide();
				mInitMenu = true;
				b_exit->reset();
			}
			// Добавление темы
			if (b_addTema->getPressed() || tb_tema->getPressedEnter()) {
				if (!tb_tema->getText().empty()) {
					bool okay = true;
					for (int i = 0; i < l_temi->getTexts()->size(); i++) {
						if (l_temi->getText(i) == tb_tema->getText()) okay = false;
					}
					if (okay) {
						l_temi->addText(tb_tema->getText());
						qal.addTopic(tb_tema->getText());
						tb_tema->clear();
					}
					else { sc.addDraw(L"QR: Такая тема уже есть в списке."); FQuester::Res::aud_warning.play(); }
				}
				else { sc.addDraw(L"QR: Поле тема не заполнено."); FQuester::Res::aud_warning.play(); }
			}
			// Удаление темы
			if (b_deleteTema->getPressed()) {
				if (l_temi->getTexts()->size() > 0) {
					if (l_temi->getCursorD() >= 0) {
						qal.deleteTopic(l_temi->getText(l_temi->getCursorD()));
						l_temi->deleteLine(l_temi->getCursorD());
						l_asks->clear();
					}
					else { sc.addDraw(L"QR: Не выделена тема."); FQuester::Res::aud_warning.play(); }
				}
				else { sc.addDraw(L"QR: Нету тем для удаления."); FQuester::Res::aud_warning.play(); }
			}
			// Переименование темы
			if (b_renameTema->getPressed()) {
				if (l_temi->getCursor() >= 0) {
					if (!tb_tema->getText().empty()) {
						l_temi->changeText(l_temi->getCursor(), tb_tema->getText());
						qal.topics[l_temi->getCursor()].name = tb_tema->getText();
					}
					else { sc.addDraw(L"QR: Поле тема не заполнено.");  FQuester::Res::aud_warning.play(); }
				}
				else { sc.addDraw(L"QR: Не выделена тема."); FQuester::Res::aud_warning.play(); }
			}
			// Добавить вопрос
			if (b_addAsk->getPressed()) {
				bool wasAction = false;

				switch (i_task) {
				case 0: // Text
					if (!tb_answer->getText().empty() && !tb_ask->getText().empty() && !tb_counts->getText().empty()) {
						if (l_temi->getCursorD() >= 0) {
							auto buf = tb_counts->getText();
							std::string bufs(buf.begin(), buf.end());
							uint16_t counts = std::atoi(bufs.c_str());
							qal.addAskText(l_temi->getText(l_temi->getCursor()), tb_ask->getText(), tb_answer->getText(), counts);
							tb_ask->clear(); tb_answer->clear();
							wasAction = true;
						}
						else { sc.addDraw(L"QR: Не выделена тема."); FQuester::Res::aud_warning.play(); }
					}
					else { sc.addDraw(L"QR: Не все поля заполнены."); FQuester::Res::aud_warning.play(); }
					break;
				case 1: // Image
					if (!tb_answer->getText().empty() && !tb_ask->getText().empty() && !tb_counts->getText().empty()) {
						if (l_temi->getCursorD() >= 0) {
							if (!tb_file->getText().empty()) {
								auto buf = tb_counts->getText();
								std::string bufs(buf.begin(), buf.end());
								uint16_t counts = std::atoi(bufs.c_str());
								if (!qal.addAskImage(l_temi->getText(l_temi->getCursor()), tb_ask->getText(), tb_answer->getText(), counts, tb_file->getText())) { sc.addDraw(L"QAL: " + qal.getError()); }
								else {
									tb_ask->clear(); tb_answer->clear(); tb_file->clear();
									wasAction = true;
								}
							}
							else { sc.addDraw(L"QR: Не выбран фаил."); FQuester::Res::aud_warning.play(); }
						}
						else { sc.addDraw(L"QR: Не выделена тема."); FQuester::Res::aud_warning.play(); }
					}
					else { sc.addDraw(L"QR: Не все поля заполнены."); FQuester::Res::aud_warning.play(); }
					break;
				case 2: // Audio
					if (!tb_answer->getText().empty() && !tb_ask->getText().empty() && !tb_counts->getText().empty()) {
						if (l_temi->getCursorD() >= 0) {
							if (!tb_file->getText().empty()) {
								auto buf = tb_counts->getText();
								std::string bufs(buf.begin(), buf.end());
								uint16_t counts = std::atoi(bufs.c_str());
								qal.addAskAudio(l_temi->getText(l_temi->getCursor()), tb_ask->getText(), tb_answer->getText(), counts, tb_file->getText(), tb_with->getText(), tb_und->getText());
								tb_ask->clear(); tb_answer->clear(); tb_file->clear();
								wasAction = true;
							}
							else { sc.addDraw(L"QR: Не выбран фаил."); FQuester::Res::aud_warning.play(); }
						}
						else { sc.addDraw(L"QR: Не выделена тема."); FQuester::Res::aud_warning.play(); }
					}
					else { sc.addDraw(L"QR: Не все поля заполнены."); FQuester::Res::aud_warning.play(); }
					break;
				case 3: // Video
					if (!tb_answer->getText().empty() && !tb_ask->getText().empty() && !tb_counts->getText().empty()) {
						if (l_temi->getCursorD() >= 0) {
							if (!tb_file->getText().empty()) {
								auto buf = tb_counts->getText();
								std::string bufs(buf.begin(), buf.end());
								uint16_t counts = std::atoi(bufs.c_str());
								qal.addAskVideo(l_temi->getText(l_temi->getCursor()), tb_ask->getText(), tb_answer->getText(), counts, tb_file->getText(), tb_with->getText(), tb_und->getText());
								tb_ask->clear(); tb_answer->clear(); tb_file->clear();
								wasAction = true;
							}
							else { sc.addDraw(L"QR: Не выбран фаил."); FQuester::Res::aud_warning.play(); }
						}
						else { sc.addDraw(L"QR: Не выделена тема."); FQuester::Res::aud_warning.play(); }
					}
					else { sc.addDraw(L"QR: Не все поля заполнены."); FQuester::Res::aud_warning.play(); }
					break;
				}
				if (wasAction) lastCursor++;
			}
			// Собрать проект
			if (b_comp->getPressed()) {
				if (!qal.canBuild()) { sc.addDraw(L"QAL: " + qal.getError());FQuester::Res::aud_warning.play(); }
				else {
					if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN == SDL_WINDOW_FULLSCREEN) { SDL_MinimizeWindow(window); }
					qm_compress = cb_compres->getChecked();
					auto buf = FQuester::OpenFile('S');
					if (!buf.empty() && buf.length() != 255) {
						RenderLoadBG(L"Сохранение пака. Ждите..."); 
						SDL_Delay(100);
						try { qal.build(buf); }
						catch (exception e) {
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Ошибка сборки", e.what(), window);
						};
						FQuester::Res::aud_shutdown.play(); 
						if (!CheckFullscreen()) SDL_ShowSimpleMessageBox(NULL, "Builder", "Completed.", FGUI::p_window); 
					}
					else { FQuester::Res::aud_cant.play(); if (!CheckFullscreen()) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Builder", "Path not selected.", FGUI::p_window); }
				}
			}
			// Добавить фаил
			if (b_file->getPressed()) {
				if (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN == SDL_WINDOW_FULLSCREEN) { SDL_MinimizeWindow(window); }

				char mode;
				switch (i_task) {
				case 1:
					mode = 'I'; break;
				case 2:
					mode = 'A'; break;
				case 3:
					mode = 'V'; break;
				}
				setFalseFullscreenCheckBox = true;
				if (CreateWindowAndRenderer(settings::getWindowSize(), false)) FGUI::NewSizeWindow();

				auto buf = FQuester::OpenFile(mode);
				if (!buf.empty()) tb_file->setText(buf);

				FGUI::NewSizeWindow();
			}
		
			FGUI::Rect a = FGUI::Rect(fwin->getPosition().x + 10 + 280 + FGUI::Vars::Window::borderSize, fwin->getPosition().y + 335 + FGUI::Vars::Window::hatSize, 820 - 670, 20);
			if (FGUI::CheckRect(a, FGUI::cursor.getPosition())) {
				FGUI::cursor.setCondition(FGUI::CURSOR_CLICK);
				www = true;
			}

			a = FGUI::Rect(fwin->getPosition().x + 10 + 737 + FGUI::Vars::Window::borderSize, fwin->getPosition().y + 335 + FGUI::Vars::Window::hatSize, 62, 20);
			if (FGUI::CheckRect(a, FGUI::cursor.getPosition())) {
				FGUI::cursor.setCondition(FGUI::CURSOR_CLICK);
				cnsl = true;
			}
		}
		
		

	}
	void checkEvents() {
		if (fwin->getShowed()) {
			if(www){
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
					if (CreateWindowAndRenderer(settings::getWindowSize(), false)) FGUI::NewSizeWindow();
					setFalseFullscreenCheckBox = true;
					SDL_MinimizeWindow(window);
					system("start https://fquester.000webhostapp.com/index.html");
				}
			}
			if (cnsl) {
				if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
					activateConsole = true;
				}
			}
		}
	}
	void update() {
		if (fwin->getShowed()) {
			if (newValuesQuesterRedactor) {
				newValuesQuesterRedactor = false;
				loadNewValues();
			}
			if (fwin->getWasResized()) {
			}
		}
	}

	void loadNewValues() {
		clear();
		for (int i = 0; i < qal.topics.size(); i++) {
			l_temi->addText(qal.topics[i].name);
		}
	}
	void clear() {
		l_temi->clear();
		l_asks->clear();

		tb_answer->clear();
		tb_ask->clear();
		tb_counts->clear();
		tb_file->clear();
		tb_tema->clear();
		tb_und->clear();
		tb_with->clear();
	}
	void fullClear() {
		qal.topics.clear();
		qal.name = L"";
		qal.autor = L"";

		l_temi->clear();
		l_asks->clear();

		tb_answer->clear();
		tb_ask->clear();
		tb_counts->clear();
		tb_file->clear();
		tb_tema->clear();
		tb_und->clear();
		tb_with->clear();
	}
} fw_questerRedactor;