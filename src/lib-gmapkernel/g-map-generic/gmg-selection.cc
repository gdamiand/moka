/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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
#include "g-map-generic.hh"
using namespace GMap3d;
//******************************************************************************
unsigned int  CGMapGeneric::markOrbit(CDart * ADart, TOrbit AOrbit,
				      int AMarkNumber)
{
  assert(ADart!=NULL);

  CCoverage * cov= getDynamicCoverage(ADart, AOrbit);
  unsigned int res = 0;
  while (cov->cont())
    {
      setMark((*cov)++, AMarkNumber);
      ++res;
    }

  delete cov;
  return res;
}
//******************************************************************************
void CGMapGeneric::halfMarkOrbit(CDart * ADart, TOrbit AOrbit,
				 int AMarkNumber)
{
  assert(ADart!=NULL);
  assert(AOrbit>=ORBIT_SELF && AOrbit<=ORBIT_CC);
  assert(isOrientable(ADart, AOrbit));

  bool usedDim[4];
  int selected = getNewMark();

  for (int dim=0; dim<=3; ++dim)
    usedDim[dim] = AND_ORBIT(AOrbit, ORBIT_DIM[dim]) != ORBIT_SELF;

  CCoverage * cov= getDynamicCoverage(ADart, AOrbit);

  setMark(**cov, selected);
  setMark(**cov, AMarkNumber);

  for (; cov->cont(); ++(*cov))
    if (!isMarked(**cov, selected))
      for (int dim=0; dim<=3; ++dim)
	if (usedDim[dim] && !isFree(**cov, dim))
	  {
	    setMark(alpha(**cov, dim), selected);
	    setMark(alpha(**cov, dim), AMarkNumber);
	  }

  delete cov;

  unmarkOrbit(ADart, AOrbit, selected);
  freeMark(selected);
}
//******************************************************************************
void CGMapGeneric::halfUnmarkOrbit(CDart * ADart, TOrbit AOrbit,
				   int AMarkNumber)
{
  negateMaskMark(AMarkNumber);
  halfMarkOrbit(ADart, AOrbit, AMarkNumber);
  negateMaskMark(AMarkNumber);
}
//******************************************************************************
void CGMapGeneric::unmarkOrbit(CDart * ADart, TOrbit AOrbit,
			       int AMarkNumber)
{
  assert(ADart!=NULL);

  CCoverage * cov= getDynamicCoverage(ADart, AOrbit);

  while (cov->cont())
    unsetMark((*cov)++, AMarkNumber);

  delete cov;
}
//******************************************************************************
void CGMapGeneric::unmarkAll(int AMarkNumber)
{
  CDynamicCoverageAll it(this);

  while (it.cont())
    unsetMark(it++, AMarkNumber);
}
//******************************************************************************
int CGMapGeneric::markIncidentCells(TOrbit AOrbit,
				    int AMarkNumberSrce, int AMarkNumberDest)
{
  assert(AMarkNumberSrce!=AMarkNumberDest);

  int treated = getNewMark();
  int nb_cells = 0;
  
  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumberSrce))
	  {
	    markOrbit(*it, AOrbit, AMarkNumberDest);
	    markOrbit(*it, AOrbit, treated        );
	    ++nb_cells;
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  return nb_cells;
}
//******************************************************************************
int CGMapGeneric::markIncidentCells(TOrbit AOrbit, int AMarkNumber)
{
  int mark = getNewMark();

  markCopy(AMarkNumber, mark);
  int nb_cells = markIncidentCells(AOrbit, mark, AMarkNumber);
  unmarkAll(mark);

  freeMark(mark);

  return nb_cells;
}
//******************************************************************************
void CGMapGeneric::halfMarkIncidentCells(TOrbit AOrbit,
					 int AMarkNumberSrce,
					 int AMarkNumberDest)
{
  assert(AMarkNumberSrce!=AMarkNumberDest);

  int treated = getNewMark();

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumberSrce))
	  {
	    if (isOrientable(*it, AOrbit))
	      {
		halfMarkOrbit(*it, AOrbit, AMarkNumberDest);
		halfMarkOrbit(*it, AOrbit, treated        );
	      }
	    else
	      {
		markOrbit(*it, AOrbit, AMarkNumberDest);
		markOrbit(*it, AOrbit, treated        );
	      }
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapGeneric::markInterior(CDart * ADart, int ADimension,
				int AMarkBorder, int AMarkInterior)
{
  assert(ADart != NULL);
  assert(2 <= ADimension && ADimension <= 3);

  if (AMarkInterior < 0)
    AMarkInterior = AMarkBorder;

  bool initialMarked = isMarked(ADart, AMarkBorder);
  unsetMark(ADart, AMarkBorder);

  TOrbit orbit = ORBIT_CELL[1+ADimension];

  CCoverage * cov = getBasicDynamicCoverage(ADart, orbit, AMarkInterior);

  for (; cov->cont(); ++(*cov))
    setMark(**cov, AMarkInterior);

  delete cov;

  if (initialMarked)
    setMark(ADart, AMarkBorder);
}
//******************************************************************************
void CGMapGeneric::halfMarkIncidentCells(TOrbit AOrbit, int AMarkNumber)
{
  int mark = getNewMark();

  markCopy(AMarkNumber, mark);
  halfMarkIncidentCells(AOrbit, mark, AMarkNumber);
  unmarkAll(mark);

  freeMark(mark);
}
//******************************************************************************
void CGMapGeneric::markIntersection(int AMarkNumber1, int AMarkNumber2,
				    TOrbit AOrbit1,TOrbit AOrbit2,
				    int AMarkResult,
				    bool AInitialMarkIncidentCells)
{
  assert(AMarkNumber1 != AMarkNumber2);

  int m1 = getNewMark();
  int m2 = getNewMark();

  if (AInitialMarkIncidentCells)
    {
      markIncidentCells(AOrbit1, AMarkNumber1, m1);
      markIncidentCells(AOrbit1, AMarkNumber2, m2);
    }
  else
    {
      markCopy(AMarkNumber1, m1);
      markCopy(AMarkNumber2, m2);
    }

  markIncidentCells(AOrbit2, m1);
  markIncidentCells(AOrbit2, m2);

  markCopy(m2, AMarkResult);
  markAnd(m1, AMarkResult);

  unmarkAll(m1); freeMark(m1);
  unmarkAll(m2); freeMark(m2);
}
//******************************************************************************
void CGMapGeneric::markCopy(int AMarkNumberFrom, int AMarkNumberTo)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    setMarkTo(*it, AMarkNumberTo, isMarked(*it, AMarkNumberFrom));
}
//******************************************************************************
void CGMapGeneric::markCopy(int AMarkNumberFrom, int AMarkNumberTo,
			    CDart * ADart, TOrbit AOrbit)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);
  assert(ADart!=NULL);

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  for (; cov->cont(); ++(*cov))
    setMarkTo(**cov, AMarkNumberTo, isMarked(**cov, AMarkNumberFrom));

  delete cov;
}
//******************************************************************************
void CGMapGeneric::markAdd(int AMarkNumberFrom, int AMarkNumberTo)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (isMarked(*it, AMarkNumberFrom))
      setMark(*it, AMarkNumberTo);
}
//******************************************************************************
void CGMapGeneric::markAdd(int AMarkNumberFrom, int AMarkNumberTo,
			   CDart * ADart, TOrbit AOrbit)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);
  assert(ADart!=NULL);

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  for (; cov->cont(); ++(*cov))
    if (isMarked(**cov, AMarkNumberFrom))
      setMark(**cov, AMarkNumberTo);

  delete cov;
}
//******************************************************************************
void CGMapGeneric::markAnd(int AMarkNumberFrom, int AMarkNumberTo)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, AMarkNumberFrom))
      unsetMark(*it, AMarkNumberTo);
}
//******************************************************************************
void CGMapGeneric::markAnd(int AMarkNumberFrom, int AMarkNumberTo,
			   CDart * ADart, TOrbit AOrbit)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);
  assert(ADart!=NULL);

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  for (; cov->cont(); ++(*cov))
    if (!isMarked(**cov, AMarkNumberFrom))
      unsetMark(**cov, AMarkNumberTo);

  delete cov;
}
//******************************************************************************
void CGMapGeneric::markSub(int AMarkNumberFrom, int AMarkNumberTo)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (isMarked(*it, AMarkNumberFrom))
      unsetMark(*it, AMarkNumberTo);
}
//******************************************************************************
void CGMapGeneric::markSub(int AMarkNumberFrom, int AMarkNumberTo,
			   CDart * ADart, TOrbit AOrbit)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);
  assert(ADart!=NULL);

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  for (; cov->cont(); ++(*cov))
    if (isMarked(**cov, AMarkNumberFrom))
      unsetMark(**cov, AMarkNumberTo);

  delete cov;
}
//******************************************************************************
void CGMapGeneric::markMove(int AMarkNumberFrom, int AMarkNumberTo)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (isMarked(*it, AMarkNumberFrom))
      {
	setMark  (*it, AMarkNumberTo  );
	unsetMark(*it, AMarkNumberFrom);
      }
}
//******************************************************************************
void CGMapGeneric::markMove(int AMarkNumberFrom, int AMarkNumberTo,
			    CDart * ADart, TOrbit AOrbit)
{
  assert(AMarkNumberFrom!=AMarkNumberTo);
  assert(ADart!=NULL);

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  for (; cov->cont(); ++(*cov))
    if (isMarked(**cov, AMarkNumberFrom))
      {
	setMark  (**cov, AMarkNumberTo  );
	unsetMark(**cov, AMarkNumberFrom);
      }

  delete cov;
}
//******************************************************************************
bool CGMapGeneric::isWholeCellMarked(CDart * ADart, TOrbit AOrbit,
				     int AMarkNumber)
{
  assert(ADart!=NULL);

  bool wholeCellMarked = true;
  CCoverage * it = getDynamicCoverage(ADart, AOrbit);

  for (; it->cont() && wholeCellMarked; ++(*it))
    if (!isMarked(**it, AMarkNumber))
      wholeCellMarked = false;

  delete it;

  return wholeCellMarked;
}
//******************************************************************************
int CGMapGeneric::getMarkedCells(TOrbit AOrbit, int AMarkNumber,
				 CDart * ALastMarked,
				 CDart * * ADart1,
				 CDart * * ADart2,
				 CDart * * ADart3)
{
  int maxToFind = ADart1==NULL ? 0 : ADart2==NULL ? 1 : ADart3==NULL ? 2 : 3;
  int nbFound = 0;

  CDart * * dart[3] = {ADart1, ADart2, ADart3};

  int treated = getNewMark();

  for (int i=0; i<maxToFind; ++i)
    *dart[i] = NULL;

  if (ALastMarked!=NULL && isMarked(ALastMarked, AMarkNumber))
    {
      markOrbit(ALastMarked, AOrbit, treated);
      *dart[nbFound++]= ALastMarked;
    }

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    markOrbit(*it, AOrbit, treated);

	    if (nbFound<maxToFind)
	      *dart[nbFound] = *it;
	
	    ++nbFound;
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  return nbFound;
}
//******************************************************************************
