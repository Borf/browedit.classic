#include "minimapwindow.h"

#include <graphics.h>


cMiniMapWindow::cMiniMap::cMiniMap( cWindow* parent ) : cWindowObject(parent)
{
	moveTo(0,0);
	resizeTo(cGraphics::world->width,cGraphics::world->height);
	alignment = ALIGN_TOPLEFT;
}

void cMiniMapWindow::cMiniMap::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	int xx, yy;
	xx = realX();
	yy = realY();
	
	glEnable(GL_TEXTURE_2D);
	
	for(int y = 0; y < cGraphics::world->height; y++)
	{
		for(int x = 0; x < cGraphics::world->width; x++)
		{
			if(cGraphics::world->cubes[y][x].tileUp == -1)
				continue;
			
			cTile* t = &cGraphics::world->tiles[cGraphics::world->cubes[y][x].tileUp];
			glBindTexture(GL_TEXTURE_2D, cGraphics::world->textures[t->texture]->texId());
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(t->u1, 1-t->v1);				glVertex2f(xx+x,yy+y);
			glTexCoord2f(t->u3, 1-t->v3);				glVertex2f(xx+x,yy+y+1);
			glTexCoord2f(t->u2, 1-t->v2);				glVertex2f(xx+x+1,yy+y);
			glTexCoord2f(t->u4, 1-t->v4);				glVertex2f(xx+x+1,yy+y+1);
			glEnd();
		}
	}
}

void cMiniMapWindow::cMiniMap::onClick()
{
	if(!inObject())
		return;
	int xx, yy;
	xx = realX()+parent->getX();
	yy = realY()+parent->getY();
	
	cGraphics::worldContainer->camera.pointer.x = -10*(cGraphics::cMouse::x - xx);
	cGraphics::worldContainer->camera.pointer.y = -10*( cGraphics::world->height - ((cGraphics::h() -cGraphics::cMouse::y) - yy));
}

void cMiniMapWindow::cMiniMap::drag()
{
	onClick();
}

cMiniMapWindow::cMiniMapWindow( ) : cWindow()
{
	windowType = WT_MINIMAP;
	resizable = false;
	visible = true;
	modal = false;
	alwaysOnTop = true;
	noTransparency = true;
	
	
	h = cGraphics::world->height+skinOffBottom+skinOffTop;
	w = cGraphics::world->width+skinOffLeft+skinOffRight;
	x = cGraphics::w()-w;
	y = 0;
	initProps("minimap");
	h = cGraphics::world->height+skinOffBottom+skinOffTop;
	w = cGraphics::world->width+skinOffLeft+skinOffRight;
	title = "";
	
	
	objects["minimap"] = new cMiniMap(this);
}
