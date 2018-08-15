/*
File Name : Collisions.h
Copyright © 2018
Original authors : Sanketh Bhat
Written under the supervision of David I.Schwartz, Ph.D., and
supported by a professional development seed grant from the B.Thomas
Golisano College of Computing & Information Sciences
(https ://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.

Description:
Demo of casting a ray for mosue picking

References:
Point in Polygon collision detection: https://www.geeksforgeeks.org/how-to-check-if-a-given-point-lies-inside-a-polygon/
*/

#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include "GLRender.h"
#include "GameObject.h"


//Checks if p lies on the line l1l2
bool PointLine(glm::vec3 l1, glm::vec3 l2, glm::vec3 p)
{
	if (p.x <= std::max(l1.x, l2.x) && p.x >= std::min(l1.x, l2.x) &&
		p.y <= std::max(l1.y, l2.y) && p.y >= std::min(l1.y, l2.y))
		return true;
	return false;
}

//returns 0 if p1, p2 and p3 lie on the same line
int orientation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	
	float val = p1.x*(p2.y - p3.y) + p2.x*(p3.y - p1.y) + p3.x*(p1.y - p2.y); //colinear formula

	if (val == 0) return 0;  // colinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

//Checks if line p1p2 intersects with line l1l2
bool LineIntersection(glm::vec3 p1, glm::vec3 p2, glm::vec3 l1, glm::vec3 l2)
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, p2, l1);
	int o2 = orientation(p1, p2, l2);
	int o3 = orientation(l1, l2, p1);
	int o4 = orientation(l1, l2, p2);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if (o1 == 0 && PointLine(p1, p2, l1)) return true;

	// p1, q1 and p2 are colinear and q2 lies on segment p1q1
	if (o2 == 0 && PointLine(p1, p2, l2)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if (o3 == 0 && PointLine(l1, l2, p1)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if (o4 == 0 && PointLine(l1, l2, p2)) return true;

	return false; // Doesn't fall in any of the above cases
}



//Checks if a point lines in a polygon
bool PointPoly(glm::vec3 point, std::vector<glm::vec3> verts)
{
	int count = 0;
	int next = 0;
	int i = 0;
	glm::vec3 far = glm::vec3(10000, point.y, 0); //creat a point faraway to the right

	do
	{
		next = (i + 1) % verts.size();
		if (LineIntersection(verts[i], verts[next], point, far)) //poly gon edge and line from the point to far intersect
		{
			if (orientation(verts[i], point, verts[next]) == 0)
				return PointLine(verts[i], verts[next], point); //point lies on an edge of the polygon

			count++;
		}
		i = next;
	} while (i != 0);

	return (count % 2 == 1); //if cout is even, it means that the point lies putside the polygon, odd means it lies inside.
}


//Checks is a line segment collides with a polygon
bool LinePoly(glm::vec3 origin,glm::vec3 dir,GameObject obj)
{
	std::vector<glm::vec3> verts = obj.Vertices();

	glm::vec3 normal = glm::vec3(0, 0, 1);	//Normal of the x-y plane

	float dist = -glm::dot(origin, normal) / glm::dot(dir, normal); //ray origin-plane distance = -ray origin . plane normal / ray direction . plane normal

	glm::vec3 point = origin + dist * dir; //gives us point on the plane that the ray intersects with 

	if (PointPoly(point, verts))
		return true;


	return false;
}





#endif _COLLISIONS_H
