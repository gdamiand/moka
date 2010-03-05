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
#ifdef MODULE_MESH
//******************************************************************************
#include "g-map-vertex.hh"
#include "controler-gmap.hh"
#include <cassert>
#include <sstream>

#include "mesh-interface.hh"

using namespace GMap3d;
//******************************************************************************
bool CControlerGMap::meshEdges()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_MESH, 1)))
    {
      undoRedoPreSave();

      int sx = getParameterOperations()->getMeshNbSubdivisionsX();
      
      int nb =
	CMesh(FMap).meshMarkedEdges(getSelectionMark(), sx, NULL,
				    getParameterOperations()
				    ->getInterpolationMerges(),
				    getParameterOperations()
				    ->getInterpolationSews());
      
      if (nb==0)
	{
	  setMessage("No edge meshed");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();	  
	  setMessage(nb, nb==1 ? " edge meshed" : " edges meshed");
	  res = true;
	}
    }
  
  return res;
}
//******************************************************************************
bool CControlerGMap::meshSquares(int ADimFrom, int ADimTo)
{
  if (! (0 <= ADimFrom && ADimFrom < ADimTo && ADimTo <= 2))
    return false;

  bool res = false;
  if (canApplyOperation(COperation(OPERATION_MESH, 2)))
    {
      undoRedoPreSave();
    
      int sx = getParameterOperations()->getMeshNbSubdivisionsX();
      int sy = getParameterOperations()->getMeshNbSubdivisionsY();

      int nb =
	CMesh(FMap).meshMarkedSquares(getSelectionMark(), sx, sy, NULL,
				      getParameterOperations()
				      ->getInterpolationMerges(),
				      getParameterOperations()
				      ->getInterpolationSews(),
				      false, ADimFrom, ADimTo);

      std::stringstream s;
      if (nb==0) s<<"Aucun";
      else s<<nb;

      s<<(nb<2 ? "square meshed" : "squared meshed");
      s<<" ["<<ADimFrom<<" -> "<<ADimTo<<"]";
    
      if (nb==0)
	undoRedoPostSaveFailed();
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  res = true;
	}
    
      setMessage(s.str());
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::meshCubes(int ADimFrom, int ADimTo)
{
  if (! (0 <= ADimFrom && ADimFrom < ADimTo && ADimTo <= 3))
    return false;

  bool res = false;
  if (canApplyOperation(COperation(OPERATION_MESH, 3)))
    {    
      undoRedoPreSave();
					
      if (! getParameterOperations()->getInterpolationMerges())
	{
	  setMessage("Not yet developed!!! (the created mesh must "
		     "be merged with the original object)");
	  return false;
	}

      int sx = getParameterOperations()->getMeshNbSubdivisionsX();
      int sy = getParameterOperations()->getMeshNbSubdivisionsY();
      int sz = getParameterOperations()->getMeshNbSubdivisionsZ();

      bool merges = getParameterOperations()->getInterpolationMerges();
      bool sews = getParameterOperations()->getInterpolationSews();

      int nb = 
	CMesh(FMap).meshMarkedCubes(getSelectionMark(), sx,sy,sz, NULL,
				    merges, sews, false, ADimFrom, ADimTo);

      std::stringstream s;
      
      if (nb==0) s<<"No";
      else s<<nb;
      
      s<<(nb<2 ? "cube meshed" : "cubes meshed");
      s<<" ["<<ADimFrom<<" -> "<<ADimTo<<"]";

      if (nb==0)
	undoRedoPostSaveFailed();
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  res = true;
	}

      setMessage(s.str());
    }
  return res;
}
//******************************************************************************
bool CControlerGMap::smoothEdges()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_SMOOTH, 1)))
    {
      undoRedoPreSave();

      int sx = getParameterOperations()->getMeshNbSubdivisionsX();

      int nb =
	CMesh(FMap).meshMarkedEdges(getSelectionMark(), sx,
				    &getParameterOperations()->getMeshMatrix(),
				    getParameterOperations()->getSmoothMerges(),
				    getParameterOperations()->getSmoothSews());

      if (nb==0)
	{
	  setMessage("No edge smoothed");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  setMessage(nb, nb==1 ? " edge smoothed" : " edges smoothed");
	  res = true;
	}
    }
  
  return res;
}
//******************************************************************************
bool CControlerGMap::smoothSquares()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_SMOOTH, 2)))
    {
      undoRedoPreSave();

      int sx = getParameterOperations()->getMeshNbSubdivisionsX();
      int sy = getParameterOperations()->getMeshNbSubdivisionsY();

      int nb =
	CMesh(FMap).meshMarkedSquares(getSelectionMark(), sx,sy,
				      &getParameterOperations()
				      ->getMeshMatrix(),
				      getParameterOperations()
				      ->getSmoothMerges(),
				      getParameterOperations()
				      ->getSmoothSews());

      if (nb==0)
	{
	  setMessage("No square smoothed");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  setMessage(nb, nb==1 ? " square smoothed" : " squares smoothed");
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::smoothCubes()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_SMOOTH, 3)))
    {
      if (!getParameterOperations()->getSmoothMerges())
	{
	  setMessage("Not yet developed!!! (the created mesh must "
		     "be merged with the original object)");
	  return false;
	}
      
      undoRedoPreSave();

      int sx = getParameterOperations()->getMeshNbSubdivisionsX();
      int sy = getParameterOperations()->getMeshNbSubdivisionsY();
      int sz = getParameterOperations()->getMeshNbSubdivisionsZ();

      int nb =
	CMesh(FMap).meshMarkedCubes(getSelectionMark(), sx,sy,sz,
				    &getParameterOperations()->getMeshMatrix(),
				    getParameterOperations()->getSmoothMerges(),
				    getParameterOperations()->getSmoothSews());
      
      if (nb==0)
	{
	  setMessage("No cube smoothed");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  setMessage(nb, nb==1 ? " cube smoothed" : " cubes smoothed");
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
#endif // MODULE_MESH
//******************************************************************************
