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
#include "parameter-drawing.hh"
#include <cassert>
#include <cstring>
using namespace std;
//******************************************************************************
CParameterDrawing::CParameterDrawing(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterDrawing::CParameterDrawing(const CParameterDrawing & AParam) :
  CParameter   (AParam),
  FDisplayLight(AParam.FDisplayLight)
{
  setCLLight	 (AParam.getCLLight());
  setCLBackground(AParam.getCLBackground());
}
//******************************************************************************
CParameterDrawing::~CParameterDrawing()
{}
//******************************************************************************
CParameter * CParameterDrawing::copy() const
{ return new CParameterDrawing(*this); }
//******************************************************************************
void CParameterDrawing::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterDrawing::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterDrawing::reinit()
{
  setCLLight( DEFAULT_DRAWING_LIGHT_COLOR_0,
	      DEFAULT_DRAWING_LIGHT_COLOR_1,
	      DEFAULT_DRAWING_LIGHT_COLOR_2 );
  
  setCLBackground( DEFAULT_DRAWING_BACKGROUND_COLOR_0,
		   DEFAULT_DRAWING_BACKGROUND_COLOR_1,
		   DEFAULT_DRAWING_BACKGROUND_COLOR_2 );

  FDisplayLight = DEFAULT_DRAWING_DISPLAY_LIGHT;

  putAllNeedToUpdate();
}
//******************************************************************************
ostream& operator<<(ostream& AStream, const CParameterDrawing & AParameter)
{
  AStream<<"CParameterDrawing:"<<endl;
  
  AStream<<"  LightColor:      "<<AParameter.FCLLight[0]<<" "
	 <<AParameter.FCLLight[1]<<" "<<AParameter.FCLLight[2]<<endl;
  
  AStream<<"  BackgroundColor: "<<AParameter.FCLBackground[0]<<" "
	 <<AParameter.FCLBackground[1]<<" "<<AParameter.FCLBackground[2]<<endl;

  AStream<<"  DisplayLight:    "<<AParameter.FDisplayLight<<endl;
  AStream<<endl;

  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream, CParameterDrawing & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterDrawing:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LightColor:") );
  AStream>>AParameter.FCLLight[0]>>AParameter.FCLLight[1]
	 >>AParameter.FCLLight[2];

  AStream>>tmp; assert ( !strcmp(tmp, "BackgroundColor:") );
  AStream>>AParameter.FCLBackground[0]>>AParameter.FCLBackground[1]
	 >>AParameter.FCLBackground[2];

  AStream>>tmp; assert ( !strcmp(tmp, "DisplayLight:") );
  AStream>>AParameter.FDisplayLight;

  AParameter.putAllNeedToUpdate();  

  return AStream;
}
//******************************************************************************
float CParameterDrawing::getCLLight(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLLight[AIndice];
}
void CParameterDrawing::setCLLight(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  FCLLight[AIndice]= AValue;
}
const float * CParameterDrawing::getCLLight() const
{ return FCLLight; }
void CParameterDrawing::setCLLight(float AValue0, float AValue1, float AValue2)
{
  FCLLight[0]= AValue0;
  FCLLight[1]= AValue1;
  FCLLight[2]= AValue2;
}
void CParameterDrawing::setCLLight(const float ATab[3])
{ setCLLight(ATab[0],ATab[1],ATab[2]); }

//******************************************************************************
float CParameterDrawing::getCLBackground(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLBackground[AIndice];
}
void CParameterDrawing::setCLBackground(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  FCLBackground[AIndice]= AValue;
}
const float * CParameterDrawing::getCLBackground() const
{ return FCLBackground; }
void CParameterDrawing::setCLBackground(float AValue0, float AValue1, float AValue2)
{
  FCLBackground[0]= AValue0;
  FCLBackground[1]= AValue1;
  FCLBackground[2]= AValue2;
}
void CParameterDrawing::setCLBackground(const float ATab[3])
{ setCLBackground(ATab[0],ATab[1],ATab[2]); }

//******************************************************************************
bool CParameterDrawing::getDisplayLight() const
{ return FDisplayLight; }
//******************************************************************************
void CParameterDrawing::setDisplayLight(bool AValue)
{
  FDisplayLight= AValue;
}
//******************************************************************************
void CParameterDrawing::invDisplayLight()
{
  FDisplayLight= !FDisplayLight;
}
//******************************************************************************
int CParameterDrawing::getType() const
{ return PARAMETER_DRAWING; }
//******************************************************************************
