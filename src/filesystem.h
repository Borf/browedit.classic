#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include <string>
#include <vector>
#include <map>
#include "grflib/grf.h"
#include "tinyxml/tinyxml.h"

#ifndef __MINGW32__
#pragma warning(disable : 4786)
#endif
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
	void			skip(int);

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
	static int						loadPackedFile(std::string);
	static cFile*					open(std::string);
	static TiXmlDocument			getXml(std::string);
	static bool						isFile(std::string);


	static std::vector<cGRFFile*>	locations;
};

#endif
