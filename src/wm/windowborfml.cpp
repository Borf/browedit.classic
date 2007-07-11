#include "windowborfml.h"
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "window.h"
#include <graphics.h>
#include <texturecache.h>
extern cGraphics Graphics;
extern cWindowObject* draggingobject;

cWindowBorfMLBox::~cWindowBorfMLBox()
{
	for(int i = 0; i < textures.size(); i++)
		TextureCache.unload(textures[i]);
	textures.clear();
}




cVector2 cWindowBorfMLBox::drawnode(cVector2 curpos, TiXmlNode* n, int& lineheight, list<cArea> &areas, cVector3 color, cVector2 size, bool draw)
{
	for(list<cArea>::iterator ii = areas.begin(); ii != areas.end(); ii++)
	{
		cArea* a = &(*ii);
		ii->tl.x = min(ii->tl.x, curpos.x);
		ii->tl.y = min(ii->tl.y, curpos.y);
		ii->br.x = max(ii->br.x, curpos.x);
		ii->br.y = max(ii->br.y, curpos.y+lineheight);
	}
	while(n != NULL)
	{
		if(n->Type() == TiXmlNode:: TEXT)
		{
			string t = n->Value();
			while(t.find(" ") != string::npos)
			{
				string word = t.substr(0, t.find(" ")+1).c_str();

				if(curpos.x > w - parent->font->textlen(word))
				{
					curpos.x = 0;
					curpos.y += lineheight;
					lineheight = 12*size.y;
				}
				curpos.x = parent->font->print(color.x, color.y, color.z,curpos.x,curpos.y,size.x,-size.y, "%s", word.c_str()).x;
				t = t.substr(t.find(" ")+1);
			}
			if(curpos.x > w - parent->font->textlen(t))
			{
				curpos.x = 0;
				curpos.y += lineheight;
				lineheight = 12*size.y;
			}
			curpos.x = parent->font->print(color.x, color.y, color.z,curpos.x,curpos.y,size.x,-size.y, "%s", t.c_str()).x;

/*			
			if(draw)
				curpos.x = parent->font->print(color.x, color.y, color.z,curpos.x,curpos.y,size.x,-size.y, n->Value()).x;
			else
				curpos.x = curpos.x + parent->font->textlen(n->Value()) * size.x;*/
			lineheight = max(lineheight, 12*size.y);
		}
		else if(n->Type() == TiXmlNode::ELEMENT)
		{
			if(strcmp(n->Value(), "br") == 0)
			{
				curpos.x = 0;
				curpos.y += lineheight;
				lineheight = 12*size.y;
			}
			if(strcmp(n->Value(), "option") == 0)
			{
				areas.push_back(cArea(cVector2(999,999),cVector2(-999,-999)));
				curpos = drawnode(curpos, n->FirstChild(), lineheight, areas, cVector3(0,0,1), cVector2(1, 1), draw);
				cArea area = areas.back();
				areas.pop_back();
				options.push_back(pair<cArea, int>(area, atoi(n->ToElement()->Attribute("id"))));

				if(mousex > area.tl.x+parent->px()+realx() && mousex < area.br.x+parent->px()+realx() &&
					mousey > area.tl.y+(Graphics.h() - parent->py() - parent->ph())+realy() && mousey < area.br.y+(Graphics.h() - parent->py() - parent->ph())+realy())
					cursortype = 1;
			}
			if(strcmp(n->Value(), "center") == 0)
			{
				areas.push_back(cArea(cVector2(999,999),cVector2(-999,-999)));
				drawnode(curpos, n->FirstChild(), lineheight, areas, color, size, false);
				cArea area = areas.back();
				areas.pop_back();
				curpos = drawnode(curpos + cVector2(innerwidth/2 - (area.br.x - area.tl.x)/2,0), n->FirstChild(), lineheight, areas, color, size, draw);
			}
			if(strcmp(n->Value(), "color") == 0)
			{
				cVector3 newcolor = color;
				if(n->ToElement()->Attribute("r"))
					newcolor.x = atof(n->ToElement()->Attribute("r"));
				if(n->ToElement()->Attribute("g"))
					newcolor.y = atof(n->ToElement()->Attribute("g"));
				if(n->ToElement()->Attribute("b"))
					newcolor.z = atof(n->ToElement()->Attribute("b"));

				curpos = drawnode(curpos, n->FirstChild(), lineheight, areas, newcolor, size, draw);
			}
			if(strcmp(n->Value(), "img") == 0)
			{
				int width = atoi(n->ToElement()->Attribute("width"));
				int height = atoi(n->ToElement()->Attribute("height"));
				lineheight = max(height, lineheight);

				
				if(draw)
				{
					string src = "data/textures/interface/" + string(n->ToElement()->Attribute("src"));
					
					bool found = false;
					for(int i = 0; i < textures.size() && !found; i++)
					{
						if(textures[i]->getfilename() == src)
						{
							glBindTexture(GL_TEXTURE_2D, textures[i]->texid());
							found = true;
						}
					}
					if(!found)
					{
						textures.push_back(TextureCache.load(src));
						glBindTexture(GL_TEXTURE_2D, textures[textures.size()-1]->texid());
					}
				
					glColor3f(1,1,1);
					glEnable(GL_TEXTURE_2D);
					glBegin(GL_QUADS);
						glTexCoord2f(1,0); glVertex2f(curpos.x+width,	curpos.y-12+height);
						glTexCoord2f(1,1); glVertex2f(curpos.x+width,	curpos.y-12);
						glTexCoord2f(0,1); glVertex2f(curpos.x,			curpos.y-12);
						glTexCoord2f(0,0); glVertex2f(curpos.x,			curpos.y-12+height);
					glEnd();
				}
				curpos.x = curpos.x + width;
			}
			if(strcmp(n->Value(), "button") == 0)
			{
				int width = -1;
				if(n->ToElement()->Attribute("width"))
					width = atoi(n->ToElement()->Attribute("width"));
				int height = 0;
				if(n->ToElement()->Attribute("height"))
					height = atoi(n->ToElement()->Attribute("height"));

				areas.push_back(cArea(cVector2(999,999),cVector2(-999,-999)));
				cVector2 newpos = drawnode(curpos + cVector2(5,5), n->FirstChild(), lineheight, areas, color, size, false);
				cArea area = areas.back();
				areas.pop_back();
				options.push_back(pair<cArea, int>(area, atoi(n->ToElement()->Attribute("id"))));
				if(mousex > area.tl.x+parent->px()+realx() && mousex < area.br.x+parent->px()+realx() &&
					mousey > area.tl.y+(Graphics.h() - parent->py() - parent->ph())+realy() && mousey < area.br.y+(Graphics.h() - parent->py() - parent->ph())+realy())
					cursortype = 1;
				

				lineheight = max(height, lineheight);

				int xx = curpos.x;
				int yy = curpos.y-12;
				int w = width;
				int h = height;

				if(w < area.br.x - area.tl.x + 10)
					w = area.br.x - area.tl.x + 10;
				if(h < area.br.y - area.tl.y + 10)
					h = area.br.y - area.tl.y + 10;

				if(draw)
				{
					glColor3f(1,1,1);
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, parent->texture->texid());
					glBegin(GL_QUADS);
						glTexCoord2f((258.0f+4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+4, yy+h-4);
						glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);
						glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);
						glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);

						glTexCoord2f((258.0f)/512.0f,	(469.0f-4.0f)/512.0f);			glVertex2d(xx, yy+h-4);
						glTexCoord2f((258.0f+4.0f)/512.0f,		(469.0f-4.0f)/512.0f);	glVertex2d(xx+4, yy+h-4);
						glTexCoord2f((258.0f+4.0f)/512.0f,		(340.0f+4.0f)/512.0f);	glVertex2d(xx+4, yy+4);
						glTexCoord2f((258.0f)/512.0f,	(340.0f+4.0f)/512.0f);			glVertex2d(xx, yy+4);

						glTexCoord2f((387.0f-4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+w-4, yy+4);
						glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);
						glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+w, yy+h-4);
						glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+w, yy+4);

						glTexCoord2f((387.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx+w, yy+4);
						glTexCoord2f((387.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx+w, yy);
						glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy);
						glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
						
						glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-4, yy+h);
						glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);
						glTexCoord2f((258.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx, yy+h-4);
						glTexCoord2f((258.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx, yy+h);

						glTexCoord2f((258.0f)/512.0f,		(340.0f+4.0f)/512.0f);		glVertex2d(xx, yy+4);
						glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f+4.0f)/512.0f);		glVertex2d(xx+4, yy+4);
						glTexCoord2f((258.0f+4.0f)/512.0f,	(340.0f)/512.0f);			glVertex2d(xx+4, yy);
						glTexCoord2f((258.0f)/512.0f,		(340.0f)/512.0f);			glVertex2d(xx, yy);

						glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f)/512.0f);			glVertex2d(xx+w-4, yy+h);
						glTexCoord2f((387.0f)/512.0f,		(469.0f)/512.0f);			glVertex2d(xx+w, yy+h);
						glTexCoord2f((387.0f)/512.0f,		(469.0f-4.0f)/512.0f);		glVertex2d(xx+w, yy+h-4);
						glTexCoord2f((387.0f-4.0f)/512.0f,	(469.0f-4.0f)/512.0f);		glVertex2d(xx+w-4, yy+h-4);


					glEnd();
				}
				curpos = drawnode(curpos + cVector2(5,5), n->FirstChild(), lineheight, areas, color, size, draw);
				curpos.x += 10;
			}
		}
		innerwidth = max(curpos.x, innerwidth);
		innerheight = max(curpos.y, innerheight);

		for(list<cArea>::iterator ii = areas.begin(); ii != areas.end(); ii++)
		{
			cArea* a = &(*ii);
			ii->tl.x = min(ii->tl.x, curpos.x);
			ii->tl.y = min(ii->tl.y, curpos.y);
			ii->br.x = max(ii->br.x, curpos.x);
			ii->br.y = max(ii->br.y, curpos.y+lineheight);
		}

		n = n->NextSibling();
	}
	return curpos;
}


void cWindowBorfMLBox::calcinner()
{
	innerwidth = 0;
	innerheight = 0;
	options.clear();
	cVector2 curpos = cVector2(0,12);

	TiXmlNode* n = body.FirstChild()->FirstChild();

	cursortype = 0;
	int lineheight = 12;
	list<cArea> areas;
	options.clear();
	
	areas.push_back(cArea(cVector2(9999,9999), cVector2(-9999,-9999)));
	drawnode(curpos, n, lineheight, areas, cVector3(0,0,0), cVector2(1,1), false);
	innerheight = areas.back().br.y - areas.back().tl.y;
	
}



void cWindowBorfMLBox::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
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
	int ybarpos = (int)(((float)scrollposy / (float)innerheight) * (h-16 - 13));

	int xbarwidth = (int)max(((float)w/(float)innerwidth)*(w-16 - 13),18.0f);
	int xbarpos = (int)(((float)scrollposx / (float)innerwidth) * (w-16 - 13));

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
	
	innerwidth = 0;
	innerheight = 0;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);			// (X, Y, Width, Height)
	glPushMatrix();
	glLoadIdentity();
	glViewport(parent->px() + xx, parent->py() + yy, w, h);
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();
	glLoadIdentity();
	glOrtho(scrollposx,scrollposx+w, scrollposy+h, scrollposy,-10000,10000);
	glMatrixMode(GL_MODELVIEW);
	

	options.clear();
	cVector2 curpos = cVector2(0,12);

	TiXmlNode* n = body.FirstChild()->FirstChild();

	cursortype = 0;
	int lineheight = 12;
	list<cArea> areas;
	options.clear();
	drawnode(curpos, n, lineheight, areas);



	
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glColor4fv(colors);
}







void cWindowBorfMLBox::click()
{
	Log(3,0,"Lookin' for a click...");
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = (Graphics.h()-(int)mousey);
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
			for(unsigned int i = 0; i < options.size(); i++)
			{
				if(mousex > options[i].first.tl.x+parent->px()+realx() && mousex < options[i].first.br.x+parent->px()+realx() &&
					mousey > options[i].first.tl.y+(Graphics.h() - parent->py() - parent->ph())+realy() && mousey < options[i].first.br.y+(Graphics.h() - parent->py() - parent->ph())+realy())
				{
					clickoption(options[i].second, clickparam);
				}

			}
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

void cWindowBorfMLBox::drag()
{
	int xx = (int)mousex;
	xx -= realx();
	xx -= parent->px();
	int yy = (Graphics.h()-(int)mousey);
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
		return;
	}

}

string cWindowBorfMLBox::ppopup()
{
	return "";
}


cWindowObject* cWindowBorfMLBox::inobject()
{
	return cWindowObject::inobject();
}


void cWindowBorfMLBox::SetText(int id, string text)
{
	body.SetCondenseWhiteSpace(true);
	body.Parse(text.c_str());
}


void cWindowBorfMLBox::scrollup()
{
	scrollposy= scrollposy - h/2;
	scrollposy = (int)max(min(scrollposy, innerheight-h), 0);
}

void cWindowBorfMLBox::scrolldown()
{
	scrollposy = scrollposy + h/2;
	scrollposy = (int)max(min(scrollposy, innerheight-h), 0);
}