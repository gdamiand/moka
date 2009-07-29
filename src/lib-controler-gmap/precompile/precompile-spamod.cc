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
#ifdef MODULE_SPAMOD
//******************************************************************************
#include "controler-gmap-types.hh"
#include "precompile-spamod.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-spamod.hh"
#include "spamod-api.hh"
#include <cassert>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CPrecompileSpamod::CPrecompileSpamod(CParameterGMapVertex * AParameterGMapV,
				     CParameterSpamod     * AParameterSpamod) :
  FParameterGMapV  (AParameterGMapV),
  FParameterSpamod (AParameterSpamod)
{
  assert(FParameterGMapV != NULL);
  assert(FParameterSpamod != NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterSpamod->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileSpamod::CPrecompileSpamod(const CPrecompileSpamod & APrecompile) :
  CPrecompile      (APrecompile),
  FParameterGMapV  (APrecompile.FParameterGMapV),
  FParameterSpamod (static_cast<CParameterSpamod*>
		    (APrecompile.FParameterSpamod->copy()))
{
  assert(FParameterGMapV != NULL);
  assert(FParameterSpamod != NULL);

  FParameterGMapV->addPrecompileToUpdate(this);
  FParameterSpamod->addPrecompileToUpdate(this);
}
//******************************************************************************
CPrecompileSpamod::~CPrecompileSpamod()
{
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterSpamod->removePrecompileToUpdate(this);
}
//******************************************************************************
CPrecompile * CPrecompileSpamod::copy() const
{ return new CPrecompileSpamod(*this); }
//******************************************************************************
void CPrecompileSpamod::setParameter(CParameter* AParameter)
{
  switch (AParameter->getType())
    {
    case PARAMETER_GMAP_VERTEX:
      setGMapVertex(static_cast<CParameterGMapVertex *>(AParameter));
      break;
    case PARAMETER_SPAMOD:
      setParameterSpamod(static_cast<CParameterSpamod*>(AParameter));
      break;
    }
}
//******************************************************************************
CParameter* CPrecompileSpamod::getParameter() const
{ return FParameterSpamod; }
//******************************************************************************
void CPrecompileSpamod::setGMapVertex(CParameterGMapVertex* AParameterGMapV)
{
  assert(AParameterGMapV != NULL);
  AParameterGMapV->addPrecompileToUpdate(this);
  FParameterGMapV->removePrecompileToUpdate(this);
  FParameterGMapV = AParameterGMapV;
  setToUpdate();
}
//******************************************************************************
void CPrecompileSpamod::setParameterSpamod(CParameterSpamod* ASpamod)
{
  assert(ASpamod != NULL);
  ASpamod->addPrecompileToUpdate(this);
  FParameterSpamod->removePrecompileToUpdate(this);
  FParameterSpamod = ASpamod;
  setToUpdate();
}
//******************************************************************************
TPrecompile CPrecompileSpamod::getType() const
{ return PRECOMPILE_SPAMOD; }
//******************************************************************************
void CPrecompileSpamod::drawModel()
{
  if (FParameterGMapV->getMap()->getFirstDart() == NULL ||
      FParameterSpamod->getViewMode() == SPAMOD_NONE)
    return;

  //  cout << "calculs spamod <beg>" << endl;
  FSpamodAPI = new CSpamodAPI((CGMap *) FParameterGMapV->getMap());

  // Récupération des paramètres :
  for (int i=0; i<2; ++i)
    FSpamodAPI->setEuclidianParam(i, FParameterSpamod->getEuclidianParam(i));

  for (int i=0; i<4; ++i)
    FSpamodAPI->setAnalyticParam(i, FParameterSpamod->getAnalyticParam(i));

  for (int i=0; i<3; ++i)
    FSpamodAPI->setVoxelParam(i, FParameterSpamod->getVoxelParam(i));

  for (int i=0; i<3; ++i)
    FSpamodAPI->setK2Param(i, FParameterSpamod->getK2Param(i));

  // Affichage :

  FSpamodAPI->setViewMode(FParameterSpamod->getViewMode());
  FSpamodAPI->drawMap();
  FSpamodAPI->setViewMode(SPAMOD_NONE);

  delete FSpamodAPI;
  //  cout << "calculs spamod <end>" << endl;
}
//******************************************************************************
#endif // MODULE_SPAMOD
//******************************************************************************


