/*
 * lib-rounding : Opérations de chamfreinage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-rounding
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
#include "rounding-interface.hh"
#include "g-map-vertex.hh"
#include "geometry.hh"
#include <list>
#include <cstdlib>
#include <cassert>
using namespace GMap3d;
using namespace std;
//******************************************************************************
CRoundingInterface::CRoundingInterface(CGMapVertex* AMap) :
  FMap(AMap),
  FRounding(AMap)
{
  assert(FMap != NULL);
}
//------------------------------------------------------------------------------
CRoundingInterface::~CRoundingInterface()
{
}
//******************************************************************************
void CRoundingInterface::setRoundingCoefOfMarkedDarts(int AMarkNumber,
  						      int ADimension,
  						      const TCoordinate& ACoef)
{
  assert(ADimension==0 || ADimension==1);

  int selection = FMap->getNewMark();
  static const TOrbit propag[2] = { ORBIT_23, ORBIT_3 };
  FMap->markIncidentCells(propag[ADimension], AMarkNumber, selection);

  for (CDynamicCoverageAll it(FMap); it.cont(); ++it)
    if (FMap->isMarked(*it, selection))
      {
  	FRounding.setDartRoundingCoef(*it, ADimension, ACoef);
  	FMap->unsetMark(*it, selection);
      }

  FMap->freeMark(selection);
}
//------------------------------------------------------------------------------
bool CRoundingInterface::getRoundingCoefOfMarkedDarts(int AMarkNumber,
  						      int ADimension,
  						      TCoordinate& AAverage)
{
  AAverage = 0.0;
  int nb = 0;

  TCoordinate value=0;
  bool found = false;
  bool same = true;

  for (CDynamicCoverageAll it(FMap); it.cont(); ++it) 
    if (FMap->isMarked(*it, AMarkNumber))
      {
  	TCoordinate coef = FRounding.getDartRoundingCoef(*it, ADimension);

  	++nb;
  	AAverage += coef;

  	if (found)
  	  {
  	    if (same && !areEqual(coef, value))
  	      same = false;
  	  }
  	else
  	  {
  	    value = coef;
  	    found = true;
  	  }
      }

  if (nb==0)
    same = false;

  AAverage = nb==0 ? 0.0 : AAverage / nb;
  return same;
}
//------------------------------------------------------------------------------
TCoordinate CRoundingInterface::selectNextRoundingCoef(int AMarkNumber,
  						       int ADimension)
{
  /* Lister les coefficients actuels (en o(n^2), mais bon...) et
   * trouver le premier brin marqué avec la marque AMarkNumber :
   */
  list<CDart*> coefs;
  CDart* firstSelected = NULL;
  int treated = FMap->getNewMark();
  CDynamicCoverageAll it(FMap);

  for (; it.cont(); ++it)
    {
      if (firstSelected == NULL && FMap->isMarked(*it, AMarkNumber))
  	firstSelected = *it;

      if (!FMap->isMarked(*it, treated))
  	{
  	  TCoordinate coef = FRounding.getDartRoundingCoef(*it, ADimension);
  	  coefs.push_back(*it);
	  
  	  for (CDynamicCoverageAll all(FMap); all.cont(); ++all)
  	    if (areEqual(FRounding.getDartRoundingCoef(*all, ADimension), coef))
  	      FMap->setMark(*all, treated);
  	}
    }

  FMap->negateMaskMark(treated);
  FMap->freeMark(treated);

  if (coefs.size() == 0)
    return 0.0; // Cas où la carte est vide.

  list<CDart*>::iterator itCoefs = coefs.begin();

  if (firstSelected != NULL)
    {
      // Chercher le coefficient actuel :
      TCoordinate actu =
	FRounding.getDartRoundingCoef(firstSelected, ADimension);

      while (! areEqual(FRounding.getDartRoundingCoef(*itCoefs, ADimension),
			actu))
  	{
  	  ++itCoefs;
  	  assert(itCoefs != coefs.end());
  	}

      // Passer au coefficient suivant :
      ++itCoefs;

      if (itCoefs == coefs.end())
  	itCoefs = coefs.begin();
    }

  TCoordinate newValue = FRounding.getDartRoundingCoef(*itCoefs, ADimension);

  for (it.reinit(); it.cont(); ++it)
    FMap->setMarkTo(*it, AMarkNumber,
  		    areEqual(FRounding.getDartRoundingCoef(*it, ADimension),
			     newValue));

  return newValue;
}
//******************************************************************************
int CRoundingInterface::roundMarkedVertices(int AMarkNumber, bool ADig)
{
  return FRounding.roundMarkedVertices(AMarkNumber, ADig);
}
//------------------------------------------------------------------------------
int CRoundingInterface::roundMarkedEdges(int AMarkNumber,
  					 bool A3D, bool ADig, bool ASetback)
{
  return FRounding.roundMarkedEdges(AMarkNumber, A3D, ADig, ASetback);
}
//******************************************************************************
