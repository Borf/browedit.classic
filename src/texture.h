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
private:
	bool				loaded;
	string				filename;
	GLuint				tid;
public:
	cTexture(string,bool);
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
	GLuint				texid();
	void				unLoad();


};


class cTextureLoaders
{
public:
	static vector<cTextureLoader*>	loaders;
	static cTexture*				load(string, bool = true);
	static void						load(string, cTexture*);
};

cTextureLoaders& GetTextureLoaders();


#include "textureloaders/textureloader.h"

#endif
