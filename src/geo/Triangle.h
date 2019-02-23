
// DOT PRODUCT = [*]

#ifndef LGC_TRIANGLE_H
#define LGC_TRIANGLE_H


// #include <Vertex.h>
#include <vector>


namespace LGC {




class Triangle {
	public:
	// indexes into a list of vertices
	unsigned int p1;
	unsigned int p2;
	unsigned int p3;
	
	Triangle() : p1(0), p2(0), p3(0) {}
	Triangle( unsigned int p1, unsigned int p2, unsigned int p3 ) : p1(p1), p2(p2), p3(p3) {}
	
	};

typedef std::vector<Triangle> TriangleList;



} // end namespace LGC



#endif
