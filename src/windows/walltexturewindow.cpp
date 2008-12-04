#include <common.h>
#include "walltexturewindow.h"
#include <graphics.h>

cWallTextureWindow::cWallTextureWindow(int tileX, int tileY, bool horizontal ) : cWindow()
{
	windowType = WT_WALLTEXTURE;
	resizable = false;
	visible = true;
	modal = false;
	
	h = 400+skinOffTop+skinOffBottom;
	w = 500+skinOffLeft+skinOffRight;
	title = "Walltexture";
	center();


	objects["texture"] = new cWindowWallTextureBox(tileX, tileY, horizontal, this);

	objects["btnClose"] = new cWindowCloseButton(this);
}

cWallTextureWindow::cWindowWallTextureBox::cWindowWallTextureBox( int tileX, int tileY, bool horizontal, cWindow* parent, TiXmlDocument* skin) : cWindowObject(parent, skin->FirstChildElement("skin")->FirstChildElement("frame"))
{
	alignment = ALIGN_TOPLEFT;
	moveTo(0,0);
	resizeTo(400,400);

	if(horizontal)
	{
		if(cGraphics::world->cubes[tileY][tileX].tileSide != -1)
		{
			applyTile = &cGraphics::world->tiles[cGraphics::world->cubes[tileY][tileX].tileSide];
			tile = cGraphics::world->tiles[cGraphics::world->cubes[tileY][tileX].tileSide];
			wallHeight = (cGraphics::world->cubes[tileY][tileX].cell3	+ cGraphics::world->cubes[tileY][tileX].cell4) -
				(cGraphics::world->cubes[tileY+1][tileX].cell1 + cGraphics::world->cubes[tileY+1][tileX].cell2);
		}
		else
			Log(2,0,"uhoh");
	}
	else
	{
		if(cGraphics::world->cubes[tileY][tileX].tileOtherSide != -1)
		{
			applyTile = &cGraphics::world->tiles[cGraphics::world->cubes[tileY][tileX].tileOtherSide];
			tile = cGraphics::world->tiles[cGraphics::world->cubes[tileY][tileX].tileOtherSide];
			wallHeight = (cGraphics::world->cubes[tileY][tileX].cell4	+ cGraphics::world->cubes[tileY][tileX].cell2) -
				(cGraphics::world->cubes[tileY][tileX+1].cell1 + cGraphics::world->cubes[tileY][tileX+1].cell3);
		}
		else
			Log(2,0,"uhoh");
	}

}

void cWallTextureWindow::cWindowWallTextureBox::draw( int a,int b,int c,int d)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	cWindowObject::draw(a,b,c,d);
	glTranslatef(realX(), realY(),0);
	
	glBindTexture(GL_TEXTURE_2D, cGraphics::world->textures[tile.texture]->texture->texId());

	int width = 200 / (wallHeight/20);

/*	glColor4f(1,1,1,1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(tile.u1,tile.v1); glVertex2f(100,			300);
		glTexCoord2d(tile.u2,tile.v2); glVertex2f(100+width,	300);
		glTexCoord2d(tile.u3,tile.v3); glVertex2f(100,			100);
		glTexCoord2d(tile.u4,tile.v4); glVertex2f(100+width,	100);
	glEnd();*/

	//glColor4f(1,1,1,0.5f);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0,0); glVertex2f(100-width*(tile.u1/(tile.u2-tile.u1)),		100+200*((tile.v3)/(tile.v1-tile.v3)));
	glTexCoord2d(1,0); glVertex2f(100+width*(1+(1-tile.u2)/(tile.u2-tile.u1)),	100+200*((tile.v4)/(tile.v2-tile.v4)));
	glTexCoord2d(0,1); glVertex2f(100-width*(tile.u3/(tile.u4-tile.u3)),		100+200*((tile.v1)/(tile.v1-tile.v3)));
	glTexCoord2d(1,1); glVertex2f(100+width*(1+(1-tile.u4)/(tile.u4-tile.u3)),	100+200*((tile.v2)/(tile.v2-tile.v4)));
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glColor4f(0,0,0,1);
	glBegin(GL_LINE_LOOP);
		glTexCoord2d(0,0); glVertex2f(100,	100);
		glTexCoord2d(0,1); glVertex2f(100,	300);
		glTexCoord2d(1,1); glVertex2f(100+width,	300);
		glTexCoord2d(1,0); glVertex2f(100+width,	100);
	glEnd();

	glColor4fv(colors);
}

void cWallTextureWindow::cWindowWallTextureBox::drag()
{
	if(SDL_GetModState() & KMOD_ALT)
	{
		tile.u2 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/128.0f;
		tile.u4 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/128.0f;

		tile.v1 += (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/128.0f;
		tile.v2 += (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/128.0f;
	}
	else
	{
		tile.u1 -= (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/128.0f;
		tile.v1 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/128.0f;
		
		tile.u2 -= (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/128.0f;
		tile.v2 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/128.0f;
		
		tile.u3 -= (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/128.0f;
		tile.v3 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/128.0f;
		
		tile.u4 -= (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/128.0f;
		tile.v4 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/128.0f;
	}

	*applyTile = tile;
}