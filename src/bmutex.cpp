#include "bmutex.h"

void cBMutex::lock()
{
#ifdef WIN32
	EnterCriticalSection(&cs);
#endif

}

void cBMutex::unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&cs);
#endif
}

cBMutex::cBMutex()
{
#ifdef WIN32
	InitializeCriticalSection(&cs);
#endif
}

cBMutex::~cBMutex()
{
#ifdef WIN32
	DeleteCriticalSection(&cs);
#endif
}
	
