#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "common.h"


#include <string>
#include <vector>
#include <map>
#include "grflib/grf.h"
#include "tinyxml/tinyxml.h"

//using namespace std;

enum STARTPOS
{
	beg, cur, end
};

class cFile
{
public:
	cFile();

	std::string		fileName;

	long			size;
	long			offset;
	int				crc;
	int				version;

	char*			data;

	int				open();
	int				close();
	bool			eof();
	void			seek(int, STARTPOS);

	char			get();
	int				getword();
	int				read(char*, int);
	int				readLine(char*, int);
	std::string		readLine();
	char			location;
	long			index; // offset in this file

};

class cGRFFile
{
public:
	~cGRFFile();
	Grf*													grf;
	std::map<std::string, cFile*, std::less<std::string> >	files;
	long													nroffiles;
};

class cFileSystem
{
private:
public:
	~cFileSystem();
	int								loadPackedFile(std::string);
	cFile*							open(std::string);
	std::vector<cGRFFile*>			locations;
	TiXmlDocument					getXml(std::string);
	bool							isFile(std::string);

};

#endif
