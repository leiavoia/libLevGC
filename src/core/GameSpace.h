#ifndef GAMESPACE_H
#define GAMESPACE_H

#include <vector>
#include <string>




#include "../objects/CollisionObjects.h"
#include "../objects/UniversalObserver.h"


namespace LGC {




class BackgroundImage;



class GameSpace {
public:

	//GameSpace( int width, int height );

	GameSpace( std::string name );
	
	virtual ~GameSpace();


	
	void Move(int x, int y);

	void MoveTo( int x, int y );


	inline int Xoff() { return xoff; }

	inline int Yoff() { return yoff; }

	//inline int ScreenWidth() { return screen->w; }

	//inline int ScreenHeight() { return screen->h; }

	inline int SpaceWidth() { return space_width; }

	inline int SpaceHeight() { return space_height; }

	void CenterOn( float x, float y );

	/** TRUE if the coords touch the screen area in whole or in part*/
	bool Touches( int x, int y, int w, int h );
	bool Touches( float x, float y, float w, float h );
	bool Touches( CollisionObject* obj );

	static void SetScreenSize( int x, int y ) {
		screen_w = x;
		screen_h = y;
		}
		
protected:

	int xoff;
	int yoff;

	int xoff_limit;
	int yoff_limit;

	int space_width;
	int space_height;
	
	static int screen_w;
	static int screen_h;

	std::vector<BackgroundImage*> bgs;

	void RecalcOffsetMatrix();
	
	std::vector< std::string > reglist; // resource class list

	};


	
	
	
} // end namespace LGC	







#endif












