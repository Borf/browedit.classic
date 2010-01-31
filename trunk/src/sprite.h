#ifndef __SPRITE_H__
#define __SPRITE_H__

#include <vector>
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
#include <bengine/math/vector3.h>


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
			GLuint texId();
		};
		std::vector<cFrame*> frames;
		class cAction
		{
		public:
			class cFrame
			{
			public:
				class cSubFrame
				{
				public:
					long offsetX;
					long offsetY;
					long image;
					long direction;
					long color;
					float scaleX;
					float scaleY;
					long  rotation;
					long dontJump;
					long sizeX;
					long sizeY;
				};
				~cFrame();
				std::vector<cSubFrame*> subframes;
				int						subFrameCount;
				int						frameCount;
				long					extraInfo;
				float					extraRotation;
				long					extraX;
				long					extraY;
			};

			~cAction();
			std::vector<cFrame*>	frames;
			int						frameCount;
		};

		std::vector<cAction*>	actions;
		std::string				fileName;
		void					load(std::string);
		bool					loaded;
	};
	
	cSprite();
	~cSprite();

	bEngine::math::cVector3				pos;
	cActSpr*				body;
	cActSpr*				head;
	float					scale;
	std::vector<cActSpr*>	extras;
	int						action;
	int						direction;

	void					loadBody(std::string);
	void					loadHead(std::string);
	void					setExtra(unsigned int, std::string);
	void					addExtra(std::string);
	void					draw();
	
};




#endif
