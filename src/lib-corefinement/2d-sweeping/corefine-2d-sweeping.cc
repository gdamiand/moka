/*
 * lib-corefinement : Opérations de corafinement.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-corefinement
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

//********************************************************************************
#include "corefine-2d-sweeping.hh"
#include "g-map-vertex.hh"
#include "geometry.hh"
#include <cassert>
using namespace GMap3d;
using namespace std;
//******************************************************************************
CCorefineSegmentsSweeping::CCorefineSegmentsSweeping(CGMapVertex* AMap) :
  FMap(AMap)
{
  assert(FMap != NULL);
}
//------------------------------------------------------------------------------
CCorefineSegmentsSweeping::~CCorefineSegmentsSweeping()
{
}
//******************************************************************************
#define GET_V(DART) FMap->getDirectInfoAsAttributeVertex((DART), directVertex)

#define IT_PRED(IT,SET) (((IT)==(SET).begin()) \
                         ? NULL \
                         : * (--(IT), (IT)++))

#define IT_SUCC(IT,SET) (++(IT), \
                         ((IT)==(SET).end()) \
                          ? (--(IT), (CDartVertex *) NULL) \
                          : (*((IT)--)))
//------------------------------------------------------------------------------
void CCorefineSegmentsSweeping::corefine(CDart* ADart1, CDart* ADart2,
					 const CVertex& ANormalVector)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);
  assert(FMap->isIFreeOrbit(ADart1, 3, ORBIT_CC));
  assert(FMap->isIFreeOrbit(ADart2, 3, ORBIT_CC));
  assert(!ANormalVector.isNull());

  if (FMap->isSameOrbit(ADart1, ADart2, ORBIT_CC))
    return;

  // 0) Réservation des ressources:
  int treated = FMap->getNewMark();

  int directVertex = FMap->getNewDirectInfo();
  int mesh1        = FMap->getNewMark();
  int extremity1   = FMap->getNewMark();

  CDart* dart;

  // 0) Fermeture des 2-bords:
  for (dart = ADart1; dart!=NULL; dart = dart==ADart1 ? ADart2 : NULL)
    {
      for (CDynamicCoverageCC cov(FMap, dart); cov.cont(); ++cov)
	if (FMap->isFree2(*cov))
	  FMap->stopUp(*cov, 2);
    }

  // 1) Initialisations:
  FMap->pointDirectInfoToAttributeVertex(directVertex);
  FMap->markOrbit(ADart1, ORBIT_CC, mesh1);

  // 2) Tri des extrémités selon l'ordre lexicographique:
  CDartLexicoCompare lexComparator(FMap, directVertex,
				   extremity1, ANormalVector,
				   true /*comparaison exacte*/);

  LEX_SET eventSet(lexComparator);

  for (dart = ADart1; dart!=NULL; dart = dart==ADart1 ? ADart2 : NULL)
    {
      for (CDynamicCoverageCC cov(FMap, dart); cov.cont(); ++cov)
	if (! FMap->isMarked(*cov, treated))
	  {
	    FMap->markOrbit(*cov, ORBIT_02, treated);
	
	    if (! FMap->isFree0(*cov))
	      {
		if (lexComparator((CDartVertex*) *cov,
				  (CDartVertex*) FMap->alpha0(*cov)))
		  FMap->setMark(             *cov , extremity1);
		else
		  FMap->setMark(FMap->alpha0(*cov), extremity1);

  		eventSet.insert((CDartVertex*)              *cov );
  		eventSet.insert((CDartVertex*) FMap->alpha0(*cov));
	      }
	  }

      FMap->unmarkOrbit(dart, ORBIT_CC, treated);
    }

  // 3) Balayage et insertion des nouveaux sommets:
  LEX_SET eventTreatedSet(lexComparator);

  CDartVertexerticalCompare
    vertComparator(FMap, directVertex, extremity1, ANormalVector);

  VERT_SET sweepingSet(vertComparator);

  while (eventSet.begin()!=eventSet.end())
    {
      CDartVertex* current = * eventSet.begin();

      vertComparator.setCurrentPoint(* GET_V(current));

      if (FMap->isMarked(current, extremity1))
	{
	  // EXTREMITÉ ENTRANTE D'UN SEGMENT:
	  // Insertion dans l'ensemble de balayage:
	  VERT_IT currentPos = sweepingSet.insert((CDartVertex *) current);
	  assert(sweepingSet.find((CDartVertex *) current) != sweepingSet.end());

	  // Récupération des voisins dans l'ensemble de balayage:
	  CDart * neighbors[2];

	  neighbors[0] = IT_PRED(currentPos, sweepingSet);
	  neighbors[1] = IT_SUCC(currentPos, sweepingSet);

	  // Tests d'intersection:
	  for (int i=0; i<2; ++i)
	    manageEdgesIntersection(current, neighbors[i], eventSet,
				    lexComparator, sweepingSet, extremity1,
				    mesh1, directVertex, ANormalVector);
	}
      else
	{
	  // EXTRÉMITÉ SORTANTE D'UN SEGMENT:
	  assert(lexComparator((CDartVertex*) FMap->alpha0(current),
			       (CDartVertex*) current));

	  VERT_IT it =
	    findElementInSweepingSet(sweepingSet, FMap->alpha0(current));

	  // Récupération des voisins dans l'ensemble de balayage:
	  CDart* neighbors[2];

	  neighbors[0] = IT_PRED(it, sweepingSet);
	  neighbors[1] = IT_SUCC(it, sweepingSet);

	  // Tests d'intersection:
  	  manageEdgesIntersection(neighbors[0], neighbors[1], eventSet,
				  lexComparator, sweepingSet, extremity1,
  				  mesh1, directVertex, ANormalVector);

	  // Suppression de l'extrémité entrante correspondante de l'ensemble
	  // de balayage:
	  sweepingSet.erase(it);
	}

      // Passage à l'élément suivant:
      eventTreatedSet.insert(current);
      eventSet.erase(current);
    }

  eventTreatedSet.swap(eventSet);

  // 4) Orientation:
  int exterior = FMap->getNewMark();

  CDart* darts[2] = { ADart1, ADart2 };
  
  for (int i=0; i<2; ++i)
    {
      if (CGeometry::getAngle(FMap->cellDimensionNormalVector(darts[i], 3),
			      ANormalVector) > 90)
	{
	  FMap->markOrbit      (darts[i], ORBIT_CC, exterior);
	  FMap->halfUnmarkOrbit(darts[i], ORBIT_CC, exterior);
	}
      else
	FMap->halfMarkOrbit(darts[i], ORBIT_CC, exterior);
    }

  // 5) Tri lexicographique des extrémités en tenant compte des erreurs
  //    d'arrondi:
  CDartLexicoCompare
    lexComparator2(FMap, directVertex,
		   extremity1, ANormalVector,
		   false /*comparaison à epsilon près*/);

  LEX_SET eventSet2(lexComparator2);
  LEX_IT it;

  for (it = eventSet.begin(); it!=eventSet.end(); ++it)
    eventSet2.insert(*it);

  // 6) Assemblage des 2 maillages:
  CDartAngularCompare
    angComparator(FMap, directVertex, ANormalVector);

  for (it = eventSet2.begin(); it!=eventSet2.end(); )
    {
      const CAttributeVertex & currentVertex = * GET_V(*it);
      bool uniqueTopoVertex = true;

      LEX_IT first = it++;

      while (it!=eventSet2.end() && * GET_V(*it) == currentVertex)
	{
	  if (GET_V(*it) != &currentVertex)
	    uniqueTopoVertex = false;

	  ++it;
	}

      if (!uniqueTopoVertex)
	{
	  ANG_SET angSet(angComparator);

	  // Tri angulaire:
	  for (LEX_IT local = first; local!=it; ++local)
	    {
	      CDart * current = *local;

	      assert(! FMap->isFree2(current));

	      if (! FMap->isMarked(current, exterior))
		current = FMap->alpha2(current);

	      assert(FMap->isMarked(current, exterior));
	      assert(FMap->findVertex(current) == GET_V(current));
	
	      angSet.insert((CDartVertex*) current);
	    }

	  // Coutures:
	  ANG_IT round = angSet.end();
	  CDart* pred = *(--round);

	  for (round = angSet.begin(); round!=angSet.end(); ++round)
	    {
	      if (! FMap->isFree1(FMap->alpha2(pred)))
		FMap->unsew1(FMap->alpha2(pred));

	      if (! FMap->isFree1(*round))
		FMap->unsew1(*round);

	      assert(FMap->isMarked(FMap->alpha2(pred), exterior) !=
		     FMap->isMarked(*round, exterior));

	      FMap->sew1(FMap->alpha2(pred), *round);
	      pred = *round;
	    }

	  // Mise-à-jour des champs direct-info:
	  FMap->pointDirectInfoToAttributeVertex(directVertex, pred);
	}
    }

  // 7) Suppression des arêtes doubles:
  int toDelete = FMap->getNewMark();

  CDynamicCoverageAll itAll(FMap);

  for (; itAll.cont(); ++itAll)
    if (! FMap->isMarked(*itAll, toDelete) &&
	! FMap->isFree0(*itAll) && ! FMap->isFree1(*itAll) &&
	(GET_V(FMap->alpha0(*itAll)) == GET_V(FMap->alpha10(*itAll))))
      {
	if (FMap->alpha01(*itAll) != FMap->alpha10(*itAll))
	  {
	    // "Décroisement" des arêtes:
	    CDart* d0  = FMap->alpha0 (*itAll); FMap->unsew0(d0 );
	    CDart* d10 = FMap->alpha10(*itAll); FMap->unsew0(d10);
	
	    FMap->sew0(             *itAll , FMap->alpha2(d10));
	    FMap->sew0(FMap->alpha1(*itAll), FMap->alpha2(d0 ));
	  }

	assert(FMap->alpha01(*itAll) == FMap->alpha10(*itAll));
	
	// Isolement et marquage de la face plate:
	CDart* d2  = FMap->alpha2 (*itAll); FMap->unsew2(d2 );
	CDart* d12 = FMap->alpha12(*itAll); FMap->unsew2(d12);
	
	FMap->sew2(d2, d12);
	
	FMap->markOrbit(*itAll, ORBIT_FACE, toDelete);
      }

  // Suppression des brins marqués:
  for (itAll.reinit(); itAll.cont(); )
    {
      CDart * current = itAll++;

      if (FMap->isMarked(current, toDelete))
	FMap->delMapDart(current);
    }

  FMap->freeMark(toDelete);

  // 8) Libération des ressources:
  FMap->freeMark(treated);
  FMap->freeDirectInfo(directVertex);

  FMap->unmarkAll(mesh1);
  FMap->freeMark(mesh1);

  FMap->unmarkAll(exterior);
  FMap->freeMark(exterior);

  FMap->unmarkAll(extremity1);
  FMap->freeMark(extremity1);
}
//------------------------------------------------------------------------------
#undef GET_V
#undef IT_PRED
#undef IT_SUCC
//******************************************************************************
VERT_IT CCorefineSegmentsSweeping::
findElementInSweepingSet(VERT_SET & ASweepingSet, CDart * AElement)
{
  assert(AElement != NULL);

  VERT_IT it = ASweepingSet.find((CDartVertex *) AElement);

  if (it == ASweepingSet.end())
    {
      cout << "Élément introuvable: " << AElement << "!" << endl;
      it = ASweepingSet.begin();
      while (it != ASweepingSet.end() && *it != AElement)
	++it;

      if (it==ASweepingSet.end())
	{
	  cout << * (CVertex*) FMap->findVertex(             AElement ) << endl;
	  cout << * (CVertex*) FMap->findVertex(FMap->alpha0(AElement)) << endl;
	}

      assert(it != ASweepingSet.end());
    }

  return it;
}
//******************************************************************************
#define GET_V(DART) FMap->getDirectInfoAsAttributeVertex((DART), ADirectVertex)
//------------------------------------------------------------------------------
void CCorefineSegmentsSweeping::
manageEdgesIntersection(CDart * ADart1, CDart * ADart2,
			LEX_SET & AEventSet,
			const CDartLexicoCompare& ALexComparator,
			VERT_SET & /*ASweepingSet*/,
			int AExtremity1, int AMesh1, int ADirectVertex,
			const CVertex & ANormalVector)
{
  if (ADart1==NULL || ADart2==NULL)
    return;

  if (FMap->isMarked(ADart1, AMesh1) == FMap->isMarked(ADart2, AMesh1))
    return;

  // Plongements de la première arête:
  CAttributeVertex* A = GET_V(             ADart1 );
  CAttributeVertex* B = GET_V(FMap->alpha0(ADart1));

  // Plongements de la deuxième arête:
  CAttributeVertex* C = GET_V(             ADart2 );
  CAttributeVertex* D = GET_V(FMap->alpha0(ADart2));

  // Intersection éventuelle:
  int cut[2];
  CDart* cutDart[2] = { ADart1, ADart2 };
  static CVertex intersections[2];

  CGeometry::getSegmentsIntersection(*A,*B,*C,*D, ANormalVector,
				     cut[0], intersections[0],
				     cut[1], intersections[1]);

  for (int i=1; i>=0; --i)
    if (cut[i]!=0)
      {
	CDart* toCut = cutDart[cut[i]-1];

	assert(ALexComparator((CDartVertex*) toCut,
			      (CDartVertex*) FMap->alpha0(toCut)));

	FMap->CGMapGeneric::insertVertex(toCut);
	FMap->setVertex(FMap->alpha0(toCut), intersections[i]);

	FMap->pointDirectInfoToAttributeVertex(ADirectVertex,
					       FMap->alpha0(toCut));

	FMap->setMark(FMap->alpha01(toCut), AExtremity1);
	
	if (FMap->isMarked(toCut, AMesh1))
	  FMap->markOrbit(FMap->alpha0(toCut), ORBIT_VERTEX, AMesh1);
	
	AEventSet.insert((CDartVertex*) FMap->alpha0 (toCut));
	AEventSet.insert((CDartVertex*) FMap->alpha01(toCut));

	assert(ALexComparator((CDartVertex*)                toCut,
			      (CDartVertex*) FMap->alpha0  (toCut)));
	assert(ALexComparator((CDartVertex*) FMap->alpha01 (toCut),
			      (CDartVertex*) FMap->alpha010(toCut)));
      }
}
//------------------------------------------------------------------------------
#undef GET_V
//******************************************************************************
