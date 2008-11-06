#ifndef __MYMATH_H__
#define __MYMATH_H__

#include <string.h>

#ifndef __COMMON_H__
#error "YOU MUST INCLUDE COMMON.H, NOT THIS FILE"
#endif

#include <math.h>
#ifndef PI
#	define PI	3.1415926535897932384626433832795
#endif

#ifndef PI_ON_180
#	define PI_ON_180	(PI/180.0)
#endif

// This is our basic 3D point/vector class
struct cVector3
{
public:
	
	// A default constructor
	cVector3() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	cVector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together 
	cVector3 operator+(cVector3 vVector)
	{
		// Return the added vectors result.
		return cVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// Here we overload the - operator so we can subtract vectors 
	cVector3 operator-(cVector3 vVector)
	{
		// Return the subtracted vectors result
		return cVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}
	
	cVector3 operator-(float f)
	{
		// Return the subtracted vectors result
		return cVector3(x - f, y - f, z - f);
	}
	cVector3 operator+(float f)
	{
		// Return the subtracted vectors result
		return cVector3(x + f, y + f, z + f);
	}
	// Here we overload the * operator so we can multiply by scalars
	cVector3 operator*(float num)
	{
		// Return the scaled vector
		return cVector3(x * num, y * num, z * num);
	}
	float &operator [](int num)
	{
		if (num == 0)
			return x;
		if (num == 1)
			return y;
		else
			return z;
	}

	// Here we overload the / operator so we can divide by a scalar
	cVector3 operator/(float num)
	{
		// Return the scale vector
		return cVector3(x / num, y / num, z / num);
	}

	bool operator==(cVector3 v2)
	{
		if (x == v2.x && y == v2.y && z == v2.z)
			return true;
		return false;
	}

	cVector3 normalize()
	{
		float m = magnitude();
		if (m != 0)
			return (*this / magnitude());
		else
			return *this;
	}
	float magnitude()
	{
		return (float)sqrt( (x * x) + 
							(y * y) + 
							(z * z) );
	}

	cVector3& operator+=(const cVector3 &rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        z = z + rhs.z;
        return *this;
    }

	float dot(cVector3 v2)
	{
		/*cVector3 vNormal;	
		vNormal.x = ((vVector1.y * z) - (vVector1.z * y));
		vNormal.y = ((vVector1.z * x) - (vVector1.x * z));
		vNormal.z = ((vVector1.x * y) - (vVector1.y * x));
		return vNormal;		*/
		return x*v2.x+y*v2.y+z*v2.z;
	}


	cVector3 cross(cVector3 v2)
	{
		return cVector3(y*v2.z - z*v2.y,
						z*v2.x - x*v2.z,
						x*v2.y - y*v2.x);
	}

	float x, y, z;						
};


// This is our basic 2D point class.  This will be used to store the UV coordinates.
class cVector2 
{
public:

		// A default constructor
	cVector2() {}

	// This is our constructor that allows us to initialize our data upon creating an instance
	cVector2(float X, float Y) 
	{ 
		x = X; y = Y;
	}

	// Here we overload the + operator so we can add vectors together 
	cVector2 operator+(cVector2 vVector)
	{
		// Return the added vectors result.
		return cVector2(vVector.x + x, vVector.y + y);
	}

	// Here we overload the - operator so we can subtract vectors 
	cVector2 operator-(cVector2 vVector)
	{
		// Return the subtracted vectors result
		return cVector2(x - vVector.x, y - vVector.y);
	}
	
	// Here we overload the * operator so we can multiply by scalars
	cVector2 operator*(float num)
	{
		// Return the scaled vector
		return cVector2(x * num, y * num);
	}

	// Here we overload the / operator so we can divide by a scalar
	cVector2 operator/(float num)
	{
		// Return the scale vector
		return cVector2(x / num, y / num);
	}

	cVector2 Normalize()
	{
		float m = Magnitude();
		if (m != 0)
			return (*this / m);
		else
			return *this;
	}
	float Magnitude()
	{
		return (float)sqrt( (x * x) + 
							(y * y));
	}
	void rotate(float angleinc)
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

	float getAngle()
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

	bool operator==(cVector2 v2)
	{
		if (x == v2.x && y == v2.y)
			return true;
		return false;
	}
	float DotProd(cVector2 v2)
	{
		return x*y + v2.x*v2.y;
	}

	cVector2 perp()
	{
		return cVector2(-y, x);
	}

	float x, y;
};





class Quaternion
{
	public:
		Quaternion( float *angles ) { fromAngles( angles ); }
		Quaternion( Quaternion& q1, Quaternion& q2, float interp ) { slerp( q1, q2, interp ); }
		void fromAngles( float *angles )
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

			m_quat[0] = ( float )( sr*cp*cy-cr*sp*sy );
			m_quat[1] = ( float )( cr*sp*cy+sr*cp*sy );
			m_quat[2] = ( float )( crcp*sy-srsp*cy );
			m_quat[3] = ( float )( crcp*cy+srsp*sy ); 
		}

		void slerp( Quaternion& q1, Quaternion& q2, float interp )
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
					m_quat[i] = ( float )( sclq1*q1[i]+sclq2*q2[i] );
			}
			else
			{
				m_quat[0] = -q1[1];
				m_quat[1] = q1[0];
				m_quat[2] = -q1[3];
				m_quat[3] = q1[2];

				sclq1 = sin(( 1.0-interp )*0.5*PI );
				sclq2 = sin( interp*0.5*PI );
				for ( i = 0; i < 3; i++ )
					m_quat[i] = ( float )( sclq1*q1[i]+sclq2*m_quat[i] );
			}
		}
		float operator[]( int index ) { return m_quat[index]; }
		void inverse()
		{
			m_quat[0] = -m_quat[0];
			m_quat[1] = -m_quat[1];
			m_quat[2] = -m_quat[2];
			m_quat[3] = -m_quat[3];
		}
	private:
		float m_quat[4];
};




class Matrix
{
	public:
		Matrix()
		{
			loadIdentity();
		}
		~Matrix()
		{
		}
		void loadIdentity()
		{
			memset( m_matrix, 0, sizeof( float )*16 );
			m_matrix[0] = m_matrix[5] = m_matrix[10] = m_matrix[15] = 1;
		}
		void set( float *matrix )
		{
			memcpy( m_matrix, matrix, sizeof( float )*16 );
		}
		void postMultiply( Matrix& matrix )
		{
			float newMatrix[16];
			float *m1 = m_matrix, *m2 = matrix.m_matrix;

			newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
			newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
			newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
			newMatrix[3] = 0;

			newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
			newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
			newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
			newMatrix[7] = 0;

			newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
			newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
			newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
			newMatrix[11] = 0;

			newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
			newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
			newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
			newMatrix[15] = 1;

			set( newMatrix );
		}
		void setTranslation( float *translation )
		{
			m_matrix[12] = translation[0];
			m_matrix[13] = translation[1];
			m_matrix[14] = translation[2];
		}
		void setInverseTranslation( float *translation )
		{
			m_matrix[12] = -translation[0];
			m_matrix[13] = -translation[1];
			m_matrix[14] = -translation[2];
		}
		void setRotationRadians( float *angles )
		{
			double cr = cos( angles[0] );
			double sr = sin( angles[0] );
			double cp = cos( angles[1] );
			double sp = sin( angles[1] );
			double cy = cos( angles[2] );
			double sy = sin( angles[2] );

			m_matrix[0] = ( float )( cp*cy );
			m_matrix[1] = ( float )( cp*sy );
			m_matrix[2] = ( float )( -sp );

			double srsp = sr*sp;
			double crsp = cr*sp;

			m_matrix[4] = ( float )( srsp*cy-cr*sy );
			m_matrix[5] = ( float )( srsp*sy+cr*cy );
			m_matrix[6] = ( float )( sr*cp );

			m_matrix[8] = ( float )( crsp*cy+sr*sy );
			m_matrix[9] = ( float )( crsp*sy-sr*cy );
			m_matrix[10] = ( float )( cr*cp );
		}
		void setRotationDegrees( float *angles )
		{
			float vec[3];
			vec[0] = ( float )( angles[0]*180.0/PI );
			vec[1] = ( float )( angles[1]*180.0/PI );
			vec[2] = ( float )( angles[2]*180.0/PI );
			setRotationRadians( vec );
		}
		void setRotationQuaternion( Quaternion& quat )
		{
			m_matrix[0] = ( float )( 1.0 - 2.0*quat[1]*quat[1] - 2.0*quat[2]*quat[2] );
			m_matrix[1] = ( float )( 2.0*quat[0]*quat[1] + 2.0*quat[3]*quat[2] );
			m_matrix[2] = ( float )( 2.0*quat[0]*quat[2] - 2.0*quat[3]*quat[1] );

			m_matrix[4] = ( float )( 2.0*quat[0]*quat[1] - 2.0*quat[3]*quat[2] );
			m_matrix[5] = ( float )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[2]*quat[2] );
			m_matrix[6] = ( float )( 2.0*quat[1]*quat[2] + 2.0*quat[3]*quat[0] );

			m_matrix[8] = ( float )( 2.0*quat[0]*quat[2] + 2.0*quat[3]*quat[1] );
			m_matrix[9] = ( float )( 2.0*quat[1]*quat[2] - 2.0*quat[3]*quat[0] );
			m_matrix[10] = ( float )( 1.0 - 2.0*quat[0]*quat[0] - 2.0*quat[1]*quat[1] );
		}
		void setInverseRotationRadians( float *angles )
		{
			double cr = cos( angles[0] );
			double sr = sin( angles[0] );
			double cp = cos( angles[1] );
			double sp = sin( angles[1] );
			double cy = cos( angles[2] );
			double sy = sin( angles[2] );

			m_matrix[0] = ( float )( cp*cy );
			m_matrix[4] = ( float )( cp*sy );
			m_matrix[8] = ( float )( -sp );

			double srsp = sr*sp;
			double crsp = cr*sp;

			m_matrix[1] = ( float )( srsp*cy-cr*sy );
			m_matrix[5] = ( float )( srsp*sy+cr*cy );
			m_matrix[9] = ( float )( sr*cp );

			m_matrix[2] = ( float )( crsp*cy+sr*sy );
			m_matrix[6] = ( float )( crsp*sy-sr*cy );
			m_matrix[10] = ( float )( cr*cp );
		}
		void setInverseRotationDegrees( float *angles )
		{
			float vec[3];
			vec[0] = ( float )( angles[0]*180.0/PI );
			vec[1] = ( float )( angles[1]*180.0/PI );
			vec[2] = ( float )( angles[2]*180.0/PI );
			setInverseRotationRadians( vec );
		}
		float *getMatrix() { return m_matrix; }
		void inverseTranslateVect( float *pVect )
		{
			pVect[0] = pVect[0]-m_matrix[12];
			pVect[1] = pVect[1]-m_matrix[13];
			pVect[2] = pVect[2]-m_matrix[14];
		}
		void inverseRotateVect( float *pVect )
		{
			float vec[3];
			vec[0] = pVect[0]*m_matrix[0]+pVect[1]*m_matrix[1]+pVect[2]*m_matrix[2];
			vec[1] = pVect[0]*m_matrix[4]+pVect[1]*m_matrix[5]+pVect[2]*m_matrix[6];
			vec[2] = pVect[0]*m_matrix[8]+pVect[1]*m_matrix[9]+pVect[2]*m_matrix[10];
			memcpy( pVect, vec, sizeof( float )*3 );
		}
	private:
		float m_matrix[16];
};



#endif
