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
#include "controler-gmap.hh"
#include "parameter-selection.hh"
#include <cassert>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterSelection::CParameterSelection(CControlerGMap* AControlerGMapV,
					 int ANbSelectionLevels, int ANbRef) :
  CParameter	 (ANbRef),
  FControlerGMapV(AControlerGMapV),
  FNbLevels	 (ANbSelectionLevels),
  FCurrentLevel	 (0),
  FSelectionOrbit(ORBIT_SELF),
  FHalfSelection (false),
  FChanged       (false)
{
  assert(AControlerGMapV != NULL);
  assert(0 < ANbSelectionLevels);

  FMarks = new int[FNbLevels];
  FLasts = new CDart* [FNbLevels];

  for (int i=0; i<FNbLevels; ++i)
    {
      FMarks[i] = FControlerGMapV->getMap()->getNewMark();
      FLasts[i] = NULL;
    }
}
//******************************************************************************
CParameterSelection::CParameterSelection(const CParameterSelection & AParam ) :
  CParameter     (AParam),
  FControlerGMapV(AParam.FControlerGMapV),
  FNbLevels	 (AParam.FNbLevels),
  FCurrentLevel	 (AParam.FCurrentLevel),
  FSelectionOrbit(AParam.FSelectionOrbit),
  FChanged       (false)
{
  FMarks = new int[FNbLevels];
  FLasts = new CDart* [FNbLevels];

  for (int i=0; i<FNbLevels; ++i)
    {
      FMarks[i] = FControlerGMapV->getMap()->getNewMark();
      FLasts[i] = NULL;
    }
}
//******************************************************************************
CParameterSelection::~CParameterSelection()
{
  for (int i=0; i<FNbLevels; ++i)
    FControlerGMapV->getMap()->freeMark(FMarks[i]);

  delete []FMarks;
  delete []FLasts;
}
//******************************************************************************
CParameter * CParameterSelection::copy() const
{ return new CParameterSelection(*this); }
//******************************************************************************
void CParameterSelection::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterSelection::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterSelection::reinit()
{}
//******************************************************************************
bool CParameterSelection::getChanged() const
{ return FChanged; }
//------------------------------------------------------------------------------
void CParameterSelection::setChanged()
{
  if ( !FChanged )
    {
      FChanged = true;
      putAllNeedToUpdate();
    }
}
//------------------------------------------------------------------------------
void CParameterSelection::unsetChanged()
{ FChanged = false; }
//******************************************************************************
int CParameterSelection::getType() const
{ return PARAMETER_SELECTION; }
//******************************************************************************
int CParameterSelection::getNbSelectionLevels() const
{ return FNbLevels; }
//******************************************************************************
int CParameterSelection::getSelectionLevel() const
{ return FCurrentLevel; }
//------------------------------------------------------------------------------
int CParameterSelection::getNextSelectionLevel(int AIndex) const
{ return (getSelectionLevel()+AIndex) % getNbSelectionLevels(); }
//------------------------------------------------------------------------------
void CParameterSelection::setSelectionLevel(int ALevel)
{
  assert( 0<=ALevel && ALevel<FNbLevels );
  if ( FCurrentLevel!=ALevel )
    {
      FCurrentLevel = ALevel;
      setChanged();
    }
}
//******************************************************************************
int CParameterSelection::getSelectionMark() const
{ return FMarks[FCurrentLevel]; }
//------------------------------------------------------------------------------
int CParameterSelection::getSelectionMark(int ALevel) const
{
  assert( 0<=ALevel && ALevel<FNbLevels );
  return FMarks[ALevel];
}
//------------------------------------------------------------------------------
int CParameterSelection::getNextSelectionMark(int AIndex) const
{ return getSelectionMark(getNextSelectionLevel(AIndex)); }
//******************************************************************************
TOrbit CParameterSelection::getSelectionOrbit() const
{ return FSelectionOrbit; }
//------------------------------------------------------------------------------
void CParameterSelection::setSelectionOrbit(TOrbit AOrbit)
{
  assert(ORBIT_SELF <= AOrbit && AOrbit <= ORBIT_CC);
  FSelectionOrbit = AOrbit;
}
//------------------------------------------------------------------------------
void CParameterSelection::setNextSelectionOrbit()
{
  FSelectionOrbit = FSelectionOrbit==ORBIT_CC ? ORBIT_SELF : 1 + FSelectionOrbit;
}
//------------------------------------------------------------------------------
bool CParameterSelection::getHalfSelection() const
{ return FHalfSelection; }
//------------------------------------------------------------------------------
void CParameterSelection::setHalfSelection(bool ABool)
{ FHalfSelection = ABool; }
//******************************************************************************
CDart* CParameterSelection::getLastSelectedDart() const
{ return FLasts[FCurrentLevel]; }
//------------------------------------------------------------------------------
CDart* CParameterSelection::getLastSelectedDart(int ALevel) const
{
  assert( 0<=ALevel && ALevel<FNbLevels );
  return FLasts[ALevel];
}
//------------------------------------------------------------------------------
CDart* CParameterSelection::getNextLastSelectedDart(int AIndex) const
{ return getLastSelectedDart(getNextSelectionLevel(AIndex)); }
//******************************************************************************
void CParameterSelection::setLastSelectedDart(CDart* ALastDart)
{
  assert( ALastDart!=NULL );
  if ( FLasts[FCurrentLevel]!=ALastDart )
    {
      FLasts[FCurrentLevel] = ALastDart;
      setChanged();
    }
}
//------------------------------------------------------------------------------
void CParameterSelection::setLastSelectedDart(int ALevel, CDart* ALastDart)
{
  assert( 0<=ALevel && ALevel<FNbLevels );
  assert( ALastDart!=NULL );
  if ( FLasts[ALevel]!=ALastDart )
    {
      FLasts[ALevel] = ALastDart;
      setChanged();
    }
}
//******************************************************************************
void CParameterSelection::unsetLastSelectedDart()
{
  if ( FLasts[FCurrentLevel]!=NULL )
    {
      FLasts[FCurrentLevel] = NULL;
      setChanged();
    }
}
//------------------------------------------------------------------------------
void CParameterSelection::unsetLastSelectedDart(int ALevel)
{
  assert( 0<=ALevel && ALevel<FNbLevels );
  
  if ( FLasts[ALevel]!=NULL )
    {
      FLasts[ALevel] = NULL;
      setChanged();
    }
}
//------------------------------------------------------------------------------
void CParameterSelection::unsetNextLastSelectedDart(int AIndex)
{ unsetLastSelectedDart(getNextSelectionLevel(AIndex)); }
//------------------------------------------------------------------------------
void CParameterSelection::unsetAllLastSelectedDarts()
{
  for (int i=0; i<getNbSelectionLevels(); ++i)
    unsetLastSelectedDart(i);
}
//******************************************************************************
void CParameterSelection::swapLevels(int ALevel1, int ALevel2)
{
  assert( 0<=ALevel1 && ALevel1<FNbLevels );
  assert( 0<=ALevel2 && ALevel2<FNbLevels );

  if ( ALevel1==ALevel2 ) return;

//    int mark	  = FMarks[ALevel1];
//    FMarks[ALevel1] = FMarks[ALevel2];
//    FMarks[ALevel2] = mark;

  CDart* last	  = FLasts[ALevel1];
  FLasts[ALevel1] = FLasts[ALevel2];
  FLasts[ALevel2] = last;

  setChanged();
}
//******************************************************************************
void CParameterSelection::toggleOrbitSelection(CDart* ADart)
{ FControlerGMapV->toggleOrbitSelection(ADart, getSelectionLevel()); }
//******************************************************************************
