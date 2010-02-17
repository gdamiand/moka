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
int CGMapGeneric::duplicateMarkedDarts(int AMarkNumber, CGMapGeneric* AMap,
				       int ADirectInfoIndex,
				       bool ADuplicateEmbeddings,
				       bool A0Sew, bool A1Sew,
				       bool A2Sew, bool A3Sew)
{
  if (AMap==NULL) AMap=this;

  int nbDuplicated = 0;
  
  int duplicated = AMap->getNewMark();
  
  int directInfoIndex =
    ADirectInfoIndex<0 ? getNewDirectInfo() : ADirectInfoIndex;
  
  CDynamicCoverageAll it(this);
  
  // Duplication des brins:
  for (; it.cont(); ++it)
    if (isMarked(*it, AMarkNumber))
      {
	CDart * newDart = AMap->addMapDart();

	if (ADuplicateEmbeddings)
	  AMap->setMark(newDart, duplicated);
	
	(*it)->setDirectInfo(directInfoIndex, newDart);
	
	++nbDuplicated;
      }
  
  // Coutures:
  sewDuplicatedDarts(&it, AMarkNumber, directInfoIndex,
		     AMap, A0Sew, A1Sew, A2Sew, A3Sew);
  
  // Duplication des plongements:
  if (ADuplicateEmbeddings)
    duplicateEmbeddingsOfDuplicatedDarts(&it,
					 AMarkNumber, duplicated,
					 directInfoIndex, AMap);
  
  // Libérations:
  if (ADirectInfoIndex<0)
    freeDirectInfo(directInfoIndex);

  assert(AMap->isWholeMapUnmarked(duplicated));
  AMap->freeMark(duplicated);

  return nbDuplicated;
}
//******************************************************************************
CDart * CGMapGeneric::duplicateDarts(CDart * ADart, TOrbit AOrbit,
				     CGMapGeneric* AMap,
				     int ADirectInfoIndex,
				     bool ADuplicateEmbeddings)
{
  assert(ADart!=NULL);
  if (AMap==NULL) AMap=this;
  
  int selected = getNewMark();
  int duplicated = AMap->getNewMark();

  int directInfoIndex =
    ADirectInfoIndex<0 ? getNewDirectInfo() : ADirectInfoIndex;

  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  // Duplication des brins:
  for (; cov->cont(); ++(*cov))
    {
      CDart * newDart = AMap->addMapDart();
      AMap->setMark(newDart, duplicated);
      setMark(**cov, selected);
      (**cov)->setDirectInfo(directInfoIndex, newDart);
    }

  // Coutures:
  sewDuplicatedDarts(cov, selected, directInfoIndex, AMap,
		     AND_ORBIT(AOrbit, ORBIT_0) != ORBIT_SELF,
		     AND_ORBIT(AOrbit, ORBIT_1) != ORBIT_SELF,
		     AND_ORBIT(AOrbit, ORBIT_2) != ORBIT_SELF,
		     AND_ORBIT(AOrbit, ORBIT_3) != ORBIT_SELF);

  // Duplication des plongements:
  if (ADuplicateEmbeddings)
    duplicateEmbeddingsOfDuplicatedDarts(cov,
					 selected, duplicated,
					 directInfoIndex, AMap);

  // Libérations:
  CDart * result = getDirectInfoAsDart(ADart, directInfoIndex);

  for (cov->reinit(); cov->cont(); ++(*cov))
    unsetMark(**cov, selected);

  delete cov;

  if (ADirectInfoIndex<0)
    freeDirectInfo(directInfoIndex);

  freeMark(selected);
  AMap->freeMark(duplicated);

  return result;
}
//******************************************************************************
#define DUP(D) (getDirectInfoAsDart(D, ADirectInfoIndex))
//******************************************************************************
void CGMapGeneric::sewDuplicatedDarts(CCoverage * ACoverage,
				      int AMarkNumber, int ADirectInfoIndex,
				      CGMapGeneric* AMap,
				      bool A0Sew, bool A1Sew,
				      bool A2Sew, bool A3Sew)
{
  assert(ACoverage!=NULL);
  assert(AMarkNumber>=0);
  assert(ADirectInfoIndex>=0);
  
  // 0 et 1 coutures:
  for (ACoverage->reinit(); ACoverage->cont(); ++(*ACoverage))
    {
      CDart * d = **ACoverage;

      if (AMarkNumber<0 || isMarked(d, AMarkNumber))
	{
	  if (A0Sew)
	    if (!isFree0(d) && isMarked(alpha0(d), AMarkNumber) &&
		AMap->isFree0(DUP(d)))
	      AMap->linkAlpha0(DUP(d), DUP(alpha0(d)));

	  if (A1Sew)
	    if (!isFree1(d) && isMarked(alpha1(d), AMarkNumber) &&
		AMap->isFree1(DUP(d)))
	      AMap->linkAlpha1(DUP(d), DUP(alpha1(d)));
	}
    }

  // 2 and 3 coutures:
  for (ACoverage->reinit(); ACoverage->cont(); ++(*ACoverage))
    {
      CDart * d = **ACoverage;

      if (isMarked(d, AMarkNumber))
	{
	  if (A2Sew)
	    if (!isFree2(d) && isMarked(alpha2(d), AMarkNumber) &&
		AMap->canSew2(DUP(d), DUP(alpha2(d))))
	      AMap->topoSew2(DUP(d), DUP(alpha2(d)));
	
	  if (A3Sew)
	    if (!isFree3(d) && isMarked(alpha3(d), AMarkNumber) &&
		AMap->canSew3(DUP(d), DUP(alpha3(d))))
	      AMap->topoSew3(DUP(d), DUP(alpha3(d)));
	}
    }
}
//******************************************************************************
void CGMapGeneric::duplicateEmbeddingsOfDuplicatedDarts(CCoverage * ACoverage,
							int AMarkInitial,
							int AMarkDuplicated,
							int ADirectInfoIndex,
							CGMapGeneric* AMap)
{
  assert(ACoverage!=NULL);
  assert(AMarkInitial>=0);
  assert(AMarkDuplicated>=0);
  assert(ADirectInfoIndex>=0);
  assert(this!=AMap || AMarkInitial!=AMarkDuplicated);

  int treated = getNewMark();
  int treated2 = (AMap==this?treated:AMap->getNewMark());

  for (int i=ORBIT_0; i<=ORBIT_0123; ++i)
    if (isOrbitUsed(TOrbit(i)))
      {
	// Duplication pour l'orbite i:
	for (ACoverage->reinit(); ACoverage->cont(); ++(*ACoverage))
	  {
	    CDart * d = **ACoverage;

	    if (!isMarked(d, treated))
	      {
		if (isMarked(d, AMarkInitial))
		  {
		    CDart * owner = getEmbeddingOwner(d, TOrbit(i));

		    if (owner!=NULL)
		      {
			CEmbedding * E = owner->getEmbedding(TOrbit(i));
			assert(E!=NULL);
		
			CCoverage * cov = getDynamicCoverage(d, TOrbit(i));
			assert(cov!=NULL);
			
			for (; cov->cont(); setMark(**cov, treated), ++(*cov))
			  if (isMarked(**cov, AMarkInitial) &&
			      !AMap->isMarked(DUP(**cov), treated2))
			    {
			      CEmbedding * dupE = E->copy();
			      DUP(**cov)->addEmbedding(dupE);
			      
			      AMap->markOrbit(DUP(**cov), TOrbit(i), treated2);
			      AMap->unmarkOrbit(DUP(**cov), TOrbit(i), AMarkDuplicated);
			    }
			
			delete cov;
		      }
		  }
		else
		  {
		    // Le 2ème test est la que pour le cas this==AMap (car on
		    // insère les brins dans la liste parcouru)
		    if ( this!=AMap || !isMarked(d, AMarkDuplicated) )
		      setMark(d, treated);
		  }
	      }
	  }

	// Démarquage:
	if (ACoverage->type() == COVERAGE_ALL && AMap==this)
	  negateMaskMark(treated);
	else
	  for (ACoverage->reinit(); ACoverage->cont(); ++(*ACoverage))
	    {
	      if (isMarked(**ACoverage, treated))
		unmarkOrbit(**ACoverage, TOrbit(i), treated);

	      if (AMap->isMarked(DUP(**ACoverage), treated2))
		AMap->unmarkOrbit(DUP(**ACoverage), TOrbit(i), treated2);
	    }
      }

  freeMark(treated);
  if ( AMap!=this ) AMap->freeMark(treated2);
}
//******************************************************************************
#undef DUP
//******************************************************************************
