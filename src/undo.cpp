#include "undo.h"

extern unsigned int undosize;

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
	while(items.size() > undosize)
	{
		delete items.front();
		items.pop_front();
	}
}
