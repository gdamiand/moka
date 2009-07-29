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
#include "precompile-axis.hh"
#include <cassert>
//******************************************************************************
CPrecompileAxis::CPrecompileAxis(CParameterAxis * AParameterAxis) :
  FParameterAxis(AParameterAxis)
{
  assert(FParameterAxis!=NULL);
  FParameterAxis->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileAxis::CPrecompileAxis(const CPrecompileAxis & APrecompile) :
  CPrecompile   (APrecompile),
  FParameterAxis(static_cast<CParameterAxis*>
		 (APrecompile.FParameterAxis->copy()))
{
  assert(FParameterAxis!=NULL);
  FParameterAxis->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileAxis::copy() const
{ return new CPrecompileAxis(*this); }
//******************************************************************************
CPrecompileAxis::~CPrecompileAxis()
{ FParameterAxis->removePrecompileToUpdate(this); }
//******************************************************************************
void CPrecompileAxis::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_AXIS:
      setAxis(static_cast<CParameterAxis *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileAxis::getParameter() const
{ return FParameterAxis; }
//******************************************************************************
void CPrecompileAxis::setAxis(CParameterAxis * AAxis)
{
  assert(AAxis != NULL);
  AAxis->addPrecompileToUpdate(this);
  FParameterAxis->removePrecompileToUpdate(this);
  FParameterAxis = AAxis;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileAxis::getType() const
{ return PRECOMPILE_AXIS; }
//******************************************************************************
void CPrecompileAxis::drawModel()
{
  const float size = FParameterAxis->getLGAxis();

  glBegin(GL_LINES);

  glLineWidth(FParameterAxis->getLWAxis());

  // X
  glColor3fv(FParameterAxis->getCLAxisX());
  glVertex3f(0,0,0); glVertex3f(size,0,0);
  // Y
  glColor3fv(FParameterAxis->getCLAxisY());
  glVertex3f(0,0,0); glVertex3f(0,size,0);
  // Z
  glColor3fv(FParameterAxis->getCLAxisZ());
  glVertex3f(0,0,0); glVertex3f(0,0,size);

  glEnd();
}
//******************************************************************************
