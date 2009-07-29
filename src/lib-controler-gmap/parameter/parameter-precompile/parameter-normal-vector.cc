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
#include "parameter-normal-vector.hh"
#include <cassert>
#include <cstring>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterNormalVector::CParameterNormalVector(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterNormalVector::
CParameterNormalVector(const CParameterNormalVector & AParam) :
  CParameter     (AParam),
  FAll		 (AParam.FAll),
  FLWNormalVector(AParam.FLWNormalVector),
  FLGNormalVector(AParam.FLGNormalVector)
{
  setCLNormalVector(AParam.getCLNormalVector());
}
//******************************************************************************
CParameterNormalVector::~CParameterNormalVector()
{}
//******************************************************************************
CParameter * CParameterNormalVector::copy() const
{ return new CParameterNormalVector(*this); }
//******************************************************************************
void CParameterNormalVector::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterNormalVector::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterNormalVector::reinit()
{
  FAll		  = DEFAULT_NORMAL_VECTOR_ALL;
  FLWNormalVector = DEFAULT_NORMAL_VECTOR_LINE_WIDTH;
  FLGNormalVector = DEFAULT_NORMAL_VECTOR_LENGTH;
  setCLNormalVector(DEFAULT_NORMAL_VECTOR_COLOR_0,
		    DEFAULT_NORMAL_VECTOR_COLOR_1,
		    DEFAULT_NORMAL_VECTOR_COLOR_2);

  putAllNeedToUpdate();
}
//******************************************************************************
namespace GMap3d
{
//------------------------------------------------------------------------------
ostream& operator<<(ostream& AStream, const CParameterNormalVector & AParameter)
{
  AStream<<"CParameterNormalVector:"<<endl;
  
  AStream<<"  DrawAll:   "<<AParameter.FAll<<endl;

  AStream<<"  LineWidth: "<<AParameter.FLWNormalVector<<endl;
  
  AStream<<"  Length:    "<<AParameter.FLGNormalVector<<endl;
  
  AStream<<"  Color:     "<<AParameter.FCLNormalVector[0]<<" "
	 <<AParameter.FCLNormalVector[1]<<" "<<AParameter.FCLNormalVector[2]<<endl;
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream, CParameterNormalVector & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterNormalVector:") );

  AStream>>tmp; assert ( !strcmp(tmp, "DrawAll:") );
  AStream>>AParameter.FAll;

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth:") );
  AStream>>AParameter.FLWNormalVector;

  AStream>>tmp; assert ( !strcmp(tmp, "Length:") );
  AStream>>AParameter.FLGNormalVector;

  AStream>>tmp; assert ( !strcmp(tmp, "Color:") );
  AStream>>AParameter.FCLNormalVector[0]>>AParameter.FCLNormalVector[1]
	 >>AParameter.FCLNormalVector[2];

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//------------------------------------------------------------------------------
} // namespace GMap3d
//******************************************************************************
bool CParameterNormalVector::getShowAllNormals() const
{ return FAll; }
//******************************************************************************
void CParameterNormalVector::setShowAllNormals(bool AShow)
{
  if (FAll != AShow)
    {
      putAllNeedToUpdate();
      FAll = AShow;
    }
}
//******************************************************************************
int CParameterNormalVector::getLWNormalVector() const
{ return FLWNormalVector; }
void CParameterNormalVector::setLWNormalVector(int AValue)
{
  if ( FLWNormalVector!=AValue )
    {
      putAllNeedToUpdate();
      FLWNormalVector= AValue;
    }
}
//******************************************************************************
float CParameterNormalVector::getLGNormalVector() const
{ return FLGNormalVector; }
void CParameterNormalVector::setLGNormalVector(float AValue)
{
  if ( FLGNormalVector!=AValue )
    {
      putAllNeedToUpdate();
      FLGNormalVector= AValue;
    }
}
//******************************************************************************
float CParameterNormalVector::getCLNormalVector(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLNormalVector[AIndice];
}
void CParameterNormalVector::setCLNormalVector(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLNormalVector[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLNormalVector[AIndice]= AValue;
    }
}
const float * CParameterNormalVector::getCLNormalVector() const
{ return FCLNormalVector; }
void CParameterNormalVector::setCLNormalVector(float AValue0, float AValue1,
					       float AValue2)
{
  if ( FCLNormalVector[0]!=AValue0 ||
       FCLNormalVector[1]!=AValue1 ||
       FCLNormalVector[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLNormalVector[0]= AValue0;
      FCLNormalVector[1]= AValue1;
      FCLNormalVector[2]= AValue2;
    }
}
void CParameterNormalVector::setCLNormalVector(const float ATab[3])
{ setCLNormalVector(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
int CParameterNormalVector::getType() const
{ return PARAMETER_NORMAL_VECTOR; }
//******************************************************************************
