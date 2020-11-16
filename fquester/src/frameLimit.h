#pragma once

#include <SFML/System.hpp>
int FRAME_LIMIT = 1000; // def
int n_frameLimit = FRAME_LIMIT;

void setFrameLimit(int _n) {
	FRAME_LIMIT = n_frameLimit = _n;
}
