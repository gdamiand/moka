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
#include "parameter-vertex.hh"
#include <cassert>
#include <cstring>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterVertex::CParameterVertex(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterVertex::CParameterVertex(const CParameterVertex & AParam) :
  CParameter(AParam),
  FSVertex  (AParam.FSVertex)
{
  setCLVertex(AParam.getCLVertex());
}
//******************************************************************************
CParameterVertex::~CParameterVertex()
{}
//******************************************************************************
CParameter * CParameterVertex::copy() const
{ return new CParameterVertex(*this); }
//******************************************************************************
void CParameterVertex::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterVertex::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterVertex::reinit()
{
  FSVertex = DEFAULT_VERTEX_SIZE;
  setCLVertex(DEFAULT_VERTEX_COUL0, DEFAULT_VERTEX_COUL1, DEFAULT_VERTEX_COUL2);
  
  putAllNeedToUpdate();
}
//******************************************************************************
namespace GMap3d
{
//------------------------------------------------------------------------------
ostream& operator<<(ostream& AStream, const CParameterVertex & AParameter)
{
  AStream<<"CParameterVertex:"<<endl;
  
  AStream<<"  Size:  "<<AParameter.FSVertex<<endl;
  
  AStream<<"  Color: "<<AParameter.FCLVertex[0]<<" "
	 <<AParameter.FCLVertex[1]<<" "<<AParameter.FCLVertex[2]<<endl;
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream, CParameterVertex & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterVertex:") );

  AStream>>tmp; assert ( !strcmp(tmp, "Size:") );
  AStream>>AParameter.FSVertex;

  AStream>>tmp; assert ( !strcmp(tmp, "Color:") );
  AStream>>AParameter.FCLVertex[0]>>AParameter.FCLVertex[1]
	 >>AParameter.FCLVertex[2];

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//------------------------------------------------------------------------------
} // namespace GMap3d
//******************************************************************************
int CParameterVertex::getSVertex() const
{ return FSVertex; }
void CParameterVertex::setSVertex(int AValue)
{
  if ( FSVertex!=AValue )
    {
      putAllNeedToUpdate();
      FSVertex= AValue;
    }
}
//******************************************************************************
float CParameterVertex::getCLVertex(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLVertex[AIndice];
}
void CParameterVertex::setCLVertex(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLVertex[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLVertex[AIndice]= AValue;
    }
}
const float * CParameterVertex::getCLVertex() const
{ return FCLVertex; }
void CParameterVertex::setCLVertex(float AValue0, float AValue1, float AValue2)
{
  if ( FCLVertex[0]!=AValue0 ||
       FCLVertex[1]!=AValue1 ||
       FCLVertex[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLVertex[0]= AValue0;
      FCLVertex[1]= AValue1;
      FCLVertex[2]= AValue2;
    }
}
void CParameterVertex::setCLVertex(const float ATab[3])
{ setCLVertex(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
int CParameterVertex::getType() const
{ return PARAMETER_VERTEX; }
//******************************************************************************
