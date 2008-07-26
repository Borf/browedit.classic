#ifndef __BMUTEX_H__
#define __BMUTEX_H__

#include <common.h>


class cBMutex
{
	
private:
#ifdef WIN32
	CRITICAL_SECTION cs;   // Windows' basic mutex object.
#endif
	
public:
	cBMutex();
	~cBMutex();

	void lock();
	void unlock() ;
	
};

#endif
