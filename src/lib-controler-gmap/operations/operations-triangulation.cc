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
#ifdef MODULE_TRIANGULATION

#include "g-map-vertex.hh"
#include "controler-gmap.hh"
#include "operations.hh"
#include "parameter-selection.hh"
#include "view-precompile.hh"

#include "triangulation-api.hh"
#include "triangulation-tools.hh"

using namespace GMap3d;

//******************************************************************************

bool CControlerGMap::triangulateTopoFaces()
{
  if (canApplyOperation(COperation(OPERATION_TRIANGULATION,
				   SUB_OPERATION_TRIANGULATION_TOPO_FACES, -1))) {
    CTriangulationAPI api(FMap);
    
    undoRedoPreSave();
    api.triangulateMarkedFaces(getSelectionMark(), true);
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Faces triangulated");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::markNonUniformFaces()
{
  if (canApplyOperation(COperation(OPERATION_TRIANGULATION,
				   SUB_OPERATION_TRIANGULATION_MARK_NON_UNIFORM_FACES,
				   -1))) {
    CTriangulationAPI api(FMap);
    CDart *d;
    
    if (FMap->getMarkedCells(ORBIT_CC, getSelectionMark(), NULL, &d) != 1) {
      setMessage("Selected mesh not correct");
      return false;
    }

    FMap->unmarkOrbit(d, ORBIT_CC, getSelectionMark());
    api.markNonUniformFaces(d, getSelectionMark(), 50);
    
    setMessage("Non uniform faces marked");
    setSelectionChanged();
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::optimizeSelectedEdges()
{
  if (canApplyOperation(COperation(OPERATION_TRIANGULATION,
				   SUB_OPERATION_TRIANGULATION_OPTIMIZE, -1))) {
    CTriangulationAPI api(FMap);
    
    undoRedoPreSave();
    api.optimizeMarkedEdges(getSelectionMark());
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Edges positions optimized");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::createDelaunayTriangulation()
{
  if (canApplyOperation(COperation(OPERATION_TRIANGULATION,
				   SUB_OPERATION_TRIANGULATION_DELAUNAY, -1))) {
    CTriangulationTools tools(FMap);
    
    undoRedoPreSave();
    tools.createDelaunaySurface(getSelectionMark());
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Delaunay triangulation done");
    return true;
  }
  
  return false;
}

//******************************************************************************

bool CControlerGMap::createOptimizedDelaunayTriangulation()
{
  if (canApplyOperation(COperation(OPERATION_TRIANGULATION,
				   SUB_OPERATION_TRIANGULATION_OPTIMIZED_DELAUNAY, -1))) {
    CTriangulationTools tools(FMap);
    
    undoRedoPreSave();
    tools.createDelaunaySurface(getSelectionMark(), 2);
    undoRedoPostSaveOk();
    
    setModelChanged();
    setMessage("Delaunay triangulation done");
    return true;
  }
  
  return false;
}

//******************************************************************************

#endif // MODULE_TRIANGULATION
