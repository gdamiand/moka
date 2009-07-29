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
#include "precompile-aimed-point.hh"
#include "parameter-aimed-position.hh"
#include <cassert>
//******************************************************************************
CPrecompileAimedPoint::CPrecompileAimedPoint(CParameterAimedPoint*
					     AParameterAimedPoint,
					     CParameterAimedPosition*
					     AParameterAimedPosition) :
  FParameterAimedPoint   (AParameterAimedPoint),
  FParameterAimedPosition(AParameterAimedPosition)
{
  assert(FParameterAimedPoint!=NULL);
  assert(FParameterAimedPosition!=NULL);

  FParameterAimedPoint->addPrecompileToUpdate(this);
  FParameterAimedPosition->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileAimedPoint::
CPrecompileAimedPoint(const CPrecompileAimedPoint & APrecompile) :
  CPrecompile            (APrecompile),
  FParameterAimedPoint   (static_cast<CParameterAimedPoint*>
			  (APrecompile.FParameterAimedPoint->copy())),
  FParameterAimedPosition(APrecompile.FParameterAimedPosition)
{
  assert(FParameterAimedPoint!=NULL);
  assert(FParameterAimedPosition!=NULL);

  FParameterAimedPoint->addPrecompileToUpdate(this);
  FParameterAimedPosition->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileAimedPoint::~CPrecompileAimedPoint()
{
  FParameterAimedPoint->removePrecompileToUpdate(this);
  FParameterAimedPosition->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile* CPrecompileAimedPoint::copy() const
{ return new CPrecompileAimedPoint(*this); }
//******************************************************************************
void CPrecompileAimedPoint::setParameter(CParameter* AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_AIMED_POINT:
      setAimedPoint(static_cast<CParameterAimedPoint*>(AParameter));
      break;
    case PARAMETER_AIMED_POSITION:
      setAimedPosition(static_cast<CParameterAimedPosition*>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileAimedPoint::getParameter() const
{ return FParameterAimedPoint; }
//******************************************************************************
void CPrecompileAimedPoint::setAimedPosition(CParameterAimedPosition*
					     APosition)
{
  assert(APosition != NULL);
  APosition->addPrecompileToUpdate(this);
  FParameterAimedPosition->removePrecompileToUpdate(this);
  FParameterAimedPosition = APosition;
  setToUpdate();
}
//******************************************************************************
void CPrecompileAimedPoint::setAimedPoint(CParameterAimedPoint* APoint)
{
  assert(APoint != NULL);
  APoint->addPrecompileToUpdate(this);
  FParameterAimedPoint->removePrecompileToUpdate(this);
  FParameterAimedPoint = APoint;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileAimedPoint::getType() const
{ return PRECOMPILE_AIMED_POINT; }
//******************************************************************************
void CPrecompileAimedPoint::drawModel()
{
  const float size = FParameterAimedPoint->getLGAimedPoint();

  glLineWidth(FParameterAimedPoint->getLWAimedPoint());
  glColor3fv (FParameterAimedPoint->getCLAimedPoint());

  glPushMatrix();

  glTranslatef(FParameterAimedPosition->getLookAt(0),
	       FParameterAimedPosition->getLookAt(1),
	       FParameterAimedPosition->getLookAt(2));

  glBegin(GL_LINES);

  glVertex3f( size,-size,    0); glVertex3f(-size, size,    0);
  glVertex3f(-size,-size,    0); glVertex3f( size, size,    0);
  glVertex3f(    0,    0,    0); glVertex3f(    0,    0, size);

  glEnd();

  glPopMatrix();
}
//******************************************************************************
