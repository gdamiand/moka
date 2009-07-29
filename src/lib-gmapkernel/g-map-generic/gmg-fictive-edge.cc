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
#include "dynamic-coverage-real-face.hh"
using namespace GMap3d;
//******************************************************************************
void CGMapGeneric::shiftOneFictiveEdge(CDart* ADart, CDart* ADart2)
{
  assert( ADart!=NULL && ADart2!=NULL );
  assert( !isFree1(ADart) );

  CDart * d1 = alpha1(ADart);
  CDart * d2 = alpha21(ADart);
  CDart * d3 = alpha1(ADart2);

  unsew1(d1);       // rappel : unsew1 1-decoud en meme temps alpha3(d1)
  if (!isFree1(d2))
    unsew1(d2);

  if (!isFree1(ADart2))
    unsew1(ADart2);

  sew1(d1,d2);

  sew1(ADart,ADart2);

  if ( isFree1(d3) )
    sew1(alpha2(ADart),d3);
}
//******************************************************************************
unsigned int CGMapGeneric::shiftAllEdgesIncidentToVertex(CDart* ADart)
{
  CDart * toShift = alpha12(ADart);
  CDart * next = NULL;
  unsigned int nb = 0;
  
  while( toShift!=ADart )
    {
      next = alpha12(toShift);
      shiftOneFictiveEdge( toShift, alpha0(ADart) );
      ++nb;
      toShift = next;
    }

  return nb;
}
//******************************************************************************
void CGMapGeneric::shiftAllAdjacentFictiveEdges(CDart* ADart, int AMark)
{
  CDart * d = alpha12(ADart);
  CDart * tmp;

  while (isMarked(d,AMark) &&   // Tant qu'on est sur une arête fictive
	 d!=ADart ) // et qu'on n'est pas revenu sur l'arête de départ
    {
      tmp = alpha12(d);
      shiftOneFictiveEdge(d,alpha0(ADart));
      d=tmp;
    }
}
//******************************************************************************
void CGMapGeneric::shiftAllFictiveEdges(CDart* ADart, int AMark)
{
  shiftAllAdjacentFictiveEdges(ADart, AMark);

  shiftAllAdjacentFictiveEdges(alpha2(ADart), AMark);
}
//******************************************************************************
void CGMapGeneric::shiftAllFictiveEdgesAroundEdge(CDart* ADart, int AMark)
{
  assert( ADart!=NULL );
  //  assert( !isEdgeLoop(ADart) );
  
  CDart* actu    = ADart;
  CDart* toShift = NULL;
  CDart* tmp     = NULL;

  do
    {
      toShift = alpha12(actu);
      
      while( isMarked(toShift, AMark) && toShift!=actu )
	{
	  tmp = alpha12(toShift);
	  shiftOneFictiveEdge( toShift, alpha0(actu) );
	  toShift = tmp;
	}
      actu  = alpha23(actu);    // Puis on passe a l'autre volume.
    }
  while ( actu!=ADart );
}
//******************************************************************************
bool CGMapGeneric::existFictiveEdgeIncidentTo(CDart *ADart, int AMark)
{
  CDynamicCoverage123 it(this,ADart);
  while(it.cont())
    {
      if (isMarked(*it,AMark)) return true;
      ++it;
    }
  return false;
}
//******************************************************************************
int CGMapGeneric::findVertexType( CDart* ADart, CDart** AFirstDart,
				  int AMark )
{
  assert( ADart!=NULL );

  int  markVertex = getNewMark();
  int  markEdge	  = getNewMark();
  int  res	  = 0;
  int  nbReel	  = 0; // Pour compter le nombre d'arête réelle non boucle
  bool reelLoop	  = false; 

  CDynamicCoverageVertex it(this, ADart);
  for ( ; it.cont(); ++it )
    setMark(*it, markVertex);
 
  // On parcours le sommet. On s'arrête si on a trouvé et une boucle réelle
  // et une arête non boucle, ou plus de 2 arêtes réelles.
  for ( it.reinit(); it.cont() && nbReel<=2 && (!reelLoop || nbReel==1); ++it )
    {
      if ( !isMarked(*it, markEdge) )
	{
	  if ( !isMarked(*it, AMark) ) // On n'est pas sur une arête fictive
	    {
	      if ( isMarked(alpha0(*it), markVertex) )
		{
		  reelLoop = true; // Ici l'arête réelle est une boucle
		}
	      else
		{
		  if ( nbReel==0 )
		    *AFirstDart = *it; // Cette arête est réelle et non boucle
		}
	      ++nbReel; // On compte le nombre d'arête réelles
	      markOrbit(*it, ORBIT_EDGE, markEdge);
	    }
	}
    }

  if ( reelLoop ) // On a trouvé une boucle réelle
    {
      if ( nbReel==1 ) res = 1; // C'est la seule arête réelle
      else res = 3; // Il y a d'autres arêtes réelle incidentes au sommet
    }
  else
    {
      if ( nbReel==2 )
	res = 2; // On a exactement 2 arêtes réelles non boucle
      else if ( nbReel!=0 ) res = 3; // cas ==1 ou >2
    }
	  
  // On refait le parcours du sommet pour enlever les marques
  for ( it.reinit(); it.cont(); ++it )
    {
      if ( isMarked(*it, markEdge) )
	unmarkOrbit(*it, ORBIT_EDGE, markEdge);

      unsetMark(*it, markVertex);
    }

  assert(isWholeMapUnmarked(markVertex));
  assert(isWholeMapUnmarked(markEdge));
  
  freeMark(markVertex);
  freeMark(markEdge);

  return res;  
}
//******************************************************************************
CDart* CGMapGeneric::findIncidentEdgeNonLoop(CDart* ADart)
{
  assert( ADart!=NULL );
  assert( !isFree2(ADart) );

  CDart* res = NULL;
  int markVertex = getNewMark();

  CDynamicCoverageVertex it(this, ADart);

  // Première boucle pour marquer le sommet
  while( it.cont() ) { setMark(*it, markVertex); ++it; }
  
  for ( it.reinit(); it.cont() && res==NULL; ++it )
    {
      if ( !isMarked(alpha0(*it), markVertex) )
	{
	  res = *it; // Cette arête n'est pas une boucle
	}
    }

  for ( it.reinit(); it.cont(); ++it ) // Pour démarquer le sommet
    unsetMark(*it, markVertex);
  
  assert(isWholeMapUnmarked(markVertex));

  freeMark(markVertex);

  return res;
}
//******************************************************************************
int CGMapGeneric::markRealFace(CDart* ADart, int AMark, int AMark2)
{
  assert( ADart!=NULL );
  assert( !isMarked(ADart,AMark) );

  int res = 0;
  CDynamicCoverageRealFace it(this, ADart, AMark);
  while ( it.cont() )
    {
      setMark(it++, AMark2);
      ++res;
    }

  return res;
}
//******************************************************************************
int CGMapGeneric::unmarkRealFace(CDart* ADart, int AMark, int AMark2)
{
  assert( ADart!=NULL );
  assert( !isMarked(ADart,AMark) );

  int res = 0;
  CDynamicCoverageRealFace it(this, ADart, AMark);
  while ( it.cont() )
    {
      unsetMark(it++, AMark2);
      ++res;
    }

  return res;
}
//******************************************************************************
