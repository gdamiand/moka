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
#include "parameter-axis.hh"
#include <cassert>
#include <cstring>
using namespace std;
//******************************************************************************
CParameterAxis::CParameterAxis(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterAxis::CParameterAxis(const CParameterAxis & AParam) :
  CParameter(AParam),
  FLWAxis   (AParam.FLWAxis),
  FLGAxis   (AParam.FLGAxis)
{
  setCLAxisX(AParam.getCLAxisX());
  setCLAxisY(AParam.getCLAxisY());
  setCLAxisZ(AParam.getCLAxisZ());
}
//******************************************************************************
CParameterAxis::~CParameterAxis()
{}
//******************************************************************************
CParameter * CParameterAxis::copy() const
{ return new CParameterAxis(*this); }
//******************************************************************************
void CParameterAxis::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterAxis::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterAxis::reinit()
{
  FLWAxis = DEFAULT_AXIS_LINE_WIDTH;
  FLGAxis = DEFAULT_AXIS_LENGTH;

  setCLAxisX(DEFAULT_AXIS_COLOR_X_0,
	     DEFAULT_AXIS_COLOR_X_1,
	     DEFAULT_AXIS_COLOR_X_2);

  setCLAxisY(DEFAULT_AXIS_COLOR_Y_0,
	     DEFAULT_AXIS_COLOR_Y_1,
	     DEFAULT_AXIS_COLOR_Y_2);

  setCLAxisZ(DEFAULT_AXIS_COLOR_Z_0,
	     DEFAULT_AXIS_COLOR_Z_1,
	     DEFAULT_AXIS_COLOR_Z_2);

  putAllNeedToUpdate();  
}
//******************************************************************************
ostream& operator<<(ostream& AStream, const CParameterAxis & AParameter)
{
  AStream<<"CParameterAxis:"<<endl;

  AStream<<"  LineWidth: "<<AParameter.FLWAxis<<endl;

  AStream<<"  Length:    "<<AParameter.FLGAxis<<endl;
    
  AStream<<"  ColorX:    "<<AParameter.FCLAxisX[0]<<" "
	 <<AParameter.FCLAxisX[1]<<" "<<AParameter.FCLAxisX[2]<<endl;

  AStream<<"  ColorY:    "<<AParameter.FCLAxisY[0]<<" "
	 <<AParameter.FCLAxisY[1]<<" "<<AParameter.FCLAxisY[2]<<endl;

  AStream<<"  ColorZ:    "<<AParameter.FCLAxisZ[0]<<" "
	 <<AParameter.FCLAxisZ[1]<<" "<<AParameter.FCLAxisZ[2]<<endl;

  AStream<<endl;

  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream,
			    CParameterAxis & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterAxis:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth:") );
  AStream>>AParameter.FLWAxis;

  AStream>>tmp; assert ( !strcmp(tmp, "Length:") );
  AStream>>AParameter.FLGAxis;

  AStream>>tmp; assert ( !strcmp(tmp, "ColorX:") );
  AStream>>AParameter.FCLAxisX[0]>>AParameter.FCLAxisX[1]
	 >>AParameter.FCLAxisX[2];

  AStream>>tmp; assert ( !strcmp(tmp, "ColorY:") );
  AStream>>AParameter.FCLAxisY[0]>>AParameter.FCLAxisY[1]
	 >>AParameter.FCLAxisY[2];

  AStream>>tmp; assert ( !strcmp(tmp, "ColorZ:") );
  AStream>>AParameter.FCLAxisZ[0]>>AParameter.FCLAxisZ[1]
	 >>AParameter.FCLAxisZ[2];

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//******************************************************************************
int CParameterAxis::getLWAxis() const
{ return FLWAxis; }
void CParameterAxis::setLWAxis(int AValue)
{
  if ( FLWAxis!=AValue )
    {
      putAllNeedToUpdate();
      FLWAxis= AValue;
    }
}
//******************************************************************************
float CParameterAxis::getLGAxis() const
{ return FLGAxis; }
void CParameterAxis::setLGAxis(float AValue)
{
  if ( FLGAxis!=AValue )
    {
      putAllNeedToUpdate();
      FLGAxis= AValue;
    }
}
//******************************************************************************
float CParameterAxis::getCLAxisX(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLAxisX[AIndice];
}
void CParameterAxis::setCLAxisX(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLAxisX[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLAxisX[AIndice]= AValue;
    }
}
const float * CParameterAxis::getCLAxisX() const
{ return FCLAxisX; }
void CParameterAxis::setCLAxisX(float AValue0, float AValue1, float AValue2)
{
  if ( FCLAxisX[0]!=AValue0 ||
       FCLAxisX[1]!=AValue1 ||
       FCLAxisX[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLAxisX[0]= AValue0;
      FCLAxisX[1]= AValue1;
      FCLAxisX[2]= AValue2;
    }
}
void CParameterAxis::setCLAxisX(const float ATab[3])
{ setCLAxisX(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterAxis::getCLAxisY(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLAxisY[AIndice];
}
void CParameterAxis::setCLAxisY(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLAxisY[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLAxisY[AIndice]= AValue;
    }
}
const float * CParameterAxis::getCLAxisY() const
{ return FCLAxisY; }
void CParameterAxis::setCLAxisY(float AValue0, float AValue1, float AValue2)
{
  if ( FCLAxisY[0]!=AValue0 ||
       FCLAxisY[1]!=AValue1 ||
       FCLAxisY[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLAxisY[0]= AValue0;
      FCLAxisY[1]= AValue1;
      FCLAxisY[2]= AValue2;
    }
}
void CParameterAxis::setCLAxisY(const float ATab[3])
{ setCLAxisY(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterAxis::getCLAxisZ(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLAxisZ[AIndice];
}
void CParameterAxis::setCLAxisZ(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLAxisZ[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLAxisZ[AIndice]= AValue;
    }
}
const float * CParameterAxis::getCLAxisZ() const
{ return FCLAxisZ; }
void CParameterAxis::setCLAxisZ(float AValue0, float AValue1, float AValue2)
{
  if ( FCLAxisZ[0]!=AValue0 ||
       FCLAxisZ[1]!=AValue1 ||
       FCLAxisZ[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLAxisZ[0]= AValue0;
      FCLAxisZ[1]= AValue1;
      FCLAxisZ[2]= AValue2;
    }
}
void CParameterAxis::setCLAxisZ(const float ATab[3])
{ setCLAxisZ(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
int CParameterAxis::getType() const
{ return PARAMETER_AXIS; }
//******************************************************************************
