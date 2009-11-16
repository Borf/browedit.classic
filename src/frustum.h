#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "common.h"
#include <vector>

class cFrustum 
{
public:
	static void calculateFrustum();																	// Call this every time the camera moves to update the frustum
	static bool pointInFrustum(float x, float y, float z);											// This takes a 3D point and returns TRUE if it's inside of the frustum
	static bool sphereInFrustum(float x, float y, float z, float radius);							// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
	static bool cubeInFrustum(float x, float y, float z, float size);								// This takes the center and half the length of the cube.
	static bool boxInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);
private:
	static float m_Frustum[6][4];
};


#endif


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// Nothing new was added to this file for this tutorial.
//
// 
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
//
