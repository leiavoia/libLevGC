
// DOT PRODUCT = [*]

#ifndef LGC_VERTEX_H
#define LGC_VERTEX_H


#include "Vector3.h"
#include "../core/DrawingTools.h"
#include <vector>

namespace LGC {



class Vertex {
   	public:
   	inline float X() { return pos.x; }
   	inline float Y() { return pos.y; }
   	inline float Z() { return pos.z; }
   	inline float R() { return color.R; }
   	inline float G() { return color.G; }
   	inline float B() { return color.B; }
   	inline float A() { return color.A; }
   	inline int TextureIndex() { return texture_index; }
   	inline float TextureX() { return texture_x; }
   	inline float TextureY() { return texture_y; }
   	inline float U() { return texture_x; }
   	inline float V() { return texture_y; }
   	
   	Vertex( Vector3& pos ) : pos(pos), texture_index(0), texture_x(0), texture_y(0) {
   	
   		}
   		
    	Vertex( float x, float y, float z ) : pos( Vector3(x,y,z) ), texture_index(0), texture_x(0), texture_y(0) {
   		color.MakeWhite();
   		}
   		   		
    	Vertex( float x, float y, float z, ColorQuad color ) : pos( Vector3(x,y,z) ), color(color), texture_index(0), texture_x(0), texture_y(0) {
   	
   		}
   		  		
    	Vertex( float x, float y, float z, ColorQuad color, int texture, float tx, float ty ) : pos( Vector3(x,y,z) ), color(color), texture_index(texture), texture_x(tx), texture_y(ty) {
   	
   		}
   		
	Vector3 pos;		
	
	Vector3 normal;
	
	ColorQuad color;
	
	int texture_index;
	float texture_x;
	float texture_y;
	
	// material? or put in renderobject?
	
	// texture
	
	// bone weights
	
	bool operator==( const Vertex &v ) const { 
		// TODO: test the other attributes later 
		return 
			pos == v.pos
			&& 
			color == v.color
			&&
			texture_index == v.texture_index
			&&
			texture_x == v.texture_x
			&&
			texture_y == v.texture_y
			&&
			normal == v.normal
			; 
		}
		
		
	};




typedef std::vector<Vertex> VertexList;



} // end namespace LGC


#endif
