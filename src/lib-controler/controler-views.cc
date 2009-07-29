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
#include "view-precompile-ortho.hh"
#include "view-precompile-perspective.hh"

#include "precompile-aimed-point.hh"
#include "precompile-axis.hh"
#include "precompile-grid.hh"

using namespace std;
//******************************************************************************
TViewId CControler::viewCreate(TView AViewType)
{
  TViewId id = 0;
  vector<CViewPrecompile *>::iterator it = FViews.begin();
  while ( FViews[id]!=NULL && it!=FViews.end() )
    {
      ++it; ++id;
    }

  if ( it==FViews.end() ) FViews.push_back(NULL);

  CParameterEyePosition   * eyePosition   = new CParameterEyePosition  ;
  CParameterAimedPosition * aimedPosition = new CParameterAimedPosition;
  CParameterDrawing       * sceneDrawing  = new CParameterDrawing      ;

  FViews[id] = newViewPrecompile(AViewType, eyePosition,
				 aimedPosition, sceneDrawing);

  assert( FViews[id]!=NULL );

  return id;
}
//******************************************************************************
TViewId CControler::viewCreateShare(TView AViewType, TViewId AView)
{
  TViewId id = viewCreate( AViewType );
  viewGroup( AView, id );
  return id;
}
//******************************************************************************
void CControler::viewDestroy(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]!=NULL )
    {
      delete FViews[AViewId];
      FViews[AViewId] = NULL;
    }
}
//******************************************************************************
void CControler::viewAddPrecompile(TViewId AViewId, TPrecompile APrecompileType)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ||
       FViews[AViewId]->findPrecompile(APrecompileType)!=NULL )
    return;

  CPrecompile* precompile = newPrecompile(AViewId,APrecompileType);

  assert( precompile!=NULL );

  FViews[AViewId]->addPrecompile(precompile);
}
//******************************************************************************
void CControler::viewRemovePrecompile(TViewId AViewId,
				      TPrecompile APrecompileType)
{
  assert( AViewId<FViews.size());

  if ( FViews[AViewId]!=NULL )
    FViews[AViewId]->removePrecompile(APrecompileType);
}
//******************************************************************************
void CControler::viewAddOrRemovePrecompile(TViewId AViewId, TPrecompile APrecom)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompile * precompile = FViews[AViewId]->findPrecompile(APrecom);
  if ( precompile==NULL )
    viewAddPrecompile(AViewId,APrecom);
  else
    FViews[AViewId]->removePrecompile(APrecom);
}
//******************************************************************************
bool CControler::viewIsEnabledPrecompile(TViewId AViewId, TPrecompile APrecom)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return false;

  CPrecompile * precompile = FViews[AViewId]->findPrecompile(APrecom);

  return ( precompile!=NULL && precompile->isEnabled() );
}
//******************************************************************************
void CControler::viewEnablePrecompile(TViewId AViewId, TPrecompile APrecom)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompile * precompile = FViews[AViewId]->findPrecompile(APrecom);

  if ( precompile!=NULL ) precompile->enable();
}
//******************************************************************************
void CControler::viewDisablePrecompile(TViewId AViewId, TPrecompile APrecom)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompile * precompile = FViews[AViewId]->findPrecompile(APrecom);

  if ( precompile!=NULL ) precompile->disable();
}
//******************************************************************************
void CControler::viewTogglePrecompile(TViewId AViewId, TPrecompile APrecom)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompile * precompile = FViews[AViewId]->findPrecompile(APrecom);
  if (precompile!=NULL )
    {
      if (precompile->isEnabled()) precompile->disable();
      else precompile->enable();
    }
  else
    viewAddPrecompile(AViewId, APrecom);
}
//******************************************************************************
void CControler::viewUpdate(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if (FViews[AViewId]==NULL) return;
  FViews[AViewId]->update();
}
//******************************************************************************
void CControler::viewInit(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if (FViews[AViewId]==NULL) return;

  FViews[AViewId]->initScene();
}
//******************************************************************************
void CControler::viewDraw(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if (FViews[AViewId]==NULL) return;

  viewUpdate(AViewId);
  FViews[AViewId]->drawScene();
}
//******************************************************************************
void CControler::viewGroup(TViewId AViewId1, TViewId AViewId2)
{
  assert( AViewId1<FViews.size() );
  assert( AViewId2<FViews.size() );

  if ( FViews[AViewId1]==NULL || FViews[AViewId2]==NULL ||  AViewId1==AViewId2 )
    return;

  // 1. On groupe tout les précompiles.
  viewGroupPrecompiles(AViewId1,AViewId2);

  // 2. Et on groupe les paramètres de vue.
  viewGroupParameter(AViewId1, AViewId2, PARAMETER_DRAWING);
  viewGroupParameter(AViewId1, AViewId2, PARAMETER_EYE_POSITION);
  viewGroupParameter(AViewId1, AViewId2, PARAMETER_AIMED_POSITION);
}
//******************************************************************************
void CControler::viewGroupAll(TViewId AViewId)
{
  for (unsigned int i=0; i<FViews.size(); ++i)
    viewGroup(AViewId,i);
}
//******************************************************************************
void CControler::viewUngroup(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]!=NULL )
    {
      // 1. On dégroupe tout les précompiles.
      viewUngroupPrecompiles(AViewId);

      // 2. Et on dégroupe les paramètres de vue.
      viewUngroupParameter(AViewId, PARAMETER_DRAWING);
      viewUngroupParameter(AViewId, PARAMETER_EYE_POSITION);
      viewUngroupParameter(AViewId, PARAMETER_AIMED_POSITION);
    }
}
//******************************************************************************
void CControler::viewUngroupAll()
{
  for (unsigned int i=1; i<FViews.size(); ++i)
    viewUngroup(i);
}
//******************************************************************************
void CControler::viewGroupPrecompile(TViewId AViewId1, TViewId AViewId2,
				     TPrecompile APrecompileType)
{
  assert( AViewId1<FViews.size() );
  assert( AViewId2<FViews.size() );

  if ( APrecompileType==PRECOMPILE_AIMED_POINT ) return;

  if ( FViews[AViewId1]==NULL || FViews[AViewId2]==NULL || AViewId1==AViewId2 )
    return;

  CPrecompile * precompile1 = FViews[AViewId1]->findPrecompile(APrecompileType);
  CPrecompile * precompile2 = FViews[AViewId2]->findPrecompile(APrecompileType);

  if ( precompile1==precompile2 )
    return;

  if ( precompile1!=NULL )
    {
      if ( precompile2!=NULL )  // 2 précompiles => Suppresion du deuxième.
	FViews[AViewId2]->removePrecompile(APrecompileType);

      FViews[AViewId2]->addPrecompile(precompile1);
    }
  else // Cas où precompile1==NULL && precompile2!=NULL
    FViews[AViewId1]->addPrecompile(precompile2);
}
//******************************************************************************
void CControler::viewGroupPrecompiles(TViewId AViewId1, TViewId AViewId2)
{
  if ( AViewId1==AViewId2 ) return;

  for (TPrecompile i=FIRST_PRECOMPILE_CONTROLER; i<FLastPrecompile; ++i)
    viewGroupPrecompile(AViewId1, AViewId2, i);
 }
//******************************************************************************
void CControler::viewGroupAllPrecompiles(TViewId AViewId)
{
  for (unsigned int i=0; i<FViews.size(); ++i)
    viewGroupPrecompiles(AViewId, i);
}
//******************************************************************************
void CControler::viewUngroupPrecompile(TViewId AViewId,
				       TPrecompile APrecompileType)
{
  assert( AViewId<FViews.size() );

  if ( APrecompileType==PRECOMPILE_AIMED_POINT ) return;

  if ( FViews[AViewId]!=NULL )
    {
      CPrecompile * precompile = FViews[AViewId]->findPrecompile(APrecompileType);

      if ( precompile!=NULL && precompile->getNbView()>1 )
	{
	  FViews[AViewId]->removePrecompile(APrecompileType);
	  FViews[AViewId]->addPrecompile( precompile->copy() );
	}
    }
}
//******************************************************************************
void CControler::viewUngroupPrecompiles(TViewId AViewId)
{
  for (TPrecompile i=FIRST_PRECOMPILE_CONTROLER; i<FLastPrecompile; ++i)
    viewUngroupPrecompile(AViewId, i);
 }
//******************************************************************************
void CControler::viewUngroupAllPrecompiles()
{
  for (unsigned int i=1; i<FViews.size(); ++i)
    viewUngroupPrecompiles(i);
}
//******************************************************************************
void CControler::viewGroupParameter(TViewId AViewId1, TViewId AViewId2,
				    TParameter AParameterType)
{
  assert( AViewId1<FViews.size() );
  assert( AViewId2<FViews.size() );

  if ( FViews[AViewId1]==NULL || FViews[AViewId2]==NULL || AViewId1==AViewId2)
    return;

  FViews[AViewId1]->groupParameter(FViews[AViewId2], AParameterType);
}
//******************************************************************************
void CControler::viewGroupAllParameter(TViewId AViewId,
				       TParameter AParameterType)
{
  for (unsigned int i=0; i<FViews.size(); ++i)
    viewGroupParameter(AViewId, i, AParameterType);
}
//******************************************************************************
void CControler::viewUngroupParameter(TViewId AViewId, TParameter AParameterType)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]!=NULL )
    FViews[AViewId]->ungroupParameter(AParameterType);
}
//******************************************************************************
void CControler::viewUngroupAllParameter(TParameter AParameterType)
{
  for (unsigned int i=1; i<FViews.size(); ++i)
    viewUngroupParameter(i, AParameterType);
}
//******************************************************************************
bool CControler::viewIsEnabled(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL )
    return false;

  return FViews[AViewId]->isEnabled();
}
//******************************************************************************
void CControler::viewEnable(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]!=NULL )
    FViews[AViewId]->enable();
}
//******************************************************************************
void CControler::viewDisable(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]!=NULL )
    FViews[AViewId]->disable();
}
//******************************************************************************
