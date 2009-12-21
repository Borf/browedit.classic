#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <common.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <map>

#include <bengine/forwards.h>
#include <bengine/math/vector2.h>
#include <bengine/math/vector3.h>

class cWorld;
class cUndoStack;
class cFont;
class cMenu;
class cRsmModel;

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
	class cMouse
	{
	public:
		static bool			lbuttondown, rbuttondown;
		static long			lastlclick, lastrclick;
		static long			x, y;
		static long			xStart, yStart;
		static long			xOld, yOld;		
		static double		x3d, y3d, z3d;
		static double		x3dStart, y3dStart, z3dStart;		
		static bool			doubleClick;
		static double		click3dx, click3dy, click3dz;

	};
	cGraphicsBase();
	static int				init(int,int,int,bool);
	static int				resizeGLScene(int, int);
	static void				killGLWindow(void);
	static int&				h()	{		return				height;	}
	static int&				w()	{		return				width;	}
	static long				getFrameTicks();

	static cMenu*			popupMenu;
	static cMenu*			menu;


	static long				dragoffsety, dragoffsetx;

protected:
	static int				createGLWindow();
	static int				initGL(void);

	static int				height;
	static int				width;
	static int				bits;
	static bool				fullscreen;

	static long				lastTick;
	static long				frameTicks;
};



class cWorldContainer
{
public:
	class cCamera
	{
	public:
		cCamera();
		float				height;
		float				angle;
		float				rot;
		bEngine::math::cVector2			pointer;
	};
	class cView
	{
	public:
		cView();
		bool				showNoTiles;
		bool				showObjects;
		bool				showLightmaps;
		bool				showTileColors;
		bool				showWater;
		bool				showSprites;
		bool				showAllLights;
		bool				showGat;
		bool				showGrid;
		bool				showGlobalLighting;
		bool				showOglLighting;
		bool				showBoundingBoxes;
		bool				showDot;
		bool				showWaterAnimation;
		bool				showObjectsAsTransparent;
		bool				topCamera;
		bool				sideCamera;
	};

	class cSettings
	{
	public:
		cSettings();
		
		float						gridSize;
		float						gridoffsetx;
		float						gridoffsety;
		
		bEngine::math::cVector2					selectedTile;
		bEngine::math::cVector2					selectionstart;
		bEngine::math::cVector2					selectionend;
		bEngine::math::cVector3					selectionstart3d;
		bEngine::math::cVector3					selectionend3d;
		std::vector<int>			selectedobjects;
		int							selectedObject;
		
		float						brushsize;
		int							brushSizeDetailHeight;
		int							texturestart;
		int							textureRot;
		bool						fliph;
		bool						flipv;
		
		GLfloat						lightAmbient[4];
		GLfloat						lightDiffuse[4];
		GLfloat						lightPosition[4];

		bEngine::math::cVector2					wallHeightMin;
		bEngine::math::cVector2					wallHeightMax;

	};

	cWorldContainer(cWorld* w);
	~cWorldContainer();

	cWorld*					world;
	cUndoStack*				undoStack;

	cCamera					camera;
	cView					view;
	cSettings				settings;
	
};


class cGraphics : public cGraphicsBase
{
public:
											cGraphics();
	static int								init(int,int,int,bool);
	static int								draw(bool = true);
	static void								closeAndCleanup();

	static cWorld*							world;
	static cWorldContainer*					worldContainer;
	static std::vector<cWorldContainer*>	worlds;
	static cWorldContainer::cView			view;

	static void								newWorld();
	static void								updateMenu();


	static bool						objectStartDrag;
	
	
	static bool						is3dSelected(float, float, float);
	static std::string				popup(std::string, std::string = "");

	static std::string				popupReturn;
	static std::string				defaultPopupText;
	static std::string				popupCaption;
	static cFont*					font;



	static int						nModelsDrawn;
	static cRsmModel*				previewModel;
	static int						previewColor;

	static bool						clearLightmaps;
	static float					gatTransparency;
	static int						quadtreeView;
	static int						gatType;	

	static bool						slope;
	static bool						groupeditmode;

	static eTool									textureTool;
	static std::vector<std::vector<bool> >			textureBrush;
	static bEngine::math::cVector2					textureBrushOffset;
	static float									textureGridSizeX;
	static float									textureGridSizeY;
	static int										textureBrushSize;


	static bEngine::math::cVector3					selectionCenter;

	static std::vector<bEngine::cTexture*>			gatTextures;
	static bEngine::cTexture*						gatBorder;
	static bEngine::cTexture*						splash;
	static std::vector<std::vector<bEngine::cTexture*> >		waterTextures;
	static std::string								waterDirectory;
	static std::string								waterExtension;
	static unsigned int								waterCount;

	static bEngine::cTexture*						texturePreview;

	static bEngine::math::cVector3					backgroundColor;
	static bEngine::math::cVector3					noTileColor;

	static std::vector<int>							gatTiles;
};

#endif
