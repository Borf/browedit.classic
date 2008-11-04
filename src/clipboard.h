#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <vector>

class cWorldContainer;
#include <world.h>

class cClipBoard
{
public:
	enum eClipBoardType
	{
		CLIP_TEXTURE,
		CLIP_HEIGHT,
		CLIP_OBJECT,
		CLIP_GAT,
	};
	eClipBoardType type;
	cClipBoard(eClipBoardType);
	virtual void apply() = 0;


	static void setClipBoard(cClipBoard*);
	static cClipBoard* currentClipBoard;

protected:	
	cWorldContainer*	worldContainer;
};


class cClipboardTexture : public cClipBoard
{
public:
	std::vector<std::vector<std::pair<int, cTile> > > data;
	cClipboardTexture(std::vector<std::vector<std::pair<int, cTile> > > pData) : cClipBoard(cClipBoard::CLIP_TEXTURE)
	{
		data = pData;
	}

	void apply();

};


#endif
