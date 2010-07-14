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
#include "g-map-vertex.hh"

#ifndef _WINDOWS
#include "chrono.hh"
#endif

#include <iostream>
using namespace std;
using namespace GMap3d;
//******************************************************************************
int CGMapVertex::findMotif( CGMapVertex* AMap, unsigned int* ANbMatched )
{
   int index = getNewDirectInfo();

   int markTreated  = getNewMark();
   int markTreated2 = AMap->getNewMark();
   bool matchOne = false;
   bool matchAll = true;

#ifndef _WINDOWS
   CChrono c;
   c.start();
#endif

   unsigned int oneMatching=0;
   unsigned int * ptrOneMatching=NULL;
   
   if ( ANbMatched!=NULL )
     {
       *ANbMatched=0; ptrOneMatching = &oneMatching;
     }
   
   CDynamicCoverageAll it1(this);
   CDynamicCoverageAll it2(AMap);
   
   for ( ; it1.cont(); ++it1 )
     setDirectInfo(*it1, index, NULL);
   
   for ( it1.reinit(); matchAll && it1.cont(); ++it1 )
     {
       if ( getDirectInfo(*it1,index)==NULL )
	 {
	   matchOne = false;
	   for ( it2.reinit(); !matchOne && it2.cont(); ++it2 )
	     {
	       matchOne=findMotifFrom(*it1, markTreated, index,
				      AMap, *it2, markTreated2,
				      ptrOneMatching);

	       unmarkMotifMark(*it1,markTreated,(matchOne?-1:index),
			       AMap,*it2,(matchOne?-1:markTreated2));

	       if ( ANbMatched!=NULL && oneMatching>(*ANbMatched) )
		 (*ANbMatched) = oneMatching;
	       
	       // assert( isWholeMapUnmarked(markTreated) );
	     }

	   if ( !matchOne ) matchAll = false;
	 }
     }

   freeMark(markTreated);

   AMap->unmarkAll(markTreated2);
   AMap->freeMark(markTreated2);

#ifndef _WINDOWS
   c.stop();
   c.display("Temps de recherche du motif");
#endif
   
   if ( !matchAll )
     {
       freeDirectInfo(index);
       return -1;
     }

   return index;
}
//******************************************************************************
unsigned int CGMapVertex::countNumberOfMotifs( CGMapVertex* AMap, 
					       unsigned int* ANbMatched )
{
  unsigned int res = 0;
  int index = getNewDirectInfo();
  
  int markTreated  = getNewMark();
  int markTreated2 = AMap->getNewMark();
  bool match = false;

  unsigned int size = 0;
  unsigned int oneMatching=0;
  unsigned int * ptrOneMatching=NULL;
  if ( ANbMatched!=NULL )
     { *ANbMatched=0; ptrOneMatching = &oneMatching; }

  CDynamicCoverageAll it1(this);
  CDynamicCoverageAll it2(AMap);
  
  for ( ; it1.cont(); ++it1 )
    {
      setDirectInfo(*it1, index, NULL);
      ++size;
    }
  it1.reinit();
  
  int mark = AMap->getNewMark();
  int totest = AMap->getNewMark();
  int ccsize;
  for ( ; it2.cont(); ++it2 )
    {
      if ( !AMap->isMarked(*it2,mark) )
	{
	  ccsize=0;
	  for (CBasicDynamicCoverage0123 it3(AMap,*it2,mark); it3.cont(); ++it3)
	    {
	      AMap->setMark(*it3,mark);
	      ++ccsize;
	    }
	  if ( ccsize>=size )
	    {
	      for (CBasicDynamicCoverage0123 it3(AMap,*it2,totest); it3.cont(); ++it3)
		AMap->setMark(*it3,totest);
	    }
	}
    }
  AMap->negateMaskMark(mark);
  AMap->freeMark(mark);

#ifndef _WINDOWS
  CChrono c; c.start();
#endif

  for ( it2.reinit(); it2.cont(); ++it2 )
    {
      if ( AMap->isMarked(*it2,totest) )
	{
	  match=findMotifFrom(*it1, markTreated, index,
			      AMap, *it2, markTreated2,
			      ptrOneMatching);
	  
	  unmarkMotifMark(*it1,markTreated,index,
			  AMap,*it2,(match?-1:markTreated2));
	  
	  if ( ANbMatched!=NULL && oneMatching>(*ANbMatched) )
	    (*ANbMatched) = oneMatching;
	  
	  if ( match ) ++res;
	  // assert( isWholeMapUnmarked(markTreated) );
	}
    }

#ifndef _WINDOWS
   c.stop();
   c.display("Temps de recherche de tout les motifs");
#endif
   
   freeMark(markTreated);
   AMap->freeMark(markTreated2);

   freeDirectInfo(index);
   AMap->unmarkAll(totest);
   AMap->freeMark(totest);

   return res;
}
//******************************************************************************
void CGMapVertex::unmarkMotifMark(CDart* ADart, int AMark, int AIndex,
				  CGMapVertex* AMap, CDart* ADart2, int AMark2)
{
  if ( !isMarked(ADart, AMark) ) return;
  
  stack<CDart*> toTreat;
  stack<CDart*> toTreat2;

   toTreat.push(ADart);
   toTreat2.push(ADart2);
   
   CDart* current;
   CDart* other;
   int i;
   
   while (!toTreat.empty())
   {
      current = toTreat.top(); toTreat.pop();
      other = toTreat2.top(); toTreat2.pop();

      if ( isMarked(current, AMark) )
      {         
	for ( i=0; i<=3; ++i )
	  if ( isMarked(alpha(current,i), AMark) )
	    {
	      toTreat.push(alpha(current,i));	
	      toTreat2.push(AMap->alpha(other,i));	
	    }
	
	unsetMark(current, AMark);
	if ( AMark2!=-1 ) AMap->unsetMark(other, AMark2);
	if ( AIndex!=-1 ) setDirectInfo(current, AIndex, NULL);
      }
   }
}
//******************************************************************************
bool CGMapVertex::findMotifFrom( CDart* AFromDart, unsigned int AMarkTreated,
				 unsigned int AIndex,
				 CGMapVertex* AMap, CDart* ADestDart,
				 unsigned int AMarkTreated2,
				 unsigned int* ANbMatched )
{
   bool match = true;
   int i = 0;

   if ( ANbMatched!=NULL) *ANbMatched = 0;
   
   // Les 2 piles pour parcourir les 2 cartes en parallèle.
   stack<CDart*> toTreat;
   stack<CDart*> toTreat2;

   toTreat.push(AFromDart);
   toTreat2.push(ADestDart);   
   
   // Le parcours, tant que la pile n'est pas vide.
   CDart* current;
   CDart* other;

   while (match && !toTreat.empty())
   {
      // Le prochain brin.
      current = toTreat.top();  toTreat.pop();
      other   = toTreat2.top(); toTreat2.pop();
      
      if ( !isMarked(current, AMarkTreated) )
	{
	  if ( ANbMatched!=NULL) ++(*ANbMatched);
	  
	  if ( AMap->isMarked(other, AMarkTreated2) )
	    match = false;
	  else
	    {	      
	      // On fixe l'injection.
	      setDirectInfo(current, AIndex, other);
	      
	      setMark(current, AMarkTreated);
	      AMap->setMark(other, AMarkTreated2);
	      
	      // On teste si l'injection est valide avec les voisins.
	      // On sort dès qu'il y a un problème.
	      for ( i=0; match && i<=3; ++i )
		{
		  if ( !isFree(current,i) )
		    {
		      if (isMarked(alpha(current,i), AMarkTreated) )
			{
			  if ( getDirectInfoAsDart(alpha(current,i), AIndex)!=
			       AMap->alpha(other,i) )
			    match = false;
			}
		      else
			{
			  if ( AMap->isMarked(AMap->alpha(other,i),AMarkTreated2) )
			    match = false;
			  else
			    {
			      toTreat.push(alpha(current,i));
			      toTreat2.push(AMap->alpha(other,i));
			    }
			}
		    }
		  else
		    {
		      if (!AMap->isFree(other,i) &&
			  AMap->isMarked(AMap->alpha(other,i),AMarkTreated2) )
			match = false;
		    }
		}
	    }	  
	}
   }

   return match;
}
//******************************************************************************
