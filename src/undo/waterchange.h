#ifndef __UNDO_WATERCHANGE_H__
#define __UNDO_WATERCHANGE_H__

#include <undo.h>

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
		cGraphics::world->water = water;
	}
};

#endif
