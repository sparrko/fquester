// .fnm 
// 31.08.20

#pragma once

#include <vector>
#include <string>

namespace FQuester {

	namespace FNM {
	
		std::vector <std::wstring> names;

		bool Update() {

			if (_waccess((DocumentsLocal + L"/FQuester/names.fnm").c_str(), 0) != -1) {
				
				FILE* f;
				unsigned int lenght;

				f = _wfopen((DocumentsLocal + L"/FQuester/names.fnm").c_str(), L"r, ccs=UTF-8");

				_fseeki64(f, 0, SEEK_END); 
				lenght = _ftelli64(f); 
				_fseeki64(f, 0, SEEK_SET);


				std::wstring bufws; wchar_t bufwc;

				names.clear();

				for (unsigned int i = 0; i < lenght; i++) {
					bufwc = fgetwc(f);

					if (bufwc != 10) {
						bufws += bufwc;
					}
					else {
						names.resize(names.size() + 1);
						names[names.size() - 1] = bufws;
						bufws.clear();
					}
				}

				fclose(f);

				return true;

			}
			else {

				names.clear();

				names.push_back(L"�������");
				names.push_back(L"����");
				names.push_back(L"����");
				names.push_back(L"�����");
				names.push_back(L"˸��");
				names.push_back(L"����");
				names.push_back(L"������");
				names.push_back(L"����");
				names.push_back(L"������");
				names.push_back(L"�����");
				names.push_back(L"�������");
				names.push_back(L"����");
				names.push_back(L"�����");
				names.push_back(L"����");
				names.push_back(L"�����");
				names.push_back(L"������");
				names.push_back(L"����");
				names.push_back(L"����");
				names.push_back(L"����");
				names.push_back(L"������");

				names.push_back(L"���� � �������");
				names.push_back(L"���� � ����� ������");
				names.push_back(L"���� � �����");
				names.push_back(L"���� � ��������� ��������");
				names.push_back(L"���� � ��������");
				names.push_back(L"���� � ��������� �������");
				names.push_back(L"���� � �������� ���������");
				names.push_back(L"���� � ������ �� ����");
				names.push_back(L"������� ����");
				names.push_back(L"����, ���������� ��������");
				names.push_back(L"����� � ������� ���� ���������");
				names.push_back(L"������� � ������� ���� ���������");
				names.push_back(L"������������");

				_wmkdir((DocumentsLocal + L"/FQuester").c_str());

				FILE* f;

				f = _wfopen((DocumentsLocal + L"/FQuester/names.fnm").c_str(), L"w+, ccs=UTF-8");

				std::wstring bufws;

				for (unsigned int i = 0; i < names.size(); i++) {

					bufws = names[i];
					bufws += (wchar_t)10;

					fwrite(bufws.c_str(), 2, bufws.size(), f);

				}

				fclose(f);

				return false;

			}

		}

		void Add(std::wstring _name) {
			
			FILE* f;
			
			_wmkdir((DocumentsLocal + L"/FQuester").c_str());

			f = _wfopen((DocumentsLocal + L"/FQuester/names.fnm").c_str(), L"a, ccs=UTF-8");

			_name += (wchar_t)10;

			fwrite(_name.c_str(), 2, _name.size(), f);

			fclose(f);

		}

		void Delete(unsigned int _num) {

			if (_waccess((DocumentsLocal + L"/FQuester/names.fnm").c_str(), 0) != -1) {
				
				if (_num <= names.size() - 1) {

					Update();

					FILE* f;

					names.erase(names.begin() + _num);

					f = _wfopen((DocumentsLocal + L"/FQuester/names.fnm").c_str(), L"w+, ccs=UTF-8");

					std::wstring bufws;

					for (unsigned int i = 0; i < names.size(); i++) {

						bufws = names[i];
						bufws += (wchar_t)10;

						fwrite(bufws.c_str(), 2, bufws.size(), f);

					}

					fclose(f);

				}

			}
		}

	}
};