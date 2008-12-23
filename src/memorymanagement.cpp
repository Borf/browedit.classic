#ifdef _MEMORYMAGAGEMENT
#pragma warning( disable : 4786 )
#include <windows.h>
#include <map>
#include <stdio.h>
#include "StackWalker.h"

extern MyStackWalker* stackWalker;

typedef struct {
	DWORD	address;
	DWORD	size;
	char	file[64];
	DWORD	line;
	std::string stack;
} ALLOC_INFO;

typedef std::map<DWORD, ALLOC_INFO*, std::less<DWORD> > allocMapType;

allocMapType *allocMap;

void AddTrack(DWORD addr,  DWORD asize,  const char *fname, DWORD lnum)
{
	ALLOC_INFO *info;
	
	if(!allocMap) {
		      allocMap = new(allocMapType);
	}
	
	info = new(ALLOC_INFO);
	info->address = addr;
	strncpy(info->file, fname, 63);
	info->line = lnum;
	info->size = asize;
	if(stackWalker)
		info->stack = stackWalker->getStack();
	(*allocMap)[addr] = info;
}

void RemoveTrack(DWORD addr)
{
	allocMapType::iterator i;
	
	if(!allocMap)
		      return;
/*	for(i = allocList->begin(); i != allocList->end(); i++)
	{
		      if((*i)->address == addr)
			  {
				  allocList->remove((*i));
				  break;
			  }
	}*/

	i = allocMap->find(addr);
	if(i == allocMap->end())
	{
		printf("Error!, trying to delete memory that has not been allocated yet!");
		printf("%s", stackWalker->getStack().c_str());
	}
	else
		allocMap->erase(i);

}


void DumpUnfreed()
{
	allocMapType::iterator i;
	DWORD totalSize = 0;
	char buf[1024];
	
	if(!allocMap)
		      return;
	
	for(i = allocMap->begin(); i != allocMap->end(); i++) 
	{
		if(i->second->stack.find("tinyxml.cpp") != std::string::npos)
			continue;
	  sprintf(buf, "\n\n%-50s:\t\tLINE %d,\t\tADDRESS %d\t%d unfreed\n",
		  i->second->file, i->second->line, i->second->address, i->second->size);
	  OutputDebugString(buf);
	  OutputDebugString(i->second->stack.c_str());
	  totalSize += i->second->size;
	}
	sprintf(buf, "-----------------------------------------------------------\n");
	OutputDebugString(buf);
	sprintf(buf, "Total Unfreed: %d bytes\n", totalSize);
	OutputDebugString(buf);
	if(allocMap->size() == 0)
		OutputDebugString("Woot, no memory leaks!");
}

#endif
