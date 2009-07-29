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
#include "parameter-face.hh"
#include <cassert>
#include <cstring>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterFace::CParameterFace(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterFace::CParameterFace(const CParameterFace & AParam) :
  CParameter          (AParam),
  FBLFace             (AParam.FBLFace),
  FRandomCLFace       (AParam.FRandomCLFace),
  FRandomCLVolume     (AParam.FRandomCLVolume),
  FRandomColorGeometry(AParam.FRandomColorGeometry)
  
{
  setCLFace(AParam.getCLFace());
  setBLFace(AParam.getBLFace());
}
//******************************************************************************
CParameterFace::~CParameterFace()
{}
//******************************************************************************
CParameter * CParameterFace::copy() const
{ return new CParameterFace(*this); }
//******************************************************************************
void CParameterFace::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterFace::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterFace::reinit()
{
  setCLFace(DEFAULT_FACE_COLOR_0,DEFAULT_FACE_COLOR_1,DEFAULT_FACE_COLOR_2);
  FBLFace = DEFAULT_FACE_TRANSPARENCY;
  FRandomCLFace = DEFAULT_FACE_RANDOM_CL_FACE;
  FRandomCLVolume = DEFAULT_FACE_RANDOM_CL_VOLUME;
  FRandomColorGeometry = DEFAULT_FACE_RANDOM_GEOMETRY;
  putAllNeedToUpdate();
}
//******************************************************************************
namespace GMap3d
{
//------------------------------------------------------------------------------
ostream& operator<<(ostream& AStream, const CParameterFace & AParameter)
{
  AStream<<"CParameterFace:"<<endl;
  
  AStream<<"  Color:               "<<AParameter.FCLFace[0]<<" "
	 <<AParameter.FCLFace[1]<<" "<<AParameter.FCLFace[2]<<endl;
  
  AStream<<"  Transparency:        "<<AParameter.FBLFace<<endl;
  
  AStream<<"  RandomColorFace:     "<<AParameter.FRandomCLFace<<endl;
  AStream<<"  RandomColorVolume:   "<<AParameter.FRandomCLVolume<<endl;
  AStream<<"  RandomColorGeometry: "<<AParameter.FRandomColorGeometry<<endl;
  
  AStream<<endl;
  
  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream, CParameterFace & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterFace:") );

  AStream>>tmp; assert ( !strcmp(tmp, "Color:") );
  AStream>>AParameter.FCLFace[0]>>AParameter.FCLFace[1]
	 >>AParameter.FCLFace[2];

  AStream>>tmp; assert ( !strcmp(tmp, "Transparency:") );
  AStream>>AParameter.FBLFace;

  AStream>>tmp; assert ( !strcmp(tmp, "RandomColorFace:") );
  AStream>>AParameter.FRandomCLFace;
  
  AStream>>tmp; assert ( !strcmp(tmp, "RandomColorVolume:") );
  AStream>>AParameter.FRandomCLVolume;

  AStream>>tmp; assert ( !strcmp(tmp, "RandomColorGeometry:") );
  AStream>>AParameter.FRandomColorGeometry;

  AParameter.putAllNeedToUpdate();
  
  return AStream;
}
//------------------------------------------------------------------------------
} // namespace GMap3d
//******************************************************************************
float CParameterFace::getCLFace(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLFace[AIndice];
}
void CParameterFace::setCLFace(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if ( FCLFace[AIndice]!=AValue )
    {
      putAllNeedToUpdate();
      FCLFace[AIndice]= AValue;
    }
}
const float * CParameterFace::getCLFace() const
{ return FCLFace; }
void CParameterFace::setCLFace(float AValue0, float AValue1, float AValue2)
{
  if ( FCLFace[0]!=AValue0 ||
       FCLFace[1]!=AValue1 ||
       FCLFace[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLFace[0]= AValue0;
      FCLFace[1]= AValue1;
      FCLFace[2]= AValue2;
    }
}
void CParameterFace::setCLFace(const float ATab[3])
{ setCLFace(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
float CParameterFace::getBLFace() const
{ return FBLFace; }
void CParameterFace::setBLFace(float AValue)
{
  if ( FBLFace!=AValue )
    {
      putAllNeedToUpdate();
      FBLFace = AValue;
    }
}
//******************************************************************************
bool CParameterFace::getRandomCLFace() const
{ return FRandomCLFace; }

void CParameterFace::setRandomCLFace(bool AValue)
{
  if ( FRandomCLFace!=AValue )
    {
      putAllNeedToUpdate();
      FRandomCLFace=AValue;
    }
}
//******************************************************************************
bool CParameterFace::getRandomCLVolume() const
{ return FRandomCLVolume; }

void CParameterFace::setRandomCLVolume(bool AValue)
{
  if ( FRandomCLVolume!=AValue )
    {
      putAllNeedToUpdate();
      FRandomCLVolume = AValue;
    }
}
//******************************************************************************
bool CParameterFace::getRandomColorGeometry() const
{ return FRandomColorGeometry; }

void CParameterFace::setRandomColorGeometry(bool AValue)
{
  if ( FRandomColorGeometry!=AValue )
    {
      putAllNeedToUpdate();
      FRandomColorGeometry = AValue;
    }
}
//******************************************************************************
int CParameterFace::getType() const
{ return PARAMETER_FACE; }
//******************************************************************************
