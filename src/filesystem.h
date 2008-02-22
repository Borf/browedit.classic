#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "common.h"


#include <string>
#include <vector>
#include <map>
#include "grflib/grf.h"
#include "tinyxml/tinyxml.h"

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
	int		getword();
	int		read(char*, int);
	int		readline(char*, int);
	string	readline();
	char	location;
	long	index; // offset in this file

};

class cGRFFile
{
public:
	~cGRFFile();
	Grf* grf;
	map<string, cFile*, less<string> > files;
	long	nroffiles;
};

class cFileSystem
{
private:
public:
	~cFileSystem();
	int LoadFile(string);
	cFile*	open(string);
	vector<cGRFFile*>	locations;
	TiXmlDocument getxml(string);
	bool	isfile(string);

};

#endif
