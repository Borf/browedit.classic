#ifndef __OBJECTSCHANGE_H__
#define __OBJECTSCHANGE_H__

#include <undo.h>

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
				o.pos = cGraphics::world->models[ob[i]]->pos;
				o.rot = cGraphics::world->models[ob[i]]->rot;
				o.scale = cGraphics::world->models[ob[i]]->scale;
				o.objectid = ob[i];
				objects.push_back(o);
			}
		}
		void undo()
		{
			unsigned int i;
			for(i = 0; i < objects.size(); i++)
			{
				cGraphics::world->models[objects[i].objectid]->pos = objects[i].pos;
				cGraphics::world->models[objects[i].objectid]->rot = objects[i].rot;
				cGraphics::world->models[objects[i].objectid]->scale = objects[i].scale;
			}
			int count = 0;
			cGraphics::selectionCenter = cVector3(0,0,0);
			for(i = 0; i < cGraphics::world->models.size(); i++)
			{
				if (cGraphics::world->models[i]->selected)
				{
					count++;
					cGraphics::selectionCenter+=cGraphics::world->models[i]->pos;
				}
			}
			cGraphics::selectionCenter = cGraphics::selectionCenter / (float)count;
		}
};

#endif
