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
#include "parameter-object-rotation.hh"
#include "transformation-matrix.hh"
#include "geometry.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterObjectRotation::CParameterObjectRotation(int ANbRef) :
  CParameter  (ANbRef),
  FAxeVertex  (ORIGIN),
  FAxeVector  (OZ),
  FAngle      (0.0),
  FPonderation(false)
{}
//******************************************************************************
CParameterObjectRotation::
CParameterObjectRotation(const CParameterObjectRotation & AParam) :
  CParameter  (AParam),
  FAxeVertex  (AParam.FAxeVertex),
  FAxeVector  (AParam.FAxeVector),
  FAngle      (AParam.FAngle),
  FPonderation(AParam.FPonderation)
{}
//******************************************************************************
CParameterObjectRotation::~CParameterObjectRotation()
{}
//******************************************************************************
CParameter * CParameterObjectRotation::copy() const
{ return new CParameterObjectRotation(*this); }
//******************************************************************************
void CParameterObjectRotation::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectRotation::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectRotation::reinit()
{}
//******************************************************************************
int CParameterObjectRotation::getType() const
{
  return PARAMETER_OBJECT_ROTATION;
}
//******************************************************************************
void CParameterObjectRotation::setAxeVertex(const CVertex & AAxeVertex)
{
  if (FAxeVertex != AAxeVertex)
    {
      putAllNeedToUpdate();
      FAxeVertex = AAxeVertex;
    }
}

CVertex CParameterObjectRotation::getAxeVertex() const
{
  return FAxeVertex;
}
//******************************************************************************
void CParameterObjectRotation::setAxeVector(const CVertex & AAxeVector)
{
  if (FAxeVector != AAxeVector && !AAxeVector.isNull())
    {
      putAllNeedToUpdate();
      FAxeVector = AAxeVector;
    }
}

CVertex CParameterObjectRotation::getAxeVector() const
{
  return FAxeVector;
}
//******************************************************************************
void CParameterObjectRotation::setAngle(float AAngle)
{
  FAngle = AAngle;
}

float CParameterObjectRotation::getAngle() const
{
  return FAngle;
}
//******************************************************************************
void CParameterObjectRotation::setPonderation(bool AActive)
{
  if (FPonderation != AActive)
    {
      putAllNeedToUpdate();
      FPonderation = AActive;
    }
}

bool CParameterObjectRotation::getPonderation() const
{
  return FPonderation;
}
//******************************************************************************
