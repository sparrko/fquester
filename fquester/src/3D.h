#include <iostream>
#include <stdint.h>
#include <cmath>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SDL.h>
#include "fgui.h"
//
#define ToRadian(x) ((x) * 3.141593f / 180.0f)
//
namespace sr { // Soft 3D Render
	bool optim = false;
	// Matrix
	typedef sf::Vector3f Matrix[3];

	Matrix m;
	// Rotate with matrix
	void RotateVertex(sf::Vector3f* t, sf::Vector3f* r) {
		sf::Vector3f b = *t;
		t->x = (cos(ToRadian(r->x)) * cos(ToRadian(r->y)) * b.x) + ((cos(ToRadian(r->x)) * sin(ToRadian(r->y)) * sin(ToRadian(r->z)) - sin(ToRadian(r->x)) * cos(ToRadian(r->z))) * b.y) + ((cos(ToRadian(r->x)) * sin(ToRadian(r->y)) * cos(ToRadian(r->z)) + sin(ToRadian(r->x)) * sin(ToRadian(r->z))) * b.z);
		t->y = (sin(ToRadian(r->x)) * cos(ToRadian(r->y)) * b.x) + ((sin(ToRadian(r->x)) * sin(ToRadian(r->y)) * sin(ToRadian(r->z)) + cos(ToRadian(r->x)) * cos(ToRadian(r->z))) * b.y) + ((sin(ToRadian(r->x)) * sin(ToRadian(r->y)) * cos(ToRadian(r->z)) - cos(ToRadian(r->x)) * sin(ToRadian(r->z))) * b.z);
		t->z = (-sin(ToRadian(r->y)) * b.x) + (cos(ToRadian(r->y)) * sin(ToRadian(r->z)) * b.y) + (cos(ToRadian(r->y)) * cos(ToRadian(r->z)) * b.z);
	}
	////////////////////////////////////////////////////////////////////////////
	float fov = 90;
	const unsigned short int ren_width = 512, ren_height = 384;
	SDL_Surface* serf;
	sf::Vector3f rotateWorld = sf::Vector3f(0, 0, 0);
	sf::Vector3f posWorld = sf::Vector3f(0, 0, 0);
	sf::Vector3f rotateCam = sf::Vector3f(0, 0, 0);
	sf::Vector3f posCam = sf::Vector3f(0, 0, 0);
	sf::Vector2i viewPos = sf::Vector2i(ren_width / 2, ren_height / 2);
	////////////////////////////////////////////////////////////////////////////
	void SetPrimary() {
		rotateWorld = sf::Vector3f(0, 0, 0); posWorld = sf::Vector3f(0, 0, 0);
		rotateCam = sf::Vector3f(0, 0, 0); posCam = sf::Vector3f(0, 0, 0);
	}
	void Rotatef(sf::Vector3f _rot) { rotateWorld += _rot; }
	void Rotatef(sf::Vector2f _rot) { rotateWorld.x += _rot.x; rotateWorld.y += _rot.y; }
	void Translatef(sf::Vector3f _pos) {
		posWorld += _pos;
	}
	void CamLookAt(sf::Vector3f _pos, sf::Vector2f _angle) {
		rotateCam.y = _angle.x;
		rotateCam.z = -_angle.y;

		posCam.x = -_pos.x;
		posCam.y = -_pos.y;
		posCam.z = _pos.z;
	}
	////////////////////////////////////////////////////////////////////////////
	bool SetPixel(FGUI::Vector2i _pos, uint32_t _color = 0xffffffff) {
		if (!(_pos.y > serf->h || _pos.y < 0 || _pos.x > serf->w || _pos.x < 0)) {
			Uint32* target_pixel = (Uint32*)serf->pixels + (_pos.y * serf->w) + _pos.x;
			*target_pixel = _color;
			return true;
		}
		else return false;
	}
	void Draw2DLine(sf::Vector2i _start, sf::Vector2i _end, sf::Uint32 _color = 0xffffffff) {
		bool steep = false;
		if (std::abs(_start.x - _end.x) < std::abs(_start.y - _end.y)) {
			std::swap(_start.x, _start.y);
			std::swap(_end.x, _end.y);
			steep = true;
		}
		if (_start.x > _end.x) {
			std::swap(_start.x, _end.x);
			std::swap(_start.y, _end.y);
		}
		int dx = _end.x - _start.x;
		int dy = _end.y - _start.y;
		int derror2 = std::abs(dy) * 2;
		int error2 = 0;
		int y = _start.y;
		for (int x = _start.x; x <= _end.x; x++) {
			if (steep) {
				if (!SetPixel(FGUI::Vector2i(x, y), _color) && optim) break;
			}
			else {
				if (!SetPixel(FGUI::Vector2i(y, x), _color) && optim) break;
			}
			error2 += derror2;

			if (error2 > dx) {
				y += (_end.y > _start.y ? 1 : -1);
				error2 -= dx * 2;
			}
		}
	}
	void DrawLineOfTriangle(sf::Vector3f _v[3]) {
		sf::Vector3f* cv = new sf::Vector3f;
		cv = _v;

		RotateVertex(&cv[0], &rotateWorld);
		RotateVertex(&cv[1], &rotateWorld);
		RotateVertex(&cv[2], &rotateWorld);

		cv[0] += posCam;
		cv[1] += posCam;
		cv[2] += posCam;

		RotateVertex(&cv[0], &rotateCam);
		RotateVertex(&cv[1], &rotateCam);
		RotateVertex(&cv[2], &rotateCam);

		if (!(cv[0].z < posWorld.z || cv[1].z < posWorld.z || cv[2].z < posWorld.z)) {
			cv[0].x = (cv[0].x - posWorld.x) / std::abs(posWorld.z - cv[0].z) / ToRadian(fov / 512);
			cv[0].y = (cv[0].y - posWorld.y) / std::abs(posWorld.z - cv[0].z) / ToRadian(fov / 512);
			cv[1].x = (cv[1].x - posWorld.x) / std::abs(posWorld.z - cv[1].z) / ToRadian(fov / 512);
			cv[1].y = (cv[1].y - posWorld.y) / std::abs(posWorld.z - cv[1].z) / ToRadian(fov / 512);
			cv[2].x = (cv[2].x - posWorld.x) / std::abs(posWorld.z - cv[2].z) / ToRadian(fov / 512);
			cv[2].y = (cv[2].y - posWorld.y) / std::abs(posWorld.z - cv[2].z) / ToRadian(fov / 512);

			Draw2DLine(sf::Vector2i((int)(cv[0].x + viewPos.x), (int)(cv[0].y + viewPos.y)), sf::Vector2i((int)(cv[1].x + viewPos.x), (int)(cv[1].y + viewPos.y)));
			Draw2DLine(sf::Vector2i((int)(cv[1].x + viewPos.x), (int)(cv[1].y + viewPos.y)), sf::Vector2i((int)(cv[2].x + viewPos.x), (int)(cv[2].y + viewPos.y)));
			Draw2DLine(sf::Vector2i((int)(cv[2].x + viewPos.x), (int)(cv[2].y + viewPos.y)), sf::Vector2i((int)(cv[0].x + viewPos.x), (int)(cv[0].y + viewPos.y)));
		}
	}
}