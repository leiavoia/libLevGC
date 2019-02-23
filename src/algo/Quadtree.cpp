#include "Quadtree.h"

#include <iostream>




namespace LGC {





Quadtree::Quadtree( int w, int h, int limit ) {

	// set params
	total_ids = 0;
	this->limit = limit;

	// start the first node:
	QuadtreeNode* node = new QuadtreeNode(0,0,w,h,total_ids);
	nodes.push_back( node );

	// create the space
	PartitionSpace( 0, 0, w, h, total_ids  );

	}


Quadtree::~Quadtree() {
	// delete all nodes
	for ( unsigned int i = 0; i < nodes.size(); i++ ) {
		delete nodes[i];
		}
	nodes.clear();
	}




void Quadtree::PartitionSpace( int x, int y, int w, int h, int node_to_split ) {

	//std::cout << "Now partitioning: " << x << "x --> " << x+w << "x,  " << y << "y --> " << y+h << "y" << std::endl;

	// break space in half
	int half_w = w/2;
	int half_h = h/2;

	// if we hit the limit, stop dividing space
	if (  half_w < limit  ||  half_h < limit  ) { return; }

	// otherwise, split the space into four and recursively execute this function:

	// top left
	++total_ids;
	QuadtreeNode* tl = new QuadtreeNode( x, y, half_w, half_h, total_ids );
	tl->SetParent( node_to_split );
	nodes[ node_to_split ]->SetChild( 0, total_ids );
	nodes.push_back( tl );
	PartitionSpace(  x, y, half_w, half_h, total_ids  );

	// top right
	++total_ids;
	QuadtreeNode* tr = new QuadtreeNode( x+half_w, y, half_w, half_h, total_ids );
	tr->SetParent( node_to_split );
	nodes[ node_to_split ]->SetChild( 1, total_ids );
	nodes.push_back( tr );
	PartitionSpace(  x+half_w, y, half_w, half_h, total_ids  );
 
	// bottom left
	++total_ids;
	QuadtreeNode* bl = new QuadtreeNode( x, y+half_h, half_w, half_h, total_ids );
	bl->SetParent( node_to_split );
	nodes[ node_to_split ]->SetChild( 2, total_ids );
	nodes.push_back( bl );
	PartitionSpace(  x, y+half_h, half_w, half_h, total_ids  );

	// bottom right
	++total_ids;
	QuadtreeNode* br = new QuadtreeNode( x+half_w, y+half_h, half_w, half_h, total_ids );
	br->SetParent( node_to_split );
	nodes[ node_to_split ]->SetChild( 3, total_ids );
	nodes.push_back( br );
	PartitionSpace(  x+half_w, y+half_h, half_w, half_h, total_ids  );

	}



QuadtreeNode* Quadtree::GetNode( unsigned int index ) {
	if ( index >= nodes.size() ) { return NULL; }
	return nodes[index];
	}




bool Quadtree::AddObject( CollisionObject* obj, int node ) {

	//std::cout << "FART" << std::endl;

// 	std::cout << "this node: " << nodes[node]->ID()
// 	<< ", dims: "
// 	<< nodes[node]->X1() << "x --> "
// 	<< nodes[node]->X2() << "x,  "
// 	<< nodes[node]->Y1() << "y --> "
// 	<< nodes[node]->Y2() << "y"
// 
// 	<< ",   object: "
// 	<< obj->XMin() << "x --> "
// 	<< obj->XMax() << "x,  "
// 	<< obj->YMin() << "y --> "
// 	<< obj->YMax() << "y"
// 
// 	<< std::endl;

	// does the object completely fit within this node?
	if (  nodes[node]->FitsAround( obj )  ) {

		//std::cout << "Node [" << node << "] fits around object" << std::endl;

		// probe the next level down:
		bool no_child_fits = true; // set to false if one of the children wraps the object

		for (int n=0; n < 4; n++) {

			int child = nodes[node]->GetChild(n);
			if ( child > -1 ) {

				//std::cout << "looking at child node " << child << std::endl;

				// call this function recursively for each child node
				if ( ! AddObject( obj, child ) ) { continue; }

				else {
					no_child_fits = false;
					break;
					}

				}
			}

		// i wrap the object and no children do. I own the object
		if ( no_child_fits ) {
			nodes[node]->AddObject( obj );
			//std::cout << node << " " << std::endl;
			}

		// object fits
		return true;

		}

	// if the object is out of bounds of world space, it belongs to the root node
	else if (node == 0) {
		nodes[0]->AddObject( obj );
		return true;
		}

	// object does not fit. So the previous call to this function contains the object.
	return false;
	}



void Quadtree::RemoveObject( CollisionObject* obj ) {
	//std::cout << "Quadtree:: remove object from node " << obj->SpaceNode() << std::endl;
	nodes[ obj->SpaceNode() ]->RemoveObject( obj );
	}



void Quadtree::GetObjectsIn( std::vector<CollisionObject*> &v, int node ) {

	// dump my own data into the return vector
	nodes[node]->DumpList(v);

	// for my parent, have it dump a vector into our return vector
	int parent = nodes[node]->GetParent();
	if ( parent > -1 ) {
		// call this function recursively for each parent node
		// and add it's return vector to my own
		GetObjectsIn( v, parent );
		}
	}
	
void Quadtree::GetObjectsIn( std::vector<CollisionObject*> &v, CollisionObject* obj, int node ) {

	QuadtreeNode* thisnode = nodes[node];
	
	// if i'm not in contact with this node
	if ( !thisnode->Touches(obj) ) { return; }
	
	// dump my own data into the return vector
	thisnode->DumpList(v);

	// for my children, have it dump a vector into my return vector if they work out
	for (int i=0; i < 4; i++) {
		int child = thisnode->GetChild(i);
		if (child == -1) { return; } // no one else has children either
		GetObjectsIn( v, obj, child );
		}
	}
	
	



void Quadtree::UpdateObject( CollisionObject* obj ) {

// ----------TODO-----------:
// SWITCH TO THIS UPDATED CODE WHEN THINGS SETTLE DOWN:

	int prev_node = obj->SpaceNode();
	//std::cout << "updating remove..." << std::endl;
	RemoveObject(obj);
	//std::cout << "updating add..." << std::endl;
	if (  !AddObject(obj, prev_node )  ) {
		AddObject(obj);
		}
	
// 	//std::cout << "updating remove..." << std::endl;
// 	RemoveObject(obj);
// 	//std::cout << "updating add..." << std::endl;
// 	AddObject(obj);
	}


void Quadtree::TransferObjectsTo( Quadtree* qt ) {
	std::vector<CollisionObject*> v;
	for (unsigned int i=0; i < nodes.size(); i++) {		
		nodes[i]->DumpList(v);
		for (unsigned int n=0; n < v.size(); n++) {		
			qt->AddObject( v[n] );
			}
		v.clear();
		}
	}
	
	
void Quadtree::DrawNodes() {
	for (unsigned int i=0; i < nodes.size(); i++) {
		nodes[i]->DrawNode( DepthCount(i) );
		}
	}


int Quadtree::DepthCount( int node ) { 
	if (node == -1 || node >= (int)nodes.size()) { return 0; }
	return 1 + DepthCount( nodes[node]->GetParent() );
	} 




	
	
	
} // end namespace LGC	





















