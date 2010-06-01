#include <common.h>
#include "toolbar.h"
#include <bengine/util.h>
#include <settings.h>

cToolBarWindow::cToolBarWindow()
{
	windowType = WT_TOOLBAR;
	resizable = false;
	visible = true;
	modal = false;
	movable = false;
	
	h = 32;
	w = cGraphics::w() - 256;
	x = 0;
	y = cGraphics::h()-h-20;
	minWidth = 20 + skinOffLeft+skinOffRight;
	minHeight = 20 + skinOffTop+skinOffBottom;
	
	title = "";

	Json::Value wSkin = cWM::skin["miniwindow"];
	
	skinTopHeight = wSkin["top"]["height"].asInt();
	skinTop =		512 - wSkin["top"]["pos"].asInt();
	skinBottomHeight = wSkin["bottom"]["height"].asInt();
	skinBottom =		512 - wSkin["bottom"]["pos"].asInt();
	
	skinLeftWidth = wSkin["left"]["width"].asInt();
	skinLeft =		wSkin["left"]["pos"].asInt();
	skinRightWidth = wSkin["right"]["width"].asInt();
	skinRight =		wSkin["right"]["pos"].asInt();
	
	wSkin = wSkin["offsets"];
	skinOffLeft =	wSkin["left"].asInt() / 2;
	skinOffRight =	wSkin["right"].asInt() / 2;
	skinOffTop =	wSkin["top"].asInt() / 2;
	skinOffBottom = wSkin["bottom"].asInt() / 2;


	for(int i = 0; i < 13; i++)
	{
		cWindowToggleButton* t = new cModeButton(this, i);
		objects["mode" + bEngine::util::intToString(i)] = t;
	}


	cWindowToggleButton* t = new cWindowToggleButton(this, &cGraphics::view.showGrid);
	t->moveTo(320, 0);
	t->setIcon("data/buttons/grid.tga");
	objects["showgrid"] = t;
	
}

void cToolBarWindow::resizeTo(int ww, int hh)
{
	cWindow::resizeTo(ww,hh);
}

void cToolBarWindow::draw()
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	glColor4f(1,1,1,1);
	cWindow::draw();
	glColor4fv(colors);
}

cToolBarWindow::cModeButton::cModeButton( cWindow* parent, int m ) : cWindowToggleButton(parent)
{
	mode = m;
	moveTo(28 * mode, 0);
	resizeTo(28,28);
	alignment = ALIGN_TOPLEFT;
	icon = NULL;

	switch(m)
	{
	case 0: setIcon("data/buttons/edit_texture.tga"); break;
	case 1: setIcon("data/buttons/edit_heightdetail.tga"); break;
	case 2: setIcon("data/buttons/edit_heightglobal.tga"); break;
	case 3: setIcon("data/buttons/edit_walls.tga"); break;
	case 4: setIcon("data/buttons/edit_objects.tga"); break;
	case 5: setIcon("data/buttons/edit_gat.tga"); break;
	case 6: setIcon("data/buttons/edit_water.tga"); break;
	case 7: setIcon("data/buttons/edit_effects.tga"); break;
	case 8: setIcon("data/buttons/edit_sounds.tga"); break;
	case 9: setIcon("data/buttons/edit_lights.tga"); break;
	case 10:setIcon("data/buttons/edit_objectgroup.tga"); break;
	case 11:setIcon("data/buttons/edit_sprite.tga"); break;
	case 12:setIcon("data/buttons/edit_texturepaint.tga"); break;
	}

}

void cToolBarWindow::cModeButton::onClick()
{
	cSettings::editMode = (eMode)mode;
}

void cToolBarWindow::cModeButton::draw( int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom )
{
	pushed = (mode == cSettings::editMode);
	cWindowToggleButton::draw(cutoffleft, cutoffright, cutofftop, cutoffbottom);
}