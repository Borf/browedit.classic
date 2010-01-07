#include <bengine/forwards.h>
#include <bengine/log.h>
#include <bengine/util.h>
#include <common.h>
#include "grffilesystem.h"
#include "settings.h"

cGrfFileSystem::cGrfFileLoader::cGrfFileLoader( std::string grfFile )
{
	this->grfFile = grfFile;

	GrfError error;
	
	grf = grf_open(grfFile.c_str(), "rb", &error);
	if (grf == NULL)
	{
		bEngine::cLog::add(GetMsg("fs/GRFOPENERROR"), grfFile.c_str());
		throw "Argh";
	}

}

bool cGrfFileSystem::cGrfFileLoader::isFile(std::string fileName)
{
	fileName = bEngine::util::replace(fileName, "/", "\\");
	fileName = bEngine::util::replace(fileName, cSettings::roDir, "");
	for(unsigned int i = 0; i < grf->nfiles; i++)
	{
		if(grf->files[i].name == fileName)
			return true;
	}
	return false;
}

bEngine::util::cFileSystem::cReadFile* cGrfFileSystem::cGrfFileLoader::open(std::string fileName)
{
	fileName = bEngine::util::replace(fileName, "/", "\\");
	fileName = bEngine::util::replace(fileName, cSettings::roDir, "");
	for(unsigned int i = 0; i < grf->nfiles; i++)
	{
		if(grf->files[i].name == fileName)
		{
			return new cGrfFile(grf->files[i].name, grf);
		}
	}
	return NULL;
}

void cGrfFileSystem::cGrfFileLoader::getFileList( std::vector<std::string> &files, std::string directory )
{
	directory = bEngine::util::replace(directory, "/", "\\");
	directory = bEngine::util::lcase(bEngine::util::replace(directory, cSettings::roDir, ""));
	for(unsigned int i = 0; i < grf->nfiles; i++)
	{
		if(bEngine::util::lcase(grf->files[i].name).substr(0, directory.length()) == directory)
		{
			files.push_back(cSettings::roDir + grf->files[i].name);
		}
	}
	
}


cGrfFileSystem::cGrfFile::cGrfFile( std::string fileName, Grf* grf ) : bEngine::util::cFileSystem::cReadFileMemory()
{
	GrfError error;
	unsigned int size2 = 0;
	char* dat = (char*)grf_get(grf, fileName.c_str(), &size2, &error);
	length = size2;
	if(!dat)
	{
		bEngine::cLog::throwError("Trying to open a file in a grf file that does not exist");
		throw "Trying to open a file in a grf file that does not exist";
	}
	data = new char[length+1];
	memcpy(data, dat, length);

}
