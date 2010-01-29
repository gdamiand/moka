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
#include "controler.hh"
#include "precompile.hh"
#include "math-extension.hh"

#include "view-precompile.hh"
#include "view-precompile-ortho.hh"
#include "view-precompile-perspective.hh"

#include "precompile-aimed-point.hh"
#include "precompile-axis.hh"
#include "precompile-grid.hh"

#include <cassert>
#include <string>
using namespace std;
//******************************************************************************
CPrecompile* CControler::newPrecompile(TViewId AViewId,
				       TPrecompile APrecompileType)
{
  CPrecompile* precompile = NULL;

  switch (APrecompileType)
    {
    case PRECOMPILE_AIMED_POINT:
      precompile =
	new CPrecompileAimedPoint(new CParameterAimedPoint,
				  & FViews[AViewId]->getAimedPosition());
      break;
    case PRECOMPILE_AXIS:
      precompile = new CPrecompileAxis(new CParameterAxis);
      break;
    case PRECOMPILE_GRID:
      precompile = new CPrecompileGrid(new CParameterGrid);
      break;
    }

  return precompile;
}
//******************************************************************************
CViewPrecompile*
CControler::newViewPrecompile(TView AViewType,
			      CParameterEyePosition* AEyePosition,
			      CParameterAimedPosition* AAimedPosition,
			      CParameterDrawing* ASceneDrawing)
{
  CViewPrecompile* view = NULL;
  
  switch (AViewType)
    {
    case VIEW_XYZ:
      view = new CViewPrecompilePerspective(AEyePosition,
					    AAimedPosition,
					    ASceneDrawing);
      break;
    case VIEW_ISO:
      view = new CViewPrecompileOrtho(AEyePosition, AAimedPosition, ASceneDrawing,
				      OX+OY, OX-OY+OZ); break;
    case VIEW_XY:
      view = new CViewPrecompileOrtho(AEyePosition, AAimedPosition, ASceneDrawing,
				      OX, OY); break;
    case VIEW_XZ:
      view = new CViewPrecompileOrtho(AEyePosition, AAimedPosition, ASceneDrawing,
				      OX, OZ); break;
    case VIEW_YZ:
      view = new CViewPrecompileOrtho(AEyePosition, AAimedPosition, ASceneDrawing,
				      OY, OZ); break;
    }

 return view;
}
//******************************************************************************
