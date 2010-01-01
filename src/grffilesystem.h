#ifndef __GRFFILESYSTEM_H__
#define __GRFFILESYSTEM_H__

#include <bengine/forwards.h>
#include <bengine/util/filesystem.h>

#include <grflib/grf.h>


class cGrfFileSystem : public bEngine::util::cFileSystem
{
public:
	class cGrfFileLoader : public bEngine::util::cFileSystem::cFileLoader
	{
	protected:
		std::string grfFile;
		Grf*		grf;
	public:
		cGrfFileLoader(std::string grfFile);
		virtual bool										isFile(std::string fileName);
		virtual bEngine::util::cFileSystem::cReadFile*		open(std::string fileName);
	};


	class cGrfFile : public bEngine::util::cFileSystem::cReadFileMemory
	{
	public:
		cGrfFile(std::string fileName, Grf* grf);

	};

};




#endif
