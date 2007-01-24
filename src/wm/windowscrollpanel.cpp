#include "windowscrollpanel.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "window.h"
#include "../graphics.h"
extern cGraphics Graphics;
extern float mousex, mousey;
extern cWindowObject* draggingobject;

cWindowScrollPanel::~cWindowScrollPanel()
{
	for(unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
}

void cWindowScrollPanel::draw()
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


	int ybarheight = (int)max(((float)h/(float)innerheight)*(h-16 - 13), 18.0f);
	int ybarpos = (int)((float)scrollposy / (float)innerheight) * (h-16 - 13);

	int xbarwidth = (int)max(((float)w/(float)innerwidth)*(w-16 - 13),18.0f);
	int xbarpos = (int)((float)scrollposx / (float)innerwidth) * (w-16 - 13);

	//ybarpos -= barheigh;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
	glColor3f(1.0f, 1.0f, 1.0f);



	glBegin(GL_QUADS);
	if (h < innerheight)
	{

// Vertical Scrollbar
/// scrollthingy-background
		glTexCoord2f((408.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+5, yy+8+13);
		glTexCoord2f((413.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+10, yy+8+13);
		glTexCoord2f((413.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+10, yy+h-8);
		glTexCoord2f((408.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+5, yy+h-8);
/// arrow up
		glTexCoord2f((404.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+1, yy+h-8);
		glTexCoord2f((417.0f)/512.0f,		(446.0f)/512.0f);			glVertex2d(xx+ww+14,yy+h-8);
		glTexCoord2f((417.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+ww+14,yy+h);
		glTexCoord2f((404.0f)/512.0f,		(454.0f)/512.0f);			glVertex2d(xx+ww+1, yy+h);
/// arrow down
		glTexCoord2f((404.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww+1, yy+13);
		glTexCoord2f((417.0f)/512.0f,		(358.0f)/512.0f);			glVertex2d(xx+ww+14,yy+13);
		glTexCoord2f((417.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+14,yy+8+13);
		glTexCoord2f((404.0f)/512.0f,		(366.0f)/512.0f);			glVertex2d(xx+ww+1, yy+8+13);
/// block
//top
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-ybarpos-9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-ybarpos-9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+ww+13,yy+h-8-ybarpos);
		glTexCoord2f((420.0f)/512.0f,		(448.0f)/512.0f);			glVertex2d(xx+ww+2, yy+h-8-ybarpos);
//bottom
		glTexCoord2f((420.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+ww+2, yy+h-8-ybarpos-ybarheight);
		glTexCoord2f((431.0f)/512.0f,		(428.0f)/512.0f);			glVertex2d(xx+ww+13,yy+h-8-ybarpos-ybarheight);
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-ybarpos-ybarheight+9);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-ybarpos-ybarheight+9);
//block		
		glTexCoord2f((431.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-ybarpos-ybarheight+9);
		glTexCoord2f((431.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+13,yy+h-8-ybarpos-9);
		glTexCoord2f((420.0f)/512.0f,		(448.0f-9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-ybarpos-9);
		glTexCoord2f((420.0f)/512.0f,		(428.0f+9.0f)/512.0f);		glVertex2d(xx+ww+2, yy+h-8-ybarpos-ybarheight+9);
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


	glTranslatef(x-scrollposx, -y+scrollposy,0);

	for(unsigned int i = 0; i < objects.size(); i++)
	{
		cWindowObject* o = objects[i];
		if (o->px() >= scrollposx && o->px() + o->pw() <= scrollposx + (w-18) &&
			o->py() >= scrollposy && o->py() + o->ph() <= scrollposy + (h-18) && o->type != OBJECT_LINE)
		{
			if(o->type == OBJECT_LABEL)
				o->moveto(o->px() - scrollposx, o->py() - scrollposy);
			o->draw();
			if(o->type == OBJECT_LABEL)
				o->moveto(o->px() + scrollposx, o->py() + scrollposy);
		}
		if (o->px() >= scrollposx && o->px() + o->pw() <= scrollposx + (w-18) &&
			o->py() >= scrollposy && o->ph() <= scrollposy + (h-18) && o->type == OBJECT_LINE)
		{
			o->draw();	
		}
	}
	glTranslatef(-x+scrollposx, y-scrollposy,0);

}



void cWindowScrollPanel::click()
{
	int xx = mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = (Graphics.h()-mousey);
	yy -= realy();
	yy -= parent->py();

	int ybarheight = (int)max(((float)h/(float)innerheight)*h, 16);
	int xbarwidth = (int)max(((float)w/(float)innerwidth)*w, 16);

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
			parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy-y);
			bool found = false;
			for(unsigned int i = 0; i < objects.size(); i++)
			{
				if (objects[i]->inobject())
				{
					found = true;
					parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
					objects[i]->click();
					break;
				}
			}
			if (!found)
				parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
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
	int xx = mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = (Graphics.h()-mousey);
	yy -= realy();
	yy -= parent->py();
	int ybarheight = (int)max(((float)h/(float)innerheight)*h, 16);
	int xbarwidth = (int)max(((float)w/(float)innerwidth)*(w-16 - 13),18);


	if(draggingobject != NULL)
	{
		parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy-y);
		draggingobject->drag();
		parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
		return ;
	}

	if (startmousex - realx() - parent->px() > w-14 && startmousex - realx() - parent->px() < w)
	{
			scrollposy = (int)((h-(((Graphics.h()-mousey)+(ybarheight/2)) -parent->py() - ybarheight)) / (float)(h-16)) * innerheight - h;
			scrollposy = (int)min(max(scrollposy, 0), innerheight-h);
	}
	else if ((Graphics.h()-startmousey) - parent->py() - realy() < 14)
	{
		scrollposx = (int)((mousex +(xbarwidth/2)- parent->px() - xbarwidth) / (float)(w-16))*innerwidth;
		scrollposx = (int)min(max(scrollposx, 0), innerwidth-w);
	}
	else
	{
		parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy-y);
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->inobject())
			{
				objects[i]->drag();
				draggingobject = objects[i];
				parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
				return;
			}
		}
		parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
		return;
	}

}

bool cWindowScrollPanel::onkeyup(int keyid)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->inobject())
			return objects[i]->onkeyup(keyid);
	}
	return false;
}

bool cWindowScrollPanel::onkeydown(int keyid)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->inobject())
			return objects[i]->onkeydown(keyid);
	}
	return false;
}

bool cWindowScrollPanel::onchar(char keyid)
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->inobject())
			return objects[i]->onchar(keyid);
	}
	return false;
}

void cWindowScrollPanel::doubleclick()
{
	parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy-y);
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->inobject())
		{
			parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
			objects[i]->doubleclick();
			return;
		}
	}
	parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
	return;
}

void cWindowScrollPanel::rightclick()
{
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->inobject())
			objects[i]->rightclick();
	}
}



string cWindowScrollPanel::ppopup()
{
	parent->moveto(parent->px()-scrollposx-x, parent->py()-scrollposy-y);
	for(unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->inobject())
		{
			parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
			return objects[i]->ppopup();
		}
	}
	parent->moveto(parent->px()+scrollposx+x, parent->py()+scrollposy+y);
	return "";
}


