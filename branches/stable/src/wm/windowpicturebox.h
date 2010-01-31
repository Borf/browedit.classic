#ifndef __WINDOWPICTUREBOX_H__
#define __WINDOWPICTUREBOX_H__

#include "windowobject.h"
#include <string>
#include <vector>
#include <texture.h>
#include <texturecache.h>
//using namespace std;

class cWindowPictureBox : public cWindowObject
{
protected:
	int highlightmode;
public:
	cTexture* tex;
	cWindowPictureBox(cWindow* parent);
	~cWindowPictureBox();
	virtual void draw(int,int,int,int);
	virtual void onClick();

	virtual void setText(int, std::string);
	void setInt(int, int);
	int getInt(int);
	bool loaded;
};




#endif
