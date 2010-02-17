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
#include "view-precompile-perspective.hh"
#include "parameter-eye-position.hh"
#include "parameter-aimed-position.hh"
//******************************************************************************
CViewPrecompilePerspective::
CViewPrecompilePerspective(CParameterEyePosition   * AParameterEyePosition,
			   CParameterAimedPosition * AParameterAimedPosition,
			   CParameterDrawing 	   * AParameterDrawing) :
  CViewPrecompile(AParameterEyePosition,
		  AParameterAimedPosition,
		  AParameterDrawing)
{}
//******************************************************************************
CViewPrecompilePerspective::~CViewPrecompilePerspective()
{}
//******************************************************************************
void CViewPrecompilePerspective::setScenePosition()
{
  setScenePositionInitialize();

  float d = FParameterEyePosition->getDistance();

  float p0 = FParameterAimedPosition->getLookAt(0);
  float p1 = FParameterAimedPosition->getLookAt(1);
  float p2 = FParameterAimedPosition->getLookAt(2);

  float alpha = FParameterEyePosition->getRotationAlpha();
  float b = FParameterEyePosition->getRotationBeta ();

  gluPerspective(45 /* angle*/, FRatio, 0.1 /*0.01*/, 2000);
  gluLookAt(0,-d,0, 0,0,0, 0,0,1);

  glRotatef(-b, 1, 0, 0);
  glRotatef(-alpha, 0, 0, 1);

  glTranslatef(-p0,-p1,-p2);

  setScenePositionFinalize();
}
//******************************************************************************
