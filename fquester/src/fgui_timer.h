#pragma once

#include <SDL.h>
#include <stdint.h>

namespace FGUI {
	// 49,71026961805556 days
	class Timer {
			uint32_t ticks;
		public:
			void restart() {
				ticks = SDL_GetTicks();
			}
			Timer() { restart(); }
			uint32_t getMilliseconds() {
				return (SDL_GetTicks() - ticks);
			}
			void setMilliseconds(uint32_t milliseconds) {
				ticks = SDL_GetTicks() - milliseconds;
			}
	};
}