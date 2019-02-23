
// DOT PRODUCT = [*]

#ifndef LGC_Vector3_H
#define LGC_Vector3_H

// #include <iostream>
#include <math.h>


namespace LGC {


class Vector3 {

   	public:
   
	float x,y,z;				
	
	
	// constructors
	Vector3() { x=y=z=0; }			
	Vector3(const float pX, const float pY, const float pZ) : x(pX), y(pY), z(pZ) {}

	// copy constructor
	Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}
	
	// assignment
	Vector3& operator=( const Vector3 &v ) { x=v.x; y=v.y; z=v.z; return *this; }
	
	// comparison
	bool operator==( const Vector3 &v ) const { return x==v.x && y==v.y && z==v.z; }
	bool operator!=( const Vector3 &v ) const { return x!=v.x || y!=v.y || z!=v.z; }
		
	// clear
	void Zero() { x = y = z = 0.0f; }
			
	// unary arithmatic
	Vector3 operator-() const { return Vector3(-x,-y,-z); }
	
	// binary arithmatic
	Vector3 operator+(const Vector3 pV) const { Vector3 out( x+pV.x, y+pV.y, z+pV.z ); return out; }
	Vector3 operator-(const Vector3 pV) const { Vector3 out( x-pV.x, y-pV.y, z-pV.z ); return out; }
	Vector3& operator+=(const Vector3 pV) { x+=pV.x; y+=pV.y; z+=pV.z; return *this; }
	Vector3& operator-=(const Vector3 pV) { x-=pV.x; y-=pV.y; z-=pV.z; return *this; }

	// dot product
	float Dot(const Vector3 pV) const { return x*pV.x + y*pV.y + z*pV.z; }
	float operator*(const Vector3 pV) const { return x*pV.x + y*pV.y + z*pV.z; }
	friend float Dot( const Vector3 p1, const Vector3 p2 ) { return p1.Dot(p2); }
	
	// cross product
	Vector3 Cross( const Vector3 pV ) const { Vector3 out( y*pV.z-z*pV.y, z*pV.x-x*pV.z, x*pV.y-y*pV.x ); return out; }
	friend Vector3 Cross( const Vector3 p1, const Vector3 p2 ) { return p1.Cross(p2); }

	// multiplication
	Vector3 operator*(const float pR) const { Vector3 out( x*pR, y*pR, z*pR ); return out; }
	friend Vector3 operator*(const float pR, const Vector3 pV) { Vector3 out( pV.x*pR, pV.y*pR, pV.z*pR ); return out; }
	Vector3& operator*=(const float pR) { x*=pR; y*=pR; z*=pR; return *this; }
		
		
	// magnitude
	float LenSqr(void) const { return x*x+y*y+z*z; }
	float Length(void) const { return sqrt(LenSqr()); }
	void Normalize() {
		float length = this->Length();
		this->x /= length;
		this->y /= length;
		this->z /= length;
		}
		
		
	// projection (component of *this parallel to pV)
	// Note: component perpendicular to pV is:  *this - Proj(pV)
	Vector3 Proj( const Vector3 pV ) { return Vector3(  pV * (this->Dot(pV) / pV.LenSqr())  ); }

	// cosine of the angle between two Vector3s:
	float CosAng( const Vector3 pV ) { return this->Dot(pV) / (Length() * pV.Length()); }



	// input/output

// 	friend ostream& operator<<(ostream& pStr, const Vector3& pV)
// 	{ return (pStr << '(' << pV.x << ',' << pV.y << ',' << pV.z << ')'); }
// 
// 	friend istream& operator>>(istream pStr, Vector3& pV)
// 	{ return (pStr >> pV.x >> pV.y >> pV.z); }


	};


} // end namespace LGC

#endif
