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

	float x, y, z;						
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

	float x, y;
};

#if 0

class Quaternion
{
	public:
		Quaternion( float *angles ) { fromAngles( angles ); }
		Quaternion( Quaternion& q1, Quaternion& q2, float interp ) { slerp( q1, q2, interp ); }
		void fromAngles( float *angles );

		void slerp( Quaternion& q1, Quaternion& q2, float interp );
		float operator[]( int index ) { return m_quat[index]; }
		void inverse();
	private:
		float m_quat[4];
};


class Matrix
{
	public:
		Matrix();
		~Matrix();
		void loadIdentity();
		void set( float *matrix );
		void postMultiply( Matrix& matrix );
		void setTranslation( float *translation );
		void setInverseTranslation( float *translation );
		void setRotationRadians( float *angles );
		void setRotationDegrees( float *angles );
		void setRotationQuaternion( Quaternion& quat );
		void setInverseRotationRadians( float *angles );
		void setInverseRotationDegrees( float *angles );
		float *getMatrix() { return m_matrix; }
		void inverseTranslateVect( float *pVect );
		void inverseRotateVect( float *pVect );
	private:
		float m_matrix[16];
};

#endif

#endif
