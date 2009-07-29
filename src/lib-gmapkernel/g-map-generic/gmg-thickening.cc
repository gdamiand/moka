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
#define DUP1(D) (getDirectInfoAsDart(D, ADirectInfoIndex1))
#define DUP2(D) (getDirectInfoAsDart(D, ADirectInfoIndex2))
//******************************************************************************
int CGMapGeneric::thickenMarkedDarts(int AMarkNumber,
				     int ADirectInfoIndex1,
				     int ADirectInfoIndex2)
{
  int nbThickened = 0;
  int dup = getNewMark();

  CDynamicCoverageAll it(this);

  // Duplication des brins:
  for (; it.cont(); ++it)
    if (isMarked(*it, AMarkNumber))
      {
	CDart * newDart;

	newDart = addMapDart();
	setMark(newDart, dup);
	(*it)->setDirectInfo(ADirectInfoIndex1, newDart);

	newDart->setDirectInfo(ADirectInfoIndex1, *it);
	newDart->setDirectInfo(ADirectInfoIndex2, NULL);

	newDart = addMapDart();
	setMark(newDart, dup);
	(*it)->setDirectInfo(ADirectInfoIndex2, newDart);

	newDart->setDirectInfo(ADirectInfoIndex1, NULL);
	newDart->setDirectInfo(ADirectInfoIndex2, *it);

	++nbThickened;
      }

  // Coutures:
  for (it.reinit(); it.cont(); ++it)
    if (isMarked(*it, AMarkNumber))
      {
	if (!isFree0(*it) && isMarked(alpha0(*it), AMarkNumber))
	  linkAlpha0(DUP1(*it), DUP2(alpha0(*it)));
	
	if (!isFree1(*it) && isMarked(alpha1(*it), AMarkNumber))
	  linkAlpha1(DUP1(*it), DUP2(alpha1(*it)));
      }

  for (it.reinit(); it.cont(); ++it)
    if (isMarked(*it, AMarkNumber))
      {
  	if (!isFree2(*it) && isMarked(alpha2(*it), AMarkNumber) &&
  	    canSew2(DUP1(*it), DUP2(alpha2(*it))))
	  topoSew2(DUP1(*it), DUP2(alpha2(*it)));
	
  	if (!isFree3(*it) && isMarked(alpha3(*it), AMarkNumber) &&
  	    canSew3(DUP1(*it), DUP2(alpha3(*it))))
	  topoSew3(DUP1(*it), DUP2(alpha3(*it)));
      }

  // Duplication des plongements:
  int treated = getNewMark();

  for (int i=ORBIT_0; i<=ORBIT_0123; ++i)
    if (isOrbitUsed(TOrbit(i)))
      {
	// Duplication pour l'orbite i:
	for (it.reinit(); it.cont(); ++it)
	  if (!isMarked(*it, treated))
	    {
	      if (isMarked(*it, AMarkNumber))
		{
		  CDart * owner = getEmbeddingOwner(*it, TOrbit(i));
		  assert(owner!=NULL);

		  CEmbedding * E = owner->getEmbedding(TOrbit(i));
		  assert(E!=NULL);
		
		  CCoverage * cov = getDynamicCoverage(*it, TOrbit(i));
		  assert(cov!=NULL);
		
		  for (; cov->cont(); setMark(**cov, treated), ++(*cov))
		    if (isMarked(**cov, AMarkNumber))
		      {
			if (!isMarked(DUP1(**cov), treated))
			  {
			    DUP1(**cov)->addEmbedding(E->copy());
			    markOrbit(DUP1(**cov), TOrbit(i), treated);
			    unmarkOrbit(DUP1(**cov), TOrbit(i), dup);
			  }

			if (!isMarked(DUP2(**cov), treated))
			  {
			    DUP2(**cov)->addEmbedding(E->copy());
			    markOrbit(DUP2(**cov), TOrbit(i), treated);
			    unmarkOrbit(DUP2(**cov), TOrbit(i), dup);
			  }
 		      }
		
		  delete cov;
		}
	      else
		if (!isMarked(*it, dup))
		  setMark(*it, treated);
	    }
	
	// Démarquage:
	negateMaskMark(treated);
      }

  freeMark(treated);
  freeMark(dup);

  return nbThickened;
}
//******************************************************************************
#undef DUP1
#undef DUP2
//******************************************************************************
