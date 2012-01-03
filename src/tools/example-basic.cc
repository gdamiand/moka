/*
 * Example : Un example d'utilisation de la librairie de 3-G-cartes.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of Example
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include "g-map-vertex.hh"
#include "corefinement-api.hh"

using namespace std;
using namespace GMap3d;

CGMapVertex g;

CDart* createTriangle()
{
  CDart* d = g.addMapDart();  
  g.linkAlpha0(d,g.addMapDart());

  CDart* d2 = g.addMapDart();  
  g.linkAlpha0(d2,g.addMapDart());

  CDart* d3 = g.addMapDart();  
  g.linkAlpha0(d3,g.addMapDart());

  g.linkAlpha1(d,d2);
  g.linkAlpha1(g.alpha0(d2),d3);
  g.linkAlpha1(g.alpha0(d3),g.alpha0(d));

  return d;
}

CDart* createTriangleGeo()
{
  CDart* d = createTriangle();
  g.setVertex(d, new CAttributeVertex(-10,0,2));
  g.setVertex(g.alpha0(d), new CAttributeVertex(15,0,2));
  g.setVertex(g.alpha10(d), new CAttributeVertex(15,15,2));
  return d;
}

CDart* createTetra()
{
  CDart* d1 = createTriangle();
  CDart* d2 = createTriangle();
  CDart* d3 = createTriangle();
  CDart* d4 = createTriangle();

  g.topoSew2(d1,d2);
  g.topoSew2(d3,g.alpha01(d2));
  g.topoSew2(g.alpha1(d3),g.alpha01(d1));

  g.topoSew2(d4,g.alpha1(d2));
  g.topoSew2(g.alpha1(d4),g.alpha1(d1));
  g.topoSew2(g.alpha01(d4),g.alpha01(d3));
  
  g.setVertex(d1, new CAttributeVertex(0,0,5));
  g.setVertex(d3, new CAttributeVertex(5,0,0));
  g.setVertex(g.alpha0(d3), new CAttributeVertex(5,5,0));
  g.setVertex(g.alpha10(d3), new CAttributeVertex(0,5,0));

  return d1;
}

int main()
{
  CDart* d1=createTetra();
  CDart* d2=createTriangleGeo();

  CCorefinementAPI c(&g);

  g.save("res1.moka",AsciiFormat);

  c.corefine3dMeshes(d1,d2);

  g.save("res2.moka",AsciiFormat);

  int m = g.getNewMark();

  int nbVol  =0;
  int nbFace = 0;
  
  for (CDynamicCoverageAll it(&g); it.cont(); ++it)
    {
      if (!g.isMarked(*it, m))
	{
	  // Ici on a trouvé un nouveau volume
	  ++nbVol;
	  for (CDynamicCoverageVolume it2(&g, *it); it2.cont(); ++it2)
	    {
	      if (!g.isMarked(*it2, m))
		{
		  // Ici on a trouvé une nouvelle face
		  ++nbFace;
		  for (CDynamicCoverageFace it3(&g,*it2);it3.cont();++it3)
		    g.setMark(*it3,m);
		}
	    }
	}
    }
  
  
  cout<<"Nombre de volumes : "<<nbVol<<", et nombre de faces : "<<nbFace<<endl;
}
