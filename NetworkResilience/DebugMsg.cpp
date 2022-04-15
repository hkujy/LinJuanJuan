#include <iostream>
#include <cstdlib>
#include <cstdio>  
#include <cstdarg> /* va_list, va_start, va_arg, va_end */
#include <ctime>
#include <fstream>
using namespace std;
void LogMsg(const char *format, ...) {

	FILE *logFile;
	errno_t err = fopen_s(&logFile, "..//OutPut//Log.txt", "a+");

	//if (LogFile == NULL)
	if (logFile == nullptr)
	{
		cout << "LogFile is Not Opened" << endl;
		system("PAUSE");
	}
	else
	{
		va_list ap;
		va_start(ap, format);
		vfprintf(logFile, format, ap);	
		va_end(ap);
		fclose(logFile);
	}

	//Log = NULL;
}


void getnowtime(ofstream filename)
{
	struct tm newTime;
	char am_pm[] = "AM";
	__time64_t long_time;
	char timebuf[26];
	errno_t err;
	// Get time as 64-bit integer.
	_time64(&long_time);
	// Convert to local time.
	err = _localtime64_s(&newTime, &long_time);
	if (err)
	{
		printf("Invalid argument to _localtime64_s.");
		exit(1);
	}
	err = asctime_s(timebuf, 26, &newTime);
	if (err)
	{
		printf("Invalid argument to asctime_s.");
		exit(1);
	}
	filename << timebuf << endl;
}

