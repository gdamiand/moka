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
#define DUP1(D) (getDirectInfoAsDart(D, directInfoIndex1))
#define DUP2(D) (getDirectInfoAsDart(D, directInfoIndex2))
//******************************************************************************
int CGMapVertex::thickenMarkedDarts(int AMarkNumber, TCoordinate AExtrusionCoef)
{
  int directInfoIndex1 = getNewDirectInfo();
  int directInfoIndex2 = getNewDirectInfo();

  // Topologie:
  int nbThickened =
    CGMapGeneric::thickenMarkedDarts(AMarkNumber,
				     directInfoIndex1, directInfoIndex2);

  if (nbThickened>0)
    {
      // Géométrie:
      CDynamicCoverageAll it(this);
      int treated = getNewMark();
      int exterior = getNewMark();

      // Marquage des brins "extérieurs" des objets épaissis
      // (1 brin sur 2, ces objets étant toujours orientables)
      for (; it.cont(); ++it)
	if (!isMarked(*it, treated) && isMarked(*it, AMarkNumber))
	  {
	    if (!isMarked(DUP1(*it), treated))
	      {
		halfMarkOrbit(DUP1(*it), ORBIT_CC, exterior);
		markOrbit(DUP1(*it), ORBIT_CC, treated);
		markCopy(exterior, 2);
	      }

	    markOrbit(DUP2(*it), ORBIT_CC, treated);
	    markOrbit(     *it , ORBIT_CC, treated);
	    setMark(*it, treated);
	  }

      negateMaskMark(treated);

      // 1) Première passe:
      for (it.reinit(); it.cont(); ++it)
	if (isMarked(*it, AMarkNumber) && isMarked(DUP1(*it), exterior) &&
	    !isMarked(DUP1(*it), treated))
	  {
	    * findVertex(DUP1(*it)) +=
	      AExtrusionCoef * regionNormalVector(*it, 0);

	    markOrbit(DUP1(*it), ORBIT_VERTEX, treated);
	  }

      // 2) Deuxième passe:
      for (it.reinit(); it.cont(); ++it)
	if (isMarked(*it, AMarkNumber) && isMarked(DUP1(*it), exterior) &&
	    !isMarked(DUP2(*it), treated))
	  {
	    * findVertex(DUP2(*it)) -=
	      AExtrusionCoef * regionNormalVector(*it, 0);

	    markOrbit(DUP2(*it), ORBIT_VERTEX, treated);
	  }

      // 3) Traitement des brins isolés:
      for (it.reinit(); it.cont(); ++it)
	if (isMarked(*it, AMarkNumber) && !isMarked(DUP1(*it), exterior) &&
	    !isMarked(DUP1(*it), treated))
	  {
	    * findVertex(DUP1(*it)) +=
	      AExtrusionCoef * regionNormalVector(*it, 0);

	    markOrbit(DUP1(*it), ORBIT_VERTEX, treated);
	  }

      for (it.reinit(); it.cont(); ++it)
	if (isMarked(*it, AMarkNumber) && !isMarked(DUP2(*it), exterior) &&
	    !isMarked(DUP2(*it), treated))
	  {
	    * findVertex(DUP2(*it)) -=
	      AExtrusionCoef * regionNormalVector(*it, 0);

	    markOrbit(DUP2(*it), ORBIT_VERTEX, treated);
	  }

      unmarkAll(exterior); freeMark(exterior);
      unmarkAll(treated ); freeMark(treated );
    }

  freeDirectInfo(directInfoIndex1);
  freeDirectInfo(directInfoIndex2);

  return nbThickened;
}
//******************************************************************************
#undef DUP1
#undef DUP2
//******************************************************************************
