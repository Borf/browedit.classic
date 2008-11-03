#ifndef _FRUSTUM_H
#define _FRUSTUM_H

#include "common.h"
#include <vector>
//using namespace std;

// This will allow us to create an object to keep track of our frustum
class cFrustum {

public:

	// Call this every time the camera moves to update the frustum
	static void CalculateFrustum();

	// This takes a 3D point and returns TRUE if it's inside of the frustum
	static bool PointInFrustum(float x, float y, float z);

	// This takes a 3D point and a radius and returns TRUE if the sphere is inside of the frustum
	static bool SphereInFrustum(float x, float y, float z, float radius);

	// This takes the center and half the length of the cube.
	static bool CubeInFrustum(float x, float y, float z, float size);


/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// This checks if a box is in the frustum
	static bool BoxInFrustum(float x, float y, float z, float sizeX, float sizeY, float sizeZ);

/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *


private:

	// This holds the A B C and D values for each side of our frustum.
	static float m_Frustum[6][4];
};


// This is our debug lines class to view the octree visually
class CDebug 
{

public:

	// This adds a line to our list of debug lines
	void AddDebugLine(cVector3 vPoint1, cVector3 vPoint2);

	// This adds a rectangle with a given center, width, height and depth to our list
	void AddDebugBox(cVector3 vCenter, float width, float height, float depth);

	// This renders all of the lines
	void RenderDebugLines();		

	// This clears all of the debug lines
	void Clear();					

private:

	// This is the vector list of all of our lines
	std::vector<cVector3> m_vLines;		
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
