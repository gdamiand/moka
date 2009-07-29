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
void CGMapGeneric::shiftOneFictiveFaceAlongDanglingEdge(CDart* ADart,
							int AFictiveFaceMark,
							int ADeleteMark)
{
  assert( ADart!=NULL );
  assert( !isFree1(ADart) );
  assert( !isFree2(ADart) );
  assert( isFree3(ADart) );
  assert( !isFree3(alpha2(ADart)) );
  assert( isMarked(alpha2(ADart), AFictiveFaceMark) );
  
  CDart* last	 = NULL;
  CDart* d1	 = NULL;
  CDart* d2	 = NULL;
  
  CDynamicCoverage23 it(this,ADart);
  while ( it.cont() )
    {
      if ( isMarked(*it, AFictiveFaceMark) && !isMarked(*it, ADeleteMark) )
	{
	  d1 = alpha1(*it);
	  d2 = alpha01(*it);
	  
	  if ( !isFree1(*it) )
	    {
	      unsew1(*it);
	      assert( !isFree1(alpha0(*it)) );
	      unsew1(alpha0(*it));
	      sew1( d1, d2 );
	    }
	  
	  setMark( *it, ADeleteMark );
	  setMark( alpha0(*it), ADeleteMark );
	}
      last = it++;
    }

  assert( isFree3(last) && last!=ADart );
  unsew2( last );
  unsew2( ADart );

  sew2( last, ADart );  
}
//******************************************************************************
void CGMapGeneric::shiftOneFictiveFace(CDart* ADart, int AFictiveFaceMark,
				       int ADeleteMark)
{
  assert( ADart!=NULL );
  assert( !isFree1(ADart) );
  assert( !isFree2(ADart) );
  assert( isFree3(ADart) );
  assert( !isFree3(alpha2(ADart)) );
  assert( isMarked(alpha2(ADart), AFictiveFaceMark) );
  
  unsigned int directInfo = getNewDirectInfo();
  unsigned int nextAlpha  = 0;
  
  CDart * d1 = alpha1(ADart);
  CDart * d2 = alpha232(ADart);
  CDart * n1 = NULL;
  CDart * n2 = NULL;
  CDart * toDelete = alpha2(ADart);
  CDart * prev = ADart;
  
  // Initialisations et décourures / re-coutures
  setDirectInfo( ADart, directInfo, alpha3(alpha21(ADart)) );
  setDirectInfo( alpha0(ADart), directInfo, alpha3(alpha201(ADart)) );

  unsew1( toDelete );
  unsew1( alpha0(toDelete) );

  unsew2( ADart );
  unsew2( d2 );

  sew2( ADart, d2 );

  // Boucle sur tout les brins à traiter.
  while ( d1!=alpha0(ADart) )
    {
      d2 = alpha2( d1 );

      n1 = addMapDart(); n2 = addMapDart();
      setMark(n1, AFictiveFaceMark); setMark(n2, AFictiveFaceMark);
      
      linkAlpha3( n1, n2 );

      linkAlpha2( d1, n1 ); linkAlpha2( d2, n2 );

      setDirectInfo( d1, directInfo, n1 );

      if ( nextAlpha==0 )
	{
	  linkAlpha1( n1, (CDart*)getDirectInfo(prev, directInfo) );
	  linkAlpha1( n2, alpha3((CDart*)getDirectInfo(prev, directInfo)) );
	}
      else
	{
	  assert ( nextAlpha==1 );
	  linkAlpha0( n1, (CDart*)getDirectInfo(prev, directInfo) );
	  linkAlpha0( n2, alpha3((CDart*)getDirectInfo(prev, directInfo)) );
	}
      
      // Passage au prochain brin
      prev = d1;
      d1 = alpha(d1, nextAlpha);
      /* En sautant les arêtes fictives et les arêtes pendantes ???
	if ( d1!=alpha0(ADart) && nextAlpha==1 )
	while ( d1!=alpha0(ADart) &&
		(isMarked(d1, AFictiveEdgeMark) || alpha01(d1)==alpha20(d1)) )
	  d1 = alpha21(d1);
      */
      nextAlpha = ( nextAlpha==0 ? 1 : 0 );
    }

  assert ( nextAlpha==0 );
  linkAlpha1( n1, (CDart*)getDirectInfo(d1, directInfo) );
  linkAlpha1( n2, alpha3((CDart*)getDirectInfo(d1, directInfo)) );

  // Suppression des brins inutiles.
  setMark(alpha03(toDelete), ADeleteMark);
  setMark(alpha3(toDelete), ADeleteMark);
  setMark(alpha0(toDelete), ADeleteMark);
  setMark(toDelete, ADeleteMark);
  //  delMapDart(alpha03(toDelete)); delMapDart(alpha3(toDelete));
  //  delMapDart(alpha0(toDelete)); delMapDart(toDelete);

  freeDirectInfo(directInfo);
}
//******************************************************************************
bool CGMapGeneric::canShiftOneFictiveFace(CDart* ADart, int AFictiveFaceMark)
{
  assert( ADart!=NULL );
  assert( !isFree1(ADart) );
  assert( !isFree2(ADart) );
  assert( !isFree3(ADart) );
  assert( !isFree2(alpha3(ADart)) );
  assert( isMarked(ADart, AFictiveFaceMark) );

  int currentFace = getNewMark();
  bool res = true;
  CDart * d1 = alpha1(ADart);
  bool edgeMarked, vertexMarked;
  
  // On marque la face courante
  markOrbit( ADart, ORBIT_FACE, currentFace);

  while ( res && d1!=alpha0(ADart) )
    {
      edgeMarked = false;
      vertexMarked = false;
      
      for (CDynamicCoverageEdge it(this, d1); !edgeMarked && it.cont(); ++it)
	edgeMarked = isMarked(*it, currentFace);

      for (CDynamicCoverageEdge it(this, alpha1(d1));
	   !edgeMarked && it.cont(); ++it)
	edgeMarked = isMarked(*it, currentFace);

      if ( !edgeMarked )
	{
	  for (CDynamicCoverageVertex it(this, d1); !vertexMarked && it.cont();
	       ++it)
	    vertexMarked = isMarked(*it, currentFace);

	  if ( vertexMarked ) res = false;
	}
      
      d1 = alpha01(d1);
    }
  
  // On démarque la face courante et on libère les marques
  unmarkOrbit( ADart, ORBIT_FACE, currentFace);
  freeMark(currentFace);

  return res;
}
//******************************************************************************
void CGMapGeneric::shiftAllFictiveFaces(CDart* ADart, int AFictiveFaceMark,
					int ADeleteMark)
{
  assert( !isMarked(ADart, AFictiveFaceMark) );
  assert( !isFree2(ADart) && isFree3(ADart) );
  
  while ( isMarked(alpha2(ADart), AFictiveFaceMark) )
    {
      shiftOneFictiveFace(ADart, AFictiveFaceMark, ADeleteMark);
    }
}
//******************************************************************************
bool CGMapGeneric::canShiftAllFictiveFaces(CDart* ADart, int AFictiveFaceMark)
{
  assert( !isMarked(ADart, AFictiveFaceMark) );
  assert( !isFree2(ADart) && isFree3(ADart) );

  bool res = true;
  CDart* current = alpha2(ADart);
  
  while ( res && isMarked(current, AFictiveFaceMark) )
    {
      res = canShiftOneFictiveFace(current, AFictiveFaceMark);
      current = alpha32(current);
    }
  return res;
}
//******************************************************************************
int CGMapGeneric::findEdgeType( CDart* ADart, CDart** AFirstDart,
				int AFictiveFaceMark )
{
  assert( ADart!=NULL );

  int  markEdge = getNewMark();
  int  markFace = getNewMark();
  int  res	= 0;
  int  nbReel	= 0; // Pour compter le nombre d'arête réelle non boucle
  bool reelDangling	= false; 

  CDynamicCoverageEdge it(this, ADart);
  for ( ; it.cont(); ++it )
    setMark(*it, markEdge);
 
  // On parcours l'arête. On s'arrête si on a trouvé et une face réelle
  // et une face non boucle, ou plus de 2 faces réelles.
  for ( it.reinit(); it.cont() && nbReel<=2 && (!reelDangling || nbReel==1);
	++it )
    {
      if ( !isMarked(*it, markFace) )
	{
	  // Si on n'est pas sur une face fictive
	  if ( !isMarked(*it, AFictiveFaceMark) ) 
	    {
	      if ( isMarked(alpha1(*it), markEdge) ||
		   isMarked(alpha01(*it), markEdge) )
		{
		  *AFirstDart = *it;
		  reelDangling = true; // Ici la face de degré 1 est incidente
		                       // à une arête pendante.
		}
	      else
		{
		  if ( nbReel==0 )
		    *AFirstDart = *it; // Cette face est réelle et non boucle
		}
	      ++nbReel; // On compte le nombre de faces réelles
	      markOrbit(*it, ORBIT_FACE, markFace);
	    }
	}
    }

  if ( reelDangling ) // On a trouvé une boucle réelle
    {
      if ( nbReel==1 ) res = 1; // C'est la seule face réelle
      else res = 3; // Il y a d'autres faces réelle incidentes à l'arête
    }
  else
    {
      if ( nbReel==2 )
	res = 2; // On a exactement 2 faces réelles sans arête pendante
      else if ( nbReel!=0 ) res = 3; // cas ==1 ou >2
    }
	  
  // On refait le parcours du sommet pour enlever les marques
  for ( it.reinit(); it.cont(); ++it )
    {
      if ( isMarked(*it, markFace) )
	unmarkOrbit(*it, ORBIT_FACE, markFace);

      unsetMark(*it, markEdge);
    }

  // assert(isWholeMapUnmarked(markEdge));
  // assert(isWholeMapUnmarked(markFace));
  
  freeMark(markEdge);
  freeMark(markFace);

  return res;  
}
//******************************************************************************
