#include "mymath.h"
#include <windows.h>


cVector3::cVector3( float X, float Y, float Z )
{
	x = X; y = Y; z = Z;
}

cVector3 cVector3::operator+( cVector3 vVector )
{
	return cVector3(vVector.x + x, vVector.y + y, vVector.z + z);
}

cVector3 cVector3::operator+( float f )
{
	return cVector3(x + f, y + f, z + f);
}
cVector3 cVector3::operator-( cVector3 vVector )
{
	return cVector3(x - vVector.x, y - vVector.y, z - vVector.z);
}
cVector3 cVector3::operator-(  )
{
	return cVector3(-x, -y, -z);
}

cVector3 cVector3::operator*(float num)
{
	return cVector3(x * num, y * num, z * num);
}




cVector3 cVector3::operator-( float f )
{
	return cVector3(x - f, y - f, z - f);
}

float & cVector3::operator[]( int num )
{
	if (num == 0)
		return x;
	if (num == 1)
		return y;
	else
		return z;
}

cVector3 cVector3::operator/( float num )
{
	// Return the scale vector
	return cVector3(x / num, y / num, z / num);
}

bool cVector3::operator==( cVector3 v2 )
{
	return (x == v2.x && y == v2.y && z == v2.z);
}

void cVector3::operator-=( cVector3 v2 )
{
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;
}

void cVector3::normalize()
{
	*this = getnormalized();
}

cVector3 cVector3::getnormalized()
{
	float m = magnitude();
	if (m != 0)
		return (*this / magnitude());
	else
		return *this;
}

float cVector3::magnitude()
{
	return (float)sqrt( (x * x) + 
		(y * y) + 
		(z * z) );
}

cVector3& cVector3::operator+=( const cVector3 &rhs )
{
	x = x + rhs.x;
	y = y + rhs.y;
	z = z + rhs.z;
	return *this;
}

float cVector3::dot( cVector3 v2 )
{
/*cVector3 vNormal;	
vNormal.x = ((vVector1.y * z) - (vVector1.z * y));
vNormal.y = ((vVector1.z * x) - (vVector1.x * z));
vNormal.z = ((vVector1.x * y) - (vVector1.y * x));
	return vNormal;		*/
	return x*v2.x+y*v2.y+z*v2.z;
}

cVector3 cVector3::cross( cVector3 v2 )
{
	return cVector3(y*v2.z - z*v2.y,
		z*v2.x - x*v2.z,
		x*v2.y - y*v2.x);
}

cVector2::cVector2()
{
	
}

cVector2::cVector2( float X, float Y )
{
	x = X; y = Y;
}

cVector2 cVector2::operator+( cVector2 vVector )
{
	// Return the added vectors result.
	return cVector2(vVector.x + x, vVector.y + y);
}

cVector2 cVector2::operator-( cVector2 vVector )
{
	// Return the subtracted vectors result
	return cVector2(x - vVector.x, y - vVector.y);
}

cVector2 cVector2::operator*( float num )
{
	// Return the scaled vector
	return cVector2(x * num, y * num);
}

cVector2 cVector2::operator/( float num )
{
	// Return the scale vector
	return cVector2(x / num, y / num);
}

cVector2 cVector2::Normalize()
{
	float m = Magnitude();
	if (m != 0)
		return (*this / m);
	else
		return *this;
}

float cVector2::Magnitude()
{
	return (float)sqrt( (x * x) + 
		(y * y));
}

void cVector2::rotate( float angleinc )
{
	if (angleinc == 0)
		return;
	if(x == 0)
		x = 0.00001f;
	float m = Magnitude();
	Normalize();
	
	float angleto = -90;
	angleto = y / x;
	angleto = ((float)atan(angleto) * (180.0f/3.1415f));
	if (x < 0) angleto+=180;
	angleto+=angleinc;
	
	x = m*(float)cos(angleto/ (180.0f/3.1415f));
	y = m*(float)sin(angleto/ (180.0f/3.1415f));
}

float cVector2::getAngle()
{
	if(x == 0)
		x = 0.00001f;
	
	float angleto = -90;
	angleto = y / x;
	angleto = ((float)atan(angleto) * (180.0f/3.1415f));
	if (x < 0)
		angleto+=180;
	
	return angleto;
}

bool cVector2::operator==( cVector2 v2 )
{
	if (x == v2.x && y == v2.y)
		return true;
	return false;
}

float cVector2::DotProd( cVector2 v2 )
{
	return x*y + v2.x*v2.y;
}

cVector2 cVector2::perp()
{
	return cVector2(-y, x);
}

cMatrix4x4::cMatrix4x4()
{
	for(int i = 0; i < 16; i++)
		values[i] = (i-i/4)%4 == 0 ? 1.0f : 0.0f;
}

cMatrix4x4::cMatrix4x4(const cMatrix4x4 &otherMatrix)
{
	memcpy(values, otherMatrix.values, 16*4);
}

cMatrix4x4 cMatrix4x4::makeIdentity()
{
	cMatrix4x4 ret;
	for(int i = 0; i < 16; i++)
		ret.values[i] = (i-i/4)%4 == 0 ? 1.0f : 0.0f;
	return ret;
}
cMatrix4x4 cMatrix4x4::makeTranslation(float x, float y, float z)
{
	cMatrix4x4 ret = makeIdentity();
	ret.values[12] = x;
	ret.values[13] = y;
	ret.values[14] = z;
	return ret;
}
cMatrix4x4 cMatrix4x4::makeTranslation(cVector3 f)
{
	return makeTranslation(f.x, f.y, f.z);
}

cMatrix4x4 cMatrix4x4::makeRotation(float angle, float x, float y, float z)
{
	cMatrix4x4 ret;
	float len = x*x+y*y+z*z;
	if(len > 1)
	{
		len = (float)sqrt(len);
		x /= len;
		y /= len;
		z /= len;
	}
	float c = (float)cos(angle/(180/PI));
	float s = (float)sin(angle/(180/PI));

	ret.values[0] = x*x*(1-c)+c;
	ret.values[4] = x*y*(1-c)-z*s;
	ret.values[8] = x*z*(1-c)+y*s;
	ret.values[12] = 0;

	ret.values[1] = y*x*(1-c)+z*s;
	ret.values[5] = y*y*(1-c)+c;
	ret.values[9] = y*z*(1-c)-x*s;
	ret.values[13] = 0;

	ret.values[2] = z*x*(1-c)-y*s;
	ret.values[6] = z*y*(1-c)+x*s;
	ret.values[10]= z*z*(1-c)+c;
	ret.values[14] = 0;

	ret.values[3] = 0;
	ret.values[7] = 0;
	ret.values[11]= 0;
	ret.values[15]= 1;

	return ret;
}

cMatrix4x4 cMatrix4x4::makeScale(float x, float y, float z)
{
	cMatrix4x4 ret;
	ZeroMemory(ret.values, 16*sizeof(float));
	ret.values[0] = x;
	ret.values[5] = y;
	ret.values[10] = z;
	ret.values[15] = 1;
	return ret;
}

cMatrix4x4 cMatrix4x4::makeScale(cVector3 f)
{
	return makeScale(f.x, f.y, f.z);
}

cVector3 cMatrix4x4::operator *(const cVector3 &other)
{
	return cVector3(values[0]*other.x + values[4]*other.y+values[8]*other.z + values[12],
					values[1]*other.x + values[5]*other.y+values[9]*other.z + values[13],
					values[2]*other.x + values[6]*other.y+values[10]*other.z+ values[14]);
}
cMatrix4x4 cMatrix4x4::operator *(const cMatrix4x4 &other)
{
	cMatrix4x4 ret;
	ret.values[0] = values[0]*other.values[0] + values[4]*other.values[1] + values[8]*other.values[2] + values[12]*other.values[3];
	ret.values[4] = values[0]*other.values[4] + values[4]*other.values[5] + values[8]*other.values[6] + values[12]*other.values[7];
	ret.values[8] = values[0]*other.values[8] + values[4]*other.values[9] + values[8]*other.values[10]+ values[12]*other.values[11];
	ret.values[12]= values[0]*other.values[12]+ values[4]*other.values[13]+ values[8]*other.values[14]+ values[12]*other.values[15];

	ret.values[1] = values[1]*other.values[0] + values[5]*other.values[1] + values[9]*other.values[2] + values[13]*other.values[3];
	ret.values[5] = values[1]*other.values[4] + values[5]*other.values[5] + values[9]*other.values[6] + values[13]*other.values[7];
	ret.values[9] = values[1]*other.values[8] + values[5]*other.values[9] + values[9]*other.values[10]+ values[13]*other.values[11];
	ret.values[13]= values[1]*other.values[12]+ values[5]*other.values[13]+ values[9]*other.values[14]+ values[13]*other.values[15];

	ret.values[2] = values[2]*other.values[0] + values[6]*other.values[1] + values[10]*other.values[2] + values[14]*other.values[3];
	ret.values[6] = values[2]*other.values[4] + values[6]*other.values[5] + values[10]*other.values[6] + values[14]*other.values[7];
	ret.values[10]= values[2]*other.values[8] + values[6]*other.values[9] + values[10]*other.values[10]+ values[14]*other.values[11];
	ret.values[14]= values[2]*other.values[12]+ values[6]*other.values[13]+ values[10]*other.values[14]+ values[14]*other.values[15];

	ret.values[3] = values[3]*other.values[0] + values[7]*other.values[1] + values[11]*other.values[2] + values[15]*other.values[3];
	ret.values[7] = values[3]*other.values[4] + values[7]*other.values[5] + values[11]*other.values[6] + values[15]*other.values[7];
	ret.values[11]= values[3]*other.values[8] + values[7]*other.values[9] + values[11]*other.values[10]+ values[15]*other.values[11];
	ret.values[15]= values[3]*other.values[12]+ values[7]*other.values[13]+ values[11]*other.values[14]+ values[15]*other.values[15];
	return ret;
}
void cMatrix4x4::operator *=(const cMatrix4x4 &other)
{
	cMatrix4x4 newMatrix = *this * other;
	memcpy(values, newMatrix.values, 16*sizeof(float));
}


cMatrix4x4 cMatrix4x4::transpose()
{
	cMatrix4x4 ret;
	ret.values[0] = values[0];
	ret.values[1] = values[4];
	ret.values[2] = values[8];
	ret.values[3] = values[12];

	ret.values[4] = values[1];
	ret.values[5] = values[5];
	ret.values[6] = values[9];
	ret.values[7] = values[13];

	ret.values[8] = values[2];
	ret.values[9] = values[6];
	ret.values[10] = values[10];
	ret.values[11] = values[14];

	ret.values[12] = values[3];
	ret.values[13] = values[7];
	ret.values[14] = values[11];
	ret.values[15] = values[15];

	return ret;
}

cVector3 cMatrix4x4::getTranslation()
{
	return cVector3(values[12], values[13], values[14]);
}

cVector3 cMatrix4x4::getRotation()
{
	return cVector3(0,0,0);
}
cVector3 cMatrix4x4::getScale()
{
	return cVector3(values[0], values[5], values[10]);
}

void cMatrix4x4::setPosition( float x, float y, float z)
{
	cVector3 p = -getTranslation();
	operator*=(cMatrix4x4::makeTranslation(p));
	operator*=(cMatrix4x4::makeTranslation(x,y,z));
}

void cMatrix4x4::setPositionX(float x)
{
	float f = -getTranslation().x + x;
	operator*=(cMatrix4x4::makeTranslation(f,0,0));
}

void cMatrix4x4::setPositionY(float y)
{
	float f = -getTranslation().y + y;
	operator*=(cMatrix4x4::makeTranslation(0,f,0));
}

void cMatrix4x4::setPositionZ(float z)
{
	float f = -getTranslation().z + z;
	operator*=(cMatrix4x4::makeTranslation(0,0,f));
}

void cMatrix4x4::setScale(float x, float y, float z)
{
	values[0] = x;
	values[5] = y;
	values[10] = z;
}

void cMatrix4x4::setScaleX(float x)
{
	values[0] = x;
}
void cMatrix4x4::setScaleY(float y)
{
	values[5] = y;
}
void cMatrix4x4::setScaleZ(float z)
{
	values[10] = z;
}

void cMatrix4x4::setRotationX(float x)
{
	float rot = -getRotation().x + x;
	operator*=(cMatrix4x4::makeRotation(rot,1,0,0));
}

void cMatrix4x4::setRotationY(float y)
{
	float rot = -getRotation().y + y;
	operator*=(cMatrix4x4::makeRotation(rot,0,1,0));
}

void cMatrix4x4::setRotationZ(float z)
{
	float rot = -getRotation().z + z;
	operator*=(cMatrix4x4::makeRotation(rot,0,0,1));
}

cMatrix4x4 cMatrix4x4::removeTranslation()
{
	cMatrix4x4 ret(*this);
	ret.values[12] = 0;
	ret.values[13] = 0;
	ret.values[14] = 0;

	return ret;
}

void cMatrix4x4::setIdentity()
{
	for(int i = 0; i < 16; i++)
		values[i] = (i-i/4)%4 == 0 ? 1.0f : 0.0f;
}


cQuaternion::cQuaternion(float x, float y, float z, float w)
{
	values[0] = x;
	values[1] = y;
	values[2] = z;
	values[3] = w;
}
cQuaternion::cQuaternion( float *angles )
{
	fromAngles( angles );
}

cQuaternion::cQuaternion( cQuaternion& q1, cQuaternion& q2, float interp )
{
	slerp( q1, q2, interp );
}



void cQuaternion::fromAngles( float *angles )
{
	float angle;
	double sr, sp, sy, cr, cp, cy;
	
	angle = angles[2]*0.5f;
	sy = sin( angle );
	cy = cos( angle );
	angle = angles[1]*0.5f;
	sp = sin( angle );
	cp = cos( angle );
	angle = angles[0]*0.5f;
	sr = sin( angle );
	cr = cos( angle );
	
	double crcp = cr*cp;
	double srsp = sr*sp;
	
	values[0] = ( float )( sr*cp*cy-cr*sp*sy );
	values[1] = ( float )( cr*sp*cy+sr*cp*sy );
	values[2] = ( float )( crcp*sy-srsp*cy );
	values[3] = ( float )( crcp*cy+srsp*sy );
}

void cQuaternion::slerp( cQuaternion& q1, cQuaternion& q2, float interp )
{
	// Decide if one of the quaternions is backwards
	int i;
	float a = 0, b = 0;
	for ( i = 0; i < 4; i++ )
	{
		a += ( q1[i]-q2[i] )*( q1[i]-q2[i] );
		b += ( q1[i]+q2[i] )*( q1[i]+q2[i] );
	}
	if ( a > b )
		q2.inverse();
	
	float cosom = q1[0]*q2[0]+q1[1]*q2[1]+q1[2]*q2[2]+q1[3]*q2[3];
	double sclq1, sclq2;
	
	if (( 1.0+cosom ) > 0.00000001 )
	{
		if (( 1.0-cosom ) > 0.00000001 )
		{
			double omega = acos( cosom );
			double sinom = sin( omega );
			sclq1 = sin(( 1.0-interp )*omega )/sinom;
			sclq2 = sin( interp*omega )/sinom;
		}
		else
		{
			sclq1 = 1.0-interp;
			sclq2 = interp;
		}
		for ( i = 0; i < 4; i++ )
			values[i] = ( float )( sclq1*q1[i]+sclq2*q2[i] );
	}
	else
	{
		values[0] = -q1[1];
		values[1] = q1[0];
		values[2] = -q1[3];
		values[3] = q1[2];
		
		sclq1 = sin(( 1.0-interp )*0.5*PI );
		sclq2 = sin( interp*0.5*PI );
		for ( i = 0; i < 3; i++ )
			values[i] = ( float )( sclq1*q1[i]+sclq2*values[i] );
	}
}

void cQuaternion::inverse()
{
	values[0] = -values[0];
	values[1] = -values[1];
	values[2] = -values[2];
	values[3] = -values[3];
}

float cQuaternion::operator[]( int index )
{
	return values[index];
}

cMatrix4x4 cQuaternion::getRotationMatrix()
{
	cMatrix4x4 ret;

	ret.values[0] = ( float )( 1.0 - 2.0*values[1]*values[1] - 2.0*values[2]*values[2] );
	ret.values[1] = ( float )( 2.0*values[0]*values[1] + 2.0*values[3]*values[2] );
	ret.values[2] = ( float )( 2.0*values[0]*values[2] - 2.0*values[3]*values[1] );
	
	ret.values[4] = ( float )( 2.0*values[0]*values[1] - 2.0*values[3]*values[2] );
	ret.values[5] = ( float )( 1.0 - 2.0*values[0]*values[0] - 2.0*values[2]*values[2] );
	ret.values[6] = ( float )( 2.0*values[1]*values[2] + 2.0*values[3]*values[0] );
	
	ret.values[8] = ( float )( 2.0*values[0]*values[2] + 2.0*values[3]*values[1] );
	ret.values[9] = ( float )( 2.0*values[1]*values[2] - 2.0*values[3]*values[0] );
	ret.values[10] = ( float )( 1.0 - 2.0*values[0]*values[0] - 2.0*values[1]*values[1] );			

	return ret;
}

cQuaternion cQuaternion::normalize()
{
	double len = sqrt(x*x+y*y+z*z+w*w);
	return cQuaternion((float)(x/len),(float)(y/len),(float)(z/len),(float)(w/len));
}
