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
CDart * CGMapGeneric::triangulateEdge(CDart * ADart)
{
  assert(ADart!=NULL);
  insertVertex(ADart);
  return alpha0(ADart);
}
//******************************************************************************
CDart * CGMapGeneric::triangulateFace(CDart * ADart)
{
  assert(ADart!=NULL);

  int treated = getNewMark();

  CStaticCoverage01 it(this, ADart);

  for (; it.cont(); setMark(*it, treated), ++it)
    {
      CDart * n1 = addMapDart();
      CDart * n2 = addMapDart(); linkAlpha0(n1,n2);

      CDart * nn1 = NULL, * nn2 = NULL, * d3;

      if (isFree3(*it))
	d3=NULL;
      else
        {
	  d3=alpha3(*it);
	  nn1=addMapDart();
	  nn2=addMapDart(); linkAlpha0(nn1,nn2);
	  linkAlpha3(n1,nn1);
	  linkAlpha3(n2,nn2);
	}

      if (!isFree0(*it) && isMarked(alpha0(*it), treated))
        {
          linkAlpha1(n2,alpha010(*it));
          if (d3!=NULL) linkAlpha1(nn2,alpha010(d3));
        }

      if (!isFree1(*it) && isMarked(alpha1(*it), treated))
        {
          topoSew2(n1,alpha1(alpha1(*it)));
          if (d3!=NULL) topoSew2(nn1,alpha1(alpha1(d3)));
        }

      linkAlpha1(*it,n1);
      if (d3!=NULL) linkAlpha1(d3,nn1);
    }

  for (it.reinit(); it.cont(); ++it)
    unsetMark(*it, treated);

  freeMark(treated);

  return alpha10(ADart);
}
//******************************************************************************
CDart * CGMapGeneric::triangulateVolume(CDart * ADart)
{
  assert(ADart!=NULL);

  int treated = getNewMark();

  CStaticCoverage012 it(this, ADart);

  for (; it.cont(); setMark(*it, treated), ++it)
    {
      CDart * n1= addMapDart();
      CDart * n2= addMapDart();
      CDart * d2= addMapDart();

      linkAlpha0(n1,n2);
      linkAlpha1(d2,n1);

      if (!isFree0(*it) && isMarked(alpha0(*it),treated))
        {
          linkAlpha0(d2,alpha02  (*it));
          linkAlpha1(n2,alpha0210(*it));
        }

      if (!isFree1(*it) && isMarked(alpha1(*it),treated))
        {
          linkAlpha2(n1,alpha121 (*it));
          linkAlpha2(n2,alpha1210(*it));
        }

      if (!isFree2(*it) && isMarked(alpha2(*it),treated))
        {
          linkAlpha3(n1,alpha21 (alpha2(*it)));
	  linkAlpha3(n2,alpha210(alpha2(*it)));
          linkAlpha3(d2,alpha2  (alpha2(*it)));
        }

      linkAlpha2(*it,d2);
    }

  for (it.reinit(); it.cont(); ++it)
    unsetMark(*it, treated);

  freeMark(treated);

  return alpha210(ADart);
}
//******************************************************************************
CDart * CGMapGeneric::triangulate(CDart * ADart, int ADim)
{
  assert(ADart!=NULL);
  assert(ADim>0 && ADim<=3);

  switch (ADim)
    {
    case 1: return triangulateEdge(ADart);
    case 2: return triangulateFace(ADart);
    case 3: return triangulateVolume(ADart);
    }

  return NULL; // pour éviter un warning à la compilation...
}
//******************************************************************************
int CGMapGeneric::triangulateMarkedCells(int AMarkNumber, int ADim)
{
  assert(ADim>=1 && ADim<=3);

  int nbTriangulated = 0;
  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (isMarked(*it, AMarkNumber) && !isMarked(*it, treated))
      {
	markOrbit(*it, ORBIT_CELL[ADim], treated);
	triangulate(*it, ADim);
	++nbTriangulated;
      }

  unmarkAll(treated);
  freeMark(treated);

  return nbTriangulated;
}
//******************************************************************************
