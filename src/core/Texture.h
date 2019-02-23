#ifndef DRAWING_H
#define DRAWING_H

#include <string>
#include <GL/gl.h>

namespace LGC {




// ===============================================================================


// This stores a rectangle. As a template, you can use any kind of number type
template< class TYPE > class AreaRect {
public:
	TYPE x1;
	TYPE y1;
	TYPE x2;
	TYPE y2;
	
	AreaRect( TYPE x, TYPE y, TYPE x2, TYPE y2 ): x1(x), y1(y), x2(x2), y2(y2) {}
	AreaRect(): x1(0), y1(0), x2(0), y2(0) {}
	
	inline TYPE Width() { return x2 - x1; }
	inline TYPE Height() { return y2 - y1; }
	inline bool Valid() { return !(!x1 && !x2 && !y1 && !y2); }
		
	inline AreaRect<TYPE>& operator /= (const AreaRect<TYPE>& r) {
		x1 / r.x1;
		x2 / r.x2;
		y1 / r.y1;
		y2 / r.y2;
		return *this;
		}
	inline AreaRect<TYPE>& operator += (const AreaRect<TYPE>& r) {
		x1 + r.x1;
		x2 + r.x2;
		y1 + r.y1;
		y2 + r.y2;
		return *this;
		}
	inline AreaRect<TYPE>& operator -= (const AreaRect<TYPE>& r) {
		x1 - r.x1;
		x2 - r.x2;
		y1 - r.y1;
		y2 - r.y2;
		return *this;
		}
	inline AreaRect<TYPE>& operator *= (const AreaRect<TYPE>& r) {
		x1 * r.x1;
		x2 * r.x2;
		y1 * r.y1;
		y2 * r.y2;
		return *this;
		}	
		
	inline AreaRect<TYPE> operator / (const AreaRect<TYPE>& r) {
		return AreaRect<TYPE> (
		x1 / r.x1,
		x2 / r.x2,
		y1 / r.y1,
		y2 / r.y2 );
		}
	inline AreaRect<TYPE> operator + (const AreaRect<TYPE>& r) {
		return AreaRect<TYPE> (
		x1 + r.x1,
		x2 + r.x2,
		y1 + r.y1,
		y2 + r.y2 );
		}
	inline AreaRect<TYPE> operator - (const AreaRect<TYPE>& r) {
		return AreaRect<TYPE> (
		x1 - r.x1,
		x2 - r.x2,
		y1 - r.y1,
		y2 - r.y2 );
		}
	inline AreaRect<TYPE> operator * (const AreaRect<TYPE>& r) {
		return AreaRect<TYPE> (
		x1 * r.x1,
		x2 * r.x2,
		y1 * r.y1,
		y2 * r.y2 );
		}	
	inline bool operator == (const AreaRect<TYPE>& r) {
		return  (
		x1 == r.x1 &&
		x2 == r.x2 &&
		y1 == r.y1 &&
		y2 == r.y2 );
		}	
	};
	
	
	
	
	

// ===============================================================================




enum BlendMode {
	BLEND_NORMAL,
	BLEND_NONE,
	BLEND_ADD,
	BLEND_SUBTRACT
	};
		
		
		
// ===============================================================================





class Texture {
public:

	Texture( 
		std::string name, 
		int texture_id, 
		AreaRect<int>& texture_coords, 
		AreaRect<int>& area_coords  
		)  
		: name(name), id (texture_id), pixels(area_coords)		
		{
		ratios.x1 = float(area_coords.x1) / float(texture_coords.x2);
		ratios.x2 = float(area_coords.x2) / float(texture_coords.x2);
		ratios.y1 = float(area_coords.y1) / float(texture_coords.y2);
		ratios.y2 = float(area_coords.y2) / float(texture_coords.y2);
		is_tilable = ( area_coords == texture_coords );
		}

	Texture() : name("NO_HANDLE"), id(0), pixels(AreaRect<int>()), ratios(AreaRect<float>()), is_tilable(false) {} 
		 	
	~Texture() {};

	// allows textures to draw correctly with roto/zoom
	static void SetScreenOffset( float x, float y ) {
		screen_x = x;
		screen_y = y;
		}
	
	// Drawing
	void Blit						( float x, float y, BlendMode blend=BLEND_NORMAL );
	void BlitFromCenter				( float x, float y, BlendMode blend=BLEND_NORMAL );
	void BlitFlip					( float x, float y, bool flip_horz=false, bool flip_vert=false, BlendMode blend=BLEND_NORMAL );
	void BlitFlipFromCenter			( float x, float y, bool flip_horz=false, bool flip_vert=false, BlendMode blend=BLEND_NORMAL );		
	void BlitRotoZoom				( float x, float y, float rot, float scale=1.0, BlendMode blend=BLEND_NORMAL );
	void BlitRotoZoomFromCenter		( float x, float y, float rot, float scale=1.0, BlendMode blend=BLEND_NORMAL );	
	void BlitRotoZoomFlip			( float x, float y, float rot, float scale=1.0, bool flip_horz=false, bool flip_vert=false, BlendMode blend=BLEND_NORMAL );
	void BlitRotoZoomFlipFromCenter	( float x, float y, float rot, float scale=1.0, bool flip_horz=false, bool flip_vert=false, BlendMode blend=BLEND_NORMAL );	
	void BlitCustomStretch			( float x, float y, float w, float h, BlendMode blend=BLEND_NORMAL );
	void TileAcross				( float x, float y, float w, float h, BlendMode blend=BLEND_NORMAL );
	void TileAcrossWithOffset		( float x, float y, float w, float h, float xoff, float yoff, BlendMode blend=BLEND_NORMAL );
	// Note: "offset" is a ratio of the texture. 0.5 offsets 50% into the texture.
	
	
	// setters
 	inline void SetID( GLuint x ) { id = x; }

	// Basic info
	inline int 			ID() 		{ return id; }
	inline int 			Width() 		{ return pixels.Width(); }
	inline int 			Height() 		{ return pixels.Height(); }
	inline std::string 		Handle() const { return name; }
	inline AreaRect<int> 	GetPixels()	{ return pixels; }
	inline AreaRect<float> 	GetRatios()	{ return ratios; }
	inline bool 			IsTilable() 	{ return is_tilable; }
	inline bool 			IsValid() 	{ return ( id != 0 ); }
			
	// Get the texture coords as a ratio of the full texture
	inline float Left() 	{ return ratios.x1; }
	inline float Top() 		{ return ratios.y1; }
	inline float Right() 	{ return ratios.x2; }
	inline float Bottom() 	{ return ratios.y2; }
	inline float U(float x)	{ return ratios.x1 + ( x * (ratios.x2 - ratios.x1) ); }
	inline float V(float y)	{ return ratios.y1 + ( y * (ratios.y2 - ratios.y1) ); }
	
	// Get the texture coords as a pixel of the full texture
	inline float LeftPx() 	{ return pixels.x1; }
	inline float TopPx() 	{ return pixels.y1; }
	inline float RightPx() 	{ return pixels.x2; }
	inline float BottomPx()	{ return pixels.y2; }

private:	
	std::string 		name;
	int 				id; 				// the actual opengl texture ID	
	AreaRect<int> 		pixels; 			// a list of area coords for fast lookup
	AreaRect<float> 	ratios; 			// a list of area coords for fast lookup
	bool is_tilable;
	
	static BlendMode global_blend_mode;
	static void SwitchBlendMode(BlendMode blend);
	
	static float screen_x;
	static float screen_y;
	};


	
} // end namespace LGC	



#endif







