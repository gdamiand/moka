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
CDart * CGMapVertex::prolongPolyline(CDart * ADart, const CVertex & AVertex)
{
  CDart * dart = CGMapGeneric::prolongPolyline(ADart);
  setVertex(dart, AVertex);
  return dart;
}
//******************************************************************************
bool CGMapVertex::isPlanarPolyline(CDart * ADart)
{
  assert(ADart!=NULL);

  CVertex s1, v1, vect;
  int treated = getNewMark();
  bool ok = true;

  CDynamicCoverage01 it(this, ADart);

  s1 = *findVertex(ADart);

  setMark(   *it , treated);
  setMark(alpha1(*it), treated);

  for (; it.cont() && ok; ++it)
    if (!isMarked(*it, treated))
    {
      if (v1.isNull())
	{
	  v1 = *findVertex(*it) - s1;

	  if (!v1.isNull())
	    v1 /= v1.norm();
	}
      else
	if (vect.isNull())
	  {
	    CVertex v2 = *findVertex(*it) - s1;

	    if (!v2.isNull())
	      v2 /= v2.norm();

	    vect = CGeometry::getNormalVector(v1,v2);
	  }
	else
	  {
	    CVertex v2 = *findVertex(*it) - s1;

	    if (!v2.isNull())
	      v2 /= v2.norm();

	    ok = CGeometry::areColinear(vect,
					CGeometry::getNormalVector(v1,v2));
	  }

      setMark(   *it , treated);
      setMark(alpha1(*it), treated);
    }

  unmarkOrbit(ADart, ORBIT_01, treated);
  freeMark(treated);

  return ok;
}
//******************************************************************************
bool CGMapVertex::markNonPlanarPolylines(int AMarkNumber)
{
  int treated = getNewMark();
  bool found = false;

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (!isPlanarPolyline(*it))
	  {
	    markOrbit(*it,ORBIT_FACE, AMarkNumber);
	    found = true;
	  }

	markOrbit(*it,ORBIT_FACE, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  return found;
}
//******************************************************************************
