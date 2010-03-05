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
#ifdef MODULE_ROUNDING
//******************************************************************************
#include "controler-gmap.hh"
#include "g-map-vertex.hh"
#include "view-precompile.hh"
using namespace GMap3d;
//******************************************************************************
void CControlerGMap::modeRoundingReinitAllCoefs()
{
  if (!areRoundingCoefsAllocated())
    return;

  TCoordinate k0 =
    getParameterOperations()->getDefaultRoundingCoefForVertices();

  TCoordinate k1 =
    getParameterOperations()->getDefaultRoundingCoefForEdges();

  int selected = FMap->getNewMark();
  FMap->negateMaskMark(selected);
  FRounding->setRoundingCoefOfMarkedDarts(selected, 0, k0);
  FRounding->setRoundingCoefOfMarkedDarts(selected, 1, k1);
  FMap->negateMaskMark(selected);
  FMap->freeMark(selected);
}
//******************************************************************************
void CControlerGMap::modeRoundingSetCoefOfMarkedDarts(int ADimension,
						  TCoordinate ACoef)
{
  if (!areRoundingCoefsAllocated() || (ADimension!=0 && ADimension!=1))
    return;

  FRounding->
    setRoundingCoefOfMarkedDarts(getSelectionMark(), ADimension, ACoef);
}
//------------------------------------------------------------------------------
bool CControlerGMap::modeRoundingGetCoefOfMarkedDarts(int ADimension,
						  TCoordinate& ACoef)
{
  if (!areRoundingCoefsAllocated() || (ADimension!=0 && ADimension!=1))
    return false;

  return FRounding->getRoundingCoefOfMarkedDarts(getSelectionMark(),
						 ADimension, ACoef);
}
//------------------------------------------------------------------------------
TCoordinate CControlerGMap::modeRoundingSelectNextCoef(int ADimension)
{
  if (!areRoundingCoefsAllocated() || (ADimension!=0 && ADimension!=1))
    return false;

  int mark = getSelectionMark();

  TCoordinate coef = FRounding->selectNextRoundingCoef(mark, ADimension);

  CDart* last = getLastSelectedDart();
  if (last != NULL && !FMap->isMarked(last, mark))
    unsetLastSelectedDart();

  setSelectionChanged();

  return coef;
}
//******************************************************************************
void CControlerGMap::modeRoundingBegin()
{
  assert(! areRoundingCoefsAllocated());

  FRounding = new CRoundingInterface(FMap);
  modeRoundingReinitAllCoefs();
}
//------------------------------------------------------------------------------
void CControlerGMap::modeRoundingEnd()
{
  assert(areRoundingCoefsAllocated());

  delete FRounding;
  FRounding = NULL;
}
//------------------------------------------------------------------------------
bool CControlerGMap::areRoundingCoefsAllocated() const
{
  return FRounding != NULL;
}
//------------------------------------------------------------------------------
void CControlerGMap::modeRoundingOperationStart()
{
  if ( FViews[FCurrentViewId]!=NULL )
    FViews[FCurrentViewId]->pick(FLastX, FLastY);
}
//------------------------------------------------------------------------------
void CControlerGMap::modeRoundingOperationMove()
{
}
//------------------------------------------------------------------------------
void CControlerGMap::modeRoundingOperationStop()
{
}
//******************************************************************************
bool CControlerGMap::modeRoundingRoundVertices(bool ADig, bool /*ASurfacic*/)
{
#ifndef MODULE_ROUNDING
  if (ASurfacic)
    return false;
#endif // MODULE_ROUNDING
  assert(areRoundingCoefsAllocated());

  if (!canApplyOperation(COperation(OPERATION_ROUND, 0)))
    return false;

  undoRedoPreSave();

  int selected = FMap->getNewMark();
  FMap->markIncidentCells(ORBIT_23, getSelectionMark(), selected);

  int nb = FRounding->roundMarkedVertices(selected, ADig);

  FMap->unmarkAll(selected);
  FMap->freeMark(selected);

  if (nb==0)
    {
      setMessage("No vertex rounded");
      undoRedoPostSaveFailed();
      return false;
    }

  undoRedoPostSaveOk();
  setModelChanged();
  setMessage(nb, nb==1 ? " vertex rounded" : " vertices rounded");
  modeRoundingReinitAllCoefs();
  return true;
}
//******************************************************************************
bool CControlerGMap::modeRoundingRoundEdges(bool A3D, bool ADig, bool ASetback,
					    bool /*ASurfacic*/)
{
#ifndef MODULE_ROUNDING
  if (ASurfacic)
    return false;
#endif // MODULE_ROUNDING
  assert(areRoundingCoefsAllocated());

  if (!canApplyOperation(COperation(OPERATION_ROUND, 1)))
    return false;

  undoRedoPreSave();

  int selected = FMap->getNewMark();
  FMap->markIncidentCells(ORBIT_EDGE, getSelectionMark(), selected);

  int nb = FRounding->roundMarkedEdges(selected, A3D, ADig, ASetback);

  FMap->unmarkAll(selected);
  FMap->freeMark(selected);  
      
  if (nb==0)
    {
      setMessage("No edge rounded");
      undoRedoPostSaveFailed();
      return false;
    }

  undoRedoPostSaveOk();
  setModelChanged();
  setMessage(nb, nb==1 ? " edge rounded" : " edges rounded");
  modeRoundingReinitAllCoefs();
  return true;
}
//******************************************************************************
#endif // MODULE_ROUNDING
//******************************************************************************
