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
#include "parameter-grid.hh"
#include <cassert>
#include <cstring>
using namespace std;
//******************************************************************************
CParameterGrid::CParameterGrid(int ANbRef) :
  CParameter(ANbRef)
{
  reinit();
}
//******************************************************************************
CParameterGrid::CParameterGrid(const CParameterGrid & AParam) :
  CParameter           (AParam),
  FLWGrid	       (AParam.FLWGrid),
  FLGGrid	       (AParam.FLGGrid),
  FDisplayGridXY       (AParam.FDisplayGridXY),
  FDisplayGridYZ       (AParam.FDisplayGridYZ),
  FDisplayGridXZ       (AParam.FDisplayGridXZ),
  FDisplayPositiveGridX(AParam.FDisplayPositiveGridX),
  FDisplayPositiveGridY(AParam.FDisplayPositiveGridY),
  FDisplayPositiveGridZ(AParam.FDisplayPositiveGridZ)
{
  setCLGrid(AParam.getCLGrid());
}
//******************************************************************************
CParameterGrid::~CParameterGrid()
{}
//******************************************************************************
CParameter * CParameterGrid::copy() const
{ return new CParameterGrid(*this); }
//******************************************************************************
void CParameterGrid::save(ostream& AStream)
{ AStream<<(*this); }
//------------------------------------------------------------------------------
void CParameterGrid::load(istream& AStream)
{ AStream>>(*this); }
//------------------------------------------------------------------------------
void CParameterGrid::reinit()
{
  FLWGrid		= DEFAULT_GRID_LINE_WIDTH;
  FLGGrid		= DEFAULT_GRID_SIZE;
  FDisplayGridXY	= DEFAULT_GRID_DISPLAY_XY;
  FDisplayGridYZ	= DEFAULT_GRID_DISPLAY_YZ;
  FDisplayGridXZ	= DEFAULT_GRID_DISPLAY_XZ;
  FDisplayPositiveGridX	= DEFAULT_GRID_POSITIVE_X;
  FDisplayPositiveGridY	= DEFAULT_GRID_POSITIVE_Y;
  FDisplayPositiveGridZ	= DEFAULT_GRID_POSITIVE_Z;
  setCLGrid(DEFAULT_GRID_COLOR_0,DEFAULT_GRID_COLOR_1,DEFAULT_GRID_COLOR_2);

  putAllNeedToUpdate();
}
//******************************************************************************
ostream& operator<<(ostream& AStream,
		    const CParameterGrid & AParameter)
{
  AStream<<"CParameterGrid:"<<endl;
  
  AStream<<"  LineWidth: "<<AParameter.FLWGrid<<endl;
  
  AStream<<"  Length:    "<<AParameter.FLGGrid<<endl;
  
  AStream<<"  DisplayXY: "<<AParameter.FDisplayGridXY<<endl;
  AStream<<"  DisplayYZ: "<<AParameter.FDisplayGridYZ<<endl;
  AStream<<"  DisplayXZ: "<<AParameter.FDisplayGridXZ<<endl;
  
  AStream<<"  PositiveX: "<<AParameter.FDisplayPositiveGridX<<endl;
  AStream<<"  PositiveY: "<<AParameter.FDisplayPositiveGridY<<endl;
  AStream<<"  PositiveZ: "<<AParameter.FDisplayPositiveGridZ<<endl;
  
  AStream<<"  Color:     "<<AParameter.FCLGrid[0]<<" "
	 <<AParameter.FCLGrid[1]<<" "<<AParameter.FCLGrid[2]<<endl;
  
  AStream<<endl;

  return AStream;
}
//------------------------------------------------------------------------------
istream& operator>>(istream& AStream,
			    CParameterGrid & AParameter)
{
  char tmp[256];
  
  AStream>>tmp; assert ( !strcmp(tmp, "CParameterGrid:") );

  AStream>>tmp; assert ( !strcmp(tmp, "LineWidth:") );
  AStream>>AParameter.FLWGrid;

  AStream>>tmp; assert ( !strcmp(tmp, "Length:") );
  AStream>>AParameter.FLGGrid;

  AStream>>tmp; assert ( !strcmp(tmp, "DisplayXY:") );
  AStream>>AParameter.FDisplayGridXY;
  AStream>>tmp; assert ( !strcmp(tmp, "DisplayYZ:") );
  AStream>>AParameter.FDisplayGridYZ;
  AStream>>tmp; assert ( !strcmp(tmp, "DisplayXZ:") );
  AStream>>AParameter.FDisplayGridXZ;
  
  AStream>>tmp; assert ( !strcmp(tmp, "PositiveX:") );
  AStream>>AParameter.FDisplayPositiveGridX;
  AStream>>tmp; assert ( !strcmp(tmp, "PositiveY:") );
  AStream>>AParameter.FDisplayPositiveGridY;
  AStream>>tmp; assert ( !strcmp(tmp, "PositiveZ:") );
  AStream>>AParameter.FDisplayPositiveGridZ;
  
  AStream>>tmp; assert ( !strcmp(tmp, "Color:") );
  AStream>>AParameter.FCLGrid[0]>>AParameter.FCLGrid[1]
	 >>AParameter.FCLGrid[2];

  AParameter.putAllNeedToUpdate();

  return AStream;
}
//******************************************************************************
int CParameterGrid::getLWGrid() const
{ return FLWGrid; }
void CParameterGrid::setLWGrid(int AValue)
{
  if (FLWGrid!=AValue)
    {
      putAllNeedToUpdate();
      FLWGrid= AValue;
    }
}
//******************************************************************************
int CParameterGrid::getLGGrid() const
{ return FLGGrid; }
void CParameterGrid::setLGGrid(int AValue)
{
  if (FLGGrid!=AValue)
    {
      putAllNeedToUpdate();
      FLGGrid= AValue;
    }
}
//******************************************************************************
float CParameterGrid::getCLGrid(int AIndice) const
{
  assert(0<=AIndice && AIndice<=2);
  return FCLGrid[AIndice];
}
void CParameterGrid::setCLGrid(int AIndice, float AValue)
{
  assert(0<=AIndice && AIndice<=2);
  if (FCLGrid[AIndice]!=AValue)
    {
      putAllNeedToUpdate();
      FCLGrid[AIndice]= AValue;
    }
}
const float * CParameterGrid::getCLGrid() const
{ return FCLGrid; }
void CParameterGrid::setCLGrid(float AValue0, float AValue1, float AValue2)
{
  if ( FCLGrid[0]!=AValue0 ||
       FCLGrid[1]!=AValue1 ||
       FCLGrid[2]!=AValue2 )
    {
      putAllNeedToUpdate();
      FCLGrid[0]= AValue0;
      FCLGrid[1]= AValue1;
      FCLGrid[2]= AValue2;
    }
}
void CParameterGrid::setCLGrid(const float ATab[3])
{ setCLGrid(ATab[0],ATab[1],ATab[2]); }
//******************************************************************************
bool CParameterGrid::getDisplayGridXY() const
{ return FDisplayGridXY; }
//******************************************************************************
void CParameterGrid::setDisplayGridXY(bool AValue)
{
  if (FDisplayGridXY!=AValue)
    {
      putAllNeedToUpdate();
      FDisplayGridXY= AValue;
    }
}
//******************************************************************************
void CParameterGrid::invDisplayGridXY()
{
  putAllNeedToUpdate();
  FDisplayGridXY= !FDisplayGridXY;
}

//******************************************************************************
bool CParameterGrid::getDisplayGridYZ() const
{ return FDisplayGridYZ; }
//******************************************************************************
void CParameterGrid::setDisplayGridYZ(bool AValue)
{
  if (FDisplayGridYZ!=AValue)
    {
      putAllNeedToUpdate();
      FDisplayGridYZ= AValue;
    }
}
//******************************************************************************
void CParameterGrid::invDisplayGridYZ()
{
  putAllNeedToUpdate();
  FDisplayGridYZ= !FDisplayGridYZ;
}

//******************************************************************************
bool CParameterGrid::getDisplayGridXZ() const
{ return FDisplayGridXZ; }
//******************************************************************************
void CParameterGrid::setDisplayGridXZ(bool AValue)
{
  if (FDisplayGridXZ!=AValue)
    {
      putAllNeedToUpdate();
      FDisplayGridXZ= AValue;
    }
}
//******************************************************************************
void CParameterGrid::invDisplayGridXZ()
{
  putAllNeedToUpdate();
  FDisplayGridXZ= !FDisplayGridXZ;
}

//******************************************************************************
bool CParameterGrid::getDisplayPositiveGridX() const
{ return FDisplayPositiveGridX; }
//******************************************************************************
void CParameterGrid::setDisplayPositiveGridX(bool AValue)
{
  if (FDisplayPositiveGridX!=AValue)
    {
      putAllNeedToUpdate();
      FDisplayPositiveGridX= AValue;
    }
}
//******************************************************************************
void CParameterGrid::invDisplayPositiveGridX()
{
  putAllNeedToUpdate();
  FDisplayPositiveGridX= !FDisplayPositiveGridX;
}

//******************************************************************************
bool CParameterGrid::getDisplayPositiveGridY() const
{ return FDisplayPositiveGridY; }
//******************************************************************************
void CParameterGrid::setDisplayPositiveGridY(bool AValue)
{
  if (FDisplayPositiveGridY!=AValue)
    {
      putAllNeedToUpdate();
      FDisplayPositiveGridY= AValue;
    }
}
//******************************************************************************
void CParameterGrid::invDisplayPositiveGridY()
{
  putAllNeedToUpdate();
  FDisplayPositiveGridY= !FDisplayPositiveGridY;
}

//******************************************************************************
bool CParameterGrid::getDisplayPositiveGridZ() const
{ return FDisplayPositiveGridZ; }
//******************************************************************************
void CParameterGrid::setDisplayPositiveGridZ(bool AValue)
{
  if (FDisplayPositiveGridZ!=AValue)
    {
      putAllNeedToUpdate();
      FDisplayPositiveGridZ= AValue;
    }
}
//******************************************************************************
void CParameterGrid::invDisplayPositiveGridZ()
{
  putAllNeedToUpdate();
  FDisplayPositiveGridZ= !FDisplayPositiveGridZ;
}
//******************************************************************************
int CParameterGrid::getType() const
{ return PARAMETER_GRID; }
//******************************************************************************
