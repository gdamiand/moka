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
#include "parameter-operations.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterOperations::CParameterOperations(int ANbRef) :
  CParameter(ANbRef),

  FRotateCells(true),
  FScaleCells(true),
  FTranslateCells(true),

  FInterpolationMerges(true),
  FInterpolationSews  (true),
  
  FSmoothMerges(true),
  FSmoothSews  (true),
  FSmoothMethod(SPLINE_SMOOTH),
  
  FExtrusionCoef                (1.0),
  FExtrusionRevolutionAngle     (360.0),
  FExtrusionAdjacentSews	(true),
  FExtrusionInitialPositionMode (1),
  FExtrusionInitialDirectionMode(1),
  FExtrusionDirection	        (true),
  FExtrusionScale	        (true),
  FExtrusionPonderateSection    (false),
  FExtrusionRevolutionNbEdges   (8)
{
  FMeshNbSubdivisions[0] = 5;
  FMeshNbSubdivisions[1] = 5;
  FMeshNbSubdivisions[2] = 5;

  FRoundingDefaultCoefs[0] = 0.2;
  FRoundingDefaultCoefs[1] = 0.2;
  FRoundingDefaultCoefs[2] = 0.2;
}
//******************************************************************************
CParameterOperations::
CParameterOperations(const CParameterOperations & AParam) :
  CParameter     (AParam),
  
  FRotateCells   (AParam.FRotateCells),
  FScaleCells    (AParam.FScaleCells),
  FTranslateCells(AParam.FTranslateCells),

  FInterpolationMerges(AParam.FInterpolationMerges),
  FInterpolationSews  (AParam.FInterpolationSews),
  
  FSmoothMerges(AParam.FSmoothMerges),
  FSmoothSews  (AParam.FSmoothSews),
  FSmoothMethod(AParam.FSmoothMethod),
  
  FExtrusionCoef		(AParam.FExtrusionCoef),
  FExtrusionRevolutionAngle     (AParam.FExtrusionRevolutionAngle),
  FExtrusionAdjacentSews	(AParam.FExtrusionAdjacentSews),
  FExtrusionInitialPositionMode (AParam.FExtrusionInitialPositionMode),
  FExtrusionInitialDirectionMode(AParam.FExtrusionInitialDirectionMode),
  FExtrusionDirection	        (AParam.FExtrusionDirection),
  FExtrusionScale	        (AParam.FExtrusionScale),
  FExtrusionPonderateSection    (AParam.FExtrusionPonderateSection),
  FExtrusionRevolutionNbEdges   (AParam.FExtrusionRevolutionNbEdges) 
{
  FMeshNbSubdivisions[0] = AParam.FMeshNbSubdivisions[0];
  FMeshNbSubdivisions[1] = AParam.FMeshNbSubdivisions[1];
  FMeshNbSubdivisions[2] = AParam.FMeshNbSubdivisions[2];

  FRoundingDefaultCoefs[0] = AParam.FRoundingDefaultCoefs[0];
  FRoundingDefaultCoefs[1] = AParam.FRoundingDefaultCoefs[1];
  FRoundingDefaultCoefs[2] = AParam.FRoundingDefaultCoefs[2];
}
//******************************************************************************
CParameterOperations::~CParameterOperations()
{
}
//******************************************************************************
CParameter * CParameterOperations::copy() const
{ return new CParameterOperations(*this); }
//******************************************************************************
void CParameterOperations::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterOperations::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterOperations::reinit()
{}
//******************************************************************************
int CParameterOperations::getType() const
{
  return PARAMETER_OPERATIONS;
}
//******************************************************************************
void CParameterOperations::setRotateCells(bool ABool)
{
  if ( FRotateCells!=ABool )
    {
      putAllNeedToUpdate();
      FRotateCells = ABool;
    }
}

bool CParameterOperations::getRotateCells() const
{
  return FRotateCells;
}
//------------------------------------------------------------------------------
void CParameterOperations::setScaleCells(bool ABool)
{
  if ( FScaleCells!=ABool )
    {
      putAllNeedToUpdate();
      FScaleCells = ABool;
    }
}

bool CParameterOperations::getScaleCells() const
{
  return FScaleCells;
}
//------------------------------------------------------------------------------
void CParameterOperations::setTranslateCells(bool ABool)
{
  if ( FTranslateCells!=ABool )
    {
      putAllNeedToUpdate();
      FTranslateCells = ABool;
    }
}

bool CParameterOperations::getTranslateCells() const
{
  return FTranslateCells;
}
//******************************************************************************
int CParameterOperations::getMeshNbSubdivisionsX() const
{
  return FMeshNbSubdivisions[0];
}

int CParameterOperations::getMeshNbSubdivisionsY() const
{
  return FMeshNbSubdivisions[1];
}

int CParameterOperations::getMeshNbSubdivisionsZ() const
{
  return FMeshNbSubdivisions[2];
}

void CParameterOperations::setMeshNbSubdivisionsX(int ASx)
{
  assert(0 < ASx);
  FMeshNbSubdivisions[0] = ASx;
}

void CParameterOperations::setMeshNbSubdivisionsY(int ASy)
{
  assert(0 < ASy);
  FMeshNbSubdivisions[1] = ASy;
}

void CParameterOperations::setMeshNbSubdivisionsZ(int ASz)
{
  assert(0 < ASz);
  FMeshNbSubdivisions[2] = ASz;
}
//------------------------------------------------------------------------------
bool CParameterOperations::getInterpolationMerges() const
{ return FInterpolationMerges; }

void CParameterOperations::setInterpolationMerges(bool ABool)
{
  if ( FInterpolationMerges!=ABool )
    {
      putAllNeedToUpdate();
      FInterpolationMerges = ABool;
    }
}
//------------------------------------------------------------------------------
bool CParameterOperations::getInterpolationSews() const
{
  if (isActiveInterpolationSews()) return FInterpolationSews;

  return false;
}

void CParameterOperations::setInterpolationSews(bool ABool)
{
  if ( FInterpolationSews!=ABool && isActiveInterpolationSews() )
    {
      putAllNeedToUpdate();
      FInterpolationSews = ABool;
    }
}
//------------------------------------------------------------------------------
bool CParameterOperations::isActiveInterpolationSews() const
{ return !getInterpolationMerges(); }
//------------------------------------------------------------------------------
bool CParameterOperations::getSmoothMerges() const
{
  if (isActiveSmoothMerges()) return FSmoothMerges;

  return false;
}

void CParameterOperations::setSmoothMerges(bool ABool)
{
  if ( FSmoothMerges!=ABool && isActiveSmoothMerges() )
    {
      putAllNeedToUpdate();
      FSmoothMerges = ABool;
    }
}
//------------------------------------------------------------------------------
bool CParameterOperations::getSmoothSews() const
{
  if (isActiveSmoothSews()) return FSmoothSews;
  
  return false;
}

void CParameterOperations::setSmoothSews(bool ABool)
{
  if ( FSmoothSews!=ABool && isActiveSmoothSews() )
    {
      putAllNeedToUpdate();
      FSmoothSews = ABool;
    }
}
//------------------------------------------------------------------------------
TSmoothType CParameterOperations:: getSmoothMethod() const
{ return FSmoothMethod; }

void CParameterOperations::setSmoothMethod(TSmoothType AValue)
{
  assert(HERMITE_SMOOTH<=AValue && AValue<=SPLINE_SMOOTH );
  if ( FSmoothMethod!=AValue )
    {
      putAllNeedToUpdate();
      FSmoothMethod = AValue;
    }
}
//------------------------------------------------------------------------------
static const CTransformationMatrix * SMOOTH_MATRIX[] =
{
  & HERMITE,
  & BEZIER,
  & OVERHAUSER,
  & SPLINE
};

const CTransformationMatrix & CParameterOperations::getMeshMatrix() const
{
  return * SMOOTH_MATRIX[FSmoothMethod];
}
//------------------------------------------------------------------------------
static const bool CAN_MERGE[] =
{
  /*HERMITE   */ true ,
  /*BEZIER    */ false,
  /*OVERHAUSER*/ true ,
  /*SPLINE    */ false
};

static const bool CAN_SEW[] =
{
  /*HERMITE   */ true ,
  /*BEZIER    */ false,
  /*OVERHAUSER*/ true ,
  /*SPLINE    */ true
};
//------------------------------------------------------------------------------
bool CParameterOperations::isActiveSmoothMerges() const
{
  assert(HERMITE_SMOOTH<=FSmoothMethod && FSmoothMethod<=SPLINE_SMOOTH );
  
  return CAN_MERGE[FSmoothMethod];
}
bool CParameterOperations::isActiveSmoothSews() const
{
  assert(HERMITE_SMOOTH<=FSmoothMethod && FSmoothMethod<=SPLINE_SMOOTH );
  
  return CAN_SEW[FSmoothMethod] && !getSmoothMerges();
}
//******************************************************************************
float CParameterOperations::getExtrusionCoef() const
{
  return FExtrusionCoef;
}

void CParameterOperations::setExtrusionCoef(float ACoef)
{
  FExtrusionCoef = ACoef;
}
//------------------------------------------------------------------------------
float CParameterOperations::getExtrusionRevolutionAngle() const
{
  return FExtrusionRevolutionAngle;
}

void CParameterOperations::setExtrusionRevolutionAngle(float AAngle)
{
  FExtrusionRevolutionAngle = AAngle;
}
//------------------------------------------------------------------------------
bool CParameterOperations::getExtrusionAdjacentSews() const
{ return FExtrusionAdjacentSews; }

void CParameterOperations::setExtrusionAdjacentSews(bool ABool)
{
  if ( FExtrusionAdjacentSews!=ABool )
    {
      putAllNeedToUpdate();
      FExtrusionAdjacentSews = ABool;
    }
}
//------------------------------------------------------------------------------
int CParameterOperations:: getExtrusionInitialPositionMode() const
{ return FExtrusionInitialPositionMode; }

void CParameterOperations::setExtrusionInitialPositionMode(int AValue)
{
  if ( FExtrusionInitialPositionMode!=AValue )
    {
      putAllNeedToUpdate();
      FExtrusionInitialPositionMode = AValue;
    }
}
//------------------------------------------------------------------------------
int CParameterOperations:: getExtrusionInitialDirectionMode() const
{ return FExtrusionInitialDirectionMode; }

void CParameterOperations::setExtrusionInitialDirectionMode(int AValue)
{
  if ( FExtrusionInitialDirectionMode!=AValue )
    {
      putAllNeedToUpdate();
      FExtrusionInitialDirectionMode = AValue;
    }
}
//------------------------------------------------------------------------------
bool CParameterOperations::getExtrusionDirection() const
{ return FExtrusionDirection; }

void CParameterOperations::setExtrusionDirection(bool ABool)
{
  if ( FExtrusionDirection!=ABool )
    {
      putAllNeedToUpdate();
      FExtrusionDirection = ABool;
    }
}
//------------------------------------------------------------------------------
bool CParameterOperations:: getExtrusionScale() const
{ return FExtrusionScale; }

void CParameterOperations::setExtrusionScale(bool ABool)
{
  if ( FExtrusionScale!=ABool )
    {
      putAllNeedToUpdate();
      FExtrusionScale = ABool;
    }
}
//------------------------------------------------------------------------------
bool CParameterOperations::getExtrusionPonderateSection() const
{ return FExtrusionPonderateSection; }

void CParameterOperations::setExtrusionPonderateSection(bool ABool)
{
  if ( FExtrusionPonderateSection!=ABool )
    {
      putAllNeedToUpdate();
      FExtrusionPonderateSection = ABool;
    }
}
//------------------------------------------------------------------------------
int CParameterOperations:: getExtrusionRevolutionNbEdges() const
{ return FExtrusionRevolutionNbEdges; }

void CParameterOperations::setExtrusionRevolutionNbEdges(int AValue)
{
  if ( FExtrusionRevolutionNbEdges!=AValue )
    {
      putAllNeedToUpdate();
      FExtrusionRevolutionNbEdges = AValue;
    }
}
//******************************************************************************
float CParameterOperations::getDefaultRoundingCoefForVertices() const
{
  return FRoundingDefaultCoefs[0];
}
//------------------------------------------------------------------------------
void CParameterOperations::setDefaultRoundingCoefForVertices(float ACoef)
{
  FRoundingDefaultCoefs[0] = ACoef;
}
//------------------------------------------------------------------------------
float CParameterOperations::getDefaultRoundingCoefForEdges() const
{
  return FRoundingDefaultCoefs[1];
}
//------------------------------------------------------------------------------
void CParameterOperations::setDefaultRoundingCoefForEdges(float ACoef)
{
  FRoundingDefaultCoefs[1] = ACoef;
}
//------------------------------------------------------------------------------
float CParameterOperations::getDefaultRoundingCoefForFaces() const
{
  return FRoundingDefaultCoefs[2];
}
//------------------------------------------------------------------------------
void CParameterOperations::setDefaultRoundingCoefForFaces(float ACoef)
{
  FRoundingDefaultCoefs[2] = ACoef;
}
//******************************************************************************
