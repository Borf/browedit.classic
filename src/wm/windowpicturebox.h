#ifndef __WINDOWPICTUREBOX_H__
#define __WINDOWPICTUREBOX_H__

#include "windowobject.h"
#include <string>
#include <vector>
#include "../texture.h"
#include "../texturecache.h"
using namespace std;

class cWindowPictureBox : public cWindowObject
{
protected:
	cTexture* tex;
	int highlightmode;
public:
	cWindowPictureBox()
	{
		highlightmode = 0;
		w = 100;
		h = 100;
		x = 5;
		y = 20;
		tex = NULL;
		alignment = ALIGN_TOPLEFT;
		loaded = false;
		type = OBJECT_PICTUREBOX;
	}
	~cWindowPictureBox() 
	{
		if (tex != NULL || loaded)
			TextureCache.unload(tex);
	}
	virtual void draw();
	virtual void click();

	void SetText(int, string);
	void SetInt(int, int);
	int GetInt(int);
	bool loaded;
};




#endif
