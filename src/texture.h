#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "common.h"
#include "filesystem.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


#include <string>
using namespace std;

class cTextureLoader;

class cTexture
{
protected:
	string				filename;
	GLuint				tid;

	bool				clamp;
public:
	bool				loaded;
	cTexture(string,bool,bool);
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
	string				getfilename() { return filename; }
	virtual GLuint		texid();
	void				unLoad();


};

class cTextureFromMemory : public cTexture
{
public:
	char*	mem_data;
	int		mem_length;
	string extension;

	cTextureFromMemory(string, char*, int);
	GLuint texid();
};


class cTextureLoaders
{
public:
	vector<cTextureLoader*>	loaders;
	static cTexture*				load(string, bool, bool = true);
	static void						load(string, cTexture*, bool);
	static void						loadfrommem(string, char*, int, cTexture*, bool);
};

cTextureLoaders& GetTextureLoaders();


#include "textureloaders/textureloader.h"

#endif
