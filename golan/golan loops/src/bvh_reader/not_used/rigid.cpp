#ifdef WIN32
#include <windows.h>
#endif

#include <iostream>

#include <GL/gl.h>												
#include <GL/glu.h>	

#include "rigid.hpp"

using namespace std;

rigid::rigid()
{
	
}

rigid::~rigid()
{
	
}


rigid::rigid(string objFile)
{
	//objloader* 
	theObj = new objloader(objFile);
	
	vertices = theObj->vertices;
	normals = theObj->normals;
	mtls = theObj->mtls;
	
	translucent  = theObj->translucent;
	centerOfMass = theObj->centerOfMass;
	
	drawSurface = true;
	
	//temp
	//delete theObj;
}


void rigid::draw()
{
	glPushMatrix();

	if (drawBB) {
		drawAABB();
	}
	
	glMultMatrixf(location.matrix);


	//	glMaterialfv(GL_FRONT,GL_SHININESS,&Se);
	glEnable(GL_LIGHTING);
	//lists arent being generated for some reason
	
	if (drawSurface)
		makeList();
	//	glCallList(listNum);
	
	glDisable(GL_LIGHTING);
//	if (drawMoI)
//		drawMoI();
	
		/*cout << theObj->iStack[theObj->iStack.size() - 1].vertex[0] << " " <<
				theObj->iStack[theObj->iStack.size() - 1].vertex[1] << " " <<
				theObj->iStack[theObj->iStack.size() - 1].vertex[2] << "\n";
	*/
	
#ifdef SHOW_TRI_NORMS
	glColor3f (0.3f, 0.6f, 0.9f);
	glBegin(GL_LINES);
	for (unsigned m = 0; m < mtls.size(); m++) {			
		for (unsigned i = 0; i <mtls[m]->faces.size(); i++) {
			glVertex3fv(	vertices[mtls[m]->faces[i].vertIndices[0]-1].vertex);
			glVertex3fv( ((vertices[mtls[m]->faces[i].vertIndices[0]-1] + mtls[m]->faces[i].normal)).vertex );
		}
	}	
	glEnd();	
#endif
			
#ifdef SHOW_INT_POINTS
		glBegin (GL_POINTS);
		glColor3f (0.3f, 0.6f, 0.9f);
		for (unsigned i = 0; i < theObj->iStack.size(); i++) {		
			glVertex3fv (theObj->iStack[i].vertex);	
		}
		
		glColor3f (0.9f, 0.6f, 0.3f);
		for (unsigned i = 0; i < theObj->upperI.size(); i++) {		
			glVertex3fv (theObj->upperI[i].vertex);	
		}
		
		glColor3f (0.3f, 0.9f, 0.3f);
		for (unsigned i = 0; i < theObj->lowerI.size(); i++) {		
			glVertex3fv (theObj->lowerI[i].vertex);	
		}
		glEnd ();
#endif
	
	
	//makeList();
	if (drawBB) {	

/*		glDisable(GL_LIGHTING);

		glBegin(GL_POINTS);
	//	glBegin(GL_LINES);

		
		for (unsigned int i =0; i < surface.size(); i++ ) {
			glColor3f(0.7f,0.7f,0.7f);
			glVertex3fv(surface[i].vertex);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3fv( ( surface[i]+(velocity+Cross(omega,surface[i]))*5.0f ).vertex );
		}
		glEnd();	

		glEnable(GL_LIGHTING);
		*/
		//for_each(iStack.begin(),iStack.end(),glVertex3fv);
		drawBoundingBox();


		// draw Center of Mass
		glBegin(GL_LINES);
		glColor3f(0.2f,0.3f,1.0f);
		vector3f up(0,0.5,0);
		glVertex3fv((centerOfMass+up).vertex);
		glVertex3fv((centerOfMass-up).vertex);
		vector3f right(.5,0,0);
		glVertex3fv((centerOfMass+right).vertex);
		glVertex3fv((centerOfMass-right).vertex);
		vector3f out(0,0,.5);
		glVertex3fv((centerOfMass+out).vertex);
		glVertex3fv((centerOfMass-out).vertex);
		glEnd();
	}


	/// draw collision triangles
	/*
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin (GL_TRIANGLES);

		for (unsigned int m = 0; m < numMtl; m++)
		{
			//float Ka[4] = {1.0f,.4f,0.3f,0.2f};
			//glMaterialfv(GL_FRONT,GL_AMBIENT, Ka);

			
			glColor4f(1.0f,1.0f,1.0f,0.1f);

			
			for (int i = 0; i <mtls[m]->numFaces; i++)
			{
				if (mtls[m]->faces[i]->collision)
				{
				glNormal3fv(	norms[mtls[m]->faces[i]->vertIndices[0]-1]->vertex);
				glVertex3fv(	points[mtls[m]->faces[i]->vertIndices[0]-1]->vertex);
				
				glNormal3fv(	norms[mtls[m]->faces[i]->vertIndices[1]-1]->vertex);
				glVertex3fv(	points[mtls[m]->faces[i]->vertIndices[1]-1]->vertex);
				
				glNormal3fv(	norms[mtls[m]->faces[i]->vertIndices[2]-1]->vertex);
				glVertex3fv(	points[mtls[m]->faces[i]->vertIndices[2]-1]->vertex);
				}
			}
			
		}

	glEnd();
	glDisable(GL_BLEND);
	//glEnable(GL_LIGHTING);
	*/
	////////////////////////////////////////////

	/*	
	// draw points for each vertex
	glLineWidth (0.08f);
	glColor3f (0.5f, 0.9f, 0.3f);
	//glEnable (GL_LINE_SMOOTH);
	glBegin (GL_POINTS);
	for (int i = 0; i <numVertices; i++)
	{
		glVertex3f(points[i]->vertex[0],points[i]->vertex[1],points[i]->vertex[2]);
	}
	glEnd();
	

	// opengl stores locations as	[right up out	] not	[right	]
	//								[				]		[up		]
	//								[				]		[out	]


//	matrix16f temp;
//	glGetFloatv(GL_MODELVIEW_MATRIX, temp.matrix);
//	vector3f p1,p2,p3;

	glBegin (GL_TRIANGLES);
	for (int i = 0; i <numFaces; i++)
	{
		//glColor3f ((float)i/numFaces, 0.9f, 0.3f);
		// obj indices start from 1, so subtract 1

		// Use own transform.
		p1 = location.Transform(*points[faces[i]->vertIndices[0]-1]);
		p2 = location.Transform(*points[faces[i]->vertIndices[1]-1]);
		p3 = location.Transform(*points[faces[i]->vertIndices[2]-1]);

		glVertex3fv(p1.vertex);
		glVertex3fv(p2.vertex);
		glVertex3fv(p3.vertex);
		

	}
	glEnd();
*/
	glPopMatrix();

	return;
}

void rigid::drawDim(vector<light*> lights)
{
	glPushMatrix();
	
	//glMultMatrixf(location.matrix);
	glEnable(GL_LIGHTING);
				
	for (unsigned int m = 0; m < mtls.size(); m++) {

			glMaterialfv(GL_FRONT,GL_AMBIENT, mtls[m]->Ka);
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mtls[m]->Kd);
			glMaterialfv(GL_FRONT,GL_SPECULAR,mtls[m]->Ks);
			glMaterialfv(GL_FRONT,GL_EMISSION,mtls[m]->Ke);

			if (translucent)
				glColor4f(1.0f,1.0f,1.0f,mtls[m]->opacity);

			
			for (unsigned i = 0; i <mtls[m]->faces.size(); i++) {

				unsigned j;
				
				//location.Scale(6.5f,7.5f,6.5f);
				matrix16f temp = location;
				temp.Scale(6.5f,7.5f,6.5f);
				for (j = 0; j < lights.size(); j++) {
					lights[j]->drawDim(temp.Transform(vertices[mtls[m]->faces[i].vertIndices[0]-1]));
				}
				
				/*
				for (int j = 0; j < lights.size(); j++) {
					lights[j]->drawDim(vector3f());
				}
				*/
				glPushMatrix();
				
				glMultMatrixf(location.matrix);
				
				glScalef(6.5f,7.5f,6.5f);
				
						
				glBegin (GL_TRIANGLES);
	
				glNormal3fv(	normals[mtls[m]->faces[i].vertIndices[0]-1].vertex);
				glVertex3fv(	vertices[mtls[m]->faces[i].vertIndices[0]-1].vertex);
				
				
				/*
				for (j = 0; j < lights.size(); j++) {
					lights[j]->drawDim(vertices[mtls[m]->faces[i].vertIndices[1]-1]);
				}*/
				
				glNormal3fv(	normals[mtls[m]->faces[i].vertIndices[1]-1].vertex);
				glVertex3fv(	vertices[mtls[m]->faces[i].vertIndices[1]-1].vertex);
				/*
				for (j = 0; j < lights.size(); j++) {
					lights[j]->drawDim(vertices[mtls[m]->faces[i].vertIndices[2]-1]);
				}*/
				
				glNormal3fv(	normals[mtls[m]->faces[i].vertIndices[2]-1].vertex);
				glVertex3fv(	vertices[mtls[m]->faces[i].vertIndices[2]-1].vertex);

				glEnd();
				glPopMatrix();
			}
			
		}

	//	glEnd();
	
	glDisable(GL_LIGHTING);
	
	glPopMatrix();

}

void rigid::makeList()
{

	//listNum = glGenLists(1);

	//cout << "\tlist " << listNum << "\n";

	//glNewList(listNum, GL_COMPILE);

		glBegin (GL_TRIANGLES);

		for (unsigned int m = 0; m < mtls.size(); m++) {

			glMaterialfv(GL_FRONT,GL_AMBIENT, mtls[m]->Ka);
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mtls[m]->Kd);
			glMaterialfv(GL_FRONT,GL_SPECULAR,mtls[m]->Ks);
			glMaterialfv(GL_FRONT,GL_EMISSION,mtls[m]->Ke);

			if (translucent)
				glColor4f(1.0f,1.0f,1.0f,mtls[m]->opacity);

			
			for (unsigned i = 0; i <mtls[m]->faces.size(); i++) {

				glNormal3fv(	normals[mtls[m]->faces[i].vertIndices[0]-1].vertex);
				glVertex3fv(	vertices[mtls[m]->faces[i].vertIndices[0]-1].vertex);
				
				glNormal3fv(	normals[mtls[m]->faces[i].vertIndices[1]-1].vertex);
				glVertex3fv(	vertices[mtls[m]->faces[i].vertIndices[1]-1].vertex);
				
				glNormal3fv(	normals[mtls[m]->faces[i].vertIndices[2]-1].vertex);
				glVertex3fv(	vertices[mtls[m]->faces[i].vertIndices[2]-1].vertex);

			}
			
		}

		glEnd();

	//glEndList();

}

void rigid::update()
{
	
}


void rigid::getBoundingBox()
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
