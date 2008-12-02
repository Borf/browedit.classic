#ifndef __UNDO_SOUNDNEW_H__
#define __UNDO_SOUNDNEW_H__

#include <undo.h>

class cUndoNewSound : public cUndoItem
{
public:
	cUndoNewSound()
	{
		
	}
	void undo()
	{
		cGraphics::world->sounds.erase(cGraphics::world->sounds.begin() + (cGraphics::world->sounds.size()-1));
	}
};

#endif
