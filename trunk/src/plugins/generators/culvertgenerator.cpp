#include <windows.h>
#include <cstdlib>
#include <vector>
#include "culvertgenerator.h"
#include "../base/interface.h"

using namespace std;

#ifndef __MINGW32__
#pragma warning( disable : 4244 )
#endif

cCulvertGenerator::cCulvertGenerator() : cPluginBase("Culvert Generator", "tools/generators/CULVERTGENERATOR")
{
	
}



bool cCulvertGenerator::action()
{
	unsigned int i;
	int xx,yy;

	float x,y;

	while(browInterface->tileCount() > 0)
		browInterface->removeTile(0);
	for(int tex = 0; tex < 5; tex++)
	{
		for(y = 0; y < 4; y++)
		{
			for(x = 0; x < 4; x++)
			{
				cBrowInterface::cPluginTile t;
				t.lightmap = 0;
				t.texture = tex;
				t.u1 = x/4.0f;
				t.v1 = y/4.0f;
				t.u2 = (x+1)/4.0f;
				t.v2 = (y)/4.0f;
				t.u3 = (x)/4.0f;
				t.v3 = (y+1)/4.0f;
				t.u4 = (x+1)/4.0f;
				t.v4 = (y+1)/4.0f;
				t.color[0] = (char)255;
				t.color[1] = (char)255;
				t.color[2] = (char)255;
				t.color[3] = (char)255;
				browInterface->addTile(t);
			}
		}
	}
	
	
	for(y = 0; y < browInterface->getWorldHeight(); y++)
	{
		for(x = 0; x < browInterface->getWorldWidth(); x++)
		{
			cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
			c->tileOtherSide = -1;
			c->tileSide = -1;
			c->tileUp = 48 + ((int)x%4) + 4*((int)y%4);
			c->cell1 = 16;
			c->cell2 = 16;
			c->cell3 = 16;
			c->cell4 = 16;
		}
	}
	
	browInterface->render();

	
	int lasta = 0;
	int reali = 0;

	int w,h;

	
	std::vector<cIntQuad> islands;

	bool filledenough = false;
	unsigned int lastsize = 0;
	while(!filledenough)
	{
		if(lastsize != islands.size())
		{
			browInterface->render();
			lastsize = islands.size();
		}
		int island = -1;
		reali++;
		if(reali > 10000)
			break;

		w = 8+rand() % 8;
		h = 8+rand() % 8;

		if(islands.size() > 0)
		{
			island = rand() % islands.size();
			if(islands[island].connections.size() > 1)
				island = rand() % islands.size();


			int a = rand()%4;
			if(a == lasta || (a+2)%4 == lasta)
				a = rand()%4;
			lasta = a;

			if(a == 0)
			{
				x = islands[island].x + islands[island].w + 4;
				y = islands[island].y;
			}
			if(a == 1)
			{
				x = islands[island].x;
				y = islands[island].y + islands[island].h + 4;
			}
			if(a == 2)
			{
				x = islands[island].x;
				y = islands[island].y - h - 4;
			}
			if(a == 3)
			{
				x = islands[island].x - w - 4;
				y = islands[island].y;
			}

		}
		else
		{
			x = (browInterface->getWorldWidth()-w)/2;
			y = (browInterface->getWorldHeight()-h)/2;
		}

		if(!(x + w >= browInterface->getWorldWidth()-1 || y+h >= browInterface->getWorldHeight()-1 || x <= 1 || y <= 1))
		{
			int takencount = 0;
			for(xx = (int)x; xx < (int)x+w; xx++)
			{
				for(yy = (int)y; yy < (int)y+h; yy++)
				{
					if(browInterface->getCube(xx,yy)->cell1 == 0)
						takencount++;
				}
			}
			if(takencount < 3)
			{
				for(xx = (int)x; xx < (int)x+w; xx++)
				{
					for(yy = (int)y; yy < (int)y+h; yy++)
					{
						cBrowInterface::cPluginCube* c = browInterface->getCube(xx,yy);
						c->cell1 = 0;//rand()%25;
						c->cell2 = 0;//rand()%25;
						c->cell3 = 0;//rand()%25;
						c->cell4 = 0;//rand()%25;
						c->tileUp = 32 + (xx%4) + 4*(yy%4);
					}
				}
				if(island != -1)
					islands[island].connections.push_back(islands.size());

				islands.push_back(cIntQuad((int)x,(int)y,w,h));
				if(island != -1)
					islands[islands.size()-1].connections.push_back(island);
			}
			int count = 0;
			for(int yy = 0; yy < browInterface->getWorldHeight(); yy++)
			{
				for(int xx = 0; xx < browInterface->getWorldWidth(); xx++)
				{
					if(browInterface->getCube(xx,yy)->cell1 == 0)
						count++;
				}
			}
			if(count > browInterface->getWorldHeight()*browInterface->getWorldWidth() / 2)
				filledenough = true;

		}
	}

	while(browInterface->modelCount() > 0)
		browInterface->removeModel(0);


	for(i = 0; i < islands.size(); i++)
	{
		for(unsigned int ii = 0; ii < islands[i].connections.size(); ii++)
		{
			browInterface->render();
			x = islands[islands[i].connections[ii]].x;
			y = islands[islands[i].connections[ii]].y;
			w = islands[islands[i].connections[ii]].w;
			h = islands[islands[i].connections[ii]].h;

			xx = islands[i].x;
			yy = islands[i].y;

			if(xx - (x+w) == 4)
				x+=w;
			else if(yy - (y+h) == 4)
				y+=h;
			else if(y - (yy+islands[i].h) == 4)
				yy+=islands[i].h;
			else if(x - (xx+islands[i].w) == 4)
				xx+=islands[i].w;


			if(xx == x)
			{
				if(w < islands[i].w)
					x = xx = xx + w/2;
				else
					x = xx = xx + islands[i].w/2;
			}
			if(yy == y)
			{
				if(h < islands[i].h)
					y = yy = yy + h/2;
				else
					y = yy = yy + islands[i].h/2;
			}


			cBrowInterface::cPluginRSMModel* model = browInterface->loadModel("data\\model\\郴何家前\\枚促府.rsm");
			model->pos.x = (xx+x) + ((xx == x) ? 1 : 0);
			model->pos.z = (yy+y) + ((yy == y) ? 1 : 0);
			model->pos.y = 10;
			model->scale = bEngine::math::cVector3((xx == x) ? 0.95f : 1,1,(yy == y) ? 0.95f : 1);
			model->rot = bEngine::math::cVector3(0,xx==x ? 0 : 90,0);
			browInterface->addModel(model);

		}
	}


	browInterface->setWaterHeight(8);





	for(y = 0; y < browInterface->getWorldHeight(); y++)
	{
		for(x = 0; x < browInterface->getWorldWidth(); x++)
		{
			cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
			browInterface->getGatCube(2*(int)x,		2*(int)y)->type =	(c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			browInterface->getGatCube(2*(int)x+1,	2*(int)y)->type =	(c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			browInterface->getGatCube(2*(int)x,		2*(int)y+1)->type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
			browInterface->getGatCube(2*(int)x+1,	2*(int)y+1)->type = (c->cell1+c->cell2+c->cell3+c->cell4) != 0 ? '\1' : '\0';
		}

	}




/*	for(x = 0; x < browInterface->getWorldWidth; x++)
		for(y = 0; y < browInterface->getWorldHeight; y++)
		{
			world->cubes[(int)y][(int)x].maxHeight = -99999;
			world->cubes[(int)y][(int)x].minHeight = 99999;
		}

	for(y = 0; y < browInterface->getWorldHeight; y++)
	{
		for(x = 0; x < browInterface->getWorldWidth; x++)
		{
			if(world->cubes[(int)y][(int)x].maxHeight != -99999)
			{
				world->gattiles[2*(int)y][2*(int)x].type = '\0';
				world->gattiles[2*(int)y][2*(int)x+1].type = '\0';
				world->gattiles[2*(int)y+1][2*(int)x].type = '\0';
				world->gattiles[2*(int)y+1][2*(int)x+1].type = '\0';

				world->gattiles[2*(int)y][2*(int)x].cell1 = 	world->gattiles[2*(int)y][2*(int)x].cell2 = 	world->gattiles[2*(int)y][2*(int)x].cell3 = 	world->gattiles[2*(int)y][2*(int)x].cell4 = 0;
				world->gattiles[2*(int)y][2*(int)x+1].cell1 = 	world->gattiles[2*(int)y][2*(int)x+1].cell2 = 	world->gattiles[2*(int)y][2*(int)x+1].cell3	= 	world->gattiles[2*(int)y][2*(int)x+1].cell4 = 0;
				world->gattiles[2*(int)y+1][2*(int)x].cell1 = 	world->gattiles[2*(int)y+1][2*(int)x].cell2 = 	world->gattiles[2*(int)y+1][2*(int)x].cell3	= 	world->gattiles[2*(int)y+1][2*(int)x].cell4 = 0;
				world->gattiles[2*(int)y+1][2*(int)x+1].cell1 = world->gattiles[2*(int)y+1][2*(int)x+1].cell2 = world->gattiles[2*(int)y+1][2*(int)x+1].cell3 = world->gattiles[2*(int)y+1][2*(int)x+1].cell4 = 0;
			}
		}

	}*/

	for(x = 0; x < browInterface->getWorldWidth()-1; x++)
	{
		for(y = 0; y < browInterface->getWorldHeight()-1; y++)
		{
			cBrowInterface::cPluginCube* c = browInterface->getCube(x,y);
			if (c->tileOtherSide == -1)
			{
				if (c->cell4 != (c+1)->cell1 && c->cell2 != (c+1)->cell3)
				{
					c->tileOtherSide = 32+((int)y%4);
				}
			}
			if (c->tileSide == -1)
			{
				if (c->cell4 != browInterface->getCube(x,y+1)->cell1 && c->cell3 != browInterface->getCube(x,y+1)->cell2)
				{
					c->tileSide = 32+((int)x%4);
				}
			}
		}
		browInterface->render();
	}


	
	return true;
}


