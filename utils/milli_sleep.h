// A small wrapper to handle timer issues between windows and linux
#ifdef WIN32
#include <windows.h>
#endif

#include <unistd.h>

void milli_sleep(int milli_sec)
{
#ifdef __GNUC__
	usleep(1000*milli_sec);
#else
	Sleep(milli_sec);
#endif
}



