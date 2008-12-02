#ifndef __UNDO_EFFECTSDELETE_H__
#define __UNDO_EFFECTSDELETE_H__

#include <undo.h>

class cUndoEffectsDelete : public cUndoItem
{
	std::vector<std::pair<int, cEffect> > effects;
public:
	cUndoEffectsDelete(std::vector<int> e)
	{
		for(unsigned int i = 0; i < e.size(); i++)
		{
			effects.push_back(std::pair<int, cEffect>(e[i], cGraphics::world->effects[e[i]]));
		}
	}
	void undo()
	{
		for(unsigned int i = 0; i < effects.size(); i++)
		{
			cGraphics::world->effects.insert(cGraphics::world->effects.begin() + effects[i].first, effects[i].second);
		}
	}
};

#endif
