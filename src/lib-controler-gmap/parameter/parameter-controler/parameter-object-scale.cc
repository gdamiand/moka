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
#include "parameter-object-scale.hh"
#include "transformation-matrix.hh"
#include "geometry.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterObjectScale::CParameterObjectScale(int ANbRef) :
  CParameter  (ANbRef),
  FScaleType  (SCALE_POINT),
  FCenter     (ORIGIN),
  FVector     (OZ),
  FCoef	      (CVertex(1.0,1.0,1.0)),
  FPonderation(false),
  FIsoScale   (true)
{}
//******************************************************************************
CParameterObjectScale::
CParameterObjectScale(const CParameterObjectScale & AParam) :
  CParameter  (AParam),
  FScaleType  (AParam.FScaleType),
  FCenter     (AParam.FCenter),
  FVector     (AParam.FVector),
  FCoef	      (AParam.FCoef),
  FPonderation(AParam.FPonderation),
  FIsoScale   (AParam.FIsoScale)
{}
//******************************************************************************
CParameterObjectScale::~CParameterObjectScale()
{
}
//******************************************************************************
CParameter * CParameterObjectScale::copy() const
{ return new CParameterObjectScale(*this); }
//******************************************************************************
void CParameterObjectScale::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectScale::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectScale::reinit()
{}
//******************************************************************************
int CParameterObjectScale::getType() const
{
  return PARAMETER_OBJECT_SCALE;
}
//******************************************************************************
void CParameterObjectScale::setScaleType(TScaleType AScaleType)
{
  if (FScaleType != AScaleType)
    {
      putAllNeedToUpdate();
      FScaleType = AScaleType;

      if ( AScaleType!=SCALE_POINT)
	setIsoScale(true);
    }
}

TScaleType CParameterObjectScale::getScaleType() const
{
  return FScaleType;
}
//******************************************************************************
void CParameterObjectScale::setCenter(const CVertex & ACenter)
{
  if (FCenter != ACenter)
    {
      putAllNeedToUpdate();
      FCenter = ACenter;
    }
}

CVertex CParameterObjectScale::getCenter() const
{
  return FCenter;
}
//******************************************************************************
void CParameterObjectScale::setVector(const CVertex & AVector)
{
  if (FVector != AVector && !AVector.isNull())
    {
      putAllNeedToUpdate();
      FVector = AVector;
    }
}

CVertex CParameterObjectScale::getVector() const
{
  return FVector;
}
//******************************************************************************
bool CParameterObjectScale::getIsoScale()
{ return FIsoScale; }

void CParameterObjectScale::setIsoScale(bool AValue)
{
  FIsoScale = AValue;
  if ( AValue )
    setIsoCoef(getCoef(0));
    
}
//******************************************************************************
void CParameterObjectScale::setIsoCoef(float AValue)
{ FCoef.setXYZ(AValue,AValue,AValue); }

float CParameterObjectScale::getIsoCoef() const
{
  assert( FCoef.getX()==FCoef.getY() && FCoef.getY()==FCoef.getZ() );

  return FCoef.getX();
}

void CParameterObjectScale::setCoef(const CVertex & AValue)
{ FCoef = AValue; }

CVertex CParameterObjectScale::getCoef() const
{ return FCoef; }

void CParameterObjectScale::setCoef(int ADim, float AValue)
{
  switch (ADim)
    {
    case 0: setCoef(CVertex(AValue, getCoef(1), getCoef(2))); break;
    case 1: setCoef(CVertex(getCoef(0), AValue, getCoef(2))); break;
    case 2: setCoef(CVertex(getCoef(0), getCoef(1), AValue)); break;
    }
}

float CParameterObjectScale::getCoef(int ADim) const
{
  assert(0 <= ADim && ADim < 3);
  return FCoef.getCoord(ADim);
}
//******************************************************************************
void CParameterObjectScale::setPonderation(bool AActive)
{
  if (FPonderation != AActive)
    {
      putAllNeedToUpdate();
      FPonderation = AActive;
    }
}

bool CParameterObjectScale::getPonderation() const
{
  return FPonderation;
}
//******************************************************************************
