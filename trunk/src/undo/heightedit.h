#ifndef __UNDO_HEIGHTEDIT_H__
#define __UNDO_HEIGHTEDIT_H__

#include <undo.h>
#include <world.h>

class cUndoHeightEdit : public cUndoItem
{
	class cCubeHeight
	{
	public:
		float cell1,cell2,cell3,cell4;
	};
	std::vector<std::vector<cCubeHeight> > data;
	int x, y;
	public:
		cUndoHeightEdit(int xfrom, int yfrom, int xto, int yto)
		{
			x = xfrom;
			y = yfrom;
			for(int yy = yfrom; yy < yto; yy++)
			{
				std::vector<cCubeHeight> row;
				for(int xx = xfrom; xx < xto; xx++)
				{
					cCubeHeight c;
					if (!(yy < 0 || yy >= cGraphics::world->height || xx < 0 || xx >= cGraphics::world->width))
					{
						c.cell1 = cGraphics::world->cubes[yy][xx].cell1;
						c.cell2 = cGraphics::world->cubes[yy][xx].cell2;
						c.cell3 = cGraphics::world->cubes[yy][xx].cell3;
						c.cell4 = cGraphics::world->cubes[yy][xx].cell4;
					}
					row.push_back(c);
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
					{
						cGraphics::world->cubes[y+yy][x+xx].cell1 = data[yy][xx].cell1;
						cGraphics::world->cubes[y+yy][x+xx].cell2 = data[yy][xx].cell2;
						cGraphics::world->cubes[y+yy][x+xx].cell3 = data[yy][xx].cell3;
						cGraphics::world->cubes[y+yy][x+xx].cell4 = data[yy][xx].cell4;
					}
				}
			}
			cGraphics::world->calcVertexNormals(x,y,x+data.size(), y+data[0].size());
		}
};

#endif
