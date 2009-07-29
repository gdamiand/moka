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
#include "parameter-polyline-position.hh"
#include "vertex.hh"
#include "controler-gmap-types.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterPolylinePosition::CParameterPolylinePosition(int ANbRef) :
  CParameter(ANbRef)
{}
//******************************************************************************
CParameterPolylinePosition::
CParameterPolylinePosition(const CParameterPolylinePosition & AParam) :
  CParameter (AParam),
  FVertexList(AParam.FVertexList)
{}
//******************************************************************************
CParameterPolylinePosition::~CParameterPolylinePosition()
{}
//******************************************************************************
CParameter * CParameterPolylinePosition::copy() const
{ return new CParameterPolylinePosition(*this); }
//******************************************************************************
void CParameterPolylinePosition::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterPolylinePosition::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterPolylinePosition::reinit()
{
  if ( getNbVertices()>0 )
    {
      FVertexList.clear();
      putAllNeedToUpdate();
    }
}
//******************************************************************************
unsigned int CParameterPolylinePosition::getNbVertices() const
{ return FVertexList.size(); }
//******************************************************************************
const CVertex & CParameterPolylinePosition::getVertex(unsigned int AIndex) const
{
  assert( AIndex<getNbVertices() );
  return FVertexList[AIndex];
}
//******************************************************************************
const CVertex & CParameterPolylinePosition::getLastVertex() const
{
  assert( getNbVertices()>0 );
  return FVertexList[FVertexList.size()-1];
}
//******************************************************************************
const vector<CVertex> & CParameterPolylinePosition::getVertices() const
{ return FVertexList; }
//******************************************************************************
void CParameterPolylinePosition::removeLastVertex()
{
  if ( getNbVertices()>0 )
    {
      FVertexList.pop_back();
      putAllNeedToUpdate();
    }
}
//******************************************************************************
void CParameterPolylinePosition::addVertex(const CVertex & AVertex)
{
  FVertexList.push_back(AVertex);
  putAllNeedToUpdate();
}
//******************************************************************************
void CParameterPolylinePosition::setVertex(unsigned int AIndex,
					   const CVertex & AVertex)
{
  assert( AIndex<getNbVertices() );
  if ( FVertexList[AIndex]!=AVertex )
    {
      FVertexList[AIndex] = AVertex;
      putAllNeedToUpdate();
    }
}
//******************************************************************************
void CParameterPolylinePosition::setLastVertex(const CVertex & AVertex)
{
  assert( getNbVertices()>0 );
  setVertex(FVertexList.size()-1,AVertex);
}
//******************************************************************************
int CParameterPolylinePosition::getType() const
{ return PARAMETER_POLYLINE_POSITION; }
//******************************************************************************
