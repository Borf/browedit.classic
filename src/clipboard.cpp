#include <common.h>
#include "clipboard.h"
#include <graphics.h>
#include <undo.h>

extern long mouseX;
extern double mouse3dx, mouse3dz;
extern std::string rodir;


cClipBoard* cClipBoard::currentClipBoard = NULL;

cClipBoard::cClipBoard( eClipBoardType t)
{
	type = t;
	worldContainer = cGraphics::worldContainer;
}

void cClipBoard::setClipBoard( cClipBoard* clipboard)
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