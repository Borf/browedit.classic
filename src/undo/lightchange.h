#ifndef __UNDO_LIGHTCHANGE_H__
#define __UNDO_LIGHTCHANGE_H__

#include <undo.h>

class cUndoChangeLight : public cUndoItem
{
	cLight light;
	int id;
public:
	cUndoChangeLight(int i)
	{
		id = i;
		light = cGraphics::world->lights[i];
	}
	void undo()
	{
		cGraphics::world->lights[id] = light;
	}
};

#endif
