#ifndef __UNDO_OBJECTDELETE_H__
#define __UNDO_OBJECTDELETE_H__

#include <undo.h>

class cUndoObjectDelete : public cUndoItem
{
	std::string filename;
	bEngine::math::cVector3 pos;
	int id;
	bEngine::math::cVector3 scale;
	bEngine::math::cVector3 rot;
public:
	cUndoObjectDelete(int i)
	{
		id = i;
		pos = cGraphics::world->models[i]->pos;
		scale = cGraphics::world->models[i]->scale;
		rot = cGraphics::world->models[i]->rot;
		filename = cGraphics::world->models[i]->filename;
	}
	void undo()
	{
		cRsmModel* model = new cRsmModel(filename);
		model->pos = pos;
		model->rot = rot;
		model->scale = scale;
		cGraphics::world->models.insert(cGraphics::world->models.begin() + id, model);
		cGraphics::worldContainer->settings.selectedObject = id;
		//		cGraphics::world->models.push_back(model);
	}
};

#endif
