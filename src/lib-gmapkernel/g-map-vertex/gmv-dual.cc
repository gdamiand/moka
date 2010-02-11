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
using namespace GMap3d;
//******************************************************************************
#define DUP(D) (getDirectInfoAsDart(D, directInfoIndex))
//******************************************************************************
int CGMapVertex::dualOfMarkedDarts(int ADim, int AMarkNumber)
{
  assert((2 <= ADim) && (ADim <= 3));

  int directInfoIndex = getNewDirectInfo();
  int treated         = getNewMark();

  // Duplication des brins:
  duplicateMarkedDarts(AMarkNumber, this, directInfoIndex, false,
		       ADim>=0, ADim>=1, ADim>=2, ADim>=3);

  // Mise à jour des coutures (algo. local):
  int nbDuplicated = 0;
  int middle = (ADim-1)/2;

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	setMark(*it, treated);

	if (isMarked(*it, AMarkNumber))
	  {
	    CDart * dup = DUP(*it);

	    ++nbDuplicated;

	    setDirectInfo(dup, directInfoIndex, *it);

	    for (int dim = 0; dim <= middle; ++dim)
	      {
		// Permutation:
		CDart * tmp = alpha(dup, dim);
		dup->setAlpha(alpha(dup, ADim-dim), dim);
		dup->setAlpha(tmp, ADim-dim);
	      }
	  }
      }

  negateMaskMark(treated);

  // Maintenant on affecte le plongement sommet des nouveaux brins:
  for (it.reinit(); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    // À optimiser car le barycentre peut être calculé plusieurs fois...
	    setVertex(DUP(*it), barycenter(*it, ORBIT_CELL[ADim]));

	    for (CDynamicCoverageVertex vertex(this, DUP(*it));
		 vertex.cont(); ++vertex)
	      setMark(DUP(*vertex), treated);
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);

  // Libérations:
  freeDirectInfo(directInfoIndex);
  freeMark       (treated);

  return nbDuplicated;
}
//******************************************************************************
#undef DUP
//******************************************************************************
