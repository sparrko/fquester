#pragma once

#include "fgui_window.h"

namespace FGUI {
	Window* FCreateWindow() {
		numWindows++;
		WindowsList[numWindows - 1] = new Window;

		for (int a = 0; a < numWindows - 1; a++) {
			WindowsList[a]->setIdu(WindowsList[a]->getIdu() + 1);
		}

		ExistsWindow.resize(numWindows);
		ExistsWindow[ExistsWindow.size() - 1] = WindowsList[numWindows - 1];
		return WindowsList[numWindows - 1];
	}
	void DeleteWindow(Window* _indexWindow) {
		for (int za = 0; za < numWindows; za++) {
			if (_indexWindow == ExistsWindow[za]) {
				winint buf = _indexWindow->getIdu();
				for (int a = 0; a < numWindows; a++) {
					if (WindowsList[a] == _indexWindow) {
						std::swap(WindowsList[a], WindowsList[numWindows - 1]);
						break;
					}
				}
				for (int a = buf; a < numWindows; a++) {
					for (int b = 0; b < numWindows; b++) {
						if (WindowsList[b]->getIdu() == a) {
							WindowsList[b]->setIdu(WindowsList[b]->getIdu() - 1);
							break;
						}
					}
				}
				WindowsList[numWindows - 1]->deinit();
				delete WindowsList[numWindows - 1];
				numWindows--;

				std::swap(ExistsWindow[za], ExistsWindow[ExistsWindow.size() - 1]);
				ExistsWindow.resize(ExistsWindow.size() - 1);
				break;
			}

		}
	}
	void DeleteLastWindow() {
		if (numWindows != 0) {
			for (int a = 0; a < numWindows; a++) {
				if (WindowsList[a]->getIdu() == (numWindows - 1)) {
					std::swap(WindowsList[a], WindowsList[numWindows - 1]);
					WindowsList[numWindows - 1]->deinit();
					delete WindowsList[numWindows - 1];
					numWindows--;
					break;
				}
			}
		}
	}
	winint getCountsWindow() {
		return numWindows;
	}
	void clearUnits() {
		for (int a = 0; a < numWindows; a++) {
			iduList[a]->deinit();
		}
	}
}
