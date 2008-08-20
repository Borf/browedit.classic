#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "common.h"
#include <GL/gl.h>
#include <map>
//using namespace std;
#include "world.h"
#include "font.h"
#include "texture.h"
#include "frustum.h"
#include "wm/wm.h"


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
	cGraphics()
	{
		width=1024;
		height=768;
		bits=32;
		fullscreen=false;
		selectedObjectProp = 0;
		showgrid = true;
		brushsize = 1;
		texturestart = 0;

		texturerot = 0;
		fliph = false;
		flipv = false;
		showObjects = false;
		selectedObject = -1;
		objectStartDrag = false;
		slope = false;
		quadtreeView = -1;
		showBoundingBoxes = false;
		gatType = 0;
		showLightmaps = false;
		showTileColors = true;
		showWater = true;
		showOglLighting = true;
		lasttick = 0;
		wallHeightMin = cVector2(-1,-1);
		wallHeightMax = cVector2(-1,-1);
		texturePreview = NULL;
		gridsize = 1;
		gridoffsetx = 0;
		gridoffsety = 0;
		topCamera = false;
		showambientlighting = true;
		groupeditmode = false;
		animateWater = true;

		showNoTiles = true;
		selectionCenter = cVector3(-1,-1,-1);
		showgat = false;
		backgroundColor = cVector3(0,0,0);
		noTileColor = cVector3(1,1,1);
		showDot = true;
		showSprites = true;
		showAllLights = false;
		clearLightmaps = false;
		gatTransparency = 0.3f;
		textureTool = TOOL_SELECTAREA;
		textureBrush.resize(4, std::vector<bool>(4,true));
		textureBrush[0][0] = false;
		textureBrush[3][3] = false;
		textureBrush[0][3] = false;
		textureBrush[3][0] = false;
		textureBrushOffset = cVector2(0,0);
		textureGridSizeX = 4;
		textureGridSizeY = 4;
	}

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
	int						texturerot;
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
	std::string					popup(std::string, std::string = "");

	std::string					popupReturn;
	std::string					defaultPopupText;
	std::string					popupCaption;
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
	std::string					clipboardFile;
	std::string					clipboardName;
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

private:
	int						createGLWindow();
	int						initGL(void);
};


#endif
