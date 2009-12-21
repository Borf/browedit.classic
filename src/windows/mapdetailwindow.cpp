#include "mapdetailwindow.h"
#include <graphics.h>

#include <wm/windowobject.h>
#include <wm/windowroundbutton.h>
#include <wm/windowlabel.h>
#include <wm/windowinputbox.h>
#include <wm/windowcheckbox.h>
#include <wm/windowframe.h>
#include <wm/windowscrollpanel.h>
#include <wm/windowpicturebox.h>
#include <bengine/util.h>

extern void	mainloop();

//#include <bthread.h>
//extern cBMutex* renderMutex;

cMapDetailWindow::cMapDetailWindow() : cWindow()
{
	windowType = WT_MAPDETAIL;
	resizable = false;
	visible = true;

	h = cGraphics::h()-20;
	w = cGraphics::w()-20;
	title = GetMsg("wm/mapdetail/TITLE");
	center();

	objects["rollup"] = new cWindowRollupButton(this);
	objects["close"] = new cWindowCloseButton(this);

/*	class cMapInfoFinished : public cDownloadThread::cDownloadThreadFinisher
	{
		cMapDetailWindow* wnd;
	public:
		cMapInfoFinished(cMapDetailWindow* w) : cDownloadThread::cDownloadThreadFinisher()
		{
			wnd = w;
		}
		void whenDone(cBThread* caller)
		{


		}
	};

	new cDownloadThread("http://browedit.excalibur-nw.com/mapdb/mapinfo.php?id=" + bEngine::util::intToString(1), new cMapInfoFinished(this));*/
}



cMapDetailWindow::~cMapDetailWindow()
{

}
