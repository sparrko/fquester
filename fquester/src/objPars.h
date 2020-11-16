#pragma once

#include <string>
#include <fstream>
#include <SFML/System.hpp>
#include <iostream>
#include <io.h>

namespace fs {
	struct Edge {
		sf::Vector3f v1;
		sf::Vector3f v2;
		Edge(sf::Vector3f _v1, sf::Vector3f _v2) {
			v1 = _v1; v2 = _v2;
		}
	};
	struct Triangle {
		sf::Vector3f v[3];
		sf::Vector2f t[3];
		sf::Vector3f n;
	};
	struct ModelIndex {
		//		vertex / texture / normal
		unsigned int v[3], t[3], n[3];
	};
	struct Model {
		std::vector <sf::Vector3f> vList;
		std::vector <sf::Vector2f> tList;
		std::vector <sf::Vector3f> nList;
		std::vector <ModelIndex> fList;
		unsigned int GetCountTriangles() { return fList.size(); }
		Triangle GetTriangle(unsigned int _numTriangle) {
			Triangle returnTriangle;
			for (int a = 0; a < 3; a++) returnTriangle.v[a] = vList[fList[_numTriangle].v[a] - 1];
			for (int a = 0; a < 3; a++) returnTriangle.t[a] = tList[fList[_numTriangle].t[a] - 1];
			returnTriangle.n = nList[fList[_numTriangle].n[0] - 1];

			return returnTriangle;
		}
	};
	Model* LoadModel(std::string _file) {
		Model* mdl = new Model;
		if (_access(_file.c_str(), 0) == -1) { std::cout << "Can't open model: " << _file << std::endl; return mdl; }
		else {
			std::ifstream file(_file.c_str());
			std::string word; unsigned int membuf;
			while (!file.eof()) {
				file >> word;
				if (word == "v")
				{
					mdl->vList.resize(mdl->vList.size() + 1);
					file >> word; mdl->vList[mdl->vList.size() - 1].x = -std::atof(word.c_str()); ///////////////// CHANGE(ROTATE) DOWNUP
					file >> word; mdl->vList[mdl->vList.size() - 1].y = std::atof(word.c_str());
					file >> word; mdl->vList[mdl->vList.size() - 1].z = std::atof(word.c_str());
				}
				else if (word == "vt") {
					mdl->tList.resize(mdl->tList.size() + 1);
					file >> word; mdl->tList[mdl->tList.size() - 1].x = std::atof(word.c_str());
					file >> word; mdl->tList[mdl->tList.size() - 1].y = std::atof(word.c_str());
				}
				else if (word == "vn") {
					mdl->nList.resize(mdl->nList.size() + 1);
					file >> word; mdl->nList[mdl->nList.size() - 1].x = std::atof(word.c_str());
					file >> word; mdl->nList[mdl->nList.size() - 1].y = std::atof(word.c_str());
					file >> word; mdl->nList[mdl->nList.size() - 1].z = std::atof(word.c_str());
				}
				else if (word == "f") {
					mdl->fList.resize(mdl->fList.size() + 1);

					for (int i = 0; i < 3; i++) {
						// v
						file >> word;
						for (int a = 0; a < word.length(); a++) {
							if (word[a] == '/') {
								mdl->fList[mdl->fList.size() - 1].v[i] = std::atoi(word.substr(0, a).c_str());
								membuf = a;
								break;
							}
						}
						// t
						for (int a = membuf + 1; a < word.length(); a++) {
							if (word[a] == '/') {
								mdl->fList[mdl->fList.size() - 1].t[i] = std::atoi(word.substr(membuf + 1, a - membuf).c_str());
								membuf = a;
								break;
							}
						}
						// n
						mdl->fList[mdl->fList.size() - 1].n[i] = std::atoi(word.substr(membuf + 1, word.length() - (membuf + 1)).c_str());
					}
				}
			}
			file.clear();
			file.close();

			return mdl;
		}
	}
}