#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <vector>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


#include <string>
//using namespace std;


enum eTextureOptions
{
	TEX_NORMAL			= 0,
	TEX_NOCLAMP			= 1,
	TEX_NEARESTFILTER	= 2,
	TEX_NOFREEDATA		= 4,
};

class cTextureLoader;

class cTexture
{
protected:
	std::string			filename;
	GLuint				tid;
	eTextureOptions		options;

public:
	bool				loaded;
						cTexture(std::string,eTextureOptions);
	virtual 			~cTexture() {};
	void				resizeToLog();

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
	~cTextureFromMemory() {};
	GLuint texId();
};


class cTextureLoaders
{
public:
	std::vector<cTextureLoader*>	loaders;
	static cTexture*				load(std::string, eTextureOptions);
	static void						load(std::string, cTexture*, eTextureOptions);
	static void						loadfrommem(std::string, char*, int, cTexture*, eTextureOptions);
};

cTextureLoaders& GetTextureLoaders();


#include "textureloaders/textureloader.h"

#endif
