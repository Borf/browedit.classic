#ifndef __MAPSWINDOW_H__
#define __MAPSWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <wm/windowframe.h>
#include <bmutex.h>
#include <bthread.h>
//#include <gl/gl.h>

class cMapsWindow : public cWindow
{
	class cSearchButton : public cWindowButton
	{
	public:
		cSearchButton(cWindow*, TiXmlDocument* = NULL);
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
		cClickableFrame(cWindow* parent, TiXmlDocument* skin = NULL);
		void click();
	};
	
	cMapsWindow();
	~cMapsWindow();

	std::vector<cDownloadThread*>	threads;
};

#endif
