#ifndef __UNDO_CHANGEWALL_H__
#define __UNDO_CHANGEWALL_H__

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
			cGraphics::world->cubes[y][x].tileSide = val;
		else
			cGraphics::world->cubes[y][x].tileOtherSide = val;
	}
};

#endif
