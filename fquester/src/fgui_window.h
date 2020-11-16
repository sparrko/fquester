#pragma once

#include "fgui_window_tpdfs.h"
#include "fgui_cursor.h"
#include <vector>

#include "fgui_element.h"
#include "fgui_e_button.h"
#include "fgui_e_text.h"
#include "fgui_e_textBox.h"
#include "fgui_e_checkBox.h"
#include "fgui_e_consoleList.h"
#include "fgui_e_list.h"
#include "fgui_e_panel.h"
#include "fgui_e_settingsList.h"
#include "fgui_e_serf.h"

#include "fgui_window_call.h"

namespace FGUI {


	//////////////////////////////////////////////////////////////////////////////////////////////////

	winint numWindows = 0;

	class Window {
		// Vars
		private:
			bool callDelete;
			bool life; // and life

			FGUI::Render::RendererPack rp;
			FGUI::IAbWnd iaw;
			FGUI::Render::FTexture ft_caption; std::wstring s_caption;
			FGUI::Render::FTexture ft_icon; uint8_t status_icon; // 0 - nothing, 1 - file, 2+ - def
			FGUI::WinCh::BarButtons buttonBarType; // in WinCh
			winint idu;

			Vector2u lp; // last position(buffer)
			bool move; bool blockMove;
			bool fullscreen; Vector2u lastSizeF; Vector2i lastPositionF;
			bool resizeR, resizeD; bool blockSize;
			uint16_t barButtons; uint8_t stBF, stBS; bool onBF, onBS; uint8_t bufm;

			FGUI::Vector2u minSize; FGUI::Vector2u maxSize;
			std::vector <FGUI::Elements::Element*> elements;

		public:
		// Func		
		void fullUpdateRender() {
			iaw.fullUpdate();
			for (int i = 0; i < elements.size(); i++) {
				elements[i]->draw();
			}
		}

		int getElementsSize() {
			return elements.size();
		}
		void activate() {
			life = true;
			if (idu != 0) { callUpdateIdu.push_back(&idu); }
		}
		void stupidHide() {
			life = false;
		}
		void hide() {
			life = false;
			if (idu != numWindows - 1) { callHideIdu.push_back(&idu); }
		}
		bool getShowed() {
			return life;
		}

		void setCenterColor(FGUI::Color _color) {
			iaw.centerColor(_color);
		}
		FGUI::Color getCenterColor() {
			return iaw.centerColor();
		}

		void setPosition(Vector2i _position) { iaw.setPosition(_position); }
		void setPosition(FGUI::WinCh::Position _pos) {
			if (_pos == FGUI::WinCh::Position::WINDOW_NONE) {
				iaw.setPosition(FGUI::Vars::Window::defPos);
			}
			else if (_pos == FGUI::WinCh::Position::WINDOW_CENTER) {
				iaw.setPosition(FGUI::Vector2i(sizeDisplay.x / 2 - iaw.getSize().x / 2 - FGUI::Vars::Window::borderSize, sizeDisplay.y / 2 - iaw.getSize().y / 2 - FGUI::Vars::Window::hatSize));
			}
		}
		Vector2i getPosition() { return iaw.getPosition(); }

		void setCaption(std::wstring _caption) {
			if (s_caption != _caption) {
				s_caption = _caption;
				ft_caption.init(FGUI::Render::Text_Blended(FGUI::Res::ttf_ubuntu_caption, _caption, FGUI::Vars::Window::c_caption));
				iaw.drawFrame(true); iaw.drawBar(true);
			}
		}
		std::wstring getCaption() {
			return s_caption;
		}

		winint& getRIdu() { return idu; }
		winint getIdu() { return idu; }
		void setIdu(winint _idu) { idu = _idu; }

		void setIcon(std::string file) { status_icon = 1; ft_icon.init(IMG_Load(file.c_str())); }
		void setIcon(FGUI::WinCh::DefIcons defIconType) { status_icon = (int)defIconType + 1; }
		void deleteIcon(){ status_icon = 0; ft_icon.clear(); }

		void setSize(FGUI::Vector2u _size) { iaw.setSize(_size); }
		void setSizeOuter(FGUI::Vector2u _sizeOuter) {
			Vector2u sizeOuter = _sizeOuter;
			_sizeOuter.x = _sizeOuter.x - Vars::Window::borderSize * 2;
			_sizeOuter.y = _sizeOuter.y - Vars::Window::borderSize - Vars::Window::hatSize;
			iaw.setSize(_sizeOuter);
		}
		bool getWasResized() {
			return iaw.wasResized();
		}
		FGUI::Vector2u getSize() { return iaw.getSize(); }

		void setMinSize(FGUI::Vector2u _minSize) { 
			minSize = _minSize;
			if (minSize.x < FGUI::Vars::Window::minSize.x) minSize.x = FGUI::Vars::Window::minSize.x; 
			if (minSize.y < FGUI::Vars::Window::minSize.y) minSize.y = FGUI::Vars::Window::minSize.y;
		}
		FGUI::Vector2u getMinSize() { return minSize; }

		void setMaxSize(FGUI::Vector2u _maxSize) {
			maxSize = _maxSize;
			if (maxSize.x < FGUI::Vars::Window::minSize.x) maxSize.x = FGUI::Vars::Window::minSize.x;
			if (maxSize.y < FGUI::Vars::Window::minSize.y) maxSize.y = FGUI::Vars::Window::minSize.y;
		}
		FGUI::Vector2u getMaxSize() { return maxSize; }

		std::vector <FGUI::Elements::Element*>* getElements() {
			return &elements;
		}

		void setButtonsBar(uint16_t _barButtons) { 
			if (_barButtons == 0) bufm = 0;
			else if ((_barButtons == (uint16_t)WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE) || (_barButtons == (uint16_t)WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN)) { bufm = 1; }
			else if (_barButtons == uint16_t(WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE | WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN)) { bufm = 2; }
			
			barButtons = _barButtons;
		}

		void setFullscreen(bool _fullscreen) {
			if (fullscreen != _fullscreen) {
				fullscreen = _fullscreen;
				if (fullscreen) {
					lastSizeF = iaw.getSize();
					lastPositionF = iaw.getPosition();
					setSizeOuter(sizeDisplay);
					setPosition(Vector2i(0, 0));
				}
				else{
					setPosition(lastPositionF);
					setSize(lastSizeF);
				}
			}
		}
		bool getFullscreen() { return fullscreen; }

		void setBlockSize(bool _blockSize) { blockSize = _blockSize; }

		void setBlockMove(bool _blockMove) { blockMove = _blockMove; }

		bool getLife() { return life; }
		void setLife(bool _life) { life = _life; }

		bool mustDelete() { return callDelete; }

		// Buttons in bar
		void firstButton() { if (!windowWasPressedClose) { hide(); windowWasPressedClose = true; } }
		void secondButton() { setFullscreen(!getFullscreen()); }

		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////

		Window() { callDelete = false; }
		void init(){
			// b.init(Vector2i(1, 1), Vector2u(100, 40));
			// System settings
			idu = 0;
			move = resizeR = resizeD = false; life = true; blockSize = blockMove = false;
			minSize = FGUI::Vars::Window::minSize;
			maxSize = Vector2u(0, 0);
			stBF = stBS = 0; onBF = onBS = false;
			fullscreen = false;
			iaw.init();
			rp.init(&iaw);
			// Start simple settings
			setPosition(FGUI::WinCh::Position::WINDOW_NONE);	
			setCaption(L"FGUI Window");
			setIcon(FGUI::WinCh::DefIcons::WINDOW_DEF_ICON_APP);
			setButtonsBar(WinCh::BarButtons::WINDOW_BAR_BUTTON_CLOSE);
		}
		void update() {
			FGUI::Rect rch;
			// In center
			rch = FGUI::Rect(iaw.getPosition().x + FGUI::Vars::Window::borderSize, iaw.getPosition().y + FGUI::Vars::Window::hatSize, iaw.getSize().x, iaw.getSize().y);
			///////////////////////////////////
			// Elements check
			///////////////////////////////////

			for (int i = 0; i < elements.size(); i++) {
				elements[i]->checkEvents(iaw);
			}

			///////////////////////////////////
			///////////////////////////////////
			///////////////////////////////////
			if (FGUI::CheckRect(rch, cursor.getPosition())) {
				
			}
			// Not in center
			else {
				// Hat
				rch = FGUI::Rect(iaw.getPosition().x, iaw.getPosition().y, iaw.getSize().x + Vars::Window::borderSize * 2, FGUI::Vars::Window::hatSize);
				if (FGUI::CheckRect(rch, cursor.getPosition())) {
					if (!blockMove) {
						rch = FGUI::Rect(iaw.getPosition().x, iaw.getPosition().y, iaw.getSize().x - Vars::Window::buttonSize * bufm - Vars::Window::buttonRetreat * (bufm - 1), FGUI::Vars::Window::hatSize);
						if (FGUI::CheckRect(rch, cursor.getPosition()) && !fullscreen) {
							if (p_event->type == SDL_MOUSEBUTTONDOWN) {
								move = true; lp = FGUI::Vector2u(cursor.getPosition().x - iaw.getPosition().x, cursor.getPosition().y - iaw.getPosition().y);
							}
						}
					}
					if (onBF | onBS) {
						if (onBF) {
							if (p_event->type == SDL_MOUSEBUTTONDOWN) {
								stBF = 2; iaw.drawBar(true);
							}
							else if (p_event->type == SDL_MOUSEBUTTONUP) {
								stBF = 1; iaw.drawBar(true);
							}
						}
						if (onBS) {
							if (p_event->type == SDL_MOUSEBUTTONDOWN) {
								stBS = 2; iaw.drawBar(true);
							}
							else if (p_event->type == SDL_MOUSEBUTTONUP) {
								stBS = 1; iaw.drawBar(true);
							}
						}
					}
					else {
						if (stBF != 0) { stBF = 0; iaw.drawBar(true); }
						else if (stBS != 0) { stBS = 0; iaw.drawBar(true); }
					}
					// Button ups
					if (bufm > 0) {
						rch = Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize - Vars::Window::buttonSize, iaw.getPosition().y + (Vars::Window::hatSize / 2 - Vars::Window::buttonSize / 2), Vars::Window::buttonSize, Vars::Window::buttonSize);
						if (FGUI::CheckRect(rch, cursor.getPosition())) {
							if (p_event->type == SDL_MOUSEBUTTONUP) {
								if (barButtons == WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN) secondButton();
								else firstButton();
							}
						}
					}
					if (bufm > 1) {
						rch = Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize - Vars::Window::buttonSize * 2 - Vars::Window::buttonRetreat, iaw.getPosition().y + (Vars::Window::hatSize / 2 - Vars::Window::buttonSize / 2), Vars::Window::buttonSize, Vars::Window::buttonSize);
						if (FGUI::CheckRect(rch, cursor.getPosition())) {
							if (p_event->type == SDL_MOUSEBUTTONUP) {
								secondButton();
							}
						}
					}
				}
				// Not in hat
				else {
					if (p_event->type == SDL_MOUSEBUTTONDOWN && p_event->button.button == SDL_BUTTON_LEFT && !fullscreen && !blockSize) {
						rch = FGUI::Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize, iaw.getPosition().y + Vars::Window::hatSize, Vars::Window::borderSize, iaw.getSize().y + Vars::Window::borderSize);
						if (FGUI::CheckRect(rch, cursor.getPosition())) {
							resizeR = true; lp.x = cursor.getPosition().x - iaw.getPosition().x - iaw.getSize().x - Vars::Window::borderSize;
						}
						rch = FGUI::Rect(iaw.getPosition().x + Vars::Window::borderSize, iaw.getPosition().y + iaw.getSize().y + Vars::Window::hatSize, iaw.getSize().x + Vars::Window::borderSize, Vars::Window::borderSize);
						if (FGUI::CheckRect(rch, cursor.getPosition())) {
							resizeD = true; lp.y = cursor.getPosition().y - iaw.getPosition().y - iaw.getSize().y - Vars::Window::hatSize;
						}
					}
				}
			}

			// Apply new info
			// - pos
			if (move) { iaw.setPosition(FGUI::Vector2i(cursor.getPosition() - FGUI::Vector2i(lp.x, lp.y))); cursor.setCondition(FGUI::CursorCondition::CURSOR_MOVE); }
			// - Resize
			Vector2i newSize(cursor.getPosition().x - iaw.getPosition().x - Vars::Window::borderSize - lp.x, cursor.getPosition().y - iaw.getPosition().y - Vars::Window::hatSize - lp.y);
			if (newSize.x < (int)minSize.x) newSize.x = minSize.x; if (newSize.y < (int)minSize.y) newSize.y = minSize.y;
			if (newSize.x > (int)maxSize.x && maxSize.x != 0) newSize.x = maxSize.x; if (newSize.y > (int)maxSize.y && maxSize.y != 0) newSize.y = maxSize.y;
			if (resizeR && resizeD) { iaw.setSize(Vector2u(newSize.x, newSize.y)); cursor.setCondition(CursorCondition::CURSOR_SIZE); }
			else if (resizeR) { iaw.setSize(Vector2u(newSize.x, iaw.getSize().y)); cursor.setCondition(CursorCondition::CURSOR_SIZELEFT); }
			else if (resizeD) { iaw.setSize(Vector2u(iaw.getSize().x, newSize.y)); cursor.setCondition(CursorCondition::CURSOR_SIZETOP); }
			// Cancel
			if (p_event->type == SDL_MOUSEBUTTONUP) {
				move = resizeD = resizeR = false;
			}
		}
		bool check() { // will call "update" if true
			if (life) {
				iaw.start();
				// Buttons Bar
				FGUI::Rect rch;
				if (bufm > 0) {
					rch = Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize - Vars::Window::buttonSize, iaw.getPosition().y + (Vars::Window::hatSize / 2 - Vars::Window::buttonSize / 2), Vars::Window::buttonSize, Vars::Window::buttonSize);
					if (FGUI::CheckRect(rch, cursor.getPosition())) {
						if (barButtons == WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN) {
							if (onBS != true) {
								onBS = true; stBS = 1; iaw.drawBar(true);
							}
						}
						else {
							if (onBF != true) {
								onBF = true; stBF = 1; iaw.drawBar(true);
							}
						}
					}
					else { 
						if (barButtons == WinCh::BarButtons::WINDOW_BAR_BUTTON_FULLSCREEN) {
							if (stBS != 0) {
								stBS = 0; onBS = false; iaw.drawBar(true);
							}
						}
						else {
							if (stBF != 0) {
								stBF = 0; onBF = false; iaw.drawBar(true);
							}
						}
					}
				}
				if (bufm > 1) {
					rch = Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize - Vars::Window::buttonSize * 2 - Vars::Window::buttonRetreat, iaw.getPosition().y + (Vars::Window::hatSize / 2 - Vars::Window::buttonSize / 2), Vars::Window::buttonSize, Vars::Window::buttonSize);
					if (FGUI::CheckRect(rch, cursor.getPosition())) {
						if (onBS != true) {
							onBS = true; stBS = 1; iaw.drawBar(true);
						}
					}
					else { if (stBS != 0) { stBS = 0; onBS = false; iaw.drawBar(true); } }
				}

				rch = Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize - (Vars::Window::buttonSize + Vars::Window::buttonRetreat) * bufm, iaw.getPosition().y + (Vars::Window::hatSize / 2 - Vars::Window::buttonSize / 2), Vars::Window::buttonSize, Vars::Window::buttonSize);
				if (!FGUI::CheckRect(rch, cursor.getPosition())) {
					if (p_event->type == SDL_MOUSEBUTTONUP) {
						if (stBF == 2) { stBF = 1; }
						if (stBS == 2) { stBS = 1; }
					}
				}

				if (!centerFinded) {
					// Return
					if (move || resizeR || resizeD) { centerFinded = true; return true; }
					rch = Rect(iaw.getPosition().x, iaw.getPosition().y, iaw.getSize().x + Vars::Window::borderSize * 2, iaw.getSize().y + Vars::Window::borderSize + Vars::Window::hatSize);
					if (FGUI::CheckRect(rch, cursor.getPosition())) {
						centerFinded = true;
						// mouse icon only for first window
						// return
						if (!windowCheckFinded) {
							// down right size
							rch = FGUI::Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize, iaw.getPosition().y + iaw.getSize().y + Vars::Window::hatSize, Vars::Window::borderSize, Vars::Window::borderSize);
							if (FGUI::CheckRect(rch, cursor.getPosition()) && !fullscreen && !blockSize) { cursor.setCondition(CursorCondition::CURSOR_SIZE); }
							// down size
							rch = FGUI::Rect(iaw.getPosition().x + Vars::Window::borderSize, iaw.getPosition().y + iaw.getSize().y + Vars::Window::hatSize, iaw.getSize().x, Vars::Window::borderSize);
							if (FGUI::CheckRect(rch, cursor.getPosition()) && !fullscreen && !blockSize) { cursor.setCondition(CursorCondition::CURSOR_SIZETOP); }
							// right size
							rch = FGUI::Rect(iaw.getPosition().x + iaw.getSize().x + Vars::Window::borderSize, iaw.getPosition().y + Vars::Window::hatSize, Vars::Window::borderSize, iaw.getSize().y);
							if (FGUI::CheckRect(rch, cursor.getPosition()) && !fullscreen && !blockSize) { cursor.setCondition(CursorCondition::CURSOR_SIZELEFT); }

							if (p_event->button.button == SDL_BUTTON_LEFT && p_event->type == SDL_MOUSEBUTTONDOWN) {
								if (idu != 0) { callUpdateIdu.push_back(&idu); }
								return true;
							}
							windowCheckFinded = true;
						}
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		void render() {
			if (life) {
				rp.renderFrame(ft_caption, status_icon, ft_icon, FGUI::Res::ft_i);
				rp.renderButtonsBar(barButtons, stBF, stBS, FGUI::Res::ft_b);
				rp.renderCenter();

				for (int i = 0; i < elements.size(); i++) {
					elements[i]->display(rp);
				}
			}
		}
		void display() {
			if (life) {
				render();
				FGUI::Render::PresentWindow(&rp, iaw.getPosition());
			}
		}
		void deinit() {
			hide();
			callDelete = true;
			rp.deinit();
		}

		///////////////////////////////////////////////////////////////////
		// Elements create
		
		void tieElement(FGUI::Elements::Element* _element) {
			//if (elements.size() + 1 <= Vars::Window::maxElements) {
				elements.push_back(_element);
			//}
		}
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector <Window*> WindowsList(std::numeric_limits<winint>::max());
	std::vector <Window*> iduList;
	std::vector <Window*> ExistsWindow;
}