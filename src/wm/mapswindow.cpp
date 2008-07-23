#include "mapswindow.h"
#include <graphics.h>

#include "windowobject.h"
#include "windowroundbutton.h"
#include "windowlabel.h"
#include "windowinputbox.h"
#include "windowcheckbox.h"
#include "windowframe.h"
#include "windowscrollpanel.h"
#include "windowpicturebox.h"
#include "mapdetailwindow.h"

extern cGraphics Graphics;
extern void	mainloop();


#include <bthread.h>
extern cBMutex* renderMutex;

cMapsWindow::cMapsWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
{
	cWindowObject* o;
	wtype = WT_MAPDATABASE;
	resizable = false;
	visible = true;

	h = Graphics.h()-20;
	w = Graphics.w()-20;
	title = GetMsg("wm/maps/TITLE");
	center();

	objects["rollup"] = new cWindowRollupButton(this,skin);
	objects["close"] = new cWindowCloseButton(this,skin);

	o = new cWindowFrame(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(0,0);
	o->resizeto(innerw(), 40);
	objects["aatopframe"] = o;

	objects["searchbutton"] = new cSearchButton(this,skin);
	
	o = new cWindowInputBox(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(10,10);
	o->resizeto(innerw()-130, o->ph());
	o->SetText(0,"");
	objects["searchq"] = o;

	o = new cWindowScrollPanel(this,skin);
	o->alignment = ALIGN_TOPLEFT;
	o->moveto(0,40);
	o->resizeto(innerw(), innerh()-40);
	objects["results"] = o;


	class cIndexFinished : public cDownloadThread::cDownloadThreadFinisher
	{
		cMapsWindow* wnd;

		class cImageFinished : public cDownloadThread::cDownloadThreadFinisher
		{
			cMapsWindow* wnd;
			int index;
		public:
			cImageFinished (cMapsWindow* w, int i) : cDownloadThread::cDownloadThreadFinisher()
			{
				wnd = w;
				index = i;
			}
			void whenDone(cBThread* caller)
			{
				renderMutex->lock();
				for(int i = 0; i < wnd->threads.size(); i++)
					if(wnd->threads[i] == caller)
						wnd->threads[i] = NULL;

				cWindowScrollPanel* panel = (cWindowScrollPanel*)wnd->objects["results"];
				int px = 20+210 * (index%(panel->pw() / 210));
				int py = 310*(index/(panel->pw() / 210))+10;

				cMapsWindow::cWindowTextureBox* o = new cMapsWindow::cWindowTextureBox(wnd);
				o->alignment = ALIGN_TOPLEFT;
				o->moveto(px+10,py+30);
				o->resizeto(180,140);
				o->tex = new cTextureFromMemory(".gif", data, length);
				panel->objects.push_back(o);		
				renderMutex->unlock();
			}
		};




	public:
		cIndexFinished(cMapsWindow* w) : cDownloadThread::cDownloadThreadFinisher()
		{
			wnd = w;
		}
		void whenDone(cBThread* caller)
		{
			TiXmlDocument list;
			list.SetCondenseWhiteSpace(false);
			list.Parse(data);
			if(list.Error())
			{
				Graphics.WM.MessageBox("Could not parse list!");
				return;
			}

			renderMutex->lock();

			TiXmlElement* m = list.RootElement()->FirstChildElement("map");
			cWindowScrollPanel* panel = (cWindowScrollPanel*)wnd->objects["results"];
			panel->objects.clear();
			int i = 0;
			cWindowObject* o;
			vector<int> screenshots;
			while(m != NULL)
			{
				o = new cMapsWindow::cClickableFrame(wnd,Graphics.WM.skin);
				o->alignment = ALIGN_TOPLEFT;
				int px = 20+210 * (i%(panel->pw() / 210));
				int py = 310*(i/(panel->pw() / 210))+10;
				o->moveto(px,py);
				o->resizeto(200,300);
				o->SetInt(0,0);
				panel->objects.push_back(o);


				o = new cWindowLabel(wnd);
				o->alignment = ALIGN_TOPLEFT;
				o->moveto(px+10, py+10);
				o->resizeto(200,12);
				o->SetText(0,m->FirstChildElement("name")->FirstChild()->Value());
				panel->objects.push_back(o);

				o = new cWindowLabel(wnd);
				o->alignment = ALIGN_TOPLEFT;
				o->moveto(px+10, py+200);
				o->resizeto(200,12);
				o->SetText(0,"By " + string(m->FirstChildElement("author")->FirstChild()->Value()));
				panel->objects.push_back(o);
			
				screenshots.push_back(atoi(m->FirstChildElement("id")->FirstChild()->Value()));
				panel->innerheight = py+310;

				
				m = m->NextSiblingElement("map");
				i++;
			}
			renderMutex->unlock();
			mainloop();

			for(i = 0; i < screenshots.size(); i++)
			{
				wnd->threads.push_back(new cDownloadThread("http://browedit.excalibur-nw.com/images/maps/" + inttostring(screenshots[i]) + "_screenshot.gif", new cImageFinished(wnd,i)));
			}

		}
	};

	new cDownloadThread("http://browedit.excalibur-nw.com/mapdb/list.php", new cIndexFinished(this));
}





cMapsWindow::cSearchButton::cSearchButton(cWindow* parent, TiXmlDocument &skin) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPRIGHT;
	moveto(10,10);
	resizeto(100,20);
	text = "Search";
}

void cMapsWindow::cSearchButton::click()
{
	Graphics.WM.MessageBox("You clicked!");

}


cMapsWindow::cWindowTextureBox::cWindowTextureBox(cWindow* parent) : cWindowObject(parent)
{
	tex = NULL;
}

cMapsWindow::cWindowTextureBox::~cWindowTextureBox()
{
//	if(tex != -1)
//		glDeleteTextures(1, &tex);
	if(tex != NULL)
		delete tex;
}


void cMapsWindow::cWindowTextureBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int xx, yy;
	xx = realx();
	yy = realy();

	glColor4f(1,1,1,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex->texid());
	glBegin(GL_QUADS);
/*		glTexCoord2f(0,cutoffbottom / (float)h);		glVertex2d(xx+cutoffleft,		yy+cutoffbottom);
		glTexCoord2f(1,cutoffbottom / (float)h);		glVertex2d(xx+w-cutoffright,	yy+cutoffbottom);
		glTexCoord2f(1,1 - cutofftop / (float)h);			glVertex2d(xx+w-cutoffright,	yy+h-cutofftop);
		glTexCoord2f(0,1 - cutofftop / (float)h);			glVertex2d(xx+cutoffleft,		yy+h-cutofftop);*/
		glTexCoord2f(0,0);		glVertex2d(xx,		yy);
		glTexCoord2f(1,0);		glVertex2d(xx+w,	yy);
		glTexCoord2f(1,1);		glVertex2d(xx+w,	yy+h);
		glTexCoord2f(0,1);		glVertex2d(xx,		yy+h);
	glEnd();
}

cMapsWindow::~cMapsWindow()
{
	for(int i = 0; i < threads.size(); i++)
		if(threads[i] != NULL)
		{
			threads[i]->stop();
			delete threads[i];
			threads[i] = NULL;
		}
}

void cMapsWindow::cClickableFrame::click()
{
	Graphics.WM.addwindow(new cMapDetailWindow(Graphics.WM.texture, &Graphics.WM.font, Graphics.WM.skin));
}