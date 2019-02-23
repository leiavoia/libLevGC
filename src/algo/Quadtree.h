#ifndef QUADTREE_H
#define QUADTREE_H

#include <vector>


#include "QuadtreeNode.h"



namespace LGC {




class Quadtree {

public:

	Quadtree( int w, int h, int limit=50 );

	~Quadtree();

	void PartitionSpace( int x, int y, int w, int h, int parent );

	QuadtreeNode* GetNode( unsigned int index );

	bool AddObject( CollisionObject* obj, int node=0 );

	void RemoveObject( CollisionObject* obj );

	void UpdateObject( CollisionObject* obj );

	//void RemoveObject( int x, int y, int w, int h, CollisionObject* obj );

	void GetObjectsIn( std::vector<CollisionObject*> &v, int node=0 );
	
	void GetObjectsIn( std::vector<CollisionObject*> &v, CollisionObject* obj, int node=0 );

	void DrawNodes();

	int DepthCount( int node ); // returns the layers deep the node is
	
	void TransferObjectsTo( Quadtree* qt );
	
protected:

	std::vector< QuadtreeNode* > nodes;

	int total_ids;

	int limit;

	};




	
	
	
} // end namespace LGC	


#endif


















