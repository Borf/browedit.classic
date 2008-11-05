#include <common.h>
#include "clipboard.h"
#include <graphics.h>
#include <undo.h>

extern long mouseX;
extern double mouse3dx, mouse3dy, mouse3dz;
extern int brushsize;
extern std::string rodir;


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
	if(mouseX < cGraphics::w()-256)
	{
		float selsizex = (fabs(cGraphics::worldContainer->settings.selectionstart.x - cGraphics::worldContainer->settings.selectionend.x) / 32);
		float selsizey = (fabs(cGraphics::worldContainer->settings.selectionstart.y - cGraphics::worldContainer->settings.selectionend.y) / 32);
		
		selsizex = floor(selsizex*cGraphics::worldContainer->settings.brushsize);
		selsizey = floor(selsizey*cGraphics::worldContainer->settings.brushsize);
		
		int posx = (int)mouse3dx / 10;
		int posy = (int)mouse3dz / 10;
		
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
						for(int i = 0; i < cGraphics::world->textures.size() && found == -1; i++)
						{
							if(cGraphics::world->textures[i]->RoFilename == texture->RoFilename)
								found = i;
						}
						if(found == -1)
						{
							cTextureContainer* t = new cTextureContainer();
							t->RoFilename = texture->RoFilename;
							t->RoFilename2 = texture->RoFilename2;
							t->texture = cTextureCache::load(rodir + "data\\texture\\" + texture->RoFilename);
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
	int posx = (int)mouse3dx / 10;
	int posy = (int)mouse3dz / 10;
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
	int posx = (int)mouse3dx / 10;
	int posy = (int)mouse3dz / 10;
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
	int posx = (int)mouse3dx / 10;
	int posy = (int)mouse3dz / 10;

	posx = posx + ceil(data[0].size()/2.0f)-1;
	posy = posy + ceil(data[0].size()/2.0f)-1;

	int yy = 0;
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	for(int y = 0; y < data.size(); y++)
	{
		for(int x = 0; x <data[y].size(); x++)
		{
			if (posx-x >= cGraphics::world->width || posx-x < 0 || posy-y < 0 || posy-y >= cGraphics::world->height)
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
	int posx = (int)mouse3dx / 5;
	int posy = (int)mouse3dz / 5;
	
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
	int posx = (int)mouse3dx / 5;
	int posy = (int)mouse3dz / 5;
	
	posx = posx + ceil(data[0].size()/2.0f)-1;
	posy = posy + ceil(data[0].size()/2.0f)-1;
	
	int yy = 0;
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	for(int y = 0; y < data.size(); y++)
	{
		for(int x = 0; x <data[y].size(); x++)
		{
			if (posx-x >= cGraphics::world->width*2 || posx-x < 0 || posy-y < 0 || posy-y >= cGraphics::world->height*2)
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
		model->pos = cVector3(mouse3dx/5.0f, -mouse3dy, mouse3dz/5.0f);
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
				
				for(int ii = 0; ii < cGraphics::world->models.size() && !found; ii++)
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
		rsmmodel->pos = cVector3(mouse3dx/5.0f, -mouse3dy, mouse3dz/5.0f);
		if (SDL_GetModState() & KMOD_SHIFT)
			rsmmodel->pos.y = clipboardY;
		rsmmodel->scale = clipboardScale;
		rsmmodel->rot = clipboardRot;
		rsmmodel->lightopacity = clipboardFloat;
		rsmmodel->name = "clipboard";
	}
	rsmmodel->pos = cVector3(mouse3dx/5.0f, -mouse3dy, mouse3dz/5.0f);
	rsmmodel->draw();
}

cClipboardObject::~cClipboardObject()
{
	if(rsmmodel)
		delete rsmmodel;
}