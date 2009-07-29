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
#include "g-map-generic.hh"
using namespace GMap3d;
//******************************************************************************
int CGMapGeneric::unsewMarkedCells(int AMarkNumber, int ADim)
{
  CDynamicCoverageAll it(this);
  int nbUnsews = 0;

  for (; it.cont(); ++it)
    if (!isFree(*it,ADim) && isMarked(*it, AMarkNumber))
      {
	unsew(*it, ADim);
	++nbUnsews;
      }

  return nbUnsews;
}
//******************************************************************************
int CGMapGeneric::intuitiveUnsewMarkedCells(int AMarkNumber)
{
  CDynamicCoverageAll it(this);
  int nbUnsews = 0;
  int treated = getNewMark();
  int memo = getNewMark();

  for (int dim=3; dim>=0; --dim)
    {
      for (it.reinit(); it.cont(); ++it)
	if (!isMarked(*it, treated))
	  {
	    // Pour des raisons de complexité (calcul de isWholeCellMarked), on
	    // traite toutes les cellules de dimension dim une par une:

	    bool cellUnsewed = false;
	
	    CCoverage * cov = getDynamicCoverage(*it, ORBIT_CELL[dim]);

	    for (; cov->cont(); ++(*cov))
	      if (!isMarked(**cov, treated))
		{
		  if (!isMarked(**cov, AMarkNumber) || isFree(**cov, dim))
		    setMark(**cov, treated);
		  else
		    {
		      markOrbit(**cov, ORBIT_SEWS[dim], treated);
		      markOrbit(**cov, ORBIT_SEWS[dim], memo);
		      unsew(**cov, dim);
		      cellUnsewed = true;
		      ++nbUnsews;
		    }
		}
	
	    delete cov;

	    if (cellUnsewed &&
		isWholeCellMarked(*it, ORBIT_CELL[dim], AMarkNumber))
	      markOrbit(*it, ORBIT_CELL[dim], memo);
	  }

      markCopy(memo, treated);
    }

  unmarkAll(memo);
  freeMark(memo);
  unmarkAll(treated);
  freeMark(treated);

  return nbUnsews;
}
//******************************************************************************
