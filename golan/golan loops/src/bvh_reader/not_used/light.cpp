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
														
#include <stdlib.h>															
	
#include <stdio.h>	
#include <iostream>
//#include <fstream>

#include <GL/gl.h>												
#include <GL/glu.h>	

#include "misc.hpp"
#include "movable.hpp"
//#include "camera.hpp"
#include "light.hpp"
//#include "objloader.hpp"

light::light()
{

}

light::light(camera *viewer, int GL_LIGHTX, float maxFade, float minFade, float scale)
{
	verbose = false;
	
	if (verbose) cout << "light " << GL_LIGHTX << " init...";

	this->viewer = viewer;
//	move(0,0,0,position->vertex[0],position->vertex[1],position->vertex[2]);
	this->GL_LIGHTX = GL_LIGHTX;
	
	/// Distances are compared squared, so just square these now to save time
	this->maxFade = maxFade * maxFade;
	this->minFade = minFade * minFade;
	this->scale = scale;

	GLfloat pos[] = {location.matrix[12],location.matrix[13],location.matrix[14],1.0f};
	

	Ka[0]=0.3f; Ka[1]=0.3f; Ka[2]=0.3f; Ka[3]=1.0f; 
	Kd[0]=0.5f; Kd[1]=0.5f; Kd[2]=0.5f; Kd[3]=1.0f; 
	Ks[0]=0.3f; Ks[1]=0.3f; Ks[2]=0.3f; Ks[3]=1.0f; 
	
	if ((GL_LIGHTX >= GL_LIGHT0) && (GL_LIGHTX <= GL_LIGHT7))
	{
		glEnable(GL_LIGHTX);
		
		glLightfv(GL_LIGHTX, GL_POSITION,pos);
		glLightfv(GL_LIGHTX, GL_AMBIENT, Ka);	
		glLightfv(GL_LIGHTX, GL_DIFFUSE, Kd);
		glLightfv(GL_LIGHTX, GL_SPECULAR,Ks);
	}

	
	if (verbose) cout << "finished\n\t";

//	string subdir = "obj/";
	//lightobj = new objloader("explosion.obj",0,subdir);

	//getBoundingBox();
	
}


light::~light()
{
	glDisable(GL_LIGHTX);
//	delete lightobj;
}

void light::draw()
{
	
	glPushMatrix();

	glEnable(GL_LIGHTING);	
	GLfloat pos[] = {location.matrix[12],location.matrix[13],location.matrix[14],1.0f};
	
	if ((GL_LIGHTX >= GL_LIGHT0) && (GL_LIGHTX <= GL_LIGHT7))
	{
		glEnable(GL_LIGHTX);
		
		glLightfv(GL_LIGHTX, GL_POSITION,pos);
		glLightfv(GL_LIGHTX, GL_AMBIENT, Ka);	
		glLightfv(GL_LIGHTX, GL_DIFFUSE, Kd);
		glLightfv(GL_LIGHTX, GL_SPECULAR,Ks);
	}

	
	//glLoadIdentity();
	//lightobj->location=(location);


	glDisable(GL_LIGHTING); 
//	glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//lightobj->draw();
	//glDisable(GL_BLEND);
	
	//glEnable(GL_LIGHTING);	

	//lightobj->draw();

	glPopMatrix();
}

void light::drawDim(vector3f distant)
{
	
	glPushMatrix();	

	vector3f lightPos(location.matrix[12],location.matrix[13],location.matrix[14]);
	GLfloat pos[] = {location.matrix[12],location.matrix[13],location.matrix[14],1.0f};
	
	float distanceSquared = (lightPos-distant).LengthSquared();
	
	
	float attenuate; // = 1.0f;

	if (maxFade != 0) {
		if (distanceSquared < minFade) 
			attenuate = scale;
		else if ((distanceSquared < maxFade) && (distanceSquared > minFade))
			attenuate = scale * (maxFade - distanceSquared) / (maxFade - minFade);
		else attenuate = 0.0f;
	} else attenuate = 1.0f;	
			
	//cout << attenuate << "\n";
	
	GLfloat Kaf[] = {Ka[0] * attenuate, Ka[1] * attenuate, Ka[2] * attenuate, Ka[3],1.0f};
	GLfloat Kdf[] = {Kd[0] * attenuate, Kd[1] * attenuate, Kd[2] * attenuate, Kd[3],1.0f};
	GLfloat Ksf[] = {Ks[0] * attenuate, Ks[1] * attenuate, Ks[2] * attenuate, Ks[3],1.0f};
	
	if ((GL_LIGHTX >= GL_LIGHT0) && (GL_LIGHTX <= GL_LIGHT7))
	{
		glEnable(GL_LIGHTX);
		
		glLightfv(GL_LIGHTX, GL_POSITION, pos);
		glLightfv(GL_LIGHTX, GL_AMBIENT, Kaf);	
		glLightfv(GL_LIGHTX, GL_DIFFUSE, Kdf);
		glLightfv(GL_LIGHTX, GL_SPECULAR,Ksf);
	}

	
	//glLoadIdentity();
//	lightobj->location=(location);


	
//	glEnable(GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//lightobj->draw();
	//glDisable(GL_BLEND);
	
	//glEnable(GL_LIGHTING);	

	//lightobj->draw();

	glPopMatrix();
}

void light::update()
{
}

void light::getBoundingBox()
{
/*	centerBB = lightobj->centerBB;
	edgesBB = lightobj->edgesBB;
	for (int i=0; i<8;i++)
	{
		boundingBox[i] = lightobj->boundingBox[i]; 
	}
	*/
}

