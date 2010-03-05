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
bool CControlerGMap::dual2d()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_DUAL, 2)))
    {
      undoRedoPreSave();

      if (FMap->dualOfMarkedDarts(2, getSelectionMark()) <= 0)
	{
	  setMessage("Dual 2D impossible: no selected darts");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  setMessage("Dual 2D done");
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::dual3d()
{
  bool res = false;
  if (canApplyOperation(COperation(OPERATION_DUAL, 3)))
    {
      undoRedoPreSave();

      if (FMap->dualOfMarkedDarts(3, getSelectionMark()) <= 0)
	{
	  setMessage("Dual 3D impossible: no selected darts");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();
	  setModelChanged();
	  setMessage("Dual 3D done");
	  res = true;
	}
    }

  return res;
}
//******************************************************************************
