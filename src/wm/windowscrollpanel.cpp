#include "windowscrollpanel.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "window.h"
#include <graphics.h>
extern cGraphics Graphics;
extern cWindowObject* draggingobject;

cWindowScrollPanel::~cWindowScrollPanel()
{
	for(unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
}

void cWindowScrollPanel::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
/*	for(int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw();
	}*/
	int xx, yy;
	xx = realx();
	yy = realy();


	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int ww = w - 14;


	int ybarheight = min(max(((float)h/(float)innerheight)*(h-(skinButtonDownHeight+skinButtonUpHeight) - 13), skinTopHeight+skinBottomHeight), h-skinTopHeight-skinBottomHeight);
	int ybarpos = (int)(((float)scrollposy / (float)innerheight) * (h-(skinButtonDownHeight+skinButtonUpHeight) - 13));

	int xbarwidth = max(((float)w/(float)innerwidth)*(w-(skinButtonDownHeight+skinButtonUpHeight) - 13),skinTopHeight+skinBottomHeight);
	int xbarpos = (int)(((float)scrollposx / (float)innerwidth) * (w-16 - 13));

	//ybarpos -= barheigh;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glColor3f(1.0f, 1.0f, 1.0f);



	glBegin(GL_QUADS);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+0,				yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+h);
		glTexCoord2f(skinLeft/512.0f,					skinTop/512.0f);						glVertex2d(xx+0,				yy+h);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+h);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+h);

		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww,				yy+h-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinTop/512.0f);						glVertex2d(xx+ww,				yy+h);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+h);

		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(xx+0,				yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+0,				yy+h-skinTopHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+skinLeftWidth,	yy+h-skinTopHeight);

		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+ww,				yy+skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww,				yy+h-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(xx+ww-skinRightWidth,	yy+h-skinTopHeight);
///////////////			
		glTexCoord2f(skinLeft/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+0,				yy+0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+skinLeftWidth,	yy+0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);
		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(xx+0,				yy+skinBottomHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+skinLeftWidth,	yy+0);
		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+ww-skinRightWidth,	yy+0);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+skinLeftWidth,	yy+skinBottomHeight);

		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+ww-skinRightWidth,	yy+0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(xx+ww,				yy+0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(xx+ww,				yy+skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(xx+ww-skinRightWidth,	yy+skinBottomHeight);
	

		if (h < innerheight)
		{
/// scrollthingy-background
		glTexCoord2f((skinBarBackLeft)/512.0f,				(skinBarBackTop)/512.0f);						glVertex2d(xx+ww,				yy+skinButtonDownHeight);
		glTexCoord2f((skinBarBackLeft+skinBarWidth)/512.0f,	(skinBarBackTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+skinButtonDownHeight);
		glTexCoord2f((skinBarBackLeft+skinBarWidth)/512.0f,	(skinBarBackTop-skinBarBackHeight)/512.0f);		glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight);
		glTexCoord2f((skinBarBackLeft)/512.0f,				(skinBarBackTop-skinBarBackHeight)/512.0f);		glVertex2d(xx+ww,				yy+h-skinButtonUpHeight);
/// arrow up
		glTexCoord2f((skinButtonUpLeft)/512.0f,					(skinButtonUpTop-skinButtonUpHeight)/512.0f);	glVertex2d(xx+ww,				yy+h-skinButtonUpHeight);
		glTexCoord2f((skinButtonUpLeft+skinBarWidth)/512.0f,	(skinButtonUpTop-skinButtonUpHeight)/512.0f);	glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight);
		glTexCoord2f((skinButtonUpLeft+skinBarWidth)/512.0f,	(skinButtonUpTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h);
		glTexCoord2f((skinButtonUpLeft)/512.0f,					(skinButtonUpTop)/512.0f);						glVertex2d(xx+ww,				yy+h);
/// arrow down
		glTexCoord2f((skinButtonDownLeft)/512.0f,				(skinButtonDownTop-skinButtonDownHeight)/512.0f);		glVertex2d(xx+ww,				yy);
		glTexCoord2f((skinButtonDownLeft+skinBarWidth)/512.0f,	(skinButtonDownTop-skinButtonDownHeight)/512.0f);		glVertex2d(xx+ww+skinBarWidth,	yy);
		glTexCoord2f((skinButtonDownLeft+skinBarWidth)/512.0f,	(skinButtonDownTop)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+skinButtonDownHeight);
		glTexCoord2f((skinButtonDownLeft)/512.0f,				(skinButtonDownTop)/512.0f);						glVertex2d(xx+ww,				yy+skinButtonDownHeight);
/// block
		if(skinBarCenterHeight != 0)
		{
			int center = (skinBarTop-skinBarTopHeight + skinBarBottom)/ 2;
			int c1 = skinBarTop-skinBarTopHeight-	skinBarCenterHeight/2;
			int c2 = (skinBarBottom+				skinBarCenterHeight/2);

			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c1)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight/2-skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(c1)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight/2-skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight);
		
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom)/512.0f);						glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos);
			glTexCoord2f((skinBarLeft)/512.0f,				(c2)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c2)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos);

			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c2)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(c2)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight/2+skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft)/512.0f,				(c1)/512.0f);									glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight/2-skinBarCenterHeight/2);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(c1)/512.0f);									glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight/2-skinBarCenterHeight/2);
		}
		else
		{
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight);
			glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos);
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom)/512.0f);						glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos);
			glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight);
		}//top
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-skinBarTopHeight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop-skinBarTopHeight)/512.0f);			glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-skinBarTopHeight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarTop)/512.0f);							glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos);
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarTop)/512.0f);							glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos);
//bottom
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom-skinBarBottomHeight)/512.0f);	glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom-skinBarBottomHeight)/512.0f);	glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight);
		glTexCoord2f((skinBarLeft+skinBarWidth)/512.0f,	(skinBarBottom)/512.0f);						glVertex2d(xx+ww+skinBarWidth,	yy+h-skinButtonUpHeight-ybarpos-ybarheight+skinBarBottomHeight);
		glTexCoord2f((skinBarLeft)/512.0f,				(skinBarBottom)/512.0f);						glVertex2d(xx+ww,				yy+h-skinButtonUpHeight-ybarpos-ybarheight+skinBarBottomHeight);

		}

	if (w < innerwidth)
	{

//horizontal scrollbar
/// scrollthingy-background
		glTexCoord2f((408.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+8, yy+10);
		glTexCoord2f((413.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+8, yy+5);
		glTexCoord2f((413.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww-9,yy+5);
		glTexCoord2f((408.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww-9, yy+10);
/// arrow up
		glTexCoord2f((417.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+1,yy+14);
		glTexCoord2f((404.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+1,yy+1);
		glTexCoord2f((404.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+9,yy+1);
		glTexCoord2f((417.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+9, yy+14);
/// arrow down
		glTexCoord2f((404.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww-9, yy+14);
		glTexCoord2f((417.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww-9,yy+1);
		glTexCoord2f((417.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww-1,yy+1);
		glTexCoord2f((404.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww-1, yy+14);
/// block
//left
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+9, yy+2);
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+9,yy+13);
		glTexCoord2f((420.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+xbarpos+9,yy+13);
		glTexCoord2f((431.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+xbarpos+9, yy+2);
//right
		glTexCoord2f((431.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+xbarpos+9+xbarwidth, yy+2);
		glTexCoord2f((420.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+xbarpos+9+xbarwidth, yy+13);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+xbarwidth-9, yy+13);
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+xbarwidth-9, yy+2);
//block		
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+9,yy+2);
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+xbarwidth-9,yy+2);
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+xbarwidth-9,yy+13);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+xbarpos+9+9,yy+13);
	}
		
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	glColor4fv(colors);


	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);			// (X, Y, Width, Height)
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();

	int xxx = parent->px() + realx();
	int yyy = parent->py() + realy();
	glViewport(xxx, yyy+(cutoffbottom)+14, w-14, h-(cutofftop) - (cutoffbottom)-14);
	glLoadIdentity();
	glOrtho(0,w-14,-scrollposy+14,-scrollposy+h,-10000,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for(unsigned int i = 0; i < objects.size(); i++)
	{
		cWindowObject* o = objects[i];
		if (//o->px() >= scrollposx && o->px() + o->pw() <= scrollposx + (w-18) &&
			o->py()+o->ph() >= scrollposy && o->py() <= scrollposy + (h-18))
		{
			int oldx = o->px();
			int oldy = o->py();

			if(o->type == OBJECT_LABEL)
				o->moveto(o->px()+x-parent->px(),o->py()+y+parent->py());
			else if(o->type == OBJECT_MODEL)
				o->moveto(o->px()+x-scrollposx, o->py()+y-scrollposy);
			else
				o->moveto(o->px(),o->py()+y);

			o->draw(0,
					0,
					(scrollposy-oldy),
					(oldy - scrollposy + o->ph()) - h	);

			o->moveto(oldx, oldy);

		}
	}

	
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



void cWindowScrollPanel::click()
{
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = (Graphics.h()-(int)mousey);
	yy -= realy();
	yy -= parent->py();

	int ybarheight = (int)max((int)(((float)h/(float)innerheight)*h), 16);
	int xbarwidth = (int)max((int)(((float)w/(float)innerwidth)*w), 16);

	if (xx < w - 14)
	{ // not on the vertical scrollbar
		if (yy < 8)
		{ // on the horizontal scrollbar
			if (xx < 10)
			{
				scrollposx=(int)max(0, scrollposx-10);
			}
			else if (xx > w-10-14)
			{
				scrollposx=(int)min(scrollposx+10, innerwidth-w);
			}
			else
			{
				scrollposx = (int)((mousex +(xbarwidth/2)- parent->px() - xbarwidth) / (float)(w-16))*innerwidth;
				scrollposx = (int)min(max(scrollposx, 0), innerwidth-w);
			}
		}
		else
		{
// in the box
			parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy-y);
			bool found = false;
			for(unsigned int i = 0; i < objects.size(); i++)
			{
				if (objects[i]->inobject())
				{
					found = true;
					parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
					objects[i]->click();
					break;
				}
			}
			if (!found)
				parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
		}
	}
	else if (yy < h-8)
	{
		if (yy < 22 && yy > 14)
		{ // arrow down
			scrollposy=min(innerheight-h, scrollposy+10);
		}
		else if (yy+8+9 > h)
		{
			scrollposy=max(0, scrollposy-10);
		}
		else if (yy > 14)
		{
			scrollposy = (int)((h-(((Graphics.h()-mousey)+(ybarheight/2)) -parent->py() - ybarheight)) / (float)(h-16)) * innerheight - h;
			scrollposy = (int)min(max(scrollposy, 0), innerheight-h);
		}	
	}

}

void cWindowScrollPanel::drag()
{
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = (Graphics.h()-(int)mousey);
	yy -= realy();
	yy -= parent->py();
	int ybarheight = (int)max((int)(((float)h/(float)innerheight)*h), 16);
	int xbarwidth = (int)max((int)(((float)w/(float)innerwidth)*(w-16 - 13)),18);


	if(draggingobject != NULL)
	{
		parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy-y);
		draggingobject->drag();
		parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
		return ;
	}

	if (mousestartx - realx() - parent->px() > w-14 && mousestartx - realx() - parent->px() < w)
	{
		scrollposy = (int)(((h-(((Graphics.h()-mousey)+(ybarheight/2)) -parent->py() - ybarheight)) / (float)(h-16)) * innerheight - h);
		scrollposy = (int)min(max(scrollposy, 0), innerheight-h);
	}
	else if ((Graphics.h()-mousestarty) - parent->py() - realy() < 14)
	{
		scrollposx = (int)((mousex +(xbarwidth/2)- parent->px() - xbarwidth) / (float)(w-16))*innerwidth;
		scrollposx = (int)min(max(scrollposx, 0), innerwidth-w);
	}
	else
	{
		parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy-y);
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->inobject())
			{
				objects[i]->drag();
				draggingobject = objects[i];
				parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
				return;
			}
		}
		parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
		return;
	}

}

bool cWindowScrollPanel::onkeyup(int keyid, bool shift)
{
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inobject())
			return objects[i]->onkeyup(keyid, shift);
	}
	return false;
}

bool cWindowScrollPanel::onkeydown(int keyid, bool shift)
{
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inobject())
			return objects[i]->onkeydown(keyid, shift);
	}
	return false;
}

bool cWindowScrollPanel::onchar(char keyid, bool shift)
{
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inobject())
			return objects[i]->onchar(keyid, shift);
	}
	return false;
}

void cWindowScrollPanel::doubleclick()
{
	parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inobject())
		{
			parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
			objects[i]->doubleclick();
			return;
		}
	}
	parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
	return;
}

void cWindowScrollPanel::rightclick()
{
	parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inobject())
			objects[i]->rightclick();
	}
	parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
}



string cWindowScrollPanel::ppopup()
{
	parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inobject())
		{
			parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
			return objects[i]->ppopup();
		}
	}
	parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
	return "";
}


cWindowObject* cWindowScrollPanel::inobject()
{
	parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inobject())
		{
			parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
			return objects[i];
		}
	}
	parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy+y);
	return cWindowObject::inobject();
}


void cWindowScrollPanel::scrollup()
{
	scrollposy= scrollposy - h/2;
	scrollposy = (int)max(min(scrollposy, innerheight-h), 0);
}

void cWindowScrollPanel::scrolldown()
{
	scrollposy = scrollposy + h/2;
	scrollposy = (int)max(min(scrollposy, innerheight-h), 0);
}
