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
int CGMapGeneric::getBorderDimension(CDart * ADart)
{
  assert(ADart!=NULL);

  if (isFree0(ADart)) return 0;
  if (isFree1(ADart)) return 1;
  if (isFree2(ADart)) return 2;
  if (isFree3(ADart)) return 3;
  return 4;
}
//******************************************************************************
bool CGMapGeneric::markBorders(int AMarkNumber, int ADim)
{
  bool marked = false;

  int selected= getNewMark();
  int treated = getNewMark();

  // 1: Copie des marques
  markCopy(AMarkNumber, selected);

  // 2: Sélection des bords:
  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, selected) && isFree(*it, ADim))
	  {
	    for (CDynamicBorderCoverage cov(this, *it, ADim);
		 cov.cont(); ++cov)
	      {
		setMark(*cov, AMarkNumber);
		setMark(*cov, treated);
	      }

	    marked = true;
	  }
	else
	  setMark(*it, treated);
      }

  // 3: Libération des marques:
  negateMaskMark(treated);
  freeMark(treated);

  unmarkAll(selected);
  freeMark(selected);

  return marked;
}
//******************************************************************************
bool CGMapGeneric::markBorders(int AMarkNumber)
{
  bool marked = false;

  int selected= getNewMark();
  int treated = getNewMark();

  // 1: Copie des marques
  markCopy(AMarkNumber, selected);

  // 2: Sélection des bords:
  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, selected))
	  {
	    int dim = getBorderDimension(*it);
	
	    if (dim<4)
	      {
		for (CDynamicBorderCoverage cov(this, *it, dim);
		     cov.cont(); ++cov)
		  {
		    setMark(*cov, AMarkNumber);
		    setMark(*cov, treated);
		  }

		marked = true;
	      }
	  }
	else
	  setMark(*it, treated);
      }

  // 3: Libération des marques:
  negateMaskMark(treated);
  freeMark(treated);

  unmarkAll(selected);
  freeMark(selected);

  return marked;
}
//******************************************************************************
bool CGMapGeneric::canSewBorders(CDart * ADart1, CDart * ADart2,
				 int ADim, int An)
{
  assert(ADart1!=NULL);
  assert(ADart2!=NULL);

  if (!isFree(ADart1, ADim) || !isFree(ADart2, ADim))
    return false;
  
  CDynamicBorderCoverage it1(this, ADart1, ADim);
  CDynamicBorderCoverage it2(this, ADart2, ADim);

  if (An==0)
    An=-1;

  for (; it1.cont() && it2.cont() && An!=0; ++it1, ++it2, --An)
    if (it1.prevOperationType()!=it2.prevOperationType())
      return false;

  return An==0 || (!it1.cont() && !it2.cont());
}
//******************************************************************************
void CGMapGeneric::borderTopoSew(CDart * ADart1, CDart * ADart2,
				 int ADim, int An)
{
  assert(canSewBorders(ADart1, ADart2, ADim, An));

  if (An==0)
    An=-1;

  CStaticBorderCoverage it1(this, ADart1, ADim);
  CStaticBorderCoverage it2(this, ADart2, ADim);

  for (; it1.cont() && An!=0; ++it1, ++it2, --An)
    if (isFree(*it1, ADim) && isFree(*it2, ADim))
      topoSew(*it1, *it2, ADim);
}
//******************************************************************************
void CGMapGeneric::borderSew(CDart * ADart1, CDart * ADart2, int ADim, int An)
{
  assert(canSewBorders(ADart1, ADart2, ADim, An));

  if (An==0)
    An=-1;

  CStaticBorderCoverage it1(this, ADart1, ADim);
  CStaticBorderCoverage it2(this, ADart2, ADim);

  for (; it1.cont() && An!=0; ++it1, ++it2, --An)
    if (isFree(*it1, ADim) && isFree(*it2, ADim))
      sew(*it1, *it2, ADim);
}
//******************************************************************************
bool CGMapGeneric::intuitiveBorderSew(CDart * ADart1, CDart * ADart2, int An)
{
  assert(ADart1!=NULL && ADart2!=NULL);

  int dim = getBorderDimension(ADart1);

  if (dim==4 || !canSewBorders(ADart1, ADart2, dim))
    return false;

  borderSew(ADart1, ADart2, dim, An);
  return true;
}
//******************************************************************************
void CGMapGeneric::border2TopoSew(CDart * ADart1, CDart * ADart2, int An)
{
  borderTopoSew(ADart1, ADart2, 2, 2*An);
}
//******************************************************************************
void CGMapGeneric::border2Sew(CDart * ADart1, CDart * ADart2, int An)
{
  borderSew(ADart1, ADart2, 2, 2*An);
}
//******************************************************************************
