#include "window.h"
#include "windowmodel.h"

#include <graphics.h>
extern cGraphics Graphics;


cWindowModel::cWindowModel(cWindow* parent) : cWindowObject(parent)
{
	type = OBJECT_MODEL;
	model = NULL;
	rotation = 0;
}
void cWindowModel::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	if(data != "" && model == NULL)
	{
		model = loadmodel(data);
	}


	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);			// (X, Y, Width, Height)
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();

	int xx, yy;
	xx = parent->px() + realx();
	yy = parent->py() + realy();
	glViewport(xx, yy+(cutoffbottom), w, h-(cutofftop) - (cutoffbottom));

	
	glLoadIdentity();
	glOrtho(-w,w,-h+2*cutoffbottom,h-2*cutofftop,-10000,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

/*	glColor4f(0,0,0,1);
	glDisable(GL_TEXTURE_2D);
	glLineWidth(2);
	glBegin(GL_QUADS);
		glVertex2f(-w,-h);
		glVertex2f(w,-h);
		glVertex2f(w,h);
		glVertex2f(-w,h);
	glEnd();*/
	glLineWidth(1);
	glEnable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);
	glEnable(GL_DEPTH_TEST);

	if (model != NULL)
	{
		glDisable(GL_LIGHTING);
		glTranslatef(0,-h/2,1000);
		glTranslatef(0,(model->boundingbox1.y - model->boundingbox2.y)/2,0);
		glRotatef(10,1,0,0);
		glRotatef(rotation,0,1,0);
		glScalef(5,5,5);

		float LightPosition[4];
		LightPosition[0] = 0;
		LightPosition[1] = 0;
		LightPosition[2] = 2000;


		glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
		
		
		
		model->draw();
		rotation += Graphics.frameticks/10.0f;
		if(rotation > 360)
			rotation -= 360;
		glDisable(GL_LIGHTING);
	}
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	glDisable(GL_DEPTH_TEST);

}


void cWindowModel::SetText(int i, string s)
{
	data = s;
}


string cWindowModel::GetText(int i)
{
	return data;
}

void cWindowModel::SetInt(int i, int id)
{
	data = "";
	if (model != NULL)
	{
		delete model;
		model = NULL;
	}
}

cWindowModel::~cWindowModel()
{
	if(model != NULL)
		delete model;
	model = NULL;
}
