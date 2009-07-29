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
int CGMapGeneric::degree(CDart * ADart, int ACellDim, int ABoundingCell)
{
  assert(ADart!=NULL);
  assert(0<=ACellDim && ACellDim<3);
  assert(0<ABoundingCell && ABoundingCell<=4);
  assert(ACellDim<ABoundingCell);

  TOrbit countOrbit =
    AND_ORBIT(ORBIT_CELL[ACellDim], ORBIT_CELL[ACellDim+1]);

  TOrbit boundingOrbit =
    AND_ORBIT(ORBIT_CELL[ACellDim], ORBIT_CELL[ABoundingCell]);

  countOrbit = AND_ORBIT(countOrbit, ORBIT_CELL[ABoundingCell]);

  int nbIncident = 0;
  int treated = getNewMark();

  CCoverage * cov = getDynamicCoverage(ADart, boundingOrbit);
  for (; cov->cont(); ++(*cov))
    if (!isMarked(**cov, treated))
      {
	++nbIncident;
	markOrbit(**cov, countOrbit, treated);
      }
  delete cov;

  unmarkOrbit(ADart, boundingOrbit, treated);
  freeMark(treated);

  assert(nbIncident!=0);

  return nbIncident;
}
//******************************************************************************
bool CGMapGeneric::isLocalDegreeTwoSup(CDart * ADart, int ACellDim)
{
  assert(ADart!=NULL);
  assert(0<=ACellDim && ACellDim<=3);
  
  if ( ACellDim==2 ) return true;
  if ( ACellDim==3 ) return false;

  // Optimisation possible surement pour l'arete mais pas pour le sommet ????
  //  TOrbit halfCell = (ACellDim == 1 ? ORBIT_23 : ORBIT_13);
  CCoverage* cov = getDynamicCoverage(ADart, ORBIT_CELL[ACellDim]);
  bool res = true;
  
  while (res && cov->cont())
    {
      if ( alpha(alpha(**cov, ACellDim+1), ACellDim+2)!=
	   alpha(alpha(**cov, ACellDim+2), ACellDim+1) )
	res = false;

      ++(*cov);
    }

  delete cov;
  
  return res;
}
//******************************************************************************
bool CGMapGeneric::isLocalDegreeTwoInf(CDart * ADart, int ACellDim)
{
  assert(ADart!=NULL);
  assert(0<=ACellDim && ACellDim<=3);
  
  if ( ACellDim==1 ) return true;
  if ( ACellDim==0 ) return false;

  CCoverage* cov = getDynamicCoverage(ADart, ORBIT_CELL[ACellDim]);
  bool res = true;
  
  while (res && cov->cont())
    {
      if ( alpha(alpha(**cov, ACellDim-1), ACellDim-2)!=
	   alpha(alpha(**cov, ACellDim-2), ACellDim-1) )
	res = false;

      ++(*cov);
    }

  delete cov;
  
  return res;
  
}
//******************************************************************************
