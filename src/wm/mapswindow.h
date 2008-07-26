#ifndef __MAPSWINDOW_H__
#define __MAPSWINDOW_H__

#include "window.h"
#include "windowbutton.h"
#include "windowframe.h"
#include <bmutex.h>
#include <bthread.h>
//#include <gl/gl.h>

class cMapsWindow : public cWindow
{
	class cSearchButton : public cWindowButton
	{
	public:
		cSearchButton(cWindow*, TiXmlDocument &);
		void click();
	};


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

	class cClickableFrame : public cWindowFrame
	{
	public:
		cClickableFrame(cWindow* parent, TiXmlDocument &skin) : cWindowFrame(parent,skin)
		{
		}
		void click();
	};
	
	cMapsWindow(cTexture* t, cFont* f, TiXmlDocument &skin);
	~cMapsWindow();

	vector<cDownloadThread*>	threads;
};

#endif
