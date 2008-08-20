#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "common.h"
#include "filesystem.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


#include <string>
//using namespace std;

class cTextureLoader;

class cTexture
{
protected:
	std::string			filename;
	GLuint				tid;

	bool				clamp;
public:
	bool				loaded;
	cTexture(std::string,bool,bool);
	void				resizeToLog();

	bool				freedata;
	int					width;
	int					height;

	int					widthOriginal;
	int					heightOriginal;

	int					bpp;
	BYTE*				data;
	int					datatype;

	void				generate();	
	std::string			getfilename() { return filename; }
	virtual GLuint		texId();
	void				unLoad();


};

class cTextureFromMemory : public cTexture
{
public:
	char*			mem_data;
	int				mem_length;
	std::string		extension;

	cTextureFromMemory(std::string, char*, int);
	GLuint texId();
};


class cTextureLoaders
{
public:
	std::vector<cTextureLoader*>	loaders;
	static cTexture*				load(std::string, bool, bool = true);
	static void						load(std::string, cTexture*, bool);
	static void						loadfrommem(std::string, char*, int, cTexture*, bool);
};

cTextureLoaders& GetTextureLoaders();


#include "textureloaders/textureloader.h"

#endif
