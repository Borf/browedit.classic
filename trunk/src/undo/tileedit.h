#ifndef __UNDO_TILEEDIT_H__
#define __UNDO_TILEEDIT_H__

#include <undo.h>
#include <world.h>

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
			cGraphics::world->tiles[data[i].first] = data[i].second;
		}
	}
};

#endif
