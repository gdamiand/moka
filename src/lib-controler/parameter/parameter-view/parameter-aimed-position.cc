/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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
#include "controler-types.hh"
#include "parameters-default-values.hh"
#include "parameter-aimed-position.hh"
#include "math-extension.hh"
#include <cassert>
#include <cstring>
using namespace std;
//******************************************************************************
CParameterAimedPosition::CParameterAimedPosition(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterAimedPosition::
CParameterAimedPosition(const CParameterAimedPosition & AParam) :
  CParameter(AParam),
  FRotation (AParam.FRotation)
{
  FLookAt[0] = AParam.FLookAt[0];
  FLookAt[1] = AParam.FLookAt[1];
  FLookAt[2] = AParam.FLookAt[2];
}
//******************************************************************************
CParameterAimedPosition::~CParameterAimedPosition()
{}
//******************************************************************************
CParameter * CParameterAimedPosition::copy() const
{ return new CParameterAimedPosition(*this); }
//******************************************************************************
void CParameterAimedPosition::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterAimedPosition::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterAimedPosition::reinit()
{
  FRotation  = DEFAULT_AIMED_POSITION_ROTATION;
  FLookAt[0] = DEFAULT_AIMED_POSITION_LOOK_AT_X;
  FLookAt[1] = DEFAULT_AIMED_POSITION_LOOK_AT_Y;
  FLookAt[2] = DEFAULT_AIMED_POSITION_LOOK_AT_Z;

  putAllNeedToUpdate();
}
//******************************************************************************
ostream& operator<<(ostream& AStream,
			    const CParameterAimedPosition & AParameter)
{
  AStream<<"CParameterAimedPosition:"<<endl;
  
  AStream<<"  Rotation: " <<AParameter.FRotation<<endl;

  AStream<<"  LookAt:   "<<AParameter.FLookAt[0]<<" "
	 <<AParameter.FLookAt[1]<<" "<<AParameter.FLookAt[2]<<endl;
  
  AStream<<endl;

  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream,
			    CParameterAimedPosition & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterAimedPosition:") );

  AStream>>tmp; assert ( !strcmp(tmp, "Rotation:") );
  AStream>>AParameter.FRotation;

  AStream>>tmp; assert ( !strcmp(tmp, "LookAt:") );
  AStream>>AParameter.FLookAt[0]>>AParameter.FLookAt[1]>>AParameter.FLookAt[2];

  AParameter.putAllNeedToUpdate();
    
  return AStream;
}
//******************************************************************************
float CParameterAimedPosition::getRotation() const
{ return FRotation; }
//------------------------------------------------------------------------------
void CParameterAimedPosition::setRotation(float ARotation)
{
  if (! areEqual(FRotation, ARotation))
    {
      FRotation = ARotation;
      putAllNeedToUpdate();
    }
}
//------------------------------------------------------------------------------
void CParameterAimedPosition::incRotation(float ADelta)
{
  if (! isZero(ADelta))
    {
      FRotation += ADelta;
      putAllNeedToUpdate();
    }
}
//******************************************************************************
float CParameterAimedPosition::getLookAt(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FLookAt[AIndice];
}
//------------------------------------------------------------------------------
void CParameterAimedPosition::setLookAt(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);

  if (! areEqual(getLookAt(AIndice), AValue))
    {
      FLookAt[AIndice] = AValue;
      putAllNeedToUpdate();
    }
}
//******************************************************************************
int CParameterAimedPosition::getType() const
{ return PARAMETER_AIMED_POSITION; }
//******************************************************************************
