#ifndef __UNDO_EFFECTCHANGE_H__
#define __UNDO_EFFECTCHANGE_H__

#include <undo.h>

class cUndoChangeEffect : public cUndoItem
{
	cEffect effect;
	int id;
public:
	cUndoChangeEffect(int i)
	{
		id = i;
		effect = cGraphics::world->effects[i];
	}
	void undo()
	{
		cGraphics::world->effects[id] = effect;
	}
};

#endif
