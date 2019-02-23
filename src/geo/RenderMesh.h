#ifndef LGC_RENDERMESH_H
#define LGC_RENDERMESH_H


#include "Vertex.h"
#include "Triangle.h"
#include "../core/RenderingLayer.h"

#include <map>


namespace LGC {



class RenderMesh: public RenderObject {

public: // -------------\/---------------
	
	RenderMesh() { }
		
	virtual ~RenderMesh() { }
	
	virtual void Render( RenderingLayer* x );
	
	void AddTriangle( Vertex p1, Vertex p2, Vertex p3 );
		
	bool Empty() { return triangles.empty(); }
	
	void Report();
	
	void CreateAverageVertexNormals();
	
protected: // -------------\/---------------
	
	unsigned int GetVertexIndex( Vertex& v );
		
	TriangleList triangles;
	
	VertexList vertexes;
	
	// maps vertex indexes to triangle indexes
	std::multimap< unsigned int, unsigned int > connectivity;
	
	void LogVertexConnections( unsigned int triangle_index );
	
	friend class RenderingLayer; // rendering layer needs full access to data
	
	typedef std::multimap< unsigned int, unsigned int >::iterator connectivity_iter;
	typedef std::pair< connectivity_iter, connectivity_iter > connectivity_pair;
	
	};

	
} // end namespace LGC


#endif


