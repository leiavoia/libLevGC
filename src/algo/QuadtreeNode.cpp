#include "QuadtreeNode.h"

#include <algorithm>
#include <iostream>
#include <GL/gl.h>


namespace LGC {




QuadtreeNode::QuadtreeNode( int x, int y, int w, int h, int id_num ) {
	id = id_num;
	x1 = x;
	y1 = y;
	x2 = x + w;
	y2 = y + h;

	for (int n=0; n < 4; n++) {
		children[n] = -1;
		}

	parent = -1;
	}



bool QuadtreeNode::FitsAround( int x, int y, int w, int h ) {
	if (
		x > x1    &&
		x+w < x2  &&
		y > y1    &&
		y+h < y2
		) { return true; }
	return false;
	}

bool QuadtreeNode::FitsAround( float x, float y, float w, float h ) {
	if (
		x > x1    &&
		x+w < x2  &&
		y > y1    &&
		y+h < y2
		) { return true; }
	return false;
	}

bool QuadtreeNode::Touches( int x, int y, int w, int h ) {
	if (
		x > x2   || // left of object to right of node
		x+w < x1 || // right of object to left of node
		y > y2   || // bottom of object to top of node
		y+h < y1    // top of object to bottom of node
		) { return false; }
	return true;
	}

bool QuadtreeNode::Touches( float x, float y, float w, float h ) {
	if (
		x > x2   || // left of object to right of node
		x+w < x1 || // right of object to left of node
		y > y2   || // bottom of object to top of node
		y+h < y1    // top of object to bottom of node
		) { return false; }
	return true;
	}


bool QuadtreeNode::FitsAround( CollisionObject* obj ) {
	return FitsAround(
			obj->XMin(),
			obj->YMin(),
			obj->XMax()-obj->XMin(),
			obj->YMax()-obj->YMin()
			);
	}


bool QuadtreeNode::Touches( CollisionObject* obj ) {
	return Touches(
			obj->XMin(),
			obj->YMin(),
			obj->XMax()-obj->XMin(),
			obj->YMax()-obj->YMin()
			);
	}


void QuadtreeNode::DumpList( std::vector<CollisionObject*> &v ) {
	for (std::list<CollisionObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++) {
		v.push_back( *iter );
		//std::cout << "Dumping list object. vsize = " << v.size() << std::endl;
		}
	}


void QuadtreeNode::AddObject( CollisionObject* obj ) {
	if (!obj) { return; }
	objects.push_back( obj );
	obj->SetSpaceNode( id );
	//std::cout << "object added to node " << id << " [" << objects.size() << " count], dims: " << x1 << "x --> " << x2 << "x,  " << y1 << "y --> " << y2 << "y" << std::endl;
	}


void QuadtreeNode::RemoveObject( CollisionObject* obj ) {
	//std::cout << "Quadtree:: removing object from node " << obj->SpaceNode() << std::endl;
	std::list<CollisionObject*>::iterator iter = find( objects.begin(), objects.end(), obj );
	if (iter == objects.end()) { return; }
	objects.erase(iter);
	obj->SetSpaceNode( 0 );
	//std::cout << "Quadtree:: object removed " << std::endl;
	}



void QuadtreeNode::DrawNode( int depth ) {

	glBindTexture( GL_TEXTURE_2D, 0 );
	glColor4f( 1.0, 0.6, 0.2, 1 ); // ORANGE

	// draw box
	glBegin(GL_LINE_STRIP);
		/* Bottom Left Of The Texture and Quad */
		glVertex2f( X1(), Y2() );
		/* Bottom Right Of The Texture and Quad */
		glVertex2f( X2(), Y2() );
		/* Top Right Of The Texture and Quad */
		glVertex2f( X2(), Y1() );
		/* Top Left Of The Texture and Quad */
		glVertex2f( X1(), Y1() );
		/* Bottom Left Of The Texture and Quad - AGAIN - close box */
		glVertex2f( X1(), Y2() );
	glEnd();

	
	glBegin(GL_LINES);
	for (unsigned int n=0; n < objects.size(); n++) {
		glVertex2f( X1()+10+(n*10), Y1()+(depth*30) );
		glVertex2f( X1()+10+(n*10), Y1()+20+(depth*30) );
		}
	glEnd();	
	}


	
	
	
} // end namespace LGC	




