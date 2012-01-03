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
#include "rounding-vertex.hh"
#include "g-map-vertex.hh"
#include "geometry.hh"
#include <cstdlib>
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CRoundingVertex::CRoundingVertex(CGMapVertex* AMap) :
  CRoundingGeneric(AMap),
  FMap(AMap)
{
  assert(FMap != NULL);

  FCoefs[0] = FMap->getNewDirectInfo();
  FCoefs[1] = FMap->getNewDirectInfo();
}
//------------------------------------------------------------------------------
CRoundingVertex::~CRoundingVertex()
{
  FMap->freeDirectInfo(FCoefs[0]);
  FMap->freeDirectInfo(FCoefs[1]);
}
//******************************************************************************
#define GET_KV(DART) ((FMap->isMarked((DART), AMarkNumber)) \
  		     ? getDartRoundingCoef((DART), 0) \
                     : 0.0 )
//------------------------------------------------------------------------------
int CRoundingVertex::roundMarkedVertices(int AMarkNumber, bool ADig)
{
  int nbRounded = 0;

  int directIndex = FMap->getNewDirectInfo();

  int treated  = FMap->getNewMark();
  int original = FMap->getNewMark();
  int newdarts = FMap->getNewMark(); FMap->negateMaskMark(newdarts);

  CDynamicCoverageAll it(FMap);

  // On marque tous les brins de la G-carte par original :
  FMap->negateMaskMark(original);

  /* Pour chaque brin de la carte, pointage du champ directInfo[directIndex]
   * sur le plongement du sommet topologique auquel il appartient :
   */
  FMap->pointDirectInfoToAttributeVertex(directIndex);

  // Chanfreinage :
  for (; it.cont(); ++it)
    if (!FMap->isMarked(*it, treated))
      {
  	if (FMap->isMarked(*it, AMarkNumber))
  	  {
  	    FMap->markOrbit(*it, ORBIT_VERTEX, treated);

  	    /* Initialement, tous les brins du sommet ont le même plongement,
	     * on ne détache donc que *it de son plongement.
  	     */
  	    CAttributeVertex* oldVertex = FMap->removeVertex(*it);

  	    CStaticCoverageVertex cov(FMap, *it);

  	    // Topologie -------------------------------------------------------

  	    /* On récupère la "dimension" d'une  cellule pour savoir si on doit 
  	     * fermer ou non :
  	     */
  	    int roundingDim  = getVertexRoundingDimension(*it, 0);
  	    int closureLevel = getEdgeRoundingDimension(*it, 0);

  	    // On chanfreine l'arête :
  	    if (!ADig)
  	      chamferVertex(*it, closureLevel);
  	    else if (roundingDim==3)
  	      chamferVertex(*it, 2);
  	    else
  	      chamferVertex(*it, roundingDim);

  	    /* Fermeture : uniquement en 3D (et pas en 2D) car cela ne veut rien
  	     * dire en 2D de fermer un objet par rapport à une arête :
  	     */
  	    if (!ADig && closureLevel==3)
  	      { 
  		bool stop = false;

  		for (CDynamicCoverageVolume cov(FMap, FMap->alpha123(*it));
  		     !stop && cov.cont(); ++cov)
  		  if (FMap->isFree2(*cov))
  		    { 
  		      FMap->stopUp(*cov, 2);
  		      stop = true;
  		    }
  	      }

  	    // Géométrie -------------------------------------------------------
  	    for ( ; cov.cont(); ++cov)
  	      // Pour le cas des faces vides ou des arêtes pendantes ??
  	      if (!FMap->isMarked(FMap->alpha1(*cov), treated))
  		{
  		  CDart* d0 = FMap->alpha0(*cov);

  		  CAttributeVertex* vertex0;

  		  if (FMap->isMarked(d0, original))
  		    vertex0 =
		      FMap->getDirectInfoAsAttributeVertex(d0, directIndex);
  		  else
  		    // Comment pourrait-il ne pas être original ???
  		    vertex0 = FMap->findVertex(d0);

  		  // Plongement du nouveau sommet:
  		  TCoordinate u = GET_KV(*cov);
  		  FMap->setVertex(*cov, (1-u) * *oldVertex + u * *vertex0);

  		  // Marquage des derniers brins créés :
		  FMap->markOrbit(*cov, ORBIT_VERTEX, treated);
  		}

  	    // Démarquage pour la destruction finale:
  	    // FMap->unsetMark(*it, treated);

  	    ++nbRounded;
  	  }
  	else
  	  FMap->setMark(*it, treated);
      }

  // Destruction des anciens plongements:
  for (it.reinit(); it.cont(); ++it)
    if (!FMap->isMarked(*it, treated))
      {
  	delete FMap->getDirectInfoAsAttributeVertex(*it, directIndex);
  	FMap->setMark(*it, treated);
      }

  // Libération des marques et des champs directInfo:
  FMap->negateMaskMark(treated);
  FMap->freeMark(treated);

  FMap->unmarkAll(original);
  FMap->freeMark(original);

  FMap->freeDirectInfo(directIndex);

  FMap->negateMaskMark(newdarts);
  FMap->markIncidentCells(ORBIT_EDGE  , newdarts);
  FMap->markIncidentCells(ORBIT_VERTEX, newdarts);

  FMap->deleteMarkedNullLengthEdges(newdarts);
  FMap->deleteMarkedFlatFaces      (newdarts);
  FMap->deleteMarkedFlatVolumes    (newdarts);

  FMap->unmarkAll(newdarts);
  FMap->freeMark(newdarts);

  return nbRounded;
}
//------------------------------------------------------------------------------
#undef GET_KV
//******************************************************************************
int CRoundingVertex::roundMarkedEdges(int AMarkNumber,
				      bool A3D, bool ADig, bool ASetback)
{
  assert(!ADig || A3D);

  int nbRounded = 0;

  int selected = FMap->getNewMark();
  int treated  = FMap->getNewMark();
  int initial  = FMap->getNewMark();
  int newdarts = FMap->getNewMark(); FMap->negateMaskMark(newdarts);

  int vertexToDestroy = FMap->getNewMark(); // sommets à détruire

  int directOldVertex = FMap->getNewDirectInfo(); // anciens sommets
  int directNewVertex = FMap->getNewDirectInfo(); // nouveaux sommets

  CDynamicCoverageAll it(FMap);

  /* Pointage du champ directInfo[directOldVertex] sur le plongement sommet,
   * ceci pour tous les brins de la carte :
   */
  FMap->pointDirectInfoToAttributeVertex(directOldVertex);

  /* Extension de la liste et suppression des anciens sommets des listes
   * d'attributs :
   */
  doListExtension(AMarkNumber, initial, selected, treated,
		  vertexToDestroy, &it);

  /* Calcul des Min-Max :
   * Le calcul des min-Max est différent en 2D et en 3D
   * RQ : Le setback n'intervient pas ici car on ne fait que chercher
   * le brin porteur du max pour les profondeurs d'arête. D'ailleurs
   * les brins comparés ici possèdent le même coeff de setback par 
   * initialisation.
   */
  int max = FMap->getNewMark();

  doMinMaxCalcul(FCoefs[1], selected, treated, initial, max, A3D, &it);
  /*
    for (it.reinit(); it.cont(); ++it)
    if (is_marked(*it, max))
    set_mark(*it,1);
    else
    set_mark(*it,2);
  */

  // Calcul des nouveaux sommets !!!!
  doNewVerticesCalcul(selected, treated, initial, max, directOldVertex, 
  		      directNewVertex, ASetback, A3D, &it);

  nbRounded = doEdgeChamferingInRounding(treated, initial, ADig, A3D, &it);

  doVerticesChamferingInRounding(treated, initial, ADig, A3D, &it);

  // Retrait des arêtes de liaison inutiles (et des faces plates) :
  for (it.reinit(); it.cont(); ++it)
    if (!FMap->isMarked(*it, treated))
      {
  	if (FMap->isMarked(*it, selected))
  	  {
  	    CDart* d101 = FMap->alpha101(*it); // d101 est forcément sélectionné

	    FMap->setMark(FMap->alpha3(*it ), treated);
  	    FMap->setMark(             d101 , treated);
  	    FMap->setMark(FMap->alpha3(d101), treated);

  	    if ((*it!=d101) && FMap->isMarked(*it, max) &&
  		FMap->isMarked(d101, max) &&
		!FMap->isMarked(FMap->alpha1(*it), selected))
  	      {
		removeEdge(FMap->alpha1(*it));
  		// À faire: traitement des volumes vides.		    
  	      }
  	  }

  	FMap->setMark(*it, treated);
      }

  FMap->negateMaskMark(treated);

  // Position des nouveaux sommets :
  for (it.reinit(); it.cont(); ++it)
    if (!FMap->isMarked(*it, treated))
      {
  	if (FMap->isMarked(*it, selected))
  	  {
  	    assert(FMap->findVertex(*it) == NULL);

  	    // Position du nouveau :
	    CAttributeVertex* vertex =
	      FMap->getDirectInfoAsAttributeVertex(*it, directNewVertex);
  	    FMap->setVertex(*it, vertex);

  	    FMap->unsetMark(*it, selected);

  	    /* Destruction des autres, créés inutilement à cause des
  	     * suppressions de cellules plates qui ont été effectuées :
  	     */
  	    for (CDynamicCoverageVertex cov(FMap, *it); cov.cont(); ++cov)
  	      if (FMap->isMarked(*cov, selected))
  		{
  		  FMap->unsetMark(*it, selected);
  		  delete FMap->getDirectInfoAsAttributeVertex(*cov,
							      directNewVertex);
  		}

  	    FMap->markOrbit(*it, ORBIT_VERTEX, treated);
  	  }
  	else
  	  FMap->setMark(*it, treated);
      }

  FMap->negateMaskMark(treated);

  // Destruction des anciens plongements :
  for (it.reinit(); it.cont(); ++it)
    if (FMap->isMarked(*it, vertexToDestroy))
      {
  	delete FMap->getDirectInfoAsAttributeVertex(*it, directOldVertex);
  	FMap->unsetMark(*it, vertexToDestroy);
      }

  // Libération des marques et des champs direct_info utilisés :
  FMap->unmarkAll(max);
  FMap->freeMark(max);

  FMap->unmarkAll(initial);
  FMap->freeMark(initial);

  FMap->unmarkAll(selected);
  FMap->freeMark(selected);

  FMap->freeMark(treated);

  FMap->freeMark(vertexToDestroy);
  FMap->freeDirectInfo(directOldVertex);
  FMap->freeDirectInfo(directNewVertex);

  FMap->negateMaskMark(newdarts);
  FMap->markIncidentCells(ORBIT_EDGE  , newdarts);
  FMap->markIncidentCells(ORBIT_VERTEX, newdarts);

  FMap->deleteMarkedNullLengthEdges(newdarts);
  FMap->deleteMarkedFlatFaces      (newdarts);
  FMap->deleteMarkedFlatVolumes    (newdarts);

  FMap->unmarkAll(newdarts);
  FMap->freeMark(newdarts);

  return nbRounded;
}
//------------------------------------------------------------------------------
void CRoundingVertex::doListExtension(int AMarkNumber, int AMarkInitial,
				      int AMarkSelected, int AMarkTreated,
				      int AMarkVertexToDestroy, CCoverage* ACov)
{
  FMap->markIncidentCells(ORBIT_EDGE, AMarkNumber, AMarkInitial );
  FMap->markIncidentCells(ORBIT_EDGE, AMarkNumber, AMarkSelected);

  for ( ; ACov->cont(); ++(*ACov))
    if (!FMap->isMarked(**ACov, AMarkTreated))
      {
  	if (FMap->isMarked(**ACov, AMarkSelected))
  	  {
  	    for (CDynamicCoverageVertex cov(FMap, **ACov); cov.cont(); ++cov)
  	      {
  		if (FMap->getVertex(*cov) != NULL)
  		  {
  		    FMap->removeVertex(*cov);
  		    FMap->setMark(*cov, AMarkVertexToDestroy);
  		  }

  		FMap->setMark(*cov, AMarkSelected);
  		FMap->setMark(*cov, AMarkTreated );
  	      }
  	  }
  	else
  	  FMap->setMark(**ACov, AMarkTreated);
      }
  
  FMap->negateMaskMark(AMarkTreated);
}
//------------------------------------------------------------------------------
#define GET_KE(DART) ((FMap->isMarked((DART), AMarkInitial)) \
  		      ? (getDartRoundingCoef((DART), 1)) \
                      : 0.0 )
//..............................................................................
void CRoundingVertex::doMinMaxCalcul(int /*ADirectCoef1*/,
				     int AMarkSelected, int AMarkTreated,
				     int AMarkInitial, int AMarkMax,
				     bool A3D, CCoverage* ACov)
{
  if (!A3D)
    {  
      for (ACov->reinit(); ACov->cont(); ++(*ACov))
  	if (!FMap->isMarked(**ACov, AMarkTreated))
  	  {
  	    if (FMap->isMarked(**ACov, AMarkSelected))
  	      {
  		CDartVertex* d    = (CDartVertex*)                **ACov ;
  		CDartVertex* d121 = (CDartVertex*) FMap->alpha121(**ACov);

  		/* Voir ce qui se passe si d121 n'est pas le brin qu'on pense
  		 * Rayons d'arrondi associés au brin courant et à son image par
  		 * FMap->alpha2 :
  		 */
  		TCoordinate val    = GET_KE(d   );
  		TCoordinate val121 = GET_KE(d121);

  		if (isPositive(val-val121)) FMap->setMark(d   , AMarkMax);
  		if (isPositive(val121-val)) FMap->setMark(d121, AMarkMax);

  		FMap->setMark(d121, AMarkTreated);
  	      }

  	    FMap->setMark(**ACov, AMarkTreated);
  	  }
    }
  else
    { 
      TCoordinate maxVal;

      for (ACov->reinit(); ACov->cont(); ++(*ACov))
  	if (!FMap->isMarked(**ACov, AMarkTreated))
  	  {
  	    if (FMap->isMarked(**ACov, AMarkSelected))
  	      {
  		maxVal = 0;

  		// Récupération du rayon d'arrondi maximum :
  		for (CDynamicCoverage23 cov1(FMap, FMap->alpha1(**ACov));
  		     cov1.cont(); ++cov1)
  		  {
  		    TCoordinate cVal = GET_KE(FMap->alpha1(*cov1));

  		    if (isPositive(cVal-maxVal))
  		      maxVal = cVal;
  		  }

  		// Marquage des brins par min ou Max :
  		for (CDynamicCoverage23 cov2(FMap, FMap->alpha1(**ACov));
  		     cov2.cont(); ++cov2)
  		  {
  		    if (isPositive(GET_KE(FMap->alpha1(*cov2)) - maxVal))
  		      FMap->setMark(FMap->alpha1(*cov2), AMarkMax);

  		    FMap->setMark(FMap->alpha1(*cov2), AMarkTreated);
  		  }
  	      }

  	    FMap->setMark(**ACov, AMarkTreated);
  	  }
    }

  FMap->negateMaskMark(AMarkTreated);
}
//..............................................................................
#undef GET_KE
//------------------------------------------------------------------------------
#define GET_KV(DART) (getDartRoundingCoef((DART), 0))
#define GET_KE(DART) ((FMap->isMarked(DART, AMarkInitial)) \
  		     ? (getDartRoundingCoef((DART), 1)) \
  		     : 0.0)
//..............................................................................
void CRoundingVertex::
doNewVerticesCalcul(int AMarkSelected, int AMarkTreated,
		    int AMarkInitial, int AMarkMax,
		    int ADirectIndexOld, int ADirectIndexNew,
		    bool ASetback, bool A3D, CCoverage* ACov)
{
  for (ACov->reinit(); ACov->cont(); ++(*ACov))
    if (!FMap->isMarked(**ACov,AMarkTreated))
      {
  	if (FMap->isMarked(**ACov, AMarkSelected))
  	  {
  	    CDartVertex* d     = (CDartVertex*) **ACov      ;
  	    CDartVertex* d0    = (CDartVertex*) FMap->alpha0 (d  );
  	    CDartVertex* d1    = (CDartVertex*) FMap->alpha1 (d  );
  	    CDartVertex* d10   = (CDartVertex*) FMap->alpha10(d  );
  	    CDartVertex* d010  = (CDartVertex*) FMap->alpha10(d0 );
  	    CDartVertex* d1010 = (CDartVertex*) FMap->alpha10(d10);

#define GET_V(DART) (* FMap-> \
	getDirectInfoAsAttributeVertex((DART), ADirectIndexOld))
  	    CAttributeVertex& V1 = GET_V(d    );
  	    CAttributeVertex& V2 = GET_V(d10  );
  	    CAttributeVertex& V3 = GET_V(d0   );
  	    CAttributeVertex& V4 = GET_V(d1010);
  	    CAttributeVertex& V5 = GET_V(d010 );
#undef GET_V
	    
  	    // Rayons associés aux brins courants:
  	    TCoordinate u   = GET_KE(d  );
  	    TCoordinate u0  = GET_KE(d0 );
  	    TCoordinate u10 = GET_KE(d10);
	    TCoordinate u1  = GET_KE(d1 );

  	    // Setback associé à d :
	    TCoordinate usb = GET_KV(d);

  	    CVertex V6 = (1-u )*V1 + u *V2;
  	    CVertex V7 = (1-u0)*V3 + u0*V5;

  	    CVertex V8,V9;

  	    if (FMap->isMarked(d1, AMarkMax)) // d1 max
  	      {
  		if (ASetback && isPositive(usb-u1) && !areEqual(usb,u1))
  		  {
  		    /* On a un setback dont on doit tenir compte.
  		     * On marque d1 min car l'arête de liaison doit maintenant
  		     * être conservée.
  		     */
  		    FMap->unsetMark(d1, AMarkMax);

  		    V8 = (1-usb )*V1 + usb*V3; 
  		    V9 = (1-(u10+usb-u1))*V2 + (u10+usb-u1)*V4;
  		  }
  		else
  		  {
  		    V8 = (1-u1 )*V1 + u1 *V3; 
  		    V9 = (1-u10)*V2 + u10*V4;
  		  }
  	      }
  	    else // d1 min
  	      {  
  		TCoordinate u2;

  		if (A3D)
  		  {
  		    CDynamicCoverage23 cov2(FMap, d);

  		    while (cov2.cont() &&
			   ! FMap->isMarked(FMap->alpha1(*cov2), AMarkMax))
		      ++cov2;

  		    u2 = GET_KE(FMap->alpha1(*cov2));
  		  }
  		else
		  {
  		    u2 = GET_KE(FMap->alpha21(d));
  		  }

  		TCoordinate ui = u10 + u2 - u1;

  		if (ASetback && isPositive(usb-u2) && !areEqual(usb,u2))
  		  {
  		    // On a un setback dont on doit tenir compte :
  		    V8 = (1-usb)*V1 + usb*V3; 
  		    V9 = (1-(u10+usb-u2))*V2 + (u10+usb-u2)*V4;    
  		  }
  		else
  		  {
  		    V8 = (1-u2)*V1 + u2*V3;
  		    V9 = (1-ui)*V2 + ui*V4;
  		  }
  	      }

  	    CVertex newVertex = CGeometry::getLinesIntersection(V6,V7, V8,V9);

  	    FMap->setDirectInfo(d, ADirectIndexNew,
				new CAttributeVertex(newVertex));

  	    if (FMap->isMarked(d1, AMarkMax) && FMap->isMarked(d,AMarkMax))
  	      {
  		FMap->setDirectInfo(d1, ADirectIndexNew,
				    new CAttributeVertex(newVertex));
  		// FMap->setMark(d1,AMarkTreated);
  	      }
  	  }

  	FMap->setMark(**ACov,AMarkTreated);
      }

  FMap->negateMaskMark(AMarkTreated);
}
//..............................................................................
#undef GET_V
#undef GET_K
#undef GET_KV
//------------------------------------------------------------------------------
int CRoundingVertex::
doEdgeChamferingInRounding(int AMarkTreated, int AMarkInitial, 
			   bool ADig,  bool A3D, CCoverage* ACov)
{
  int nbRounded = 0;
  int closureLevel;
  
  // Chanfreinage des arêtes :
  for (ACov->reinit(); ACov->cont(); ++(*ACov))
    if (! FMap->isMarked(**ACov, AMarkTreated))
      {
  	if (FMap->isMarked(**ACov, AMarkInitial))
  	  {
  	    CDartVertex* d = (CDartVertex*) **ACov;

  	    // On marque tous les brins de l'arête contenant d :
  	    FMap->markOrbit(**ACov, ORBIT_EDGE, AMarkTreated);

  	    FMap->negateMaskMark(AMarkTreated);

  	    /* On récupère la "dimension" d'une  cellule pour savoir si on doit 
  	     * fermer ou non :
  	     */
  	    closureLevel = getEdgeRoundingDimension(d,1);

  	    // On chanfreine l'arête :
  	    if (!ADig)
  	      chamferEdge(d, closureLevel);
  	    else
  	      {		
  		if (getVertexRoundingDimension(d, 1) == 3)
  		  chamferEdge(d, 2);
  		else
  		  chamferEdge(d, getVertexRoundingDimension(d, 1));
  	      }

  	    /* Fermeture : uniquement en 3D et pas en 2D car cela ne veut rien
  	     * dire en 2D de fermer un objet par rapport à une arête.
  	     */
  	    if (A3D && !ADig && closureLevel==3)
  	      {
  		bool stop = false;

  		for (CDynamicCoverageVolume cov(FMap, FMap->alpha23(d));
  		     !stop && cov.cont(); ++cov) 
  		  if (FMap->isFree2(*cov))
  		    {
  		      FMap->stopUp(*cov, 2);
  		      stop = true;
  		    }
  	      }

  	    FMap->negateMaskMark(AMarkTreated);
	    ++nbRounded;
  	  }

  	FMap->setMark(**ACov, AMarkTreated);
      }

  FMap->negateMaskMark(AMarkTreated);

  return nbRounded;
}
//------------------------------------------------------------------------------
void CRoundingVertex::doVerticesChamferingInRounding(int AMarkTreated,
						     int AMarkInitial,
						     bool ADig, bool A3D,
						     CCoverage* ACov)
{
  int closureLevel;

  // Chanfreinage des sommets :
  for (ACov->reinit(); ACov->cont(); ++(*ACov))
    if (!FMap->isMarked(**ACov, AMarkTreated))
      {
  	if (FMap->isMarked(**ACov, AMarkInitial))
  	  {
	    // On marque tous les brins de l'orbite sommet incidente à *it :
  	    FMap->markOrbit(**ACov, ORBIT_VERTEX, AMarkTreated);

  	    closureLevel = getEdgeRoundingDimension(**ACov,0);

  	    // On chanfreine le sommet:
  	    FMap->negateMaskMark(AMarkTreated);

  	    if (!ADig)
  	      chamferVertex(**ACov, closureLevel);
  	    else if (getVertexRoundingDimension(**ACov, 0)==3)
  	      chamferVertex(**ACov, 2);
  	    else
  	      chamferVertex(**ACov, getVertexRoundingDimension(**ACov, 0));

  	    // Fermeture :
  	    if (A3D && !ADig && closureLevel==3)
  	      {
  		bool stop = false;

  		for (CDynamicCoverageVolume cov(FMap, FMap->alpha123(**ACov));
  		     !stop && cov.cont(); ++cov)
  		  if (FMap->isFree2(*cov))
  		    {
  		      FMap->stopUp(*cov, 2);
		      stop = true;
  		    }
  	      }
  	    // 2D et 3D même combat :
  	    else if (!ADig && closureLevel==2)
  	      {
  		bool stop = false;

  		for (CDynamicCoverageVolume cov(FMap, FMap->alpha123(**ACov));
  		     !stop && cov.cont(); ++cov) 
  		  if (FMap->isFree1(*cov))
  		    {
  		      FMap->stopUp(*cov, 1);
  		      stop = true;
  		    }
  	      }

  	    FMap->negateMaskMark(AMarkTreated);
  	  }

  	FMap->setMark(**ACov, AMarkTreated);
      }

  FMap->negateMaskMark(AMarkTreated);
}
//******************************************************************************
TCoordinate CRoundingVertex::getDartRoundingCoef(CDart* ADart,
						 int ADimension) const
{
  assert(ADimension == 0 || ADimension == 1);
  return ((long int) (ADart->getDirectInfo(FCoefs[ADimension]))) / 1000.0;
}
//------------------------------------------------------------------------------
void CRoundingVertex::setDartRoundingCoef(CDart* ADart, int ADimension,
					  TCoordinate AValue) const
{
  assert(ADimension == 0 || ADimension == 1);
  ADart->setDirectInfo(FCoefs[ADimension], (void*) (long int) (1000 * AValue));
}
//******************************************************************************
