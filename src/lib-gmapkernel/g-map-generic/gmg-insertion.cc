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
using namespace std;
using namespace GMap3d;
//******************************************************************************
CDart * CGMapGeneric::insertVertex(CDart * ADart)
{
  assert(ADart!=NULL);

  bool closed = !isFree0(ADart);
  int treated = getNewMark();

  CDynamicCoverage23 it(this, ADart);

  for (; it.cont(); setMark(*it, treated), ++it)
    {
      CDart * d0 = alpha0(*it);
      CDart * n1 = addMapDart();
      CDart * n2 = addMapDart();

      linkAlpha0(*it, n1);
      if (closed) linkAlpha0(d0 , n2);

      linkAlpha1(n1, n2);

      if (!isFree2(*it) && isMarked(alpha2(*it), treated))
	{
	  linkAlpha2(n1, alpha20(*it));
	  linkAlpha2(n2, alpha21(n1 ));
	}

      if (!isFree3(*it) && isMarked(alpha3(*it), treated))
	{
	  linkAlpha3(n1, alpha30(*it));
	  linkAlpha3(n2, alpha31(n1 ));
	}
    }

  unmarkOrbit(ADart, ORBIT_23, treated);
  freeMark(treated);

  return alpha0(ADart);
}
//******************************************************************************
int CGMapGeneric::insertVertexOnMarkedEdges(int AMarkNumber)
{
  CDynamicCoverageAll it(this);
  int nbInserted = 0;
  int treated = getNewMark();

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    markOrbit(*it, ORBIT_EDGE, treated);

	    CDart * newVertex= insertVertex(*it);
	    markOrbit(newVertex, ORBIT_VERTEX, treated);
	    ++nbInserted;
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  return nbInserted;
}
//******************************************************************************
CDart * CGMapGeneric::insertEdge(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1!=NULL && ADart2!=NULL);

  // Première demi-face:
  CDart * n1 = addMapDart();
  CDart * n2 = addMapDart();

  linkAlpha0(n1,n2);

  CDart * dd1 = isFree1(ADart1) ? NULL : alpha1(ADart1);
  CDart * dd2 = isFree1(ADart2) ? NULL : alpha1(ADart2);

  linkAlpha1(ADart1,n1); linkAlpha1(ADart2,n2);

  if (dd1!=NULL || dd2!=NULL)
    {
      CDart * nn1=addMapDart();
      CDart * nn2=addMapDart();

      linkAlpha0(nn1,nn2);
      linkAlpha2(n1,nn1); linkAlpha2(n2,nn2);

      if (dd1!=NULL) linkAlpha1(dd1,nn1);
      if (dd2!=NULL) linkAlpha1(dd2,nn2);
    }

  // Éventuellement, deuxième demi-face:
  if (!isFree3(ADart1))
    {
      n1=addMapDart(); n2=addMapDart();
      linkAlpha0(n1,n2);
      linkAlpha3(n1,alpha1(ADart1)); linkAlpha3(n2,alpha1(ADart2));

      if (dd1!=NULL) unlinkAlpha1(alpha3(ADart1));
      if (dd2!=NULL) unlinkAlpha1(alpha3(ADart2));

      linkAlpha1(n1,alpha3(ADart1)); linkAlpha1(n2,alpha3(ADart2));
      if (dd1!=NULL || dd2!=NULL)
        {
          CDart * nn1=addMapDart();
	  CDart * nn2=addMapDart();

          linkAlpha0(nn1,nn2);
          linkAlpha2(n1,nn1); linkAlpha2(n2,nn2);

          if (dd1!=NULL) linkAlpha1(alpha3(dd1),nn1);
          if (dd2!=NULL) linkAlpha1(alpha3(dd2),nn2);

          linkAlpha3(alpha1(dd1),nn1); linkAlpha3(alpha1(dd2),nn2);
        }
    }

  return alpha1(ADart1);
}
//******************************************************************************
int CGMapGeneric::insertEdgeOnMarkedFaces(int AMarkNumber,
					  bool ANoCrossedFace,
					  bool /*ANoTwoEdgesFace*/)
{
  CDynamicCoverageAll it(this);
  int nbInserted = 0;
  int treated = getNewMark();
  int mark = getNewMark();

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (!isMarked(*it, AMarkNumber))
	  setMark(*it, treated);
	else
	  {
	    // Pour chaque face non traitée, faire:
	    markOrbit(*it, ORBIT_FACE, treated);

	    // Retrouver deux sommets sélectionnés de la face:
	    bool canInsert = true;
	    CDart * d1 = NULL, * d2 = NULL;

	    CDynamicCoverageFace cov(this, *it);
	
	    for (; cov.cont() && canInsert; ++cov)
	      if (isMarked(*cov, AMarkNumber) && !isMarked(*cov, mark))
		{
		  if (ANoCrossedFace)
		    markOrbit(*cov, ORBIT_13, mark);
		
		  if (d1==NULL)
		    d1= *cov;
		  else if (d2==NULL)
		    d2= *cov;
		  else
		    canInsert = false; // trop de sommets sélectionnés
		}
	
	    if (ANoCrossedFace)
	      unmarkOrbit(*it, ORBIT_FACE, mark);
	
	    canInsert &= (d2!=NULL);
	
  	    if (canInsert && ANoCrossedFace && !isSameOrbit(d1,d2,ORBIT_01))
  	      d2 = alpha3(d2);

	    if (canInsert && belongToSameOrientedOrbit(d1, d2, ORBIT_01))
	      d2 = alpha1(d2);
	
	    // Insertion effective:
	    if (canInsert)
	      {
		CDart * newEdge = insertEdge(d1,d2);
		markOrbit(newEdge, ORBIT_EDGE, treated);
		++nbInserted;
	      }
	  }
      }

  negateMaskMark(treated);
  freeMark(treated);
  freeMark(mark);

  return nbInserted;
}
//******************************************************************************
#define IS_MARKED_EDGE(D) \
 ( \
   isMarked(D, AMarkNumber) || isMarked(alpha0(D), AMarkNumber) || \
   ( \
     ANoCrossedVolume && !isFree2(D) && \
     (isMarked(alpha2(D), AMarkNumber) || isMarked(alpha20(D), AMarkNumber)) \
   ) \
 )

#define IS_VALID_EDGE(D) \
 (ANoCrossedVolume || isFree2(D) || \
  (!isMarked(   D , AMarkNumber) && !isMarked(alpha0 (D), AMarkNumber)) || \
  (!isMarked(alpha2(D), AMarkNumber) && !isMarked(alpha02(D), AMarkNumber)))

#define IS_ON_SAME_EDGE(D1,D2) \
 ((D1)==(D2) || (ANoCrossedVolume && ((D1)==alpha2(D2))))
//******************************************************************************
// Méthode protégée!
bool CGMapGeneric::turnAroundVertex(CDart * ADart, bool ANoCrossedVolume,
				    int AMarkNumber,
				    CDart * & ANext, bool & ACrossed)
{
  assert(ADart!=NULL);
  assert(IS_MARKED_EDGE(ADart));

  bool firstDirection = true;
  bool validVertex = true;

  ANext = NULL;

  for (CDynamicCoverage12 cov(this, ADart); validVertex && cov.cont(); ++cov)
    {
      if (cov.prevOperationType() == OP_JUMP)
	firstDirection = false;

      if (IS_MARKED_EDGE(*cov) && !IS_ON_SAME_EDGE(*cov, ADart) &&
	  (ANext==NULL || !IS_ON_SAME_EDGE(*cov, ANext)))
	{
	  validVertex = ANext==NULL && IS_VALID_EDGE(*cov);

	  if (validVertex)
	    {
	      ACrossed =
		cov.prevOperationType() ==
		(firstDirection ? OP_ALPHAJ : OP_ALPHAI);

	      if (ACrossed && !isFree2(*cov) && IS_MARKED_EDGE(alpha2(*cov)))
		{
		  ACrossed = false;
		  ANext = alpha2(*cov);
		}
	      else
		ANext = *cov;
	    }
	  else
	    ANext = NULL;
	}
    }

  return validVertex;
}
//******************************************************************************
bool CGMapGeneric::canInsertFace(CDart * ADart, int AMarkNumber,
				 bool ANoCrossedVolume,
				 bool ANoTwoEdgesFace,
				 bool ANoTwoFacesVolume)
{
  assert(ADart!=NULL);
  assert(isMarked(ADart, AMarkNumber));

  /* On suit le chemin donné par les arêtes sélectionnées du volume en vérifiant
   * à chaque sommet qu'il n'y alpha pas plus de 2 arêtes sélectionnées
   * incidentes au sommet:
   */

  CDart * current = ADart;
  int nbVertices = 0;

  bool firstDirection = true;
  bool finished       = false;
  bool canInsert      = IS_VALID_EDGE(current);
  bool sameFace[2]    = { true, true };

  // On part d'un côté, puis de l'autre:

  while (canInsert && !finished)
    {
      // Pour chaque arête du chemin:
      // On tourne autour du sommet:
      ++nbVertices;

      // On tourne autour du sommet courant:
      CDart * next;
      bool crossed;

      canInsert = turnAroundVertex(current, ANoCrossedVolume, AMarkNumber,
				   next, crossed);
      if (next!=NULL)
	{
      sameFace[0] =
	sameFace[crossed ? 1 : 0] && isSameOrbit(alpha1 (next), current, ORBIT_2);

      sameFace[1] =
	sameFace[crossed ? 0 : 1] && isSameOrbit(alpha21(next), current, ORBIT_2);
	}

      if (canInsert)
	{
	  if (next==NULL || isFree0(next))
	    {
	      // On essaie de repartir dans l'autre sens:
	      finished = !firstDirection || isFree0(ADart);
	      firstDirection = false;
	      current = alpha0(ADart);
	    }
	  else
	    {
	      // On continue notre bonhomme de chemin:
	      current = alpha0(next);
	
	      if (IS_ON_SAME_EDGE(current, ADart))
		finished = true;
	    }
	}
    }

  // Résultat (si firstDirection vaut 'vrai', c'est que la face est fermée):
  bool enoughEdges = !ANoTwoEdgesFace || nbVertices >= (firstDirection ? 3 : 2);
  bool enoughFaces = !ANoTwoFacesVolume || (!sameFace[0] && !sameFace[1]);

  return canInsert && enoughEdges && enoughFaces;
}
//******************************************************************************
CDart * CGMapGeneric::insertFace(CDart * ADart, int AMarkNumber,
				bool ANoCrossedVolume)
{
  assert(ADart!=NULL);
  assert(canInsertFace(ADart, AMarkNumber, ANoCrossedVolume));

  // On suit le chemin donné par les arêtes sélectionnées du volume:
  bool firstDirection = true ;
  bool finished       = false;
  bool turnedAround   = false; // dernière opération: tourner autour d'un sommet
  bool jumped         = false; // dernière opération: sauter au sommet suivant

  CDart * prev = NULL, * current = ADart, * next;

  // On part d'un côté, puis de l'autre:

  while (!finished)
    {
      assert(!turnedAround || !jumped);

      // Pour chaque brin du chemin:

      // 1) Recherche du brin suivant:
      if (turnedAround)
	{
	  // On passe au sommet suivant:
	  next = isFree0(current) ? NULL : alpha0(current);
	}
      else
	{
	  // On tourne autour du sommet:
	  bool crossed;
	  turnAroundVertex(current, ANoCrossedVolume,
			   AMarkNumber, next, crossed);
	
	  if (next!=NULL && crossed && ANoCrossedVolume)
	    {
	      assert(!isFree2(current));
	      current = alpha2(current);
	      if (prev!=NULL)
		{
		  assert(!isFree2(prev));
		  prev = alpha2(prev);
		}
	    }
	}

      // 2) Création d'une partie de la nouvelle face:
      CDart * d1 = current;
      CDart * d2 = isFree2(d1) ? NULL : alpha2(d1);
      CDart * n1 = addMapDart();
      CDart * n2 = addMapDart();

      if (d2!=NULL)
    	{
    	  unlinkAlpha2(d1);
    	  linkAlpha2(d2,n2);
	}

      linkAlpha3(n1,n2);
      linkAlpha2(d1,n1);

      if (turnedAround)
    	{
    	  linkAlpha1(alpha2 (d1), alpha2 (prev));
	  linkAlpha1(alpha23(d1), alpha23(prev));
    	}

      if (jumped)
  	{
  	  linkAlpha0(n1, alpha02 (d1));
	  linkAlpha0(n2, alpha023(d1));
  	}

      // 3) Passage au brin suivant:
      if (next==NULL)
	{
	  // On essaie de repartir dans l'autre sens:
	  finished = !firstDirection || isFree0(ADart);
	  firstDirection = false;
	  turnedAround = false;
	  jumped = true;
	  prev = ADart;
	  current = alpha0(ADart);
	}
      else
	if (IS_ON_SAME_EDGE(next, ADart))
	  finished = true;
	else
	  {
	    prev    = current; jumped       =  turnedAround;
	    current = next   ; turnedAround = !turnedAround;
	  }
    }

  // Dernière 0-couture (si la face est fermée):
  if (firstDirection)
    {
      assert(!isFree2(next));
      linkAlpha0(alpha2 (ADart), alpha02 (ADart));
      linkAlpha0(alpha23(ADart), alpha023(ADart));
    }

  return alpha2(ADart);
}
//******************************************************************************
#undef IS_ON_SAME_EDGE
#undef IS_VALID_EDGE
#undef IS_MARKED_EDGE
//******************************************************************************
int CGMapGeneric::insertFaceOnMarkedVolumes(int AMarkNumber,
					    bool ANoCrossedVolume,
					    bool ANoTwoEdgesFace,
					    bool ANoTwoFacesVolume)
{
  CDynamicCoverageAll it(this);
  int nbInserted = 0;
  int treated = getNewMark();

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    // Marquage de l'ensemble de la "face" sélectionnée:
	    stack<CDart *> toTreat; toTreat.push(*it);
	
	    while (!toTreat.empty())
	      {
		CDart * dart = toTreat.top(); toTreat.pop();
		
		for (CDynamicCoverage12 cov(this, dart); cov.cont(); ++cov)
		  if (!isMarked(*cov, treated))
		    {
		      if (!isFree0(*cov) && !isMarked(alpha0(*cov), treated) &&
			  (isMarked(*cov, AMarkNumber) ||
			   isMarked(alpha0(*cov), AMarkNumber)))
			toTreat.push(alpha0(*cov));

		      setMark(   *cov , treated);
		      setMark(alpha2(*cov), treated);
		    }
	      }
	
	    // Tentative d'insertion de face:
	    if (canInsertFace(*it, AMarkNumber, ANoCrossedVolume,
			      ANoTwoEdgesFace, ANoTwoFacesVolume))
	      {
		CDart * newFace = insertFace(*it, AMarkNumber, ANoCrossedVolume);

		markOrbit(newFace, ORBIT_FACE, treated);
		++nbInserted;
	      }
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  return nbInserted;
}
//******************************************************************************
