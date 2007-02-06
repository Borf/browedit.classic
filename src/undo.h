#ifndef __UNDO_H__
#define __UNDO_H__

#include "common.h"
#include <stack>
#include "graphics.h"

extern cGraphics Graphics;

enum eUndoItem
{
	UNDO_NOOP,
	UNDO_OTHER,
};

class cUndoItem
{
public:
	eUndoItem	type;
	virtual void undo() = 0;
};

class cUndoNOOP: public cUndoItem
{
public:

	cUndoNOOP()
	{
		type = UNDO_NOOP;
	}
	void undo()
	{
	}

};

class cUndoChangeObject : public cUndoItem
{
	cVector3 pos;
	cVector3 rot;
	cVector3 scale;
	int objectid;
public:

	cUndoChangeObject(int id, cVector3 oldpos, cVector3 oldrot, cVector3 oldscale)
	{
		type = UNDO_OTHER;
		pos = oldpos;
		rot = oldrot;
		scale = oldscale;
		objectid = id;
	}
	void undo()
	{
		Graphics.world.models[objectid]->pos = pos;
		Graphics.world.models[objectid]->rot = rot;
		Graphics.world.models[objectid]->scale = scale;
	}
};

class cUndoTexture : public cUndoItem
{
	vector<vector<int> > data;
	int x, y;
public:
	cUndoTexture(int xfrom, int yfrom, int xto, int yto)
	{
		x = xfrom;
		y = yfrom;
		for(int yy = yfrom; yy < yto; yy++)
		{
			vector<int> row;
			for(int xx = xfrom; xx < xto; xx++)
			{
				row.push_back(Graphics.world.cubes[yy][xx].tileup);
			}
			data.push_back(row);
		}
	}
	void undo()
	{
		for(int yy = 0; yy < data.size(); yy++)
		{
			for(int xx = 0; xx < data[yy].size(); xx++)
			{
				Graphics.world.cubes[y+yy][x+xx].tileup = data[yy][xx];
			}
		}
	}
};

class cUndoHeightEdit : public cUndoItem
{
	class cCubeHeight
	{public:
		float cell1,cell2,cell3,cell4;
	};
	vector<vector<cCubeHeight> > data;
	int x, y;
public:
	cUndoHeightEdit(int xfrom, int yfrom, int xto, int yto)
	{
		x = xfrom;
		y = yfrom;
		for(int yy = yfrom; yy < yto; yy++)
		{
			vector<cCubeHeight> row;
			for(int xx = xfrom; xx < xto; xx++)
			{
				cCubeHeight c;
				c.cell1 = Graphics.world.cubes[yy][xx].cell1;
				c.cell2 = Graphics.world.cubes[yy][xx].cell2;
				c.cell3 = Graphics.world.cubes[yy][xx].cell3;
				c.cell4 = Graphics.world.cubes[yy][xx].cell4;
				row.push_back(c);
			}
			data.push_back(row);
		}
	}
	void undo()
	{
		for(int yy = 0; yy < data.size(); yy++)
		{
			for(int xx = 0; xx < data[yy].size(); xx++)
			{
				Graphics.world.cubes[y+yy][x+xx].cell1 = data[yy][xx].cell1;
				Graphics.world.cubes[y+yy][x+xx].cell2 = data[yy][xx].cell2;
				Graphics.world.cubes[y+yy][x+xx].cell3 = data[yy][xx].cell3;
				Graphics.world.cubes[y+yy][x+xx].cell4 = data[yy][xx].cell4;
			}
		}
	}
};


class cUndoGatTileEdit : public cUndoItem
{
	vector<vector<int> > data;
	int x, y;
public:
	cUndoGatTileEdit(int xfrom, int yfrom, int xto, int yto)
	{
		x = xfrom;
		y = yfrom;
		for(int yy = yfrom; yy < yto; yy++)
		{
			vector<int> row;
			for(int xx = xfrom; xx < xto; xx++)
			{
				if (yy >= Graphics.world.gattiles.size() || yy < 0 || xx < 0 || xx > Graphics.world.gattiles[0].size())
					row.push_back(0);
				else
					row.push_back(Graphics.world.gattiles[yy][xx].type);
			}
			data.push_back(row);
		}
	}
	void undo()
	{
		for(int yy = 0; yy < data.size(); yy++)
		{
			for(int xx = 0; xx < data[yy].size(); xx++)
			{
				if (!(yy >= Graphics.world.gattiles.size() || yy < 0 || xx < 0 || xx > Graphics.world.gattiles[0].size()))
					Graphics.world.gattiles[y+yy][x+xx].type = data[yy][xx];
			}
		}
	}
};


class cUndoTileEdit : public cUndoItem
{
	vector<pair<int, cTile> > data;
public:
	cUndoTileEdit(vector<pair<int, cTile> > mydata)
	{
		data = mydata;
	}
	void undo()
	{
		for(int i = 0; i < data.size(); i++)
		{
			Graphics.world.tiles[data[i].first] = data[i].second;
		}
	}
};

class cUndoStack
{
public:
	stack<cUndoItem*> items;

	void undo();

	cUndoStack()
	{
		items.push(new cUndoNOOP());
	}
};

#endif
