#include "DrawingTools.h"
#include "GameApplication.h"



namespace LGC {




void DrawQuad( float x, float y, float w, float h ) {
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin(GL_QUADS);
		glVertex2f( x, 	y+h 	);
		glVertex2f( x+w, 	y+h 	);
		glVertex2f( x+w, 	y 	);
		glVertex2f( x, 	y 	);
	glEnd();
	}

void DrawQuadByCoords( float x1, float y1, float x2, float y2 ) {
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin(GL_QUADS);
		glVertex2f( x1, y1 );
		glVertex2f( x1, y2 );
		glVertex2f( x2, y2 );
		glVertex2f( x2, y1 );
	glEnd();
	}

void ScissorStack::Push( int x, int y, int w, int h ) {
	rect in = { x, y, w+x, h+y };
	rect on;

	if (!stack.empty()) {
		rect& last = stack.top();

// 		if ( !(in.x1 <= last.x2 && in.x1 >= last.x1)  &&
// 			!(in.x2 <= last.x2 && in.x2 >= last.x1)  &&
// 			!(in.y1 <= last.y2 && in.y1 >= last.y1)  &&
// 			!(in.y2 <= last.y2 && in.y2 >= last.y1)
// 			)

		// check for overlap
		if ( (  (in.x1 <= last.x2 && in.x1 >= last.x1)  ||  (in.x2 <= last.x2 && in.x2 >= last.x1)  ) || // X's
			(  (in.y1 <= last.y2 && in.y1 >= last.y1)  ||  (in.y2 <= last.y2 && in.y2 >= last.y1)  )    // Y's
			) {
			// we have overlap. Find specific new rectangle
			on.x1 = (in.x1 > last.x1) ? in.x1 : last.x1; // left
			on.x2 = (in.x2 < last.x2) ? in.x2 : last.x2; // right
			on.y1 = (in.y1 > last.y1) ? in.y1 : last.y1; // top
			on.y2 = (in.y2 < last.y2) ? in.y2 : last.y2; // bottom
			stack.push(on);
			glScissor( on.x1, on.y1, on.x2-on.x1, on.y2-on.y1 );
			}
		else {
			// no viewable area!
			rect no = {0,0,0,0};
			stack.push(no);
			glScissor(0,0,0,0);
			}
		}
	else {
		// just set a first rectangle
		glEnable(GL_SCISSOR_TEST);
		stack.push(in);
		glScissor( in.x1, in.y1, in.x2-in.x1, in.y2-in.y1 );
		}
	}

void ScissorStack::Pop() {
	stack.pop();
	if ( !stack.empty() ) {
		rect& r= stack.top();
		glScissor( r.x1, r.y1, r.x2-r.x1, r.y2-r.y1 );
		}
	else {
		glDisable(GL_SCISSOR_TEST);
		}

	}



} // end namespace LGC

