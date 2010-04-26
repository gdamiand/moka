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
bool CControlerGMap::
isNonModifyingOperation(const COperation& AOperation) const
{
  return
    AOperation.getType()==OPERATION_SELECT ||
    AOperation.getType()==OPERATION_DEFORM ||
    AOperation.getType()==OPERATION_EXPORT ||
    AOperation.getType()==OPERATION_FIND_MOTIF_OFF ||
    AOperation.getType()==OPERATION_FIND_MOTIF_MOKA;
}
//------------------------------------------------------------------------------
bool CControlerGMap::
isRemovalOperation(const COperation& AOperation) const
{
  return
    AOperation.getType()==OPERATION_MERGE ||
    AOperation.getType()==OPERATION_MERGE_COLINEAR ||
    AOperation.getType()==OPERATION_MERGE_FACE_NODISCONNECTION ||
    AOperation.getType()==OPERATION_REMOVE_FACES_KEEP_BALLS ||
    AOperation.getType()==OPERATION_SHIFT_EDGES_INCIDENT_TO_VERTEX ||
    AOperation.getType()==OPERATION_REMOVE_DANGLING_EDGES ||
    AOperation.getType()==OPERATION_SIMPLIFY_3D_OBJECT;
}
//------------------------------------------------------------------------------
bool CControlerGMap::
isContractionOperation(const COperation& AOperation) const
{
  return
    AOperation.getType()==OPERATION_CONTRACT ||
    AOperation.getType()==OPERATION_DEL_FLAT_FACES ||
    AOperation.getType()==OPERATION_DEL_FLAT_VOLUMES ||
    AOperation.getType()==OPERATION_DEL_NULL_EDGE;
}
//******************************************************************************
bool CControlerGMap::canApplyOperation(const COperation& AOperation)
{
  // Pas d'opération en mode translation, rotation ou scale.
  if (FCurrentMode == MODE_TRANSLATION ||
      FCurrentMode == MODE_ROTATION ||
      FCurrentMode == MODE_SCALE)
    {
      setMessage("Operation possible only in selection mode");
      return false;
    }

  // Pas d'undo-redo en mode arrondi :
  if (FCurrentMode == MODE_ROUNDING &&
      AOperation.getType()==OPERATION_UNDO_REDO)
    {
      setMessage("Undo/redo not possible in rounding mode");
      return false;
    }

  if (isModelBlocked()) return true;  

  // In simplification mode, we can only apply non modifying operations,
  // and removal operations (except the intuitive one which is not possible
  // since we must know the dimension of removed cells)
  if (/*isModelBlocked() ||*/ isModeSimplification())
    {
      if ( (!isNonModifyingOperation(AOperation) &&
	    !isRemovalOperation(AOperation)) ||
	   (AOperation.getSubType()==SUB_OPERATION_INTUITIVE_TOPO) ) 
	{
	  setMessage("Operation not possible in bloc of simplification mode");
	  return false;
	}
    }
  
  FCurrentOperation = AOperation;

  return true;

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
