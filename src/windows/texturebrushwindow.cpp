#include "texturebrushwindow.h"
#include <wm/windowscrollpanel.h>

#include <graphics.h>
#include <filesystem.h>

extern cGraphics Graphics;
extern cFileSystem fs;


cTextureBrushWindow::cWindowBrushButton::cWindowBrushButton(cWindow* parent, TiXmlDocument &skin, int brushWidth, int brushHeight, std::string brushData) : cWindowObject(parent,skin.FirstChildElement("skin")->FirstChildElement("button"))
{
	alignment = ALIGN_TOPLEFT;
	resizeTo(128,128);


	brush = std::vector<std::vector<bool> >(brushHeight, std::vector<bool>(brushWidth,false));

	int i = 0;

	for(int y = 0; y < brushHeight; y++)
	{
		for(int x = 0; x < brushWidth; x++)
		{
			if(brushData[i] == '@')
				brush[y][x] = true;
			i++;
		}
	}

}

void cTextureBrushWindow::cWindowBrushButton::draw(int,int,int,int)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);

	cWindowObject::draw();

	float sx = (getWidth()-8)  / (float)max(brush[0].size(), brush.size());
	float sy = (getHeight()-8) / (float)max(brush[0].size(), brush.size());

	int xx = realX();
	int yy = realY();
	glDisable(GL_TEXTURE_2D);
	glColor4f(0,0,0,colors[3]);
	glBegin(GL_QUADS);
	for(int yyy = 0; yyy < brush.size(); yyy++)
	{
		for(int xxx = 0; xxx < brush[yyy].size(); xxx++)
		{
			if(brush[brush.size()-1-yyy][xxx])
			{
				glVertex2f(xx + 4 + xxx*sx,		yy + 4 + yyy*sy);
				glVertex2f(xx + 4 + xxx*sx+sx,	yy + 4 + yyy*sy);
				glVertex2f(xx + 4 + xxx*sx+sx,	yy + 4 + yyy*sy+sy);
				glVertex2f(xx + 4 + xxx*sx,		yy + 4 + yyy*sy+sy);
			}
		}
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glColor4fv(colors);


}


cTextureBrushWindow::cTextureBrushWindow(cTexture* t, cFont* f, TiXmlDocument &skin) : cWindow(t,f,skin)
{
	windowType = WT_TEXTUREBRUSH;
	resizable = true;
	visible = true;
	modal = false;
	
	h = Graphics.h()-100;
	w = Graphics.w()-100;
	center();
	minWidth = 300;
	minHeight = 400;
	
	title = "Texture Brushes";
	initprops("texturebrush");

	objects["closebutton"] = new cWindowCloseButton(this, skin);

	cWindowObject* o;


	cWindowScrollPanel* scroll = new cWindowScrollPanel(this, skin);
	scroll->alignment = ALIGN_TOPLEFT;
	scroll->moveTo(0,300);
	scroll->innerheight = 1000;
	scroll->innerwidth = 100;

	
	objects["presets"] = scroll;
		

	TiXmlDocument brushes = fs.getXml("data/brushes.xml");
	TiXmlElement* brush = brushes.FirstChildElement("brushes")->FirstChildElement("brush");
	while(brush != NULL)
	{
		o = new cWindowBrushButton(this,skin, atoi(brush->Attribute("width")), atoi(brush->Attribute("height")), brush->FirstChild()->Value());
		o->setPopup(brush->Attribute("name"));
		scroll->objects.push_back(o);
		brush = brush->NextSiblingElement("brush");
	}


	resizeTo(w,h);
}


void cTextureBrushWindow::resizeTo(int xx, int yy)
{
	cWindow::resizeTo(xx,yy);

	cWindowScrollPanel* panel = (cWindowScrollPanel*)objects["presets"];
	panel->resizeTo(innerWidth(), innerHeight()-300);
	panel->innerwidth = innerWidth();

	int x = 2;
	int y = 10;
	for(int i = 0; i < panel->objects.size(); i++)
	{
		panel->objects[i]->moveTo(x,y);
		x+=132;
		if(x+132 > panel->getWidth()-panel->skinBarWidth)
		{
			x = 2;
			y+=132;
		}
	}

}