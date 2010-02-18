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
#include "precompile-sew.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-sew.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CPrecompileSew::CPrecompileSew(CParameterGMapVertex * AParameterGMapV,
			       CParameterSew        * AParameterSew) :
  FParameterGMapV (AParameterGMapV),
  FParameterSew   (AParameterSew)
{
  assert(FParameterGMapV!=NULL && FParameterSew!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterSew->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileSew::CPrecompileSew(const CPrecompileSew & APrecompile) :
  CPrecompile     (APrecompile),
  FParameterGMapV (APrecompile.FParameterGMapV),
  FParameterSew   (static_cast<CParameterSew*>
		   (APrecompile.FParameterSew->copy()))
{
  assert(FParameterGMapV!=NULL && FParameterSew!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterSew->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileSew::~CPrecompileSew()
{
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterSew->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileSew::copy() const
{ return new CPrecompileSew(*this); }
//******************************************************************************
void CPrecompileSew::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_GMAP_VERTEX:
      setGMapVertex(static_cast<CParameterGMapVertex *>(AParameter));
      break;
    case PARAMETER_SEW: setSew(static_cast<CParameterSew *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileSew::getParameter() const
{ return FParameterSew; }
//******************************************************************************
void CPrecompileSew::setGMapVertex(CParameterGMapVertex* AParameterGMapV)
{
  assert(AParameterGMapV != NULL);
  AParameterGMapV->addPrecompileToUpdate(this);
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterGMapV = AParameterGMapV;
  setToUpdate();
}
//******************************************************************************
void CPrecompileSew::setSew(CParameterSew* ASew)
{
  assert(ASew != NULL);
  ASew->addPrecompileToUpdate(this);
  FParameterSew->removePrecompileToUpdate(this);
  FParameterSew = ASew;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileSew::getType() const
{ return PRECOMPILE_SEW; }
//******************************************************************************
#define WEIGHTING(k, v1,v2) ( (1-(k)) * (v1) + (k) * (v2) )
//******************************************************************************
void CPrecompileSew::drawSewsI(int AIndice, int AMark)
{
  assert(0<=AIndice && AIndice<=3);

  CDynamicCoverageAll it(FParameterGMapV->getMap());
  CDart *dart, *darti;
  CVertex e,m;

  float kw; kw= FParameterSew->getSewPosCoef(AIndice);

  for (; it.cont(); ++it)
    {
      dart= *it;
      if (!FParameterGMapV->getMap()->isMarked(dart,AMark))
        {
          FParameterGMapV->getMap()->setMark(dart , AMark);

          if (!FParameterGMapV->getMap()->isFree(dart,AIndice))
            {
              darti= FParameterGMapV->getMap()->alpha(dart,AIndice);
              FParameterGMapV->getMap()->setMark(darti, AMark);
              if (AIndice==0)
                {
		  m= (FParameterGMapV->getDrawingMap()->
		      getBurstVertex(FParameterGMapV->getDartWithEmbedding(dart)) +
		      FParameterGMapV->getDrawingMap()->
		      getBurstVertex(FParameterGMapV->getDartWithEmbedding(darti)))/2;

                  e= WEIGHTING(kw, m, FParameterGMapV->getDrawingMap()->
			       computeBurstExtremity
			       (FParameterGMapV->getDartWithEmbedding(dart)));
		  PLOT(&e);
                  e= WEIGHTING(kw, m, FParameterGMapV->getDrawingMap()->
			       computeBurstExtremity
			       (FParameterGMapV->getDartWithEmbedding(darti)));
		  PLOT(&e);
                }
              else
                {
                  e= WEIGHTING(kw,
			       FParameterGMapV->getDrawingMap()->
			       getBurstVertex
			       (FParameterGMapV->getDartWithEmbedding(dart)),
			       FParameterGMapV->getDrawingMap()->
			       computeBurstExtremity
			       (FParameterGMapV->getDartWithEmbedding(dart)));
		  PLOT(&e);
                  e= WEIGHTING(kw,
			       FParameterGMapV->getDrawingMap()->
			       getBurstVertex
			       (FParameterGMapV->getDartWithEmbedding(darti)),
			       FParameterGMapV->getDrawingMap()->
			       computeBurstExtremity
			       (FParameterGMapV->getDartWithEmbedding(darti)));
		  PLOT(&e);
                }
            }
        }
    }
}
//******************************************************************************
void CPrecompileSew::drawModel()
{
  int mark = FParameterGMapV->getMap()->getNewMark();

  for (int i=0; i<=3; ++i)
    if (FParameterSew->getDisplayAlpha(i))
      {
	glColor3fv (FParameterSew->getCLSew(i));
	glLineWidth(FParameterSew->getLWSew(i));
	glBegin(GL_LINES);
	drawSewsI(i,mark);
	glEnd();
	FParameterGMapV->getMap()->negateMaskMark(mark);
      }

  glEnd();

  FParameterGMapV->getMap()->freeMark(mark);
}
//******************************************************************************
#undef WEIGHTING
//******************************************************************************
