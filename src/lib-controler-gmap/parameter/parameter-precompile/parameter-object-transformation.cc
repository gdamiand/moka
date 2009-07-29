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
#include "parameter-object-transformation.hh"
#include <cassert>
#include <cstring>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterObjectTransformation::CParameterObjectTransformation(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterObjectTransformation::
CParameterObjectTransformation(const CParameterObjectTransformation & AParam) :
  CParameter             (AParam),
  FLWObjectTransformation(AParam.FLWObjectTransformation)
{
  setCLObjectTransformation(AParam.getCLObjectTransformation());
}
//******************************************************************************
CParameterObjectTransformation::~CParameterObjectTransformation()
{}
//******************************************************************************
CParameter * CParameterObjectTransformation::copy() const
{ return new CParameterObjectTransformation(*this); }
//******************************************************************************
void CParameterObjectTransformation::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterObjectTransformation::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterObjectTransformation::reinit()
{
  FLWObjectTransformation = DEFAULT_OBJECT_TRANSFORMATION_LINE_WIDTH;
  setCLObjectTransformation(DEFAULT_OBJECT_TRANSFORMATION_COLOR_0,
			    DEFAULT_OBJECT_TRANSFORMATION_COLOR_1,
			    DEFAULT_OBJECT_TRANSFORMATION_COLOR_2);

  putAllNeedToUpdate();
}
//******************************************************************************
namespace GMap3d
{
//------------------------------------------------------------------------------
ostream& operator<<(ostream& AStream,
		    const CParameterObjectTransformation & AParameter)
{
  AStream<<"CParameterObjectTransformation:"<<endl;
  
  AStream<<"  LineWidth: "<<AParameter.FLWObjectTransformation<<endl;
  
  AStream<<"  Color:     "<<AParameter.FCLObjectTransformation[0]<<" "
	 <<AParameter.FCLObjectTransformation[1]<<" "
	 <<AParameter.FCLObjectTransformation[2]<<endl;
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream,
		    CParameterObjectTransformation & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterObjectTransformation:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth:") );
  AStream>>AParameter.FLWObjectTransformation;

  AStream>>tmp; assert ( !strcmp(tmp, "Color:") );
  AStream>>AParameter.FCLObjectTransformation[0]
	 >>AParameter.FCLObjectTransformation[1]
	 >>AParameter.FCLObjectTransformation[2];

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//------------------------------------------------------------------------------
} // namespace GMap3d
//******************************************************************************
int CParameterObjectTransformation::getLWObjectTransformation() const
{ return FLWObjectTransformation; }
void CParameterObjectTransformation::setLWObjectTransformation(int AValue)
{
  if ( FLWObjectTransformation!=AValue )
    {
      putAllNeedToUpdate();
      FLWObjectTransformation= AValue;
    }
}
//******************************************************************************
float CParameterObjectTransformation::getCLObjectTransformation(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLObjectTransformation[AIndice];
}
void CParameterObjectTransformation::setCLObjectTransformation(int AIndice,
							       float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLObjectTransformation[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLObjectTransformation[AIndice]= AValue;
    }
}
const float * CParameterObjectTransformation::getCLObjectTransformation() const
{ return FCLObjectTransformation; }
void CParameterObjectTransformation::setCLObjectTransformation(float AValue0,
							       float AValue1,
							       float AValue2)
{
  if ( FCLObjectTransformation[0]!=AValue0 ||
       FCLObjectTransformation[1]!=AValue1 ||
       FCLObjectTransformation[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLObjectTransformation[0]= AValue0;
      FCLObjectTransformation[1]= AValue1;
      FCLObjectTransformation[2]= AValue2;
    }
}
void CParameterObjectTransformation::
setCLObjectTransformation(const float ATab[3])
{ setCLObjectTransformation(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
int CParameterObjectTransformation::getType() const
{ return PARAMETER_OBJECT_TRANSFORMATION; }
//******************************************************************************
