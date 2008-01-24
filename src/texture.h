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


#include "textureloaders/textureloader.h"

/*



class cTexture
{
	class BMPClass
	{
	public:
		BMPClass();
		~BMPClass();
		BYTE& pixel(int x,int y,int c);
		void allocateMem();
		int width,height;
		BYTE* bytes;			//OpenGL formatted pixels
	};

	#define BMPError char
	#define BMPNOTABITMAP 'b'	//Possible error flags
	#define BMPNOOPEN 'o'
	#define BMPFILEERROR 'f'
	#define BMPBADINT 'i'
	#define BMPNOERROR '\0'
	#define BMPUNKNOWNFORMAT 'u'
	BMPError BMPLoad(std::string fname,BMPClass& bmp);

public:
	cTexture()
	{
		tid = 0;
		loaded = false;
	}
	GLuint				texid();
	GLuint				masktexid;
	int					Load(string, bool freedata = true);
	int					unLoad();
	bool				load() { return loaded;}

	int					height;
	int					width;
	int					bpp;
	GLuint				type;											// Image Type (GL_RGB, GL_RGBA)
	bool				freedata;
	GLubyte	*			imageData;
	string				getfilename() { return filename; }

private:
	cTexture*			cache;
	bool				loaded;
	string				filename;

	int					LoadFromTGA(string);
	int					LoadUncompressedTGA(string, cFile*);
	int					LoadCompressedTGA(string, cFile*);

	int					LoadFromJpeg(string);
	GLuint				tid;
	

};

*/
#endif
