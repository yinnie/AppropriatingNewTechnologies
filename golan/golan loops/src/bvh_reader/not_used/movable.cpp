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

#include <stdio.h>
#include <string>

#include <GL/gl.h>												
//#include <GL/glu.h>	


//#include <Rectangle.h>


#include "misc.hpp"
#include "movable.hpp"
//#include "app.hpp"

using namespace std;
/*
CheckBoxListener::CheckBoxListener(bool *toggle) 
{	this->toggle = toggle;}


ActionListener& CheckBoxListener::onAction(const std::string& aName)
{	
	//if (aName == "Bounding Box")
	//{
		*toggle = !(*toggle);
	//}

    return *this;
}
*/

movable::movable()
{
	init();
}

movable::movable(string name)
{
	setName(name);
	
	init();
}

movable::~movable()
{
	
}

void movable::init()
{
	location.LoadIdentity();
	location.matrix[10] = -1.0f;  

	drawBB		= false;
	BBcollided  = false;

	physical = false;

	normalize = true;

	//getBoundingBox();

/*	// GLG stuff
	movableFrame = new FrameWindow(Gooey::Rectangle(1, 400, 150, SCREEN_H), 0, "Current");
	movablePanel = new Panel(Gooey::Rectangle(10, 10, 20, 20), movableFrame);

	movableFrame->setClientWindow(movablePanel);

	bbBox = new CheckBox(0,"Test Points");
	bbBoxListener = new CheckBoxActionListener(&drawBB);
	bbBox->addMessageListener(bbBoxListener);
	
	movablePanel->addChildWindow(bbBox);
	movablePanel->arrangeChildren();
*/
}

movable& movable::operator= (const movable& other)
{
	location = other.location;

	return *this;
}


void movable::setName(string name)
{
	
	location.LoadIdentity();
	location.matrix[10] = -1.0f; 
	
	this->name = name;
/*
	for (int i = 0; name[i] !=0; i++)
		this->name[i] = name[i];

	this->name[i] = name[i];*/

//	movableFrame->setText(name);
}


void movable::draw()
{

}

void movable::update(void)
{
	
}

void movable::move(int pitch, int turn, int roll, float x, float y, float z)
{
	if (pitch != 0)
		location.RotateX(-pitch/ROTATE_SLOWNESS);

	if (turn != 0)
		location.RotateY(-turn/ROTATE_SLOWNESS);

	if (roll != 0)
		location.RotateZ(roll/ROTATE_SLOWNESS);

	
	location.Translate(x,y,z);

	newLocation = location;
}

void movable::getBoundingBox()
{

}

void movable::getAABB()
{
	oldCenterAABB = centerAABB;

	vector3f max = newLocation.Transform(boundingBox[0]);
	vector3f min = newLocation.Transform(boundingBox[0]);

	vector3f offset(newLocation.matrix[12],newLocation.matrix[13],newLocation.matrix[14]);
	// quick and dirty using the transformed oriented BBox, and hardly volume
	// minimizing
	for (int i = 0; i<8; i++)
	{
		for (int j = 0; j < 3; j++)
		{

			if (max.vertex[j] < newLocation.Transform(boundingBox[i]).vertex[j])
				max.vertex[j] = newLocation.Transform(boundingBox[i]).vertex[j];

			if (min.vertex[j] > newLocation.Transform(boundingBox[i]).vertex[j])
				min.vertex[j] = newLocation.Transform(boundingBox[i]).vertex[j];

		}
	}

	min = min + offset;
	max = max + offset;

	centerAABB = (min + max)/2;
	edgesAABB = max - centerAABB;

	AABB[0] = min;
	AABB[6] = max;

		// having found maximums, populate rest of corners
	AABB[1].vertex[0] = AABB[0].vertex[0];
	AABB[1].vertex[1] = AABB[0].vertex[1];
	AABB[1].vertex[2] = AABB[6].vertex[2];
	
	AABB[2].vertex[0] = AABB[0].vertex[0];
	AABB[2].vertex[1] = AABB[6].vertex[1];
	AABB[2].vertex[2] = AABB[6].vertex[2];
	
	AABB[3].vertex[0] = AABB[0].vertex[0];
	AABB[3].vertex[1] = AABB[6].vertex[1];
	AABB[3].vertex[2] = AABB[0].vertex[2];
		
	AABB[4].vertex[0] = AABB[6].vertex[0];
	AABB[4].vertex[1] = AABB[0].vertex[1];
	AABB[4].vertex[2] = AABB[0].vertex[2];
	
	AABB[5].vertex[0] = AABB[6].vertex[0];
	AABB[5].vertex[1] = AABB[0].vertex[1];
	AABB[5].vertex[2] = AABB[6].vertex[2];
	

	AABB[7].vertex[0] = AABB[6].vertex[0];
	AABB[7].vertex[1] = AABB[6].vertex[1];
	AABB[7].vertex[2] = AABB[0].vertex[2];


	
}


void movable::drawAABB()
{


	//getAABB();

	glPushMatrix();

	//glTranslatef(location.matrix[12],location.matrix[13],location.matrix[14]);
	glDisable(GL_LIGHTING);

	
	if (BBcollided)
		glColor3f ( 0.7f, 0.3f, 0.4f);
	else	
		glColor3f ( 0.4f, 0.3f, 0.7f);


	for (int i =0; i<4; i++)
	{
		glBegin (GL_LINES);
		glVertex3fv( AABB[i].vertex);
		
		glVertex3fv( AABB[i+4].vertex);
		glEnd ();
	}
	
	glBegin (GL_LINE_LOOP);	
	for (int j = 0; j<4;j++)
	{
		glVertex3fv( AABB[j].vertex );
		
	}
	glEnd ();
	
	glBegin (GL_LINE_LOOP);	
	for (int k = 4; k<8;k++)
	{
		glVertex3fv( AABB[k].vertex );
	}
	glEnd ();

	
	glEnable(GL_LIGHTING);

	glPopMatrix();
}


void movable::drawBoundingBox()
{
	glDisable(GL_LIGHTING);

	if (BBcollided)
		glColor3f ( 0.9f, 0.3f, 0.2f);
	else	
		glColor3f ( 0.2f, 0.4f, 0.2f);
	
	
	
	
	for (int i =0; i<4; i++)
	{
		glBegin (GL_LINES);
		glVertex3fv( boundingBox[i].vertex);
		
		glVertex3fv( boundingBox[i+4].vertex);
		glEnd ();
	}
	
	glBegin (GL_LINE_LOOP);	
	for (int j = 0; j<4;j++)
	{
		glVertex3fv( boundingBox[j].vertex );
		
	}
	glEnd ();
	
	glBegin (GL_LINE_LOOP);	
	for (int k = 4; k<8;k++)
	{
		glVertex3fv( boundingBox[k].vertex );
	}
	glEnd ();
	
		

/*
	//glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	glColor4f(1.0f,1.0f,1.0f,0.1f);

	float step =.4f;

	float f;

	// transparent planes intersecting the volume

	// this could work as a volumetric fog effect billboard style with a volume always
	// oriented to present a face towards the viewer
	glBegin (GL_QUADS);
	for (f = boundingBox[0].vertex[0];f <boundingBox[6].vertex[0]+step; f+=step)
	{
		glVertex3f( f, boundingBox[0].vertex[1], boundingBox[0].vertex[2] );
		glVertex3f( f, boundingBox[1].vertex[1], boundingBox[1].vertex[2] );
		glVertex3f( f, boundingBox[2].vertex[1], boundingBox[2].vertex[2] );
		glVertex3f( f, boundingBox[3].vertex[1], boundingBox[3].vertex[2] );

		glVertex3f( boundingBox[6].vertex[0]+step-f+boundingBox[0].vertex[0], boundingBox[3].vertex[1], boundingBox[3].vertex[2] );
		glVertex3f( boundingBox[6].vertex[0]+step-f+boundingBox[0].vertex[0], boundingBox[2].vertex[1], boundingBox[2].vertex[2] );
		glVertex3f( boundingBox[6].vertex[0]+step-f+boundingBox[0].vertex[0], boundingBox[1].vertex[1], boundingBox[1].vertex[2] );
		glVertex3f( boundingBox[6].vertex[0]+step-f+boundingBox[0].vertex[0], boundingBox[0].vertex[1], boundingBox[0].vertex[2] );

	}
	for (f = boundingBox[0].vertex[1];f <boundingBox[6].vertex[1]+step; f+=step)
	{
		glVertex3f( boundingBox[3].vertex[0], f, boundingBox[3].vertex[2] );
		glVertex3f( boundingBox[4].vertex[0], f, boundingBox[4].vertex[2] );
		glVertex3f( boundingBox[6].vertex[0], f, boundingBox[6].vertex[2] );
		glVertex3f( boundingBox[1].vertex[0], f, boundingBox[1].vertex[2] );

		glVertex3f( boundingBox[1].vertex[0],boundingBox[6].vertex[0]+step -f, boundingBox[1].vertex[2] );
		glVertex3f( boundingBox[6].vertex[0],boundingBox[6].vertex[0]+step -f, boundingBox[6].vertex[2] );
		glVertex3f( boundingBox[4].vertex[0],boundingBox[6].vertex[0]+step -f, boundingBox[4].vertex[2] );
		glVertex3f( boundingBox[3].vertex[0],boundingBox[6].vertex[0]+step -f, boundingBox[3].vertex[2] );
	}
	for (f = boundingBox[0].vertex[2];f <boundingBox[6].vertex[2]+step; f+=step)
	{
		glVertex3f( boundingBox[0].vertex[0], boundingBox[0].vertex[1], f );
		glVertex3f( boundingBox[2].vertex[0], boundingBox[2].vertex[1], f );
		glVertex3f( boundingBox[7].vertex[0], boundingBox[7].vertex[1], f );
		glVertex3f( boundingBox[5].vertex[0], boundingBox[5].vertex[1], f );

		glVertex3f( boundingBox[5].vertex[0], boundingBox[5].vertex[1], boundingBox[6].vertex[0]+step-f );
		glVertex3f( boundingBox[7].vertex[0], boundingBox[7].vertex[1], boundingBox[6].vertex[0]+step-f );
		glVertex3f( boundingBox[2].vertex[0], boundingBox[2].vertex[1], boundingBox[6].vertex[0]+step-f );
		glVertex3f( boundingBox[0].vertex[0], boundingBox[0].vertex[1], boundingBox[6].vertex[0]+step-f );
	}

	glEnd ();

	glColor4f(0.9f,0.0f,0.0f,0.8f);

	glBegin (GL_POINTS);

	for (float xf = boundingBox[0].vertex[0]; xf < boundingBox[6].vertex[0]+step; xf+=step)
	{
		for (float yf  = boundingBox[0].vertex[1]; yf < boundingBox[6].vertex[1]+step; yf+=step)
		{
			for (float zf = boundingBox[0].vertex[2]; zf < boundingBox[6].vertex[2]+step; zf+=step)
			{
					glVertex3f(xf,yf,zf);
			}
		}
	}

	glEnd ();


	//glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
*/	

	glEnable(GL_LIGHTING);
}





/*
bool operator< (const movable& other)
{
	return false;
}

bool operator== (const movable& other)
{
	return (movable == this);
}

*/
