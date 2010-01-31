#ifndef __UNDO_OBJECTSDELETE_H__
#define __UNDO_OBJECTSDELETE_H__

#include <undo.h>

class cUndoObjectsDelete : public cUndoItem
{
public:
	class cObject
	{public:
	std::string filename;
	bEngine::math::cVector3 pos;
	bEngine::math::cVector3 rot;
	bEngine::math::cVector3 scale;
	int id;
	};
	std::vector<cObject> objects;
	
	cUndoObjectsDelete(std::vector<cObject> myobjects)
	{
		objects = myobjects;
	}
	void undo()
	{
		for(unsigned int i = 0; i < objects.size(); i++)
		{
			cRsmModel* model = new cRsmModel(objects[i].filename);
			model->pos = objects[i].pos;
			model->rot = objects[i].rot;
			model->scale = objects[i].scale;
			model->selected = true;
			cGraphics::world->models.insert(cGraphics::world->models.begin() + objects[i].id, model);
		}
	}
};

#endif
