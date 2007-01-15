#include "common.h"
#ifdef WIN32
    #include <windows.h>
#endif
#include <fstream>
#include "filesystem.h"
#include <zlib.h>

extern cFileSystem fs;

int cFileSystem::LoadFile(string filename)
{
	cFSFile fsfile;
	fsfile.set(filename);
	locations.push_back(fsfile);
	int thislocation = locations.size()-1;

	ifstream pFile;
	pFile.open(filename.c_str(), ios_base::in | ios_base::binary);
	if (!pFile.good() || pFile.eof() || pFile.bad())
	{
		pFile.close();
		//Log(2,0,"FileSystem: Error opening datafile");
		return -1;
	}
	Log(3,0,"FileSystem: datafile opened");
	char header[16];
	pFile.read(header, 16);
	if (header[0] != 'D' && 
		header[1] != 'A' &&
		header[2] != 'T' &&
		header[3] != 'A')
	{
		pFile.close();
		Log(3,0,"FileSystem: Error opening datafile: This is no datafile");
		return -1;
	}
	Log(3,0,"FileSystem: Identified as datafile");

	locations[thislocation].majorversion = header[4];
	locations[thislocation].minorversion = header[5];

	memcpy(&nroffiles, header+6,4);


	files = new cFile[nroffiles];

	long pos;
	memcpy(&pos, header+10,4);
	pFile.seekg(pos, ios_base::beg);
	
	for(int i = 0; i < nroffiles; i++)
	{
		if (locations[thislocation].majorversion == 1)
		{
			char buf[5];
			pFile.read(buf, 4);

			memcpy(&files[i].offset, buf,4);
			files[i].location = thislocation;
			char a = 1;
			int abc = 0;
			while(a != 0)
			{
				a = pFile.get();
				if (a != 0)
					files[i].filename += a;
				abc++;
			}
			Log(3,0,"FileSystem: found %s", files[i].filename.c_str());
		}
		else if (locations[thislocation].majorversion == 2)
		{
			char buf[6];
			pFile.read(buf, 6);
			
			memcpy(&files[i].offset, buf,4);
			files[i].location = thislocation;
			files[i].version = ((unsigned char)buf[4]) | (((unsigned char)buf[5])<<8);
			char a = 10;
			while(a != 0)
			{
				a = pFile.get();
				a^=(((long)pFile.tellg()-1)%256);
				if (a != 0)
					files[i].filename += a;
			}
			Log(3,0,"FileSystem: found %s", files[i].filename.c_str());
		}
	}
	Log(3,0,"Done reading %s", filename.c_str());
	pFile.close();

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
	filename = replace(filename, "/", "\\");

	int ind = -1;
	for(int i = 0; i < this->nroffiles && ind == -1; i++)
	{
		if (lcase(files[i].filename) == lcase(filename))
			ind = i;
	}
	if (ind != -1)
	{
		files[ind].open();
		return &files[ind];
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////
int cFile::open()
{
	unsigned long uncomprsize;
	ifstream pFile;
	if (location == -1)
	{
		pFile.open(filename.c_str(), ios_base::in | ios_base::binary);
		if(pFile.bad())
		{
			Log(1,0,"Error, could not open file %s", filename.c_str());
			#ifdef WIN32
//			MessageBox(NULL, ("Error: could not open file: " + filename).c_str(), "File not found", MB_OK);
			#endif
		}
	}
	else
	{
		pFile.open(fs.locations[location].c_str(), ios_base::in | ios_base::binary);
		pFile.seekg(offset);
		if (fs.locations[location].majorversion == 1)
		{
			char buf[5];
			pFile.read(buf, 5);
			memcpy(&size, buf,4);
			Log(3,0,"FileSystem: %s: filesize %ib", filename.c_str(), size);
			crc = buf[4];
		}
		else if (fs.locations[location].majorversion == 2)
		{
			char buf[9];
			pFile.read(buf, 8);
			memcpy(&size, buf,4);
			memcpy(&uncomprsize, buf+4,4);
			Log(3,0,"FileSystem: %s: filesize %ib compressed, %ib uncompressed", filename.c_str(), size, uncomprsize);
			crc = buf[8];
		}
	}
	data = new char[size+1];
	
	for(int i = 0; i < size; i+=1024)
	{
		char buf[1024];
		pFile.read(buf, 1024);
		memcpy(data+i, buf, min((long)1024,size-i));
	}

	if (location != -1)
	{
		if (fs.locations[location].majorversion == 2)
		{
			char* comprdata;
			comprdata = data;
			data = new char[uncomprsize+100];
			//decrypt :P
			if (fs.locations[location].minorversion == 1)
			{
				for(int i = 0; i < size; i++)
					comprdata[i] ^= ((offset)%256);
			}
			
			unsigned long comprsize = size;

			if (uncompress((BYTE*)data, &uncomprsize, (BYTE*)comprdata, comprsize) < 0)
				Log(1,0,"Error decompressing %s", filename.c_str());

	//		//decrypt again :p
	//		if (fs.locations[location].minorversion == 1)
	//		{
	//			for(i = 0; i < size; i++)
	//				data[i] ^= ((offset)%256);
	//		}


			delete[] comprdata;
			size = uncomprsize;
		}
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
	return (index == size);
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
	return 0;
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
