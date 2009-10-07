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
#include "view-precompile.hh"
#include "precompile.hh"
#include "parameter-aimed-point.hh"
#include "parameter-axis.hh"
#include "parameter-grid.hh"
using namespace std;
//******************************************************************************
CParameterAimedPosition* CControler::getParameterAimedPosition(TViewId AViewId) const
{
  assert( 0<=AViewId && AViewId<FViews.size() && FViews[AViewId]!=NULL);
  return FViews[AViewId]->getParameterAimedPosition();
}
//******************************************************************************
CParameterDrawing* CControler::getParameterDrawing(TViewId AViewId) const
{
  assert( 0<=AViewId && AViewId<FViews.size() && FViews[AViewId]!=NULL);
  return FViews[AViewId]->getParameterDrawing();
}
//******************************************************************************
CParameterEyePosition* CControler::getParameterEyePosition(TViewId AViewId) const
{
  assert( 0<=AViewId && AViewId<FViews.size() && FViews[AViewId]!=NULL);
  return FViews[AViewId]->getParameterEyePosition();
}
//******************************************************************************
CParameter* CControler::getParameterPrecompile(TViewId AViewId,
					       TPrecompile APrecompile) const
{
  assert ( 0<=AViewId && AViewId<FViews.size() && FViews[AViewId]!=NULL);

  CPrecompile* precomp = FViews[AViewId]->findPrecompile(APrecompile);
  if ( precomp==NULL ) return NULL;

  return precomp->getParameter();
}
//******************************************************************************
CParameterAimedPoint* CControler::getParameterAimedPoint(TViewId AViewId) const
{ return static_cast<CParameterAimedPoint*>
    (getParameterPrecompile(AViewId, PRECOMPILE_AIMED_POINT));
}
//******************************************************************************
CParameterAxis* CControler::getParameterAxis(TViewId AViewId) const
{ return static_cast<CParameterAxis*>
    (getParameterPrecompile(AViewId, PRECOMPILE_AXIS));
}
//******************************************************************************
CParameterGrid* CControler::getParameterGrid(TViewId AViewId) const
{ return static_cast<CParameterGrid*>
    (getParameterPrecompile(AViewId, PRECOMPILE_GRID));
}
//******************************************************************************
void CControler::saveOnePrecompileParameter(TViewId AViewId,
					    TPrecompile APrecompileType,
					    ostream& os)
{
  CParameter* parameter = getParameterPrecompile(AViewId, APrecompileType);
  
  if (parameter!=NULL)
    {
      parameter->save(os);
    }
  else
    {
      viewAddPrecompile(AViewId, APrecompileType);
      
      parameter = getParameterPrecompile(AViewId, APrecompileType);
      parameter->save(os);
      
      viewRemovePrecompile(AViewId, APrecompileType);
    }
}
//******************************************************************************
void CControler::loadOnePrecompileParameter(TViewId AViewId,
					    TPrecompile APrecompileType,
					    istream& is)
{
  CParameter* parameter = getParameterPrecompile(AViewId, APrecompileType);
  
  if (parameter!=NULL)
    {
      parameter->load(is);
    }
  else
    {
      viewAddPrecompile(AViewId, APrecompileType);
      
      parameter = getParameterPrecompile(AViewId, APrecompileType);
      parameter->load(is);
      
      viewRemovePrecompile(AViewId, APrecompileType);
    }
}
//******************************************************************************
void CControler::saveAllParameters(TViewId AViewId)
{
  stringstream s;
  s<<FConfigDirectory<<"/model-parameters.txt";  
  ofstream os(s.str().c_str());

  if (!os.good())
    return;

  // Les paramètres de view : 1 par CView  
  getParameterAimedPosition(AViewId)->save(os);
  getParameterDrawing(AViewId)->save(os);
  getParameterEyePosition(AViewId)->save(os);

  // Les paramètres de précompile
  TPrecompile aprecompile;
  for (aprecompile = FIRST_PRECOMPILE_CONTROLER;
       aprecompile < FLastPrecompile; ++aprecompile)
    {
      saveOnePrecompileParameter(AViewId, aprecompile, os);
    }
  
  os.close();
}
//******************************************************************************
void CControler::loadAllParameters(TViewId AViewId)
{
  stringstream s;
  s<<FConfigDirectory<<"/model-parameters.txt";  
  ifstream is(s.str().c_str());

  if (!is.good())
    return;

  // Les paramètres de view : 1 par CView  
  getParameterAimedPosition(AViewId)->load(is);
  getParameterDrawing(AViewId)->load(is);
  getParameterEyePosition(AViewId)->load(is);

  // Les paramètres de précompile
  TPrecompile aprecompile;
  for (aprecompile = FIRST_PRECOMPILE_CONTROLER;
       aprecompile < FLastPrecompile; ++aprecompile)
    {
      loadOnePrecompileParameter(AViewId, aprecompile, is);
    }
  
  is.close();
}
//******************************************************************************
void CControler::reinitAllParameters(TViewId AViewId)
{
  // Les paramètres de view : 1 par CView
  getParameterAimedPosition(AViewId)->reinit();
  getParameterDrawing(AViewId)->reinit();
  getParameterEyePosition(AViewId)->reinit();
  
  // Les paramètres de précompile
  CParameter* parameter;  
  TPrecompile aprecompile;
  for (aprecompile = FIRST_PRECOMPILE_CONTROLER;
       aprecompile < FLastPrecompile; ++aprecompile)
    {
      parameter = getParameterPrecompile(AViewId, aprecompile);
      if (parameter!=NULL) parameter->reinit();
    }
}
//******************************************************************************
