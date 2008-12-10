#ifndef __UNDO_OBJECTDELETE_H__
#define __UNDO_OBJECTDELETE_H__

#include <undo.h>

class cUndoObjectDelete : public cUndoItem
{
	std::string filename;
	cVector3 pos;
	int id;
	cVector3 scale;
	cVector3 rot;
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
		cRSMModel* model = new cRSMModel();
		model->load(filename);
		model->pos = pos;
		model->rot = rot;
		model->scale = scale;
		cGraphics::world->models.insert(cGraphics::world->models.begin() + id, model);
		cGraphics::worldContainer->settings.selectedObject = id;
		//		cGraphics::world->models.push_back(model);
	}
};

#endif
