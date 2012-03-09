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


#include <GL/gl.h>
#include <GL/glu.h>

#include "body.hpp"

body::body(string bvhFile)
{
	counter = 1;
	theBvh = new bvh(bvhFile);
	crement = 1;

	//vector3f zero;
	//joint.push(zero);
	display = OBLOIDS;
	matrix16f ident;
	ident.LoadIdentity();
 	//ident.matrix[10]=-1.0f;
	orient.push(ident);
	printf("Good job, made body\n");
}

body::~body()
{

}

void body::update()
{
	counter+= crement;

	if (counter >= theBvh->root->motion.size()) {
		crement = 0-crement;
  }
  if (counter <= 1){
		crement = 0-crement;
  }
}

void body::update(int counterOffset)
{

	counter+= crement*counterOffset;

	if (counter >= theBvh->root->motion.size()) {
		crement = 0-crement;
  }
  if (counter <= 1){
		crement = 0-crement;
  }

	/*
	counter += counterOffset;

	if (counter >= theBvh->root->motion.size()) counter %= theBvh->root->motion.size();
	else if (counter < 0)
		counter = 0;//theBvh->root->motion.size() - (-counter%theBvh->root->motion.size());
		*/


}

void body::draw()
{
	glPushMatrix();

	/*
	/// draw lines on xyz axes
	glColor3f(0.5f,0.0f,0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(1000.0f,0.0f,0.0f);
	glColor3f(0.0f,0.5f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,1000.0f,0.0f);
	glColor3f(0.0f,0.0f,0.5f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0f,1000.0f);
	glEnd();
	*/

// glColor3f(1.0f,0.0f,0.5f);

// if (drawBB) drawAABB();
//	glBegin(GL_LINE_STRIP);
// glBegin(GL_POINTS);
	recursDraw(theBvh->root);
//	glEnd();

	glPopMatrix();
}

void body::switchDisplay()
{
	if (display == OBJ)
		display = LINES;
	else if (display == LINES)
		display = OBLOIDS;
	else if (display == OBLOIDS)
		display = OBJ;


}

void body::recursDraw(bvhPart* part)
{
	GLUquadricObj *q = gluNewQuadric();

	// motion[0] holds the offset, so it is also needed
	matrix16f ident = orient.top() * part->motion[0];
//	ident.Rotate(part->motion[counter]);  //rotate in place
	//if (part == theBvh->root)	part->motion[counter].print();

	ident.Translate(
		part->motion[counter].matrix[12],
		part->motion[counter].matrix[13],
		part->motion[counter].matrix[14]);
	ident.Rotate(part->motion[counter]);  //rotate in place

	orient.push(ident);

	vector3f up,right,out;
	matrix16f face;
	face.LoadIdentity();

	float BB = 1.0;

	unsigned i = 0;
	for (i = 0; i < part->child.size(); i++) {
		vector3f offset = part->child[i]->offset;
		// display = LINES;
		float upX,upY,upZ;

		switch (display) {

			case LINES:
				/*
				if (part->child[i]->child.size() == 0) glColor3f(0.9f,0.5f,0.0f);
				else glColor3f(1.0f,0.1f,0.1f);
				*/

				glPushMatrix();
				glMultMatrixf(ident.matrix);

				glBegin(GL_LINES);
				//glVertex3fv( (ident.Transform(vector3f())).vertex );
				//glVertex3fv( (ident.Transform(part->child[i]->offset)).vertex );
				glVertex3f( 0,0,0 );
				glVertex3fv( offset.vertex );
				glEnd();
				glPopMatrix();
				break;

		case OBLOIDS:
			glPushMatrix();

			glMultMatrixf(ident.matrix);
			glTranslatef(	offset.vertex[0]/2,
								offset.vertex[1]/2,
								offset.vertex[2]/2);


			upX = upY = upZ = 1.0f;
			if (offset.vertex[2] != 0)
				upZ = -(offset.vertex[0] +
						offset.vertex[1])/
						offset.vertex[2];
			else if (offset.vertex[1] != 0)
				upY = -(offset.vertex[0] +
						offset.vertex[2])/
						offset.vertex[1];
			else if (offset.vertex[0] != 0)
				upX = -(offset.vertex[2] +
						offset.vertex[1])/
						offset.vertex[0];

			out = offset;
			out = out/out.Length();

			up.vertex[0] = upX; up.vertex[1] = upY; up.vertex[2] = upZ;
			up = up/up.Length();
			right = Cross(out,up);



			face.Set(right,up,out);
			glMultMatrixf(face.matrix);
			glScalef(BB,BB, part->child[i]->offset.Length()/2);
			gluSphere(q,1,9,9);
			glPopMatrix();


			// THIS IS GOOD
			glPushMatrix();
			glMultMatrixf(ident.matrix);
			glBegin(GL_LINES);
			glTranslatef(offset.vertex[0],offset.vertex[1],offset.vertex[2]);
			gluSphere(q,BB*0.66,7,7);
			glEnd();
			glPopMatrix();


/*
			glMultMatrixf(ident.matrix);
			glTranslatef(	offset.vertex[0],
								offset.vertex[1],
								offset.vertex[2]);


			upX = upY = upZ = 1.0f;
			if (offset.vertex[2] != 0)
				upZ = -(offset.vertex[0] +
						offset.vertex[1])/
						offset.vertex[2];
			else if (offset.vertex[1] != 0)
				upY = -(offset.vertex[0] +
						offset.vertex[2])/
						offset.vertex[1];
			else if (offset.vertex[0] != 0)
				upX = -(offset.vertex[2] +
						offset.vertex[1])/
						offset.vertex[0];

			out = offset;
			out = out/out.Length();

			up.vertex[0] = upX; up.vertex[1] = upY; up.vertex[2] = upZ;
			up = up/up.Length();
			right = Cross(out,up);

			face.Set(right,up,out);
			glMultMatrixf(face.matrix);
			glScalef(3.0f, 3.0f, 3.0f);
			gluSphere(q,1,15,15);
			glPopMatrix();*/


			break;

/*
		case OBLOIDS:

			/// gluSphere instead of body parts
			glPushMatrix();

			glMultMatrixf(ident.matrix);

			/*
			glEnable(GL_LIGHTING);

			GLfloat Ka[4]; // Ambient
			GLfloat Kd[4]; // Diffuse
			GLfloat Ks[4]; // Specular

			Ka[0]=0.9f; Ka[1]=0.6f; Ka[2]=0.3f; Ka[3]=1.0f;
			Kd[0]=0.8f; Kd[1]=0.7f; Kd[2]=0.5f; Kd[3]=1.0f;
			Ks[0]=0.3f; Ks[1]=0.3f; Ks[2]=0.3f; Ks[3]=1.0f;


			/// put spheres at center of limb rather than at joint
			glTranslatef(	offset.vertex[0]/2,
								offset.vertex[1]/2,
								offset.vertex[2]/2);

			/// generate arbitrary up vector- doesn't matter
			/// since obloid is symmetrical
			upX = upY = upZ = 1.0f;
			if (offset.vertex[2] != 0)
				upZ = -(offset.vertex[0] +
						offset.vertex[1])/
						offset.vertex[2];
			else if (offset.vertex[1] != 0)
				upY = -(offset.vertex[0] +
						offset.vertex[2])/
						offset.vertex[1];
			else if (offset.vertex[0] != 0)
				upX = -(offset.vertex[2] +
						offset.vertex[1])/
						offset.vertex[0];

			out = offset;
			out = out/out.Length();

			up.vertex[0] = upX; up.vertex[1] = upY; up.vertex[2] = upZ;
			up = up/up.Length();
			right = Cross(out,up);

			face.Set(right,up,out);
			glMultMatrixf(face.matrix);

			//cout << "Dot " << dot(some,part->child[i]->offset) << "\n";

			// orient the matrix along the offset axis
			/*gluLookAt(0,
						0,
						0,
						part->child[i]->offset.vertex[0],
						part->child[i]->offset.vertex[1],
						part->child[i]->offset.vertex[2],
						upX,upY,upZ);



			glMaterialfv(GL_FRONT,GL_AMBIENT, Ka);
			glMaterialfv(GL_FRONT,GL_DIFFUSE, Kd);
			glMaterialfv(GL_FRONT,GL_SPECULAR,Ks);


			glScalef(3.0f, 3.0f, part->child[i]->offset.Length()/2);

			/// scale the sphere so it fills the whole limb joint-to-joint
			/*if (part->child[i]->offset.vertex[0] != 0)
				glScalef(part->child[i]->offset.vertex[0]/2, 1.0f,1.0f);
			if (part->child[i]->offset.vertex[1] != 0)
				glScalef(1.0f, part->child[i]->offset.vertex[1]/2, 1.0f);
			if (part->child[i]->offset.vertex[2] != 0)
				glScalef(1.0f, 1.0f, part->child[i]->offset.vertex[2]/2);



			//gluSphere(q,part->child[i]->offset.Length(),15,15);
			gluSphere(q,1,15,15);


			glDisable(GL_LIGHTING);

			glPopMatrix();
			///
			break;
			*/

		case OBJ:

			/*
			if (part->objPart) {

				glPushMatrix();
				//glMultMatrixf(ident.matrix);
				part->objPart->location = ident;
				//part->objPart->location.Scale(6.5f,7.5f,6.5f);
				//glScalef(6.5f,7.5f,6.5f);
				glEnable(GL_LIGHTING);
			//	cout << part->objPart->name << "\n";
				part->objPart->drawDim(lights);
			//	part->objPart->draw();
				glDisable(GL_LIGHTING);

				glPopMatrix();
			}
			*/
			break;
	}

		if (part->child[i]->child.size() != 0) recursDraw(part->child[i]);
	}
	gluDeleteQuadric(q);
	orient.pop();
}
