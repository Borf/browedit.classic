#include "mapdetailwindow.h"
#include <graphics.h>

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "windowcheckbox.h"
#include "windowframe.h"
#include "windowscrollpanel.h"
#include "windowpicturebox.h"

extern cGraphics Graphics;
extern void	mainloop();


#include <bthread.h>
extern cBMutex* renderMutex;

cMapDetailWindow::cMapDetailWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
{
	cWindowObject* o;
	wtype = WT_MAPDETAIL;
	resizable = false;
	visible = true;

	h = Graphics.h()-20;
	w = Graphics.w()-20;
	title = GetMsg("wm/mapdetail/TITLE");
	center();

	objects["rollup"] = new cWindowRollupButton(this,skin);
	objects["close"] = new cWindowCloseButton(this,skin);

	class cMapInfoFinished : public cDownloadThread::cDownloadThreadFinisher
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

	new cDownloadThread("http://browedit.excalibur-nw.com/mapdb/mapinfo.php?id=" + inttostring(1), new cMapInfoFinished(this));
}



cMapDetailWindow::~cMapDetailWindow()
{

}
