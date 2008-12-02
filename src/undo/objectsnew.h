#ifndef __UNDO_OBJECTSNEW_H__
#define __UNDO_OBJECTSNEW_H__

#include <undo.h>

class cUndoNewObjects : public cUndoItem
{
	std::vector<int> prevselection;
	unsigned int size;
public:
	cUndoNewObjects(int previoussize)
	{
		size = previoussize;
		for(unsigned int i = 0; i < cGraphics::world->models.size(); i++)
		{
			if (cGraphics::world->models[i]->selected)
				prevselection.push_back(i);
		}
	}
	void undo()
	{
		while(cGraphics::world->models.size() > size)
		{
			delete cGraphics::world->models.back();
			cGraphics::world->models.erase(cGraphics::world->models.begin() + (cGraphics::world->models.size()-1));
		}
		for(unsigned int i = 0; i < prevselection.size(); i++)
			cGraphics::world->models[prevselection[i]]->selected = true;
	}
};

#endif
