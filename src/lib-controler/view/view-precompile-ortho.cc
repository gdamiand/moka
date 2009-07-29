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
#include "view-precompile-ortho.hh"
#include "parameter-eye-position.hh"
#include "parameter-aimed-position.hh"
//******************************************************************************
CViewPrecompileOrtho::
CViewPrecompileOrtho(CParameterEyePosition * AParameterEyePosition,
		     CParameterAimedPosition * AParameterAimedPosition,
		     CParameterDrawing * AParameterDrawing,
		     const CVertex & AOx,
		     const CVertex & AOy) :
  CViewPrecompile(AParameterEyePosition,
		  AParameterAimedPosition, AParameterDrawing),
  FOx(AOx),
  FOy(AOy)
{
  FOz = FOx * FOy;
}
//******************************************************************************
CViewPrecompileOrtho::~CViewPrecompileOrtho()
{}
//******************************************************************************
void CViewPrecompileOrtho::setScenePosition()
{
  setScenePositionInitialize();

  float d = FParameterEyePosition->getDistance();

  float p0 = FParameterAimedPosition->getLookAt(0);
  float p1 = FParameterAimedPosition->getLookAt(1);
  float p2 = FParameterAimedPosition->getLookAt(2);

  glOrtho(-d*FRatio/2,+d*FRatio/2, -d/2,+d/2, -1000,+1000);

  gluLookAt(p0+d*FOz.getX(), p1+d*FOz.getY(), p2+d*FOz.getZ(),
	    p0,p1,p2,
	    FOy.getX(), FOy.getY(), FOy.getZ());

  setScenePositionFinalize();
}
//******************************************************************************
