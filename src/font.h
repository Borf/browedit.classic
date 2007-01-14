#ifndef __FONT_H__
#define __FONT_H__

#include "common.h"
#include "texture.h"
#include <string>
#include <vector>
using namespace std;

class cFont
{
public:
	int	width[256];


	GLuint	list_base;
	cTexture	texture;

	cFont();
	~cFont();

	int load(string);

	int print(float, float, float, float, float, const char*, ...);
	int print3d(float, float, float, float, float, float, float, float, const char*, ...);
	int		textlen(string);
};


#endif
