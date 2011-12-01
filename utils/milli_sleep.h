// A small wrapper to handle timer issues between windows and linux
#ifdef WIN32
#include <windows.h>
#endif

void milli_sleep(int milli_sec)
{
#ifdef _WIN32
	Sleep(milli_sec);
#else
	usleep(1000*milli_sec);
#endif
}



