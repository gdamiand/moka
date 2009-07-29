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
#include "parameter-preview.hh"
#include <cassert>
#include <cstring>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterPreview::CParameterPreview(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterPreview::
CParameterPreview(const CParameterPreview & AParam) :
  CParameter     (AParam),
  FLWPreview     (AParam.FLWPreview),
  FLWPreviewPoint(AParam.FLWPreviewPoint)
{
  setCLPreview   (AParam.getCLPreview());
  setCLPreviewBad(AParam.getCLPreviewBad());
}
//******************************************************************************
CParameterPreview::~CParameterPreview()
{}
//******************************************************************************
CParameter * CParameterPreview::copy() const
{ return new CParameterPreview(*this); }
//******************************************************************************
void CParameterPreview::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterPreview::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterPreview::reinit()
{
  FLWPreview	  = DEFAULT_PREVIEW_LINE_WIDTH;
  FLWPreviewPoint = DEFAULT_PREVIEW_LINE_WIDTH_POINT;
  setCLPreview   (DEFAULT_PREVIEW_COLOR_0, DEFAULT_PREVIEW_COLOR_1,
		  DEFAULT_PREVIEW_COLOR_2);
  setCLPreviewBad(DEFAULT_PREVIEW_COLOR_BAD_0, DEFAULT_PREVIEW_COLOR_BAD_1,
		  DEFAULT_PREVIEW_COLOR_BAD_2);

  putAllNeedToUpdate();
}
//******************************************************************************
namespace GMap3d
{
//------------------------------------------------------------------------------
ostream& operator<<(ostream& AStream, const CParameterPreview & AParameter)
{
  AStream<<"CParameterPreview:"<<endl;
  
  AStream<<"  LineWidth:      "<<AParameter.FLWPreview<<endl;
  
  AStream<<"  PointLineWidth: "<<AParameter.FLWPreviewPoint<<endl;
  
  AStream<<"  Color:          "<<AParameter.FCLPreview[0]<<" "
	 <<AParameter.FCLPreview[1]<<" "<<AParameter.FCLPreview[2]<<endl;
  
  AStream<<"  ColorBad:       "<<AParameter.FCLPreviewBad[0]<<" "
	 <<AParameter.FCLPreviewBad[1]<<" "<<AParameter.FCLPreviewBad[2]<<endl;
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream, CParameterPreview & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterPreview:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth:") );
  AStream>>AParameter.FLWPreview;

  AStream>>tmp; assert ( !strcmp(tmp, "PointLineWidth:") );
  AStream>>AParameter.FLWPreviewPoint;

  AStream>>tmp; assert ( !strcmp(tmp, "Color:") );
  AStream>>AParameter.FCLPreview[0]>>AParameter.FCLPreview[1]
	 >>AParameter.FCLPreview[2];

  AStream>>tmp; assert ( !strcmp(tmp, "ColorBad:") );
  AStream>>AParameter.FCLPreviewBad[0]>>AParameter.FCLPreviewBad[1]
	 >>AParameter.FCLPreviewBad[2];

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//------------------------------------------------------------------------------
} // namespace GMap3d
//******************************************************************************
int CParameterPreview::getLWPreview() const
{ return FLWPreview; }
void CParameterPreview::setLWPreview(int AValue)
{
  if ( FLWPreview!=AValue )
    {
      putAllNeedToUpdate();
      FLWPreview= AValue;
    }
}
//******************************************************************************
int CParameterPreview::getLWPreviewPoint() const
{ return FLWPreviewPoint; }
void CParameterPreview::setLWPreviewPoint(int AValue)
{
  if ( FLWPreviewPoint!=AValue )
    {
      putAllNeedToUpdate();
      FLWPreviewPoint = AValue;
    }
}
//******************************************************************************
float CParameterPreview::getCLPreview(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLPreview[AIndice];
}
void CParameterPreview::setCLPreview(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLPreview[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLPreview[AIndice]= AValue;
    }
}
const float * CParameterPreview::getCLPreview() const
{ return FCLPreview; }
void CParameterPreview::setCLPreview(float AValue0, float AValue1, float AValue2)
{
  if ( FCLPreview[0]!=AValue0 ||
       FCLPreview[1]!=AValue1 ||
       FCLPreview[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLPreview[0]= AValue0;
      FCLPreview[1]= AValue1;
      FCLPreview[2]= AValue2;
    }
}
void CParameterPreview::setCLPreview(const float ATab[3])
{ setCLPreview(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterPreview::getCLPreviewBad(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLPreviewBad[AIndice];
}
void CParameterPreview::setCLPreviewBad(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLPreviewBad[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLPreviewBad[AIndice]= AValue;
    }
}
const float * CParameterPreview::getCLPreviewBad() const
{ return FCLPreviewBad; }
void CParameterPreview::setCLPreviewBad(float AValue0, float AValue1, float AValue2)
{
  if ( FCLPreviewBad[0]!=AValue0 ||
       FCLPreviewBad[1]!=AValue1 ||
       FCLPreviewBad[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLPreviewBad[0]= AValue0;
      FCLPreviewBad[1]= AValue1;
      FCLPreviewBad[2]= AValue2;
    }
}
void CParameterPreview::setCLPreviewBad(const float ATab[3])
{ setCLPreviewBad(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
int CParameterPreview::getType() const
{ return PARAMETER_PREVIEW; }
//******************************************************************************
