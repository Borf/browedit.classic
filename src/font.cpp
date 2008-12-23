#define __FONT_CPP__


#include "font.h"

#include "texture.h"
#include <vector>
#include <texturecache.h>

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#ifdef __MINGW32__
#include <GL/glext.h>											// We use a define from this file: GL_BGRA_EXT
#endif

#include <stdexcept>
#include <vector>
#include <map>
#ifndef WIN32
#include <cstdarg>
#include <unistd.h>
#endif
//using namespace std;



cFont::cFont()
{

}

cFont::~cFont()
{
  glDeleteLists(list_base,256);                       // Delete All 256 Display Lists
  texture->unLoad();
  delete texture;
  texture = NULL;
}


int cFont::print(float r, float g, float b, float x, float y, const char *fmt, ...)
{
	char    text[1024];
	int screenStats[4],blendSrc,blendDst;
	unsigned char blending;

	va_list ap;

	if (fmt == NULL)
		return 1;

	va_start(ap, fmt);
		vsprintf(text, fmt, ap);
	va_end(ap);


	glGetIntegerv(GL_VIEWPORT, screenStats);
	glGetIntegerv(GL_BLEND_SRC, &blendSrc);
	glGetIntegerv(GL_BLEND_DST, &blendDst);
	glGetBooleanv(GL_BLEND, &blending);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture->texId());
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	GLfloat tempColors[4];
	glGetFloatv(GL_CURRENT_COLOR, tempColors);
	if (r != 2 && g != 2 && b != 2)
		glColor4f(r,g,b,tempColors[3]);

	glLoadIdentity();
	glTranslated(x,y,0);

	std::string txt = text;
	std::string colorcode = "#";
	glListBase(list_base);
	while(txt.find(colorcode) != std::string::npos && txt.find(colorcode) < txt.length()-6)
	{
		std::string before = txt.substr(0, txt.find(colorcode));		
		std::string color = txt.substr(txt.find(colorcode)+colorcode.length(),6);

		if(color.substr(0,colorcode.length()) == colorcode)
		{
			before = before + colorcode;
			color = "";
			txt = txt.substr(txt.find(colorcode)+2*colorcode.length());

		}
		else
			txt = txt.substr(txt.find(colorcode)+colorcode.length()+6);


		glCallLists(before.length(),GL_UNSIGNED_BYTE, before.c_str());
		if(color != "")
			glColor4ub(hex2dec(color.substr(0,2)),hex2dec(color.substr(2,2)),hex2dec(color.substr(4,2)), tempColors[3]*255);
	}
	glCallLists(txt.length(),GL_UNSIGNED_BYTE, txt.c_str());


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	if (!blending)
		glDisable(GL_BLEND);
	glBlendFunc(blendSrc, blendDst);
	return 1;
}



int cFont::print3d(float r, float g, float b, float a, float x, float y, float z, float scale, const char *fmt, ...)
{
	char type = 0;
	char    text[1024];
	int screenStats[4],blendSrc,blendDst;
	char typ=type;
	unsigned char blending;
	
	va_list ap;

	if (fmt == NULL)
		return 1;

	va_start(ap, fmt);
		vsprintf(text, fmt, ap);
	va_end(ap);

	if (type>3)
		type=3;


	glGetIntegerv(GL_VIEWPORT, screenStats);

	glGetIntegerv(GL_BLEND_SRC, &blendSrc);
	glGetIntegerv(GL_BLEND_DST, &blendDst);
	glGetBooleanv(GL_BLEND, &blending);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, texture->texId());
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	if (r != 2 && g != 2 && b != 2)
		glColor4f(r,g,b, a);

	if(type>1) typ=typ-2;
	glTranslatef(x,y,z);

	glPushMatrix();
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	for(int xx = 0; xx < 3; xx++)
		for(int yy = 0; yy < 3; yy++)
			modelview[xx*4+yy] = ((xx == yy) ? 1.0 : 0.0);
	glLoadMatrixf(modelview);
	
	glScalef(scale, scale, scale);
	
	std::string txt = text;
	std::string colorcode = "#";
	glListBase(list_base);
	while(txt.find(colorcode) != std::string::npos)
	{
		std::string before = txt.substr(0, txt.find(colorcode));		
		std::string color = txt.substr(txt.find(colorcode)+colorcode.length(),6);

		if(color.substr(0,colorcode.length()) == colorcode)
		{
			before = before + colorcode;
			color = "";
			txt = txt.substr(txt.find(colorcode)+2*colorcode.length());

		}
		else
			txt = txt.substr(txt.find(colorcode)+colorcode.length()+6);


		glCallLists(before.length(),GL_UNSIGNED_BYTE, before.c_str());
		if(color != "")
			glColor3ub(hex2dec(color.substr(0,2)),hex2dec(color.substr(2,2)),hex2dec(color.substr(4,2)));
	}
	glCallLists(txt.length(),GL_UNSIGNED_BYTE, txt.c_str());


	if (!blending)
		glDisable(GL_BLEND);
	glBlendFunc(blendSrc, blendDst);
	
	glPopMatrix();
	glTranslatef(-x,-y,-z);
	return 1;
}




int	cFont::textLen(std::string s)
{
	int l = 0;
	for(int i = 0; i < (int)s.length(); i++)
	{
		if (((BYTE)s[i]) < 255)
		{
			l = l + width[((BYTE)s[i])]+1;
			if(width[((BYTE)s[i])] == 0)
				l+=10;
		}
		else
			l = l + 10;

	}
	return l;
}



int cFont::load(std::string filename)
{
	texture = cTextureLoaders::load(filename,TEX_NOFREEDATA);
	Log(5,0,"Loading font %s", filename.c_str());
	float cx, cy;
	list_base=glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, texture->texId());
	for (int loop1=0; loop1<256; loop1++)
	{
		cx=(float)(loop1%16)/16.0f;
		cy=(float)(loop1/16)/16.0f;
	
		int w = 2;
		for(int x = (int)(cx*(float)texture->width)+2; x < (int)((cx+(1.0f/16.0f))*(float)texture->width); x++)
		{
			bool found = false;
			for(int y = (int)(cy*(float)texture->height); y < (int)((cy+(1.0f/16.0f))*(float)texture->height); y++)
			{
				if (texture->data[4*(texture->width*y+x)+3] > 1)
					found = true;
			}
			if (found)
				w++;
			else
				break;
		}
		width[loop1] = w;

		// for the letter J overlapping the letter I
		#define fontwidth  0.059f
		#define fontheight 0.0625f

		glNewList(list_base+loop1,GL_COMPILE); 
		glBegin(GL_QUADS);
			glTexCoord2f(cx,cy);						glVertex2d(0,16);
			glTexCoord2f(cx,cy+fontheight);				glVertex2i(0,0); 
			glTexCoord2f(cx+fontwidth,cy+fontheight);	glVertex2i(16,0);
			glTexCoord2f(cx+fontwidth,cy);				glVertex2i(16,16);
		glEnd();
		glTranslated(width[loop1]+1,0,0);
		glEndList();
	}
	Log(3,0,"Done loading font %s", filename.c_str());
	return 1;
}
