#ifndef __UNDO_TEXTURE_H__
#define __UNDO_TEXTURE_H__

#include <undo.h>

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
				if (yy < 0 || yy >= cGraphics::world->height || xx < 0 || xx >= cGraphics::world->width)
					row.push_back(0);
				else
					row.push_back(cGraphics::world->cubes[yy][xx].tileUp);
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
				if (!(yy+y < 0 || yy+y >= cGraphics::world->height || xx+x < 0 || xx+x >= cGraphics::world->width))
					cGraphics::world->cubes[y+yy][x+xx].tileUp = data[yy][xx];
			}
		}
	}
};

#endif
