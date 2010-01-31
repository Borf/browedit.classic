#ifndef __GATHEIGHTEDIT_H__
#define __GATHEIGHTEDIT_H__

#include <undo.h>

class cUndoGatHeightEdit : public cUndoItem
{
	class cCubeHeight
	{public:
	float cell1,cell2,cell3,cell4;
	};
	std::vector<std::vector<cCubeHeight> > data;
	int x, y;
	public:
		cUndoGatHeightEdit(int xfrom, int yfrom, int xto, int yto)
		{
			x = xfrom;
			y = yfrom;
			for(int yy = yfrom; yy < yto; yy++)
			{
				std::vector<cCubeHeight> row;
				for(int xx = xfrom; xx < xto; xx++)
				{
					cCubeHeight c;
					if (!(yy < 0 || yy >= (int)cGraphics::world->gattiles.size() || xx < 0 || xx >= (int)cGraphics::world->gattiles[0].size()))
					{
						c.cell1 = cGraphics::world->gattiles[yy][xx].cell1;
						c.cell2 = cGraphics::world->gattiles[yy][xx].cell2;
						c.cell3 = cGraphics::world->gattiles[yy][xx].cell3;
						c.cell4 = cGraphics::world->gattiles[yy][xx].cell4;
					}
					row.push_back(c);
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
					if (!(y+yy < 0 || y+yy >= cGraphics::world->gattiles.size() || x+xx < 0 || x+xx >= cGraphics::world->gattiles[0].size()))
					{
						cGraphics::world->gattiles[y+yy][x+xx].cell1 = data[yy][xx].cell1;
						cGraphics::world->gattiles[y+yy][x+xx].cell2 = data[yy][xx].cell2;
						cGraphics::world->gattiles[y+yy][x+xx].cell3 = data[yy][xx].cell3;
						cGraphics::world->gattiles[y+yy][x+xx].cell4 = data[yy][xx].cell4;
					}
				}
			}
		}
};

#endif
