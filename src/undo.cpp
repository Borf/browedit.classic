#include "undo.h"
#include <settings.h>

void cUndoStack::undo()
{
	if (items.size() > 0)
	{
		cUndoItem* item = items.back();
		items.pop_back();
		item->undo();
		delete item;
	}
}

void cUndoStack::push(cUndoItem* u)
{
	items.push_back(u);
	while(items.size() > cSettings::undoSize)
	{
		delete items.front();
		items.pop_front();
	}
}

cUndoStack::~cUndoStack()
{
	std::list<cUndoItem*>::iterator i;
	for(i = items.begin(); i != items.end(); i++)
		delete *i;
	items.clear();
}