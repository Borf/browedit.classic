#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "common.h"
#include <gl/gl.h>
#include <map>
using namespace std;
#include "world.h"
#include "font.h"
#include "texture.h"
#include "frustum.h"

class cGraphics
{
public:
	cGraphics()
	{
		width=1024;
		height=768;
		bits=32;
		fullscreen=false;
		selectedobjectprop = 0;
		showgrid = true;
		brushsize = 1;
		texturestart = 0;

		texturerot = 0;
		fliph = false;
		flipv = false;
		showobjects = false;
		selectedobject = -1;
		objectstartdrag = false;
		slope = false;
		quadtreeview = -1;
		showboundingboxes = false;
		gattype = 0;
		showlightmaps = false;
		showtilecolors = true;
		showwater = true;
		showoglighting = true;
		lasttick = SDL_GetTicks();
	}

	int						init();				// initializes graphics
	int						draw();				// draws graphics
	int						ReSizeGLScene(int, int);		// Resize And Initialize The GL Window

	void					KillGLWindow(void);

	int&					h()
	{
		return				height;
	}

	int&					w()
	{
		return				width;
	}

	cWorld					world;

	float					cameraheight;
	float					cameraangle;
	float					camerarot;
	cVector2				camerapointer;

	bool					showgrid;

	cVector2				selectedTile;

	cVector2				selectionstart;
	cVector2				selectionend;
	float					brushsize;
	int						texturestart;
	int						texturerot;
	bool					fliph;
	bool					flipv;

	long					lasttick;
	long					frameticks;


	cVector3				selectionstart3d;
	cVector3				selectionend3d;
	vector<int>				selectedobjects;
	string					editstring;
	float*					editval;

	bool					is3dSelected(float, float, float);

	string					popup(string, string = "");

	string					popupreturn;
	string					defaultpopuptext;
	string					popupcaption;
	cFont*					font;
	CFrustum				frustum;


	cTexture*				mask;
	cTexture*				bulb;
	int						selectedobjectprop;

	int						selectedobject;
	bool					objectstartdrag;
	bool					showoglighting;

	bool					slope;


	cRSMModel*				previewmodel;
	int						previewcolor;


	cVector3				clipboardrot;
	cVector3				clipboardscale;
	string					clipboardfile;
	float					clipboardy;
	GLfloat LightAmbient[4];			// lights
	GLfloat LightDiffuse[4];
	GLfloat LightPosition[4];


	bool					showobjects;
	bool					showlightmaps;
	bool					showtilecolors;
	bool					showwater;


	int						quadtreeview;
	bool					showboundingboxes;
	int						gattype;
	cTexture*				gattextures[7];
	cTexture*				splash;
	vector<vector<cTexture*> >		watertextures;


private:
	int						CreateGLWindow();
	int						InitGL(void);

////////////////////
	int						height;						// windowheight
	int						width;						// windowwidth
	int						bits;						// bpp (8,16,24,32)
	bool					fullscreen;					// fullscreen ?
	void					drawvertexview();
	void					drawfloorview();
	void					drawwallview();
	void					drawsideview();
};


#endif
