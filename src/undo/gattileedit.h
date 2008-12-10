#ifndef __UNDO_GATTILEEDIT_H__
#define __UNDO_GATTILEEDIT_H__

#include <undo.h>

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
				if (yy >= (int)cGraphics::world->gattiles.size() || yy < 0 || xx < 0 || xx >= (int)cGraphics::world->gattiles[0].size())
					row.push_back(0);
				else
					row.push_back(cGraphics::world->gattiles[yy][xx].type);
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
				if (!(yy+y >= cGraphics::world->gattiles.size() || yy+y < 0 || xx+x < 0 || xx+x > cGraphics::world->gattiles[0].size()))
					cGraphics::world->gattiles[y+yy][x+xx].type = data[yy][xx];
			}
		}
	}
};

#endif
