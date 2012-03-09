/*****************************************************************************

	bioviewer
		
    Copyright (C) 2002-2003  Lucas Walter

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

#include <SDL/SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>

#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>												
#include <GL/glu.h>	
//#include <GL/glaux.h>

//#include <math.h>
#include <cmath>

#include "main.hpp"
#include "misc.hpp"
#include "movable.hpp"
#include "pointMass.hpp"
#include "objloader.hpp"

#ifdef WIN32
#define M_PI PI
#endif 

using namespace std;

objloader::objloader()
{	
/*
	counter = 0;

	numVertices = 0;
	numMtl		= 0;

	
	fprintf(stderr,"spaceship.obj init...");

	load("spaceship.obj");

	fprintf(stderr,"finished\n");*/
}


objloader::objloader(string objFile)
{	
	counter = 0;

	verbose = false;
	
	translucent = false;
	
	//drawBB = true;
	
	
	/// Strip directory info out of objFile
	string::size_type pos = objFile.find_last_of("/");
	this->subdir = objFile.substr(0,pos+1);
	objFile = objFile.substr(pos+1,objFile.size());

	if (verbose) cout << subdir + objFile << " init...";
	try { load(objFile); }
	catch (fileNotFound) { throw fileNotFound(); return;}
	if (verbose) cout << "finished\n";
	
	mass = 1;

	getBoundingBox();

	//makeList();

	if (verbose) cout << "\n";
/*
	// GLG toggles //////////////////////
	surfBox = new CheckBox(0,"Surface");
	surfBoxListener = new CheckBoxListener(&drawSurface);
	surfBox->addMessageListener(surfBoxListener);

	movablePanel->addChildWindow(surfBox);
	
	normBox = new CheckBox(0,"Normalize");
	normBoxListener = new CheckBoxListener(&normalize);
	normBox->addMessageListener(normBoxListener);

	movablePanel->addChildWindow(normBox);

	movablePanel->arrangeChildren();
*/
}

objloader::~objloader()
{

}

objloader& objloader::operator= (const objloader &obj1)
{

	return *this;
}


bool objloader::load(string objFile)
{
	/*
	// find the last forward slash in the string
	string::size_type lastSlashPosition;
	while (lastSlashPosition = objFile.find("/")) {}
	subdir = objFile.substr(0,lastSlashPosition);
	cout << "subdir = " << subdir << "\n";
	*/
	objFile = subdir + objFile;
	
	
	ifstream objStream(objFile.c_str());
	if (!objStream) { 
		cout << "file \"" << objFile << "\" not found\n";
		throw fileNotFound(); 
		return false; 
	}
	istream_iterator<string> objIt(objStream);
	istream_iterator<string> sentinel;

	vector<string> lines(objIt,sentinel);

	theMode = NONE;
	
	//	for_each(lines.begin(),lines.end(),&bvh::process);
	
	unsigned i;
	for (i =0; i< lines.size(); i++) {
		try { process(lines[i]); }
		catch (fileNotFound) { throw fileNotFound(); return false; }
	}
	
	if (verbose) cout << vertices.size() << " vertices\n";
	if (verbose) cout << normals.size() << " normals (should == vertices)\n";
	if (verbose) cout << mtls.size() << " materials\n";
	
	/// find face normals for collision detection and other uses
	vector3f normal;
	for (i = 0; i < mtls.size(); i++)
	{				
		for (unsigned j = 0; j < mtls[i]->faces.size(); j++)
		{
			int ind0 = mtls[i]->faces[j].vertIndices[0]-1;
			int ind1 = mtls[i]->faces[j].vertIndices[1]-1;
			int ind2 = mtls[i]->faces[j].vertIndices[2]-1;
				
			normal = Cross( vertices[ind1] - vertices[ind0],
				 		vertices[ind2] - vertices[ind0] );
			
			normal = normal/(normal.Length());

			mtls[i]->faces[j].normal = normal;

		}
	}
	
	return true;
}
	
void objloader::process(string line)
{	
	int index; 
	string::size_type slashes;
	
	//cout << "Mode = " << theMode << "\n";
	if ((line.size()) && (line.substr(0,1) != "#"))
	{
		if (line == "mtllib") {
			theMode = MTLLIB;
		} else if (line == "v") {
			theMode = VERTEX;
			tempVectorIndex = 0;
		} else if (line == "vn") {
			theMode = NORMAL; 
			tempVectorIndex = 0;
		} else if (line == "f") {
			theMode = FACE;
			tempVectorIndex = 0;
		} else switch (theMode) {
		
			case (MTLLIB):
				if (verbose) cout << "\n\tparsing .mtl file: " << subdir + line << "...";
				mtlFile = line;
				
				if (!matchMtl(mtlIndex, mtlFile)) {
					try { loadMtl(mtlFile); }
					catch (fileNotFound) { throw fileNotFound(); return; }
						
					matchMtl(mtlIndex, mtlFile);
				}
				if (verbose) cout << ".mtl " << mtlIndex << " done\n";
				theMode = NONE;
			break;
			
			case (VERTEX):
				
				tempVector.vertex[tempVectorIndex] = atof(line.c_str());
				//cout << tempVector.vertex[tempVectorIndex] << "\n";
				tempVectorIndex++;
				if (tempVectorIndex >=3) {
					theMode = NONE;
					vertices.push_back(tempVector);	
				}
				
			break;
			
			case (NORMAL):
				tempVector.vertex[tempVectorIndex] = atof(line.c_str());
				tempVectorIndex++;
				if (tempVectorIndex >=3) {
					theMode = NONE;
					normals.push_back(tempVector);
				}
			
			break;
			
			case (FACE):
				/// face indices are denoted index//index (redundantly) in the .obj format
				slashes = line.find("//");
				index = atoi(line.substr(0,slashes).c_str());
				//cout << slashes << " " << index << "\n";
				tempTriangle.vertIndices[tempVectorIndex] = index;
				tempVectorIndex++;
				if (tempVectorIndex >= 3) {
					theMode = NONE;
					mtls[mtlIndex]->faces.push_back(tempTriangle);	
				}
				
			break;
			
			case (NONE):
				break;
		}
		
	}
	
		
}

void objloader::processMtl(string line, material *mtl)
{	
	if ((line.size()) && (line.substr(0,1) != "#"))
	{
		if (line == "newmtl") {
			theMtlMode = NEWMTL;
		} else if (line == "Ns") {
			theMtlMode = NS;
		} else	if (line == "d") {
			theMtlMode = D;
		} else	if (line == "illum") {
			theMtlMode = ILLUM;
		} else	if (line == "Kd") {
			theMtlMode = KD;
			kIndex = 0;
		} else	if (line == "Ka") {
			theMtlMode = KA;
			kIndex = 0;
		} else	if (line == "Ks") {
			theMtlMode = KS;
			kIndex = 0;
		} else switch (theMtlMode) {
		
			case KD:
				mtl->Kd[kIndex] = atof(line.c_str());
				kIndex++;
				if (kIndex >= 3) {
					mtl->Kd[3] = 1.0f;
					/*cout << mtl->name << " " << mtl->Kd[0] << " " << 
						mtl->Kd[1] << " " << mtl->Kd[2] << " " <<
						mtl->Kd[3] << "\n";*/
					theMtlMode = NONEM;
				}
				break;
				
			case KA:
				mtl->Ka[kIndex] = atof(line.c_str());
				kIndex++;
				if (kIndex >= 3) {
					mtl->Ka[3] = 1.0f;
					theMtlMode = NONEM;
				}
				break;
				
			case KS:
				mtl->Ks[kIndex] = atof(line.c_str());
				kIndex++;
				if (kIndex >= 3) {
					mtl->Ks[3] = 1.0f;
					theMtlMode = NONEM;
				}
				break;
				
			case (NONEM):
			case (NEWMTL):
			case (NS):
			case (D):
			case (ILLUM):
				break;
				
		}
	}
}

void objloader::loadMtl(string mtlFile)
{	
	ifstream objStream((subdir + mtlFile).c_str());
	
	if (!objStream) { 
		cout << "file \"" << subdir + mtlFile << "\" not found\n";
		throw fileNotFound(); 
		return;
	}
	
	istream_iterator<string> objIt(objStream);
	istream_iterator<string> sentinel;

	vector<string> lines(objIt,sentinel);

	theMtlMode = NONEM;
	
	material* newMtl = new material;
	newMtl->name = mtlFile;
	
	newMtl->Ke[0] = 0;
	newMtl->Ke[1] = 0;
	newMtl->Ke[2] = 0;
	newMtl->Ke[3] = 1.0f;
	
	unsigned i;
	for (i = 0; i< lines.size(); i++) {
		processMtl(lines[i],newMtl);
	}
	
	mtls.push_back(newMtl);
}

bool objloader::matchMtl(unsigned &index, string name)
{
	unsigned m;
	
	for (m = 0; m < (mtls.size()); m++) {
		
		if (name == mtls[m]->name) {
			index = m;
			return true;
		}
	}
	//cout << (int)mtls.size() <<" mtlssize \n";
	
	return false;
}


void objloader::setMass(float newMass)
{
	if (step != 0)
	{
		Ibody = (Ibody*newMass)/mass;
		IbodyInv = Ibody.Inverse();
	}

	mass = newMass;

}

/*void objloader::InitTexture()
{
//	image = auxDIBImageLoad("spaceshipTex2.bmp");
//LoadBMP("spaceshipTex2.bmp");

//	GLfloat env_color[4];
//	GLfloat border_color[4];
    
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, env_color);

//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->sizeX, image->sizeX, GL_RGB, GL_UNSIGNED_BYTE, image->data);

}
*/

void objloader::update(float seconds)
{
	getAABB();

	/*
	if (counter <90)
		location.RotateX(1);
	else if (counter <180)
		location.RotateY(1);
	else if (counter <300)
		location.Translate(0,0,0.2f);
		//location.RotateY(1);
		//location.RotateZ(1);
	else if (counter < 290)
		location.RotateX(1);
	else if (counter < 400)
		location.Translate(0,0,0.2f);
	else
		counter = -1;

	counter++;*/


}

void objloader::draw(void)
{
	return;
}


void objloader::getBoundingBox()
{
	/*	
	for (int j = 0; j<3;j++)
	{
		// lower right bound
		boundingBox[0].vertex[j] = test[0]->location.vertex[j];

		// upper right bound
		boundingBox[6].vertex[j] = test[0]->location.vertex[j];
	}
	*/
	
	boundingBox[0] = vertices[1];
	boundingBox[6] = vertices[1];
			
	// find extremities
	for (unsigned i = 1; i < vertices.size(); i++)
	{
		for (int j = 0; j<3;j++)
		{
		// lower left   minimum bound 
		if (boundingBox[0].vertex[j] > vertices[i].vertex[j])
			boundingBox[0].vertex[j] = vertices[i].vertex[j];

		// upper right  maximum bound 
		if (boundingBox[6].vertex[j] < vertices[i].vertex[j])
			boundingBox[6].vertex[j] = vertices[i].vertex[j];
	
		}
	}

	centerBB = (boundingBox[0] + boundingBox[6])/2;
	edgesBB = boundingBox[6] - centerBB;

/*	Log("%f %f %f\n\n",
		edgesBB.vertex[0],
		edgesBB.vertex[1],
		edgesBB.vertex[2]);*/



	// having found maximums, populate rest of corners
	boundingBox[1].vertex[0] = boundingBox[0].vertex[0];
	boundingBox[1].vertex[1] = boundingBox[0].vertex[1];
	boundingBox[1].vertex[2] = boundingBox[6].vertex[2];
	
	boundingBox[2].vertex[0] = boundingBox[0].vertex[0];
	boundingBox[2].vertex[1] = boundingBox[6].vertex[1];
	boundingBox[2].vertex[2] = boundingBox[6].vertex[2];
	
	boundingBox[3].vertex[0] = boundingBox[0].vertex[0];
	boundingBox[3].vertex[1] = boundingBox[6].vertex[1];
	boundingBox[3].vertex[2] = boundingBox[0].vertex[2];
		
	boundingBox[4].vertex[0] = boundingBox[6].vertex[0];
	boundingBox[4].vertex[1] = boundingBox[0].vertex[1];
	boundingBox[4].vertex[2] = boundingBox[0].vertex[2];
	
	boundingBox[5].vertex[0] = boundingBox[6].vertex[0];
	boundingBox[5].vertex[1] = boundingBox[0].vertex[1];
	boundingBox[5].vertex[2] = boundingBox[6].vertex[2];
	

	boundingBox[7].vertex[0] = boundingBox[6].vertex[0];
	boundingBox[7].vertex[1] = boundingBox[6].vertex[1];
	boundingBox[7].vertex[2] = boundingBox[0].vertex[2];

}


