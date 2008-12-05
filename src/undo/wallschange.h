#ifndef __UNDO_WALLSCHANGE_H__
#define __UNDO_WALLSCHANGE_H__

#include <undo.h>

class cUndoWallsChange : public cUndoItem
{
	int type;
	std::vector<std::pair<std::pair<int,int>, int> > vals;
public:
	cUndoWallsChange(int t, std::vector<std::pair<std::pair<int,int>, int> > v)
	{
		type = t;
		vals = v;
	}
	void undo()
	{
		if (type == 0)
		{
			for(unsigned int i = 0; i < vals.size(); i++)
				cGraphics::world->cubes[vals[i].first.second][vals[i].first.first].tileSide = vals[i].second;
		}
		else
		{
			for(unsigned int i = 0; i < vals.size(); i++)
				cGraphics::world->cubes[vals[i].first.second][vals[i].first.first].tileOtherSide = vals[i].second;
		}
	}
};

#endif
