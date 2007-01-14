#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "common.h"


#include <string>
#include <vector>
using namespace std;

enum STARTPOS
{
	beg, cur, end
};

class cFile
{
public:
	cFile()
	{
		offset = -1;
		size = -1;
		crc = 0;
		location = -1;
		version = -1;
	};

	string filename;

	long	size;
	long	offset;
	int		crc;
	int		version;

	char*	data;

	int		open();
	int		close();
	bool	eof();
	void	seek(int, STARTPOS);

	char	get();
	int		read(char*, int);
	int		readline(char*, int);
	string	readline();
	char	location;
	long	index; // offset in this file

};

class cFSFile
{
public:
	cFSFile()
	{
		majorversion = -1;
		minorversion = -1;
	}
	void set(string s)
	{
		file = s;
	}
	bool operator==(string s)
	{
		return (file == s);
	}
	const char* c_str()
	{
		return file.c_str();
	}
	string file;
	int majorversion;
	int minorversion;
};

class cFileSystem
{
private:
	cFile*	files;
	long	nroffiles;
public:
	int LoadFile(string);
	cFile*	open(string);
	vector<cFSFile>	locations;
	long	filecount()
	{
		return nroffiles;
	}
	cFile&	getfile(int i)
	{
		return files[i];
	}
};

#endif
