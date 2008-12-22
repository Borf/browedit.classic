#ifndef __UNDO_H__
#define __UNDO_H__

#include "common.h"
#include <list>
#include "graphics.h"


enum eUndoItem
{
	UNDO_NOOP,
	UNDO_OTHER,
};

class cUndoItem
{
public:
	virtual ~cUndoItem() {};
	eUndoItem	type;
	virtual void undo() = 0;
};

class cUndoNOOP: public cUndoItem
{
public:

	cUndoNOOP()
	{
		type = UNDO_NOOP;
	}
	void undo()
	{
	}

};






class cUndoStack
{
public:
	std::list<cUndoItem*> items;

	void undo();
	void push(cUndoItem*);

	cUndoStack()
	{
	}
	~cUndoStack();
};

#endif

