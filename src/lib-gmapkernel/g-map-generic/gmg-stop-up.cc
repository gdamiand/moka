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
void CGMapGeneric::stopUp(CDart * ADart, int ADim)
{
  int treated= getNewMark();

  CStaticBorderCoverage cov(this, ADart, ADim);

  for (; cov.cont(); ++cov)
    {
      CDart * n = addMapDart();
      linkAlpha(*cov,n,ADim);

      for (int i=0; i<=3; ++i)
        if ((i<ADim-1 || i>ADim+1) && isMarked(alpha(*cov,i), treated))
	  linkAlpha(n, alpha(alpha(*cov,i),ADim), i);

      if (ADim==0)
	linkAlpha0(n, *cov);
      else
	{
	  CDart * tmp = alpha(*cov, ADim-1);
	
	  while (!isMarked(tmp,treated) &&
		 !isFree(tmp,ADim) &&
		 !isFree(alpha(tmp,ADim),ADim-1) &&
		 tmp!=alpha(*cov,ADim))
	    tmp = alpha(alpha(tmp,ADim),ADim-1);

	  if (isMarked(tmp,treated))
	    linkAlpha(n,alpha(tmp,ADim),ADim-1);
	}

      setMark(*cov,treated);
    }

  for (cov.reinit(); cov.cont(); ++cov)
    unsetMark(*cov,treated);

  freeMark(treated);
}
//******************************************************************************
int CGMapGeneric::stopUpMarkedBorders(int AMarkNumber, int ADim)
{
  assert(ADim>=0 && ADim<=3);
  int nbStoppedUp = 0;
  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (isMarked(*it,AMarkNumber) && getBorderDimension(*it)==ADim)
      {
	stopUp(*it,ADim);
	++nbStoppedUp;
      }

  return nbStoppedUp;
}
//******************************************************************************
int CGMapGeneric::intuitiveStopUpMarkedBorders(int AMarkNumber)
{
  CDynamicCoverageAll it(this);
  int nbStoppedUp = 0;
  int treated = getNewMark();
  int memo = getNewMark();

  for (int dim=0; dim<=3; ++dim)
    {
      for (it.reinit(); it.cont(); ++it)
	if (isMarked(*it,AMarkNumber) &&
	    !isMarked(*it, treated) && isFree(*it, dim))
	  {
	    markOrbit(*it, ORBIT_INF [dim<3 ? dim+1 : 3], memo);
	    markOrbit(*it, ORBIT_BORDER[dim], treated);
	    stopUp(*it,dim);
	    ++nbStoppedUp;
	  }

      markMove(memo, treated);
    }

  freeMark(memo);
  unmarkAll(treated);
  freeMark(treated);

  return nbStoppedUp;
}
//******************************************************************************
