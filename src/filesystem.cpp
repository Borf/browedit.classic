#include "common.h"
#ifdef WIN32
    #include <windows.h>
#endif
#include <fstream>
#include "filesystem.h"
#include <zlib.h>

extern cFileSystem fs;

extern string rodir;

int cFileSystem::LoadFile(string grffilename)
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
	
	for(int i =0 ; i < grffile->grf->nfiles; i++)
	{
		string filename = replace(lcase(rodir + grffile->grf->files[i].name),"/","\\");
		grffile->files[filename] = new cFile();
		grffile->files[filename]->filename = grffile->grf->files[i].name;
		grffile->files[filename]->location = thislocation;
	}


	Log(3,0,GetMsg("fs/GRFOPENDONE"), grffilename.c_str());

	return 0;
}




cFile* cFileSystem::open(string filename)
{
	#ifndef WIN32
	filename = replace(filename, "\\", "/");
	#endif

	FILE* f = fopen(filename.c_str(), "rb");
	if (f != NULL)
	{
		cFile* fil = new cFile;
		fil->location = -1;
		fil->filename = filename;

		fseek(f, 0, SEEK_END);
		fil->size = ftell(f);
		fclose(f);
		fil->open();
		return fil;
	}

	filename = replace(filename,"/","\\");
	for(int i = 0; i < locations.size(); i++)
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
	ifstream pFile;
	if (location == -1)
	{
		pFile.open(filename.c_str(), ios_base::in | ios_base::binary);
		if(pFile.bad())
		{
			Log(1,0,GetMsg("fs/FILEERROR"), filename.c_str());
			#ifdef WIN32
//			MessageBox(NULL, ("Error: could not open file: " + filename).c_str(), "File not found", MB_OK);
			#endif
		}
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
		char* dat = (char*)grf_get(grffile->grf, filename.c_str(), (unsigned int*)&size, &error);
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

int cFile::readline(char* buf, int maxlen)
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
string cFile::readline()
{
	string s;
	while(data[index] != '\r' && data[index] != '\n' && !eof())
	{
		s+=data[index];
		index++;
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
		if (data[index] == '\r')
			index++;
	}
	return s;
}


TiXmlDocument cFileSystem::getxml(string filename)
{
	TiXmlDocument ret;
	ret.SetCondenseWhiteSpace(false);
	cFile* pFile = open(filename);
	if(pFile == NULL)
	{
		Log(1,0,"Couldn't open %s", filename.c_str());
		return ret;
	}
	string xmldata;
	while(!pFile->eof())
		xmldata += pFile->readline() + "\n";
	pFile->close();

	ret.Parse( xmldata.c_str() );
	if ( ret.Error() )
	{
		Log(1,0,"could not load '%s': error in %s: %s at line %i, col %i\n", filename.c_str(), ret.Value(), ret.ErrorDesc() , ret.ErrorRow(), ret.ErrorCol());
		return ret;
	}
	return ret;
}