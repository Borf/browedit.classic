#ifndef __UNDO_LIGHTDELETE_H__
#define __UNDO_LIGHTDELETE_H__

#include <undo.h>

class cUndoLightDelete : public cUndoItem
{
	int id;
	cLight light;
public:
	cUndoLightDelete(int i)
	{
		id = i;
		light = cGraphics::world->lights[i];
	}
	void undo()
	{
		cGraphics::world->lights.insert(cGraphics::world->lights.begin() + id, light);
		cGraphics::worldContainer->settings.selectedObject = id;
	}
};

#endif
