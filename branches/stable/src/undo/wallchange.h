#ifndef __UNDO_CHANGEWALL_H__
#define __UNDO_CHANGEWALL_H__

#include <undo.h>

class cUndoWallChange : public cUndoItem
{
	int x,y,type,val;
public:
	cUndoWallChange(int t, int xx, int yy,int v)
	{
		type = t;
		x = xx;
		y = yy;
		val = v;
	}
	void undo()
	{
		if (type == 0)
			cGraphics::world->cubes[y][x].tileSide = val;
		else
			cGraphics::world->cubes[y][x].tileOtherSide = val;
	}
};

#endif
