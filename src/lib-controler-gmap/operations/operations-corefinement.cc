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
#ifdef MODULE_COREFINEMENT
//******************************************************************************
#include "g-map-vertex.hh"
#include "controler-gmap.hh"
#include <cassert>

#include "corefinement-api.hh"
#include "geometry.hh"

using namespace GMap3d;

//******************************************************************************
bool CControlerGMap::check2dCorefinability(CDart ** d1, CDart ** d2,
				       CVertex * normal) // :)
{
  assert(d1 != NULL);
  assert(d2 != NULL);
  assert(normal != NULL);

  if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, d1) != 1)
    {
      setMessage("Selection 1 not correct");
      return false;
    }

  if (!FMap->isIFreeOrbit(*d1, 3, ORBIT_CC))
    {
      setMessage("Mesh 1 invalid: all the darts must be 3-free");
      return false;
    }

  if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1), NULL, d2) != 1)
    {
      setMessage("Selection 2 not correct");
      return false;
    }

  if (!FMap->isIFreeOrbit(*d2, 3, ORBIT_CC))
    {
      setMessage("Mesh 2 invalid: all the darts must be 3-free");
      return false;
    }

  CVertex n1 = FMap->cellDimensionNormalVector(*d1, 3);
  CVertex n2 = FMap->cellDimensionNormalVector(*d2, 3);

  TCoordinate angle = CGeometry::getAngle(n1,n2);

  if (angle>90)
    angle = 180 - angle;

  if (fabs(angle) > 0.5)
    {
      setMessage("The two meshes are not parallel");
      return false;
    }

  CVertex bary1 = FMap->barycenter(*d1, ORBIT_CC);
  CVertex bary2 = FMap->barycenter(*d2, ORBIT_CC);

  if (!(bary1 - CGeometry::orthoProjectVertexOnPlane(bary1, bary2,n2)).isNull())
    {
      setMessage("The two meshes are not coplanar");
      return false;
    }

  if (angle > 90)
    n2 = - n2;

  * normal = n1 + n2;

  if (normal->isNull())
    * normal = OZ;
  else
    * normal /= normal->norm();

  return true;
}
//******************************************************************************
bool CControlerGMap::corefine2dSegmentsSweeping()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE2D_SWEEPING, -1)))
    {
      CDart * d1, * d2;
      CVertex normal;
      
      // À modifier:
      if (!check2dCorefinability(& d1, & d2, & normal))
	return false;
      
      undoRedoPreSave();
      CCorefinementAPI(FMap).corefine2dMeshesSweeping(d1,d2, normal);
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Co-refinement by sweeping line done");
      return true;
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::corefine2dSegmentsPropagation()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE2D_PROPAGATION, -1)))
    {
      CCorefinementAPI corefinator(FMap);
      CDart * d1, * d2;
      CVertex normal;

      if (!check2dCorefinability(& d1, & d2, & normal))
	return false;

      undoRedoPreSave();
      corefinator.corefine2dMeshesPropagation(d1, d2, normal);
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Co-refinement 2D by propagation done");
      return true;
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::booleanOperations2d()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE_BOOL_OPS2D, -1))) {
    CCorefinementAPI coref(FMap);
    CDart *d1, *d2;
    CVertex normal;
    
    if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d1) != 1) {
      setMessage("Selection 1 not correct");
      return false;
    }
    
    if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1),
			     NULL, &d2) != 1) {
      setMessage("Selection 2 not correct");
      return false;
    }

    if (!check2dCorefinability(& d1, & d2, & normal))
      return false;
    
    undoRedoPreSave();
    coref.booleanOperations2d(d1, d2, normal,
			      getSelectionMark(),
			      getNextSelectionMark(1),
			      getNextSelectionMark(2));
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("2D boolean operations done");
    return true;
  }
  
  return false;
}
//******************************************************************************
bool CControlerGMap::corefine3dFaces()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE3D, -1)))
    {
      CCorefinementAPI corefinator(FMap);
      CDart * d1, * d2;

      if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d1) != 1)
	{
	  setMessage("Selection 1 not correct");
	  return false;
	}

      if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1),
			       NULL, &d2) != 1)
	{
	  setMessage("Selection 2 not correct");
	  return false;
	}

      undoRedoPreSave();
      corefinator.corefine3dMeshes(d1, d2);
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Co-refinement 3D done");
      return true;
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::corefineMarked3dMeshesWith()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE3D_MARKED_MESHES, -1)))
    {
      CCorefinementAPI corefinator(FMap);
      CDart *d;

      if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1),
			       NULL, &d) != 1)
	{
	  setMessage("Selection 1 not correct");
	  return false;
	}

      undoRedoPreSave();
      corefinator.corefineMarked3dMeshesWith(getSelectionMark(), d);
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Co-refinement 3D done");
      return true;
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::booleanOperations3d()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE_BOOL_OPS3D, -1))) {
    CCorefinementAPI coref(FMap);
    CDart *d1, *d2;
    
    if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d1) != 1) {
      setMessage("Selection 1 not correct");
      return false;
    }
    
    if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1),
			     NULL, &d2) != 1) {
      setMessage("Selection 2 not correct");
      return false;
    }

    undoRedoPreSave();
    coref.booleanOperations3d(d1, d2,
			      getSelectionMark(),
			      getNextSelectionMark(1),
			      getNextSelectionMark(2));
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("3D boolean operations done");
    return true;
  }
  
  return false;
}
//******************************************************************************
bool CControlerGMap::corefine3dMeshes()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE3D, -1)))
    {
      CCorefinementAPI corefinator(FMap);
      CDart * d1, * d2;

      if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d1) != 1)
	{
	  setMessage("Selection 1 not correct");
	  return false;
	}

      if (FMap->getMarkedCells(ORBIT_CC, getNextSelectionMark(1),
			       NULL, &d2) != 1)
	{
	  setMessage("Selection 2 not correct");
	  return false;
	}

      undoRedoPreSave();
      corefinator.corefine3dMeshes(d1, d2);
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Co-refinement 3D done");
      return true;
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::corefineMarked3dFaces()
{
  if (canApplyOperation(COperation(OPERATION_COREFINE,
				   SUB_OPERATION_COREFINE3D_MARKED_FACES, -1)))
    {
      CCorefinementAPI corefinator(FMap);

      undoRedoPreSave();
      corefinator.corefineMarked3dFaces(getSelectionMark(), 
					getNextSelectionMark(1));
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Co-refinement 3D done");
      return true;
    }

  return false;
}
//******************************************************************************
#endif // COREFINEMENT
//******************************************************************************
