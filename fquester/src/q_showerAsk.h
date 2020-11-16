#pragma once

#include <stdint.h>
#include <string>
#include "res.h"
#include "fgui.h"
#include "objects.h"
#include "QAL_Reader.h"
#include "q_globalVars.h"
#include "textOpt.h"

#include "q_comShow.h"
#include "q_timer.h"
#include "names.h"

bool typeShow = true;

// —низу в вверх
int SHOWERASK_H_RETREAT_DOWN = 10;
int SHOWERASK_H_TEXT = 300;
int SHOWERASK_H_IMAGE = 700;
int SHOWERASK_H_RETREAT_UP = 60;

int SHOWERASK_CTM1_PERSIZE = 90;



float SHOWERASK_VIDEO_DSTPLAYED_FPS = 60;
int SHOWERASK_RED_SYNC_DUR = 100;

int SHOWERASK_TRYRANDOM = 100;

bool sync = true; uint16_t timeRight = 100; 

int sa_del_t; int sa_del_a; bool sa_md;

namespace FQuester {
	struct ShowerAsk {
		

		SDL_Texture* sh_text[2]; SDL_Rect dr_text[2]; 
		SDL_RWops* rw_image; SDL_Texture* sh_image; SDL_Rect dr_image; SDL_Texture* dp_image;
		SDL_Rect rdp_text; SDL_Texture* dp_text; 
		

		bool skipAsk; int typeRandomDp;

		std::wstring ask;
		std::wstring answer;
		
		bool comPeek;

		SDL_Texture* t_helpTab; SDL_Rect r_helpTab; sf::Clock cl_helpTab; bool helpTabInit; int trans;

	public:
		ShowerAsk() {
			sh_text[0] = NULL;
			sh_text[1] = NULL;
			sh_image = NULL;
			v_txtr = NULL;
			rw_image = NULL;
			dp_text = NULL;

			sa_del_t = sa_del_a = -1; sa_md = false;

			// Help Tab
			helpTabInit = false; trans = 0;
		}
		void clear() {
			if (v_txtr) { SDL_DestroyTexture(v_txtr); v_txtr = NULL; }
			if (sh_text[0]) { SDL_DestroyTexture(sh_text[0]); sh_text[0] = NULL; }
			if (sh_text[1]) { SDL_DestroyTexture(sh_text[1]); sh_text[1] = NULL; }
			if (dp_text) { SDL_DestroyTexture(dp_text); dp_text = NULL; }
 			if (sh_image) { SDL_DestroyTexture(sh_image); sh_image = NULL; } 
			if (rw_image) { SDL_FreeRW(rw_image); rw_image = NULL; }
			if (cap.isOpened()) cap.release();	
		}
		void loadAsk(uint16_t _nTopic, uint16_t _nAsk) {
			try {
				sa_del_t = _nTopic; sa_del_a = _nAsk;

				// Start
				comShow.init();
				typeRandomDp = 100;
				skipAsk = false;

				// Clear
				clear();
				comPeek = true;
				n_animMusic = 0;
				if (FileExists(TempLocal + "videobuffer.bin")) remove((TempLocal + "videobuffer.bin").c_str());

				// Start init
				sa_part = 0;
				type = QM_Reader::topics[_nTopic].asks[_nAsk].type;
				plusPoints = QM_Reader::topics[_nTopic].asks[_nAsk].points;

				// Cat random start
				bool good = false;

				if (settings::s_catper != 0) {
					if (!(!settings::s_exchange && !settings::s_transmission && !settings::s_auction && !settings::s_names)) {
						if (FQuester::points[0] != 0 || FQuester::points[1] != 0 || FQuester::points[2] != 0) {
							if (!(FNM::names.empty() && !settings::s_exchange && !settings::s_transmission && !settings::s_auction && settings::s_names)) {

								srand(random_srand.getElapsedTime().asMicroseconds());

								if (rand() % 1000 < settings::s_catper * 10 + 10) {

									int result;

									while (!good) {
										result = rand() % 4;
										if (result == 0 && settings::s_exchange == true) good = true;
										else if (result == 1 && settings::s_transmission == true) good = true;
										else if (result == 2 && settings::s_auction == true) good = true;
										else if (result == 3 && settings::s_names == true) good = true;
									}


									if (result == 0) {
										typeRandomDp = 0;

										Res::aud_haha.play();

										uint8_t sender = rand() % 3;
										uint8_t geter = 69;
										do {
											geter = rand() % 3;
										} while (geter == sender);
										uint8_t psender = rand() % 3;

										std::wstring message = L"¬ыразим уважение ";
										if (sender == 0) message += L"красной команде ";
										else if (sender == 1) message += L"зеленой команде ";
										else if (sender == 2) message += L"синей команде ";
										message += L"за своевольное пожертвование ";
										int perSend = 0;
										if (psender == 0) { message += L"5% "; perSend = 5; }
										else if (psender == 1) { message += L"10% "; perSend = 10; }
										else if (psender == 2) { message += L"25% "; perSend = 25; }
										message += L"своих очков ";
										if (geter == 0) message += L"красной команде.";
										else if (geter == 1) message += L"зеленой команде.";
										else if (geter == 2) message += L"синей команде.";

										points[geter] += points[sender] / 100. * perSend;
										points[sender] -= points[sender] / 100. * perSend;

										updatePoints = true;

										dp_text = FRenderAsk(message, WIDTH_TEXTOUT_RENDER_1);
										SDL_QueryTexture(dp_text, NULL, NULL, &rdp_text.w, &rdp_text.h);
										rdp_text.w = FGUI::sizeDisplay.x * rdp_text.w / 1920.; rdp_text.h = FGUI::sizeDisplay.y * rdp_text.h / 1080.;
										rdp_text.x = FGUI::sizeDisplay.x / 2. - rdp_text.w / 2.; rdp_text.y = FGUI::sizeDisplay.y / 2. - rdp_text.h / 2.;

										good = true;

										if (settings::s_deleteafter) skipAsk = true;
									}
									else if (result == 1) {
										typeRandomDp = 1;

										std::wstring message = L"Ќа этот вопрос должна ответить ";
										uint8_t sender = rand() % 3;
										if (sender == 0) message += L"красна€ команда. ";
										else if (sender == 1) message += L"зелена€ команда. ";
										else if (sender == 2) message += L"син€€ команда. ";
										message += L"¬се остальные молчат! ";

										dp_text = FRenderAsk(message, WIDTH_TEXTOUT_RENDER_1);

										SDL_QueryTexture(dp_text, NULL, NULL, &rdp_text.w, &rdp_text.h);
										rdp_text.w = FGUI::sizeDisplay.x * rdp_text.w / 1920.; rdp_text.h = FGUI::sizeDisplay.y * rdp_text.h / 1080.;
										rdp_text.x = FGUI::sizeDisplay.x / 2. - rdp_text.w / 2.; rdp_text.y = FGUI::sizeDisplay.y / 2. - rdp_text.h / 2.;

										Res::aud_haha.play();
										good = true;
									}
									else if (result == 2) {
										typeRandomDp = 2;

										Res::aud_auction.play();

										dp_text = FRenderAsk(L"јукцион", WIDTH_TEXTOUT_RENDER_1);

										SDL_QueryTexture(dp_text, NULL, NULL, &rdp_text.w, &rdp_text.h);
										rdp_text.w = FGUI::sizeDisplay.x * rdp_text.w / 1920.; rdp_text.h = FGUI::sizeDisplay.y * rdp_text.h / 1080.;
										rdp_text.x = FGUI::sizeDisplay.x / 2. - rdp_text.w / 2.; rdp_text.y = FGUI::sizeDisplay.y / 2. - rdp_text.h / 2.;

										activateAuctionWindow = true;

										good = true;
									}
									else if (result == 3) {

										if (FNM::names.size() > 0) {

											typeRandomDp = 3;

											std::wstring message;

											if (rand() % 2) {
												message = L" оманды, в которых есть \"";
												message += FNM::names[rand() % FNM::names.size()];
												if (rand() % 2) message += L"\", получают ";
												else message += L"\", тер€ют ";
												message += L"половину очков от этого вопроса.";

												if (settings::s_deleteafter) skipAsk = true;
											}
											else {
												message = L"≈сли в отвечающей команде есть \"";
												message += FNM::names[rand() % FNM::names.size()];
												message += L"\", то она получает ";

												uint8_t clrand = rand() % 2;
												if (clrand == 0) { message += L"в полтора раза больше очков от цены вопроса в случае правильного ответа."; }
												else if (clrand == 1) { message += L"двойное количество очков от цены вопроса в случае правильного ответа."; }
											}

											dp_text = FRenderAsk(message, WIDTH_TEXTOUT_RENDER_1);
											SDL_QueryTexture(dp_text, NULL, NULL, &rdp_text.w, &rdp_text.h);
											rdp_text.w = FGUI::sizeDisplay.x * rdp_text.w / 1920.; rdp_text.h = FGUI::sizeDisplay.y * rdp_text.h / 1080.;
											rdp_text.x = FGUI::sizeDisplay.x / 2. - rdp_text.w / 2.; rdp_text.y = FGUI::sizeDisplay.y / 2. - rdp_text.h / 2.;

											Res::aud_haha.play();
											good = true;

										}
									}

									sa_part = 4;
								}
							}
						}
					}
				}

				// Last init
				if (qTimer.mustInit()) qTimer.init();
				qTimer.startLong();

				// Skip ask
				if (!skipAsk) {
					// Graphic init (error)
					if (type == 'V') {
						if (sh_text[0]) SDL_DestroyTexture(sh_text[0]);
						sh_text[0] = NULL;
						if (settings::s_contentTypeModeV == 1) sh_text[0] = FRenderAsk(QM_Reader::topics[_nTopic].asks[_nAsk].ask, WIDTH_TEXTOUT_RENDER_1);
						else sh_text[0] = FRenderAsk(QM_Reader::topics[_nTopic].asks[_nAsk].ask, WIDTH_TEXTOUT_RENDER_0);
					}
					else {
						if (sh_text[0]) SDL_DestroyTexture(sh_text[0]);
						sh_text[0] = NULL;
						if (settings::s_contentTypeMode == 1) sh_text[0] = FRenderAsk(QM_Reader::topics[_nTopic].asks[_nAsk].ask, WIDTH_TEXTOUT_RENDER_1);
						else sh_text[0] = FRenderAsk(QM_Reader::topics[_nTopic].asks[_nAsk].ask, WIDTH_TEXTOUT_RENDER_0);
					}
					SDL_QueryTexture(sh_text[0], NULL, NULL, &dr_text[0].w, &dr_text[0].h);
					if (sh_text[1]) SDL_DestroyTexture(sh_text[1]);
					sh_text[1] = FRenderAsk(QM_Reader::topics[_nTopic].asks[_nAsk].answer, WIDTH_TEXTOUT_RENDER_0);
					SDL_QueryTexture(sh_text[1], NULL, NULL, &dr_text[1].w, &dr_text[1].h);
					dr_text[1].w = FGUI::sizeDisplay.x * dr_text[1].w / 1920.; dr_text[1].h = FGUI::sizeDisplay.y * dr_text[1].h / 1080.;
					dr_text[1].x = FGUI::sizeDisplay.x / 2. - dr_text[1].w / 2.; dr_text[1].y = FGUI::sizeDisplay.y / 2. - dr_text[1].h / 2.;

					// Ask
					if (type != 'V') {
						if (settings::s_contentTypeMode == 0) {
							if (SHOWERASK_H_TEXT * FGUI::sizeDisplay.y / 1080. < dr_text[0].h && type != 'T') {
								dr_text[0].w = (float)SHOWERASK_H_TEXT * FGUI::sizeDisplay.y / 1080. / dr_text[0].h * dr_text[0].w;
								dr_text[0].h = SHOWERASK_H_TEXT * FGUI::sizeDisplay.y / 1080.;
								if (dr_text[0].w > FGUI::sizeDisplay.x) {
									dr_text[0].h = dr_text[0].h * ((FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_text[0].w);
									dr_text[0].w = FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE;
								}
							}
							else {
								dr_text[0].w = FGUI::sizeDisplay.x * dr_text[0].w / 1920.;
								dr_text[0].h = FGUI::sizeDisplay.y * dr_text[0].h / 1080.;
							}
							dr_text[0].y = FGUI::sizeDisplay.y - (SHOWERASK_H_RETREAT_DOWN + SHOWERASK_H_TEXT / 2.) * FGUI::sizeDisplay.y / 1080. - dr_text[0].h / 2.;
						}
						else if (settings::s_contentTypeMode == 1) {
							dr_text[0].w = FGUI::sizeDisplay.x * dr_text[0].w / 1920.;
							dr_text[0].h = FGUI::sizeDisplay.y * dr_text[0].h / 1080.;

							dr_text[0].y = FGUI::sizeDisplay.y / 2. - dr_text[0].h / 2.;
						}
					}
					else {
						if (settings::s_contentTypeModeV == 0) {
							if (SHOWERASK_H_TEXT * FGUI::sizeDisplay.y / 1080. < dr_text[0].h) {
								dr_text[0].w = (float)SHOWERASK_H_TEXT * FGUI::sizeDisplay.y / 1080. / dr_text[0].h * dr_text[0].w;
								dr_text[0].h = SHOWERASK_H_TEXT * FGUI::sizeDisplay.y / 1080.;
								if (dr_text[0].w > FGUI::sizeDisplay.x) {
									dr_text[0].h = dr_text[0].h * ((FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_text[0].w);
									dr_text[0].w = FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE;
								}
							}
							else {
								dr_text[0].w = FGUI::sizeDisplay.x * dr_text[0].w / 1920.;
								dr_text[0].h = FGUI::sizeDisplay.y * dr_text[0].h / 1080.;
							}
							dr_text[0].y = FGUI::sizeDisplay.y - (SHOWERASK_H_RETREAT_DOWN + SHOWERASK_H_TEXT / 2.) * FGUI::sizeDisplay.y / 1080. - dr_text[0].h / 2.;
						}
						else if (settings::s_contentTypeModeV == 1) {
							dr_text[0].w = FGUI::sizeDisplay.x * dr_text[0].w / 1920.;
							dr_text[0].h = FGUI::sizeDisplay.y * dr_text[0].h / 1080.;

							dr_text[0].y = FGUI::sizeDisplay.y / 2. - dr_text[0].h / 2.;
						}
					}


					dr_text[0].x = FGUI::sizeDisplay.x / 2. - dr_text[0].w / 2.;

					// Content
					FILE* f;
					switch (type) {
					case 'T':
						dr_text[0].y = FGUI::sizeDisplay.y / 2. - dr_text[0].h / 2.;
						break;
					case 'A':
						Res::aude_part_one.setDiv(AUDE_DIV_MINUS); Res::aude_part_two.setDiv(AUDE_DIV_MINUS); Res::aude_part_three.setDiv(AUDE_DIV_MINUS); Res::aude_part_four.setDiv(AUDE_DIV_MINUS);
						sb_sound.loadFromMemory(QM_Reader::topics[_nTopic].asks[_nAsk].res, QM_Reader::topics[_nTopic].asks[_nAsk].size_res);
						s_sound.setBuffer(sb_sound);
						if (!good) s_sound.play();

						break;
					case 'V':
						f = fopen((TempLocal + "videobuffer.bin").c_str(), "wb");
						fwrite(QM_Reader::topics[_nTopic].asks[_nAsk].res_v, sizeof(char), QM_Reader::topics[_nTopic].asks[_nAsk].size_res_v, f);
						fclose(f);

						cap.open((TempLocal + "videobuffer.bin").c_str());
						if (!cap.isOpened()) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Could not initialize capturing...", window);

						v_fps = cap.get(CV_CAP_PROP_FPS);

						if (v_txtr) SDL_DestroyTexture(v_txtr);
						v_txtr = SDL_CreateTexture(
							renderer,
							SDL_PIXELFORMAT_BGR24,
							SDL_TEXTUREACCESS_STREAMING,
							(int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
							(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)
						);

						dr_image.h = FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080.;
						dr_image.w = (float)dr_image.h * 640 / 480;
						dr_image.y = FGUI::sizeDisplay.y * (SHOWERASK_H_IMAGE / 2. + SHOWERASK_H_RETREAT_UP) / 1080. - dr_image.h / 2.;
						dr_image.x = FGUI::sizeDisplay.x / 2. - dr_image.w / 2.;

						Res::aude_part_one.setDiv(AUDE_DIV_MINUS); Res::aude_part_two.setDiv(AUDE_DIV_MINUS); Res::aude_part_three.setDiv(AUDE_DIV_MINUS); Res::aude_part_four.setDiv(AUDE_DIV_MINUS);
						sb_sound.loadFromMemory(QM_Reader::topics[_nTopic].asks[_nAsk].res, QM_Reader::topics[_nTopic].asks[_nAsk].size_res);
						s_sound.setBuffer(sb_sound);
						s_sound.setVolume(100.);
						s_sound.setLoop(false);
						s_sound.setPitch(1.);
						if (!good) s_sound.play();

						break;
					case 'I':
						rw_image = SDL_RWFromMem(QM_Reader::topics[_nTopic].asks[_nAsk].res, QM_Reader::topics[_nTopic].asks[_nAsk].size_res);
						auto bufserf = IMG_Load_RW(rw_image, 1);
						sh_image = SDL_CreateTextureFromSurface(renderer, bufserf);
						dr_image.w = bufserf->w;
						dr_image.h = bufserf->h;


						if (settings::s_contentTypeMode == 0) {
							if (dr_image.h > FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080.) {
								dr_image.h = FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080.;
								dr_image.w = (float)dr_image.h * bufserf->w / bufserf->h;
							}

							if (dr_image.h < FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080.) {
								dr_image.y = FGUI::sizeDisplay.y * SHOWERASK_H_RETREAT_UP / 1080. + FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080. / 2. - dr_image.h / 2.;
							}
							else {
								dr_image.y = FGUI::sizeDisplay.y * SHOWERASK_H_RETREAT_UP / 1080.;
							}

							if (settings::s_scalingContent) {
								if (dr_image.h < FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080.) {
									dr_image.w = dr_image.w * ((FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080.) / dr_image.h);
									dr_image.h = FGUI::sizeDisplay.y * SHOWERASK_H_IMAGE / 1080.;
									if (dr_image.w > FGUI::sizeDisplay.x) {
										dr_image.h = dr_image.h * ((FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_image.w);
										dr_image.w = FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE;
									}
								}
							}

							dr_image.y = FGUI::sizeDisplay.y * (SHOWERASK_H_IMAGE / 2. + SHOWERASK_H_RETREAT_UP) / 1080. - dr_image.h / 2.;
						}
						else {
							dr_image.w = FGUI::sizeDisplay.x * dr_image.w / 1920.;
							dr_image.h = FGUI::sizeDisplay.y * dr_image.h / 1080.;

							if (settings::s_scalingContent) {
								if (dr_image.w < FGUI::sizeDisplay.x * 100.f * SHOWERASK_CTM1_PERSIZE) {
									dr_image.h = dr_image.h * ((FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_image.w);
									dr_image.w = FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE;
									if (dr_image.h > FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE) {
										dr_image.w = dr_image.w * ((FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_image.h);
										dr_image.h = FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE;
									}
								}
								if (dr_image.h < FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE) {
									dr_image.w = dr_image.w * ((FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_image.h);
									dr_image.h = FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE;
									if (dr_image.w > FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) {
										dr_image.h = dr_image.h * ((FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_image.w);
										dr_image.w = FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE;
									}
								}
							}
							else {
								if (dr_image.w > FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) {
									dr_image.h = dr_image.h * ((FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_image.w);
									dr_image.w = FGUI::sizeDisplay.x / 100.f * SHOWERASK_CTM1_PERSIZE;
								}
								if (dr_image.h > FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE) {
									dr_image.w = dr_image.w * ((FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE) / dr_image.h);
									dr_image.h = FGUI::sizeDisplay.y / 100.f * SHOWERASK_CTM1_PERSIZE;
								}
							}

							dr_image.y = FGUI::sizeDisplay.y / 2. - dr_image.h / 2.;
						}

						dr_image.x = FGUI::sizeDisplay.x / 2. - dr_image.w / 2.;

						SDL_FreeSurface(bufserf);
						break;
					}
				}
			}
			catch (exception e) {
				sa_part = 100;
			}
		}
		// //////////////////////////////
		void init() {
			if (v_txtr) SDL_DestroyTexture(v_txtr);
			if (sh_text[0]) SDL_DestroyTexture(sh_text[0]);
			if (sh_text[1]) SDL_DestroyTexture(sh_text[1]);
			if (dp_text) SDL_DestroyTexture(dp_text);
			if (sh_image) SDL_DestroyTexture(sh_image);
			if (rw_image) SDL_FreeRW(rw_image);
			if (cap.isOpened()) cap.release();

			// Help Tab
			if (!helpTabInit) {
				helpTabInit = true;
				SDL_Color w; w.r = w.g = w.b = w.a = 255;
				auto autoserf = TTF_RenderUNICODE_Blended(Res::ttf_u_menu_footer, (Uint16*)L"Ќажмите \"TAB\" дл€ того, чтобы открыть инструменты управлени€ медиапотоком.", w);
				t_helpTab = SDL_CreateTextureFromSurface(renderer, autoserf);
				r_helpTab.x = 0;
				r_helpTab.w = autoserf->w;
				r_helpTab.h = autoserf->h;
				SDL_FreeSurface(autoserf);
			}
		}
		void checkEvents() {
			if (type == 'A' && ((settings::s_contentTypeMode == 0 && sa_part < 2) || (settings::s_contentTypeMode == 1 && sa_part < 1)) || type == 'V' && ((settings::s_contentTypeModeV == 0 && sa_part < 2) || (settings::s_contentTypeModeV == 1 && sa_part < 1))) {
				if (event.type == SDL_KEYDOWN) {
					if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_T) {
						showPosMedia = !showPosMedia;
					}

					if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_R) {
						FQuester::s_sound.setLoop(!FQuester::s_sound.getLoop());
					}

					if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_SPACE) {
						if (s_sound.getStatus() == sf::Sound::Status::Paused) s_sound.play();
						else if (s_sound.getStatus() == sf::Sound::Status::Stopped) s_sound.play();
						else if (s_sound.getStatus() == sf::Sound::Status::Playing) s_sound.pause();
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}

					if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_UP) {
						float move = s_sound.getVolume() + 5; if (move > 100.) move = 100.;
						s_sound.setVolume(move);
					}
					else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_DOWN) {
						float move = s_sound.getVolume() - 5; if (move < 0) move = 0;
						s_sound.setVolume(move);
					}

					if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_O) {
						s_sound.setPitch(s_sound.getPitch() - 0.1);
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}
					else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_P) {
						float move = s_sound.getPitch() + 0.1; if (move > 3.) move = 3.;
						s_sound.setPitch(move);
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}
					else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_K) {
						s_sound.setPitch(s_sound.getPitch() - 0.01);
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}
					else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_L) {
						float move = s_sound.getPitch() + 0.01; if (move > 3.) move = 3.;
						s_sound.setPitch(move);
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}
					else if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_M) {
						s_sound.setPitch(1.);
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}

					if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_LEFT) {
						if (s_sound.getStatus() == sf::Sound::Status::Stopped) { s_sound.play(); s_sound.setPlayingOffset(sf::milliseconds(sb_sound.getDuration().asMilliseconds() - SHOWERASK_MUSIC_DIVMOVE)); }
						else {
							float move = s_sound.getPlayingOffset().asMilliseconds() - SHOWERASK_MUSIC_DIVMOVE;
							if (move < 0) move = 0;
							s_sound.setPlayingOffset(sf::milliseconds(move));
						}
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}
					if (FGUI::p_event->key.keysym.scancode == SDL_SCANCODE_RIGHT) {
						float move = s_sound.getPlayingOffset().asMilliseconds() + SHOWERASK_MUSIC_DIVMOVE;
						if (move > sb_sound.getDuration().asMilliseconds() - 5) move = sb_sound.getDuration().asMilliseconds() - 5;
						s_sound.setPlayingOffset(sf::milliseconds(move));
						if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
					}
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (type == 'A' || type == 'V') s_sound.stop();

				if (event.button.button == SDL_BUTTON_LEFT) {
					if (sa_part == 4) { 
						if (skipAsk) sa_part = 3;
						else {
							sa_part = 0;
							if (typeRandomDp == 0 || typeRandomDp == 1 || typeRandomDp == 3) goPlayContent = true;
						}
						Res::aud_haha.stop();
						Res::aud_auction.stop();
					}
					else sa_part++;
				}
				else if (event.button.button == SDL_BUTTON_RIGHT) { 
					if(sa_part != 4) sa_part--; 
					
					if (type == 'A') {
						if (settings::s_contentTypeMode == 0 && sa_part < 2) { s_sound.play(); }
						else if (settings::s_contentTypeMode == 1 && sa_part < 1) { s_sound.play(); }
					}
					if (type == 'V') {
						if (settings::s_contentTypeModeV == 0 && sa_part < 2) { cap.set(CV_CAP_PROP_POS_MSEC, 0.); s_sound.play(); }
						else if (settings::s_contentTypeModeV == 1 && sa_part < 1) { cap.set(CV_CAP_PROP_POS_MSEC, 0.); s_sound.play(); }
					}
				}
				
				if (sa_part == 3) {
					if (settings::s_bmusic) {
						start_num_ask++;

						int musicPer = start_num_ask * 100. / sum_ask;
						int musicPart;
						if (musicPer >= 80) musicPart = 4;
						else if (musicPer >= 46) musicPart = 3;
						else if (musicPer >= 12) musicPart = 2;
						else musicPart = 1;

						if (musicPart == 1 && !Res::aude_part_one.getPlaying()) { Res::aude_part_one.setDiv(AUDE_DIV_PLUS); }
						else if (musicPart == 2 && !Res::aude_part_two.getPlaying()) { Res::aude_part_one.setDiv(AUDE_DIV_MINUS); Res::aude_part_two.setDiv(AUDE_DIV_PLUS); }
						else if (musicPart == 3 && !Res::aude_part_three.getPlaying()) { Res::aude_part_one.setDiv(AUDE_DIV_MINUS); Res::aude_part_two.setDiv(AUDE_DIV_MINUS); Res::aude_part_three.setDiv(AUDE_DIV_PLUS); }
						else if (musicPart == 4 && !Res::aude_part_four.getPlaying()) { Res::aude_part_one.setDiv(AUDE_DIV_MINUS); Res::aude_part_two.setDiv(AUDE_DIV_MINUS); Res::aude_part_three.setDiv(AUDE_DIV_MINUS); Res::aude_part_four.setDiv(AUDE_DIV_PLUS); }
					}

					part_q_game = 1;
					mRenderAskTable = true;

					// Delete last qal ask
					switch (QM_Reader::topics[sa_del_t].asks[sa_del_a].type) {
						case 'I':
							delete QM_Reader::topics[sa_del_t].asks[sa_del_a].res;
							break;
						case 'A':
							delete QM_Reader::topics[sa_del_t].asks[sa_del_a].res;
							break;
						case 'V':
							delete QM_Reader::topics[sa_del_t].asks[sa_del_a].res;
							delete QM_Reader::topics[sa_del_t].asks[sa_del_a].res_v;
							break;
					}
					QM_Reader::topics[sa_del_t].asks.erase(QM_Reader::topics[sa_del_t].asks.begin() + sa_del_a);					
				}
			}
		}
		void display() {
			// Com
			if (comPeek && newButton && lastNumButton > 0) {
				if (lastNumButton == 1) {
					comShow.push(0);
					FQuester::Res::aud_press.play();
					comPeek = false;
				}
				else if (lastNumButton == 2) {
					comShow.push(1);
					FQuester::Res::aud_press.play();
					comPeek = false;
				}
				else if (lastNumButton == 3) {
					comShow.push(2);
					FQuester::Res::aud_press.play();
					comPeek = false;
				}
				newButton = false;
			}

			// Other
			if (goPlayContent) {
				sa_part = 0;
				if (type == 'A' || type == 'V') s_sound.play();
				if (type == 'V') cap.set(CV_CAP_PROP_POS_MSEC, 0.01);
				goPlayContent = false;
			}

			if (sa_part < 1 && type == 'T') sa_part = 1;
			else if (sa_part < 0) sa_part = 0;
			
			if (type == 'V') { if (settings::s_contentTypeModeV == 0 && sa_part < 1) sa_part = 1; }
			else if (settings::s_contentTypeMode == 0 && sa_part < 1) sa_part = 1;

			if(n_frameLimit != FRAME_LIMIT) n_frameLimit = FRAME_LIMIT;
			ignoreUpdateBg = fullIgnoreUpdateBg = false;

			SDL_SetRenderTarget(renderer, NULL);

			if (sa_part == 0) {
				Res::aud_haha.stop();
				Res::aud_auction.stop();
				switch (type) {
					case 'I':
						SDL_RenderCopy(renderer, sh_image, NULL, &dr_image);
						break;
					case 'V':
						if (s_sound.getStatus() == sf::Sound::Status::Stopped) {
							sa_part = 1;
							shadow.set(255, -DIR_SHAD);
						}

						n_frameLimit = 0;
						fullIgnoreUpdateBg = true;

						if (s_sound.getStatus() == sf::Sound::Status::Playing) {
							if (c1.getElapsedTime().asMicroseconds() > (1000000. / v_fps)) {
								c1.restart();
								cap >> frame;
								if (frame.empty()) {
									cap.set(CV_CAP_PROP_POS_MSEC, 0.1);
								}
								FQuester::vPars(v_txtr, frame);
							}
							
						}
						SDL_RenderCopy(renderer, v_txtr, NULL, NULL);

						if (sync) {
							long int droped = (s_sound.getPlayingOffset().asMilliseconds()) - (cap.get(CV_CAP_PROP_POS_MSEC));

							if (droped > timeRight) {
								c3.restart();
								cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
							}
						}

						if (c2.getElapsedTime().asMilliseconds() > 1000. / SHOWERASK_VIDEO_DSTPLAYED_FPS) {
							c2.restart();
							dstPlayed.x = 0;
							dstPlayed.y = FGUI::sizeDisplay.y - FGUI::sizeDisplay.y / 100. * 1;
							dstPlayed.h = FGUI::sizeDisplay.y / 100. * SHOWERASK_AUDIOHEIGHTPERSIZE;
							if (s_sound.getStatus() == sf::Sound::Status::Stopped) dstPlayed.w = (float)FGUI::sizeDisplay.x;
							else dstPlayed.w = (float)FGUI::sizeDisplay.x / sb_sound.getDuration().asMilliseconds() * s_sound.getPlayingOffset().asMilliseconds();
						}
						if (c3.getElapsedTime().asMilliseconds() > SHOWERASK_RED_SYNC_DUR) {
							if (!s_sound.getLoop()) SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); else SDL_SetRenderDrawColor(renderer, 255, 255, 80, 255);
						}
						else { SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); }
						if (showPosMedia) SDL_RenderFillRect(renderer, &dstPlayed);
						
						break;
					case 'A':
						dstPlayed.x = 0;
						dstPlayed.y = FGUI::sizeDisplay.y - FGUI::sizeDisplay.y / 100. * 1;
						dstPlayed.h = FGUI::sizeDisplay.y / 100. * SHOWERASK_AUDIOHEIGHTPERSIZE;
						if (s_sound.getStatus() == sf::Sound::Status::Stopped) dstPlayed.w = (float)FGUI::sizeDisplay.x;
						else dstPlayed.w = (float)FGUI::sizeDisplay.x / sb_sound.getDuration().asMilliseconds() * s_sound.getPlayingOffset().asMilliseconds();
						if (!s_sound.getLoop()) SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); else SDL_SetRenderDrawColor(renderer, 255, 255, 80, 255);
						if (showPosMedia) SDL_RenderFillRect(renderer, &dstPlayed);

						dstAnimMusic.w = FGUI::sizeDisplay.x * 210 / 1920.;
						dstAnimMusic.h = FGUI::sizeDisplay.y * 540 / 1080.;
						dstAnimMusic.x = FGUI::sizeDisplay.x / 2. - dstAnimMusic.w / 2.;
						dstAnimMusic.y = FGUI::sizeDisplay.y / 2. - dstAnimMusic.h / 2.;
						SDL_RenderCopy(renderer, Res::music_anim[n_animMusic], NULL, &dstAnimMusic);

						if (c_animMusic.getElapsedTime().asMilliseconds() > 1000. / SHOWERASK_ANIMAUDIO_FPS && s_sound.getStatus() == sf::Sound::Status::Playing) {
							c_animMusic.restart();
							n_animMusic++;
							if (n_animMusic > Res::music_anim.size() - 1)
								n_animMusic = 0;
						}

						break;
				}
			}
			if (sa_part == 1) {
				Res::aud_haha.stop();
				Res::aud_auction.stop();
				if (type == 'V') {
					if (settings::s_contentTypeModeV == 0) {
						n_frameLimit = 0;


						if (s_sound.getStatus() == sf::Sound::Status::Playing) {
							if (c1.getElapsedTime().asMicroseconds() > (1000000. / v_fps)) {
								c1.restart();
								cap >> frame;
								FQuester::vPars(v_txtr, frame);
							}
							ignoreUpdateBg = true;
						}

						SDL_RenderCopy(renderer, v_txtr, NULL, &dr_image);

						if (sync) {
							long int droped = (s_sound.getPlayingOffset().asMilliseconds()) - (cap.get(CV_CAP_PROP_POS_MSEC));

							if (droped > timeRight) {
								c3.restart();
								cap.set(CV_CAP_PROP_POS_MSEC, s_sound.getPlayingOffset().asMilliseconds());
							}
						}

						if (c2.getElapsedTime().asMilliseconds() > 1000. / SHOWERASK_VIDEO_DSTPLAYED_FPS) {
							c2.restart();
							dstPlayed.x = 0;
							dstPlayed.y = FGUI::sizeDisplay.y - FGUI::sizeDisplay.y / 100. * 1;
							dstPlayed.h = FGUI::sizeDisplay.y / 100. * SHOWERASK_AUDIOHEIGHTPERSIZE;
							if (s_sound.getStatus() == sf::Sound::Status::Stopped) dstPlayed.w = (float)FGUI::sizeDisplay.x;
							else dstPlayed.w = (float)FGUI::sizeDisplay.x / sb_sound.getDuration().asMilliseconds() * s_sound.getPlayingOffset().asMilliseconds();
						}
						if (c3.getElapsedTime().asMilliseconds() > SHOWERASK_RED_SYNC_DUR) {
							if (!s_sound.getLoop()) SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); else SDL_SetRenderDrawColor(renderer, 255, 255, 80, 255);
						}
						else { SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); }
						if (showPosMedia) SDL_RenderFillRect(renderer, &dstPlayed);

						SDL_RenderCopy(renderer, sh_text[0], NULL, &dr_text[0]);
					}
					else if (settings::s_contentTypeModeV == 1) {
						SDL_RenderCopy(renderer, sh_text[0], NULL, &dr_text[0]);
					}
				}
				else if (settings::s_contentTypeMode == 0) {
					switch (type) {
					case 'T':
						SDL_RenderCopy(renderer, sh_text[0], NULL, &dr_text[0]);
						break;
					case 'A':
						dstPlayed.x = 0;
						dstPlayed.y = FGUI::sizeDisplay.y - FGUI::sizeDisplay.y / 100. * 1;
						dstPlayed.h = FGUI::sizeDisplay.y / 100. * SHOWERASK_AUDIOHEIGHTPERSIZE;
						if (s_sound.getStatus() == sf::Sound::Status::Stopped) dstPlayed.w = (float)FGUI::sizeDisplay.x;
						else dstPlayed.w = (float)FGUI::sizeDisplay.x / sb_sound.getDuration().asMilliseconds() * s_sound.getPlayingOffset().asMilliseconds();
						if (!s_sound.getLoop()) SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255); else SDL_SetRenderDrawColor(renderer, 255, 255, 80, 255);
						if (showPosMedia) SDL_RenderFillRect(renderer, &dstPlayed);
						
						dstAnimMusic.w = FGUI::sizeDisplay.x * 210 / 1920.;
						dstAnimMusic.h = FGUI::sizeDisplay.y * 540 / 1080.;
						dstAnimMusic.x = FGUI::sizeDisplay.x / 2. - dstAnimMusic.w / 2.;
						dstAnimMusic.y = FGUI::sizeDisplay.y * (SHOWERASK_H_IMAGE / 2. + SHOWERASK_H_RETREAT_UP) / 1080. - dstAnimMusic.h / 2.;
						SDL_RenderCopy(renderer, Res::music_anim[n_animMusic], NULL, &dstAnimMusic); 
						if (c_animMusic.getElapsedTime().asMilliseconds() > 1000. / SHOWERASK_ANIMAUDIO_FPS / s_sound.getPitch() && s_sound.getStatus() == sf::Sound::Status::Playing) {
							c_animMusic.restart();
							n_animMusic++;
							if (n_animMusic > Res::music_anim.size() - 1)
								n_animMusic = 0;
						}

						SDL_RenderCopy(renderer, sh_text[0], NULL, &dr_text[0]);
						break;
					case 'I':
						SDL_RenderCopy(renderer, sh_image, NULL, &dr_image);
						SDL_RenderCopy(renderer, sh_text[0], NULL, &dr_text[0]);
						break;
					}
				}
				else if (settings::s_contentTypeMode == 1) {
					SDL_RenderCopy(renderer, sh_text[0], NULL, &dr_text[0]);
				}
			}
			else if (sa_part == 2) {
				qTimer.end();
				SDL_RenderCopy(renderer, sh_text[1], NULL, &dr_text[1]);
			}
			else if (sa_part == 4) {
				SDL_RenderCopy(renderer, dp_text, NULL, &rdp_text);
			}

			comShow.display();

			// Timer 
			if (s_sound.getStatus() == sf::Sound::Status::Stopped && sa_part != 4) qTimer.display();


			// Help Tab
			if (helpTab && s_sound.getStatus() == sf::Sound::Status::Playing) {
				cl_helpTab.restart();
				helpTab = false;
				trans = 255;
			}
			if (cl_helpTab.getElapsedTime().asSeconds() < 4) {
				trans = 255;
			}
			else {
				trans--; if (trans < 0) trans = 0;
			}
			if (trans != 0) { 
				r_helpTab.y = FGUI::sizeDisplay.y - FGUI::sizeDisplay.y / 100. - r_helpTab.h; 
				SDL_SetTextureAlphaMod(t_helpTab, trans); 
				SDL_RenderCopy(renderer, t_helpTab, NULL, &r_helpTab); 
			}

		}
		void shutDown() {
			if (cap.isOpened()) cap.release();
			remove((TempLocal + "videobuffer.bin").c_str());
			s_sound.stop();
		}
	} showerAsk;
}