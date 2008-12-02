#ifndef __UNDO_EFFECTDELETE_H__
#define __UNDO_EFFECTDELETE_H__

#include <undo.h>

class cUndoEffectDelete : public cUndoItem
{
	int id;
	cEffect effect;
public:
	cUndoEffectDelete(int i)
	{
		id = i;
		effect = cGraphics::world->effects[i];
	}
	void undo()
	{
		cGraphics::world->effects.insert(cGraphics::world->effects.begin() + id, effect);
		cGraphics::worldContainer->settings.selectedObject = id;
	}
};

#endif
