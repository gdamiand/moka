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
#include "precompile.hh"
#include "view-precompile.hh"
#include "g-map-vertex.hh"
#include "parameter-selection.hh"
#include "math-extension.hh"

#include "precompile-dart.hh"
#include "precompile-face.hh"
#include "precompile-preview.hh"
#include "precompile-normal-vector.hh"
#include "precompile-object-transformation.hh"
#include "precompile-sew.hh"
#include "precompile-vertex.hh"
#ifdef MODULE_SPAMOD
#include "precompile-spamod.hh"
#endif // MODULE_SPAMOD

#include <cassert>
#include <string>
using namespace std;
using namespace GMap3d;
//******************************************************************************
bool CControlerGMap::isMapOk() const
{
  if ( !FMap->checkTopology() )
    return false;

  if ( !FMap->checkEmbeddings(ORBIT_VERTEX, ATTRIBUTE_VERTEX, true) )
    return false;

  return true;
}
//******************************************************************************
bool CControlerGMap::isModelChanged() const
{
  assert( getParameterGMapVertex()!=NULL );
  return getParameterGMapVertex()->getChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::setModelChanged()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->setChanged();
  setSelectionChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::unsetModelChanged()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->unsetChanged();
  unsetSelectionChanged();
}
//******************************************************************************
bool CControlerGMap::isPartialModelChanged() const
{
  assert( getParameterGMapVertex()!=NULL );
  return getParameterGMapVertex()->getPartialChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::setPartialModelChanged()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->setPartialChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::unsetPartialModelChanged()
{
  assert( getParameterGMapVertex()!=NULL );
  getParameterGMapVertex()->unsetPartialChanged();
}
//******************************************************************************
void CControlerGMap::updateModelIfNeeded()
{
  // Recalculer les coordonnées éclatées ssi la carte à changé
  // et si on n'est pas bloqué.
  if ( isModelBlocked() ) return;

  updateSelectionIfNeeded();

  if ( isModelChanged() )
    {
      FMap->updateAllBurstDarts();
      if ( FParameterGMapVertex->getMapEmbedding()!=NULL )
	FParameterGMapVertex->getMapEmbedding()->updateAllBurstDarts();
      
      unsetModelChanged();
    }
  else if ( isPartialModelChanged() )
    {
      if ( getParameterGMapVertex()->getMarkPartialChanged()!=-1 )
	FMap->updateMarkedBurstDarts(getParameterGMapVertex()->
				     getMarkPartialChanged(),
				     false);
      else
	FMap->updateAllBurstDarts();
      
      unsetPartialModelChanged();
    }
}
//******************************************************************************
bool CControlerGMap::isSelectionChanged() const
{
  assert( getParameterSelection()!=NULL );
  return getParameterSelection()->getChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::setSelectionChanged()
{
  assert( getParameterSelection()!=NULL );
  getParameterSelection()->setChanged();
}
//------------------------------------------------------------------------------
void CControlerGMap::unsetSelectionChanged()
{
  assert( getParameterSelection()!=NULL );
  getParameterSelection()->unsetChanged();
}
//******************************************************************************
void CControlerGMap::updateSelectionIfNeeded()
{
  /* Recalculer si besoin est les caractéristiques de la carte : nb de brins,
   * de sommets, de brins sélectionnés.
   */
  if ( isSelectionChanged() )
    {
      int  nbLevels = getNbSelectionLevels();
      bool* validLast=new bool[nbLevels];
      //  bool validLast[nbLevels]; marche pas sous VC++ !!!
      
      for (int i=0; i<nbLevels; ++i)
	validLast[i] = false;
      
      FNbDarts = 0;
      FNbSelectedDarts = 0;
      FNbVertices = 0;
      
      for (CDynamicCoverageAll it(FMap); it.cont(); ++it)
	{
	  ++FNbDarts;
	  
	  if (isDartSelected(*it)) ++FNbSelectedDarts;
	  
	  if (FMap->getVertex(*it)!=NULL) ++FNbVertices;
	  
	  for (int j=0; j<nbLevels; ++j)
	    if (*it == getLastSelectedDart(j))
	      validLast[j] = true;
	}
      
      for (int k=0; k<nbLevels; ++k)
	if (!validLast[k])
	  unsetLastSelectedDart(k);

      unsetSelectionChanged();

      delete []validLast;
    }
}
//******************************************************************************
CPrecompile* CControlerGMap::newPrecompile(TViewId AViewId,
					   TPrecompile APrecompileType)
{
  CPrecompile* precompile = NULL;
  int n;
  
  switch (APrecompileType)
    {
    case PRECOMPILE_DART:
      n = FParameterSelection->getNbSelectionLevels();
      precompile =
	new CPrecompileDart(FParameterGMapVertex, new CParameterDart(n),
			    FParameterSelection);
      break;
    case PRECOMPILE_FACE:
      precompile = new CPrecompileFace(FParameterGMapVertex,
				       new CParameterFace);
      break;
    case PRECOMPILE_NORMAL_VECTOR:
      precompile = new CPrecompileNormalVector(FParameterGMapVertex,
					       new CParameterNormalVector,
					       FParameterSelection);
      break;
    case PRECOMPILE_OBJECT_TRANSFORMATION:
      precompile =
	new CPrecompileObjectTransformation(new CParameterObjectTransformation,
					    FParameterTranslation,
					    FParameterRotation,
					    FParameterScale,
					    FParameterPonderation);
      break;
    case PRECOMPILE_PREVIEW:
      precompile = new CPrecompilePreview(new CParameterPreview,
					  FParameterObjectPosition[0],
					  FParameterCreation,
					  FParameterPolylinePosition);
      break;
    case PRECOMPILE_SEW:
      precompile = new CPrecompileSew(FParameterGMapVertex,
				      new CParameterSew);
      break;
#ifdef MODULE_SPAMOD
    case PRECOMPILE_SPAMOD:
      precompile = new CPrecompileSpamod(FParameterGMapVertex,
					 new CParameterSpamod);
      break;
#endif // MODULE_SPAMOD
    case PRECOMPILE_VERTEX:
      precompile = new CPrecompileVertex(FParameterGMapVertex,
					 new CParameterVertex);
      break;
    default:
      precompile = CControler::newPrecompile(AViewId, APrecompileType);
      break;
    }

  return precompile;
}
//******************************************************************************
bool CControlerGMap::getDartsForSew(CDart** ADart1, CDart** ADart2)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);

  CDart* last = getLastSelectedDart();
  int mark = getSelectionMark();

  if (last == NULL ||
      FMap->getMarkedCells(ORBIT_SELF, mark, last, ADart2, ADart1) != 2)
    {
      setMessage("No correct selection: you need two darts");
      return false;
    }

  return true;
}
//******************************************************************************
