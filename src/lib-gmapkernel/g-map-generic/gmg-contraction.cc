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
#include <deque>
using namespace std;
using namespace GMap3d;
//******************************************************************************
// Contraction d'une i-cellules incidente à un brin donné.
//
// dim == 3: contraction de volume
//           (précondition : volume de degré inférieur local égal à 2)
// dim == 2: contraction de face
//           (précondition : face de degré inférieur local égal à 2)
// dim == 1: contraction d'arête
//           (pas de précondition)
//
//******************************************************************************
bool CGMapGeneric::canContract(CDart * ADart, int ADim)
{
  assert(ADart!=NULL);
  assert(ADim>=1 && ADim<=3);

  return isLocalDegreeTwoInf(ADart, ADim);
}
//******************************************************************************
void CGMapGeneric::contract(CDart * ADart, int ADim, bool ADeleteDarts)
{
  assert(ADart!=NULL);
  assert(ADim>=1 && ADim<=3);
  assert(canContract(ADart, ADim));

  int mark = getNewMark();
  markOrbit(ADart, ORBIT_CELL[ADim], mark);

  CDart* current = NULL;
  CDart* t2      = NULL;
  
  CCoverage * it = getStaticCoverage(ADart, ORBIT_CELL[ADim]);
  while ( it->cont() )
  {
    current = alpha((*it)++, ADim);
    if ( !isMarked(current, mark) )
    {
      t2 = alpha(current, ADim);
      while ( isMarked(t2, mark) )
      {
        t2 = alpha(alpha(t2, ADim-1), ADim);
      }

      if ( t2 != alpha(current, ADim) )
      {
        unsew(current, ADim);
        if (!isFree(t2, ADim)) unsew(t2, ADim);
        if (current!=t2) sew(current, t2, ADim);
      }
    }
  }

  if (ADeleteDarts)
  {
    for (it->reinit(); it->cont(); )
      delMapDart((*it)++);
  }

  freeMark(mark);

  delete it;
}
//******************************************************************************
int CGMapGeneric::contractMarkedCells( int AMarkNumber, int ADim,
                                       bool ADeleteDarts )
{
  assert(ADim>=1 && ADim<=3);

  int nbContract = markIncidentCells(ORBIT_CELL[ADim], AMarkNumber);

  CDynamicCoverageAll cov(this);
  for ( ; cov.cont(); ++cov )
  {
    if ( isMarked(*cov, AMarkNumber) )
    {
      if ( !canContract(*cov, ADim) )
      {
        unmarkOrbit( *cov, ORBIT_CELL[ADim], AMarkNumber );
        --nbContract;
      }
    }
  }

  CDart* current = NULL;
  CDart* t2 = NULL;

  for ( cov.reinit(); cov.cont(); ++cov )
  {
    if (  !isMarked( *cov, AMarkNumber) &&
          isMarked( alpha(*cov, ADim), AMarkNumber) )
    {
      current = *cov;
      t2 = alpha(current, ADim);

      while (isMarked(t2, AMarkNumber))
      {
        t2 = alpha(alpha(t2, ADim-1), ADim);
      }

      if ( t2 != alpha(current, ADim) )
      {
        unsew(current, ADim);
        if ( !isFree(t2, ADim) ) unsew(t2, ADim);
        if ( t2!=current ) sew(current, t2, ADim);
      }
    }
  }
  
  if (ADeleteDarts)
  {
    for (cov.reinit(); cov.cont(); )
    {
      if ( isMarked(*cov, AMarkNumber) )
        delMapDart(cov++);
      else
        ++cov;
    }
  }
  
  return nbContract;
}
//******************************************************************************
