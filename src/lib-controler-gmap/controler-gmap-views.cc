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
#include "controler-gmap.hh"
#include "g-map-vertex.hh"

#include "view-precompile.hh"
#include "view-precompile-ortho.hh"
#include "view-precompile-perspective.hh"

#include "precompile-aimed-point.hh"
#include "precompile-axis.hh"
#include "precompile-dart.hh"
#include "precompile-face.hh"
#include "precompile-grid.hh"
#include "precompile-normal-vector.hh"
#include "precompile-object-transformation.hh"
#include "precompile-preview.hh"
#include "precompile-sew.hh"
#include "precompile-vertex.hh"

#ifdef MODULE_SPAMOD
#include "precompile-spamod.hh"
#endif // MODULE_SPAMOD

using namespace std;
using namespace GMap3d;
//******************************************************************************
void CControlerGMap::viewEnablePreviewPrecompile(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompilePreview * precompile =
    static_cast<CPrecompilePreview *>(FViews[AViewId]->
				      findPrecompile(PRECOMPILE_PREVIEW));

  if ( precompile!=NULL )
    {
      precompile->enable();
      precompile->setCurrentMode(FCurrentMode);

      // On fixe le paramètre FParameterObjectPosition du précompile
      // (permet d'avoir un paramètre différent pour chaque objet).
      int index = FCurrentMode - MODE_CREATION_POLYGON;
      if ( index>=0 && index<NB_PREDEFINED_OBJECTS )
	precompile->setPosition(FParameterObjectPosition[index]);
    }
}
//******************************************************************************
void CControlerGMap::viewDisablePreviewPrecompile(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompile * precompile =
    FViews[AViewId]->findPrecompile(PRECOMPILE_PREVIEW);

  if ( precompile!=NULL ) precompile->disable();
}
//******************************************************************************
void CControlerGMap::viewEnableObjectTransformationPrecompile(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompileObjectTransformation * precompile =
    static_cast<CPrecompileObjectTransformation *>(FViews[AViewId]->
                findPrecompile(PRECOMPILE_OBJECT_TRANSFORMATION));

  if ( precompile!=NULL )
    {
      precompile->enable();
      precompile->setCurrentMode(FCurrentMode);
    }
}
//******************************************************************************
void CControlerGMap::viewDisableObjectTransformationPrecompile(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  if ( FViews[AViewId]==NULL ) return;

  CPrecompile * precompile =
    FViews[AViewId]->findPrecompile(PRECOMPILE_OBJECT_TRANSFORMATION);

  if ( precompile!=NULL ) precompile->disable();
}
//******************************************************************************
void CControlerGMap::viewUpdate(TViewId AViewId)
{
  assert( AViewId<FViews.size() );

  updateModelIfNeeded    ();
  updateSelectionIfNeeded();

  CControler::viewUpdate(AViewId);
}
//******************************************************************************
TCoordinate CControlerGMap::getMapBurstCoef(int ADim) const
{ return FMap->getBurstCoef(ADim); }
//******************************************************************************
void CControlerGMap::setMapBurstCoef(int ADim, TCoordinate ACoef)
{
  if ( ACoef!=FMap->getBurstCoef(ADim) )
    {
      FMap->setBurstCoef(ADim, ACoef);
      if ( FParameterGMapVertex->getMapEmbedding()!=NULL )
	FParameterGMapVertex->getMapEmbedding()->setBurstCoef(ADim, ACoef);
      setModelChanged();
    }
}
//******************************************************************************
void CControlerGMap::setMapBurstCoefs(TCoordinate ACoef0, TCoordinate ACoef1,
				      TCoordinate ACoef2,TCoordinate ACoef3)
{
  if ( ACoef0!=FMap->getBurstCoef(0) ||  ACoef1!=FMap->getBurstCoef(1) ||
       ACoef2!=FMap->getBurstCoef(2) ||  ACoef3!=FMap->getBurstCoef(3) )
    {
      FMap->setBurstCoef(0, ACoef0);
      FMap->setBurstCoef(1, ACoef1);
      FMap->setBurstCoef(2, ACoef2);
      FMap->setBurstCoef(3, ACoef3);
      if ( FParameterGMapVertex->getMapEmbedding()!=NULL )
	{
	  FParameterGMapVertex->getMapEmbedding()->setBurstCoef(0, ACoef0);
	  FParameterGMapVertex->getMapEmbedding()->setBurstCoef(1, ACoef1);
	  FParameterGMapVertex->getMapEmbedding()->setBurstCoef(2, ACoef2);
	  FParameterGMapVertex->getMapEmbedding()->setBurstCoef(3, ACoef3);
	}
      setModelChanged();
    }  
}
//******************************************************************************
void CControlerGMap::setBurstMethod(TBurstMethod AMethod)
{
  FParameterGMapVertex->setBurstMethod(AMethod);
}
//------------------------------------------------------------------------------
TBurstMethod CControlerGMap::getBurstMethod() const
{
  return FParameterGMapVertex->getBurstMethod();
}
//******************************************************************************
bool CControlerGMap::isModelBlocked() const
{
  assert( getParameterGMapVertex()!=NULL );
  return getParameterGMapVertex()->getBlocked();
}
//------------------------------------------------------------------------------
void CControlerGMap::setModelBlocked()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->setBlocked();
}
//------------------------------------------------------------------------------
void CControlerGMap::unsetModelBlocked()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->unsetBlocked();
}
//------------------------------------------------------------------------------
void CControlerGMap::toggleModelBlocked()
{
  if ( isModelBlocked() ) unsetModelBlocked();
  else                    setModelBlocked();
}
//******************************************************************************
bool CControlerGMap::isModeSimplification() const
{
  assert( getParameterGMapVertex()!=NULL );
  return getParameterGMapVertex()->getModeSimplification();
}
//------------------------------------------------------------------------------
void CControlerGMap::setModeSimplification()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->setModeSimplification();
}
//------------------------------------------------------------------------------
void CControlerGMap::unsetModeSimplification()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->unsetModeSimplification();
}
//------------------------------------------------------------------------------
void CControlerGMap::toggleModeSimplification()
{
  if ( isModeSimplification() ) unsetModeSimplification();
  else                          setModeSimplification();
}
//******************************************************************************
