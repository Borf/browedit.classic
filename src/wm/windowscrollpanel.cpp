#include <common.h>
#include "windowscrollpanel.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "window.h"
#include <graphics.h>
#include <bengine/texture.h>

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
	xx = realX();
	yy = realY();


	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int ww = w - 14;


	int ybarheight = min(max(((float)h/(float)innerheight)*(h-(skinButtonDownHeight+skinButtonUpHeight) - 13), skinTopHeight+skinBottomHeight), h-skinTopHeight-skinBottomHeight);
	int ybarpos = (int)(((float)scrollposy / (float)innerheight) * (h-(skinButtonDownHeight+skinButtonUpHeight) - 13));

	int xbarwidth = max(((float)w/(float)innerwidth)*(w-(skinButtonDownHeight+skinButtonUpHeight) - 13),skinTopHeight+skinBottomHeight);
	int xbarpos = (int)(((float)scrollposx / (float)innerwidth) * (w-16 - 13));

	//ybarpos -= barheigh;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());
	glColor4f(1.0f, 1.0f, 1.0f, colors[3]);



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

	int xxx = parent->getX() + realX();
	int yyy = parent->getY() + realY();
	glViewport(xxx, yyy+(cutoffbottom)+14, w-14, h-(cutofftop) - (cutoffbottom)-14);
	glLoadIdentity();
	glOrtho(0,w-14,-scrollposy+14,-scrollposy+h,-10000,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for(unsigned int i = 0; i < objects.size(); i++)
	{
		cWindowObject* o = objects[i];
		if (//o->getX() >= scrollposx && o->getX() + o->pw() <= scrollposx + (w-18) &&
			o->getY()+o->getHeight() >= scrollposy && o->getY() <= scrollposy + (h-18))
		{
			int oldx = o->getX();
			int oldy = o->getY();

			if(o->type == OBJECT_LABEL)
				o->moveTo(-parent->getX() + o->getX() - parent->skinOffLeft, o->parent->getY() + o->getY() + parent->skinOffTop);
			else if(o->type == OBJECT_MODEL)
				o->moveTo(o->getX()+x-scrollposx, o->getY()+y-scrollposy);
			else
				o->moveTo(o->getX(),o->getY()+y);

			o->draw(0,
					0,
					(scrollposy-oldy),
					(oldy - scrollposy + o->getHeight()) - h	);

			o->moveTo(oldx, oldy);

		}
	}

	
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}



void cWindowScrollPanel::onClick()
{
	int xx = (int)cGraphics::cMouse::x;
	xx -= realX();
	xx -= parent->getX();
	int yy = (cGraphics::h()-(int)cGraphics::cMouse::y);
	yy -= realY();
	yy -= parent->getY();

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
				scrollposx = (int)((cGraphics::cMouse::x +(xbarwidth/2)- parent->getX() - xbarwidth) / (float)(w-16))*innerwidth;
				scrollposx = (int)min(max(scrollposx, 0), innerwidth-w);
			}
		}
		else
		{
// in the box
			parent->moveTo(parent->getX()+scrollposx+x, parent->getY()+scrollposy-y);
			bool found = false;
			for(unsigned int i = 0; i < objects.size(); i++)
			{
				if (objects[i]->inObject())
				{
					found = true;
					parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
					objects[i]->onClick();
					break;
				}
			}
			if (!found)
				parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
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
			scrollposy = (int)((h-(((cGraphics::h()-cGraphics::cMouse::y)+(ybarheight/2)) -parent->getY() - ybarheight)) / (float)(h-16)) * innerheight - h;
			scrollposy = (int)min(max(scrollposy, 0), innerheight-h);
		}	
	}

}

void cWindowScrollPanel::drag()
{
	int xx = (int)cGraphics::cMouse::x;
	xx -= realX();
	xx -= parent->getX();
	int yy = (cGraphics::h()-(int)cGraphics::cMouse::y);
	yy -= realY();
	yy -= parent->getY();
	int ybarheight = (int)max((int)(((float)h/(float)innerheight)*h), 16);
	int xbarwidth = (int)max((int)(((float)w/(float)innerwidth)*(w-16 - 13)),18);


	if(draggingObject != NULL)
	{
		parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy-y);
		draggingObject->drag();
		parent->moveTo(parent->getX()+scrollposx+x, parent->getY()+scrollposy+y);
		return ;
	}

	if (cGraphics::cMouse::xStart - realX() - parent->getX() > w-14 && cGraphics::cMouse::xStart - realX() - parent->getX() < w)
	{
		scrollposy = (int)(((h-(((cGraphics::h()-cGraphics::cMouse::y)+(ybarheight/2)) -parent->getY() - ybarheight)) / (float)(h-16)) * innerheight - h);
		scrollposy = (int)min(max(scrollposy, 0), innerheight-h);
	}
	else if ((cGraphics::h()-cGraphics::cMouse::yStart) - parent->getY() - realY() < 14)
	{
		scrollposx = (int)((cGraphics::cMouse::x +(xbarwidth/2)- parent->getX() - xbarwidth) / (float)(w-16))*innerwidth;
		scrollposx = (int)min(max(scrollposx, 0), innerwidth-w);
	}
	else
	{
		parent->moveTo(parent->getX()+scrollposx+x, parent->getY()+scrollposy-y);
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->inObject())
			{
				objects[i]->drag();
				draggingObject = objects[i];
				parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
				return;
			}
		}
		parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
		return;
	}

}

bool cWindowScrollPanel::onKeyUp(int keyid, bool shift)
{
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inObject())
			return objects[i]->onKeyUp(keyid, shift);
	}
	return false;
}

bool cWindowScrollPanel::onKeyDown(int keyid, bool shift)
{
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inObject())
			return objects[i]->onKeyDown(keyid, shift);
	}
	return false;
}

bool cWindowScrollPanel::onChar(char keyid, bool shift)
{
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inObject())
			return objects[i]->onChar(keyid, shift);
	}
	return false;
}

void cWindowScrollPanel::onDoubleClick()
{
	parent->moveTo(parent->getX()+scrollposx+x, parent->getY()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inObject())
		{
			parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
			objects[i]->onDoubleClick();
			return;
		}
	}
	parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
	return;
}

void cWindowScrollPanel::onRightClick()
{
	parent->moveTo(parent->getX()+scrollposx+x, parent->getY()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inObject())
			objects[i]->onRightClick();
	}
	parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
}



std::string cWindowScrollPanel::getPopup()
{
	parent->moveTo(parent->getX()+scrollposx+x, parent->getY()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inObject())
		{
			parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
			return objects[i]->getPopup();
		}
	}
	parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
	return "";
}


cWindowObject* cWindowScrollPanel::inObject()
{
	parent->moveTo(parent->getX()+scrollposx+x, parent->getY()+scrollposy-y);
	for(int i = objects.size()-1; i >= 0; i--)
	{
		if (objects[i]->inObject())
		{
			parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
			return objects[i];
		}
	}
	parent->moveTo(parent->getX()-scrollposx-x, parent->getY()-scrollposy+y);
	return cWindowObject::inObject();
}


void cWindowScrollPanel::onScrollUp()
{
	scrollposy= scrollposy - h/2;
	scrollposy = (int)max(min(scrollposy, innerheight-h), 0);
}

void cWindowScrollPanel::onScrollDown()
{
	scrollposy = scrollposy + h/2;
	scrollposy = (int)max(min(scrollposy, innerheight-h), 0);
}

cWindowScrollPanel::cWindowScrollPanel( cWindow* parent, TiXmlDocument* skin ) : cWindowObject(parent, skin->FirstChildElement("skin")->FirstChildElement("list"))
{
	w = 100;
	h = 25;
	x = 10;
	y = 10;
	alignment = ALIGN_CENTER;
	cursorType = 0;
	selectable = true;
	type = OBJECT_BUTTON;
	innerheight = 1000;
	innerwidth = 1000;
	
	scrollposx = 0;
	scrollposy = 0;
	draggingObject = NULL;
	
	TiXmlElement* bSkin = skin->FirstChildElement("skin")->FirstChildElement("list")->FirstChildElement("scroll");
	
	skinBarWidth =			atoi(bSkin->FirstChildElement("width")->FirstChild()->Value());
	skinBarLeft =			atoi(bSkin->FirstChildElement("left")->FirstChild()->Value());
	skinButtonUpLeft =		atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("left")->FirstChild()->Value());
	skinButtonUpTop =		512-atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("top")->FirstChild()->Value());
	skinButtonUpHeight =	atoi(bSkin->FirstChildElement("buttonup")->FirstChildElement("height")->FirstChild()->Value());
	skinButtonDownLeft =	atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("left")->FirstChild()->Value());
	skinButtonDownTop =		512-atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("top")->FirstChild()->Value());
	skinButtonDownHeight =	atoi(bSkin->FirstChildElement("buttondown")->FirstChildElement("height")->FirstChild()->Value());
	
	skinBarTopHeight = atoi(bSkin->FirstChildElement("top")->Attribute("height"));
	skinBarTop =		512 - atoi(bSkin->FirstChildElement("top")->FirstChild()->Value());
	skinBarBottomHeight = atoi(bSkin->FirstChildElement("bottom")->Attribute("height"));
	skinBarBottom =		512 - atoi(bSkin->FirstChildElement("bottom")->FirstChild()->Value());
	
	skinBarBackTop=		512-atoi(bSkin->FirstChildElement("background")->FirstChildElement("top")->FirstChild()->Value());
	skinBarBackHeight =	atoi(bSkin->FirstChildElement("background")->FirstChildElement("height")->FirstChild()->Value());
	skinBarBackLeft =	atoi(bSkin->FirstChildElement("background")->FirstChildElement("left")->FirstChild()->Value());
	skinBarCenterHeight = atoi(bSkin->FirstChildElement("centerheight")->FirstChild()->Value());
}
