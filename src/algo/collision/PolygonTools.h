#ifndef OLI_POLYGON_H
#define OLI_POLYGON_H


/*
#include "Polygon.h"
*/

/*
------------------------------------------------------------------
File: Polygon.h
Started: 12/06/2004 14:37:12
  
$Header: $
$Revision: $
$Locker: $
$Date: $
  
Author: Olivier renault
------------------------------------------------------------------
Module: 
Description: 
------------------------------------------------------------------
$History: $
------------------------------------------------------------------
*/


#include <vector>

typedef unsigned char   u_char;
typedef unsigned short  u_short;
typedef unsigned int    u_int;
typedef unsigned long   u_long;

class Vector;



namespace LGC {


namespace PolygonTools {

void	BuildBox	(std::vector<Vector> &vertices, float width, float height);
void	BuildBlob	(std::vector<Vector> &vertices, int iNumVertices, float radius);
void	Render	(const Vector& xOffset, /*u_int ARGBfill, u_int ARGBline,*/ std::vector<Vector> &vertices, bool filled=false);
//bool	Collide   (std::vector<Vector> &A, std::vector<Vector> &B, Vector& xOffset);
bool Collide (std::vector<Vector> &A, std::vector<Vector> &B, Vector& xOffset, const Vector& xVel, Vector& N, float& t, bool adjust);

}


	
	
	
} // end namespace LGC	



#endif






