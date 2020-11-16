#pragma once

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255
#define SERIAL_SPEED 28800

#define N_PORTS 20
#define KEY_PORT 'd'

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>

class Com
{
private:
    HANDLE handler;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
	Com(uint8_t port)
	{
		std::string buf = "COM";
		buf += std::to_string((long long)port);

		this->connected = false;

		this->handler = CreateFileA(static_cast<LPCSTR>((char*)buf.c_str()),
			GENERIC_READ | GENERIC_WRITE,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (this->handler == INVALID_HANDLE_VALUE) {
			printf("COM: bad try");
		}
		else {
			DCB dcbSerialParameters = { 0 };

			if (!GetCommState(this->handler, &dcbSerialParameters)) {
				printf("COM: bad try");
			}
			else {
				dcbSerialParameters.BaudRate = SERIAL_SPEED;
				dcbSerialParameters.ByteSize = 8;
				dcbSerialParameters.StopBits = ONESTOPBIT;
				dcbSerialParameters.Parity = NOPARITY;
				dcbSerialParameters.fDtrControl = DTR_CONTROL_ENABLE;

				if (!SetCommState(handler, &dcbSerialParameters))
				{
					printf("COM: bad try");
				}
				else {
					this->connected = true;
					PurgeComm(this->handler, PURGE_RXCLEAR | PURGE_TXCLEAR);
					Sleep(ARDUINO_WAIT_TIME);
				}
			}
		}
	}
	~Com()
	{
		if (this->connected) {
			this->connected = false;
			CloseHandle(this->handler);
		}
	}

    int readSerialPort(char* buffer, unsigned int buf_size)
	{
		DWORD bytesRead;
		unsigned int toRead = 0;

		ClearCommError(this->handler, &this->errors, &this->status);

		if (this->status.cbInQue > 0) {
			if (this->status.cbInQue > buf_size) {
				toRead = buf_size;
			}
			else toRead = this->status.cbInQue;
		}

		if (ReadFile(this->handler, buffer, toRead, &bytesRead, NULL)) return bytesRead;

		PurgeComm(handler, PURGE_RXCLEAR);

		return 0;
	}
    bool writeSerialPort(char* buffer, unsigned int buf_size)
	{
		DWORD bytesSend;

		if (!WriteFile(this->handler, (void*)buffer, buf_size, &bytesSend, 0)) {
			ClearCommError(this->handler, &this->errors, &this->status);
			return false;
		}
		else return true;
	}
    bool isConnected()
	{
		return this->connected;
	}
	void close() {
		CloseHandle(handler);
		this->connected = false;
	}
};

