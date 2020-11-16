#include "fgui.h"
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"
#include "objects.h"
#include "Com.h"
#include <SDL_thread.h>
#include "fgui_RWGlobalVars.h"

#include "res.h"

char outset[MAX_DATA_LENGTH];

FGUI::Elements::ConsoleList* clb_infoPort;
std::wstring fguiRW_settings_msgb = L"-";
int fguiRW_settings_fndPort = -1; bool fguiRW_settings_mufnd = false;

bool checkPrCreated = false;

int thread_com(void* unused);

int lastNumButton = 0;
bool newButton = false;

struct Settings {
	FGUI::Window* fwin;

	// Main
	FGUI::Elements::Button* bm_apply;
	FGUI::Elements::SettingsList* lsm_list; int lastCursorList;

	// Video
	FGUI::Elements::Panel* vp_p;
	FGUI::Elements::Text* tv_resol; FGUI::Elements::Button* bv_resol[2]; FGUI::Elements::Text* tv_rresol; int i_resol; bool mu_resol;
	FGUI::Elements::Text* tv_render; FGUI::Elements::Button* bv_render[2]; FGUI::Elements::Text* tv_rrender; int i_render; bool mu_render;
	FGUI::Elements::Text* tv_filter; FGUI::Elements::Button* bv_filter[2]; FGUI::Elements::Text* tv_rfilter; int i_filter; bool mu_filter;
	FGUI::Elements::CheckBox* cbv_fullscreen; 
	FGUI::Elements::CheckBox* cbv_vsync;
	FGUI::Elements::CheckBox* cbv_noise;
	FGUI::Elements::Text* tv_bright; FGUI::Elements::Button* bv_bright[2]; FGUI::Elements::Text* tv_rbright; int i_bright; bool mu_bright;
	FGUI::Elements::Text* tv_msg;
	FGUI::Elements::Text* tv_frameLimit; FGUI::Elements::TextBox* tbv_frameLimit;

	// Audio 
	FGUI::Elements::Panel* ap_p;
	FGUI::Elements::Text* tv_volume; FGUI::Elements::Button* bv_volume[2]; FGUI::Elements::Text* tv_rvolume; int i_volume; bool mu_volume;
	FGUI::Elements::CheckBox* cba_bmusic;
	FGUI::Elements::CheckBox* cbc_bticks;

	// Buttons
	FGUI::Elements::Panel* bp_p;
	FGUI::Elements::Button* bb_search; FGUI::Elements::Text* tb_search;
	
	// Cat
	FGUI::Elements::Panel* cp_p;
	FGUI::Elements::Text* tc_catper; FGUI::Elements::Button* bc_catper[2]; FGUI::Elements::Text* tc_rcatper; int i_catper; bool mu_catper;
	FGUI::Elements::CheckBox* cbc_exchange;
	FGUI::Elements::CheckBox* cbc_transmission;
	FGUI::Elements::CheckBox* cbc_names; FGUI::Elements::Text* tc_cbc_names; FGUI::Elements::Button* bc_names;
	FGUI::Elements::CheckBox* cbc_auction;
	FGUI::Elements::CheckBox* cbc_deleteafter;
	FGUI::Elements::Text* tc_timer; FGUI::Elements::TextBox* tbc_timer;

	// Other
	FGUI::Elements::Panel* op_p;
	FGUI::Elements::Text* to_contentTypeMode; FGUI::Elements::Button* bo_contentTypeMode[2]; FGUI::Elements::Text* to_rcontentTypeMode; int i_contentTypeMode; bool mu_contentTypeMode;
	FGUI::Elements::Text* to_contentTypeModeV; FGUI::Elements::Button* bo_contentTypeModeV[2]; FGUI::Elements::Text* to_rcontentTypeModeV; int i_contentTypeModeV; bool mu_contentTypeModeV;
	FGUI::Elements::CheckBox* cbo_scalingContent;

	SDL_Thread* thread;

	void init() {
		fwin = FGUI::FCreateWindow();
		fwin->init();
		fwin->setCaption(L"Настройки");
		fwin->setBlockSize(true);
		fwin->setIcon(FGUI::WinCh::DefIcons::WINDOW_DEF_ICON_SETTINGS);
		fwin->setSize(FGUI::Vector2u(500, 320));

		// Main
		bm_apply = FGUI::Elements::CreateButton(FGUI::Vector2i(398, 290), L"Сохранить"); fwin->tieElement(bm_apply);
		lsm_list = FGUI::Elements::CreateSettingsList(FGUI::Vector2i(5, 5), FGUI::Vector2u(160, 310)); fwin->tieElement(lsm_list);
		lsm_list->addText(L"Видео"); lsm_list->addText(L"Аудио"); lsm_list->addText(L"Кнопки"); lsm_list->addText(L"Правила"); lsm_list->addText(L"Вид");
		lastCursorList = -1; lsm_list->setCursor(0);

		// Other
		op_p = FGUI::Elements::CreatePanel(FGUI::Vector2i(170, 5), FGUI::Vector2u(325, 280)); fwin->tieElement(op_p);

		to_contentTypeMode = FGUI::Elements::CreateText(FGUI::Vector2i(180, 15), L"Вопрос: "); to_contentTypeMode->setClearColor(op_p->getColor()); fwin->tieElement(to_contentTypeMode);
		bo_contentTypeMode[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(280, 12), L"<"); bo_contentTypeMode[0]->setClearColor(op_p->getColor()); fwin->tieElement(bo_contentTypeMode[0]);
		bo_contentTypeMode[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 12), L">"); bo_contentTypeMode[1]->setClearColor(op_p->getColor()); fwin->tieElement(bo_contentTypeMode[1]);
		to_rcontentTypeMode = FGUI::Elements::CreateText(FGUI::Vector2i(314, 15), L"error"); to_rcontentTypeMode->setClearColor(op_p->getColor()); fwin->tieElement(to_rcontentTypeMode);
		i_contentTypeMode = settings::s_contentTypeMode; mu_contentTypeMode = true;

		to_contentTypeModeV = FGUI::Elements::CreateText(FGUI::Vector2i(180, 75), L"VID вопрос: "); to_contentTypeModeV->setClearColor(op_p->getColor()); fwin->tieElement(to_contentTypeModeV);
		bo_contentTypeModeV[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(280, 72), L"<"); bo_contentTypeModeV[0]->setClearColor(op_p->getColor()); fwin->tieElement(bo_contentTypeModeV[0]);
		bo_contentTypeModeV[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 72), L">"); bo_contentTypeModeV[1]->setClearColor(op_p->getColor()); fwin->tieElement(bo_contentTypeModeV[1]);
		to_rcontentTypeModeV = FGUI::Elements::CreateText(FGUI::Vector2i(314, 75), L"error"); to_rcontentTypeModeV->setClearColor(op_p->getColor()); fwin->tieElement(to_rcontentTypeModeV);
		i_contentTypeModeV = settings::s_contentTypeModeV; mu_contentTypeModeV = true;

		cbo_scalingContent = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 45), L"Масштабирование контента"); cbo_scalingContent->setClearColor(op_p->getColor()); fwin->tieElement(cbo_scalingContent); cbo_scalingContent->setChecked(settings::s_scalingContent);

		// Cat
		cp_p = FGUI::Elements::CreatePanel(FGUI::Vector2i(170, 5), FGUI::Vector2u(325, 280)); fwin->tieElement(cp_p);
		
		tc_catper = FGUI::Elements::CreateText(FGUI::Vector2i(180, 15), L"Вероятность псевдокота: "); tc_catper->setClearColor(cp_p->getColor()); fwin->tieElement(tc_catper);
		bc_catper[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(370, 12), L"<"); bc_catper[0]->setClearColor(cp_p->getColor()); fwin->tieElement(bc_catper[0]);
		bc_catper[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 12), L">"); bc_catper[1]->setClearColor(cp_p->getColor()); fwin->tieElement(bc_catper[1]);
		tc_rcatper = FGUI::Elements::CreateText(FGUI::Vector2i(410, 15), L"error"); tc_rcatper->setClearColor(cp_p->getColor()); fwin->tieElement(tc_rcatper);
		i_catper = settings::s_catper; mu_catper = true;

		cbc_exchange = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 65), L"Размен очков"); cbc_exchange->setClearColor(cp_p->getColor()); fwin->tieElement(cbc_exchange); cbc_exchange->setChecked(settings::s_exchange);
		cbc_transmission = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 85), L"Контрольная передача вопроса"); cbc_transmission->setClearColor(cp_p->getColor()); fwin->tieElement(cbc_transmission); cbc_transmission->setChecked(settings::s_transmission);
		cbc_names = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 125), L"Множитель и размен очков в"); cbc_names->setClearColor(cp_p->getColor()); fwin->tieElement(cbc_names); cbc_names->setChecked(settings::s_names);
		tc_cbc_names = FGUI::Elements::CreateText(FGUI::Vector2i(210, 140), L"зависимости от совпадений"); tc_cbc_names->setClearColor(cp_p->getColor()); fwin->tieElement(tc_cbc_names);
		bc_names = FGUI::Elements::CreateButton(FGUI::Vector2i(180, 170), L"Список совпадений"); bc_names->setClearColor(cp_p->getColor()); fwin->tieElement(bc_names);
		cbc_auction = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 105), L"Аукцион за вопрос"); cbc_auction->setClearColor(cp_p->getColor()); fwin->tieElement(cbc_auction); cbc_auction->setChecked(settings::s_auction);

		cbc_deleteafter = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 225), L"Удалить вопрос после псевдокота"); cbc_deleteafter->setClearColor(cp_p->getColor()); fwin->tieElement(cbc_deleteafter); cbc_deleteafter->setChecked(settings::s_deleteafter);
		
		tc_timer = FGUI::Elements::CreateText(FGUI::Vector2i(180, 255), L"Таймер:"); tc_timer->setClearColor(cp_p->getColor()); fwin->tieElement(tc_timer);
		tbc_timer = FGUI::Elements::CreateTextBox(FGUI::Vector2i(260, 252), 228); tbc_timer->setMaxChars(2); tbc_timer->setInputOnlyNum(true); tbc_timer->setClearColor(cp_p->getColor()); fwin->tieElement(tbc_timer); tbc_timer->setText(std::to_wstring((long long)settings::s_timer));

		// Buttons
		bp_p = FGUI::Elements::CreatePanel(FGUI::Vector2i(170, 5), FGUI::Vector2u(325, 280)); fwin->tieElement(bp_p);

		tb_search = FGUI::Elements::CreateText(FGUI::Vector2i(180, 15), L"Статус: приемник не найден"); tb_search->setClearColor(bp_p->getColor()); fwin->tieElement(tb_search);
		bb_search = FGUI::Elements::CreateButton(FGUI::Vector2i(180, 255), L"Поиск"); fwin->tieElement(bb_search);

		clb_infoPort = FGUI::Elements::CreateConsoleList(FGUI::Vector2i(175, 40), FGUI::Vector2u(315, 200)); fwin->tieElement(clb_infoPort);

		clb_infoPort->addText(L"      Для того, что бы подключить приёмник");
		clb_infoPort->addText(L"необходимо установить драйвер (в конце");
		clb_infoPort->addText(L"установки игры галочка <Установить драй-");
		clb_infoPort->addText(L"вер для приёмника>).");
		clb_infoPort->addText(L"-------------------------  Инструкция  ----------------------------------");
		clb_infoPort->addText(L"1. Подключите приёмник кнопок.");
		clb_infoPort->addText(L"2. Дождитесь когда загорится красный све-");
		clb_infoPort->addText(L"  тодиод на приемнике.");
		clb_infoPort->addText(L"3. Нажмите кнопку <Поиск>.");
		clb_infoPort->addText(L"4. Здесь будет отображаться процесс");
		clb_infoPort->addText(L"  поиска (возможно устройство подключится");
		clb_infoPort->addText(L"  не с первого раза, следует повторить ");
		clb_infoPort->addText(L"  поиск ~5 раз).");
		clb_infoPort->addText(L"5. Если устройство подключится: статус изме-");
		clb_infoPort->addText(L"  нится и светодиод загорится зелёным.");
		clb_infoPort->addText(L"-------------------------------------------------------------------------");

		bp_p->setHide(true); tb_search->setHide(true); bb_search->setHide(true); clb_infoPort->setHide(true);
		bp_p->noClear(); tb_search->noClear(); bb_search->noClear(); clb_infoPort->noClear();

		// Audio
		ap_p = FGUI::Elements::CreatePanel(FGUI::Vector2i(170, 5), FGUI::Vector2u(325, 280)); fwin->tieElement(ap_p);

		bv_volume[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 12), L">"); bv_volume[1]->setClearColor(ap_p->getColor()); fwin->tieElement(bv_volume[1]);
		tv_volume = FGUI::Elements::CreateText(FGUI::Vector2i(180, 15), L"Общая громкость: "); tv_volume->setClearColor(ap_p->getColor()); fwin->tieElement(tv_volume);
		bv_volume[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(320, 12), L"<"); bv_volume[0]->setClearColor(ap_p->getColor()); fwin->tieElement(bv_volume[0]);
		tv_rvolume = FGUI::Elements::CreateText(FGUI::Vector2i(360, 15), L"error"); tv_rvolume->setClearColor(ap_p->getColor()); fwin->tieElement(tv_rvolume);
		i_volume = settings::s_volume; mu_volume = true;

		cba_bmusic = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 45), L"Атмосферная музыка"); cba_bmusic->setClearColor(ap_p->getColor()); fwin->tieElement(cba_bmusic); cba_bmusic->setChecked(settings::s_bmusic);
		cbc_bticks = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 65), L"Тиканье"); cbc_bticks->setClearColor(ap_p->getColor()); fwin->tieElement(cbc_bticks); cbc_bticks->setChecked(settings::s_bticks);

		ap_p->setHide(true); tv_volume->setHide(true); bv_volume[0]->setHide(true); bv_volume[1]->setHide(true); tv_rvolume->setHide(true); cba_bmusic->setHide(true); cbc_bticks->setHide(true);
		ap_p->noClear(); tv_volume->noClear(); bv_volume[0]->noClear(); bv_volume[1]->noClear(); tv_rvolume->noClear(); cba_bmusic->noClear(); cbc_bticks->noClear();

		// Video
		vp_p = FGUI::Elements::CreatePanel(FGUI::Vector2i(170, 5), FGUI::Vector2u(325, 280)); fwin->tieElement(vp_p);

		tv_resol = FGUI::Elements::CreateText(FGUI::Vector2i(180, 15), L"Разрешение: "); tv_resol->setClearColor(vp_p->getColor()); fwin->tieElement(tv_resol);
		bv_resol[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(320, 12), L"<"); bv_resol[0]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_resol[0]);
		bv_resol[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 12), L">"); bv_resol[1]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_resol[1]);
		tv_rresol = FGUI::Elements::CreateText(FGUI::Vector2i(360, 15), L"error"); tv_rresol->setClearColor(vp_p->getColor()); fwin->tieElement(tv_rresol);
		i_resol = settings::s_resol; mu_resol = true;

		tv_render = FGUI::Elements::CreateText(FGUI::Vector2i(180, 45), L"Рендер: "); tv_render->setClearColor(vp_p->getColor()); fwin->tieElement(tv_render);
		bv_render[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(320, 42), L"<"); bv_render[0]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_render[0]);
		bv_render[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 42), L">"); bv_render[1]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_render[1]);
		tv_rrender = FGUI::Elements::CreateText(FGUI::Vector2i(360, 45), L"error"); tv_rrender->setClearColor(vp_p->getColor()); fwin->tieElement(tv_rrender);
		i_render = settings::s_renderer; mu_render = true;

		cbv_fullscreen = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 85), L"На весь экран"); cbv_fullscreen->setClearColor(vp_p->getColor()); fwin->tieElement(cbv_fullscreen); cbv_fullscreen->setChecked(settings::s_fullscreen);
		cbv_vsync = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 105), L"Вертикальная синхронизация (GPU)"); cbv_vsync->setClearColor(vp_p->getColor()); fwin->tieElement(cbv_vsync); cbv_vsync->setChecked(settings::s_vsync);
		cbv_noise = FGUI::Elements::CreateCheckBox(FGUI::Vector2i(180, 125), L"TV шум"); cbv_noise->setClearColor(vp_p->getColor()); fwin->tieElement(cbv_noise); cbv_noise->setChecked(settings::s_noise);

		tv_bright = FGUI::Elements::CreateText(FGUI::Vector2i(180, 158), L"Яркость: "); tv_bright->setClearColor(vp_p->getColor()); fwin->tieElement(tv_bright);
		bv_bright[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(320, 155), L"<"); bv_bright[0]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_bright[0]);
		bv_bright[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 155), L">"); bv_bright[1]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_bright[1]);
		tv_rbright = FGUI::Elements::CreateText(FGUI::Vector2i(360, 158), L"error"); tv_rbright->setClearColor(vp_p->getColor()); fwin->tieElement(tv_rbright);
		i_bright = settings::s_bright; mu_bright = true;

		tv_filter = FGUI::Elements::CreateText(FGUI::Vector2i(180, 186), L"Фильтрация: "); tv_filter->setClearColor(vp_p->getColor()); fwin->tieElement(tv_filter);
		bv_filter[0] = FGUI::Elements::CreateButton(FGUI::Vector2i(320, 183), L"<"); bv_filter[0]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_filter[0]);
		bv_filter[1] = FGUI::Elements::CreateButton(FGUI::Vector2i(460, 183), L">"); bv_filter[1]->setClearColor(vp_p->getColor()); fwin->tieElement(bv_filter[1]);
		tv_rfilter = FGUI::Elements::CreateText(FGUI::Vector2i(360, 186), L"error"); tv_rfilter->setClearColor(vp_p->getColor()); fwin->tieElement(tv_rfilter);
		i_filter = settings::s_filter; mu_filter = true;

		tv_frameLimit = FGUI::Elements::CreateText(FGUI::Vector2i(180, 224), L"Лимит кадров:"); tv_frameLimit->setClearColor(vp_p->getColor()); fwin->tieElement(tv_frameLimit);
		tbv_frameLimit = FGUI::Elements::CreateTextBox(FGUI::Vector2i(320, 221), 169); tbv_frameLimit->setClearColor(vp_p->getColor()); tbv_frameLimit->setMaxChars(5); tbv_frameLimit->setInputOnlyNum(true); tbv_frameLimit->setText(std::to_wstring((long long)settings::s_frameLimit)); fwin->tieElement(tbv_frameLimit);

		tv_msg = FGUI::Elements::CreateText(FGUI::Vector2i(180, 255), L"Рендер настройки требуют перезапуск!"); tv_msg->setColor(FGUI::Color(140, 140, 140)); tv_msg->setClearColor(vp_p->getColor()); fwin->tieElement(tv_msg);


		fwin->hide();
	}
	void checkFguiEvents() {
		if (activateSettings) { if (!fwin->getShowed()) { fwin->activate(); fwin->setPosition(FGUI::WinCh::Position::WINDOW_NONE); } activateSettings = false; }
		if (changeFullscreenCheckBox) {
			cbv_fullscreen->setChecked(cbv_fullscreen->getChecked()); changeFullscreenCheckBox = false;
		}
		if (setFalseFullscreenCheckBox) {
			cbv_fullscreen->setChecked(false); setFalseFullscreenCheckBox = false;
		}
		if (fwin->getShowed()) {
			// Main
			if (lsm_list->getCursor() != lastCursorList) {
				lastCursorList = lsm_list->getCursor();

				vp_p->setHide(true); tv_resol->setHide(true); bv_resol[0]->setHide(true); bv_resol[1]->setHide(true); tv_rresol->setHide(true); tv_render->setHide(true); bv_render[0]->setHide(true); bv_render[1]->setHide(true); tv_rrender->setHide(true); cbv_fullscreen->setHide(true); cbv_vsync->setHide(true); tv_bright->setHide(true); bv_bright[0]->setHide(true); bv_bright[1]->setHide(true); tv_rbright->setHide(true); tv_msg->setHide(true); tv_filter->setHide(true); bv_filter[0]->setHide(true); bv_filter[1]->setHide(true); tv_rfilter->setHide(true); cbv_noise->setHide(true); tv_frameLimit->setHide(true); tbv_frameLimit->setHide(true);
				vp_p->noClear(); tv_resol->noClear(); bv_resol[0]->noClear(); bv_resol[1]->noClear(); tv_rresol->noClear(); tv_render->noClear(); bv_render[0]->noClear(); bv_render[1]->noClear(); tv_rrender->noClear(); cbv_fullscreen->noClear(); cbv_vsync->noClear();  tv_bright->noClear(); bv_bright[0]->noClear(); bv_bright[1]->noClear(); tv_rbright->noClear(); tv_msg->noClear(); tv_filter->noClear(); bv_filter[0]->noClear(); bv_filter[1]->noClear(); tv_rfilter->noClear(); cbv_noise->noClear(); tv_frameLimit->noClear(); tbv_frameLimit->noClear();

				ap_p->setHide(true); tv_volume->setHide(true); bv_volume[0]->setHide(true); bv_volume[1]->setHide(true); tv_rvolume->setHide(true); cba_bmusic->setHide(true); cbc_bticks->setHide(true);
				ap_p->noClear(); tv_volume->noClear(); bv_volume[0]->noClear(); bv_volume[1]->noClear(); tv_rvolume->noClear(); cba_bmusic->noClear(); cbc_bticks->noClear();
				
				bp_p->setHide(true); tb_search->setHide(true); bb_search->setHide(true); clb_infoPort->setHide(true);
				bp_p->noClear(); tb_search->noClear(); bb_search->noClear(); clb_infoPort->noClear();

				cp_p->setHide(true); tc_catper->setHide(true); bc_catper[0]->setHide(true); bc_catper[1]->setHide(true); tc_rcatper->setHide(true); cbc_exchange->setHide(true); cbc_transmission->setHide(true); cbc_names->setHide(true); tc_cbc_names->setHide(true); bc_names->setHide(true); cbc_auction->setHide(true); cbc_deleteafter->setHide(true); tc_timer->setHide(true); tbc_timer->setHide(true);
				cp_p->noClear(); tc_catper->noClear(); bc_catper[0]->noClear(); bc_catper[1]->noClear(); tc_rcatper->noClear(); cbc_exchange->noClear(); cbc_transmission->noClear(); bc_names->noClear(); cbc_names->noClear(); tc_cbc_names->noClear(); cbc_auction->noClear(); cbc_deleteafter->noClear(); tc_timer->noClear(); tbc_timer->noClear();

				op_p->setHide(true); to_contentTypeMode->setHide(true); bo_contentTypeMode[0]->setHide(true); bo_contentTypeMode[1]->setHide(true); to_rcontentTypeMode->setHide(true); to_contentTypeModeV->setHide(true); bo_contentTypeModeV[0]->setHide(true); bo_contentTypeModeV[1]->setHide(true); to_rcontentTypeModeV->setHide(true); cbo_scalingContent->setHide(true);
				op_p->noClear(); to_contentTypeMode->noClear(); bo_contentTypeMode[0]->noClear(); bo_contentTypeMode[1]->noClear(); to_rcontentTypeMode->noClear(); to_contentTypeModeV->noClear(); bo_contentTypeModeV[0]->noClear(); bo_contentTypeModeV[1]->noClear(); to_rcontentTypeModeV->noClear(); cbo_scalingContent->noClear();

				switch (lastCursorList) {
				case 0:
					// Video
					vp_p->setHide(false); tv_resol->setHide(false); bv_resol[0]->setHide(false); bv_resol[1]->setHide(false); tv_rresol->setHide(false); tv_render->setHide(false); bv_render[0]->setHide(false); bv_render[1]->setHide(false); tv_rrender->setHide(false); cbv_fullscreen->setHide(false); cbv_vsync->setHide(false); tv_bright->setHide(false); bv_bright[0]->setHide(false); bv_bright[1]->setHide(false); tv_rbright->setHide(false); tv_msg->setHide(false); tv_filter->setHide(false); bv_filter[0]->setHide(false); bv_filter[1]->setHide(false); tv_rfilter->setHide(false); cbv_noise->setHide(false); tv_frameLimit->setHide(false); tbv_frameLimit->setHide(false);
					break;
				case 1:
					// Audio
					ap_p->setHide(false); tv_volume->setHide(false); bv_volume[0]->setHide(false); bv_volume[1]->setHide(false); tv_rvolume->setHide(false); cba_bmusic->setHide(false); cbc_bticks->setHide(false);
					break;
				case 2:
					// Buttons
					bp_p->setHide(false); tb_search->setHide(false); bb_search->setHide(false); clb_infoPort->setHide(false);
					break;
				case 3:
					// Cat
					cp_p->setHide(false); tc_catper->setHide(false); bc_catper[0]->setHide(false); bc_catper[1]->setHide(false); tc_rcatper->setHide(false); cbc_exchange->setHide(false); cbc_transmission->setHide(false); cbc_names->setHide(false); tc_cbc_names->setHide(false); bc_names->setHide(false); cbc_auction->setHide(false); cbc_deleteafter->setHide(false); tc_timer->setHide(false); tbc_timer->setHide(false);
					break;
				case 4:
					// Other
					op_p->setHide(false); to_contentTypeMode->setHide(false); bo_contentTypeMode[0]->setHide(false); bo_contentTypeMode[1]->setHide(false); to_rcontentTypeMode->setHide(false); to_contentTypeModeV->setHide(false); bo_contentTypeModeV[0]->setHide(false); bo_contentTypeModeV[1]->setHide(false); to_rcontentTypeModeV->setHide(false); cbo_scalingContent->setHide(false);
					break;
				}
			}

			if (bm_apply->getPressed()) {
				settings::s_resol = i_resol; 
				settings::s_renderer = i_render; 
				settings::s_filter = i_filter;
				settings::s_frameLimit = _wtoi(tbv_frameLimit->getText().c_str());
				settings::s_fullscreen = cbv_fullscreen->getChecked(); 
				settings::s_vsync = cbv_vsync->getChecked(); 
				settings::s_noise = cbv_noise->getChecked();
				settings::s_bright = i_bright;
				settings::s_volume = i_volume;
				settings::s_bmusic = cba_bmusic->getChecked();
				settings::s_bticks = cbc_bticks->getChecked();
				settings::s_contentTypeMode = i_contentTypeMode;
				settings::s_contentTypeModeV = i_contentTypeModeV;
				settings::s_scalingContent = cbo_scalingContent->getChecked();
				settings::s_catper = i_catper;
				settings::s_exchange = cbc_exchange->getChecked();
				settings::s_transmission = cbc_transmission->getChecked();
				settings::s_names = cbc_names->getChecked();
				settings::s_auction = cbc_auction->getChecked();
				settings::s_deleteafter = cbc_deleteafter->getChecked();
				settings::s_timer = std::stoi(tbc_timer->getText());

				settings::save();
				settings::read();

				if (CreateWindowAndRenderer(settings::getWindowSize(), settings::s_fullscreen)) FGUI::NewSizeWindow();

				FQuester::Res::aud_startup.play();
			}

			// Video

			if (bv_resol[0]->getPressed()) { i_resol--; mu_resol = true; if (!devMode) { if (i_resol < 0) i_resol = settings::resolMax - 1; } else { if (i_resol < 0) i_resol = 22; } }
			if (bv_resol[1]->getPressed()) { i_resol++; mu_resol = true; if (!devMode) { if (i_resol == settings::resolMax) i_resol = 0; } else { if (i_resol == 23) i_resol = 0; } }
			if (mu_resol) {
				std::wstring bufws = L""; bufws += std::to_wstring((long long)settings::getWindowSize(i_resol).x); bufws += L"x"; bufws += std::to_wstring((long long)settings::getWindowSize(i_resol).y);
				tv_rresol->setText(bufws);
				mu_resol = false;
			}

			if (bv_render[0]->getPressed()) { i_render--; mu_render = true; if (i_render < 0) i_render = N_RENDERER - 1; }
			if (bv_render[1]->getPressed()) { i_render++; mu_render = true; if (i_render == N_RENDERER) i_render = 0; }
			if (mu_render) {
				if (i_render == 0) tv_rrender->setText(L"Software"); 
				else if (i_render == 1) tv_rrender->setText(L"Direct3D 9");
				else if (i_render == 2) tv_rrender->setText(L"OpenGL");
				mu_render = false;
			}

			if (bv_bright[0]->getEventPressing()) { i_bright -= 5; mu_bright = true; if (i_bright < 30) i_bright = 30; }
			if (bv_bright[1]->getEventPressing()) { i_bright += 5; mu_bright = true; if (i_bright > 400) i_bright = 400; }
			if (mu_bright) {
				tv_rbright->setText(std::to_wstring((long long)i_bright) + L"%");
				SDL_SetWindowBrightness(FGUI::p_window, (float)i_bright/100);
				mu_bright = false;
			}

			if (bv_filter[0]->getPressed()) { i_filter--; mu_filter = true; if (i_filter < 0) i_filter = 3 - 1; }
			if (bv_filter[1]->getPressed()) { i_filter++; mu_filter = true; if (i_filter == 3) i_filter = 0; }
			if (mu_filter) {
				if (i_filter == 0) tv_rfilter->setText(L"Nearest"); 
				else if (i_filter == 1) tv_rfilter->setText(L"Linear");
				else if (i_filter == 2) tv_rfilter->setText(L"Anisotropic");
				mu_filter = false;
			}

			// Audio
			if (bv_volume[0]->getEventPressing()) { i_volume -= 5; mu_volume = true; if (i_volume < 0) i_volume = 0; }
			else if (bv_volume[1]->getEventPressing()) { i_volume += 5; mu_volume = true; if (i_volume > 100) i_volume = 100; }
			if (mu_volume) {
				tv_rvolume->setText(std::to_wstring((long long)i_volume) + L"%");
				sf::Listener::setGlobalVolume(i_volume);

				// SFML sounds bag
				// Global volume not work
				// I need change volume for each sound
				FQuester::Res::aud_auction.setVolume(i_volume);
				FQuester::Res::aud_cant.setVolume(i_volume);
				FQuester::Res::aud_end.setVolume(i_volume);
				FQuester::Res::aud_haha.setVolume(i_volume);
				FQuester::Res::aud_press.setVolume(i_volume);
				FQuester::Res::aud_shutdown.setVolume(i_volume);
				FQuester::Res::aud_startup.setVolume(i_volume);
				FQuester::Res::aud_tack.setVolume(i_volume);
				FQuester::Res::aud_tick.setVolume(i_volume);
				FQuester::Res::aud_time.setVolume(i_volume);
				FQuester::Res::aud_warning.setVolume(i_volume);

				mu_volume = false;
			}

			// Buttons

			if (wcscmp(fguiRW_settings_msgb.c_str(), L"-")) {
				clb_infoPort->addText(fguiRW_settings_msgb);
				fguiRW_settings_msgb = L"-";
			}

			if (fguiRW_settings_mufnd) {
				std::wstring bufws = L"Статус: чтение из COM"; bufws += std::to_wstring((long long)fguiRW_settings_fndPort);
				tb_search->setText(bufws);
				fguiRW_settings_mufnd = false;
			}

			if (bb_search->getPressed()) {
				if (!checkPrCreated) {
					thread = SDL_CreateThread(thread_com, "Search com", NULL);
				}
			}

			// Cat

			if (bc_catper[0]->getEventPressing()) { if (i_catper < 25) i_catper -= 1; else i_catper -= 2; mu_catper = true; if (i_catper < 0) i_catper = 0; }
			if (bc_catper[1]->getEventPressing()) { if (i_catper < 25) i_catper += 1; else i_catper += 2; mu_catper = true; if (devMode) { if (i_catper > 100) i_catper = 100; } else { if (i_catper > 75) i_catper = 75; } }
			if (mu_catper) {
				std::wstring bufws = L""; bufws += std::to_wstring((long long)i_catper); bufws += L"%";
				tc_rcatper->setText(bufws);
				mu_catper = false;
			}
			if (bc_names->getEventPressing()) {
				activateNameList = true;
			}

			// Other

			if (bo_contentTypeMode[0]->getEventPressing()) { i_contentTypeMode--; mu_contentTypeMode = true; if (i_contentTypeMode < 0) i_contentTypeMode = 1; }
			if (bo_contentTypeMode[1]->getEventPressing()) { i_contentTypeMode++; mu_contentTypeMode = true; if (i_contentTypeMode > 1) i_contentTypeMode = 0; }
			if (mu_contentTypeMode) {
				std::wstring bufws = L""; 
				
				if (i_contentTypeMode == 0) {
					bufws += L"Вместе с контентом";
				}
				else if (i_contentTypeMode == 1) {
					bufws += L"Многоуровневый";
				}
				
				to_rcontentTypeMode->setText(bufws);
				mu_contentTypeMode = false;
			}

			if (bo_contentTypeModeV[0]->getEventPressing()) { i_contentTypeModeV--; mu_contentTypeModeV = true; if (i_contentTypeModeV < 0) i_contentTypeModeV = 1; }
			if (bo_contentTypeModeV[1]->getEventPressing()) { i_contentTypeModeV++; mu_contentTypeModeV = true; if (i_contentTypeModeV > 1) i_contentTypeModeV = 0; }
			if (mu_contentTypeModeV) {
				std::wstring bufws = L"";

				if (i_contentTypeModeV == 0) {
					bufws += L"Вместе с контентом";
				}
				else if (i_contentTypeModeV == 1) {
					bufws += L"Многоуровневый";
				}

				to_rcontentTypeModeV->setText(bufws);
				mu_contentTypeModeV = false;
			}
		}
	}
	void checkEvents() {
		if (fwin->getShowed()) {
		}
	}
	void update() {
		if (fwin->getShowed()) {
		}
	}
} fw_settings;



int thread_com(void* unused) {
	int finded = -1;
	for (long long i = 0; i < N_PORTS; i++) {
		Com com(i);
		int trues = 0;
		if (com.isConnected()) {
			com.readSerialPort(outset, MAX_DATA_LENGTH);
			if (outset[0] == 101) {
				char* buf = new char[1];
				buf[0] = 'd';
				finded = i;
				com.close();

				std::wstring msg = L"] Порт приемника найден (COM"; msg += std::to_wstring(i); msg += L")";
				fguiRW_settings_msgb = msg;

				fguiRW_settings_fndPort = i;
				fguiRW_settings_mufnd = true;

				delete buf;

				checkPrCreated = true;

				break;
			}
		}

		com.close();
	}
	if (finded == -1) {
		std::wstring msg = L"] Приемник не найден, повторите попытку.";
		fguiRW_settings_msgb = msg;
	}
	else {
		Com com(finded);
		char* z = nullptr;
		std::wstring bufs;
		char* bufnc = new char[1];
		bufnc[0] = KEY_PORT;
		while (com.isConnected()) {
			if (!clb_infoPort->getHide()) {
				com.writeSerialPort(bufnc, 1);
				com.readSerialPort(outset, MAX_DATA_LENGTH);

				if (fw_settings.fwin->getShowed()) {
					switch (outset[0]) {
						case 2:
							if (FQuester::Res::aud_cant.getStatus() != sf::Sound::Status::Playing && FQuester::Res::aud_time.getStatus() != sf::Sound::Status::Playing && FQuester::Res::aud_warning.getStatus() != sf::Sound::Status::Playing) FQuester::Res::aud_cant.play(); break;
						case 3:
							if (FQuester::Res::aud_cant.getStatus() != sf::Sound::Status::Playing && FQuester::Res::aud_time.getStatus() != sf::Sound::Status::Playing && FQuester::Res::aud_warning.getStatus() != sf::Sound::Status::Playing) FQuester::Res::aud_time.play(); break;
						case 4:
							if (FQuester::Res::aud_cant.getStatus() != sf::Sound::Status::Playing && FQuester::Res::aud_time.getStatus() != sf::Sound::Status::Playing && FQuester::Res::aud_warning.getStatus() != sf::Sound::Status::Playing) FQuester::Res::aud_warning.play(); break;
					}
				}
				else {
					newButton = true;
					switch (outset[0]) {
						case 0:
							lastNumButton = 0;
							break;
						case 2:
							lastNumButton = 1;
							newButton = true;
							break;
						case 3:
							lastNumButton = 2;
							newButton = true;
							break;
						case 4:
							lastNumButton = 3;
							newButton = true;
							break;
					}
				}
			}
		}
	}
	return finded;
}