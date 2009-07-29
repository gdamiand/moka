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
#include "parameter-object-ponderation.hh"
#include "transformation-matrix.hh"
#include "geometry.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterObjectPonderation::CParameterObjectPonderation(int ANbRef) :
  CParameter		   (ANbRef),
  FPonderationType	   (PONDERATION_POINT),
  FPonderationCenter	   (ORIGIN),
  FPonderationVector	   (OZ),
  FPonderationStretchIn	   (false),
  FPonderationStretchInMin (0.0),
  FPonderationStretchInMax (1.0),
  FPonderationFunctionType (FUNCTION_LIN),
  FPonderationStretchOut   (false),
  FPonderationStretchOutMin(0.75),
  FPonderationStretchOutMax(1.50)
{}
//******************************************************************************
CParameterObjectPonderation::
CParameterObjectPonderation(const CParameterObjectPonderation & AParam) :
  CParameter               (AParam),
  FPonderationType	   (AParam.FPonderationType),
  FPonderationCenter	   (AParam.FPonderationCenter),
  FPonderationVector	   (AParam.FPonderationVector),
  FPonderationStretchIn	   (AParam.FPonderationStretchIn),
  FPonderationStretchInMin (AParam.FPonderationStretchInMin),
  FPonderationStretchInMax (AParam.FPonderationStretchInMax),
  FPonderationFunctionType (AParam.FPonderationFunctionType),
  FPonderationStretchOut   (AParam.FPonderationStretchOut),
  FPonderationStretchOutMin(AParam.FPonderationStretchOutMin),
  FPonderationStretchOutMax(AParam.FPonderationStretchOutMax)
{}
//******************************************************************************
CParameterObjectPonderation::~CParameterObjectPonderation()
{}
//******************************************************************************
CParameter * CParameterObjectPonderation::copy() const
{ return new CParameterObjectPonderation(*this); }
//******************************************************************************
void CParameterObjectPonderation::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectPonderation::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectPonderation::reinit()
{}
//******************************************************************************
int CParameterObjectPonderation::getType() const
{
  return PARAMETER_OBJECT_PONDERATION;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationType(TPonderationType
						     APonderationType)
{
  if (FPonderationType != APonderationType)
    {
      putAllNeedToUpdate();
      FPonderationType = APonderationType;
    }
}

TPonderationType CParameterObjectPonderation::getPonderationType() const
{
  return FPonderationType;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationCenter(const CVertex &
						       APonderationCenter)
{
  if (FPonderationCenter != APonderationCenter)
    {
      putAllNeedToUpdate();
      FPonderationCenter = APonderationCenter;
    }
}

CVertex CParameterObjectPonderation::getPonderationCenter() const
{
  return FPonderationCenter;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationVector(const CVertex &
						       APonderationVector)
{
  if (FPonderationVector != APonderationVector)
    {
      putAllNeedToUpdate();
      FPonderationVector = APonderationVector;
    }
}

CVertex CParameterObjectPonderation::getPonderationVector() const
{
  return FPonderationVector;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationStretchIn(bool AActive)
{
  FPonderationStretchIn = AActive;
}

bool CParameterObjectPonderation::getPonderationStretchIn() const
{
  return FPonderationStretchIn;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationStretchInMin(float AMin)
{
  FPonderationStretchInMin = AMin;
}

float CParameterObjectPonderation::getPonderationStretchInMin() const
{
  return FPonderationStretchInMin;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationStretchInMax(float AMax)
{
  FPonderationStretchInMax = AMax;
}

float CParameterObjectPonderation::getPonderationStretchInMax() const
{
  return FPonderationStretchInMax;
}
//******************************************************************************
void CParameterObjectPonderation
::setPonderationFunctionType(TFunctionType AFunctionType)
{
  FPonderationFunctionType = AFunctionType;
}

TFunctionType
CParameterObjectPonderation::getPonderationFunctionType() const
{
  return FPonderationFunctionType;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationStretchOut(bool AActive)
{
  FPonderationStretchOut = AActive;
}

bool CParameterObjectPonderation::getPonderationStretchOut() const
{
  return FPonderationStretchOut;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationStretchOutMin(float AMin)
{
  FPonderationStretchOutMin = AMin;
}

float CParameterObjectPonderation::getPonderationStretchOutMin() const
{
  return FPonderationStretchOutMin;
}
//******************************************************************************
void CParameterObjectPonderation::setPonderationStretchOutMax(float AMax)
{
  FPonderationStretchOutMax = AMax;
}

float CParameterObjectPonderation::getPonderationStretchOutMax() const
{
  return FPonderationStretchOutMax;
}
//******************************************************************************
