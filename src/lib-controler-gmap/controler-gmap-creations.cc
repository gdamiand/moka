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
#include "controler-gmap.hh"
#include "view-precompile.hh"
#include "parameter-creation.hh"
#include "parameter-object-position.hh"
#include "parameter-polyline-position.hh"
#include "transformation-matrix.hh"
#include "g-map-vertex.hh"
#include "vertex.hh"
#include "transformation-matrix.hh"
using namespace GMap3d;
//******************************************************************************
void CControlerGMap::modeCreationBegin()
{
  setMessage("Mode création");

  for (unsigned int i=0; i<FViews.size(); ++i)
    viewEnablePreviewPrecompile(i);
}
//******************************************************************************
void CControlerGMap::modeCreationEnd()
{
  for (unsigned int i=0; i<FViews.size(); ++i)
    viewDisablePreviewPrecompile(i);
}
//******************************************************************************
void CControlerGMap::modeCreationPolylineOperationStart()
{
  float last3D[3];
  FViews[FCurrentViewId]->unproject(FLastX, FLastY, last3D);
  FParameterPolylinePosition->addVertex(CVertex(last3D[0],
						last3D[1],
						last3D[2]));
}
//******************************************************************************
void CControlerGMap::modeCreationOperationStart()
{
  FViews[FCurrentViewId]->unproject(FFirstX, FFirstY, FFirst3D);

  float x = FFirst3D[0];
  float y = FFirst3D[1];
  float z = FFirst3D[2];

  CParameterObjectPosition * param = FParameterObjectPosition[FCreationType];

  param->setScale(0.0);
  param->setCenter(x,y,z);
  param->setNormalVector(FViews[FCurrentViewId]->getEyeDirection());
  param->setFirstVertex(x,y,z, true);
}
//******************************************************************************
void CControlerGMap::modeCreationPolylineOperationMove()
{
  float last3D[3];
  FViews[FCurrentViewId]->unproject(FLastX, FLastY, last3D);
  FParameterPolylinePosition->setLastVertex(CVertex(last3D[0],
						    last3D[1],
						    last3D[2]));
}
//******************************************************************************
void CControlerGMap::modeCreationOperationMove()
{
  float last3D[3];

  FViews[FCurrentViewId]->unproject(FLastX, FLastY, last3D);

  float x = last3D[0];
  float y = last3D[1];
  float z = last3D[2];

  FParameterObjectPosition[FCreationType]->setFirstVertex(x,y,z, true);
}
//******************************************************************************
void CControlerGMap::modeCreationPolylineOperationStop()
{
}
//******************************************************************************
void CControlerGMap::modeCreationOperationStop()
{
}
//******************************************************************************
void CControlerGMap::modeCreationReinitPosition()
{
  switch (getMode())
    {
    case MODE_CREATION_POLYLINE:
      FParameterPolylinePosition->reinit(); break;
    case MODE_CREATION_POLYGON:
    case MODE_CREATION_MESH:
    case MODE_CREATION_SPHERE:
    case MODE_CREATION_CYLINDER:
    case MODE_CREATION_PYRAMID:
    case MODE_CREATION_TORUS:
      FParameterObjectPosition[FCreationType]->reinit(); break;
    }
}
//******************************************************************************
