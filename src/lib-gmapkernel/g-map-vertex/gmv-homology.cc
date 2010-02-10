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
int CGMapVertex::computeIncidenceNumber(CDart* ADart, int ADim, CDart* ADart2)
{
  assert((0 <= ADim) && (ADim < 3));

  // 1) marquer m1 la i cell orientée incidence à ADart
  // 2) marquer m2 la i+1 cell orientée incidence à ADart2
  
  // 2) Parcourir a01,...,a0i incidente à d2


  // si brin non traité
  //    si brin marqué m1 alors +1
  //    sinon -1
  //    marquer traité orbite a0,...,i-1
  
  int incidenceNumber=0;
  int i=0;

  int m1 = getNewMark();   // A priori inutile car dans le parcours on parcours que des brins marqué
  halfMarkOrbit(ADart2, ORBIT_INF[ADim+1], m1);
  //  halfMarkOrbit(ADart, ORBIT_CELL[ADim], m1);
  

  int m2 = getNewMark();
  markOrbit(ADart2, ORBIT_CELL[ADim+1], m2);

  int m1oriented = getNewMark();
  halfMarkOrbit(ADart, ORBIT_INF[ADim], m1oriented);

  int treated = getNewMark();
  
  CCoverage * cov = getDynamicCoverage(ADart, ORBIT_CELL[ADim]);
  for (; cov->cont(); ++(*cov))
  {
    if ( !isMarked(**cov,treated) ) //&& isMarked(**cov,m1)/* && isMarked(**cov,m2)*/ )
    {
      if ( isMarked(**cov,m1oriented)==isMarked(**cov,m1) ) ++incidenceNumber;
      else --incidenceNumber;

      markOrbit(**cov,ORBIT_INF[ADim],treated);
    }
  }
  
  for (cov->reinit(); cov->cont(); ++(*cov))
  {
    if ( isMarked(**cov,treated) )
    {
      unmarkOrbit(**cov,ORBIT_INF[ADim],treated);
    }
  }
  delete cov;

  halfUnmarkOrbit(ADart2, ORBIT_INF[ADim+1], m1);
  //halfUnmarkOrbit(ADart, ORBIT_CELL[ADim], m1);
  
  unmarkOrbit(ADart2, ORBIT_CELL[ADim+1], m2);
  halfUnmarkOrbit(ADart, ORBIT_CELL[ADim], m1oriented);
  
  assert(isWholeMapUnmarked(treated));
  assert(isWholeMapUnmarked(m1));
  assert(isWholeMapUnmarked(m2));
  assert(isWholeMapUnmarked(m1oriented));
  
  // Libérations:
  freeMark(treated);
  freeMark(m1);
  freeMark(m2);
  freeMark(m1oriented);

  return incidenceNumber;
}
//******************************************************************************
