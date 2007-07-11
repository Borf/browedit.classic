#include <common.h>
#include <model.h>

#include "windowobject.h"

class cWindowModel : public cWindowObject
{
protected:
	cModel* model;
	string data;
	float rotation;
public:
	cWindowModel(cWindow* parent);
	void draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom);

	void SetText(int i, string s);
	string GetText(int i);
	void SetInt(int i, int id);
	~cWindowModel();
};