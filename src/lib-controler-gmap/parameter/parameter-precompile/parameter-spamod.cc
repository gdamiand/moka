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
#ifdef MODULE_SPAMOD
//******************************************************************************
#include "controler-gmap-types.hh"
#include "parameters-gmap-default-values.hh"
#include "parameter-spamod.hh"
#include <cassert>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterSpamod::CParameterSpamod(int ANbRef) :
  CParameter(ANbRef),
  FViewMode (SPAMOD_TOPOLOGICAL)
{
  FEuclidianParam[0] = true;
  FEuclidianParam[1] = true;

  FAnalyticParam [0] = true;
  FAnalyticParam [1] = true;
  FAnalyticParam [2] = true;
  FAnalyticParam [3] = true;

  FVoxelParam 	 [0] = true;
  FVoxelParam 	 [1] = true;
  FVoxelParam 	 [2] = true;

  FK2Param 	 [0] = true;
  FK2Param 	 [1] = true;
  FK2Param 	 [2] = true;
}
//******************************************************************************
CParameterSpamod::CParameterSpamod(const CParameterSpamod & AParam) :
  CParameter(AParam),
  FViewMode (AParam.FViewMode)
{
  FEuclidianParam[0] = AParam.FEuclidianParam[0];
  FEuclidianParam[1] = AParam.FEuclidianParam[1];

  FAnalyticParam [0] = AParam.FAnalyticParam [0];
  FAnalyticParam [1] = AParam.FAnalyticParam [1];
  FAnalyticParam [2] = AParam.FAnalyticParam [2];
  FAnalyticParam [3] = AParam.FAnalyticParam [3];

  FVoxelParam 	 [0] = AParam.FVoxelParam    [0];
  FVoxelParam 	 [1] = AParam.FVoxelParam    [1];
  FVoxelParam 	 [2] = AParam.FVoxelParam    [2];

  FK2Param 	 [0] = AParam.FK2Param       [0];
  FK2Param 	 [1] = AParam.FK2Param       [1];
  FK2Param 	 [2] = AParam.FK2Param       [2];
}
//******************************************************************************
CParameterSpamod::~CParameterSpamod()
{}
//******************************************************************************
CParameter * CParameterSpamod::copy() const
{ return new CParameterSpamod(*this); }
//******************************************************************************
void CParameterSpamod::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterSpamod::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterSpamod::reinit()
{}
//******************************************************************************
int CParameterSpamod::getType() const
{
  return PARAMETER_SPAMOD;
}
//******************************************************************************
void CParameterSpamod::setViewMode(TSpamodViewMode AMode)
{
  if ( FViewMode!=AMode )
    {
      FViewMode = AMode;
      putAllNeedToUpdate();
    }
}
//******************************************************************************
void CParameterSpamod::setNextViewMode()
{
  if (FViewMode==SPAMOD_K2) FViewMode=SPAMOD_TOPOLOGICAL;
  else FViewMode = 1+(int)FViewMode;
  putAllNeedToUpdate();
}
//******************************************************************************
TSpamodViewMode CParameterSpamod::getViewMode() const
{
  return FViewMode;
}
//******************************************************************************
bool CParameterSpamod::getEuclidianParam(int AIndex)
{
  assert(0<=AIndex && AIndex<=1);
  return FEuclidianParam[AIndex];
}
//******************************************************************************
void CParameterSpamod::setEuclidianParam(int AIndex, bool AValue)
{
  assert(0<=AIndex && AIndex<=1);
  FEuclidianParam[AIndex] = AValue;
}
//******************************************************************************
bool CParameterSpamod::getAnalyticParam(int AIndex)
{
  assert(0<=AIndex && AIndex<=3);
  return FAnalyticParam[AIndex];
}
//******************************************************************************
void CParameterSpamod::setAnalyticParam(int AIndex, bool AValue)
{
  assert(0<=AIndex && AIndex<=1);
  FAnalyticParam[AIndex] = AValue;
}
//******************************************************************************
bool CParameterSpamod::getVoxelParam(int AIndex)
{
  assert(0<=AIndex && AIndex<=2);
  return FVoxelParam[AIndex];
}
//******************************************************************************
void CParameterSpamod::setVoxelParam(int AIndex, bool AValue)
{
  assert(0<=AIndex && AIndex<=1);
  FVoxelParam[AIndex] = AValue;
}
//******************************************************************************
bool CParameterSpamod::getK2Param(int AIndex)
{
  assert(0<=AIndex && AIndex<=2);
  return FK2Param[AIndex];
}
//******************************************************************************
void CParameterSpamod::setK2Param(int AIndex, bool AValue)
{
  assert(0<=AIndex && AIndex<=1);
  FK2Param[AIndex] = AValue;
}
//******************************************************************************
#endif // MODULE_SPAMOD
//******************************************************************************
