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
bool CControlerGMap::intuitiveTopoSew()
{
  if (canApplyOperation(COperation(OPERATION_SEW,
				   SUB_OPERATION_INTUITIVE_TOPO, -1)))
    {
      CDart * dart1, * dart2;

      if (getDartsForSew(& dart1, & dart2))
	{
	  int dim = FMap->getSewDimension(dart1, dart2);
	  if (dim!=4)
	    {
	      undoRedoPreSave();
	      FMap->sew(dart1, dart2, dim);
	      undoRedoPostSaveOk();
	      unmarkAllCurrentSelectionLevel();

	      setModelChanged();
	      setMessage("Intuitive sew done");
	      return true;
	    }
	}
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::topoSew(int ADimension)
{
  assert(0 <= ADimension && ADimension <= 3);

  if (canApplyOperation(COperation(OPERATION_SEW, SUB_OPERATION_TOPO,
				   ADimension)))
    {
      CDart * dart1, * dart2;

      if (getDartsForSew(& dart1, & dart2))
	{
	  if (FMap->canSew(dart1, dart2, ADimension))
	    {
	      undoRedoPreSave();
	      FMap->sew(dart1, dart2, ADimension);
	      undoRedoPostSaveOk();
	      unmarkAllCurrentSelectionLevel();

	      setModelChanged();
	      setMessage("Sew done");
	      return true;
	    }
	}
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::intuitiveGeoSew()
{
  if (canApplyOperation(COperation(OPERATION_SEW,
				   SUB_OPERATION_INTUITIVE_GEO, -1)))
    {
      CDart * dart1, * dart2;

      if (getDartsForSew(& dart1, & dart2))
	{
	  int dim = FMap->getSewDimension(dart1, dart2);
	  if (dim!=4)
	    {
	      undoRedoPreSave();
	      FMap->geoSew(dart1, dart2, dim,
			   getParameterOperations()->getRotateCells(),
			   getParameterOperations()->getScaleCells(),
			   getParameterOperations()->getTranslateCells());
	      undoRedoPostSaveOk();
	      unmarkAllCurrentSelectionLevel();

	      setModelChanged();
	      setMessage("Intuitive sew with plating done");
	      return true;
	    }
	}
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::geoSew(int ADimension)
{
  assert(0 < ADimension && ADimension <= 3); // ADimension != 0

  if (canApplyOperation(COperation(OPERATION_SEW, SUB_OPERATION_GEO,
				   ADimension)))
    {
      CDart * dart1, * dart2;

      if (getDartsForSew(& dart1, & dart2))
	{
	  if (FMap->canSew(dart1, dart2, ADimension))
	    {
	      undoRedoPreSave();
	      FMap->geoSew(dart1, dart2, ADimension,
			   getParameterOperations()->getRotateCells(),
			   getParameterOperations()->getScaleCells(),
			   getParameterOperations()->getTranslateCells());
	      undoRedoPostSaveOk();
	      unmarkAllCurrentSelectionLevel();

	      setModelChanged();
	      setMessage("Sew with plating done");
	      return true;
	    }
	}
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::intuitiveUnsew()
{
  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_UNSEW,
				   SUB_OPERATION_INTUITIVE_TOPO, -1)))
    {
      undoRedoPreSave();
      
      int nb = FMap->intuitiveUnsewMarkedCells(getSelectionMark());
      
      if (nb==0)
	{
	  setMessage("No unsew done");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();

	  setModelChanged();
	  res = true;
	  setMessage(nb, (nb==1 ? " intuitive unsew done" :
			  " intuitive unsews done"));
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::unsew(int ADimension)
{
  assert(ADimension>=0 && ADimension<=3);

  bool res = false;
  
  if (canApplyOperation(COperation(OPERATION_UNSEW, SUB_OPERATION_TOPO,
				   ADimension)))
    {
      undoRedoPreSave();

      int nb = FMap->unsewMarkedCells(getSelectionMark(), ADimension);

      if (nb==0)
	{
	  setMessage("No unsew done");
	  undoRedoPostSaveFailed();
	}
      else
	{
	  undoRedoPostSaveOk();

	  setModelChanged();
	  res = true;
	  setMessage(nb, (nb==1 ? " unsew done" :
			  " unsews done"));
	}
    }

  return res;
}
//******************************************************************************
bool CControlerGMap::invertLastOperation()
  // Invertion de la dernière opération lorsque cela est possible. Par exemple
  // si on vient de 2-coudre(d1,d2), alors découd et 2-coud(d1,alpha0(d2))
{
  if (!canApplyOperation(COperation(OPERATION_INVERT_LAST_OPERATION)))
    return false;

  if (FLastOperation.getType()!=OPERATION_SEW &&
      FLastOperation.getType()!=OPERATION_PLATE)
    {
      setMessage("Last operation not reversible");
      return false;
    }

  COperation toReevaluate(FLastOperation);
  basicUndo();
  CDart *d1, *d2;

  if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
			   getLastSelectedDart(), &d2,&d1) != 2)
    {
      setMessage("Selection not correct [???]");
      basicRedo();
      return false;
    }

  deselectDart(d2);
  selectDart(FMap->alpha0(d2));

  if (!applyOperation(toReevaluate))
    {
      setMessage("Not possible to revert the last operation");
      return false;
    }

  setMessage("Last operation reversed");
  return true;
}
//******************************************************************************
bool CControlerGMap::invertLastOperationGeometry()
  // Ici on inverse la géométrie, c'est à dire l'ordre des paramètre.
{
  if (!canApplyOperation(COperation(OPERATION_INVERT_LAST_OPERATION)))
    return false;

  if (FLastOperation.getType()!=OPERATION_SEW &&
      FLastOperation.getType()!=OPERATION_PLATE)
    {
      setMessage("Last operation not reversible");
      return false;
    }

  COperation toReevaluate(FLastOperation);
  basicUndo();
  CDart *d1, *d2;

  if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
			   getLastSelectedDart(), &d2,&d1) != 2)
    {
      setMessage("Selection not correct [???]");
      basicRedo();
      return false;
    }

  selectDart(d1);

  if (!applyOperation(toReevaluate))
    {
      setMessage("Not possible to revert the last operation");
      return false;
    }

  setMessage("Last operation reversed");
  return true;
}
//******************************************************************************
bool CControlerGMap::topoSewBorders(int ADimension)
{
  assert( ADimension==1 || ADimension==2 );

  if (canApplyOperation(COperation(OPERATION_SEW, SUB_OPERATION_TOPO_BORDERS,
				   ADimension)))
    {
      CDart *d1, *d2;
      
      if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
			       getLastSelectedDart(), &d2,&d1) != 2)
	{
	  setMessage("Selection not correct: you need two darts");
	  return false;
	}
      
      if (FMap->getBorderDimension(d1)!=ADimension ||
	  !FMap->canSewBorders(d1,d2, ADimension))
	{
	  setMessage("Borders not possible to sew");
	  return false;
	}
      
      undoRedoPreSave();
      FMap->intuitiveBorderSew(d1,d2);
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();

      setModelChanged();
      setMessage("Borders sewn");
      return true;
    }

  return false;
}
//******************************************************************************
bool CControlerGMap::geoSewBorders(int ADimension)
{
  assert( ADimension==1 || ADimension==2 );

  if (canApplyOperation(COperation(OPERATION_SEW, SUB_OPERATION_GEO_BORDERS,
				   ADimension)))
    {
      CDart *d1, *d2;
      
      if (FMap->getMarkedCells(ORBIT_SELF, getSelectionMark(),
			       getLastSelectedDart(), &d2,&d1) != 2)
	{
	  setMessage("Selection not correct");
	  return false;
	}
      
      if (FMap->getBorderDimension(d1)!=ADimension ||
	  !FMap->canSewBorders(d1,d2, ADimension))
	{
	  setMessage("Borders impossible to sew");
	  return false;
	}

      undoRedoPreSave();
      FMap->intuitiveGeoBorderSew(d1,d2,
				  getParameterOperations()->getRotateCells(),
				  getParameterOperations()->getScaleCells(),
				  getParameterOperations()->getTranslateCells());
      undoRedoPostSaveOk();
      unmarkAllCurrentSelectionLevel();

      setModelChanged();
      setMessage("Borders sewn");
      return true;
    }

  return false;
}
//******************************************************************************
