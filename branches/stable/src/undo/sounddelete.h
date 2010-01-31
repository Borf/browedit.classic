#ifndef __UNDO_SOUNDDELETE_H__
#define __UNDO_SOUNDDELETE_H__

#include <undo.h>

class cUndoSoundDelete : public cUndoItem
{
	int id;
	cSound sound;
public:
	cUndoSoundDelete(int i)
	{
		id = i;
		sound = cGraphics::world->sounds[i];
	}
	void undo()
	{
		cGraphics::world->sounds.insert(cGraphics::world->sounds.begin() + id, sound);
		cGraphics::worldContainer->settings.selectedObject = id;
	}
};

#endif
