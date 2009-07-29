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
#include "parameter-aimed-point.hh"
#include <cassert>
#include <cstring>
using namespace std;
//******************************************************************************
CParameterAimedPoint::CParameterAimedPoint(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterAimedPoint::CParameterAimedPoint(const CParameterAimedPoint & AParam):
  CParameter   (AParam),
  FLWAimedPoint(AParam.FLWAimedPoint),
  FLGAimedPoint(AParam.FLGAimedPoint)
{
  setCLAimedPoint(AParam.getCLAimedPoint());
}
//******************************************************************************
CParameterAimedPoint::~CParameterAimedPoint()
{}
//******************************************************************************
CParameter * CParameterAimedPoint::copy() const
{ return new CParameterAimedPoint(*this); }
//******************************************************************************
void CParameterAimedPoint::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterAimedPoint::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterAimedPoint::reinit()
{
  FLWAimedPoint = DEFAULT_AIMED_POINT_LINE_WIDTH;
  FLGAimedPoint = DEFAULT_AIMED_POINT_LENGTH;
  setCLAimedPoint(DEFAULT_AIMED_POINT_COLOR_0, DEFAULT_AIMED_POINT_COLOR_1,
		  DEFAULT_AIMED_POINT_COLOR_2);

  putAllNeedToUpdate();
}
//******************************************************************************
ostream& operator<<(ostream& AStream,
			    const CParameterAimedPoint & AParameter)
{
  AStream<<"CParameterAimedPoint:"<<endl;
  
  AStream<<"  LineWidth: "<<AParameter.FLWAimedPoint<<endl;
  
  AStream<<"  Length:    "<<AParameter.FLGAimedPoint<<endl;
  
  AStream<<"  Color:     "<<AParameter.FCLAimedPoint[0]<<" "
	 <<AParameter.FCLAimedPoint[1]<<" "<<AParameter.FCLAimedPoint[2]<<endl;
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream,
			    CParameterAimedPoint & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterAimedPoint:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth:") );
  AStream>>AParameter.FLWAimedPoint;

  AStream>>tmp; assert ( !strcmp(tmp, "Length:") );
  AStream>>AParameter.FLGAimedPoint;

  AStream>>tmp; assert ( !strcmp(tmp, "Color:") );
  AStream>>AParameter.FCLAimedPoint[0]>>AParameter.FCLAimedPoint[1]
	 >>AParameter.FCLAimedPoint[2];

  AParameter.putAllNeedToUpdate();

  return AStream;
}
//******************************************************************************
int CParameterAimedPoint::getLWAimedPoint() const
{ return FLWAimedPoint; }
void CParameterAimedPoint::setLWAimedPoint(int AValue)
{
  if ( FLWAimedPoint!=AValue )
    {
      putAllNeedToUpdate();
      FLWAimedPoint= AValue;
    }
}
//******************************************************************************
float CParameterAimedPoint::getLGAimedPoint() const
{ return FLGAimedPoint; }
void CParameterAimedPoint::setLGAimedPoint(float AValue)
{
  if ( FLGAimedPoint!=AValue )
    {
      putAllNeedToUpdate();
      FLGAimedPoint= AValue;
    }
}
//******************************************************************************
float CParameterAimedPoint::getCLAimedPoint(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLAimedPoint[AIndice];
}
void CParameterAimedPoint::setCLAimedPoint(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLAimedPoint[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLAimedPoint[AIndice]= AValue;
    }
}
const float * CParameterAimedPoint::getCLAimedPoint() const
{ return FCLAimedPoint; }
void CParameterAimedPoint::setCLAimedPoint(float AValue0, float AValue1, float AValue2)
{
  if ( FCLAimedPoint[0]!=AValue0 ||
       FCLAimedPoint[1]!=AValue1 ||
       FCLAimedPoint[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLAimedPoint[0]= AValue0;
      FCLAimedPoint[1]= AValue1;
      FCLAimedPoint[2]= AValue2;
    }
}
void CParameterAimedPoint::setCLAimedPoint(const float ATab[3])
{ setCLAimedPoint(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
int CParameterAimedPoint::getType() const
{ return PARAMETER_AIMED_POINT; }
//******************************************************************************
