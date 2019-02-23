
#include "Vector.h"
#include "PolygonTools.h"


#define ARGB_A(u) (((u)>>24) & 0x000000FF)
#define ARGB_R(u) (((u)>>16) & 0x000000FF)
#define ARGB_G(u) (((u)>> 8) & 0x000000FF)
#define ARGB_B(u) (((u)>> 0) & 0x000000FF)
 



namespace LGC {

namespace PolygonTools {


void BuildBox( std::vector<Vector> &vertices, float width, float height ) {
	vertices.push_back( Vector(0,0) );
	vertices.push_back( Vector(width,0) );
	vertices.push_back( Vector(width,height) );
	vertices.push_back( Vector(0,height) );
	}

	
	
void BuildBlob(std::vector<Vector> &vertices, int iNumVertices, float radius) {
	for(int i = 0; i < iNumVertices; i ++) {
		float a = 2.0f * Pi() * (i / (float) iNumVertices);
		vertices.push_back(   Vector( cos(a), sin(a) )  *  radius   );
		}
	}

	
	
void Render(
	const Vector& xOffset, 
	std::vector<Vector> &vertices,
	bool filled
	) {
	
	if ( vertices.empty() ) return;

	glBindTexture( GL_TEXTURE_2D, 0 );
	
	glPushMatrix();
	glTranslatef(xOffset.x, xOffset.y, 0.0f);
	
	if (filled) {
		glColor4f(1,0.4,0.4,0.4);
		glBegin(GL_TRIANGLE_FAN);
	
		for(unsigned int i = 0; i < vertices.size(); i ++) {
			glVertex2f(vertices[i].x, vertices[i].y);
			}
			
		glVertex2f(vertices[0].x, vertices[0].y);
		
		glEnd();
		}
	
	// create outline
	glColor4f(1,0.7,0.7,1);
	glBegin(GL_LINE_LOOP);
	
	for(unsigned int i = 0; i < vertices.size(); i ++) {
		glVertex2f(vertices[i].x, vertices[i].y);
		}
		
	glVertex2f(vertices[0].x, vertices[0].y);
	glEnd();
		
		
	glPopMatrix();
	}	




// calculate the projection range of a polygon along an axis
void GetInterval(std::vector<Vector> &vertices, const Vector& xAxis, float& min, float& max) {
	min = max = (vertices[0] * xAxis);

	for(unsigned int i = 1; i < vertices.size(); i ++) {
		float d = (vertices[i] * xAxis);
		if (d < min) min = d; else if (d > max) max = d;
		}
	}

	
			   
bool IntervalIntersect(
	std::vector<Vector> &A,
	std::vector<Vector> &B,
	const Vector& xAxis, 
	const Vector& xOffset, 
	const Vector& xVel, 
	float& taxis, 
	float tmax)
	{
	
	float min0, max0;
	float min1, max1;
	GetInterval(A, xAxis, min0, max0);
	GetInterval(B, xAxis, min1, max1);

	float h = xOffset * xAxis;
	min0 += h;
	max0 += h;

	float d0 = min0 - max1; // if overlapped, do < 0
	float d1 = min1 - max0; // if overlapped, d1 > 0

	// separated, test dynamic intervals
	if (d0 > 0.0f || d1 > 0.0f) {
		float v = xVel * xAxis;

		// small velocity, so only the overlap test will be relevant. 
		if (fabs(v) < 0.0000001f)
			return false;

		float t0 =-d0 / v; // time of impact to d0 reaches 0
		float t1 = d1 / v; // time of impact to d0 reaches 1

		if (t0 > t1) { float temp = t0; t0 = t1; t1 = temp; }
		taxis  = (t0 > 0.0f)? t0 : t1;

		if (taxis < 0.0f || taxis > tmax)
			return false;

		return true;
		}
	else {
		// overlap. get the interval, as a the smallest of |d0| and |d1|
		// return negative number to mark it as an overlap
		taxis = (d0 > d1)? d0 : d1;
		return true;
		}
		
	return false;
	}
	
	
	

bool FindMTD(Vector* xAxis, float* taxis, int iNumAxes, Vector& N, float& t) {
	// find collision first
	int mini = -1;
	t = 0.0f;
	for(int i = 0; i < iNumAxes; i ++) {	
		if (taxis[i] > 0) {
			if (taxis[i] > t) {
				mini = i;
				t = taxis[i];
				N = xAxis[i];
				N.Normalise();
				}
			}
		}

	// found one
	if (mini != -1) { return true; }

	// nope, find overlaps
	mini = -1;
	for(int i = 0; i < iNumAxes; i ++) {
		float n = xAxis[i].Normalise();
		taxis[i] /= n;

		if (taxis[i] > t || mini == -1) {
			mini = i;
			t = taxis[i];
			N = xAxis[i];
			}
		}

	if (mini == -1) { printf("Error\n"); }

	return (mini != -1);
	}



		   
					   
					   
bool Collide(
	std::vector<Vector> &A, 
	std::vector<Vector> &B,
	Vector& xOffset,
	const Vector& xVel, 
	Vector& N, 
	float& t,
	bool adjust
	) {
	
	if ( A.empty() || B.empty() ) { return false; }

	// All the separation axes
	Vector xAxis[64]; // note : a maximum of 32 vertices per poly is supported
	float  taxis[64];
	int    iNumAxes=0;

	xAxis[iNumAxes] = Vector(-xVel.y, xVel.x);

	float fVel2 = xVel * xVel;

	if (fVel2 > 0.000001f) {
		if (  !IntervalIntersect( A, B, xAxis[iNumAxes], xOffset, xVel, taxis[iNumAxes], t)  ) {
			return false;
			}
		iNumAxes++;
		}

	// test separation axes of A
	for(unsigned int j = A.size()-1, i = 0; i < A.size(); j = i, i ++) {
		Vector E0 = A[j];
		Vector E1 = A[i];
		Vector E  = E1 - E0;
		xAxis[iNumAxes] = Vector(-E.y, E.x);

		if (  !IntervalIntersect(A, B, xAxis[iNumAxes],  xOffset, xVel,  taxis[iNumAxes], t)  ) {
			return false;
			}

		iNumAxes++;
		}

	// test separation axes of B
	for(unsigned int j = B.size()-1, i = 0; i < B.size(); j = i, i ++) {
		Vector E0 = B[j];
		Vector E1 = B[i];
		Vector E  = E1 - E0;
		xAxis[iNumAxes] = Vector(-E.y, E.x);

		if (  !IntervalIntersect(A, B, xAxis[iNumAxes], xOffset, xVel,  taxis[iNumAxes], t)  ) {
			return false;
			}

		iNumAxes++;
		}

	// special case for segments
	if (B.size() == 2) {
		Vector E  = B[1] - B[0];
		xAxis[iNumAxes] = E;
		
		if (  !IntervalIntersect( A, B, xAxis[iNumAxes],  xOffset, xVel, taxis[iNumAxes], t)  ) {
			return false;
			}

		iNumAxes++;
		}

	// special case for segments
	if (A.size() == 2) {
		Vector E  = A[1] - A[0];
		xAxis[iNumAxes] = E;
		
		if (  !IntervalIntersect( A, B,  xAxis[iNumAxes], xOffset, xVel, taxis[iNumAxes], t)  ) {
			return false;
			}

		iNumAxes++;
		}

	// SLIDE PREVENTION: 
	// Check to make sure the separation axis is not too slanty
	//if () {}
	
	if (!FindMTD(xAxis, taxis, iNumAxes, N, t)) { return false; }

	// make sure the polygons gets pushed away from each other.
	if ( adjust && (N * xOffset < 0.0f) ) { N = -N; }

	return true;
	}

	
	
	
	
	
	
} // end namespace LGC	


} // end namespace
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
