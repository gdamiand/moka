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
bool CControlerGMap::canExtendSelection()
{
  if (!canApplyOperation(COperation(OPERATION_SELECT,
				    SUB_OPERATION_EXTEND_SELECTION, -1)))
    return false;

  bool ok =
    getSelectionOrbit()==ORBIT_03  || getSelectionOrbit()==ORBIT_02  ||
    getSelectionOrbit()==ORBIT_023 || getSelectionOrbit()==ORBIT_01  ||
    getSelectionOrbit()==ORBIT_013 || getSelectionOrbit()==ORBIT_012;

  if (!ok)
    {
      setMessage("Mode de sélection incohérent");
      return false;
    }

  if (getLastSelectedDart()==NULL)
    {
      setMessage("Dernier brin sélectionné introuvable");
      return false;
    }

  return true;
}
//******************************************************************************
bool CControlerGMap::goForward1()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Forward, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection en avant effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goForward0()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Forward, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection en avant effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goBackward1()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Backward, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection en arrière effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goBackward0()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Backward, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection en arrière effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goLeft1()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Left, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection à gauche effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goLeft0()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Left, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection à gauche effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goRight1()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Right, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection à droite effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goRight0()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->go(CGMapGeneric::Right, getLastSelectedDart(),
			 getSelectionOrbit(), getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection à droite effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goForward1Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Forward,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection en avant effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goForward0Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Forward,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection en avant effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goBackward1Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Backward,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection en arrière effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goBackward0Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Backward,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection en arrière effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goLeft1Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Left,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection à gauche effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goLeft0Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Left,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection à gauche effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goRight1Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Right,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), true);
  selectDart(next);
  setMessage("Sélection à droite effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goRight0Rep()
{
  if (!canExtendSelection())
    return false;

  CDart* next = FMap->goAsFarAsPossible(CGMapGeneric::Right,
					getLastSelectedDart(),
					getSelectionOrbit(),
					getSelectionMark(), false);
  selectDart(next);
  setMessage("Désélection à droite effectuée");
  return true;
}
//******************************************************************************
bool CControlerGMap::goAlphai( unsigned int ADim )
{
  assert( ADim<= 3);
  
  if (!canApplyOperation(COperation(OPERATION_SELECT,
				    SUB_OPERATION_MOVE_SELECTION, -1)))
    {
      setMessage("Mode de sélection incohérent");
      return false;
    }

  if (getLastSelectedDart()==NULL)
    {
      setMessage("Dernier brin sélectionné introuvable");
      return false;
    }

  if ( !FMap->isFree(getLastSelectedDart(),ADim) )
    {
      selectDart(FMap->alpha(getLastSelectedDart(),ADim));
      setMessage("Sélection par alpha", ADim, " effectuée");
    }
  else
    {
      setMessage("Le dernier brin sélectionné est ", ADim, " libre");
      return false;
    }
  
  return true;
}
//------------------------------------------------------------------------------
bool CControlerGMap::goAlpha0()
{ return goAlphai(0); }
//------------------------------------------------------------------------------
bool CControlerGMap::goAlpha1()
{ return goAlphai(1); }
//------------------------------------------------------------------------------
bool CControlerGMap::goAlpha2()
{ return goAlphai(2); }
//------------------------------------------------------------------------------
bool CControlerGMap::goAlpha3()
{ return goAlphai(3); }
//******************************************************************************
