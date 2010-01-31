#ifndef __MYMATH_H__
#define __MYMATH_H__

#include <string.h>

#include <math.h>
#ifndef PI
#	define PI	3.1415926535897932384626433832795
#endif

#ifndef PI_ON_180
#	define PI_ON_180	(PI/180.0)
#endif

class cVector3
{
public:
	cVector3() {}
	cVector3(float X, float Y, float Z);
	cVector3 operator+(cVector3 vVector);
	cVector3 operator-(cVector3 vVector);
	cVector3 operator-(float f);
	cVector3 operator-();
	cVector3 operator+(float f);
	cVector3 operator*(float num);
	float &operator [](int num);
	cVector3 operator/(float num);
	bool operator==(cVector3 v2);
	void operator-=(cVector3 v2);

	cVector3 getnormalized();
	void normalize();
	float magnitude();
	cVector3& operator+=(const cVector3 &rhs);
	float dot(cVector3 v2);
	cVector3 cross(cVector3 v2);

	union
	{
		struct  
		{
			float x, y, z;
		};
		float	data[3];
	};
};

class cVector2 
{
public:
	cVector2();
	cVector2(float X, float Y);
	cVector2 operator+(cVector2 vVector);
	cVector2 operator-(cVector2 vVector);
	cVector2 operator*(float num);
	cVector2 operator/(float num);
	cVector2 Normalize();
	float Magnitude();
	void	rotate(float angleinc);
	float getAngle();
	bool operator==(cVector2 v2);
	float DotProd(cVector2 v2);
	cVector2 perp();

	union
	{
		struct  
		{
			float x, y;
		};
		float	data[2];
	};
};



class cMatrix4x4
{
public:
	cMatrix4x4();
	cMatrix4x4(const cMatrix4x4 &otherMatrix);
	
	float values[16];
	
	cMatrix4x4 operator*(const cMatrix4x4&);
	void operator*=(const cMatrix4x4&);
	cVector3 operator*(const cVector3&);
	
	cMatrix4x4 transpose();
	cVector3   getTranslation();
	cVector3   getRotation();
	cVector3   getScale();
	
	
	void		setPosition(const cVector3);
	void		setPosition(const cVector2);
	void		setPosition(float = 0, float = 0, float = 0);
	
	void		setPositionX(float);
	void		setPositionY(float);
	void		setPositionZ(float);
	
	void		setScale(float = 1, float = 1, float = 1);
	void		setScaleX(float);
	void		setScaleY(float);
	void		setScaleZ(float);
	
	void		setRotationX(float);
	void		setRotationY(float);
	void		setRotationZ(float);

	
	cMatrix4x4	removeTranslation();
	
	
	//constructor-like thingies
	void		setIdentity();

	static cMatrix4x4 makeIdentity();
	static cMatrix4x4 makeRotation(float,float,float,float);
	static cMatrix4x4 makeTranslation(float = 0,float = 0,float = 0);
	static cMatrix4x4 makeTranslation(cVector3);
	static cMatrix4x4 makeScale(float = 1,float = 1,float = 1);
	static cMatrix4x4 makeScale(cVector3);
};




class cQuaternion
{
public:
	union
	{
		struct 
		{
			float x,y,z,w;
		};
		float values[4];
	};

	cQuaternion() {};	
	cQuaternion(float x, float y, float z, float w);
	cQuaternion( float *angles );
	cQuaternion( cQuaternion& q1, cQuaternion& q2, float interp );
	
	void fromAngles( float *angles );
	void slerp( cQuaternion& q1, cQuaternion& q2, float interp );
	cQuaternion normalize();
	float operator[]( int index );
	void inverse();
	cMatrix4x4	getRotationMatrix();
};	



#endif
