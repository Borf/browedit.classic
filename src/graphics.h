#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <map>

#include "world.h"
#include <wm/wm.h>
#include "frustum.h"

class cFont;


enum eTool
{
	TOOL_SELECTAREA,
	TOOL_SELECTBRUSH,
	TOOL_SELECTWAND,
	TOOL_BRUSH,
};


class cGraphics
{
public:
	cGraphics();

	int						init();
	int						draw(bool = true);
	int						resizeGLScene(int, int);

	void					killGLWindow(void);

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

	bool					showgat;

	bool					showgrid;
	float					gridsize;
	float					gridoffsetx;
	float					gridoffsety;

	cVector2				selectedTile;

	cVector2				selectionstart;
	cVector2				selectionend;
	float					brushsize;
	int						texturestart;
	int						textureRot;
	bool					fliph;
	bool					flipv;

	long					lasttick;
	long					frameticks;
	bool					showambientlighting;

	bool					groupeditmode;


	cVector3				selectionstart3d;
	cVector3				selectionend3d;
	std::vector<int>		selectedobjects;
	std::string				editstring;
	float*					editval;

	bool					is3dSelected(float, float, float);
	std::string				popup(std::string, std::string = "");

	std::string				popupReturn;
	std::string				defaultPopupText;
	std::string				popupCaption;
	cFont*					font;
	CFrustum				frustum;


	int						selectedObjectProp;

	int						selectedObject;
	bool					objectStartDrag;
	bool					showOglLighting;
	bool					topCamera;

	bool					slope;
	bool					showNoTiles;


	cRSMModel*				previewModel;
	int						previewColor;


	cVector3				clipboardRot;
	cVector3				clipboardScale;
	std::string				clipboardFile;
	std::string				clipboardName;
	float					clipboardY;
	float					clipboardFloat;
	GLfloat					lightAmbient[4];
	GLfloat					lightDiffuse[4];
	GLfloat					lightPosition[4];


	bool					showObjects;
	bool					showLightmaps;
	bool					showTileColors;
	bool					showWater;
	bool					animateWater;
	bool					transparentObjects;
	bool					showSprites;
	bool					showAllLights;
	bool					clearLightmaps;
	float					gatTransparency;


	int						quadtreeView;
	bool					showBoundingBoxes;
	int						gatType;
	std::vector<cTexture*>	gatTextures;
	cTexture*				gatBorder;
	cTexture*				splash;
	std::vector<std::vector<cTexture*> >		waterTextures;
	std::string					waterDirectory;
	std::string					waterExtension;
	int						waterCount;

	cVector2				wallHeightMin;
	cVector2				wallHeightMax;

	cTexture*				texturePreview;
	int						height;
	int						width;
	int						bits;
	bool					fullscreen;

	cVector3				selectionCenter;
	cVector3				backgroundColor;
	cVector3				noTileColor;

	cWM						WM;
	bool					showDot;
	std::vector<int>				gatTiles;

	eTool					textureTool;
	std::vector<std::vector<bool> >	textureBrush;
	cVector2				textureBrushOffset;
	float					textureGridSizeX;
	float					textureGridSizeY;

	int						textureBrushSize;

private:
	int						createGLWindow();
	int						initGL(void);
};


#endif
