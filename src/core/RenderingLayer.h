#ifndef LGC_RENDERING_LAYER_H
#define LGC_RENDERING_LAYER_H

#include "SDL/SDL.h"
#include "../etc/Timer.h"
#include <GL/gl.h>
// #include "../geo/RenderObject.h"
#include <list>


#include "../geo/RenderObject.h"
#include "../geo/RenderMesh.h"

namespace LGC {


class RenderObject; // forward dec
class RenderMesh; // forward dec


#define Renderer RenderingLayer::GetInstance()

class RenderingLayer {
public:

	static RenderingLayer* GetInstance();

	~RenderingLayer();

	void Init(
		int width=800,
		int height=600,
		int bitdepth=32,
		bool fullscreen=false
		);

	void Shutdown();

	// Double dispatch rendering functions
	void Render(); // main one
	void Render( RenderMesh* x );

	void ResizeWindow( int width, int height );

	void ToggleFullScreen();

	void Ortho( bool x );

	inline const int ScreenWidth() { return screen_width; };
	inline const int ScreenHeight() { return screen_height; };
	inline const int ColorBitDepth() { return color_bitdepth; };


	void Register( RenderObject* x );
	void Unregister( RenderObject* x );

	void ClearScreen();


private:

	RenderingLayer();

	int screen_width;
	int screen_height;
	int color_bitdepth;

	int videoFlags;

	SDL_Surface* screen;

	bool inited;

	std::list<RenderObject*> registry;

	void SwapBuffers();

	};





} // end namespace LGC




#endif




