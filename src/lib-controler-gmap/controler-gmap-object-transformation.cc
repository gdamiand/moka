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
#include "parameter-object-translation.hh"
#include "parameter-object-rotation.hh"
#include "parameter-object-scale.hh"
#include "parameter-object-ponderation.hh"
#include "parameter-selection.hh"
#include "view-precompile.hh"
#include "vertex.hh"
#include "vector.hh"
#include "g-map-vertex.hh"
#include "geometry.hh"
using namespace GMap3d;
//******************************************************************************
void CControlerGMap::setObjectTranslationVector(const CVertex & AVector)
{
  if ( AVector!= FParameterTranslation->getVector() )
    {
      FParameterTranslation->setVector(AVector);
      if (FCurrentMode==MODE_TRANSLATION)
	{
	  FMap->translate(FDeformationMark, AVector,
			  FDirectInfoVertex, FDirectInfoPonderation);
	  setPartialModelChanged();
	}
    }
}
//******************************************************************************
CVertex CControlerGMap::getObjectTranslationVector() const
{ return FParameterTranslation->getVector(); }
//******************************************************************************
void CControlerGMap::setObjectTranslationPonderation(bool AActive)
{
  if (FParameterTranslation->getPonderation()!=AActive)
    {
      FParameterTranslation->setPonderation(AActive);
      if (FCurrentMode==MODE_TRANSLATION)
	{
	  if (FParameterTranslation->getPonderation())
	    {
	      addPonderationCoefs();
	      updatePonderationCoefs();
	    }
	  else
	    delPonderationCoefs();

	  FMap->updateDirectInfoWithVertex(FDirectInfoVertex);
	}
    }
}
//******************************************************************************
bool CControlerGMap::getObjectTranslationPonderation() const
{ return FParameterTranslation->getPonderation(); }
//******************************************************************************
void CControlerGMap::toggleObjectTranslationPonderation()
{ setObjectTranslationPonderation(!getObjectTranslationPonderation()); }
//******************************************************************************
void CControlerGMap::setObjectRotationAxeVertex(const CVertex & AAxeVertex)
{
  if ( FCurrentModeOperation==MODE_OPERATION_NONE )
    {
      FParameterRotation->setAxeVertex(AAxeVertex);
      FParameterRotation->setAngle(0.0);
    }
}
//******************************************************************************
CVertex CControlerGMap::getObjectRotationAxeVertex() const
{ return FParameterRotation->getAxeVertex(); }
//******************************************************************************
void CControlerGMap::setObjectRotationAxeVector(const CVertex & AAxeVector)
{
  if ( FCurrentModeOperation==MODE_OPERATION_NONE )
    {
      FParameterRotation->setAxeVector(AAxeVector);
      FParameterRotation->setAngle(0.0);
    }
}
//******************************************************************************
CVertex CControlerGMap::getObjectRotationAxeVector() const
{ return FParameterRotation->getAxeVector(); }
//******************************************************************************
void CControlerGMap::setObjectRotationAngle(float AAngle)
{
  if (AAngle!=FParameterRotation->getAngle())
    {
      FParameterRotation->setAngle(AAngle);
      if (FCurrentMode==MODE_ROTATION)
	{
	  FMap->rotate(FDeformationMark,  FParameterRotation->getAxeVertex(),
		       FParameterRotation->getAxeVector(),
		       AAngle, FDirectInfoVertex, FDirectInfoPonderation);
	  setPartialModelChanged();
	}
    }
}
//******************************************************************************
float CControlerGMap::getObjectRotationAngle() const
{ return FParameterRotation->getAngle(); }
//******************************************************************************
void CControlerGMap::setObjectRotationPonderation(bool AActive)
{
  if (FParameterRotation->getPonderation()!=AActive)
    {
      FParameterRotation->setPonderation(AActive);
      if (FCurrentMode==MODE_ROTATION)
	{
	  if (FParameterRotation->getPonderation())
	    {
	      addPonderationCoefs();
	      updatePonderationCoefs();
	    }
	  else
	    delPonderationCoefs();

	  FMap->updateDirectInfoWithVertex(FDirectInfoVertex);
	}
    }
}
//******************************************************************************
bool CControlerGMap::getObjectRotationPonderation() const
{ return FParameterRotation->getPonderation(); }
//******************************************************************************
void CControlerGMap::toggleObjectRotationPonderation()
{ setObjectRotationPonderation(!getObjectRotationPonderation()); }
//******************************************************************************
void CControlerGMap::setObjectScaleType(TScaleType AScaleType)
{
  if ( FCurrentModeOperation==MODE_OPERATION_NONE )
    {
      FParameterScale->setScaleType(AScaleType);
    }
}
//******************************************************************************
TScaleType CControlerGMap::getObjectScaleType() const
{ return FParameterScale->getScaleType(); }
//******************************************************************************
void CControlerGMap::setObjectScaleIso(bool AValue)
{
  if ( AValue!=FParameterScale->getIsoScale() )
    {
      FParameterScale->setIsoScale(AValue);
      FParameterScale->setIsoCoef(1.0);

      FMap->updateDirectInfoWithVertex(FDirectInfoVertex);
    }
}
//******************************************************************************
bool CControlerGMap::getObjectScaleIso() const
{ return FParameterScale->getIsoScale(); }
//******************************************************************************
void CControlerGMap::toggleObjectScaleIso()
{ setObjectScaleIso(!getObjectScaleIso()); }
//******************************************************************************
void CControlerGMap::setObjectScaleCenter(const CVertex & ACenter)
{
  if ( FCurrentModeOperation==MODE_OPERATION_NONE )
    {
      FParameterScale->setCenter(ACenter);
    }
}
//******************************************************************************
CVertex CControlerGMap::getObjectScaleCenter() const
{ return FParameterScale->getCenter(); }
//******************************************************************************
void CControlerGMap::setObjectScaleVector(const CVertex & AVector)
{
  if ( FCurrentModeOperation==MODE_OPERATION_NONE )
    {
      FParameterScale->setVector(AVector);
    }
}
//******************************************************************************
CVertex CControlerGMap::getObjectScaleVector() const
{ return FParameterScale->getVector(); }
//******************************************************************************
void CControlerGMap::setObjectScaleIsoCoef(float AValue)
{
  if ( AValue!=FParameterScale->getIsoCoef() )
    {
      FParameterScale->setIsoCoef(AValue);
      if (FCurrentMode==MODE_SCALE)
	{
	  FMap->scale(FDeformationMark, FParameterScale->getCenter(),
		      CVertex(AValue,AValue,AValue),
		      FDirectInfoVertex, FDirectInfoPonderation);
	  setPartialModelChanged();
	}
    }
}
//******************************************************************************
float CControlerGMap::getObjectScaleIsoCoef() const
{ return FParameterScale->getIsoCoef(); }
//******************************************************************************
void CControlerGMap::setObjectScaleCoef(const CVertex & AValue)
{
  if ( AValue!=FParameterScale->getCoef() )
    {
      FParameterScale->setCoef(AValue);
      if (FCurrentMode==MODE_SCALE)
	{
	  FMap->scale(FDeformationMark, FParameterScale->getCenter(),
		      AValue, FDirectInfoVertex, FDirectInfoPonderation);
	  setPartialModelChanged();
	}
    }
}
//******************************************************************************
CVertex CControlerGMap::getObjectScaleCoef() const
{ return FParameterScale->getCoef(); }
//******************************************************************************
void CControlerGMap::setObjectScaleCoef(int ADim, float AValue)
{
  if ( AValue!=FParameterScale->getCoef(ADim) )
    {
      FParameterScale->setCoef(ADim,AValue);
      if (FCurrentMode==MODE_SCALE)
	{
	  FMap->scale(FDeformationMark, FParameterScale->getCenter(),
		      getObjectScaleCoef(), FDirectInfoVertex,
		      FDirectInfoPonderation);
	  setPartialModelChanged();
	}
    }
}
//******************************************************************************
float CControlerGMap::getObjectScaleCoef(int ADim) const
{ return FParameterScale->getCoef(ADim); }
//******************************************************************************
void CControlerGMap::setObjectScalePonderation(bool AActive)
{
  if (FParameterScale->getPonderation()!=AActive)
    {
      FParameterScale->setPonderation(AActive);
      if (FCurrentMode==MODE_SCALE)
	{
	  if (FParameterScale->getPonderation())
	    {
	      addPonderationCoefs();
	      updatePonderationCoefs();
	    }
	  else
	    delPonderationCoefs();

	  FMap->scale(FDeformationMark,  FParameterScale->getCenter(),
		      FParameterScale->getCoef(), FDirectInfoVertex,
		      FDirectInfoPonderation);
	  setPartialModelChanged();
	}
    }
}
//******************************************************************************
bool CControlerGMap::getObjectScalePonderation() const
{ return FParameterScale->getPonderation(); }
//******************************************************************************
void CControlerGMap::toggleObjectScalePonderation()
{ setObjectScalePonderation(!getObjectScalePonderation()); }
//******************************************************************************
void CControlerGMap::modeObjectTransformationBegin(bool APonderation)
{
  FDeformationDone = false;
  basicPreSave();
  FDeformationMark = getSelectionMark();

  FDirectInfoVertex = FMap->getNewDirectInfo();
  FMap->duplicateVertexToDirectInfo(FDeformationMark, FDirectInfoVertex);

  assert(getParameterGMapVertex()->getMarkPartialChanged() == -1);
  getParameterGMapVertex()->setMarkPartialChanged(FMap->getNewMark());

  FMap->markIncidentCells(ORBIT_VOLUME, FDeformationMark,
			  getParameterGMapVertex()->getMarkPartialChanged());
  FMap->markIncidentCells(ORBIT_VERTEX,
			  getParameterGMapVertex()->getMarkPartialChanged());
  FMap->markIncidentCells(ORBIT_VOLUME,
			  getParameterGMapVertex()->getMarkPartialChanged());

  if (APonderation)
    {
      addPonderationCoefs();
      updatePonderationCoefs();
    }

  for (unsigned int i=0; i<FViews.size(); ++i)
    viewEnableObjectTransformationPrecompile(i);
}
//------------------------------------------------------------------------------
void CControlerGMap::modeTranslationBegin()
{
  modeObjectTransformationBegin(FParameterTranslation->getPonderation());
  setMessage("Translation mode");
  FParameterTranslation->setVector(ORIGIN);
}
//------------------------------------------------------------------------------
void CControlerGMap::modeRotationBegin()
{
  modeObjectTransformationBegin(FParameterRotation->getPonderation());
  setMessage("Rotation mode");
  CVertex bary = FMap->directInfoBarycenter(FDirectInfoVertex);
  FParameterRotation->setAxeVertex(bary);
}
//------------------------------------------------------------------------------
void CControlerGMap::modeScaleBegin()
{
  modeObjectTransformationBegin(FParameterScale->getPonderation());
  setMessage("Scaling mode");
  FParameterScale->setIsoCoef(1.0);
  FParameterScale->setCenter(FMap->directInfoBarycenter(FDirectInfoVertex));
}
//******************************************************************************
void CControlerGMap::modeObjectTransformationEnd(bool APonderation)
{
  FMap->deleteDuplicatedVertex(FDirectInfoVertex);
  FMap->freeDirectInfo(FDirectInfoVertex);

  FMap->unmarkAll(getParameterGMapVertex()->getMarkPartialChanged());
  FMap->freeMark(getParameterGMapVertex()->getMarkPartialChanged());
  getParameterGMapVertex()->setMarkPartialChanged(-1);

  if (APonderation)
    delPonderationCoefs();

  for (unsigned int i=0; i<FViews.size(); ++i)
    viewDisableObjectTransformationPrecompile(i);

  if (FDeformationDone)
    basicPostSaveOk();
  else
    basicPostSaveFailed();
}
//------------------------------------------------------------------------------
void CControlerGMap::modeTranslationEnd()
{
  modeObjectTransformationEnd(FParameterTranslation->getPonderation());
}
//------------------------------------------------------------------------------
void CControlerGMap::modeRotationEnd()
{
  modeObjectTransformationEnd(FParameterRotation->getPonderation());
}
//------------------------------------------------------------------------------
void CControlerGMap::modeScaleEnd()
{
  modeObjectTransformationEnd(FParameterScale->getPonderation());
}
//******************************************************************************
void CControlerGMap::modeTranslationOperationStart()
{
  CViewPrecompile * view = FViews[FCurrentViewId];
  assert(view != NULL);

  view->unproject(FFirstX, FFirstY, FFirst3D);

  FMap->updateDirectInfoWithVertex(FDirectInfoVertex);
}
//******************************************************************************
void CControlerGMap::modeRotationOperationStart()
{
  CViewPrecompile * view = FViews[FCurrentViewId];
  assert(view != NULL);

  view->unproject(FFirstX, FFirstY, FFirst3D);

  FParameterRotation->setAxeVector(view->getEyeDirection());

  FMap->updateDirectInfoWithVertex(FDirectInfoVertex);
}
//******************************************************************************
void CControlerGMap::modeScaleOperationStart()
{
  CViewPrecompile * view = FViews[FCurrentViewId];
  assert(view != NULL);

  view->unproject(FFirstX, FFirstY, FFirst3D);

  FMap->updateDirectInfoWithVertex(FDirectInfoVertex);
}
//******************************************************************************
void CControlerGMap::modeTranslationOperationMove()
{
  float last3D[3];
  FViews[FCurrentViewId]->unproject(FLastX , FLastY , last3D);

  CVertex vect= CVertex(last3D[0], last3D[1], last3D[2]) -
    CVertex(FFirst3D[0], FFirst3D[1], FFirst3D[2]);
  
  FDeformationDone |= !vect.isNull();
  setObjectTranslationVector(vect);
}
//******************************************************************************
void CControlerGMap::modeRotationOperationMove()
{
  float last3D[3];
  FViews[FCurrentViewId]->unproject(FLastX , FLastY , last3D);

  CVertex R =
    CGeometry::orthoProjectVertexOnPlane(FParameterRotation->getAxeVertex(),
					 FViews[FCurrentViewId]->getLookAt(),
					 FViews[FCurrentViewId]->getEyeDirection());

  float newAngle = CGeometry::getAngle(CVertex(FFirst3D[0], FFirst3D[1], FFirst3D[2])-R,
				       CVertex(last3D[0], last3D[1], last3D[2])-R,
				       FViews[FCurrentViewId]->getEyeDirection());

  FDeformationDone |= !isZero(newAngle);
  setObjectRotationAngle(newAngle);
}
//******************************************************************************
void CControlerGMap::modeScaleOperationMove()
{
  CViewPrecompile * view = FViews[FCurrentViewId];

  if (FParameterScale->getIsoScale())
    {
      float coef = (FLastX-FFirstX+FLastY-FFirstY)/100.0 + 1.0;

      FDeformationDone |= !areEqual(coef, 1.0);

      setObjectScaleIsoCoef(coef);
    }
  else
    {
      float last3D[3];
      view->unproject(FLastX , FLastY , last3D);

      CVertex R =
	CGeometry::orthoProjectVertexOnPlane(FParameterScale->getCenter(),
					     view->getLookAt() +
					     view->getEyeDirection(),
					     view->getEyeDirection());

      CVertex vect = CVertex(FFirst3D[0], FFirst3D[1], FFirst3D[2])-R;

      for (int i=0;i<3;++i)
	if (!isZero(vect.getCoord(i)))
	  vect.setCoord(i,1/vect.getCoord(i));

      vect = (CVertex(last3D[0], last3D[1], last3D[2])-R).multiply(vect);

      FDeformationDone |= ( vect != CVertex(1,1,1) );

      setObjectScaleCoef(vect);
    }
}
//******************************************************************************
void CControlerGMap::modeTransformationOperationStop()
{}
//******************************************************************************
void CControlerGMap::addPonderationCoefs()
{
  FDirectInfoPonderation = FMap->getNewDirectInfo();
  FMap->allocCoordToDirectInfo(getSelectionMark(),
			       FDirectInfoPonderation,
			       ORBIT_VERTEX);
}
//******************************************************************************
void CControlerGMap::delPonderationCoefs()
{
  FMap->freeMemoryFromDirectInfo(getSelectionMark(),
				 FDirectInfoPonderation,
				 ORBIT_VERTEX);
  FMap->freeDirectInfo(FDirectInfoPonderation);
  FDirectInfoPonderation = -1;
}
//******************************************************************************
void CControlerGMap::updatePonderationCoefs()
{
  CVertex center = FParameterPonderation->getPonderationCenter();
  CVertex vect = FParameterPonderation->getPonderationVector();

  if (vect.isNull())
    vect = OZ;

  switch (FParameterPonderation->getPonderationType())
    {
    case PONDERATION_POINT:
      FMap->computeDistanceToVertex(getSelectionMark(),
				    FDirectInfoPonderation, center);
      break;

    case PONDERATION_AXE:
      FMap->computeDistanceToLine(getSelectionMark(),
				  FDirectInfoPonderation, center, vect);
      break;

    case PONDERATION_PLANE:
      FMap->computeDistanceToPlane(getSelectionMark(),
				   FDirectInfoPonderation, center, vect);
      break;

    default:
      return;
    }

  if (FParameterPonderation->getPonderationStretchIn())
    FMap->normalizeParameter(getSelectionMark(),
			     FDirectInfoPonderation,
			     FParameterPonderation->
			     getPonderationStretchInMin(),
			     FParameterPonderation->
			     getPonderationStretchInMax());

  FMap->applyFunctionOnParameter(getSelectionMark(),
				 FDirectInfoPonderation,
				 FParameterPonderation->
				 getPonderationFunctionType());

  if (FParameterPonderation->getPonderationStretchOut())
    FMap->normalizeParameter(getSelectionMark(),
			     FDirectInfoPonderation,
			     FParameterPonderation->
			     getPonderationStretchOutMin(),
			     FParameterPonderation->
			     getPonderationStretchOutMax());
}
//******************************************************************************
CVertex CControlerGMap::getVertexPosition() const
{
  if ( getLastSelectedDart()==NULL ) return ORIGIN;
  return *FMap->findVertex(getLastSelectedDart());
}
//------------------------------------------------------------------------------
void CControlerGMap::setVertexPosition(const CVertex& AVertex)
{
  if ( getLastSelectedDart()!=NULL && getVertexPosition()!=AVertex)
    {
      undoRedoPreSave();
      FMap->translate(getSelectionMark(), AVertex-getVertexPosition());
      undoRedoPostSaveOk();
    
      setModelChanged();
    }
}
//******************************************************************************
