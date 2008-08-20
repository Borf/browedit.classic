#include "common.h"
#ifdef WIN32
    #include <windows.h>
#endif
#include <fstream>
#include "filesystem.h"
#include <zlib.h>

extern cFileSystem fs;

extern std::string rodir;

int cFileSystem::loadPackedFile(std::string grffilename)
{
	cGRFFile* grffile = new cGRFFile();
	locations.push_back(grffile);
	char thislocation = locations.size()-1;
	GrfError error;

	grffile->grf = grf_open(grffilename.c_str(), "rb", &error);
	if (grffile->grf == NULL)
	{
		Log(2,0,GetMsg("fs/GRFOPENERROR"), grffilename.c_str());
		return -1;
	}
	
	for(unsigned int i =0 ; i < grffile->grf->nfiles; i++)
	{
		std::string filename = replace(lcase(rodir + grffile->grf->files[i].name),"/","\\");
		grffile->files[filename] = new cFile();
		grffile->files[filename]->fileName = grffile->grf->files[i].name;
		grffile->files[filename]->location = thislocation;
	}


	Log(3,0,GetMsg("fs/GRFOPENDONE"), grffilename.c_str());

	return 0;
}




cFile* cFileSystem::open(std::string filename)
{
	#ifndef WIN32
	filename = replace(filename, "\\", "/");
	#endif

	FILE* f = fopen(filename.c_str(), "rb");
	if (f != NULL)
	{
		cFile* fil = new cFile;
		fil->location = -1;
		fil->fileName = filename;

		fseek(f, 0, SEEK_END);
		fil->size = ftell(f);
		fil->index = 0;
		fclose(f);
		fil->open();
		return fil;
	}

	filename = replace(filename,"/","\\");
	for(unsigned int i = 0; i < locations.size(); i++)
	{
		if(locations[i]->files.find(lcase(filename)) != locations[i]->files.end())
		{
			locations[i]->files[lcase(filename)]->open();
			return locations[i]->files[lcase(filename)];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////
int cFile::open()
{
	std::ifstream pFile;
	if (location == -1)
	{
		pFile.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
		if(pFile.bad())
		{
			Log(1,0,GetMsg("fs/FILEERROR"), fileName.c_str());
			#ifdef WIN32
//			MessageBox(NULL, ("Error: could not open file: " + fileName).c_str(), "File not found", MB_OK);
			#endif
		}
		index = 0;
		data = new char[size+1];
		if(!data)
			return 0;
		for(int i = 0; i < size; i+=1024)
		{
			char buf[1024];
			pFile.read(buf, 1024);
			memcpy(data+i, buf, min((long)1024,size-i));
		}
	}
	else
	{
		cGRFFile* grffile = fs.locations[location];
		GrfError error;
		unsigned int size2 = 0;
		char* dat = (char*)grf_get(grffile->grf, fileName.c_str(), &size2, &error);
		size = size2;
		if(!dat)
		{
			return 0;
		}
		data = new char[size+1];
		memcpy(data, dat, size);
	}


	index = 0;
	return 0;
}


int cFile::close()
{
	delete [] data;
	if (location == -1)
		delete this;
	return 0;
}

bool cFile::eof()
{
	return (index >= size);
}


char cFile::get()
{
	index++;
	return data[index-1];
}

int cFile::getword()
{
	index+=2;
	return ((BYTE)data[index-2]) | (((BYTE)data[index-1])<<8);
}


int cFile::read(char* buffer, int length)
{
  	int i = min((long)length, size+1-index);
	memcpy(buffer, data+index, i);
	if (length > size-index) 
		buffer[size-index] = 0;
	index+=i;
	return i;
}

void cFile::seek(int o, enum STARTPOS p)
{
	switch (p)
	{
	case beg:
		index = o;
		break;
	case cur:
		index+=o;
		break;
	case end:
		index=size-o;
		break;
	}
}

int cFile::readLine(char* buf, int maxlen)
{
	int i = 0;
	while(data[index] != '\r' && data[index] != '\n' && !eof() && i < maxlen)
	{
		buf[i] = data[index];
		index++;
		i++;
	}
	buf[i] = 0;
	if (data[index] == '\r')
	{
		index++;
		if (data[index] == '\n')
			index++;
	}
	else if (data[index] == '\n')
	{
		index++;
		if (data[index] == '\r')
			index++;
	}
	return i;
}
std::string cFile::readLine()
{
	if(eof())
		return "";
	std::string s = "";
	while(data[index] != '\r' && data[index] != '\n' && !eof())
	{
		s+=data[index];
		index++;
		if(eof())
			return s;
	}
	if (data[index] == '\r')
	{
		index++;
		if (data[index] == '\n')
			index++;
	}
	else if (data[index] == '\n')
	{
		index++;
		if(eof())
			return s;
		if (data[index] == '\r')
			index++;
	}
	return s;
}

cFile::cFile()
{
	offset = -1;
	size = -1;
	crc = 0;
	location = -1;
	version = -1;
}


TiXmlDocument cFileSystem::getXml(std::string filename)
{
	TiXmlDocument ret;
	ret.SetCondenseWhiteSpace(false);
	cFile* pFile = open(filename);
	if(pFile == NULL)
	{
		Log(1,0,"Couldn't open %s", filename.c_str());
		return ret;
	}
	std::string xmldata;
	while(!pFile->eof())
		xmldata += pFile->readLine() + "\n";
	pFile->close();

	ret.Parse( xmldata.c_str() );
	if ( ret.Error() )
	{
		Log(1,0,"could not load '%s': error in %s: %s at line %i, col %i\n", filename.c_str(), ret.Value(), ret.ErrorDesc() , ret.ErrorRow(), ret.ErrorCol());
		return ret;
	}
	return ret;
}



bool cFileSystem::isFile(std::string filename)
{
	#ifndef WIN32
	filename = replace(filename, "\\", "/");
	#endif

	FILE* f = fopen(filename.c_str(), "rb");
	if (f != NULL)
	{
		fclose(f);
		return true;
	}

	filename = replace(filename,"/","\\");
	for(unsigned int i = 0; i < locations.size(); i++)
		if(locations[i]->files.find(lcase(filename)) != locations[i]->files.end())
			return true;
	return false;
}



cFileSystem::~cFileSystem()
{
	printf("Cleaning grf files\n");
	for(unsigned int i = 0; i < locations.size(); i++)
		delete locations[i];
	locations.clear();
}

cGRFFile::~cGRFFile()
{
	for(std::map<std::string, cFile*, std::less<std::string> >::iterator i = files.begin(); i != files.end(); i++)
		delete i->second;
	files.clear();

	if(grf)
	{
		grf_close(grf);
//		grf_free(grf);
	}
	grf = NULL;
}

