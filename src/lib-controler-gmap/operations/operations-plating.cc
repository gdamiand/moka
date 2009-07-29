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
bool CControlerGMap::intuitivePlate()
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_PLATE,
				   SUB_OPERATION_INTUITIVE, -1)))
    {
      CDart *d1, *d2;

      if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
			       getLastSelectedDart(), &d2,&d1) != 2)
	{
	  setMessage("Sélection incohérente");
	  return false;
	}

      undoRedoPreSave();
      
      if (FMap->intuitivePlate(d1,d2,
			       getParameterOperations()->getRotateCells(),
			       getParameterOperations()->getScaleCells(),
			       getParameterOperations()->getTranslateCells()))
	{
	  undoRedoPostSaveOk();
	  unmarkAllCurrentSelectionLevel();
	  setModelChanged();
	  setMessage("Cellules plaquées");
	  res = true;
	}
      else
	{
	  setMessage("Cellules impossible à plaquer");
	  undoRedoPostSaveFailed();
	}
    }
  
  return res;
}
//******************************************************************************
bool CControlerGMap::plate(int ADimension)
{
  if (canApplyOperation(COperation(OPERATION_PLATE, ADimension)))
    {
      assert(ADimension>=0 && ADimension<3); // i!=3 !!!!
      
      CDart *d1, *d2;
      
      if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
			       getLastSelectedDart(), &d2,&d1) != 2)
	{
	  setMessage("Sélection incohérente");
	  return false;
	}

      if (!FMap->canPlate(d1,d2))
	{
	  setMessage(ADimension, "-cellules impossible à plaquer");
	  return false;
	}

      undoRedoPreSave();
      FMap->plate(d1,d2, ADimension,
		  getParameterOperations()->getRotateCells(),
		  getParameterOperations()->getScaleCells(),
		  getParameterOperations()->getTranslateCells());
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();

      setModelChanged();
      setMessage(ADimension, "-cellules plaquées");
      return true;
    }
  
  return false;
}
//******************************************************************************
bool CControlerGMap::plateBorders(int ADimension)
{
  if (canApplyOperation(COperation(OPERATION_PLATE,
				   SUB_OPERATION_GEO_BORDERS, -1)))
    {
      assert(ADimension==1 || ADimension==2);
      
      CDart *d1, *d2;

      if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
			       getLastSelectedDart(), &d2,&d1) != 2)
	{
	  setMessage("Sélection incohérente");
	  return false;
	}

      if (!FMap->canPlate(d1,d2))
	{
	  setMessage(ADimension, "-bords impossible à plaquer");
	  return false;
	}

      undoRedoPreSave();
      FMap->borderPlate(d1,d2, ADimension,
			getParameterOperations()->getRotateCells(),
			getParameterOperations()->getScaleCells(),
			getParameterOperations()->getTranslateCells());
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();

      setModelChanged();
      setMessage(ADimension, "-bords plaqués");
      return true;
    }

  return false;
}
//******************************************************************************
