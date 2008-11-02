#include "minimapwindow.h"

#include <graphics.h>

extern cGraphics Graphics;

cMiniMapWindow::cMiniMap::cMiniMap( cWindow* parent ) : cWindowObject(parent)
{
	moveTo(0,0);
	resizeTo(Graphics.world.width,Graphics.world.height);
	alignment = ALIGN_TOPLEFT;
}

void cMiniMapWindow::cMiniMap::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	int xx, yy;
	xx = realX();
	yy = realY();
	
	glEnable(GL_TEXTURE_2D);
	
	for(int y = 0; y < Graphics.world.height; y++)
	{
		for(int x = 0; x < Graphics.world.width; x++)
		{
			if(Graphics.world.cubes[y][x].tileUp == -1)
				continue;
			
			cTile* t = &Graphics.world.tiles[Graphics.world.cubes[y][x].tileUp];
			glBindTexture(GL_TEXTURE_2D, Graphics.world.textures[t->texture]->texId());
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(t->u1, 1-t->v1);				glVertex2f(xx+x,yy+y);
			glTexCoord2f(t->u3, 1-t->v3);				glVertex2f(xx+x,yy+y+1);
			glTexCoord2f(t->u2, 1-t->v2);				glVertex2f(xx+x+1,yy+y);
			glTexCoord2f(t->u4, 1-t->v4);				glVertex2f(xx+x+1,yy+y+1);
			glEnd();
		}
	}
}

void cMiniMapWindow::cMiniMap::click()
{
	if(!inObject())
		return;
	int xx, yy;
	xx = realX()+parent->getX();
	yy = realY()+parent->getY();
	
	Graphics.camerapointer.x = -10*(mouseX - xx);
	Graphics.camerapointer.y = -10*( Graphics.world.height - ((Graphics.h() -mouseY) - yy));
}

void cMiniMapWindow::cMiniMap::drag()
{
	click();
}

cMiniMapWindow::cMiniMapWindow( ) : cWindow()
{
	windowType = WT_MINIMAP;
	resizable = false;
	visible = true;
	modal = false;
	alwaysOnTop = true;
	noTransparency = true;
	
	
	h = Graphics.world.height+skinOffBottom+skinOffTop;
	w = Graphics.world.width+skinOffLeft+skinOffRight;
	x = Graphics.w()-w;
	y = 0;
	initprops("minimap");
	h = Graphics.world.height+skinOffBottom+skinOffTop;
	w = Graphics.world.width+skinOffLeft+skinOffRight;
	title = "";
	
	
	objects["minimap"] = new cMiniMap(this);
}