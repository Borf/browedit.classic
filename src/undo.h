#ifndef __UNDO_H__
#define __UNDO_H__

#include "common.h"
#include <list>
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
	virtual ~cUndoItem() {};
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
	cUndoChangeObject(int id)
	{
		type = UNDO_OTHER;
		objectid = id;
		pos = Graphics.world.models[id]->pos;
		rot = Graphics.world.models[id]->rot;
		scale = Graphics.world.models[id]->scale;
	}
	void undo()
	{
		Graphics.world.models[objectid]->pos = pos;
		Graphics.world.models[objectid]->rot = rot;
		Graphics.world.models[objectid]->scale = scale;
	}
};


class cUndoChangeObjects : public cUndoItem
{
	class cObject
	{public:
		cVector3 pos;
		cVector3 rot;
		cVector3 scale;
		int objectid;
	};
	std::vector<cObject>	objects;
public:

	cUndoChangeObjects(std::vector<int> ob)
	{
		type = UNDO_OTHER;
		for(unsigned int i = 0; i < ob.size(); i++)
		{
			cObject o;
			o.pos = Graphics.world.models[ob[i]]->pos;
			o.rot = Graphics.world.models[ob[i]]->rot;
			o.scale = Graphics.world.models[ob[i]]->scale;
			o.objectid = ob[i];
			objects.push_back(o);
		}
	}
	void undo()
	{
		unsigned int i;
		for(i = 0; i < objects.size(); i++)
		{
			Graphics.world.models[objects[i].objectid]->pos = objects[i].pos;
			Graphics.world.models[objects[i].objectid]->rot = objects[i].rot;
			Graphics.world.models[objects[i].objectid]->scale = objects[i].scale;
		}
		int count = 0;
		Graphics.selectionCenter = cVector3(0,0,0);
		for(i = 0; i < Graphics.world.models.size(); i++)
		{
			if (Graphics.world.models[i]->selected)
			{
				count++;
				Graphics.selectionCenter+=Graphics.world.models[i]->pos;
			}
		}
		Graphics.selectionCenter = Graphics.selectionCenter / (float)count;
	}
};


class cUndoTexture : public cUndoItem
{
	std::vector<std::vector<int> > data;
	int x, y;
public:
	cUndoTexture(int xfrom, int yfrom, int xto, int yto)
	{
		x = xfrom;
		y = yfrom;
		for(int yy = yfrom; yy < yto; yy++)
		{
			std::vector<int> row;
			for(int xx = xfrom; xx < xto; xx++)
			{
				if (yy < 0 || yy >= Graphics.world.height || xx < 0 || xx >= Graphics.world.width)
					row.push_back(0);
				else
					row.push_back(Graphics.world.cubes[yy][xx].tileUp);
			}
			data.push_back(row);
		}
	}
	void undo()
	{
		for(int yy = 0; yy < (int)data.size(); yy++)
		{
			for(int xx = 0; xx < (int)data[yy].size(); xx++)
			{
				if (!(yy+y < 0 || yy+y >= Graphics.world.height || xx+x < 0 || xx+x >= Graphics.world.width))
					Graphics.world.cubes[y+yy][x+xx].tileUp = data[yy][xx];
			}
		}
	}
};

class cUndoHeightEdit : public cUndoItem
{
	class cCubeHeight
	{
	public:
		float cell1,cell2,cell3,cell4;
	};
	std::vector<std::vector<cCubeHeight> > data;
	int x, y;
public:
	cUndoHeightEdit(int xfrom, int yfrom, int xto, int yto)
	{
		x = xfrom;
		y = yfrom;
		for(int yy = yfrom; yy < yto; yy++)
		{
			std::vector<cCubeHeight> row;
			for(int xx = xfrom; xx < xto; xx++)
			{
				cCubeHeight c;
				if (!(yy < 0 || yy >= Graphics.world.height || xx < 0 || xx >= Graphics.world.width))
				{
					c.cell1 = Graphics.world.cubes[yy][xx].cell1;
					c.cell2 = Graphics.world.cubes[yy][xx].cell2;
					c.cell3 = Graphics.world.cubes[yy][xx].cell3;
					c.cell4 = Graphics.world.cubes[yy][xx].cell4;
				}
				row.push_back(c);
			}
			data.push_back(row);
		}
	}
	void undo()
	{
		for(int yy = 0; yy < (int)data.size(); yy++)
		{
			for(int xx = 0; xx < (int)data[yy].size(); xx++)
			{
				if (!(yy+y < 0 || yy+y >= Graphics.world.height || xx+x < 0 || xx+x >= Graphics.world.width))
				{
					Graphics.world.cubes[y+yy][x+xx].cell1 = data[yy][xx].cell1;
					Graphics.world.cubes[y+yy][x+xx].cell2 = data[yy][xx].cell2;
					Graphics.world.cubes[y+yy][x+xx].cell3 = data[yy][xx].cell3;
					Graphics.world.cubes[y+yy][x+xx].cell4 = data[yy][xx].cell4;
					Graphics.world.cubes[y+yy][x+xx].calcNormal();
				}
			}
		}
	}
};

class cUndoGatHeightEdit : public cUndoItem
{
	class cCubeHeight
	{public:
		float cell1,cell2,cell3,cell4;
	};
	std::vector<std::vector<cCubeHeight> > data;
	int x, y;
public:
	cUndoGatHeightEdit(int xfrom, int yfrom, int xto, int yto)
	{
		x = xfrom;
		y = yfrom;
		for(int yy = yfrom; yy < yto; yy++)
		{
			std::vector<cCubeHeight> row;
			for(int xx = xfrom; xx < xto; xx++)
			{
				cCubeHeight c;
				if (!(yy < 0 || yy >= (int)Graphics.world.gattiles.size() || xx < 0 || xx >= (int)Graphics.world.gattiles[0].size()))
				{
					c.cell1 = Graphics.world.gattiles[yy][xx].cell1;
					c.cell2 = Graphics.world.gattiles[yy][xx].cell2;
					c.cell3 = Graphics.world.gattiles[yy][xx].cell3;
					c.cell4 = Graphics.world.gattiles[yy][xx].cell4;
				}
				row.push_back(c);
			}
			data.push_back(row);
		}
	}
	void undo()
	{
		for(unsigned int yy = 0; yy < data.size(); yy++)
		{
			for(unsigned int xx = 0; xx < data[yy].size(); xx++)
			{
				if (!(y+yy < 0 || y+yy >= Graphics.world.gattiles.size() || x+xx < 0 || x+xx >= Graphics.world.gattiles[0].size()))
				{
					Graphics.world.gattiles[y+yy][x+xx].cell1 = data[yy][xx].cell1;
					Graphics.world.gattiles[y+yy][x+xx].cell2 = data[yy][xx].cell2;
					Graphics.world.gattiles[y+yy][x+xx].cell3 = data[yy][xx].cell3;
					Graphics.world.gattiles[y+yy][x+xx].cell4 = data[yy][xx].cell4;
				}
			}
		}
	}
};

class cUndoGatTileEdit : public cUndoItem
{
	std::vector<std::vector<int> > data;
	int x, y;
public:
	cUndoGatTileEdit(int xfrom, int yfrom, int xto, int yto)
	{
		x = xfrom;
		y = yfrom;
		for(int yy = yfrom; yy < yto; yy++)
		{
			std::vector<int> row;
			for(int xx = xfrom; xx < xto; xx++)
			{
				if (yy >= (int)Graphics.world.gattiles.size() || yy < 0 || xx < 0 || xx >= (int)Graphics.world.gattiles[0].size())
					row.push_back(0);
				else
					row.push_back(Graphics.world.gattiles[yy][xx].type);
			}
			data.push_back(row);
		}
	}
	void undo()
	{
		for(unsigned int yy = 0; yy < data.size(); yy++)
		{
			for(unsigned int xx = 0; xx < data[yy].size(); xx++)
			{
				if (!(yy+y >= Graphics.world.gattiles.size() || yy+y < 0 || xx+x < 0 || xx+x > Graphics.world.gattiles[0].size()))
					Graphics.world.gattiles[y+yy][x+xx].type = data[yy][xx];
			}
		}
	}
};


class cUndoTileEdit : public cUndoItem
{
	std::vector<std::pair<int, cTile> > data;
public:
	cUndoTileEdit(std::vector<std::pair<int, cTile> > mydata)
	{
		data = mydata;
	}
	void undo()
	{
		for(unsigned int i = 0; i < data.size(); i++)
		{
			Graphics.world.tiles[data[i].first] = data[i].second;
		}
	}
};


class cUndoNewObject : public cUndoItem
{
public:
	cUndoNewObject()
	{

	}
	void undo()
	{
		delete Graphics.world.models.back();
		Graphics.world.models.erase(Graphics.world.models.begin() + (Graphics.world.models.size()-1));
	}
};

class cUndoNewObjects : public cUndoItem
{
	std::vector<int> prevselection;
	unsigned int size;
public:
	cUndoNewObjects(int previoussize)
	{
		size = previoussize;
		for(unsigned int i = 0; i < Graphics.world.models.size(); i++)
		{
			if (Graphics.world.models[i]->selected)
				prevselection.push_back(i);
		}
	}
	void undo()
	{
		while(Graphics.world.models.size() > size)
		{
			delete Graphics.world.models.back();
			Graphics.world.models.erase(Graphics.world.models.begin() + (Graphics.world.models.size()-1));
		}
		for(unsigned int i = 0; i < prevselection.size(); i++)
			Graphics.world.models[prevselection[i]]->selected = true;
	}
};


class cUndoChangeWall : public cUndoItem
{
	int x,y,type,val;
public:
	cUndoChangeWall(int t, int xx, int yy,int v)
	{
		type = t;
		x = xx;
		y = yy;
		val = v;
	}
	void undo()
	{
		if (type == 0)
			Graphics.world.cubes[y][x].tileSide = val;
		else
			Graphics.world.cubes[y][x].tileOtherSide = val;
	}
};

class cUndoChangeWalls : public cUndoItem
{
	int type;
	std::vector<std::pair<std::pair<int,int>, int> > vals;
public:
	cUndoChangeWalls(int t, std::vector<std::pair<std::pair<int,int>, int> > v)
	{
		type = t;
		vals = v;
	}
	void undo()
	{
		if (type == 0)
		{
			for(unsigned int i = 0; i < vals.size(); i++)
				Graphics.world.cubes[vals[i].first.second][vals[i].first.first].tileSide = vals[i].second;
		}
		else
		{
			for(unsigned int i = 0; i < vals.size(); i++)
				Graphics.world.cubes[vals[i].first.second][vals[i].first.first].tileOtherSide = vals[i].second;
		}
	}
};

class cUndoChangeWater : public cUndoItem
{
	cWater water;
public:
	cUndoChangeWater(cWater w)
	{
		water = w;
	}
	void undo()
	{
		Graphics.world.water = water;
	}
};

class cUndoObjectDelete : public cUndoItem
{
	std::string filename;
	cVector3 pos;
	int id;
	cVector3 scale;
	cVector3 rot;
public:
	cUndoObjectDelete(int i)
	{
		id = i;
		pos = Graphics.world.models[i]->pos;
		scale = Graphics.world.models[i]->scale;
		rot = Graphics.world.models[i]->rot;
		filename = Graphics.world.models[i]->filename;
	}
	void undo()
	{
		cRSMModel* model = new cRSMModel();
		model->load(filename);
		model->pos = pos;
		model->rot = rot;
		model->scale = scale;
		Graphics.world.models.insert(Graphics.world.models.begin() + id, model);
		Graphics.selectedObject = id;
//		Graphics.world.models.push_back(model);
	}
};

class cUndoEffectDelete : public cUndoItem
{
	int id;
	cEffect effect;
public:
	cUndoEffectDelete(int i)
	{
		id = i;
		effect = Graphics.world.effects[i];
	}
	void undo()
	{
		Graphics.world.effects.insert(Graphics.world.effects.begin() + id, effect);
		Graphics.selectedObject = id;
	}
};

class cUndoEffectsDelete : public cUndoItem
{
	std::vector<std::pair<int, cEffect> > effects;
public:
	cUndoEffectsDelete(std::vector<int> e)
	{
		for(unsigned int i = 0; i < e.size(); i++)
		{
			effects.push_back(std::pair<int, cEffect>(e[i], Graphics.world.effects[e[i]]));
		}
	}
	void undo()
	{
		for(unsigned int i = 0; i < effects.size(); i++)
		{
			Graphics.world.effects.insert(Graphics.world.effects.begin() + effects[i].first, effects[i].second);
		}
	}
};


class cUndoChangeEffect : public cUndoItem
{
	cEffect effect;
	int id;
public:
	cUndoChangeEffect(int i)
	{
		id = i;
		effect = Graphics.world.effects[i];
	}
	void undo()
	{
		Graphics.world.effects[id] = effect;
	}
};


class cUndoLightDelete : public cUndoItem
{
	int id;
	cLight light;
public:
	cUndoLightDelete(int i)
	{
		id = i;
		light = Graphics.world.lights[i];
	}
	void undo()
	{
		Graphics.world.lights.insert(Graphics.world.lights.begin() + id, light);
		Graphics.selectedObject = id;
	}
};

class cUndoNewLight : public cUndoItem
{
public:
	cUndoNewLight()
	{

	}
	void undo()
	{
		Graphics.world.lights.erase(Graphics.world.lights.begin() + (Graphics.world.lights.size()-1));
	}
};

class cUndoChangeLight : public cUndoItem
{
	cLight light;
	int id;
public:
	cUndoChangeLight(int i)
	{
		id = i;
		light = Graphics.world.lights[i];
	}
	void undo()
	{
		Graphics.world.lights[id] = light;
	}
};


class cUndoObjectsDelete : public cUndoItem
{
public:
	class cObject
	{public:
		std::string filename;
		cVector3 pos;
		cVector3 rot;
		cVector3 scale;
		int id;
	};
	std::vector<cObject> objects;

	cUndoObjectsDelete(std::vector<cObject> myobjects)
	{
		objects = myobjects;
	}
	void undo()
	{
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			cRSMModel* model = new cRSMModel();
			model->load(objects[i].filename);
			model->pos = objects[i].pos;
			model->rot = objects[i].rot;
			model->scale = objects[i].scale;
			model->selected = true;
			Graphics.world.models.insert(Graphics.world.models.begin() + objects[i].id, model);
		}
	}
};








class cUndoSoundDelete : public cUndoItem
{
	int id;
	cSound sound;
public:
	cUndoSoundDelete(int i)
	{
		id = i;
		sound = Graphics.world.sounds[i];
	}
	void undo()
	{
		Graphics.world.sounds.insert(Graphics.world.sounds.begin() + id, sound);
		Graphics.selectedObject = id;
	}
};

class cUndoNewSound : public cUndoItem
{
public:
	cUndoNewSound()
	{

	}
	void undo()
	{
		Graphics.world.sounds.erase(Graphics.world.sounds.begin() + (Graphics.world.sounds.size()-1));
	}
};

class cUndoChangeSound : public cUndoItem
{
	cSound sound;
	int id;
public:
	cUndoChangeSound(int i)
	{
		id = i;
		sound = Graphics.world.sounds[i];
	}
	void undo()
	{
		Graphics.world.sounds[id] = sound;
	}
};



























class cUndoStack
{
public:
	std::list<cUndoItem*> items;

	void undo();
	void push(cUndoItem*);

	cUndoStack()
	{
	}
};

#endif

