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
#include "rounding-generic.hh"
#include "g-map-generic.hh"
#include <cstdlib>
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CRoundingGeneric::CRoundingGeneric(CGMapGeneric* AMap) :
  FMap(AMap)
{
  assert(FMap != NULL);
}
//------------------------------------------------------------------------------
CRoundingGeneric::~CRoundingGeneric()
{
}
//******************************************************************************
void CRoundingGeneric::chamferCell(CDart* ADart, int ACellDim, int AMaxDim)
{
  assert(ADart != NULL);
  assert(0 <= ACellDim && ACellDim <= 2);

  int treated = FMap->getNewMark();

  CCoverage* cov = FMap->getStaticCoverage(ADart, ORBIT_CELL[ACellDim]);

  for (; cov->cont(); ++(*cov))
    {
      CDart* current = **cov;

      CDart* d0 = FMap->alpha0(current);
      CDart* d1 = FMap->alpha1(current);
      CDart* d2 = FMap->alpha2(current);
      CDart* d3 = FMap->alpha3(current);

      addDartChain(current, 0, AMaxDim);

      if (FMap->isMarked(d0, treated))
  	chamfreinLink(current, d1, 0, ACellDim, AMaxDim);

      if (FMap->isMarked(d1, treated))
  	chamfreinLink(current, d1, 1, ACellDim, AMaxDim);

      if (FMap->isMarked(d2, treated))
  	chamfreinLink(current, d2, 2, ACellDim, AMaxDim);

      if (FMap->isMarked(d3, treated))
  	chamfreinLink(current, d3, 3, ACellDim, AMaxDim);

      FMap->setMark(current, treated);
    }

  for (cov->reinit(); cov->cont(); ++(*cov))
    FMap->unsetMark(**cov, treated);

  FMap->freeMark(treated);

  delete cov;
}
//******************************************************************************
void CRoundingGeneric::chamferVertex(CDart* ADart, int AMaxDim)
{
  assert(ADart != NULL);

  CStaticCoverageVertex cov(FMap, ADart);

  int treated = FMap->getNewMark();

  for (; cov.cont(); ++cov)
    {
      CDart* current = *cov;

      CDart* d1 = FMap->alpha1(current);
      CDart* d2 = FMap->alpha2(current);
      CDart* d3 = FMap->alpha3(current);

      addDartChain(current, 0, AMaxDim);
	
      if (FMap->isMarked(d1, treated)) chamfreinLink(current, d1, 1,0, AMaxDim);
      if (FMap->isMarked(d2, treated)) chamfreinLink(current, d2, 2,0, AMaxDim);
      if (FMap->isMarked(d3, treated)) chamfreinLink(current, d3, 3,0, AMaxDim);

      FMap->setMark(current, treated);
    }

  for (cov.reinit(); cov.cont(); ++cov)
    FMap->unsetMark(*cov, treated);

  FMap->freeMark(treated);
}
//------------------------------------------------------------------------------
void CRoundingGeneric::chamferEdge(CDart * ADart, int AMaxDim)
{
  assert(ADart!=NULL);

  CStaticCoverageEdge cov(FMap, ADart);

  int treated = FMap->getNewMark();

  for (; cov.cont(); ++cov)
    {
      CDart* current = *cov;

      CDart* d0 = FMap->alpha0(current);
      CDart* d2 = FMap->alpha2(current);
      CDart* d3 = FMap->alpha3(current);

      addDartChain(current, 1, AMaxDim);

      if (FMap->isMarked(d0, treated)) chamfreinLink(current, d0, 0,1, AMaxDim);
      if (FMap->isMarked(d2, treated)) chamfreinLink(current, d2, 2,1, AMaxDim);
      if (FMap->isMarked(d3, treated)) chamfreinLink(current, d3, 3,1, AMaxDim);

      FMap->setMark(current, treated);
    }

  for (cov.reinit(); cov.cont(); ++cov)
    FMap->unsetMark(*cov, treated);

  FMap->freeMark(treated);
}
//******************************************************************************
void CRoundingGeneric::addDartChain(CDart* ADart, int ADimMin, int ADimMax)
{
  assert(ADart != NULL);
  assert(0 <= ADimMin);
  assert(ADimMin < ADimMax);
  assert(ADimMax <= 3);

  CDart* d1 = ADart;

  for (int dim = ADimMin+1; dim <= ADimMax; ++dim)
    {
      CDart* d2 = FMap->addMapDart();
      FMap->linkAlpha(d1, d2, dim);
      d1 = d2;
    }
}
//******************************************************************************
void CRoundingGeneric::chamfreinLink(CDart* ADart1, CDart* ADart2,
				     int ADim, int ADimMin, int ADimMax)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);

  assert(ADim != ADimMin);
  assert(ADimMin < ADimMax);

  for (int dim = ADimMin+1; dim <= ADimMax; ++dim)
    {
      ADart1 = FMap->alpha(ADart1, dim);
      ADart2 = FMap->alpha(ADart2, dim);

      if (dim < ADim)
  	{
  	  if (dim < ADim-1)
  	    FMap->linkAlpha(ADart1, ADart2, ADim);
  	}
      else
  	if (ADim != ADimMin)
  	  FMap->linkAlpha(ADart1, ADart2, ADim<ADimMin ? ADim : ADim-1);
    }
}
//******************************************************************************
int CRoundingGeneric::getVertexRoundingDimension(CDart* ADart, int ACellDim)
{
  assert(ADart != NULL);
  assert(0<=ACellDim && ACellDim<=3);

  int i = 0;
  bool cont = true;

  while (cont && i<4)
    {
      if (i != ACellDim)
  	cont = FMap->isIClosedOrbit(ADart, i, ORBIT_CELL[ACellDim]);

      if (cont)
  	++i;
    }

  return i-1;
}
//------------------------------------------------------------------------------
int CRoundingGeneric::getEdgeRoundingDimension(CDart* ADart, int ACellDim)
{
  assert(ADart != NULL);
  assert(0<=ACellDim && ACellDim<=3);

  int i = getVertexRoundingDimension(ADart, ACellDim);

  if (i<3 && ! FMap->isIFreeOrbit(ADart, i, ORBIT_CELL[ACellDim]))
    return i+1;

  return i;
}
//- AUTRE VERSION: -------------------------------------------------------------
// int CRoundingGeneric::getEdgeRoundingDimension(CDart* ADart, int ACellDim)
// {
//   assert(ADart != NULL);
//   assert(0<=ACellDim && ACellDim<=3);
// 
//   int i, j;
//   bool cont = true;
// 
//   for (i=0; cont && i<4; ++i)
//     if (i != j)
// 	 cont = FMap->isIClosedOrbit(ADart, i, ORBIT_CELL[ACellDim]);	
// 
//   // À ce stade, on sait que la cellule est fermée
//   // jusqu'à la dimension i-1 incluse.
// 
//   if (i==4)
//     {
// 	 if (FMap->isIClosedOrbit(ADart, 3, ORBIT_CELL[ACellDim]))
// 	   return 3;
// 
// 	 if (FMap->isIFreeOrbit(ADart, 3, ORBIT_CELL[ACellDim]))
// 	   return 2;
// 
// 	 return 3;
//     }
// 
//   return FMap->isIFreeOrbit(ADart, i, ORBIT_CELL[ACellDim]) ? i-1 : i;
// }
//******************************************************************************
void CRoundingGeneric::removeEdge(CDart* ADart)
{
  assert(ADart != NULL);
  
  for (CDynamicCoverage23 cov(FMap, ADart); cov.cont(); )
    {
      CDart* current = cov++;

      if (! FMap->isFree1(current) &&
	  ! FMap->isFree1(FMap->alpha0(current)))
	FMap->linkAlpha1(FMap->alpha1(current), FMap->alpha01(current));
      else
	{
	  FMap->unlinkAlpha1(             current );
	  FMap->unlinkAlpha1(FMap->alpha0(current));
	}

      if (! FMap->isFree0(current))
	FMap->delMapDart(FMap->alpha0(current));

      FMap->delMapDart(current);
    }
}
//******************************************************************************
