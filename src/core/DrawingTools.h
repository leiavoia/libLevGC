#ifndef DRAWING_TOOLS_H
#define DRAWING_TOOLS_H

#include <stack>
#include <GL/gl.h>

namespace LGC {


inline void SetColor(float r, float g, float b, float a) { glColor4f(r,g,b,a); }


class ColorQuad {
public:
	float R,G,B,A;
	ColorQuad( float R, float G, float B, float A ): R(R), G(G), B(B), A(A) {}
	ColorQuad(): R(0), G(0), B(0), A(0) {}
	bool IsClear() {
		return (!R && !G && !B && !A);
		}
	void MakeBlack() {
		R = G = B = 0;
		A = 1;
		}
	void MakeWhite() {
		R = G = B = 1;
		A = 1;
		}
	void MakeWhitePlus() {
		R = G = B = 1.5;
		A = 1;
		}
	void MakeGrey() {
		R = G = B = 0.5;
		A = 1;
		}
	void MakeClear() {
		R = G = B = A = 0;
		}
	bool operator==( const ColorQuad &c ) const {
		return R==c.R && G==c.G && B==c.B && A==c.A;
		}
	};




// requires the color set beforehand
void DrawQuad( float x, float y, float w, float h );
void DrawQuadByCoords( float x1, float y1, float x2, float y2 );




class ScissorStack {
public:
	ScissorStack() {}
	~ScissorStack() {}
	void Push( int x, int y, int w, int h );
	void Pop();

private:
	struct rect {
		int x1,y1,x2,y2;
		};

	std::stack< rect > stack;
	};



} // end namespace LGC


#endif










