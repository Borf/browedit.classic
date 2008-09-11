#ifndef __BTHREAD_H__
#define __BTHREAD_H__

#include <common.h>
#ifdef WIN32
#include <windows.h>
#endif

#include "bmutex.h"

class cBThread
{
private:
#ifdef WIN32
	DWORD dwThreadID;
	HANDLE hSignal;
	HANDLE hHandle;
	static DWORD WINAPI fakeworker(void*);
#else
#endif

	virtual void		worker() = 0;
	int					lastsignal;
	cBMutex*			signalMutex;
public:
						cBThread();
						~cBThread();
	void				start();
	void				stop();
	void				wait();

	void				signal(int=0);
	void				waitForSignal();
	int					getSignal();
};



class cDownloadThread : public cBThread
{
public:
	class cDownloadThreadFinisher
	{
	public:
		char* data;
		int length;

		
		cDownloadThreadFinisher();
		~cDownloadThreadFinisher();
		virtual void whenDone(cBThread*) = 0;
	};
	cDownloadThread(std::string, cDownloadThreadFinisher*);
	cDownloadThread(std::string, std::string, cDownloadThreadFinisher*);
private:
	std::string					url;
	std::string					post;
	cDownloadThreadFinisher*	finisher;
	void						worker();

};


#endif
