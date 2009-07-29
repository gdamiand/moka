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
#include "parameter-object-translation.hh"
#include "transformation-matrix.hh"
#include "geometry.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CParameterObjectTranslation::CParameterObjectTranslation(int ANbRef) :
  CParameter  (ANbRef),
  FVector     (ORIGIN),
  FPonderation(false)
{}
//******************************************************************************
CParameterObjectTranslation::
CParameterObjectTranslation(const CParameterObjectTranslation & AParam) :
  CParameter  (AParam),
  FVector     (AParam.FVector),
  FPonderation(AParam.FPonderation)
{}
//******************************************************************************
CParameterObjectTranslation::~CParameterObjectTranslation()
{}
//******************************************************************************
CParameter * CParameterObjectTranslation::copy() const
{ return new CParameterObjectTranslation(*this); }
//******************************************************************************
void CParameterObjectTranslation::load(istream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectTranslation::save(ostream& /*AStream*/)
{}
//------------------------------------------------------------------------------
void CParameterObjectTranslation::reinit()
{}
//******************************************************************************
int CParameterObjectTranslation::getType() const
{
  return PARAMETER_OBJECT_TRANSLATION;
}
//******************************************************************************
void CParameterObjectTranslation::setVector(const CVertex & AVector)
{
  FVector = AVector;
}

CVertex CParameterObjectTranslation::getVector() const
{
  return FVector;
}
//******************************************************************************
void CParameterObjectTranslation::setPonderation(bool AActive)
{
  if (FPonderation != AActive)
    {
      putAllNeedToUpdate();
      FPonderation = AActive;
    }
}

bool CParameterObjectTranslation::getPonderation() const
{
  return FPonderation;
}
//******************************************************************************
