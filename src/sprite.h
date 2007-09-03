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
		~cActSpr();

		class cFrame
		{
		public:
			~cFrame();

			bool loaded;
			GLuint tex;
			char* data;
			int w;
			int h;
			GLuint texid();
		};
		vector<cFrame*> frames;
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
				~cFrame();
				vector<cSubFrame*> subframes;
				int subframecount;
				int   framecount;
				long  extrainfo;
				float extrarotation;
				long  extrax;
				long  extray;
			};

			~cAction();
			vector<cFrame*> frames;
			int framecount;
		};

		vector<cAction*> actions;


		string filename;
		void load(string);
		bool loaded;
	};
	
	cSprite();
	~cSprite();

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
