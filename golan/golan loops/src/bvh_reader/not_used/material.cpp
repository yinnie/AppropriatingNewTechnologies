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

#include "material.hpp"

material::material()
{
}

material::~material()
{

}

material& material::operator= (const material &mat)
{
	name = mat.name;
	faces = mat.faces;
	
	Se = mat.Se;
	
	for (int i=0; i <4; i++)
	{	
		Ka[i] = mat.Ka[i];
		Ks[i] = mat.Ks[i];
		Kd[i] = mat.Kd[i];
		
		Ke[i] = mat.Ke[i];	
	}
	
	opacity = mat.opacity;

	return *this;
}
