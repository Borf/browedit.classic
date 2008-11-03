#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <map>

#include "world.h"
#include <wm/wm.h>

class cFont;


enum eTool
{
	TOOL_SELECTAREA,
	TOOL_SELECTBRUSH,
	TOOL_SELECTWAND,
	TOOL_BRUSH,
};


class cGraphicsBase
{
public:
	cGraphicsBase();
	static int				init(int,int,int,bool);
	static int				resizeGLScene(int, int);
	static void				killGLWindow(void);
	static int&				h()	{		return				height;	}
	static int&				w()	{		return				width;	}
	static long				getFrameTicks();
protected:
	static int				createGLWindow();
	static int				initGL(void);

	static int				height;
	static int				width;
	static int				bits;
	static bool				fullscreen;

	static long				lasttick;
	static long				frameticks;


};



class cGraphics : public cGraphicsBase
{
public:
									cGraphics();
	static int						init(int,int,int,bool);
	static int						draw(bool = true);
	static void						closeAndCleanup();
	static cWorld*					world;



	static float					cameraheight;
	static float					cameraangle;
	static float					camerarot;
	static cVector2					camerapointer;

	static bool						showgat;
		
	static bool						showgrid;
	static float					gridsize;
	static float					gridoffsetx;
	static float					gridoffsety;
		
	static cVector2					selectedTile;

	static cVector2					selectionstart;
	static cVector2					selectionend;
	static float					brushsize;
	static int						texturestart;
	static int						textureRot;
	static bool						fliph;
	static bool						flipv;

	static bool						showambientlighting;
	static bool						groupeditmode;


	static cVector3					selectionstart3d;
	static cVector3					selectionend3d;
	static std::vector<int>			selectedobjects;
	static std::string				editstring;
	static float*					editval;

	static bool						is3dSelected(float, float, float);
	static std::string				popup(std::string, std::string = "");

	static std::string				popupReturn;
	static std::string				defaultPopupText;
	static std::string				popupCaption;
	static cFont*					font;


	static int						selectedObjectProp;

	static int						selectedObject;
	static bool						objectStartDrag;
	static bool						showOglLighting;
	static bool						topCamera;

	static bool						slope;
	static bool						showNoTiles;

	static cRSMModel*				previewModel;
	static int						previewColor;


	static cVector3					clipboardRot;
	static cVector3					clipboardScale;
	static std::string				clipboardFile;
	static std::string				clipboardName;
	static float					clipboardY;
	static float					clipboardFloat;
	static GLfloat					lightAmbient[4];
	static GLfloat					lightDiffuse[4];
	static GLfloat					lightPosition[4];


	static bool						showObjects;
	static bool						showLightmaps;
	static bool						showTileColors;
	static bool						showWater;
	static bool						animateWater;
	static bool						transparentObjects;
	static bool						showSprites;
	static bool						showAllLights;
	static bool						clearLightmaps;
	static float					gatTransparency;


	static int						quadtreeView;
	static bool						showBoundingBoxes;
	static int						gatType;
	static std::vector<cTexture*>	gatTextures;
	static cTexture*				gatBorder;
	static cTexture*				splash;
	static std::vector<std::vector<cTexture*> >		waterTextures;
	static std::string								waterDirectory;
	static std::string								waterExtension;
	static unsigned int								waterCount;

	static cVector2									wallHeightMin;
	static cVector2									wallHeightMax;

	static cTexture*								texturePreview;

	static cVector3									selectionCenter;
	static cVector3									backgroundColor;
	static cVector3									noTileColor;

	static bool										showDot;
	static std::vector<int>							gatTiles;

	static eTool									textureTool;
	static std::vector<std::vector<bool> >			textureBrush;
	static cVector2									textureBrushOffset;
	static float									textureGridSizeX;
	static float									textureGridSizeY;

	static int										textureBrushSize;
};


#endif
