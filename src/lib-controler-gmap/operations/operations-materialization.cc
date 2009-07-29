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
bool CControlerGMap::materializeBarycenter()
{
  if (canApplyOperation(COperation(OPERATION_MATERIALIZE,
				   SUB_OPERATION_MATERIALIZE_BARYCENTER, -1)))
    {
      undoRedoPreSave();
      FMap->materializeBarycenter(getSelectionMark());
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Barycentre matérialisé");
      return true;
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::materializeAxe()
{
  if (canApplyOperation(COperation(OPERATION_MATERIALIZE,
				   SUB_OPERATION_MATERIALIZE_AXE, -1)))
    {
      CDart *d1, *d2;
      
      if (FMap->getMarkedCells(ORBIT_VERTEX, getSelectionMark(),
			       getLastSelectedDart(), &d2,&d1) != 2)
	{
	  setMessage("Sélection incohérente");
	  return false;
	}

      undoRedoPreSave();
      CDart * d= FMap->materializeAxe(d1,d2);
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();
      FMap->markOrbit(d,ORBIT_EDGE, getSelectionMark());
      selectDart(d);

      setModelChanged();
      setMessage("Axe matérialisé");
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::materializePlane()
{
  if (canApplyOperation(COperation(OPERATION_MATERIALIZE,
				   SUB_OPERATION_MATERIALIZE_PLANE, -1)))
    {
      CDart *d1, *d2, *d3;
      
      if (FMap->getMarkedCells(ORBIT_VERTEX, getSelectionMark(),
			       getLastSelectedDart(), &d3,&d2,&d1) != 3)
	{
	  setMessage("Sélection incohérente");
	  return false;
	}
      
      undoRedoPreSave();
      CDart * d= FMap->materializePlane(d1,d2,d3);
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();
      FMap->markOrbit(d,ORBIT_FACE, getSelectionMark());
      selectDart(d);

      setModelChanged();
      setMessage("Plan matérialisé");
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::materializeNormalVector()
{
  if (canApplyOperation(COperation(OPERATION_MATERIALIZE,
				   SUB_OPERATION_MATERIALIZE_NORMAL_VECTOR, -1)))
    {
      CDart * last = getLastSelectedDart();
      
      if (last==NULL ||
	  FMap->getMarkedCells(ORBIT_FACE, getSelectionMark()) != 1 ||
	  FMap->getNbPolylineVertices(last) < 3)
	{
	  setMessage("Sélection incohérente");
	  return false;
	}
      
      undoRedoPreSave();
      CDart * d= FMap->materializeNormalVector(last);
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();
      FMap->markOrbit(d,ORBIT_EDGE, getSelectionMark());
      selectDart(d);

      setModelChanged();
      setMessage("Vecteur normal matérialisé");
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::materializeOrthoPlane()
{
  if (canApplyOperation(COperation(OPERATION_MATERIALIZE,
				   SUB_OPERATION_MATERIALIZE_ORTHO_PLANE, -1)))
    {
      CDart *d1, *d2;
      
      CDart * last = getLastSelectedDart();
      
      if (last==NULL ||
	  FMap->getMarkedCells(ORBIT_VERTEX, getSelectionMark(),
			       last, &d2, &d1) != 2)
	{
	  setMessage("Sélection incohérente");
	  return false;
	}

      undoRedoPreSave();
      CDart * d= FMap->materializeNormalPlane(d1,d2);
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();
      FMap->markOrbit(d,ORBIT_FACE, getSelectionMark());
      selectDart(d);

      setModelChanged();
      setMessage("Plan normal matérialisé");
      return true;
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::materializeReferential()
{
  if (canApplyOperation(COperation(OPERATION_MATERIALIZE,
				   SUB_OPERATION_MATERIALIZE_REFERENTIAL, -1)))
    {
      undoRedoPreSave();
      FMap->materializeReferential();
      undoRedoPostSaveOk();

      setModelChanged();
      setMessage("Référentiel matérialisé");
      return false;
    }
  return false;
}
//******************************************************************************
