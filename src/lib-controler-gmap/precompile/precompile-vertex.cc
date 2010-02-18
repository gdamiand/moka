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
#include "precompile-vertex.hh"
#include "dart-vertex.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-vertex.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CPrecompileVertex::CPrecompileVertex(CParameterGMapVertex * AParameterGMapV,
				     CParameterVertex     * AParameterVertex) :
  FParameterGMapV  (AParameterGMapV),
  FParameterVertex (AParameterVertex)
{
  assert(FParameterGMapV!=NULL && FParameterVertex!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterVertex->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileVertex::CPrecompileVertex(const CPrecompileVertex & APrecompile) :
  CPrecompile      (APrecompile),
  FParameterGMapV  (APrecompile.FParameterGMapV),
  FParameterVertex (static_cast<CParameterVertex*>
		    (APrecompile.FParameterVertex->copy()))
{
  assert(FParameterGMapV!=NULL && FParameterVertex!=NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterVertex->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileVertex::~CPrecompileVertex()
{
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterVertex->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileVertex::copy() const
{ return new CPrecompileVertex(*this); }
//******************************************************************************
void CPrecompileVertex::setParameter(CParameter * AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_GMAP_VERTEX:
      setGMapVertex(static_cast<CParameterGMapVertex *>(AParameter));
      break;
    case PARAMETER_VERTEX:
      setVertex(static_cast<CParameterVertex *>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileVertex::getParameter() const
{ return FParameterVertex; }
//******************************************************************************
void CPrecompileVertex::setGMapVertex(CParameterGMapVertex* AParameterGMapV)
{
  assert(AParameterGMapV != NULL);
  AParameterGMapV->addPrecompileToUpdate(this);
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterGMapV = AParameterGMapV;
  setToUpdate();
}
//******************************************************************************
void CPrecompileVertex::setVertex(CParameterVertex * AVertex)
{
  assert(AVertex != NULL);
  AVertex->addPrecompileToUpdate(this);
  FParameterVertex->removePrecompileToUpdate(this);
  FParameterVertex = AVertex;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileVertex::getType() const
{ return PRECOMPILE_VERTEX; }
//******************************************************************************
void CPrecompileVertex::drawModel()
{
  glPointSize(FParameterVertex->getSVertex());
  glColor3fv(FParameterVertex->getCLVertex());

  glBegin(GL_POINTS);

  for (CDynamicCoverageAll it(FParameterGMapV->getMap()); it.cont(); ++it)
    {
      CVertex * vertex = FParameterGMapV->getDrawingMap()
	->getVertex(FParameterGMapV->getDartWithEmbedding(*it));

      if (vertex != NULL)
	PLOT(vertex);
    }

  glEnd();
}
//******************************************************************************
