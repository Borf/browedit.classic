#ifndef __UNDO_OBJECTNEW_H__
#define __UNDO_OBJECTNEW_H__

#include <undo.h>

class cUndoNewObject : public cUndoItem
{
public:
	cUndoNewObject()
	{
		
	}
	void undo()
	{
		delete cGraphics::world->models.back();
		cGraphics::world->models.erase(cGraphics::world->models.begin() + (cGraphics::world->models.size()-1));
	}
};

#endif
