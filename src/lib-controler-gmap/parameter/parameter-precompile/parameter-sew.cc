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
#include "parameters-gmap-default-values.hh"
#include "parameter-sew.hh"
#include <cassert>
#include <cstring>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterSew::CParameterSew(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterSew::CParameterSew(const CParameterSew & AParam) :
  CParameter(AParam)
{
  for (int i=0; i<4; ++i)
    {
      FLWSew[i]	       = AParam.FLWSew[i];
      FSewPosCoef[i]   = AParam.FSewPosCoef[i];
      FDisplayAlpha[i] = AParam.FDisplayAlpha[i];
      setCLSew(i, AParam.getCLSew(i));
    }
}
//******************************************************************************
CParameterSew::~CParameterSew()
{}
//******************************************************************************
CParameter * CParameterSew::copy() const
{ return new CParameterSew(*this); }
//******************************************************************************
void CParameterSew::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterSew::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterSew::reinit()
{
  FLWSew[0] = DEFAULT_SEW0_LINE_WIDTH;
  FLWSew[1] = DEFAULT_SEW1_LINE_WIDTH;
  FLWSew[2] = DEFAULT_SEW2_LINE_WIDTH;
  FLWSew[3] = DEFAULT_SEW3_LINE_WIDTH;

  FSewPosCoef[0] = DEFAULT_SEW0_POSITION;
  FSewPosCoef[1] = DEFAULT_SEW1_POSITION;
  FSewPosCoef[2] = DEFAULT_SEW2_POSITION;
  FSewPosCoef[3] = DEFAULT_SEW3_POSITION;

  FDisplayAlpha[0] = DEFAULT_SEW0_DISPLAY;
  FDisplayAlpha[1] = DEFAULT_SEW1_DISPLAY;
  FDisplayAlpha[2] = DEFAULT_SEW2_DISPLAY;
  FDisplayAlpha[3] = DEFAULT_SEW3_DISPLAY;

  setCLSew(0, DEFAULT_SEW0_COLOR_0, DEFAULT_SEW0_COLOR_1, DEFAULT_SEW0_COLOR_2);
  setCLSew(1, DEFAULT_SEW1_COLOR_0, DEFAULT_SEW1_COLOR_1, DEFAULT_SEW1_COLOR_2);
  setCLSew(2, DEFAULT_SEW2_COLOR_0, DEFAULT_SEW2_COLOR_1, DEFAULT_SEW2_COLOR_2);
  setCLSew(3, DEFAULT_SEW3_COLOR_0, DEFAULT_SEW3_COLOR_1, DEFAULT_SEW3_COLOR_2);

  putAllNeedToUpdate();
}
//******************************************************************************
namespace GMap3d
{
//------------------------------------------------------------------------------
ostream& operator<<(ostream& AStream, const CParameterSew & AParameter)
{
  AStream<<"CParameterSew:"<<endl;
  
  AStream<<"  LineWidth0: "<<AParameter.FLWSew[0]<<endl;
  AStream<<"  LineWidth1: "<<AParameter.FLWSew[1]<<endl;
  AStream<<"  LineWidth2: "<<AParameter.FLWSew[2]<<endl;
  AStream<<"  LineWidth3: "<<AParameter.FLWSew[3]<<endl;
  
  AStream<<"  PosCoef0:   "<<AParameter.FSewPosCoef[0]<<endl;
  AStream<<"  PosCoef1:   "<<AParameter.FSewPosCoef[1]<<endl;
  AStream<<"  PosCoef2:   "<<AParameter.FSewPosCoef[2]<<endl;
  AStream<<"  PosCoef3:   "<<AParameter.FSewPosCoef[3]<<endl;

  AStream<<"  Display0:   "<<AParameter.FDisplayAlpha[0]<<endl;
  AStream<<"  Display1:   "<<AParameter.FDisplayAlpha[1]<<endl;
  AStream<<"  Display2:   "<<AParameter.FDisplayAlpha[2]<<endl;
  AStream<<"  Display3:   "<<AParameter.FDisplayAlpha[3]<<endl;
    
  AStream<<"  Color0:     "<<AParameter.FCLSew[0][0]<<" "
	 <<AParameter.FCLSew[0][1]<<" "<<AParameter.FCLSew[0][2]<<endl;
  AStream<<"  Color1:     "<<AParameter.FCLSew[1][0]<<" "
	 <<AParameter.FCLSew[1][1]<<" "<<AParameter.FCLSew[1][2]<<endl;
  AStream<<"  Color2:     "<<AParameter.FCLSew[2][0]<<" "
	 <<AParameter.FCLSew[2][1]<<" "<<AParameter.FCLSew[2][2]<<endl;
  AStream<<"  Color3:     "<<AParameter.FCLSew[3][0]<<" "
	 <<AParameter.FCLSew[3][1]<<" "<<AParameter.FCLSew[3][2]<<endl;
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream, CParameterSew & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterSew:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth0:") );
  AStream>>AParameter.FLWSew[0];
  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth1:") );
  AStream>>AParameter.FLWSew[1];
  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth2:") );
  AStream>>AParameter.FLWSew[2];
  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth3:") );
  AStream>>AParameter.FLWSew[3];

  AStream>>tmp; assert ( !strcmp(tmp, "PosCoef0:") );
  AStream>>AParameter.FSewPosCoef[0];
  AStream>>tmp; assert ( !strcmp(tmp, "PosCoef1:") );
  AStream>>AParameter.FSewPosCoef[1];
  AStream>>tmp; assert ( !strcmp(tmp, "PosCoef2:") );
  AStream>>AParameter.FSewPosCoef[2];
  AStream>>tmp; assert ( !strcmp(tmp, "PosCoef3:") );
  AStream>>AParameter.FSewPosCoef[3];

  AStream>>tmp; assert ( !strcmp(tmp, "Display0:") );
  AStream>>AParameter.FDisplayAlpha[0];
  AStream>>tmp; assert ( !strcmp(tmp, "Display1:") );
  AStream>>AParameter.FDisplayAlpha[1];
  AStream>>tmp; assert ( !strcmp(tmp, "Display2:") );
  AStream>>AParameter.FDisplayAlpha[2];
  AStream>>tmp; assert ( !strcmp(tmp, "Display3:") );
  AStream>>AParameter.FDisplayAlpha[3];

  AStream>>tmp; assert ( !strcmp(tmp, "Color0:") );
  AStream>>AParameter.FCLSew[0][0]>>AParameter.FCLSew[0][1]
	 >>AParameter.FCLSew[0][2];
  AStream>>tmp; assert ( !strcmp(tmp, "Color1:") );
  AStream>>AParameter.FCLSew[1][0]>>AParameter.FCLSew[1][1]
	 >>AParameter.FCLSew[1][2];
  AStream>>tmp; assert ( !strcmp(tmp, "Color2:") );
  AStream>>AParameter.FCLSew[2][0]>>AParameter.FCLSew[2][1]
	 >>AParameter.FCLSew[2][2];
  AStream>>tmp; assert ( !strcmp(tmp, "Color3:") );
  AStream>>AParameter.FCLSew[3][0]>>AParameter.FCLSew[3][1]
	 >>AParameter.FCLSew[3][2];

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//------------------------------------------------------------------------------
} // namespace GMap3d
//******************************************************************************
int CParameterSew::getLWSew(int ADimension) const
{
  assert(0<=ADimension && ADimension<=3);
  return FLWSew[ADimension];
}
void CParameterSew::setLWSew(int ADimension, int AValue)
{
  assert(0<=ADimension && ADimension<=3);
  if (FLWSew[ADimension]!=AValue)
    {
      putAllNeedToUpdate();
      FLWSew[ADimension]= AValue;
    }
}
//******************************************************************************
float CParameterSew::getSewPosCoef(int ADimension) const
{
  assert(0<=ADimension && ADimension<=3);
  return FSewPosCoef[ADimension];
}
void CParameterSew::setSewPosCoef(int ADimension, float AValue)
{
  assert(0<=ADimension && ADimension<=3);
  if (FSewPosCoef[ADimension]!=AValue)
    {
      putAllNeedToUpdate();
      FSewPosCoef[ADimension]= AValue;
    }
}
//******************************************************************************
float CParameterSew::getCLSew(int ADimension, int AIndice) const
{
  assert(0<=ADimension && ADimension<=3);
  assert(0<=AIndice && AIndice<=2);
  return FCLSew[ADimension][AIndice];
}
void CParameterSew::setCLSew(int ADimension, int AIndice, float AValue)
{
  assert(0<=ADimension && ADimension<=3);
  assert(0<=AIndice && AIndice<=2);
  if (FCLSew[ADimension][AIndice]!=AValue)
    {
      putAllNeedToUpdate();
      FCLSew[ADimension][AIndice]= AValue;
    }
}
const float * CParameterSew::getCLSew(int ADimension) const
{
  assert(0<=ADimension && ADimension<=3);
  return FCLSew[ADimension];
}
void CParameterSew::setCLSew(int ADimension, float AValue0,
			     float AValue1, float AValue2)
{
  assert(0<=ADimension && ADimension<=3);
  if ( FCLSew[ADimension][0]!=AValue0 ||
       FCLSew[ADimension][1]!=AValue1 ||
       FCLSew[ADimension][2]!=AValue2 )
  {
    putAllNeedToUpdate();
    FCLSew[ADimension][0]= AValue0;
    FCLSew[ADimension][1]= AValue1;
    FCLSew[ADimension][2]= AValue2;
  }
}
void CParameterSew::setCLSew(int ADimension,const float ATab[3])
{ setCLSew(ADimension,ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
bool CParameterSew::getDisplayAlpha(int AIndex) const
{
  assert(0<=AIndex && AIndex<=3);
  return FDisplayAlpha[AIndex];
}
//******************************************************************************
void CParameterSew::setDisplayAlpha(int AIndex,bool AValue)
{
  assert(0<=AIndex && AIndex<=3);
  if ( FDisplayAlpha[AIndex]!=AValue)
    {
      putAllNeedToUpdate();
      FDisplayAlpha[AIndex]= AValue;
    }
}
//******************************************************************************
void CParameterSew::invDisplayAlpha(int AIndex)
{
  assert(0<=AIndex && AIndex<=3);
  putAllNeedToUpdate();
  FDisplayAlpha[AIndex]= !FDisplayAlpha[AIndex];
}
//******************************************************************************
int CParameterSew::getType() const
{ return PARAMETER_SEW; }
//******************************************************************************
