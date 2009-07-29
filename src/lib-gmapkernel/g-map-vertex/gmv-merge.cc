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
#include "geometry.hh"
using namespace GMap3d;
//******************************************************************************
int CGMapVertex::mergeMarkedColinearEdges(int AMarkNumber, bool ADeleteDarts)
{
  return mergeMarkedAlignedCells(1, AMarkNumber, ADeleteDarts);
}
//******************************************************************************
int CGMapVertex::mergeMarkedCoplanarFaces(int AMarkNumber, bool ADeleteDarts)
{
  return mergeMarkedAlignedCells(2, AMarkNumber, ADeleteDarts);
}
//******************************************************************************
int CGMapVertex::mergeMarkedAlignedCells(int ADim,
					 int AMarkNumber, bool ADeleteDarts)
{
  assert(ADim==1 || ADim==2);

  CDynamicCoverageAll it(this);

  int selected = getNewMark();
  int treated  = getNewMark();

  // Repérage des cellules de dimension ADim-1 sélectionnées incidentes à des
  // cellules de dimension ADim alignées:

  markIncidentCells(ORBIT_CELL[ADim-1], AMarkNumber, selected);

  // Démarquage des cellules qui ne peuvent pas ou ne doivent pas être
  // fusionnées:

  for (it.reinit(); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, selected))
	  {
	    bool possible =
	      !isFree(*it, ADim) && canMerge(*it, alpha(*it,ADim), ADim);

	    if (possible)
	      {
		CDart * side1 = *it;
		CDart * side2 = alpha(*it,ADim);

		CVertex vector1 =
		  ADim==1 ? edgeVector(side1) : faceNormalVector(side1);

		CVertex vector2 =
		  ADim==1 ? edgeVector(side2) : faceNormalVector(side2);

		possible = CGeometry::areColinear(vector1, vector2);
	      }

	    if (!possible)
	      unmarkOrbit(*it, ORBIT_CELL[ADim-1], selected);

	    markOrbit(*it, ORBIT_CELL[ADim-1], treated);
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);

  // Fusion effective:
  int nbMerged = isolateMarkedCells(selected, ADim-1, ADeleteDarts, false);

  freeMark(treated);

  unmarkAll(selected);
  freeMark(selected);

  return nbMerged;
}
//******************************************************************************
