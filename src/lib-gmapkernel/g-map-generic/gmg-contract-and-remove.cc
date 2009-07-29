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
// Contractions et suppressions simultanées de cellules de dimension quelconque.
// Pour chaque cellule concernée, un marquage sur un brin incident à celle-ci
// indique l'opération à affectuer.
//
// dim == 3: contraction de volume
//           (précondition : volume de degré inférieur local égal à 2)
// dim == 2: contraction ou suppression de face
//           (pas de précondition pour la suppression)
//           (précondition contraction : face de degré inférieur local égal à 2)
// dim == 1: contraction ou suppression d'arête
//           (pas de précondition pour la contraction)
//           (précondition suppression: arête de degré inférieur local égal à 2)
// dim == 0: suppression de sommet
//           (précondition : sommet de degré inférieur local égal à 2)
//******************************************************************************
int CGMapGeneric::contractAndRemoveMarkedCells( int AMarkNumberC1,
					        int AMarkNumberC2, 
					        int AMarkNumberC3,
					        int AMarkNumberS0,
					        int AMarkNumberS1, 
					        int AMarkNumberS2,
					        bool ADeleteDarts )
{

  int marks[6]; // contiendra les 6 marques passées en paramètre
  //   - La dimension de la cellule concernée par l'opération
  //      de marque rangée dans la case i du tableau est (i+1)%4.

  int nbCellMarked = 0; // Indique le nombre cellules supprimées et contractées

  marks[0] = AMarkNumberC1;
  marks[1] = AMarkNumberC2;
  marks[2] = AMarkNumberC3;
  marks[3] = AMarkNumberS0;
  marks[4] = AMarkNumberS1;
  marks[5] = AMarkNumberS2;

  // On marque les brins de chaque cellule concernée par l'une des opérations
  for(int i=0 ; i<=5; i++)
    {
      if (marks[i]!=-1)
	{
	  nbCellMarked += markIncidentCells(ORBIT_CELL[(i+1)%4], marks[i]);
	}
    }

  // On démarque les cellules qui ne respectent pas les préconditions de la suppression
  // ou de la contraction
  CDynamicCoverageAll cov(this);
  for ( ; cov.cont(); ++cov )
    {
      for(int i=0; i<=2; i++)
	{
	  if (marks[i]!=-1)
	    {
	      if ( isMarked(*cov, marks[i]) )
		{
		  int cell = i+1;
		  if ( !canContract(*cov, cell) )
		    {
		      unmarkOrbit( *cov, ORBIT_CELL[cell], marks[i]);
		      nbCellMarked--;
		    }
		}
	    }
	}
      for(int i=3; i<=5; i++)
	{
	  if (marks[i]!= -1)
	    {
	      if ( isMarked(*cov, marks[i]) )
		{
		  int cell = (i+1)%4;
		  if ( !canRemove(*cov, cell) )
		    {
		      unmarkOrbit( *cov, ORBIT_CELL[cell], marks[i]);
		      nbCellMarked--;
		    }
		}
	    }
	}
    }

  int firstMark; // La première marque rencontrée.
  int nbMark;
  int cell;
  
  // On démarque les cellules non disjointes
  for ( cov.reinit(); cov.cont(); ++cov )
    {
      firstMark = -1;
      nbMark = 0;
      
      for(int i=0; i<=5; i++)
	{
	  if( marks[i] != -1 )
	    if ( isMarked(*cov, marks[i]) )
	      {
		++nbMark;
		
		if ( firstMark==-1 ) firstMark = i;
		else
		  {
		    cell = (i+1)%4;
		    unmarkOrbit( *cov, ORBIT_CELL[cell], marks[i]);
		    nbCellMarked--;
		  }
	      }
	}
      
      if ( nbMark>1 )
	{
	  cell = (firstMark+1)%4;
	  unmarkOrbit( *cov, ORBIT_CELL[cell], marks[firstMark]);
	}
    }

  // et on contracte ou supprime ce qui reste marqué
  CDart* current = NULL;
  CDart* t2 = NULL;

  for ( cov.reinit(); cov.cont(); ++cov )
    {
      for(int i=0; i<=1; i++)
	{
	  int cell = (i+1)%4;
	  if (marks[i] != -1)
	    {
	      if (  !(isMarked( *cov, marks[i]) || 
		      isMarked( *cov, marks[i+4]) )  &&
		    (isMarked( alpha(*cov, cell), marks[i]) || 
		     isMarked( alpha(*cov, cell), marks[i+4]))  )
		{
		  current = *cov;
		  t2 = alpha(current, cell);
		  
		  while (isMarked(t2, marks[i]) || isMarked(t2, marks[i+4]) )
		    {
		      if (isMarked(t2, marks[i]))
			  t2 = alpha(alpha(t2, cell-1), cell);
		      else 
			  t2 = alpha(alpha(t2, cell+1), cell);
		    }
		  
		  if ( t2 != alpha(current, cell) )
		    {
		      unsew(current, cell);
		      if ( !isFree(t2, cell) ) unsew(t2, cell);
		      if ( t2!=current ) sew(current, t2, cell);
		    }
		}
	    }
	}
      if (AMarkNumberC3 != -1)
	{
	  if (  !isMarked( *cov, AMarkNumberC3) &&
		isMarked( alpha(*cov, 3), AMarkNumberC3) )
	    {
	      current = *cov;
	      t2 = alpha(current, 3);
	      
	      while (isMarked(t2, AMarkNumberC3))
		{
		  t2 = alpha(alpha(t2, 2), 3);
		}
	      
	      if ( t2 != alpha(current, 3) )
		{
		  unsew(current, 3);
		  if ( !isFree(t2, 3) ) unsew(t2, 3);
		  if ( t2!=current ) sew(current, t2, 3);
		}
	    }
	}
      if (AMarkNumberS0 != -1)
	{
	  if (  !isMarked( *cov, AMarkNumberS0) &&
		isMarked( alpha(*cov, 0), AMarkNumberS0) )
	    {
	      current = *cov;
	      t2 = alpha(current, 0);
	      
	      while (isMarked(t2, AMarkNumberS0))
		{
		  t2 = alpha(alpha(t2, 1), 0);
		}
	      
	      if ( t2 != alpha(current, 0) )
		{
		  unsew(current, 0);
		  if ( !isFree(t2, 0) ) unsew(t2, 0);
		  if ( t2!=current ) sew(current, t2, 0);
		}
	    }
	}
    }
  
  // on netoie si demandé
  if (ADeleteDarts)
    {
      for (cov.reinit(); cov.cont(); )
	{   
	  for (int i=0; i<=5; i++)
	    {
	      if(marks[i] != -1)
		{
		  if ( isMarked(*cov, marks[i]) )
		    {		
		      delMapDart(*cov);
		      i=5;
		    }
		}
	      if (i==5) ++cov;
	    }
	}
    }
  return nbCellMarked;
}
