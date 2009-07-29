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
#include "precompile-object-transformation.hh"
#include "parameter-object-transformation.hh"
#include "parameter-object-translation.hh"
#include "parameter-object-rotation.hh"
#include "parameter-object-scale.hh"
#include "parameter-object-ponderation.hh"
#include "geometry.hh"
#include "transformation-matrix.hh"
#include "vertex.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CPrecompileObjectTransformation::
CPrecompileObjectTransformation( CParameterObjectTransformation *
				 AParameterObjectTransformation,
				 CParameterObjectTranslation *
				 AParameterObjectTranslation,
				 CParameterObjectRotation *
				 AParameterObjectRotation,
				 CParameterObjectScale *
				 AParameterObjectScale,
				 CParameterObjectPonderation *
				 AParameterObjectPonderation) :
  FParameterObjectTransformation(AParameterObjectTransformation),
  FParameterObjectTranslation	(AParameterObjectTranslation),
  FParameterObjectRotation	(AParameterObjectRotation),
  FParameterObjectScale		(AParameterObjectScale),
  FParameterObjectPonderation	(AParameterObjectPonderation),
  FCurrentMode			(MODE_DEFAULT)
{
  assert(FParameterObjectTransformation!=NULL);

  FParameterObjectTransformation->addPrecompileToUpdate(this);

  if (FParameterObjectTranslation!=NULL)
     FParameterObjectTranslation->addPrecompileToUpdate(this);

  if (FParameterObjectRotation!=NULL)
    FParameterObjectRotation->addPrecompileToUpdate(this);

  if (FParameterObjectScale!=NULL)
    FParameterObjectScale->addPrecompileToUpdate(this);

  if (FParameterObjectPonderation!=NULL)
    FParameterObjectPonderation->addPrecompileToUpdate(this);

  disable();
}
//******************************************************************************
CPrecompileObjectTransformation::
CPrecompileObjectTransformation( const CPrecompileObjectTransformation &
				 APrecompile ) :
  CPrecompile                   (APrecompile),
  FParameterObjectTransformation(static_cast<CParameterObjectTransformation*>
				 (APrecompile.FParameterObjectTransformation->copy())),
  FParameterObjectTranslation	(APrecompile.FParameterObjectTranslation),
  FParameterObjectRotation	(APrecompile.FParameterObjectRotation),
  FParameterObjectScale		(APrecompile.FParameterObjectScale),
  FParameterObjectPonderation	(APrecompile.FParameterObjectPonderation),
  FCurrentMode			(APrecompile.FCurrentMode)
{
  assert(FParameterObjectTransformation!=NULL);

  FParameterObjectTransformation->addPrecompileToUpdate(this);

  if (FParameterObjectTranslation!=NULL)
     FParameterObjectTranslation->addPrecompileToUpdate(this);

  if (FParameterObjectRotation!=NULL)
    FParameterObjectRotation->addPrecompileToUpdate(this);

  if (FParameterObjectScale!=NULL)
    FParameterObjectScale->addPrecompileToUpdate(this);

  if (FParameterObjectPonderation!=NULL)
    FParameterObjectPonderation->addPrecompileToUpdate(this);

  disable();
}
//******************************************************************************
CPrecompileObjectTransformation::~CPrecompileObjectTransformation()
{
  FParameterObjectTransformation->removePrecompileToUpdate(this);

  if (FParameterObjectTranslation!=NULL)
    FParameterObjectTranslation->removePrecompileToUpdate(this);

  if (FParameterObjectRotation!=NULL)
    FParameterObjectRotation->removePrecompileToUpdate(this);

  if (FParameterObjectScale!=NULL)
    FParameterObjectScale->removePrecompileToUpdate(this);

  if (FParameterObjectPonderation!=NULL)
    FParameterObjectPonderation->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileObjectTransformation::copy() const
{ return new CPrecompileObjectTransformation(*this); }
//******************************************************************************
void CPrecompileObjectTransformation::setCurrentMode(TMode AMode)
{
  if ( AMode!=FCurrentMode )
    {
      FCurrentMode = AMode;
      setToUpdate();
    }
}
//******************************************************************************
void CPrecompileObjectTransformation::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_OBJECT_TRANSFORMATION:
      setObjectTransformation(static_cast<CParameterObjectTransformation *>(AParameter));
      break;
    case PARAMETER_OBJECT_TRANSLATION:
      setObjectTranslation(static_cast<CParameterObjectTranslation *>(AParameter));
      break;
    case PARAMETER_OBJECT_ROTATION:
      setObjectRotation(static_cast<CParameterObjectRotation *>(AParameter));
      break;
    case PARAMETER_OBJECT_SCALE:
      setObjectScale(static_cast<CParameterObjectScale *>(AParameter));
      break;
    case PARAMETER_OBJECT_PONDERATION:
      setObjectPonderation(static_cast<CParameterObjectPonderation *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileObjectTransformation::getParameter() const
{ return FParameterObjectTransformation; }
//******************************************************************************
void CPrecompileObjectTransformation::
setObjectTransformation(CParameterObjectTransformation* AObjectTransformation)
{
  assert(AObjectTransformation != NULL);
  AObjectTransformation->addPrecompileToUpdate(this);
  FParameterObjectTransformation->removePrecompileToUpdate(this);
  FParameterObjectTransformation = AObjectTransformation;
  setToUpdate();
}
//******************************************************************************
void CPrecompileObjectTransformation::
setObjectTranslation(CParameterObjectTranslation* AObjectTranslation)
{
  if (AObjectTranslation != NULL)
    AObjectTranslation->addPrecompileToUpdate(this);

  if (FParameterObjectTranslation!= NULL)
    FParameterObjectTranslation->removePrecompileToUpdate(this);

  FParameterObjectTranslation = AObjectTranslation;
  setToUpdate();
}
//******************************************************************************
void CPrecompileObjectTransformation::
setObjectRotation(CParameterObjectRotation* AObjectRotation)
{
  if (AObjectRotation != NULL)
    AObjectRotation->addPrecompileToUpdate(this);

  if (FParameterObjectRotation!= NULL)
    FParameterObjectRotation->removePrecompileToUpdate(this);

  FParameterObjectRotation = AObjectRotation;
  setToUpdate();
}
//******************************************************************************
void CPrecompileObjectTransformation::
setObjectScale(CParameterObjectScale* AObjectScale)
{
  if (AObjectScale != NULL)
    AObjectScale->addPrecompileToUpdate(this);

  if (FParameterObjectScale!= NULL)
    FParameterObjectScale->removePrecompileToUpdate(this);

  FParameterObjectScale = AObjectScale;
  setToUpdate();
}
//******************************************************************************
void CPrecompileObjectTransformation::
setObjectPonderation(CParameterObjectPonderation* AObjectPonderation)
{
  if (AObjectPonderation != NULL)
    AObjectPonderation->addPrecompileToUpdate(this);

  if (FParameterObjectPonderation!= NULL)
    FParameterObjectPonderation->removePrecompileToUpdate(this);

  FParameterObjectPonderation = AObjectPonderation;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileObjectTransformation::getType() const
{ return PRECOMPILE_OBJECT_TRANSFORMATION; }
//******************************************************************************
#define AXIAL_EDGES (10)
#define PLANAR_EDGES (4)
//******************************************************************************
void CPrecompileObjectTransformation::drawModel()
{
  glLineWidth(FParameterObjectTransformation->getLWObjectTransformation());
  glColor3fv(FParameterObjectTransformation->getCLObjectTransformation());

  switch(FCurrentMode)
    {
    case MODE_TRANSLATION:
      if ( FParameterObjectTranslation!=NULL &&
	   FParameterObjectTranslation->getPonderation() )
	drawInteractivePonderation();
      break;
    case MODE_ROTATION:
      if (FParameterObjectRotation!=NULL)
	{
	  CVertex C = FParameterObjectRotation->getAxeVertex();
	  CVertex V = FParameterObjectRotation->getAxeVector();
	
	  if (!V.isNull())
	    drawInteractiveAxe(C, V, AXIAL_EDGES);
	
	  if ( FParameterObjectRotation->getPonderation() )
	    drawInteractivePonderation();
	}
      break;
    case MODE_SCALE:
      if (FParameterObjectScale!=NULL)
	{
	  CVertex P;
	  CVertex C = FParameterObjectScale->getCenter();
	  CVertex V = FParameterObjectScale->getVector();
	
	  switch (FParameterObjectScale->getScaleType())
	    {
	    case SCALE_POINT:
	      drawInteractiveAxe(C, ORIGIN, 0);
	      break;
	
	    case SCALE_AXE:
	      if (!V.isNull())
		drawInteractiveAxe(C, V, AXIAL_EDGES);
	      break;
	
	    case SCALE_PLANE:
	      if (!V.isNull())
		drawInteractiveAxe(C, V, PLANAR_EDGES);
	      break;
	    }
	
	  if (FParameterObjectScale->getPonderation())
	    drawInteractivePonderation();
	}
      break;
    default:
      {
	drawInteractivePonderation();
      }
    }
}
//******************************************************************************
void CPrecompileObjectTransformation::drawInteractiveAxe(const CVertex & C,
							 const CVertex & V,
							 int circleNbEdges,
							 bool ponderation)
{
  CVertex P;

  if (V.isNull())
    {
#define K (0.1)
      glBegin(GL_LINES);

      if (ponderation)
	{
	  P = C - K*(OX+OY); PLOT(&P);
	  P = C + K*(OX+OY); PLOT(&P);
	
	  P = C - K*(OX-OY); PLOT(&P);
	  P = C + K*(OX-OY); PLOT(&P);
	
	  P = C - K*OZ; PLOT(&P);
	  P = C + K*OZ; PLOT(&P);
	}
      else
	for (int i=0; i<2; ++i)
	  for (int j=0; j<2; ++j)
	    {
	      P = C+K*CVertex(2*i-1,2*j-1,-1); PLOT(&P);
	      P = C-K*CVertex(2*i-1,2*j-1,-1); PLOT(&P);
	    }

      glEnd();
#undef K
      return;
    }
  else
    {
#define N (21) // doit être impair!
      CVertex VV = 2*V/V.norm();

      glBegin(GL_LINES);

      int i = ponderation ? 0 : 1;

      for (; i<=N; i+=2)
	{
	  P = C + VV*(float(i  )/N - 0.5); PLOT(&P);
	  P = C + VV*(float(i+1)/N - 0.5); PLOT(&P);
	}

      glEnd();
#undef N

      CVertex N = CGeometry::getNormalVector(V);
      CVertex E = C + N/N.norm()/circleNbEdges;

      glBegin(GL_LINE_LOOP);
      {
	CTransformationMatrix
	  rotationMatrix(CTransformationMatrix::IdentityMatrix);
	rotationMatrix.rotate(C, V, 360.0/circleNbEdges);
	
	for (int i=0; i<circleNbEdges; ++i)
	  {
	    PLOT(&E);
	    rotationMatrix.applyOn(E);
	  }
      }
      glEnd();
    }
}
//******************************************************************************
void CPrecompileObjectTransformation::drawInteractivePonderation()
{
  if (FParameterObjectPonderation==NULL) return;

  CVertex C = FParameterObjectPonderation->getPonderationCenter();
  CVertex V = FParameterObjectPonderation->getPonderationVector();

  if (V.isNull())
    V = OZ;

  switch (FParameterObjectPonderation->getPonderationType())
    {
    case PONDERATION_POINT:
      drawInteractiveAxe(C, ORIGIN, 0, true);
      break;

    case PONDERATION_AXE:
      drawInteractiveAxe(C, V, AXIAL_EDGES, true);
      break;

    case PONDERATION_PLANE:
      drawInteractiveAxe(C, V, PLANAR_EDGES, true);
      break;
    }
}
//******************************************************************************
