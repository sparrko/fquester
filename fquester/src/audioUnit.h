#pragma once

#include <SFML/Audio.hpp>

#define FPS_AUDIO 60.

#define AUDE_DIV_PLUS 1.
#define AUDE_DIV_MINUS -2.

struct Aud {
	sf::SoundBuffer sb; 
	sf::Sound aud;
	float volume; // 0% - 100%
	float div;
	Aud() { volume = 0.; div = 0; }
	void openFromMem(const void* data, size_t size, bool loop) {
		sb.loadFromMemory(data, size);
		aud.setBuffer(sb); 
		aud.setLoop(loop);
	}
	void setPitch(float pitch) {
		aud.setPitch(pitch);
	}
	void setVolume(float _volume) {
		volume = _volume;
	}
	void setDiv(float _div) {
		div = _div;
	}
	bool getPlaying() {
		if (aud.getStatus() == sf::Sound::Status::Playing) return true;
		else return false;
	}
};

class AudEngine{
	sf::Clock clock;
	std::vector <Aud*> mulist;
	public:
	void update() {
		if (clock.getElapsedTime().asMilliseconds() > 1000. / FPS_AUDIO) {
			for (int a = 0; a < mulist.size(); a++) {
				if (mulist[a]->div != 0.) {
					if (mulist[a]->aud.getStatus() != sf::Sound::Status::Playing) mulist[a]->aud.play();
					mulist[a]->volume += mulist[a]->div;
					if (mulist[a]->volume < 0) {
						mulist[a]->volume = 0; mulist[a]->div = 0;
						mulist[a]->aud.stop();
					}
					else if (mulist[a]->volume > 100) {
						mulist[a]->volume = 100; mulist[a]->div = 0;
					}
					mulist[a]->aud.setVolume(mulist[a]->volume);
				}
			}
			clock.restart();
		}
	}
	void addInArray(Aud* aud) {
		mulist.push_back(aud);
	}
} audengine;