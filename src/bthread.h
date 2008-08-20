#ifndef __BTHREAD_H__
#define __BTHREAD_H__

#include <common.h>
#ifdef WIN32
#include <windows.h>
#endif


class cBThread
{
private:
#ifdef WIN32
	DWORD threadid;
	HANDLE handle;
	static DWORD WINAPI fakeworker(void*);
#else
#endif

	virtual void worker() = 0;
public:
	void start();
	void stop();
	void wait();
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
