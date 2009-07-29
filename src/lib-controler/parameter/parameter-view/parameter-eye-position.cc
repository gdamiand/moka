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
#include "parameter-eye-position.hh"
#include <cassert>
#include <cstring>
using namespace std;
//******************************************************************************
CParameterEyePosition::CParameterEyePosition(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterEyePosition::
CParameterEyePosition(const CParameterEyePosition & AParam) :
  CParameter    (AParam),
  FRotationAlpha(AParam.FRotationAlpha),
  FRotationBeta	(AParam.FRotationBeta),
  FDistance	(AParam.FDistance),
  FPasAvancement (AParam.FPasAvancement),
  FAngleRotation (AParam.FAngleRotation)
{}
//******************************************************************************
CParameterEyePosition::~CParameterEyePosition()
{}
//******************************************************************************
CParameter * CParameterEyePosition::copy() const
{ return new CParameterEyePosition(*this); }
//******************************************************************************
void CParameterEyePosition::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterEyePosition::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterEyePosition::reinit()
{
  FRotationAlpha = DEFAULT_EYE_POSITION_ROTATION_ALPHA;
  FRotationBeta  = DEFAULT_EYE_POSITION_ROTATION_BETA;
  FDistance      = DEFAULT_EYE_POSITION_DISTANCE;
  FPasAvancement = DEFAULT_EYE_POSITION_PAS_AVANCEMENT;
  FAngleRotation = DEFAULT_EYE_POSITION_ANGLE_ROTATION;
  putAllNeedToUpdate();
}
//******************************************************************************
ostream& operator<<(ostream& AStream,
			    const CParameterEyePosition & AParameter)
{
  AStream<<"CParameterEyePosition:"<<endl;
  AStream<<"  RotationAlpha: "<<AParameter.FRotationAlpha<<endl
	 <<"  RotationBeta:  "<<AParameter.FRotationBeta<<endl
	 <<"  Distance:      "<<AParameter.FDistance<<endl
	 <<"  PasAvancement: "<<AParameter.FPasAvancement<<endl
	 <<"  AngleRotation: "<<AParameter.FAngleRotation<<endl;
  AStream<<endl;

  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream,
			    CParameterEyePosition & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterEyePosition:") );

  AStream>>tmp; assert ( !strcmp(tmp, "RotationAlpha:") );
  AStream>>AParameter.FRotationAlpha;

  AStream>>tmp; assert ( !strcmp(tmp, "RotationBeta:") );
  AStream>>AParameter.FRotationBeta;

  AStream>>tmp; assert ( !strcmp(tmp, "Distance:") );
  AStream>>AParameter.FDistance;

  AStream>>tmp; assert( !strcmp(tmp, "PasAvancement:") );
  AStream>>AParameter.FPasAvancement;
  
  AStream>>tmp; assert( !strcmp(tmp, "AngleRotation:") );
  AStream>>AParameter.FAngleRotation;

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//******************************************************************************
float CParameterEyePosition::getRotationAlpha() const
{ return FRotationAlpha; }
//******************************************************************************
void CParameterEyePosition::setRotationAlpha(float ARotation)
{ FRotationAlpha= ARotation; }
//******************************************************************************
float CParameterEyePosition::getRotationBeta() const
{ return FRotationBeta; }
//******************************************************************************
void CParameterEyePosition::setRotationBeta(float ARotation)
{ FRotationBeta= ARotation; }
//******************************************************************************
float CParameterEyePosition::getDistance() const
{ return FDistance; }
//******************************************************************************
void CParameterEyePosition::setDistance(float ADistance)
{ FDistance= ADistance; }
//******************************************************************************
void CParameterEyePosition::incRotationAlpha(float ADelta)
{ FRotationAlpha+= ADelta; }
//******************************************************************************
void CParameterEyePosition::incRotationBeta(float ADelta)
{ FRotationBeta+= ADelta; }
//******************************************************************************
void CParameterEyePosition::incDistance(float ADelta)
{ FDistance+= ADelta; }
//******************************************************************************
int CParameterEyePosition::getType() const
{ return PARAMETER_EYE_POSITION; }
//******************************************************************************
float CParameterEyePosition::getPasAvancement() const
{ return FPasAvancement; }
//******************************************************************************
void CParameterEyePosition::setPasAvancement(float pas)
{ FPasAvancement = pas; }
//******************************************************************************
float CParameterEyePosition::getAngleRotation() const
{ return FAngleRotation; }
//******************************************************************************
void CParameterEyePosition::setAngleRotation(float angle)
{ FAngleRotation = angle; }
//******************************************************************************
