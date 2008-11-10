#include <common.h>
#include "clipboard.h"
#include <graphics.h>
#include <undo.h>
#include "settings.h"

extern int brushsize;


cClipBoardContents* cClipBoard::currentClipBoard = NULL;
bool				cClipBoard::pasting = false;

cClipBoardContents::cClipBoardContents( cClipBoard::eClipBoardType t)
{
	type = t;
	worldContainer = cGraphics::worldContainer;
}

void cClipBoard::setClipBoard( cClipBoardContents* clipboard)
{
	if(currentClipBoard)
		delete currentClipBoard;
	currentClipBoard = clipboard;
}

void cClipboardTexture::apply()
{
	if(cGraphics::cMouse::x < cGraphics::w()-256)
	{
		float selsizex = (fabs(cGraphics::worldContainer->settings.selectionstart.x - cGraphics::worldContainer->settings.selectionend.x) / 32);
		float selsizey = (fabs(cGraphics::worldContainer->settings.selectionstart.y - cGraphics::worldContainer->settings.selectionend.y) / 32);
		
		selsizex = floor(selsizex*cGraphics::worldContainer->settings.brushsize);
		selsizey = floor(selsizey*cGraphics::worldContainer->settings.brushsize);
		
		int posx = (int)cGraphics::cMouse::x3d / 10;
		int posy = (int)cGraphics::cMouse::z3d / 10;
		
		if (cGraphics::worldContainer->settings.textureRot % 2 == 1)
		{
			float a = selsizex;
			selsizex = selsizey;
			selsizey = a;
		}
		if(data.size() != selsizex || selsizex == 0)
			return;
		if(data[0].size() != selsizey)
			return;
		
		
		cGraphics::worldContainer->undoStack->push(new cUndoTexture(posx-(int)selsizex+1, posy-(int)selsizey+1, posx+1, posy+1));
		
		for(int x = posx; x > posx-selsizex; x--)
		{
			for(int y = posy; y > posy-selsizey; y--)
			{
				if(x < 0 || y < 0)
					continue;
				int xx = posx - x;
				int yy = posy - y;
				if(data[xx][yy].first == 2)
				{
					cGraphics::world->cubes[y][x].tileUp = -1;
				}
				if(data[xx][yy].first == 1)
				{
					int oldTexId = data[xx][yy].second.texture;
					if(worldContainer != cGraphics::worldContainer)
					{
						cTile* tile = &data[xx][yy].second;
						cTextureContainer* texture = worldContainer->world->textures[tile->texture];
						int found = -1;
						for(unsigned int i = 0; i < cGraphics::world->textures.size() && found == -1; i++)
						{
							if(cGraphics::world->textures[i]->RoFilename == texture->RoFilename)
								found = i;
						}
						if(found == -1)
						{
							cTextureContainer* t = new cTextureContainer();
							t->RoFilename = texture->RoFilename;
							t->RoFilename2 = texture->RoFilename2;
							t->texture = cTextureCache::load(cSettings::roDir + "data\\texture\\" + texture->RoFilename);
							found = cGraphics::world->textures.size();
							cGraphics::world->textures.push_back(t);

						}
						data[xx][yy].second.texture = found;
					}
					cGraphics::world->tiles.push_back(data[xx][yy].second);
					cGraphics::world->cubes[y][x].tileUp = cGraphics::world->tiles.size()-1;
					data[xx][yy].second.texture = oldTexId;
				}
			}
		}
	}	
}

void cClipboardTexture::render()
{
	int posx = (int)cGraphics::cMouse::x3d / 10;
	int posy = (int)cGraphics::cMouse::z3d / 10;
	unsigned int x,y;

	for(x = 0; x < data.size(); x++)
	{
		for(y = 0; y < data[x].size(); y++)
		{
			if(data[x][y].first != 1)
				continue;
			cTile t = data[x][y].second;
			glBindTexture(GL_TEXTURE_2D, worldContainer->world->textures[t.texture]->texId());
			cCube* c = &cGraphics::world->cubes[y][x];
			glColor4f(1,1,1,0.8f);
			glBegin(GL_QUADS);
			glTexCoord2f(t.u1, 1-t.v1); glVertex3f((posx-x)*10,		-c->cell1+0.1,(cGraphics::world->height-(posy-y))*10);
			glTexCoord2f(t.u2, 1-t.v2); glVertex3f((posx-x)*10+10,	-c->cell2+0.1,(cGraphics::world->height-(posy-y))*10);
			glTexCoord2f(t.u4, 1-t.v4); glVertex3f((posx-x)*10+10,	-c->cell4+0.1,(cGraphics::world->height-(posy-y))*10-10);
			glTexCoord2f(t.u3, 1-t.v3); glVertex3f((posx-x)*10,		-c->cell3+0.1,(cGraphics::world->height-(posy-y))*10-10);
			glEnd();
			
		}
	}
	
}
void cClipboardHeight::apply()
{
	int posx = (int)cGraphics::cMouse::x3d / 10;
	int posy = (int)cGraphics::cMouse::z3d / 10;
	if ((int)data.size() != brushsize)
		return;
	
	cGraphics::worldContainer->undoStack->push(new cUndoHeightEdit(posx-(int)floor(brushsize/2.0f), posy-(int)floor(brushsize/2.0f), posx+(int)ceil(brushsize/2.0f), posy+(int)ceil(brushsize/2.0f)));
	//					if (posx >= (int)floor(brushsize/2.0f) && posx <= cGraphics::world->width-(int)ceil(brushsize/2.0f) && posy >= (int)floor(brushsize/2.0f) && posy <= cGraphics::world->height-(int)ceil(brushsize/2.0f))
	{
		int yy = 0;
		for(int y = posy-(int)floor(brushsize/2.0f); y < posy+(int)ceil(brushsize/2.0f); y++)
		{
			std::vector<std::vector<float> > row;
			int xx = 0;
			for(int x = posx-(int)floor(brushsize/2.0f); x < posx+(int)ceil(brushsize/2.0f); x++)
			{
				if (x >= cGraphics::world->width || x < 0 || y < 0 || y >= cGraphics::world->height)
					continue;
				cGraphics::world->cubes[y][x].cell1 = data[yy][xx][0];
				cGraphics::world->cubes[y][x].cell2 = data[yy][xx][1];
				cGraphics::world->cubes[y][x].cell3 = data[yy][xx][2];
				cGraphics::world->cubes[y][x].cell4 = data[yy][xx][3];
				cGraphics::world->cubes[y][x].calcNormal();
				xx++;
			}
			yy++;
		}
	}
}

void cClipboardHeight::render()
{
	int posx = (int)cGraphics::cMouse::x3d / 10;
	int posy = (int)cGraphics::cMouse::z3d / 10;

	posx = posx + ceil(data[0].size()/2.0f)-1;
	posy = posy + ceil(data[0].size()/2.0f)-1;

	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	for(unsigned int y = 0; y < data.size(); y++)
	{
		for(unsigned int x = 0; x <data[y].size(); x++)
		{
			if (posx-(int)x >= cGraphics::world->width || posx-(int)x < 0 || posy-(int)y < 0 || posy-(int)y >= cGraphics::world->height)
				continue;
			
			cCube* c = &cGraphics::world->cubes[posy-y][posx-x];
			if(c->tileUp == -1)
				continue;
			
			cTile* t = &cGraphics::world->tiles[c->tileUp];
			
			glBindTexture(GL_TEXTURE_2D, cGraphics::world->textures[t->texture]->texId());
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(t->u1, 1-t->v1);				glVertex3f((posx-x)*10,		-data[data.size()-1-y][data[y].size()-1-x][0],(cGraphics::world->height-(posy-y))*10);
			glTexCoord2f(t->u3, 1-t->v3);				glVertex3f((posx-x)*10,		-data[data.size()-1-y][data[y].size()-1-x][2],(cGraphics::world->height-(posy-y))*10-10);
			glTexCoord2f(t->u2, 1-t->v2);				glVertex3f((posx-x)*10+10,			-data[data.size()-1-y][data[y].size()-1-x][1],(cGraphics::world->height-(posy-y))*10);
			glTexCoord2f(t->u4, 1-t->v4);				glVertex3f((posx-x)*10+10,			-data[data.size()-1-y][data[y].size()-1-x][3],(cGraphics::world->height-(posy-y))*10-10);
			glEnd();
		}
	}


}

void cClipboardGat::apply()
{
	int posx = (int)cGraphics::cMouse::x3d / 5;
	int posy = (int)cGraphics::cMouse::z3d / 5;
	
	int f = data.size();
		
	cGraphics::worldContainer->undoStack->push(new cUndoGatHeightEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
	cGraphics::worldContainer->undoStack->push(new cUndoGatTileEdit(posx-(int)floor(f/2.0f), posy-(int)floor(f/2.0f), posx+(int)ceil(f/2.0f), posy+(int)ceil(f/2.0f)));
	
	if (posx >= (int)floor(f/2.0f) && posx < 2*cGraphics::world->width-(int)ceil(f/2.0f) && posy >= f && posy< 2*cGraphics::world->height-f)
	{
		int yy = 0;
		for(int y = posy-(int)floor(f/2.0f); y < posy+(int)ceil(f/2.0f); y++)
		{
			int xx = 0;
			for(int x = posx-(int)floor(f/2.0f); x < posx+(int)ceil(f/2.0f); x++)
			{
				cGraphics::world->gattiles[y][x].cell1 = data[yy][xx].second[0];
				cGraphics::world->gattiles[y][x].cell2 = data[yy][xx].second[1];
				cGraphics::world->gattiles[y][x].cell3 = data[yy][xx].second[2];
				cGraphics::world->gattiles[y][x].cell4 = data[yy][xx].second[3];
				cGraphics::world->gattiles[y][x].type = data[yy][xx].first;
				xx++;
			}
			yy++;
		}
	}
	
}

void cClipboardGat::render()
{
	int posx = (int)cGraphics::cMouse::x3d / 5;
	int posy = (int)cGraphics::cMouse::z3d / 5;
	
	posx = posx + ceil(data[0].size()/2.0f)-1;
	posy = posy + ceil(data[0].size()/2.0f)-1;
	
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	for(unsigned int y = 0; y < data.size(); y++)
	{
		for(unsigned int x = 0; x <data[y].size(); x++)
		{
			if (posx-(int)x >= cGraphics::world->width*2 || posx-(int)x < 0 || posy-(int)y < 0 || posy-(int)y >= cGraphics::world->height*2)
				continue;
						

			std::vector<float> &tile = data[data.size()-1-y][data[y].size()-1-x].second;
			glBindTexture(GL_TEXTURE_2D, cGraphics::gatTextures[data[data.size()-1-y][data[y].size()-1-x].first]->texId());
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0,0); glVertex3f((posx-x)*5,	-tile[0]+0.1,	(2*cGraphics::world->height-(posy-y))*5);
			glTexCoord2f(1,0); glVertex3f((posx-x)*5+5,	-tile[1]+0.1,	(2*cGraphics::world->height-(posy-y))*5);
			glTexCoord2f(0,1); glVertex3f((posx-x)*5,	-tile[2]+0.1,	(2*cGraphics::world->height-(posy-y))*5-5);
			glTexCoord2f(1,1); glVertex3f((posx-x)*5+5,	-tile[3]+0.1,	(2*cGraphics::world->height-(posy-y))*5-5);
			glEnd();
		}
	}

}



void cClipboardObject::apply()
{
	if (clipboardFile != "")
	{
		cRSMModel* model = new cRSMModel();
		model->load(clipboardFile);
		if(usePos)
			model->pos = pos+pos2;
		else
			model->pos = cVector3(cGraphics::cMouse::x3d/5.0f, -cGraphics::cMouse::y3d, cGraphics::cMouse::z3d/5.0f);
		if (SDL_GetModState() & KMOD_SHIFT)
			model->pos.y = clipboardY;
		model->scale = clipboardScale;
		model->rot = clipboardRot;
		model->lightopacity = clipboardFloat;
		
		model->name = clipboardName;
		if(model->name != "")
		{
			int i = model->name.length()-1;
			while((atoi(model->name.substr(i).c_str()) != 0 || model->name.substr(i,1) == "0") && i > 0)
				i--;
			
			char buf[100];
			int newid = atoi(model->name.substr(i+1).c_str());
			
			bool found = true;
			
			while(found)
			{
				newid++;
				found = false;
				sprintf(buf, "%s%i", model->name.substr(0,i+1).c_str(), newid);
				
				for(unsigned int ii = 0; ii < cGraphics::world->models.size() && !found; ii++)
				{
					if(cGraphics::world->models[ii]->name == buf)
						found = true;
				}
			}
			model->name = buf;
		}
		
		cGraphics::world->models.push_back(model);
		cGraphics::worldContainer->settings.selectedObject = cGraphics::world->models.size()-1;
		cGraphics::worldContainer->undoStack->push(new cUndoNewObject());
	}
	
}

void cClipboardObject::render()
{
	if(!rsmmodel)
	{
		rsmmodel = new cRSMModel();
		rsmmodel->load(clipboardFile);
		rsmmodel->pos = cVector3(cGraphics::cMouse::x3d/5.0f, -cGraphics::cMouse::y3d, cGraphics::cMouse::z3d/5.0f);
		if (SDL_GetModState() & KMOD_SHIFT)
			rsmmodel->pos.y = clipboardY;
		rsmmodel->scale = clipboardScale;
		rsmmodel->rot = clipboardRot;
		rsmmodel->lightopacity = clipboardFloat;
		rsmmodel->name = "clipboard";
	}
	if(usePos)
		rsmmodel->pos = pos+pos2;
	else
		rsmmodel->pos = cVector3(cGraphics::cMouse::x3d/5.0f, -cGraphics::cMouse::y3d, cGraphics::cMouse::z3d/5.0f);
	rsmmodel->draw();
}

cClipboardObject::~cClipboardObject()
{
	if(rsmmodel)
		delete rsmmodel;
}

cClipBoardArea::cClipBoardArea(bool pTextures, bool pHeight, bool pObjects, bool pGat, bool pWalls, bool pLights, bool pEffects, bool pSounds) : cClipBoardContents(cClipBoard::CLIP_AREA)
{
	doTextures = pTextures;
	doHeight = pHeight;
	doObjects = pObjects;
	doGat = pGat;
	doWalls = pWalls;
	doLights = pLights;
	doEffects = pEffects;
	doSounds = pSounds;

	startX = round(cGraphics::cMouse::x3d/10.0f);
	startZ = round(cGraphics::cMouse::z3d/10.0f);

	int x,y;
	unsigned int i;

	int minx = 9999, miny = 9999;

	for(y = 0; y < cGraphics::world->height; y++)
	{
		for(x = 0; x < cGraphics::world->width; x++)
		{
			if(cGraphics::world->cubes[y][x].selected)
			{
				if(x < minx)
					minx = x;
				if(y < miny)
					miny = y;
				cContainer container;
				container.cube = cGraphics::world->cubes[y][x];
				container.x = x;
				container.y = y;
				container.gat = cGraphics::world->gattiles[y][x];
				tiles.push_back(container);
			}
		}
	}
	for(i = 0; i < cGraphics::world->models.size(); i++)
	{
		if(cGraphics::world->models[i]->pos.x > 0 && cGraphics::world->models[i]->pos.z > 0)
		{
			x = round(cGraphics::world->models[i]->pos.x / 2.0f);
			y = round(cGraphics::world->models[i]->pos.z / 2.0f);
			if(x < cGraphics::world->width && y < cGraphics::world->height)
			{
				if(cGraphics::world->cubes[y][x].selected)
				{
					cClipboardObject* clipboard = new cClipboardObject();
					clipboard->clipboardRot = cGraphics::world->models[i]->rot;
					clipboard->clipboardScale = cGraphics::world->models[i]->scale;
					clipboard->clipboardFile = cGraphics::world->models[i]->filename;
					clipboard->clipboardY = cGraphics::world->models[i]->pos.y;
					clipboard->clipboardName = cGraphics::world->models[i]->name;
					clipboard->clipboardFloat = cGraphics::world->models[i]->lightopacity;
					clipboard->usePos = true;
					clipboard->pos = cGraphics::world->models[i]->pos - cVector3(2*minx,0,2*miny);
					objects.push_back(clipboard);
				}
			}
		}
	}
	for(i = 0; i < cGraphics::world->lights.size(); i++)
	{
		if(cGraphics::world->lights[i].pos.x > 0 && cGraphics::world->lights[i].pos.z > 0)
		{
			x = round(cGraphics::world->lights[i].pos.x / 2.0f);
			y = round(cGraphics::world->lights[i].pos.z / 2.0f);
			if(x < cGraphics::world->width && y < cGraphics::world->height)
			{
				if(cGraphics::world->cubes[y][x].selected)
				{
					lights.push_back(cGraphics::world->lights[i]);
					lights[lights.size()-1].pos -= cVector3(2*minx,0,2*miny);
				}
			}
		}
	}
	for(i = 0; i < cGraphics::world->effects.size(); i++)
	{
		if(cGraphics::world->effects[i].pos.x > 0 && cGraphics::world->effects[i].pos.z > 0)
		{
			x = round(cGraphics::world->effects[i].pos.x / 2.0f);
			y = round(cGraphics::world->effects[i].pos.z / 2.0f);
			if(x < cGraphics::world->width && y < cGraphics::world->height)
			{
				if(cGraphics::world->cubes[y][x].selected)
				{
					effects.push_back(cGraphics::world->effects[i]);
					effects[effects.size()-1].pos -= cVector3(2*minx,0,2*miny);
				}
			}
		}
	}
	for(i = 0; i < cGraphics::world->sounds.size(); i++)
	{
		if(cGraphics::world->sounds[i].pos.x > 0 && cGraphics::world->sounds[i].pos.z > 0)
		{
			x = round(cGraphics::world->sounds[i].pos.x / 2.0f);
			y = round(cGraphics::world->sounds[i].pos.z / 2.0f);
			if(x < cGraphics::world->width && y < cGraphics::world->height)
			{
				if(cGraphics::world->cubes[y][x].selected)
				{
					sounds.push_back(cGraphics::world->sounds[i]);
					sounds[sounds.size()-1].pos -= cVector3(2*minx,0,2*miny);
				}
			}
		}
	}
	Log(3,0,"Copied %i models, %i lights, %i effects and %i sounds", objects.size(), lights.size(), effects.size(), sounds.size());


	for(i = 0; i < tiles.size(); i++)
	{
		tiles[i].x -= minx;
		tiles[i].y -= miny;
	}
}

void cClipBoardArea::apply()
{
	unsigned int i;
	int offX = round(cGraphics::cMouse::x3d/10.0f);
	int offZ = round(cGraphics::cMouse::z3d/10.0f);
	for(i = 0; i < tiles.size(); i++)
	{
		if(doHeight)
		{
			cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].cell1 = tiles[i].cube.cell1;
			cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].cell2 = tiles[i].cube.cell2;
			cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].cell3 = tiles[i].cube.cell3;
			cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].cell4 = tiles[i].cube.cell4;
		}
		if(doTextures)
		{
			if(tiles[i].cube.tileUp == -1)
			{
				cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].tileUp = -1;
			}
			else
			{
				cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].tileUp = cGraphics::world->tiles.size();
				cGraphics::world->tiles.push_back(worldContainer->world->tiles[tiles[i].cube.tileUp]);
				
				cTextureContainer* texture = worldContainer->world->textures[worldContainer->world->tiles[tiles[i].cube.tileUp].texture];
				int found = -1;
				for(unsigned int ii = 0; ii < cGraphics::world->textures.size() && found == -1; ii++)
				{
					if(cGraphics::world->textures[ii]->RoFilename == texture->RoFilename)
						found = ii;
				}
				if(found == -1)
				{
					cTextureContainer* container = new cTextureContainer();
					container->RoFilename = texture->RoFilename;
					container->RoFilename2 = texture->RoFilename2;
					container->texture = cTextureCache::load(cSettings::roDir + "data\\texture\\" + texture->RoFilename);
					found = cGraphics::world->textures.size();				
					cGraphics::world->textures.push_back(container);
				}
				cGraphics::world->tiles[cGraphics::world->tiles.size()-1].texture = found;
				
			}
		}
		if(doGat)
		{
			cGraphics::world->gattiles[tiles[i].y+offZ][tiles[i].x+offX] = tiles[i].gat;

		}
	}


	if(doObjects)
	{
		for(i = 0; i < objects.size(); i++)
			objects[i]->apply();
	}
	if(doLights)
	{
		for(i = 0; i < lights.size(); i++)
		{
			cGraphics::world->lights.push_back(lights[i]);
			cGraphics::world->lights[cGraphics::world->lights.size()-1].pos += cVector3(2*offX, 0, 2*offZ);
		}
	}
	if(doEffects)
	{
		for(i = 0; i < effects.size(); i++)
		{
			cGraphics::world->effects.push_back(effects[i]);
			cGraphics::world->effects[cGraphics::world->effects.size()-1].pos += cVector3(2*offX, 0, 2*offZ);
		}
	}
	if(doSounds)
	{
		for(i = 0; i < sounds.size(); i++)
		{
			cGraphics::world->sounds.push_back(sounds[i]);
			cGraphics::world->sounds[cGraphics::world->sounds.size()-1].pos += cVector3(2*offX, 0, 2*offZ);
		}
	}


}

void cClipBoardArea::render()
{
	int offX = round(cGraphics::cMouse::x3d/10.0f);
	int offZ = round(cGraphics::cMouse::z3d/10.0f);
	glColor4f(1,1,1,1);

	unsigned int i;
	for(i = 0; i < tiles.size(); i++)
	{
		cCube* c = &tiles[i].cube;
		if(c->tileUp == -1)
			continue;

		cTile* t = &worldContainer->world->tiles[c->tileUp];

		if(!doHeight)
			c = &cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX];
		if(!doTextures)
		{
			if(cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].tileUp == -1)
				continue;
			t = &cGraphics::world->tiles[cGraphics::world->cubes[tiles[i].y+offZ][tiles[i].x+offX].tileUp];
		}

		glBindTexture(GL_TEXTURE_2D, worldContainer->world->textures[t->texture]->texId());
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(t->u1, 1-t->v1);	glVertex3f((offX+tiles[i].x)*10,	-c->cell1+0.1f,(cGraphics::world->height-(offZ+tiles[i].y))*10);
			glTexCoord2f(t->u3, 1-t->v3);	glVertex3f((offX+tiles[i].x)*10,	-c->cell3+0.1f,(cGraphics::world->height-(offZ+tiles[i].y))*10-10);
			glTexCoord2f(t->u2, 1-t->v2);	glVertex3f((offX+tiles[i].x)*10+10,	-c->cell2+0.1f,(cGraphics::world->height-(offZ+tiles[i].y))*10);
			glTexCoord2f(t->u4, 1-t->v4);	glVertex3f((offX+tiles[i].x)*10+10,	-c->cell4+0.1f,(cGraphics::world->height-(offZ+tiles[i].y))*10-10);
		glEnd();
	}

	if(doObjects)
	{
		glPushMatrix();
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glTranslatef(0,0,cGraphics::world->height*10);
		glScalef(1,1,-1);
		for(i = 0; i < objects.size(); i++)
		{
			objects[i]->pos2 = cVector3(2*offX, 0.1f, 2*offZ);
			objects[i]->render();
		}
		glPopMatrix();
	}

}

cClipBoardArea::~cClipBoardArea()
{
	for(unsigned int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
}
