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
#include "controler-gmap-types.hh"
#include "parameter-gmap-vertex.hh"
#include <cassert>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterGMapVertex::CParameterGMapVertex(CGMapVertex* AGMap, int ANbRef) :
  CParameter         (ANbRef),
  FMap               (AGMap),
  FChanged           (false),
  FPartialChanged    (false),
  FMarkPartialChanged(-1),
  FBlocked           (false),
  FModeSimplification(false)
{}
//******************************************************************************
CParameterGMapVertex::
CParameterGMapVertex(const CParameterGMapVertex & AParam) :
  FMap               (AParam.FMap),
  FChanged           (false),
  FPartialChanged    (false),
  FMarkPartialChanged(-1),
  FBlocked           (false),
  FModeSimplification(false)
{}
//******************************************************************************
CParameterGMapVertex::~CParameterGMapVertex()
{}
//******************************************************************************
CParameter * CParameterGMapVertex::copy() const
{ return new CParameterGMapVertex(*this); }
//******************************************************************************
void CParameterGMapVertex::save(ostream& AStream)
{}
//------------------------------------------------------------------------------
void CParameterGMapVertex::load(istream& AStream)
{}
//------------------------------------------------------------------------------
void CParameterGMapVertex::reinit()
{}
//******************************************************************************
CGMapVertex* CParameterGMapVertex::getMap() const
{ return FMap; }
//------------------------------------------------------------------------------
void CParameterGMapVertex::setMap(CGMapVertex* AMap)
{
  if ( AMap!=FMap )
    {
      FMap = AMap;
      setChanged();
    }
}
//------------------------------------------------------------------------------
CGMapVertex* CParameterGMapVertex::getMapEmbedding() const
{ return FMapEmbedding; }
//------------------------------------------------------------------------------
CGMapVertex* CParameterGMapVertex::getDrawingMap() const
{
  if ( FModeSimplification ) return FMapEmbedding;
  return FMap;
}
//******************************************************************************
bool CParameterGMapVertex::getChanged() const
{ return FChanged; }
//------------------------------------------------------------------------------
void CParameterGMapVertex::setChanged()
{
  if ( !FChanged )
    {
      FChanged = true;
      putAllNeedToUpdate();
    }
}
//------------------------------------------------------------------------------
void CParameterGMapVertex::unsetChanged()
{
  FChanged = false;
  FPartialChanged = false;
}
//******************************************************************************
bool CParameterGMapVertex::getPartialChanged() const
{ return FPartialChanged; }
//------------------------------------------------------------------------------
void CParameterGMapVertex::setPartialChanged()
{
  if ( !FPartialChanged && !FChanged )
    {
      FPartialChanged = true;
      putAllNeedToUpdate();
    }
}
//------------------------------------------------------------------------------
void CParameterGMapVertex::unsetPartialChanged()
{ FPartialChanged = false; }
//******************************************************************************
bool CParameterGMapVertex::getBlocked() const
{ return FBlocked; }
//------------------------------------------------------------------------------
void CParameterGMapVertex::setBlocked()
{ FBlocked = true; }
//------------------------------------------------------------------------------
void CParameterGMapVertex::unsetBlocked()
{
  if ( FBlocked )
    {
      FBlocked = false;
      putAllNeedToUpdate();
    }
}
//******************************************************************************
bool CParameterGMapVertex::getModeSimplification() const
{ return FModeSimplification; }
//------------------------------------------------------------------------------
void CParameterGMapVertex::setModeSimplification()
{
  if ( !FModeSimplification )
    {
      FModeSimplification = true;
      FMapEmbedding =  new CGMapVertex;
      FDirectIndex = FMap->getNewDirectInfo();

      // We copy FMap into the new map FMapEmbedding.
      int mark = FMap->getNewMark();
      FMap->negateMaskMark(mark);
      FMap->duplicateMarkedDarts(mark,FMapEmbedding,FDirectIndex);

      FMapEmbedding->setBurstMethod(FMap->getBurstMethod());
      FMapEmbedding->setBurstCoef  (0, FMap->getBurstCoef(0));
      FMapEmbedding->setBurstCoef  (1, FMap->getBurstCoef(1));
      FMapEmbedding->setBurstCoef  (2, FMap->getBurstCoef(2));
      FMapEmbedding->setBurstCoef  (3, FMap->getBurstCoef(3));
      FMapEmbedding->updateAllBurstDarts();

      FMap->negateMaskMark(mark);
      FMap->freeMark(mark);
    }
}
//------------------------------------------------------------------------------
void CParameterGMapVertex::unsetModeSimplification()
{
  if ( FModeSimplification )
    {
      FModeSimplification = false;
      FMap->freeDirectInfo(FDirectIndex);
      delete FMapEmbedding; FMapEmbedding=NULL;
      putAllNeedToUpdate();
    }
}
//------------------------------------------------------------------------------
CDart* CParameterGMapVertex::getDartWithEmbedding(CDart * ADart)
{
  if ( FModeSimplification )
    {
      assert(FMap->getDirectInfoAsDart(ADart, FDirectIndex)!=NULL);
      return FMap->getDirectInfoAsDart(ADart, FDirectIndex);
    }

  return ADart;
}
//******************************************************************************
void CParameterGMapVertex::setBurstMethod(TBurstMethod AMethod)
{
  if (AMethod != getBurstMethod())
    {
      FMap->setBurstMethod(AMethod);
      if (FMapEmbedding!=NULL) FMapEmbedding->setBurstMethod(AMethod);
      
      FChanged = true;
      putAllNeedToUpdate();
    }
}
//------------------------------------------------------------------------------
TBurstMethod CParameterGMapVertex::getBurstMethod() const
{ return FMap->getBurstMethod(); }
//******************************************************************************
int CParameterGMapVertex::getMarkPartialChanged() const
{ return FMarkPartialChanged; }
//------------------------------------------------------------------------------
void CParameterGMapVertex::setMarkPartialChanged(int AValue)
{ FMarkPartialChanged = AValue; }
//******************************************************************************
int CParameterGMapVertex::getType() const
{ return PARAMETER_GMAP_VERTEX; }
//******************************************************************************
