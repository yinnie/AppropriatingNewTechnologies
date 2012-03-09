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

#ifdef WIN32									
#include <windows.h>								
#endif										
#include <iostream>
										
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>	
#include <GL/glu.h>	

#include "misc.hpp"
#include "movable.hpp"
#include "camera.hpp"

#include <cmath>
#ifdef WIN32
#define M_PI PI
#endif

camera::camera()
{
	init();
}

camera::camera(string name, mode lookMode)
{
    this->name = name;
    this->lookMode = lookMode;
	cout << lookMode << "\n";
	init();
}

camera::~camera()
{

}

void camera::init()
{
	radius = 10.0f;

	//location.LoadIdentity();
	//location.matrix[10] = -1.0f;  // this is critical!! remember in OpenGL x cross y = z
	
	other.LoadIdentity();
	other.matrix[10] = -1.0f;  
	
	// position camera behind and above object
	other.RotateY(180);
	//other.RotateX(-20);

	for (int i=0; i<20;i++)
	{
		delay[i].LoadIdentity();
		delay[i].matrix[10] = -1.0f;
	}
	
	angles.vertex[0] = 0.0f;//M_PI/2;
	angles.vertex[1] = 0.0f;//3*M_PI/4;
	angles.vertex[2] = 0.0f;
}

void camera::draw()
{
}

/*void camera::update()
{
}*/

// update currently doesn't work within movable framework.
void camera::update()
{
	/*int i;
	switch (lookMode)
	{

	case FREE:
	// one if is better than the dozen or so
	// additions and cos/sin calls required by rotates
	// if the value is zero

	if (pitch != 0)
		location.RotateX(-pitch/8);

	if (turn != 0)
		location.RotateY(-turn/8);

	if (roll != 0)
		location.RotateZ(roll/8);

	
	location.Translate(x,y,z);
	break;

	case CENTER:

	/*	if (pitch != 0)
			location.RotateX(pitch/5);

		if (turn != 0)
			location.RotateY(turn/5);

		if (roll != 0)
			location.RotateZ(roll/5);

		location.matrix[12] = center->matrix[12];
		location.matrix[13] = center->matrix[13];
		location.matrix[14] = center->matrix[14];

//		radius -= z;
		location.Translate(0,0,-radius);
		break;

		
	case FOLLOW:

		if (pitch != 0)
			other.RotateX(pitch/4);

		if (turn != 0)
			other.RotateY(turn/4);

		if (roll != 0)
			other.RotateZ(roll/4);

		location = ((*center * other));

//		radius -= z;
		location.Translate(0,0,-radius);
		break;

				
	case DELAY:
		
		for (i = 0; i<19;i++)
		{
			delay[i] = (delay[i+1]);
		}
		delay[19]=(*center);

		/*
		if (pitch != 0)
			other.RotateX(pitch/4);

		if (turn != 0)
			other.RotateY(turn/4);

		if (roll != 0)
			other.RotateZ(roll/4);

		location=((delay[14] * other));

//		radius -= z;
		location.Translate(0,0,-30);//-radius);
		break;


	default:
		break;
	}
*/
}

void camera::move(int pitch, int turn, int roll, float x, float y, float z)
{
	vector3f temp;
	
    switch (lookMode)
	{
	case FREE:
	
		if (pitch != 0)
			location.RotateX(-pitch/ROTATE_SLOWNESS);
		
		if (turn != 0)
			location.RotateY(-turn/ROTATE_SLOWNESS);
		
		if (roll != 0)
			location.RotateZ(roll/ROTATE_SLOWNESS);
		
		location.Translate(x,y,z);
	
	break;
	
	case FREEORIENTED:
	
		//vector3f temp2(-pitch/ROTATE_SLOWNESS, -turn/ROTATE_SLOWNESS, roll/ROTATE_SLOWNESS);
		angles = angles + vector3f(DEG_TO_RAD(-pitch/ROTATE_SLOWNESS),
								   DEG_TO_RAD(-turn/ROTATE_SLOWNESS), 
								   DEG_TO_RAD( roll/ROTATE_SLOWNESS)  );
		
		/// vertex[0] is theta, or pitch
		if (angles.vertex[0] >  M_PI/2) angles.vertex[0] =  M_PI/2;
		if (angles.vertex[0] < -M_PI/2) angles.vertex[0] = -M_PI/2;
		/// vertex[1] is phi, or turn   wrap the angles here
		while (angles.vertex[1] > 2*M_PI)  angles.vertex[1] -= 2*M_PI;
		while (angles.vertex[1] < 0.0)     angles.vertex[1] += 2*M_PI;
		
		if (angles.vertex[2] >  M_PI/2) angles.vertex[2] =  M_PI/2;
		if (angles.vertex[2] < -M_PI/2) angles.vertex[2] = -M_PI/2;
		
		
		temp.Set(location.matrix[12],location.matrix[13],location.matrix[14]);
		location.LoadIdentity();
		location.matrix[10] = -1.0f; 
		location.matrix[12] = temp.vertex[0];
		location.matrix[13] = temp.vertex[1];
		location.matrix[14] = temp.vertex[2];
	
		/// Ordering of rotations is important
		location.RotateY(angles.vertex[1]);
		/// We want to move in the proper direction here
		location.Translate(x,y,z);
		/// but not take these rotations into account
		location.RotateX(angles.vertex[0]);
		location.RotateZ(angles.vertex[2]);
	
	break;
		
	case CENTERED:
		
		if (pitch != 0)
			location.RotateX(-2*pitch/ROTATE_SLOWNESS);
		
		if (turn != 0)
			location.RotateY(-2*turn/ROTATE_SLOWNESS);
		
		if (roll != 0)
			location.RotateZ(2*roll/ROTATE_SLOWNESS);
		
		location.matrix[12] = centerPoint.vertex[0];
		location.matrix[13] = centerPoint.vertex[1];
		location.matrix[14] = centerPoint.vertex[2];

		radius -= z;
		location.Translate(0,0,-radius);
	
	break;
			
	
	}
	
	newLocation = location;

}

void camera::look()
{
	
	gluLookAt( 
				//eye
				location.matrix[12],
				location.matrix[13],
				location.matrix[14], 

				// center
				location.matrix[12]+location.matrix[8],
				location.matrix[13]+location.matrix[9],
				location.matrix[14]+location.matrix[10], 

				// up
				location.matrix[4],
				location.matrix[5],
				location.matrix[6]);


	return;	


}
