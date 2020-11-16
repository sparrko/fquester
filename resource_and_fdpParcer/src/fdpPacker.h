#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <io.h>
#include <algorithm>
#include <ctime>

#include <iostream>

//#define FDP_PACKER_DEVMODE
#define FDP_PACKER_OUTPUTLOCALE ""
#define FDP_PACKER_OFICIALBUILD
#define FDP_PACKER_FORBIDDENCHARACTERS "<>|?*:"

//// TREE EXAMPLE /////////////////////////////////////////////////////////////////////////////////////////
//
// FDPCTest file20022102205400>input:logoGame.png?0*2077772:loop.ogg?2077772*91971:rus.flf?2169743*153<:a.flf?2169896*153|
// год день месяц часы минуты секунды

bool FileExists(const char* fname)
{ 
	return access(fname, 0) != -1; 
}

uint16_t numCatalogs;

/////////////////////////////////////////////////////////////////

struct FDP_UnitP {
	std::string fdpFile;
	std::string realFile;
	uint64_t size;
};
struct FDP_UnitP_SortStruct {
	bool operator () (const FDP_UnitP& _a, const FDP_UnitP& _b) {
		// Copy
		std::string a = _a.fdpFile;
		std::string b = _b.fdpFile;

		// Delete big simbols
		for (uint16_t i = 0; i < a.length(); i++) { // A
			if (a[i] >= 65 && a[i] <= 90) a[i] = a[i] + 32; // ENG
			else if (a[i] >= 127 && a[i] <= 159) a[i] = a[i] + 33; // RUS
		}
		for (uint16_t i = 0; i < b.length(); i++) { // B
			if (b[i] >= 65 && b[i] <= 90) b[i] = b[i] + 32; // ENG
			else if (b[i] >= 127 && b[i] <= 159) b[i] = b[i] + 33; // RUS
		}

		// Count catalogs
		int countCatalogsA = 0;
		for (uint16_t i = 0; i < a.length(); i++) {
			if (a[i] == '/') {
				countCatalogsA++;
			}
		}
		int countCatalogsB = 0;
		for (uint16_t i = 0; i < b.length(); i++) {
			if (b[i] == '/') {
				countCatalogsB++;
			}
		}

		// Check steps
		uint8_t depth = 1;
		uint8_t depthCountA = 0; uint8_t depthCountB = 0;
		bool good = false; bool veryBad = false;
		std::string stepA; uint16_t a_n = 0;
		std::string stepB; uint16_t b_n = 0;
		while (!good) {
			depthCountA = 0;
			for (uint16_t i = 0; i < a.length(); i++) { // A
				if (a[i] == '/') depthCountA++;
				if (depthCountA == depth) { stepA = a.substr(0, i); break; }
				if (i == a.length() - 1 && depthCountA == 0) { veryBad = true; }
			}
			depthCountB = 0;
			for (uint16_t i = 0; i < b.length(); i++) { // B
				if (b[i] == '/') depthCountB++;
				if (depthCountB == depth) { stepB = b.substr(0, i); break; }
				if (i == b.length() - 1 && depthCountB == 0) { veryBad = true; }
			}

			if (veryBad) {
				break;
			}

			if (stepA != stepB) {
				stepA = a.substr(a_n, a.size() - a_n);
				stepB = b.substr(b_n, b.size() - b_n);
				return stepA < stepB;
			}

			a_n = stepA.size();
			b_n = stepB.size();

			depth++;
			if (depth > countCatalogsA || depth > countCatalogsB) good = true;
		}

		if (countCatalogsA > countCatalogsB) return true;
		if (countCatalogsA < countCatalogsB) return false;
		
		return a < b;	
	}
};

class FDP_Packer {
	// Vars
	std::string error;
	std::vector <FDP_UnitP> resources;
	std::vector <std::string> files;
	// Functions
	public:
	FDP_Packer() { }
	bool addResource(std::string realFile, std::string fdpFile) {
		// LocaleFDP has a forbidden character
		if (fdpFile.find_first_of(FDP_PACKER_FORBIDDENCHARACTERS) != std::wstring::npos) {
			error = "LocaleFDP has a forbidden character.";
			return false;
		}
		// File not found
		if (!FileExists(realFile.c_str())) {
			error = "File <"; error += realFile.c_str(); error += "> not found.";
			return false; 
		}
		// File exists
		for (int i = 0; i < resources.size(); i++) {
			if (resources[i].fdpFile == fdpFile) {
				error = "Such file already exists.";
				return false;
			}
		}

		// Add
		resources.resize(resources.size() + 1);
		resources[resources.size() - 1].realFile = realFile;
		resources[resources.size() - 1].fdpFile = fdpFile;

		// Size
		FILE* in;
		in = fopen(realFile.c_str(), "rb");
		fseek(in, 0, SEEK_END);
		resources[resources.size() - 1].size = ftell(in);
		fclose(in);

		return true;
	}
	bool deleteResource(std::string fdpFile) {
		for (int i = 0; i < resources.size(); i++) {
			if (resources[i].fdpFile == fdpFile) {
				resources.erase(resources.begin() + i);
				std::vector<FDP_UnitP>(resources).swap(resources);
				return true;
			}
		}
		error = "File not found.";
		return false;
	}
	void sort() {
		std::sort(resources.begin(), resources.end(), FDP_UnitP_SortStruct());
	}
	bool build(std::string nameFile, std::string about = "") {
		if(about.find_first_of(FDP_PACKER_FORBIDDENCHARACTERS) != std::wstring::npos) {
			error = "LocaleFDP has a forbidden character.";
			return false;
		}
		if (!resources.empty()) {
			sort();

			FILE* in, * out;

			// Write head
			out = fopen(nameFile.c_str(), "wb");
			fputs("FDPC", out);
			fputs(about.c_str(), out);
			fputs("|", out);

			// Time
			time_t rawtime; struct tm* timeinfo; std::time(&rawtime); timeinfo = localtime(&rawtime);
			fputs(std::to_string((long long)timeinfo->tm_year + 1900).c_str(), out); // year
			if (timeinfo->tm_mday < 10) fputs("0", out); // day
			fputs(std::to_string((long long)timeinfo->tm_mday).c_str(), out);
			if(timeinfo->tm_mon + 1 < 10) fputs("0", out); // month 0 - 11
			fputs(std::to_string((long long)timeinfo->tm_mon + 1).c_str(), out);
			if (timeinfo->tm_hour < 10) fputs("0", out); // hour
			fputs(std::to_string((long long)timeinfo->tm_hour).c_str(), out);
			if (timeinfo->tm_min < 10) fputs("0", out); // min
			fputs(std::to_string((long long)timeinfo->tm_min).c_str(), out);
			if (timeinfo->tm_sec < 10) fputs("0", out); // sec
			fputs(std::to_string((long long)timeinfo->tm_sec).c_str(), out);

			// Tree
			uint64_t postep = 0; bool nonStart = false;
			uint16_t posStart = 0; std::string localNow = "";
			for (uint16_t i = 0; i < resources.size(); i++) {
				#ifdef FDP_PACKER_DEVMODE
				std::cout << "Target: \t" + resources[i].fdpFile << std::endl; //////////////////////////////////////////
				#endif
				bool wasDeleted = false;
				int backTime = 0;
				while(localNow.size() > 0) {
					nonStart = true;
					if (localNow[localNow.size() - 1] == '/') {
						wasDeleted = true;
						localNow.erase(localNow.end() - 1);
						if (backTime >= 1) {
							fputs("<", out);
						}
						backTime++;
						if (resources[i].fdpFile.find(localNow) != std::string::npos) {
							posStart = localNow.size();
							break;
						}
					}
					else {
						localNow.erase(localNow.end() - 1);
					}

				}

				if (localNow.size() == 0) {
					if (nonStart && wasDeleted) fputs("<", out);
					posStart = 0;
				}

				for (uint16_t j = posStart; j < resources[i].fdpFile.size(); j++) {
					if (resources[i].fdpFile[j] == '/') { 
						if (j - posStart > 0) {
							localNow += resources[i].fdpFile.substr(posStart, j - posStart);
							#ifdef FDP_PACKER_DEVMODE
							std::cout << "\t\t" << posStart << " " << j - posStart << " " << localNow << std::endl; /////////////////////////////////////////
							#endif	
							std::string buf = resources[i].fdpFile.substr(posStart, j - posStart);
							for (uint16_t i = 0; i < buf.size(); i++) {
								if (buf[i] == '/') buf.erase(i, 1);
							}
							buf.insert(0, ">");
							fputs(buf.c_str(), out);

							posStart = j;
						}
					}
					else if (j == resources[i].fdpFile.size() - 1) {
						localNow += resources[i].fdpFile.substr(posStart, j - posStart + 1);

						std::string buf = resources[i].fdpFile.substr(posStart, j - posStart + 1);
						for (uint16_t i = 0; i < buf.size(); i++) {
							if (buf[i] == '/') buf.erase(i, 1);
						}
						buf.insert(0, ":");
						buf += "?" + std::to_string((long long)postep); buf += "*" + std::to_string((long long)resources[i].size);
						postep += resources[i].size;
						fputs(buf.c_str(), out);
						#ifdef FDP_PACKER_DEVMODE
						std::cout << "\t\t" << posStart << " " << j - posStart + 1 << " " << localNow << std::endl; //////////////////////////////////////
						#endif	
					}
				}
				#ifdef FDP_PACKER_DEVMODE
				std::cout << "Answer: \t" + localNow << std::endl; ///////////////////////////////////////////
				#endif
			}
			                
			fputs("|", out);

			for (uint16_t i = 0; i < resources.size(); i++) {
				in = fopen(resources[i].realFile.c_str(), "rb");
				char copy[1];
				uint16_t nread;
				while (nread = fread(copy, sizeof(char), sizeof(copy), in)) {
					fwrite(copy, sizeof(char), nread, out);
				}
				fclose(in);
			}
			fclose(out);
		}
		else {
			error = "FDP buffer is empty.";
			return false;
		}
	}
	std::string getError() { return error; }
	// Test
	void showTree() {
		std::cout << "Tree of this file: " << std::endl;
		for (int i = 0; i < resources.size(); i++) {
			std::cout << "\t" + resources[i].fdpFile << std::endl;
		}
	}
};