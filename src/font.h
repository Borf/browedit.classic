#ifndef __FONT_H__
#define __FONT_H__

#ifdef WIN32
	#include <windows.h>
#endif
#include <string>
#include <GL/gl.h>

class cTexture;

class cFont
{
public:
	int	width[256];

	cTexture* texture;

	GLuint	list_base;

	cFont();
	~cFont();

	int		load(std::string);

	int		print(float, float, float, float, float, const char*, ...);
	int		print3d(float, float, float, float, float, float, float, float, const char*, ...);
	int		textlen(std::string);
};


#endif
