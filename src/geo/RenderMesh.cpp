#include "RenderMesh.h"

#include <vector>
#include <algorithm>

// testing only
#include <iostream>


class RenderingLayer; // forward dec

namespace LGC {



void RenderMesh::Render( RenderingLayer* x ) {
	x->Render( this );
	}

	
void RenderMesh::AddTriangle( Vertex p1, Vertex p2, Vertex p3 ) {
	triangles.push_back(
		Triangle(
			GetVertexIndex( p1 ),
			GetVertexIndex( p2 ),
			GetVertexIndex( p3 )
			)
		);
	LogVertexConnections( triangles.size() - 1 );
	}
	
	
unsigned int RenderMesh::GetVertexIndex( Vertex& v ) {
	// TODO: find-from-end would be more efficient
	// ??? VertexList::iterator iter = std::find( vertexes.rbegin(), vertexes.rend(), v );
	VertexList::iterator iter = std::find( vertexes.begin(), vertexes.end(), v );
	// found existing index
	if ( iter != vertexes.end() ) {
		return iter - vertexes.begin();
		}
	// create new index
	else {
		vertexes.push_back( v );
		return vertexes.size() - 1;
		}
	}
 
 
void RenderMesh::LogVertexConnections( unsigned int triangle_index ) {
	Triangle t = triangles[triangle_index];
	connectivity.insert( std::pair<int, int>( t.p1, triangle_index ) );
	connectivity.insert( std::pair<int, int>( t.p2, triangle_index ) );
	connectivity.insert( std::pair<int, int>( t.p3, triangle_index ) );
	}


void RenderMesh::CreateAverageVertexNormals() {
	// foreach unique vertex
	for ( unsigned int vi = 0; vi < vertexes.size(); ++vi ) {
		unsigned int num = connectivity.count( vi );
		std::vector< Vector3 > vectors;
		Vector3 normal;
		// find all the triangles this vertex schmoozes with and get the face normals
		connectivity_pair range = connectivity.equal_range( vi );
		for ( connectivity_iter ti = range.first; ti != range.second; ++ti ) {
      		Triangle t = triangles[ ti->second ];
			Vector3 v1 = Vector3( vertexes[t.p1].X() - vertexes[t.p2].X(), vertexes[t.p1].Y() - vertexes[t.p2].Y(), vertexes[t.p1].Z() - vertexes[t.p2].Z() );
      		Vector3 v2 = Vector3( vertexes[t.p1].X() - vertexes[t.p3].X(), vertexes[t.p1].Y() - vertexes[t.p3].Y(), vertexes[t.p1].Z() - vertexes[t.p3].Z() );
			v1.Normalize();
			v2.Normalize();
      		vectors.push_back(  v1.Cross( v2 )  );
      		}
      	// face normals acquired - now average and normalize them
      	for ( std::vector< Vector3 >::iterator i = vectors.begin(); i != vectors.end(); i++ ) {
      		normal.x += i->x;
      		normal.y += i->y;
      		normal.z += i->z;
      		}
		normal.x /= float(num);
		normal.y /= float(num);
		normal.z /= float(num);
		normal.Normalize();
		vertexes[vi].normal = normal;
		}
	}
	
	
void RenderMesh::Report() {
	std::cout << triangles.size() << " triangles, " << vertexes.size() << " vertexes\n";
	}
	
} // end namespace LGC
