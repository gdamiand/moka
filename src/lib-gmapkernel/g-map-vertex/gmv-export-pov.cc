/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//******************************************************************************
#include "g-map-vertex.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
#define PRINT_VERTEX(V) \
	AOutStream << "<" << (V)->getX() << "," << (V)->getZ() << "," \
                   << (V)->getY() << ">"
//******************************************************************************
bool CGMapVertex::exportToPov(ofstream & AOutStream)
{
  int treated = getNewMark();

  AOutStream
    << "// Compilation:" << endl
    << "// $ povray +A0.1 +W640 +H480 +I<thisFile.pov>" << endl
    << endl
    << "#include \"colors.inc\"" << endl
    << "#include \"textures.inc\"" << endl
    << "#include \"finish.inc\"" << endl
    << endl
    << "camera" << endl
    << "{" << endl
    << "\t" << "location <-1,3,-7>" << endl
    << "\t" << "right 640/480*x" << endl
    << "\t" << "look_at <0,0,0>" << endl
    << "}" << endl
    << endl
    << "light_source" << endl
    << "{"
    << "\t" << "<-10,20,-20>" << endl
    << "\t" << "color White" << endl
    << "}" << endl
    << endl
    << "light_source" << endl
    << "{"
    << "\t" << "<0,10,-20>" << endl
    << "\t" << "color White/2" << endl
    << "}" << endl
    << endl
    << "#declare the_texture =" << endl
    << "texture" << endl
    << "{" << endl
    << "\t" << "pigment { rgbf <0.5,0.9,1, 0.7> }" << endl
    << "\t" << "finish { phong 0.9 phong_size 10 reflection 0.1 }" << endl
    << "}" << endl
    << endl
    << "mesh" << endl
    << "{" << endl;

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	markOrbit(*it, ORBIT_FACE, treated);

	if (isClosedPolyline(*it))
	  {
	    int nbVertices = getNbPolylineVertices(*it);

	    if (nbVertices >= 3)
	      {
		if (nbVertices == 3)
		  {
		    AOutStream << "\ttriangle {";

		    for (CDynamicCoverage01 face(this, *it);
			 face.cont(); ++face, ++face)
		      {
			CVertex & vertex = * findVertex(*face);

			if (*face != *it)
			  AOutStream << ",";

			AOutStream << " ";
			PRINT_VERTEX(& vertex);
		      }

		    AOutStream << " }" << endl;
		  }
		else
		  {
		    CVertex bary = barycenter(*it, ORBIT_01);
		    CVertex * pred = findVertex(alpha10(*it));

		    for (CDynamicCoverage01 face(this, *it);
			 face.cont(); ++face, ++face)
		      {
			CVertex * vertex = findVertex(*face);

			AOutStream << "\ttriangle { ";
			PRINT_VERTEX(& bary);
			AOutStream << ", ";
			PRINT_VERTEX(pred);
			AOutStream << ", ";
			PRINT_VERTEX(vertex);
			AOutStream << " }" << endl;

			pred = vertex;
		      }
		  }
	      }
	  }
      }

  negateMaskMark(treated);
  freeMark(treated);

  AOutStream << endl
	     << "\t" << "texture { the_texture }" << endl
	     << "}" << endl;

  return true;
}
//******************************************************************************
#undef PRINT_VERTEX
//******************************************************************************
