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

	int width = 200 / (wallHeight/15);

	glColor4f(1,1,1,1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(0,0); glVertex2f(50,	50);
		glTexCoord2d(0,1); glVertex2f(50,	350);
		glTexCoord2d(1,0); glVertex2f(350,	50);
		glTexCoord2d(1,1); glVertex2f(350,	350);
	glEnd();


	glDisable(GL_TEXTURE_2D);
	glColor4f(0,0,0,1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(50+300*tile.u1,	50+300*tile.v1);
		glVertex2f(50+300*tile.u2,	50+300*tile.v2);
		glVertex2f(50+300*tile.u4,	50+300*tile.v4);
		glVertex2f(50+300*tile.u3,	50+300*tile.v3);
	glEnd();
	glColor4f(1,1,1,0.25f);
	glBegin(GL_QUADS);
		glVertex2f(50+300*tile.u1,	50+300*tile.v1);
		glVertex2f(50+300*tile.u2,	50+300*tile.v2);
		glVertex2f(50+300*tile.u4,	50+300*tile.v4);
		glVertex2f(50+300*tile.u3,	50+300*tile.v3);
	glEnd();

	glColor4fv(colors);
}

void cWallTextureWindow::cWindowWallTextureBox::drag()
{
	if((cVector2(cGraphics::cMouse::xOld-parent->getX(), cGraphics::h()-cGraphics::cMouse::yOld-parent->getY()) - cVector2(realX() + 50+300*tile.u1,	realY() + 50+300*tile.v1)).Magnitude() < 20)
	{
		tile.u1 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v1 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
	}
	else if((cVector2(cGraphics::cMouse::xOld-parent->getX(), cGraphics::h()-cGraphics::cMouse::yOld-parent->getY()) - cVector2(realX() + 50+300*tile.u2,	realY() + 50+300*tile.v2)).Magnitude() < 20)
	{
		tile.u2 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v2 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
	}
	else if((cVector2(cGraphics::cMouse::xOld-parent->getX(), cGraphics::h()-cGraphics::cMouse::yOld-parent->getY()) - cVector2(realX() + 50+300*tile.u3,	realY() + 50+300*tile.v3)).Magnitude() < 20)
	{
		tile.u3 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v3 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
	}
	else if((cVector2(cGraphics::cMouse::xOld-parent->getX(), cGraphics::h()-cGraphics::cMouse::yOld-parent->getY()) - cVector2(realX() + 50+300*tile.u4,	realY() + 50+300*tile.v4)).Magnitude() < 20)
	{
		tile.u4 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v4 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
	}



	else if(SDL_GetModState() & KMOD_ALT)
	{
		cVector2 center = (cVector2(tile.u1, tile.v1) + cVector2(tile.u2, tile.v2) + cVector2(tile.u3, tile.v3) + cVector2(tile.u4, tile.v4)) / 4.0f;
		
		cVector2 v;
		v = cVector2(tile.u1, tile.v1) - center;;
		v = v * (1 + ((cGraphics::cMouse::x - cGraphics::cMouse::xOld)/50.0f));
		v = v + center;
		tile.u1 = v.x;		tile.v1 = v.y;
		
		v = cVector2(tile.u2, tile.v2) - center;
		v = v * (1 + ((cGraphics::cMouse::x - cGraphics::cMouse::xOld)/50.0f));
		v = v + center;
		tile.u2 = v.x;		tile.v2 = v.y;
		
		v = cVector2(tile.u3, tile.v3) - center;
		v = v * (1 + ((cGraphics::cMouse::x - cGraphics::cMouse::xOld)/50.0f));
		v = v + center;
		tile.u3 = v.x;		tile.v3 = v.y;
		
		v = cVector2(tile.u4, tile.v4) - center;
		v = v * (1+((cGraphics::cMouse::x - cGraphics::cMouse::xOld)/50.0f));
		v = v + center;
		tile.u4 = v.x;		tile.v4 = v.y;
	}
 	else if(SDL_GetModState() & KMOD_CTRL)
	{
		cVector2 center = (cVector2(tile.u1, tile.v1) + cVector2(tile.u2, tile.v2) + cVector2(tile.u3, tile.v3) + cVector2(tile.u4, tile.v4)) / 4.0f;

		cVector2 v;
		v = cVector2(tile.u1, tile.v1) - center;;
		v.rotate((cGraphics::cMouse::x - cGraphics::cMouse::xOld)*1.0f);
		v = v + center;
		tile.u1 = v.x;		tile.v1 = v.y;
	
		v = cVector2(tile.u2, tile.v2) - center;
		v.rotate((cGraphics::cMouse::x - cGraphics::cMouse::xOld)*1.0f);
		v = v + center;
		tile.u2 = v.x;		tile.v2 = v.y;
		
		v = cVector2(tile.u3, tile.v3) - center;
		v.rotate((cGraphics::cMouse::x - cGraphics::cMouse::xOld)*1.0f);
		v = v + center;
		tile.u3 = v.x;		tile.v3 = v.y;
		
		v = cVector2(tile.u4, tile.v4) - center;
		v.rotate((cGraphics::cMouse::x - cGraphics::cMouse::xOld)*1.0f);
		v = v + center;
		tile.u4 = v.x;		tile.v4 = v.y;
	}
	else
	{
		tile.u1 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v1 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
		
		tile.u2 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v2 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
		
		tile.u3 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v3 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
		
		tile.u4 += (cGraphics::cMouse::x - cGraphics::cMouse::xOld)/300.0f;
		tile.v4 -= (cGraphics::cMouse::y - cGraphics::cMouse::yOld)/300.0f;
		

	}
	if(SDL_GetModState() & KMOD_SHIFT)
	{
		tile.u1 = round(tile.u1*4.0f) / 4.0f;
		tile.v1 = round(tile.v1*4.0f) / 4.0f;
		
		tile.u2 = round(tile.u2*4.0f) / 4.0f;
		tile.v2 = round(tile.v2*4.0f) / 4.0f;
		
		tile.u3 = round(tile.u3*4.0f) / 4.0f;
		tile.v3 = round(tile.v3*4.0f) / 4.0f;
		
		tile.u4 = round(tile.u4*4.0f) / 4.0f;
		tile.v4 = round(tile.v4*4.0f) / 4.0f;
		
	}

	*applyTile = tile;
}