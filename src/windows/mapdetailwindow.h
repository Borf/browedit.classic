#ifndef __MAPDETAILWINDOW_H__
#define __MAPDETAILWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <bmutex.h>
#include <bthread.h>
//#include <gl/gl.h>

class cMapDetailWindow : public cWindow
{
public:
	class cWindowTextureBox : public cWindowObject
	{
	public:
		//GLuint tex;
		cTexture* tex;
		cWindowTextureBox(cWindow* parent);
		~cWindowTextureBox();
		void draw(int,int,int,int);
	};

	
	cMapDetailWindow(cTexture* t, cFont* f, TiXmlDocument &skin);
	~cMapDetailWindow();
};

#endif
