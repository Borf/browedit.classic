#include "bthread.h"


#ifdef WIN32
DWORD WINAPI cBThread::fakeworker(void* param)
{
	((cBThread*)param)->worker();
	return 0;
}
#endif



void cBThread::start()
{
#ifdef WIN32
	handle = CreateThread(NULL,0,this->fakeworker,this,0,&threadid);
#else

#endif

}


void cBThread::stop()
{
#ifdef WIN32
	TerminateThread(handle,0);
	CloseHandle(handle);
#else

#endif

}


void cBThread::wait()
{
#ifdef WIN32
	WaitForSingleObject(handle, INFINITE);
#else

#endif

}









//downloadthread
#include <curl/curl.h>

cDownloadThread::cDownloadThread(string u, cDownloadThread::cDownloadThreadFinisher* f) : cBThread()
{
	finisher = f;
	url = u;
	start();
}

cDownloadThread::cDownloadThread(string u, string p, cDownloadThread::cDownloadThreadFinisher* f) : cBThread()
{
	finisher = f;
	url = u;
	post = p;
	start();
}

void *myrealloc(void *ptr, size_t size)
{
/* There might be a realloc() out there that doesn't like reallocing
	NULL pointers, so we take care of it here */
	if(ptr)
		return realloc(ptr, size);
	else
		return malloc(size);
}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{

	size_t realsize = size * nmemb;
	cDownloadThread::cDownloadThreadFinisher* mem = (cDownloadThread::cDownloadThreadFinisher*)data;
	
	mem->data = (char *)myrealloc(mem->data, mem->length + realsize + 1);
	if (mem->data) {
		memcpy(&(mem->data[mem->length]), ptr, realsize);
		mem->length += realsize;
		mem->data[mem->length] = 0;
	}
	return realsize;
}

void cDownloadThread::worker()
{
	CURL *curl_handle;
	finisher->data = NULL;
	finisher->length = 0;
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	if(post != "")
	{
		curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDSIZE, post.length());
		curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, post.c_str());
	}
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)finisher);
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	curl_easy_perform(curl_handle);
	curl_easy_cleanup(curl_handle);
	Log(3,0,"Done downloading %s", url.c_str());
	finisher->whenDone(this);
	delete finisher;
	finisher = NULL;
	delete this;
}





cDownloadThread::cDownloadThreadFinisher::cDownloadThreadFinisher()
{
	data = NULL;
	length = -1;
}

cDownloadThread::cDownloadThreadFinisher::~cDownloadThreadFinisher()
{
	if(data)
		delete[] data;
	data = NULL;
}