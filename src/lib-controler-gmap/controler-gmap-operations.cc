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
#include "operations.hh"
#include "view-precompile.hh"

#include <cassert>
using namespace std;
using namespace GMap3d;
//******************************************************************************
bool CControlerGMap::canApplyOperation(const COperation& AOperation)
{
  bool res =
    !(FCurrentMode == MODE_TRANSLATION ||
      FCurrentMode == MODE_ROTATION ||
      FCurrentMode == MODE_SCALE);

  // Pas d'undo-redo en mode arrondi :
  if (FCurrentMode == MODE_ROUNDING &&
      AOperation.getType()==OPERATION_UNDO_REDO)
    res = false;

  if (!res)
    setMessage("Opération possible uniquement en mode sélection");

  FCurrentOperation = AOperation;

  return res;

  // Autre manière de gérer le canApplyOperation
  // setMode(MODE_SELECTION); return true;
}
//******************************************************************************
bool CControlerGMap::applyOperation(const COperation& AOperation)
{
  switch (AOperation.getType())
    {
    case OPERATION_CREATE:
      {
	switch (AOperation.getSubType())
	  {
	  case SUB_OPERATION_CREATE_POLYLINE: return createPolyline (); break;
	  case SUB_OPERATION_CREATE_POLYGON : return createPolygon  (); break;
	  case SUB_OPERATION_CREATE_MESH    : return createMesh     (); break;
	  case SUB_OPERATION_CREATE_SPHERE  : return createSphere   (); break;
	  case SUB_OPERATION_CREATE_CYLINDER: return createCylinder (); break;
	  case SUB_OPERATION_CREATE_PYRAMID : return createPyramid  (); break;
	  case SUB_OPERATION_CREATE_TORUS   : return createTorus    (); break;
	  default: return false;
	  }
	break;
      }
    case OPERATION_SEW:
      {
	switch (AOperation.getSubType())
	  {
	  case SUB_OPERATION_INTUITIVE_TOPO: return intuitiveTopoSew(); break;
	  case SUB_OPERATION_INTUITIVE_GEO : return intuitiveGeoSew (); break;
	  case SUB_OPERATION_TOPO          :
	    return topoSew(AOperation.getDimension()); break;
	  case SUB_OPERATION_GEO           :
	    return geoSew(AOperation.getDimension()); break;
	  case SUB_OPERATION_TOPO_BORDERS  :
	    return topoSewBorders(AOperation.getDimension()); break;
	  case SUB_OPERATION_GEO_BORDERS   :
	    return geoSewBorders(AOperation.getDimension()); break;
	  default: return false;
	  }
	break;
      }
    case OPERATION_PLATE:
      {
	switch (AOperation.getSubType())
	  {
	  case SUB_OPERATION_INTUITIVE:
	    return intuitivePlate(); break;
	  case SUB_OPERATION_BORDERS:
	    return plateBorders(AOperation.getDimension()); break;
	  default:
	    return plate(AOperation.getDimension()); break;
	  }
	break;
      }
    default: return false;
    }

  return false;
}
//******************************************************************************
int CControlerGMap::getNbDarts() const
{ return FNbDarts; }
int CControlerGMap::getNbSelectedDarts() const
{ return FNbSelectedDarts; }
int CControlerGMap::getNbVertices() const
{ return FNbVertices; }
//******************************************************************************