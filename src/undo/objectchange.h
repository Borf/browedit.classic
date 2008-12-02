#ifndef __OBJECT_CHANGE_H__
#define __OBJECT_CHANGE_H__

#include <undo.h>

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
		pos = cGraphics::world->models[id]->pos;
		rot = cGraphics::world->models[id]->rot;
		scale = cGraphics::world->models[id]->scale;
	}
	void undo()
	{
		cGraphics::world->models[objectid]->pos = pos;
		cGraphics::world->models[objectid]->rot = rot;
		cGraphics::world->models[objectid]->scale = scale;
	}
};


#endif
