/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
#include "controler-gmap.hh"
#include <cassert>

using namespace GMap3d;
//******************************************************************************
bool CControlerGMap::lookAtLast(TViewId AView)
{
  CDart *last=getLastSelectedDart();
  if (last!=NULL)
    {
      CVertex* v = FMap->findVertex(last);
      getParameterAimedPosition(AView)->setLookAt(0, v->getX());
      getParameterAimedPosition(AView)->setLookAt(1, v->getY());
      getParameterAimedPosition(AView)->setLookAt(2, v->getZ());
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::lookAtFaceBary(TViewId AView)
{
  CDart *last=getLastSelectedDart();
  if (last!=NULL)
    {
      CVertex v = FMap->centerOfBoundingBox(last,ORBIT_01);
      getParameterAimedPosition(AView)->setLookAt(0,v.getX());
      getParameterAimedPosition(AView)->setLookAt(1,v.getY());
      getParameterAimedPosition(AView)->setLookAt(2,v.getZ());
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::lookAtVolumeBary(TViewId AView)
{
  CDart *last=getLastSelectedDart();
  if (last!=NULL)
    {
      CVertex v = FMap->centerOfBoundingBox(last,ORBIT_012);
      getParameterAimedPosition(AView)->setLookAt(0,v.getX());
      getParameterAimedPosition(AView)->setLookAt(1,v.getY());
      getParameterAimedPosition(AView)->setLookAt(2,v.getZ());
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::lookAtCcBary(TViewId AView)
{
  CDart *last=getLastSelectedDart();
  if (last!=NULL)
    {
      CVertex v = FMap->centerOfBoundingBox(last,ORBIT_0123);
      getParameterAimedPosition(AView)->setLookAt(0,v.getX());
      getParameterAimedPosition(AView)->setLookAt(1,v.getY());
      getParameterAimedPosition(AView)->setLookAt(2,v.getZ());
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::lookAtSelectedBary(TViewId AView)
{
  CVertex v = FMap->centerOfBoundingBox(getSelectionMark());

  getParameterAimedPosition(AView)->setLookAt(0,v.getX());
  getParameterAimedPosition(AView)->setLookAt(1,v.getY());
  getParameterAimedPosition(AView)->setLookAt(2,v.getZ());

  return true;
}
//******************************************************************************
bool CControlerGMap::lookAtAllBary(TViewId AView)
{
  int mark = FMap->getNewMark();

  FMap->negateMaskMark(mark);

  CVertex v = FMap->centerOfBoundingBox(mark);

  FMap->negateMaskMark(mark);

  FMap->freeMark(mark);

  getParameterAimedPosition(AView)->setLookAt(0,v.getX());
  getParameterAimedPosition(AView)->setLookAt(1,v.getY());
  getParameterAimedPosition(AView)->setLookAt(2,v.getZ());

  return true;
}
//******************************************************************************
