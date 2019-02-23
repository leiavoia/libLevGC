#ifndef QUADTREE_NODE_H
#define QUADTREE_NODE_H

#include "../objects/CollisionObjects.h"

#include <list>
#include <vector>


namespace LGC {




class QuadtreeNode {

public:

	QuadtreeNode( int x, int y, int w, int h, int id_num );

	~QuadtreeNode() {}


	/** TRUE if the coords are completely wrapped by the node */
	bool FitsAround( int x, int y, int w, int h );
	bool FitsAround( float x, float y, float w, float h );
	bool FitsAround( CollisionObject* obj );


	/** TRUE if the coords touch the node in whole or in part*/
	bool Touches( int x, int y, int w, int h );
	bool Touches( float x, float y, float w, float h );
	bool Touches( CollisionObject* obj );


	void AddObject( CollisionObject* obj );

	void RemoveObject( CollisionObject* obj );


	inline int ID() { return id; }
	inline int X1() { return x1; }
	inline int X2() { return x2; }
	inline int Y1() { return y1; }
	inline int Y2() { return y2; }

	inline int GetChild( int x=0 ) {
		if ( x > 3 || x < 0 ) { return -1; }
		return children[x];
		}

	inline void SetChild( int x, int id ) {
		if ( x > 3 || x < 0 ) { return; }
		children[x] = id;
		}

	inline int GetParent() { return parent; }
	inline void SetParent( int x ) { parent = x; }

	void DumpList( std::vector<CollisionObject*> &v );

	void DrawNode( int depth );

	
	
protected:

	int id;
	int x1;
	int y1;
	int x2;
	int y2;
	int children[4];
	int parent;

	std::list<CollisionObject*> objects;


	};


	
	
	
} // end namespace LGC	


#endif


















