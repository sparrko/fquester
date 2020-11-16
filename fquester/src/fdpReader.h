#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <io.h>
#include <algorithm>
#include <ctime>

#define FDP_PACKER_FORBIDDENCHARACTERS "<>|?*:"

#define FDP_READER_FORMAT "FDPC"

namespace FDP {

	bool FileExists(const char* _file) {
		return access(_file, 0) != -1;
	}
	bool WFileExists(const wchar_t* _file) {
		return _waccess(_file, 0) != -1;
	}

	struct UnitR {
		std::string fdpFile;
		char* res;
		uint64_t size;
		uint64_t pos;
	};

	class Reader {
		std::string error;
		std::vector <UnitR> resources;
		std::string file;
		std::string time;
		std::string about;
		uint64_t lastSize;
	public:
		bool open(std::string _file) {
			if (!FileExists(_file.c_str())) { error = "File not found."; return false; }

			FILE* rf = fopen(_file.c_str(), "rb");
			file = _file;

			// Read head
			std::string buf = "";
			for (uint8_t i = 0; i < 4; i++) buf += (char)fgetc(rf);
			if (buf != FDP_READER_FORMAT) { error = "Wrong format file."; return false; }

			// Read about
			if (!about.empty()) about.clear();
			char bufc;
			while (true) { bufc = (char)fgetc(rf); if (bufc == '|') break; else about += bufc; }

			// Read time
			if (!time.empty()) time.clear();
			for (uint8_t i = 0; i < 14; i++) time += (char)fgetc(rf);


			// Read pathes
			std::string depth = ""; std::string cons = ""; uint16_t posRes;
			while (true) {
				bufc = (char)fgetc(rf);

				if (bufc == '>') {
					while (true) {
						char buffc = (char)fgetc(rf);
						if (buffc == '>' || buffc == '<' || buffc == ':' || buffc == '|') { _fseeki64(rf, _ftelli64(rf) - 1, SEEK_SET); break; }
						else depth += buffc;
					}
					depth += "/";
					_fseeki64(rf, _ftelli64(rf) - 1, SEEK_SET);
				}
				else if (bufc == '<') {
					if (depth[depth.size() - 1] == '/') depth.resize(depth.size() - 1);
					while (true) {
						if (depth.size() == 0) break;
						else if (depth[depth.size() - 1] == '/') break;
						else depth.resize(depth.size() - 1);
					}
				}
				else if (bufc == ':') {
					while (true) {
						char buffc = (char)fgetc(rf);
						if (buffc == '?') { break; }
						else depth += buffc;
					}
					std::string poss;
					while (true) {
						char buffc = (char)fgetc(rf);
						if (buffc == '*') { break; }
						else poss += buffc;
					}
					std::string sizes;
					while (true) {
						char buffc = (char)fgetc(rf);
						if (buffc == ':' || buffc == '>' || buffc == '<' || buffc == '|') { _fseeki64(rf, _ftelli64(rf) - 1, SEEK_SET); break; }
						else sizes += buffc;
					}

					resources.resize(resources.size() + 1);

					resources[resources.size() - 1].fdpFile = depth;
					resources[resources.size() - 1].pos = std::atol(poss.c_str());
					resources[resources.size() - 1].size = std::atol(sizes.c_str());

					while (true) {
						if (depth.size() == 0) break;
						else if (depth[depth.size() - 1] == '/') break;
						else depth.resize(depth.size() - 1);
					}
				}
				else if (bufc == '|') { posRes = _ftelli64(rf); break; }
			}

			// Init res
			for (uint16_t i = 0; i < resources.size(); i++) {
				resources[i].res = new char[resources[i].size];
				_fseeki64(rf, resources[i].pos + posRes, SEEK_SET);
				fread(resources[i].res, sizeof(char), resources[i].size, rf); // experemental
				//for (uint64_t j = 0; j < resources[i].size; j++) {
				//	resources[i].res[j] = (char)fgetc(rf);
				//}
			}

			return true;
		}
		char* getRes(std::string fdpFile) {
			for (uint16_t i = 0; i < resources.size(); i++) {
				if (resources[i].fdpFile == fdpFile) { lastSize = resources[i].size; return resources[i].res; }
			}
			lastSize = 0;
			return NULL;
		}
		uint64_t getSize(std::string fdpFile) {
			for (uint16_t i = 0; i < resources.size(); i++) {
				if (resources[i].fdpFile == fdpFile) return resources[i].size;
			}
			return 0;
		}
		void clear() {
			if (!resources.empty()) {
				for (uint16_t i = 0; i < resources.size(); i++) {
					delete[]resources[i].res;
				}
				resources.clear();
				resources.resize(0);
				resources.shrink_to_fit();
				std::vector<UnitR>().swap(resources);
			}
		}
		// Test
		void showInfo() {
			if (!file.empty() && !about.empty() && !time.empty()) {
				std::cout << "About " + file + ": " << std::endl;
				std::cout << "\tFile name: " + file << std::endl;
				std::cout << "\tAbout this pack: " + about << std::endl;
				std::cout << "\tTime create: " << time.substr(0, 4) + "-" <<
					time.substr(4, 2) << "-" <<
					time.substr(6, 2) << " " <<
					time.substr(8, 2) << ":" <<
					time.substr(10, 2) << ":" <<
					time.substr(12, 2) << std::endl;
			}
		}
		void showTree() {
			if (!resources.empty()) {
				std::cout << "Tree of " + file + ": " << std::endl;
				for (int i = 0; i < resources.size(); i++) {
					std::cout << "\t" + resources[i].fdpFile << std::endl;
				}
			}
		}
	};
}