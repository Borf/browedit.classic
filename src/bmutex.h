#ifndef __BMUTEX_H__
#define __BMUTEX_H__

#include <common.h>
class cBMutex
{
	
private:
	CRITICAL_SECTION cs;   // Windows' basic mutex object.
	
public:
	void lock()
	{
		EnterCriticalSection(&cs);
	}
	
	void unlock() 
	{
		LeaveCriticalSection(&cs);
	}
	
	cBMutex() 
	{
		InitializeCriticalSection(&cs);
	}
	
	~cBMutex() 
	{
		DeleteCriticalSection(&cs);
	}
	
};
#endif
