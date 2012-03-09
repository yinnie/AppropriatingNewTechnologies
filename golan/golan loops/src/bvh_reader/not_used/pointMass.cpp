/*****************************************************************************

	bioviewer

    Copyright (C) 2002  Lucas Walter

	lucasw@u.washington.edu
	http://students.washington.edu/lucasw

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

******************************************************************************/


#include <math.h>
#include "pointMass.hpp"
#include "misc.hpp"


pointMass::pointMass()
{
	DRAG= .99f;

	init();
}

pointMass::pointMass(float DRAG)
{
	this->DRAG= DRAG;

	init();
}

pointMass::~pointMass()
{
}

void pointMass::update()
{
	location = newLocation;


	// Euler method
	velocity = velocity + force/mass;

	newLocation = location + velocity;



/////////////////////////////////////////////////////
#ifdef CANCELFORCE
	if (doCancelForce)
	{
		// cancel forces
		// cancel additional forces in the direction of further stretching
		// if the spring is stretched
		// beyond a limit
		for (int i = 0; i < numCancelForces; i++)
		{
			float cancelMag = dot(*cancelForce[i],force);
			if (cancelMag > 0)
			{
				vector3f thisCancelForce = (*cancelForce[i])*(cancelMag);
				force = force + thisCancelForce;
			}

			delete cancelForce[i];
		}
		fprintf(stderr,"vertex[1] %f numCancel %d\n ",force.vertex[1],numCancelForces);

		numCancelForces = 0;
		//delete cancelForce;
	}
#endif // CANCELFORCE
//////////////////////////////////////////////////////////////////



	// zero out forces so they don't accumulate over cycles
	vector3f zero;
	force = zero;

	// drag
	velocity = velocity*DRAG;

	return;
}


void pointMass::init()
{
	force.vertex[0]	   = force.vertex[1]    = force.vertex[2]    = 0;
	location.vertex[0] = location.vertex[1] = location.vertex[2] = 0;
	velocity.vertex[0] = velocity.vertex[1] = velocity.vertex[2] = 0;

	newLocation.vertex[0] = newLocation.vertex[1] = newLocation.vertex[2] = 0;

	mass = 1.0f;

/////////
#ifdef CANCELFORCE
	numCancelForces = 0;

	doCancelForce = false;
#endif
////////////
}

void pointMass::collisionTest()
{/*
	int i,j;

	bool collided = false;

	for (i = 0; i< SIZE-1; i++)
	{
		for (j=0; j<SIZE-1; j++)
		{
			float ACC = (float)(2*PI/3);

			// first triangle in i,j grid location

			// both triangles share the i,j+1 vertex
			vector3f triangleVertex;
			triangleVertex.vertex[0] = (float)i;
			triangleVertex.vertex[1] = map->heightmap[i][j+1];
			triangleVertex.vertex[2] = (float)j+1;

			float mu = planeTest(&triangleVertex,&map->surfaceNorms[i][j][1]);

			//map->collisionFlag[i][j][1] = false;

			if ((mu > 0 ) && (mu <= 1))
			{

				vector3f tri2;
				tri2.vertex[0] = (float)i;
				tri2.vertex[1] = map->heightmap[i][j];
				tri2.vertex[2] = (float)j;

				vector3f tri3;
				tri3.vertex[0] = (float)i+1;
				tri3.vertex[1] = map->heightmap[i+1][j];
				tri3.vertex[2] = (float)j;

				vector3f intersection = location + (newLocation-location)*mu;

				float sumRadians = triangleTest(&intersection, &triangleVertex,&tri2,&tri3);

				if ((sumRadians >= 2*PI - ACC) && (sumRadians <= 2*PI + ACC))
				{
					// a collision has occurred

					// don't do this more than once per point!
					if (!collided)
					{
						collisionResponse(&intersection,&tri3,&map->surfaceNorms[i][j][1]);
					}

					collided = true;
					map->collisionFlag[i][j][1] = true;
				}

			}


			////////////////////////////////////
			// second triangle in grid location

			mu = planeTest(&triangleVertex,&map->surfaceNorms[i][j][0]);

			//map->collisionFlag[i][j][0] = false;

			if ((mu > 0) && (mu <= 1))
			{
				vector3f tri2;
				tri2.vertex[0] = (float)i+1;
				tri2.vertex[1] = map->heightmap[i+1][j+1];
				tri2.vertex[2] = (float)j+1;

				vector3f tri3;
				tri3.vertex[0] = (float)i+1;
				tri3.vertex[1] = map->heightmap[i+1][j];
				tri3.vertex[2] = (float)j;

				vector3f intersection = location + (newLocation-location)*mu;

				float sumRadians = triangleTest(&intersection, &triangleVertex,&tri2,&tri3);


				if ((sumRadians >= 2*PI - ACC) && (sumRadians <= 2*PI + ACC))
				{
					// a collision has occurred

					// don't do this more than once per point!
					if (!collided)
					{
						collisionResponse(&intersection,&tri3,&map->surfaceNorms[i][j][0]);
					}

					collided = true;
					map->collisionFlag[i][j][0] = true;
				}
			}


		}
	}*/

}


// take the normal to the plane and a point on the plane triangleVertex
// and find where the line location-newLocation intersects
float pointMass::planeTest(vector3f &location, vector3f &newLocation,
				vector3f &triangleVertex, vector3f &surfaceNorm)
{
	/*
	// since Ax + By + Cz + D = 0 to satisfy the plane equation
	// solve for D:
	float D = -(dot(surfaceNorm,triangleVertex));

	// mu is the solution to the intersection equation
	// dot( (location + mu* (newLocation-location)),surfaceNorm) + D = 0
	float mu = 0;
	float muNum = -(dot(location,surfaceNorm) + D);
	float muDen = dot((*newLocation - *location),*surfaceNorm);
	*/
	float mu = 0;
	float muNum = dot(surfaceNorm, (triangleVertex-location));
	float muDen = dot(surfaceNorm, (newLocation-location));

	if (muDen != 0)
		mu =  muNum/muDen;

	return mu;

//	return 0.0f;
}


float pointMass::triangleTest(vector3f &intersection,
						   vector3f &tri1, vector3f &tri2, vector3f &tri3)
{
	// collision with the plane has occurred
	// so test for intersection with triangle



	// If the angles between the intersection point and all the
	// triangle vertexes is about 180 then the intersection point
	// is within the triangle and a collision has occured.


	vector3f p1 = (tri1 - intersection);
	vector3f p2 = (tri2 - intersection);
	vector3f p3 = (tri3 - intersection);

	// Normalize
	p1 = p1/p1.Length();
	p2 = p2/p2.Length();
	p3 = p3/p3.Length();

	float sumRadians = (float)(	acos(dot(p1,p2)) +
								acos(dot(p2,p3)) +
								acos(dot(p3,p1))  );

	return sumRadians;

	//return 0.0f;
}


void pointMass::collisionResponse(vector3f *intersection, vector3f *tri3, vector3f *surfaceNorm)
{
/*	// compute newLocation, velocity for collision response

	// first get two orthogonal vectors also orthogonal to
	// the surfaceNorm
	vector3f base1 = *tri3;//(tri2 - intersection);
	base1 = base1/base1.Length();

	vector3f base2 = Cross(base1,*surfaceNorm);

	// compute the part of the vector that passes through
	// the other side of the surface
	vector3f thru = newLocation - *intersection;
	thru = thru/thru.Length();

	// reflect that vector about the intersection point
	// for the new newLocation
	newLocation = *intersection +
		(base1 * (dot(base1,thru)) +
		base2 * (dot(base2,thru))  ) -
		(*surfaceNorm *
		(dot(surfaceNorm,&thru)));

	// frictionTransverse only acts along the surface plane
	velocity	=	(base1 * (dot(base1,velocity) * map->frictionTransverse) +
					 base2 * (dot(base2,velocity) * map->frictionTransverse)  ) -
					(*surfaceNorm *
						(dot(surfaceNorm,&velocity)) * map->frictionNormal);
	*/
}
