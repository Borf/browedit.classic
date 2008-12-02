#ifndef __UNDO_LIGHTNEW_H__
#define __UNDO_LIGHTNEW_H__

#include <undo.h>

class cUndoNewLight : public cUndoItem
{
public:
	cUndoNewLight()
	{
		
	}
	void undo()
	{
		cGraphics::world->lights.erase(cGraphics::world->lights.begin() + (cGraphics::world->lights.size()-1));
	}
};

#endif
