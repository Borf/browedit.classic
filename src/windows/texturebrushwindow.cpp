#include <common.h>
#include "texturebrushwindow.h"
#include <wm/windowscrollpanel.h>

#include <graphics.h>
#include <bengine/util/filesystem.h>



cTextureBrushWindow::cWindowBrushButton::cWindowBrushButton(cWindow* parent, int brushWidth, int brushHeight, std::string brushData, TiXmlDocument* skin) : cWindowObject(parent,skin->FirstChildElement("skin")->FirstChildElement("button"))
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
	for(unsigned int yyy = 0; yyy < brush.size(); yyy++)
	{
		for(unsigned int xxx = 0; xxx < brush[yyy].size(); xxx++)
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

void cTextureBrushWindow::cWindowBrushButton::onClick()
{
	int oldWidth =  ((cTextureBrushWindow*)parent)->brushWidth;
	int oldHeight = ((cTextureBrushWindow*)parent)->brushHeight;

	int x,y;
	for(y = 0; y < oldHeight; y++)
	{
		for(x = 0; x < oldWidth; x++)
		{
			char buf[16];
			sprintf(buf, "tile%i,%i", x,y);
			delete parent->objects[buf];
			parent->objects.erase(parent->objects.find(buf));
		}
	}

	for(y = 0; y < (int)brush.size(); y++)
	{
		for(x = 0; x < (int)brush[y].size(); x++)
		{
			cWindowBrushTile* tile = new cWindowBrushTile(parent);
			tile->moveTo(64*x,64*y);
			tile->resizeTo(64,64);
			tile->on = brush[y][x];
			char buf[16];
			sprintf(buf, "tile%i,%i", x,y);
			parent->objects[buf] = tile;
		}
	}

	((cTextureBrushWindow*)parent)->brushWidth  = brush[0].size();;
	((cTextureBrushWindow*)parent)->brushHeight = brush.size();

	parent->resizeTo(parent->getWidth(), parent->getHeight());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


cTextureBrushWindow::cWindowBrushTile::cWindowBrushTile( cWindow* parent, TiXmlDocument* skin ) : cWindowObject(parent,skin->FirstChildElement("skin")->FirstChildElement("button"))
{
	alignment = ALIGN_TOPLEFT;
	resizeTo(128,128);
	on = false;	
}

void cTextureBrushWindow::cWindowBrushTile::draw( int,int,int,int )
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	
	cWindowObject::draw();
		
	if(on)
	{
		int xx = realX();
		int yy = realY();
		glDisable(GL_TEXTURE_2D);
		glColor4f(0,0,0,colors[3]);
		glBegin(GL_QUADS);
			glVertex2f(xx + 1,			yy + 1);
			glVertex2f(xx + 1 + w-2,	yy + 1);
			glVertex2f(xx + 1 + w-2,	yy + 1 + h-2);
			glVertex2f(xx + 1,			yy + 1 + h-2);
		glEnd();
		glEnable(GL_TEXTURE_2D);
	}	
	glColor4fv(colors);
}

void cTextureBrushWindow::cWindowBrushTile::onClick()
{
	on = !on;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


cTextureBrushWindow::cWindowBrushChangeButton::cWindowBrushChangeButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	text = "Change";
	resizeTo(140,h);
}

void cTextureBrushWindow::cWindowBrushChangeButton::onClick()
{
	int width = atoi(parent->objects["inpWidth"]->getText(0).c_str());
	int height = atoi(parent->objects["inpHeight"]->getText(0).c_str());

	int oldWidth =  ((cTextureBrushWindow*)parent)->brushWidth;
	int oldHeight = ((cTextureBrushWindow*)parent)->brushHeight;

	if(oldWidth < width)
	{
		for(int x = oldWidth; x < width; x++)
		{
			for(int y = 0; y < oldHeight; y++)
			{
				cWindowBrushTile* tile = new cWindowBrushTile(parent);
				tile->moveTo(64*x,64*y);
				tile->resizeTo(64,64);
				char buf[16];
				sprintf(buf, "tile%i,%i", x,y);
				parent->objects[buf] = tile;

			}
		}
	}
	else if(oldWidth > width)
	{
		for(int x = width; x < oldWidth; x++)
		{
			for(int y = 0; y < oldHeight; y++)
			{
				char buf[16];
				sprintf(buf, "tile%i,%i", x,y);
				delete parent->objects[buf];
				parent->objects.erase(parent->objects.find(buf));
				
			}
		}
	}

	if(oldHeight < height)
	{
		for(int y = oldHeight; y < height; y++)
		{
			for(int x = 0; x < width; x++)
			{
				cWindowBrushTile* tile = new cWindowBrushTile(parent);
				tile->moveTo(64*x,64*y);
				tile->resizeTo(64,64);
				char buf[16];
				sprintf(buf, "tile%i,%i", x,y);
				parent->objects[buf] = tile;
			}
		}
	}
	else if (oldHeight > height)
	{
		for(int y = height; y < oldHeight; y++)
		{
			for(int x = 0; x < width; x++)
			{
				char buf[16];
				sprintf(buf, "tile%i,%i", x,y);
				delete parent->objects[buf];
				parent->objects.erase(parent->objects.find(buf));
			}
		}
	}
	((cTextureBrushWindow*)parent)->brushWidth = width;
	((cTextureBrushWindow*)parent)->brushHeight = height;

	parent->resizeTo(parent->getWidth(), parent->getHeight());

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


cTextureBrushWindow::cWindowBrushOkButton::cWindowBrushOkButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	text = "Ok";
	resizeTo(140,h);	
}

void cTextureBrushWindow::cWindowBrushOkButton::onClick()
{
	cGraphics::textureBrush.clear();

	int width =  ((cTextureBrushWindow*)parent)->brushWidth;
	int height = ((cTextureBrushWindow*)parent)->brushHeight;

	cGraphics::textureBrush = std::vector<std::vector<bool> >(height, std::vector<bool>(width, false) );

	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			char buf[16];
			sprintf(buf, "tile%i,%i", x,y);
			cGraphics::textureBrush[y][x] = ((cWindowBrushTile*)parent->objects[buf])->on;
		}
	}
	parent->close();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



cTextureBrushWindow::cWindowBrushSaveButton::cWindowBrushSaveButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	resizeTo(140,h);
	text = "Save";
}

void cTextureBrushWindow::cWindowBrushSaveButton::onClick()
{
	std::string name = cWM::inputWindow("Please enter a new name");
	if(name == "")
		return;

	TiXmlDocument brushes;//TODObengine = cFileSystem::getXml("data/brushes.xml");

	TiXmlElement newBrush("brush");
	newBrush.SetAttribute("name", name.c_str());
	newBrush.SetAttribute("width", ((cTextureBrushWindow*)parent)->brushWidth);
	newBrush.SetAttribute("height", ((cTextureBrushWindow*)parent)->brushHeight);

	std::string data = "";
	for(int y = 0; y < ((cTextureBrushWindow*)parent)->brushHeight; y++)
	{
		for(int x = 0; x < ((cTextureBrushWindow*)parent)->brushWidth; x++)
		{
			char buf[16];
			sprintf(buf, "tile%i,%i", x,y);
			data += ((cWindowBrushTile*)parent->objects[buf])->on ? "@" : ".";
		}
	}
	newBrush.InsertEndChild(TiXmlText(data.c_str()));
	brushes.FirstChildElement("brushes")->InsertEndChild(newBrush);

	brushes.SaveFile("data/brushes.xml");
	cWindowScrollPanel* panel = (cWindowScrollPanel*)parent->objects["presets"];
	cWindowObject* o = new cWindowBrushButton(parent,((cTextureBrushWindow*)parent)->brushWidth, ((cTextureBrushWindow*)parent)->brushHeight, data);
	o->setPopup(name);
	panel->objects.push_back(o);
	panel->resizeTo(panel->getWidth(), panel->getHeight());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



cTextureBrushWindow::cWindowBrushRotateButton::cWindowBrushRotateButton( cWindow* parent, TiXmlDocument* skin ) : cWindowButton(parent,skin)
{
	alignment = ALIGN_TOPLEFT;
	text = "Rotate";
	resizeTo(140,h);
}

void cTextureBrushWindow::cWindowBrushRotateButton::onClick()
{
	int newHeight = ((cTextureBrushWindow*)parent)->brushWidth;
	int newWidth = ((cTextureBrushWindow*)parent)->brushHeight;
	std::vector<std::vector<bool> > brush(newHeight, std::vector<bool>(newWidth, false) );

	char buf[32];
	int y,x;
	for(y = 0; y < newHeight; y++)
	{
		for(x = 0; x < newWidth; x++)
		{
			sprintf(buf, "tile%i,%i", newHeight-1-y,x);
			brush[y][x] = ((cWindowBrushTile*)parent->objects[buf])->on;
			delete parent->objects[buf];
			parent->objects.erase(parent->objects.find(buf));
		}
	}
	((cTextureBrushWindow*)parent)->brushWidth = newWidth;
	((cTextureBrushWindow*)parent)->brushHeight = newHeight;

	for(y = 0; y < newHeight; y++)
	{
		for(x = 0; x < newWidth; x++)
		{
			cWindowBrushTile* tile = new cWindowBrushTile(parent);
			tile->on = brush[y][x];
			char buf[16];
			sprintf(buf, "tile%i,%i", x,y);
			parent->objects[buf] = tile;
		}
	}
	parent->resizeTo(parent->getWidth(), parent->getHeight());
}

///////////////////////////////////////////////////////////////end componends

cTextureBrushWindow::cTextureBrushWindow() : cWindow()
{
	windowType = WT_TEXTUREBRUSH;
	resizable = true;
	visible = true;
	modal = false;
	
	h = cGraphics::h()-100;
	w = cGraphics::w()-100;
	center();
	minWidth = 300;
	minHeight = 400;
	
	title = "Texture Brushes";
	initProps("texturebrush");

	objects["closebutton"] = new cWindowCloseButton(this);

	cWindowObject* o;


	cWindowScrollPanel* scroll = new cWindowScrollPanel(this);
	scroll->alignment = ALIGN_TOPLEFT;
	scroll->moveTo(0,300);
	scroll->innerheight = 1000;
	scroll->innerwidth = 100;

	
	objects["presets"] = scroll;
		

	TiXmlDocument brushes;//TODObengine = cFileSystem::getXml("data/brushes.xml");
	TiXmlElement* brushEl = brushes.FirstChildElement("brushes")->FirstChildElement("brush");
	while(brushEl != NULL)
	{
		o = new cWindowBrushButton(this,atoi(brushEl->Attribute("width")), atoi(brushEl->Attribute("height")), brushEl->FirstChild()->Value());
		o->setPopup(brushEl->Attribute("name"));
		scroll->objects.push_back(o);
		brushEl = brushEl->NextSiblingElement("brush");
	}


	cWindowBrushTile* tile;
	
	brushWidth = cGraphics::textureBrush[0].size();
	brushHeight = cGraphics::textureBrush.size();
	
	for(unsigned int y = 0; y < cGraphics::textureBrush.size(); y++)
	{
		for(unsigned int x = 0; x < cGraphics::textureBrush[y].size(); x++)
		{
			tile = new cWindowBrushTile(this);
			tile->moveTo(64*x,64*y);
			tile->resizeTo(64,64);
			tile->on = cGraphics::textureBrush[y][x];
			char buf[16];
			sprintf(buf, "tile%i,%i", x,y);
			objects[buf] = tile;
		}
	}

	char buf[10];

	addLabel("lblWidth", 0,0,"Width");
	sprintf(buf, "%i", brushWidth);
	addInputBox("inpWidth", 140,140,140,buf);

	addLabel("lblHeight", 0,50,"Height");
	sprintf(buf, "%i", brushHeight);
	addInputBox("inpHeight", 140,140,140,buf);

	objects["btnChange"] = new cWindowBrushChangeButton(this);
	objects["btnOk"] = new cWindowBrushOkButton(this);
	objects["btnSave"] = new cWindowBrushSaveButton(this);
	objects["btnRotate"] = new cWindowBrushRotateButton(this);
	
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
	for(unsigned int i = 0; i < panel->objects.size(); i++)
	{
		panel->objects[i]->moveTo(x,y);
		x+=132;
		if(x+132 > panel->getWidth()-panel->skinBarWidth)
		{
			x = 2;
			y+=132;
		}
	}

	int tileSize = min((innerWidth()-150) / brushWidth, 300 / brushHeight);

	for(y = 0; y < brushHeight; y++)
	{
		for(x = 0; x < brushWidth; x++)
		{
			char buf[16];
			sprintf(buf, "tile%i,%i", x,y);
			objects[buf]->resizeTo(tileSize, tileSize);
			objects[buf]->moveTo(x*tileSize, y*tileSize);
		}
	}
	objects["lblWidth"]->moveTo(innerWidth()-140, 0);
	objects["inpWidth"]->moveTo(innerWidth()-140, 15);

	objects["lblHeight"]->moveTo(innerWidth()-140, 30);
	objects["inpHeight"]->moveTo(innerWidth()-140, 45);

	objects["btnChange"]->moveTo(innerWidth()-140,70);
	objects["btnOk"]->moveTo(innerWidth()-140,100);
	objects["btnSave"]->moveTo(innerWidth()-140,150);
	objects["btnRotate"]->moveTo(innerWidth()-140,180);
}
