#ifndef __MAPSWINDOW_H__
#define __MAPSWINDOW_H__

#include <wm/window.h>
#include <wm/windowbutton.h>
#include <wm/windowframe.h>
//#include <gl/gl.h>

class cMapsWindow : public cWindow
{
	class cSearchButton : public cWindowButton
	{
	public:
		cSearchButton(cWindow*, TiXmlDocument* = NULL);
		void onClick();
	};


public:

	class cWindowTextureBox : public cWindowObject
	{
	public:
		//GLuint tex;
		bEngine::cTexture* tex;
		cWindowTextureBox(cWindow* parent);
		~cWindowTextureBox();
		void draw(int,int,int,int);
	};

	class cClickableFrame : public cWindowFrame
	{
	public:
		cClickableFrame(cWindow* parent, TiXmlDocument* skin = &cWM::skin);
		void onClick();
	};
	
	cMapsWindow();
	~cMapsWindow();

//	std::vector<cDownloadThread*>	threads;
};

#endif
