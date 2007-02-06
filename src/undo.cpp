#include "undo.h"


void cUndoStack::undo()
{
	cUndoItem* item = items.top();
	if(item->type == UNDO_NOOP)
		return;
	items.pop();
	item->undo();
	delete item;
}