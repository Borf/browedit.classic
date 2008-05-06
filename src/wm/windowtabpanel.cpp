#include "windowtabpanel.h"

#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include "window.h"
#include <graphics.h>
extern cGraphics Graphics;

void cWindowTabPanel::draw(int cutoffleft, int cutoffright, int cutofftop, int cutoffbottom)
{
	int i;
	GLfloat colors[4];
	glGetFloatv(GL_CURRENT_COLOR, colors);
	int xx, yy;
	xx = realx();
	yy = realy2();
	glEnable(GL_TEXTURE_2D);
	bool overtabs = false;
	if (this->inobject() && parent->istopwindow() && ((Graphics.h()-mousey) - parent->py() - yy - h > -14))
		overtabs = true;

	glTranslatef((float)xx, (float)yy, 0);
	glBindTexture(GL_TEXTURE_2D, parent->texture->texid());

	int hh = h - max(skinTabHeight[0], skinTabHeight[1]);

	glBegin(GL_QUADS);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(0,				hh-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	hh-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(skinLeftWidth,	hh);
		glTexCoord2f(skinLeft/512.0f,					skinTop/512.0f);						glVertex2d(0,				hh);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	hh-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,hh-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(w-skinRightWidth,hh);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinTop/512.0f);						glVertex2d(skinLeftWidth,	hh);

		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,hh-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(w,				hh-skinTopHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinTop/512.0f);						glVertex2d(w,				hh);
		glTexCoord2f(skinRight/512.0f,					skinTop/512.0f);						glVertex2d(w-skinRightWidth,hh);

		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(0,				skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	hh-skinTopHeight);
		glTexCoord2f(skinLeft/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(0,				hh-skinTopHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,hh-skinTopHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(skinLeftWidth,	hh-skinTopHeight);

		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(w,				skinBottomHeight);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinTop-skinTopHeight)/512.0f);		glVertex2d(w,				hh-skinTopHeight);
		glTexCoord2f(skinRight/512.0f,					(skinTop-skinTopHeight)/512.0f);		glVertex2d(w-skinRightWidth,hh-skinTopHeight);
///////////////			
		glTexCoord2f(skinLeft/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(0,				0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(skinLeftWidth,	0);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);
		glTexCoord2f(skinLeft/512.0f,					skinBottom/512.0f);						glVertex2d(0,				skinBottomHeight);

		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(skinLeftWidth,	0);
		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(w-skinRightWidth,0);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
		glTexCoord2f((skinLeft+skinLeftWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(skinLeftWidth,	skinBottomHeight);

		glTexCoord2f(skinRight/512.0f,					(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(w-skinRightWidth,0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	(skinBottom-skinBottomHeight)/512.0f);	glVertex2d(w,				0);
		glTexCoord2f((skinRight+skinRightWidth)/512.0f,	skinBottom/512.0f);						glVertex2d(w,				skinBottomHeight);
		glTexCoord2f(skinRight/512.0f,					skinBottom/512.0f);						glVertex2d(w-skinRightWidth,skinBottomHeight);
	glEnd();
	
	
	
	int maxOverlapLeft = max(skinTabOverlapLeft[0], skinTabOverlapLeft[1]);
	int maxOverlapRight = max(skinTabOverlapRight[0], skinTabOverlapRight[1]);

	for(i = tabs.size()-1; i >= 0 ; i--)
	{
		float left = maxOverlapLeft+((w-maxOverlapLeft-maxOverlapRight)/(float)tabs.size()) * i;
		float right = maxOverlapLeft+((w-maxOverlapLeft-maxOverlapRight)/(float)tabs.size()) * (i+1);

		int t = (i == selectedtab) ? 0 : 1;

		glDisable(GL_CULL_FACE);
		glBegin(GL_QUADS);
			glTexCoord2f(skinTabLeft[t]/512.0f,							skinTabTop[t]/512.0f);						glVertex2d(left-skinTabOverlapLeft[t],						hh+skinTabHeight[t]);
			glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	skinTabTop[t]/512.0f);						glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],	hh+skinTabHeight[t]);
			glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],	hh);
			glTexCoord2f(skinTabLeft[t]/512.0f,							(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(left-skinTabOverlapLeft[t],						hh);		

			glTexCoord2f((skinTabRight[t])/512.0f,						skinTabTop[t]/512.0f);						glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh+skinTabHeight[t]);
			glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	skinTabTop[t]/512.0f);						glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],		hh+skinTabHeight[t]);
			glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],		hh);
			glTexCoord2f((skinTabRight[t])/512.0f,						(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh);

			glTexCoord2f((skinTabRight[t])/512.0f,						skinTabTop[t]/512.0f);						glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh+skinTabHeight[t]);
			glTexCoord2f((skinTabRight[t]+skinTabRightWidth[t])/512.0f,	skinTabTop[t]/512.0f);						glVertex2d(right+skinTabOverlapRight[t],						hh+skinTabHeight[t]);
			glTexCoord2f((skinTabRight[t]+skinTabRightWidth[t])/512.0f,	(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(right+skinTabOverlapRight[t],						hh);
			glTexCoord2f((skinTabRight[t])/512.0f,						(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh);
		glEnd();
	}	

		float left = maxOverlapLeft+((w-maxOverlapLeft-maxOverlapRight)/(float)tabs.size()) * selectedtab;
		float right = maxOverlapLeft+((w-maxOverlapLeft-maxOverlapRight)/(float)tabs.size()) * (selectedtab+1);

	int t = 0;

	glDisable(GL_CULL_FACE);
	glBegin(GL_QUADS);
		glTexCoord2f(skinTabLeft[t]/512.0f,							skinTabTop[t]/512.0f);						glVertex2d(left-skinTabOverlapLeft[t],						hh+skinTabHeight[t]);
		glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	skinTabTop[t]/512.0f);						glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],	hh+skinTabHeight[t]);
		glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],	hh);
		glTexCoord2f(skinTabLeft[t]/512.0f,							(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(left-skinTabOverlapLeft[t],						hh);		

		glTexCoord2f((skinTabRight[t])/512.0f,						skinTabTop[t]/512.0f);						glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh+skinTabHeight[t]);
		glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	skinTabTop[t]/512.0f);						glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],		hh+skinTabHeight[t]);
		glTexCoord2f((skinTabLeft[t]+skinTabLeftWidth[t])/512.0f,	(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(left-skinTabOverlapLeft[t]+skinTabLeftWidth[t],		hh);
		glTexCoord2f((skinTabRight[t])/512.0f,						(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh);		

		glTexCoord2f((skinTabRight[t])/512.0f,						skinTabTop[t]/512.0f);						glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh+skinTabHeight[t]);
		glTexCoord2f((skinTabRight[t]+skinTabRightWidth[t])/512.0f,	skinTabTop[t]/512.0f);						glVertex2d(right+skinTabOverlapRight[t],						hh+skinTabHeight[t]);
		glTexCoord2f((skinTabRight[t]+skinTabRightWidth[t])/512.0f,	(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(right+skinTabOverlapRight[t],						hh);
		glTexCoord2f((skinTabRight[t])/512.0f,						(skinTabTop[t]-skinTabHeight[t])/512.0f);	glVertex2d(right-skinTabRightWidth[t]+skinTabOverlapRight[t],	hh);
	glEnd();
	

	glTranslatef(-(float)realx(), -(float)realy2(), 0);

	for(i = 0; i < tabs.size(); i++)
		parent->font->print(0,0,0, maxOverlapLeft+parent->px() + xx+5+skinTabFontOffX + ((w-maxOverlapLeft-maxOverlapRight) / (float)tabs.size()) * i, parent->py() + yy+h-16+skinTabFontOffY, tabs[i].c_str());

	glColor4fv(colors);
}

void cWindowTabPanel::click()
{
	if(tabs.size() == 0)
		return;
	int xx, yy;
	xx = realx();
	yy = realy2();
	if (this->inobject() && parent->istopwindow() && ((Graphics.h()-mousey) - parent->py() - yy - h > -14))
	{
		int oldtab = selectedtab;
		selectedtab = ((int)mousex - parent->px() - xx) / (w/tabs.size());
		if (selectedtab != oldtab)
			tabchange(oldtab);
	}
}

