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
bool CControlerGMap::contextTriangulate()
{
  switch (getSelectionOrbit())
    {
    case ORBIT_EDGE   : return triangulate(1); break;
    case ORBIT_FACE   : return triangulate(2); break;
    case ORBIT_VOLUME : return triangulate(3); break;
    default: setMessage("Selected orbit not correct");
    }
  return false;
}
//******************************************************************************
bool CControlerGMap::triangulate(int ADimension)
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_TRIANGULATE, ADimension)))
    {
      undoRedoPreSave();
      
      int nb = FMap->triangulateMarkedCells(getSelectionMark(), ADimension);
      
      if (nb==0)
	{
	  setMessage("No ",ADimension,"-cell triangulated");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();

	  setModelChanged();
	  setMessage(nb, ADimension, nb==1 ?
		     "-cell triangulated" :
		     "-cells triangulated");
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::quadrangulateFaces()
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_QUADRANGULATE,
				   SUB_OPERATION_QUADRANGULATE_FACES, -1)))
    {
      undoRedoPreSave();
      
      int nb = FMap->quadrangulateMarkedFaces(getSelectionMark());
      
      if (nb==0)
	{
	  setMessage("No face quadrangulated");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();

	  setModelChanged();	  
	  setMessage(nb, nb==1 ? " face quadrangulated" :
		     " faces quadrangulated");
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::basicQuadrangulateFaces()
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_QUADRANGULATE,
				   SUB_OPERATION_QUADRANGULATE_FACES_BASIC, -1)))
    {
      undoRedoPreSave();
      
      int nb = FMap->quadrangulateMarkedFaces(getSelectionMark(), false);
      
      if (nb==0)
	{
	  setMessage("No face quadrangulated");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();

	  setModelChanged();
	  setMessage(nb, nb==1 ? " face quadrangulated" :
		     " faces quadrangulated");
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::swapEdge()
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_TRIANGULATION,
				   SUB_OPERATION_TRIANGULATION_SWAP_EDGE, -1))) {
    CDart *d;
    
    if (FMap->getMarkedCells(ORBIT_EDGE, getSelectionMark(), NULL, &d) != 1) {
      setMessage("Selected edge not correct: you need one edge");
      return false;
    }

    undoRedoPreSave();

    if (FMap->swapEdge(d)) {
      undoRedoPostSaveOk();
      setModelChanged();
      setMessage("No edge swapped");
      res = true;
    }
    else {
      undoRedoPostSaveFailed();
      setMessage("Not possible to swap the edge");
    }
    
    assert(isMapOk());
  }
  
  return res;
}
//******************************************************************************
bool CControlerGMap::triangulateGeoFaces()
{
  if (canApplyOperation(COperation(OPERATION_TRIANGULATION,
				   SUB_OPERATION_TRIANGULATION_GEO_FACES, -1)))
    {
      /* int vertexDI = getNewDirectInfo();    
	 pointDirectInfoToAttributeVertex(vertexDI); */
  
      undoRedoPreSave();
      FMap->triangulateMarkedFaces(getSelectionMark());
      undoRedoPostSaveOk();

      // freeDirectInfo(vertexDI);

      setModelChanged();
      setMessage("Faces triangulated");
      return true;
    }
  
  return false;
}
//******************************************************************************
