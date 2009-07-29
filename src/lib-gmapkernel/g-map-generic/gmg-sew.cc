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
int CGMapGeneric::getSewDimension(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1!=NULL && ADart2!=NULL);

  for (int dim=0; dim<=3; ++dim)
    if (canSew(ADart1, ADart2, dim) &&
	!isSameOrbit(ADart1, ADart2, ORBIT_INF[dim]))
      return dim;

  return 4;
}
//******************************************************************************
bool CGMapGeneric::intuitiveSew(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1!=NULL && ADart2!=NULL);

  int dim = getSewDimension(ADart1, ADart2);

  if (dim!=4)
    sew(ADart1, ADart2, dim);

  return dim!=4;
}
//******************************************************************************
int CGMapGeneric::multiSew(CDart * ADart1, int AMarkNumber1,
			   CDart * ADart2, int ADimension)
{
  assert(ADart1 != NULL && ADart2 != NULL);
  assert(2 <= ADimension && ADimension <= 3);

  if (!isFree(ADart1, ADimension) || !isFree(ADart2, ADimension))
    return 0;

  int nbSewed = 0;
  int treated = getNewMark();

  markCopy(AMarkNumber1, treated);
  negateMaskMark(treated);

  CCoverage * cov1 =
    getBasicDynamicCoverage(ADart1, ORBIT_BORDER[ADimension], treated);

  for (; cov1->cont(); ++(*cov1))
    {

    }

  delete cov1;

  unmarkAll(treated);
  freeMark(treated);

  return nbSewed;
}
//******************************************************************************
