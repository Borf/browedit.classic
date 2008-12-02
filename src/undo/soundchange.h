#ifndef __UNDO_SOUNDCHANGE_H__
#define __UNDO_SOUNDCHANGE_H__

#include <undo.h>

class cUndoChangeSound : public cUndoItem
{
	cSound sound;
	int id;
public:
	cUndoChangeSound(int i)
	{
		id = i;
		sound = cGraphics::world->sounds[i];
	}
	void undo()
	{
		cGraphics::world->sounds[id] = sound;
	}
};

#endif
