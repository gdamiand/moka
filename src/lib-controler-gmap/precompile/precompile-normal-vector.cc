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
#include "precompile-normal-vector.hh"
#include "dart-vertex.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-normal-vector.hh"
#include "parameter-selection.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CPrecompileNormalVector::
CPrecompileNormalVector(CParameterGMapVertex   * AParameterGMapV,
			CParameterNormalVector * AParameterNormalVector,
			CParameterSelection    * AParameterSelection) :
  FParameterGMapV       (AParameterGMapV),
  FParameterNormalVector(AParameterNormalVector),
  FParameterSelection	(AParameterSelection)
{
  assert(FParameterGMapV!=NULL);
  assert(FParameterNormalVector!=NULL && FParameterSelection!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterNormalVector->addPrecompileToUpdate(this);
  FParameterSelection->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileNormalVector::
CPrecompileNormalVector(const CPrecompileNormalVector & APrecompile) :
  CPrecompile           (APrecompile),
  FParameterGMapV       (APrecompile.FParameterGMapV),
  FParameterNormalVector(static_cast<CParameterNormalVector*>
			 (APrecompile.FParameterNormalVector->copy())),
  FParameterSelection	(APrecompile.FParameterSelection)
{
  assert(FParameterGMapV!=NULL);
  assert(FParameterNormalVector!=NULL && FParameterSelection!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterNormalVector->addPrecompileToUpdate(this);
  FParameterSelection->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileNormalVector::~CPrecompileNormalVector()
{
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterNormalVector->removePrecompileToUpdate(this);
  FParameterSelection->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileNormalVector::copy() const
{ return new CPrecompileNormalVector(*this); }
//******************************************************************************
void CPrecompileNormalVector::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_GMAP_VERTEX:
      setGMapVertex(static_cast<CParameterGMapVertex *>(AParameter));
      break;
    case PARAMETER_NORMAL_VECTOR:
      setNormalVector(static_cast<CParameterNormalVector *>(AParameter));
      break;
    case PARAMETER_SELECTION:
      setSelection(static_cast<CParameterSelection *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileNormalVector::getParameter() const
{ return FParameterNormalVector; }
//******************************************************************************
void CPrecompileNormalVector::setGMapVertex(CParameterGMapVertex* AParameterGMapV)
{
  assert(AParameterGMapV != NULL);
  AParameterGMapV->addPrecompileToUpdate(this);
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterGMapV = AParameterGMapV;
  setToUpdate();
}
//******************************************************************************
void CPrecompileNormalVector::setNormalVector(CParameterNormalVector*
					      ANormalVector)
{
  assert(ANormalVector != NULL);
  ANormalVector->addPrecompileToUpdate(this);
  FParameterNormalVector->removePrecompileToUpdate(this);
  FParameterNormalVector = ANormalVector;
  setToUpdate();
}
//******************************************************************************
void CPrecompileNormalVector::setSelection(CParameterSelection* ASelection)
{
  assert(ASelection != NULL);
  ASelection->addPrecompileToUpdate(this);
  FParameterSelection->removePrecompileToUpdate(this);
  FParameterSelection = ASelection;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileNormalVector::getType() const
{ return PRECOMPILE_NORMAL_VECTOR; }
//******************************************************************************
void CPrecompileNormalVector::drawNormal(CDart* ADart, bool ATwoSides)
{
  CVertex bary = FParameterGMapV->getMap()->barycenter(ADart, ORBIT_01);
  CVertex normal = FParameterGMapV->getMap()->faceNormalVector(ADart);

  float k = FParameterNormalVector->getLGNormalVector();

  CVertex p1 = bary + k*normal;
  CVertex p2 = ATwoSides ? bary-k*normal : bary;

  LINE(& p1, & p2);
}
//******************************************************************************
void CPrecompileNormalVector::drawModel()
{
  glLineWidth(FParameterNormalVector->getLWNormalVector());
  glColor3fv(FParameterNormalVector->getCLNormalVector());
  glBegin(GL_LINES);

  if (FParameterNormalVector->getShowAllNormals())
    {
      int treated = FParameterGMapV->getMap()->getNewMark();

      for (CDynamicCoverageAll it(FParameterGMapV->getMap()); it.cont(); ++it)
	if (! FParameterGMapV->getMap()->isMarked(*it, treated))
	  {
	    bool done = false;
	    for (CDynamicCoverage01 face(FParameterGMapV->getMap(), *it); face.cont(); ++face)
	      {
		if (! FParameterGMapV->getMap()->isFree0(*face))
		  {
		    done = true;
		    drawNormal(*face, true);
		  }

		FParameterGMapV->getMap()->setMark(*face, treated);
		FParameterGMapV->getMap()->setMark(FParameterGMapV->getMap()->alpha3(*face), treated);
	      }

	    if (!done)
	      drawNormal(*it, false);
	  }

      FParameterGMapV->getMap()->negateMaskMark(treated);
      FParameterGMapV->getMap()->freeMark(treated);
    }
  else
    {
      CDart* last = FParameterSelection->getLastSelectedDart();

      if (last != NULL)
	drawNormal(last, false);
    }

  glEnd();
}
//******************************************************************************
