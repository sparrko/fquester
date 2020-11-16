#pragma once

#include <stdint.h>

namespace FGUI {
	// -------------------------------------- //
	// typdefines
	typedef uint8_t winint;
	// -------------------------------------- //
	// Vectors
	template <typename T>
	struct Vector2 {
		T x, y;
		Vector2() {}
		Vector2(T _x, T _y) { x = _x; y = _y; }

		friend std::ostream& operator <<(std::ostream& os, const Vector2<T>& out){
			os << out.x << ", " << out.y; return os;
		}
		friend Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right) {
			Vector2<T> buf; buf.x = left.x + right.x; buf.y = left.y + right.y; return buf;
		}
		friend Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right) {
			Vector2<T> buf; buf.x = left.x - right.x; buf.y = left.y - right.y; return buf;
		}
		friend Vector2<T> operator +=(const Vector2<T>& left, const Vector2<T>& right) {
			left.x = left.x + right.x; left.y = left.y + right.y;
		}
		friend Vector2<T> operator -=(const Vector2<T>& left, const Vector2<T>& right) {
			left.x = left.x - right.x; left.y = left.y - right.y;
		}
	};

	typedef Vector2<float> Vector2f;
	typedef Vector2<int> Vector2i;
	typedef Vector2<unsigned int> Vector2u;

	// -------------------------------------- //
	// Color
	struct Color {
		uint8_t r, g, b, a;
		Color() {}
		Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255) { r = _r; g = _g; b = _b; a = _a; }
	};
	namespace Colors {
		// Main
		const Color Red = FGUI::Color(255, 0, 0);
		const Color Green = FGUI::Color(0, 255, 0);
		const Color Blue = FGUI::Color(0, 0, 255);
		const Color Transparent = FGUI::Color(0, 0, 0, 0);

		// Brightness
		const Color Black = FGUI::Color(0, 0, 0);
		const Color White = FGUI::Color(255, 255, 255);

		// Additional
		const Color Yellow = FGUI::Color(255, 255, 0);
		const Color Magenta = FGUI::Color(255, 0, 255);
		const Color Cyan = FGUI::Color(0, 255, 255);

		// Especially for FGUI
		namespace SpecGUI {
			const Color Control = FGUI::Color(200, 200, 200);
		}
	}

	// -------------------------------------- //
	// Rect
	struct Rect {
		int x, y, w, h;
		Rect() {}
		Rect(int _x, int _y, int _w, int _h) { x = _x; y = _y; w = _w; h = _h; }
		Rect(int _all) { x = y = w = h = _all; }
	};
	bool CheckRect(FGUI::Rect rect, FGUI::Vector2i mousePos) {
		if (mousePos.x > rect.x && mousePos.y > rect.y && mousePos.x < rect.x + rect.w && mousePos.y < rect.y + rect.h) return true;
		else return false;
	}

	// -------------------------------------- //
	// Other
	Vector2i GetVectorSize(FGUI::Rect r) // Only size 
	{
		FGUI::Vector2i vec; vec.x = r.w; vec.y = r.h;
		return vec;
	}
}