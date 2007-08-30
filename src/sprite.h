#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <common.h>
#include <vector>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library


class cSprite
{
public:
	class cActSpr
	{
	public:
		cActSpr();

		class cFrame
		{
		public:
			GLuint tex;
			int w;
			int h;
		};
		vector<cFrame> frames;
		class cAction
		{
		public:
			class cFrame
			{
			public:
				class cSubFrame
				{
				public:
					long offsetx;
					long offsety;
					long image;
					long direction;
					long color;
					float scalex;
					float scaley;
					long  rotation;
					long dontjump;
					long sizex;
					long sizey;
				};
				vector<cSubFrame*> subframes;
				int subframecount;
				int   framecount;
				long  extrainfo;
				float extrarotation;
				long  extrax;
				long  extray;
			};
			vector<cFrame*> frames;
			int framecount;
		};

		vector<cAction*> actions;


		void load(string);
		bool loaded;
	};
	
	cSprite();

	cVector3			pos;
	cActSpr*			body;
	cActSpr*			head;
	float				scale;
	vector<cActSpr*>	extras;

	void loadbody(string);
	void loadhead(string);
	void setextra(int, string);
	void addextra(string);

	void draw();


	int action;
	int direction;
	
};




#endif
