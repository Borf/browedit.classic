#include "windowprogressbar.h"
#include "window.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

void cWindowProgressBar::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);


	float ratio = ((float)value / (float)max-min);
	float ww = (float)w*ratio;

	glBindTexture(GL_TEXTURE_2D, parent->texture->texId());

	drawprogress(w,0);

	if(skinType == NORMAL)
	{
		drawprogress(ww,1);
	}
	else if(skinType == CONDITIONAL)
	{
		int s = 1;
		for(int i = 1; i < skins.size(); i++)
		{
			if(ratio*100 < skins[i].condition && ratio*100 > skins[i-1].condition)
				s = i;
		}
		drawprogress(ww,s);
	}

	glColor4fv(colors);
	
}


void cWindowProgressBar::drawprogress(int ww, int i)
{
	int xx, yy;
	xx = realX();
	yy = realY();

	glBegin(GL_QUADS);
		glTexCoord2f((skins[i].left)/512.0f,					(skins[i].top-skins[i].height)/512.0f);		glVertex2d(xx,							yy+skins[i].height);
		glTexCoord2f((skins[i].left+skins[i].leftWidth)/512.0f,	(skins[i].top-skins[i].height)/512.0f);		glVertex2d(xx+skins[i].leftWidth,		yy+skins[i].height);
		glTexCoord2f((skins[i].left+skins[i].leftWidth)/512.0f,	(skins[i].top)/512.0f);						glVertex2d(xx+skins[i].leftWidth,		yy);
		glTexCoord2f((skins[i].left)/512.0f,					(skins[i].top)/512.0f);						glVertex2d(xx,							yy);

		glTexCoord2f((skins[i].left+skins[i].leftWidth)/512.0f,	(skins[i].top-skins[i].height)/512.0f);		glVertex2d(xx+skins[i].leftWidth,		yy+skins[i].height);
		glTexCoord2f((skins[i].left+skins[i].leftWidth)/512.0f,	(skins[i].top)/512.0f);						glVertex2d(xx+skins[i].leftWidth,		yy);
		glTexCoord2f((skins[i].right)/512.0f,					(skins[i].top)/512.0f);						glVertex2d(xx+ww-skins[i].rightWidth,	yy);
		glTexCoord2f((skins[i].right)/512.0f,					(skins[i].top-skins[i].height)/512.0f);		glVertex2d(xx+ww-skins[i].rightWidth,	yy+skins[i].height);
		
		glTexCoord2f((skins[i].right)/512.0f,						(skins[i].top-skins[i].height)/512.0f);	glVertex2d(xx+ww-skins[i].rightWidth,	yy+skins[i].height);
		glTexCoord2f((skins[i].right)/512.0f,						(skins[i].top)/512.0f);					glVertex2d(xx+ww-skins[i].rightWidth,	yy);
		glTexCoord2f((skins[i].right+skins[i].rightWidth)/512.0f,	(skins[i].top)/512.0f);					glVertex2d(xx+ww,						yy);
		glTexCoord2f((skins[i].right+skins[i].rightWidth)/512.0f,	(skins[i].top-skins[i].height)/512.0f);	glVertex2d(xx+ww,						yy+skins[i].height);
	glEnd();
}

void cWindowProgressBar::click()
{
	Log(3,0,"Clicked the progress bar");
}


int cWindowProgressBar::getInt(int id)
{
	return 0;
}

void cWindowProgressBar::setInt(int id, int val)
{
	if (id == 0)
		value = val;
	else if (id == 1)
		min = val;
	else if (id == 2)
		max = val;
	else if (id == 3)
		gradcolors = (val != 0);
}

std::string cWindowProgressBar::getPopup()
{
	char buf[100];
	sprintf(buf ,"%i/%i (%.02f%%)", value, max-min, ((float)value / (float)(max-min))*100.0f);
	return buf;
}

cWindowProgressBar::cWindowProgressBar( cWindow* parent, TiXmlDocument &skin ) : cWindowObject(parent)
{
	w = 100;
	h = 10;
	x = 5;
	y = 20;
	alignment = ALIGN_TOPLEFT;
	type = OBJECT_PROGRESSBAR;
	min = 0;
	max = 100;
	value = 33;
	
	
	TiXmlElement* p = skin.FirstChildElement("skin")->FirstChildElement("progress");
	skinType = (strcmp(p->Attribute("type"), "normal") == 0 ? NORMAL : (strcmp(p->Attribute("type"), "conditional") == 0 ? CONDITIONAL : NORMAL));
	
	
	skins.push_back(cProgressSkin(p->FirstChildElement("back")));
	TiXmlElement* el = p->FirstChildElement("bar");
	while(el != NULL)
	{
		skins.push_back(cProgressSkin(el));
		el = el->NextSiblingElement("bar");
	}
}
cWindowProgressBar::cProgressSkin::cProgressSkin( TiXmlElement* el )
{
	height = atoi(el->FirstChildElement("height")->FirstChild()->Value());
	top = 512-atoi(el->FirstChildElement("top")->FirstChild()->Value());
	leftWidth = atoi(el->FirstChildElement("left")->Attribute("width"));
	left = atoi(el->FirstChildElement("left")->FirstChild()->Value());
	rightWidth = atoi(el->FirstChildElement("right")->Attribute("width"));
	right = atoi(el->FirstChildElement("right")->FirstChild()->Value());
	
	if(el->Attribute("cond"))
		condition = atoi(el->Attribute("cond"));
	else
		condition = 0;
}