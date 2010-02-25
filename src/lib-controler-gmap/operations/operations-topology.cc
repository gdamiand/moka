/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
#include "compute-homology.hh"
#include "controler-gmap.hh"
#include <cassert>

using namespace GMap3d;
using namespace std;
//******************************************************************************
unsigned int CControlerGMap::countNonLocalDegreeTwoEdges()
{
  unsigned int nb=FMap->countNonLocalDegreeTwoEdges();
  if (nb==0)
    {
      setMessage("No edge with local degre > 2: "
		 "the map is a 2D quasi manifold.");
    }
  else
    {
      if (nb==1)
	setMessage("1 edge with local degre > 2: "
		   "the map is NOT a 2D quasi manifold.");
      else
	setMessage(nb," edges with local degre > 2: "
		   "the map is NOT a 2D quasi manifold.");
    }
  return nb;
}
//******************************************************************************
void CControlerGMap::getMapGlobalCharacteristics(int* ANbDarts,
						 int* ANbVertices, int* ANbEdges,
						 int* ANbFaces, int* ANbVolumes,
						 int* ANbCC,
						 int* ANb0Borders, int* ANb1Borders,
						 int* ANb2Borders, int* ANb3Borders)
{
  FMap->getGlobalCharacteristics(ANbDarts,
				 ANbVertices, ANbEdges, ANbFaces, ANbVolumes,
				 ANbCC,
				 ANb0Borders, ANb1Borders, ANb2Borders,
				 ANb3Borders);
}
//******************************************************************************
bool CControlerGMap::getSurfaceCharacteristics(string* AName,
					       int* ANbDarts,
					       int* ANbVertices, int* ANbEdges,
					       int* ANbFaces,
					       int* ANb0Borders, int* ANb1Borders,
					       int* ANb2Borders,
					       int* ANb2BordersWhenClosed,
					       int* AEuler, int* AOrient,
					       int* AGenus)
{
  // Caractéristiques de l'objet pointé:
  CDart* root;
  CDart* last = FParameterSelection->getLastSelectedDart();

  int n = FMap->getMarkedCells(ORBIT_VOLUME,
			       FParameterSelection->getSelectionMark(),
			       last, & root);

  bool ok = n==1 || (n>1 && last != NULL);

  if (ok)
    {
      int nb2WhenClosed;
      int orient;
      int genus;

      int* pNb2WhenClosed = ANb2BordersWhenClosed;
      int* pOrient        = AOrient;
      int* pGenus         = AGenus;

      if (AName == NULL)
	{
	  if (pNb2WhenClosed == NULL) pNb2WhenClosed = & nb2WhenClosed;
	  if (pOrient        == NULL) pOrient        = & orient       ;
	  if (pGenus         == NULL) pGenus         = & genus        ;
	}

      FMap->getSurfaceCharacteristics(root, ANbDarts,
				      ANbVertices, ANbEdges, ANbFaces,
				      ANb0Borders, ANb1Borders, ANb2Borders,
				      pNb2WhenClosed,
				      AEuler, pOrient, pGenus);

      if (AName != NULL)
	*AName = treatAccent(FMap->getSurfaceName(*pNb2WhenClosed, *pOrient,
						  *pGenus));
    }

  return ok;
}
//******************************************************************************
