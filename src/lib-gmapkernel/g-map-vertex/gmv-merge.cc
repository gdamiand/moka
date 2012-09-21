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
#include <algorithm>
#include "g-map-vertex.hh"
#include "geometry.hh"
using namespace GMap3d;
//******************************************************************************
int CGMapVertex::mergeMarkedColinearEdges(int AMarkNumber, bool ADeleteDarts)
{
  return mergeMarkedAlignedCells(1, AMarkNumber, ADeleteDarts);
}
//******************************************************************************
int CGMapVertex::mergeMarkedCoplanarFaces(int AMarkNumber, bool ADeleteDarts)
{
  return mergeMarkedAlignedCells(2, AMarkNumber, ADeleteDarts);
}
//******************************************************************************
int CGMapVertex::mergeMarkedAlignedCells(int ADim,
					 int AMarkNumber, bool ADeleteDarts)
{
  assert(ADim==1 || ADim==2);

  CDynamicCoverageAll it(this);

  int selected = getNewMark();
  int treated  = getNewMark();

  // Repérage des cellules de dimension ADim-1 sélectionnées incidentes à des
  // cellules de dimension ADim alignées:

  markIncidentCells(ORBIT_CELL[ADim-1], AMarkNumber, selected);

  // Démarquage des cellules qui ne peuvent pas ou ne doivent pas être
  // fusionnées:

  for (it.reinit(); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, selected))
	  {
	    bool possible =
	      !isFree(*it, ADim) && canMerge(*it, alpha(*it,ADim), ADim);

	    if (possible)
	      {
		CDart * side1 = *it;
		CDart * side2 = alpha(*it,ADim);

		CVertex vector1 =
		  ADim==1 ? edgeVector(side1) : faceNormalVector(side1);

		CVertex vector2 =
		  ADim==1 ? edgeVector(side2) : faceNormalVector(side2);

		possible = CGeometry::areColinear(vector1, vector2);
	      }

	    if (!possible)
	      unmarkOrbit(*it, ORBIT_CELL[ADim-1], selected);

	    markOrbit(*it, ORBIT_CELL[ADim-1], treated);
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);

  // Fusion effective:
  int nbMerged = isolateMarkedCells(selected, ADim-1, ADeleteDarts, false);

  freeMark(treated);

  unmarkAll(selected);
  freeMark(selected);

  return nbMerged;
}
//******************************************************************************
unsigned int CGMapVertex::simplify3DObject(int AMark0, int AMark1, int AMark2)
{
  // Simplify a 3D map in its minimal form, without use shifting operations, and
  // by keeping each cell homeomorphic to a ball.
  // This method suppose that each cell is initially homeomorphic to a ball, and
  // that there is no dangling cell.
  // First we remove each degree two face, then each degree two edges, last each
  // degree two vertex.
  // std::cout<<"simplify3DObject(m0,m1,m2)"<<std::endl;
  int  toDelete	  = getNewMark();
  int  treated	  = getNewMark();
  CDart* current  = NULL;
  CDart* t1  = NULL;
  CDart* t2  = NULL;
  std::stack<CDart*> FToTreat;
  bool dangling = false;
  unsigned int nbRemove = 0;

  int toDelete0 = (AMark0==-1?toDelete:AMark0);
  int toDelete1 = (AMark1==-1?toDelete:AMark1);
  int toDelete2 = (AMark2==-1?toDelete:AMark2);
  
  int indexVol  = getNewDirectInfo();
  int indexFace = getNewDirectInfo();
  if ( indexVol!=-1 && indexFace!=-1 )
    initUnionFindTreesFaceVolume(indexFace, indexVol);
  else
  {
    std::cout<<"Not enough directInfo to use union-find trees."<<std::endl;
    return 0;
  }
  
  // 1) We remove faces.
  CDynamicCoverageAll cov(this);
  while ( cov.cont() )
  {
    dangling = false;
    if ( ! FToTreat.empty() )
    {      
      do
      {
        current = FToTreat.top();
        FToTreat.pop();
        if ( !isMarked(current, toDelete2) && isDanglingFace(current) )
          dangling = true;
      }
      while ( !dangling && ! FToTreat.empty() );
    }

    if ( !dangling )
      current = cov++;

    if ( !isMarked(current,toDelete2) &&
         (dangling || !isMarked(current, treated)) )
    {
      if ( !isFree3(current) )
      {
        // We remove dangling faces and degree two faces.
        if ( dangling ||
             findUnionFindTrees(current, indexVol)!=
             findUnionFindTrees(alpha3(current),indexVol) )
        {
          // First we mark the current face.
          CDynamicCoverage01 itFace(this, current);
          for ( ; itFace.cont(); ++itFace )
          {
            setMark( *itFace, treated );
            setMark( alpha3(*itFace), treated );
            setMark( *itFace, toDelete2 );
            setMark(  alpha3(*itFace), toDelete2 );
            }
		  
          // Second, we push in the stack all the neighboors of the current
          // face that become dangling after the removal.
          // Moreover, we make the removal manually instead of calling
          // remove(current, 2, false) for optimisation reasons.
          for ( itFace.reinit(); itFace.cont(); ++itFace )
          {
            if (alpha23(*itFace)==alpha32(*itFace) &&
                !isMarked(alpha2(*itFace), toDelete2) &&
                !isFree3(alpha2(*itFace)) )
            {
              FToTreat.push(alpha2(*itFace));
            }
		  
            // Now we update alpha2
            t1 = alpha(*itFace, 2);
            if ( !isMarked(t1, toDelete2) )
            {
              t2 = *itFace;
              while ( isMarked(t2, toDelete2) )
              {
                t2 = alpha32(t2);
              }
			  
              if ( t2 != alpha(t1, 2) )
              {
                unsew2(t1);
                if (!isFree(t2, 2)) unsew2(t2);
                if (t1!=t2) sew2(t1,t2);
              }
            }
          }
          
          if ( !dangling )
            mergeUnionFindTrees(current, alpha3(current), indexVol);
        }
        else
        {
          CDynamicCoverage01 itFace(this, current);
          for ( ; itFace.cont(); ++itFace )
          {
            setMark( *itFace, treated );
            setMark( alpha3(*itFace), treated );
          }
        }
      }
      else
      {
        CDynamicCoverage01 itFace(this, current);
        for ( ; itFace.cont(); ++itFace )
        {
          setMark( *itFace, treated );
        }
      }
    }
  }
  negateMaskMark(treated);
  // assert( isWholeMapUnmarked(treated) );
  save("after-remove-faces.moka");

  // 2) We remove edges.
  cov.reinit();
  while ( cov.cont() )
  {
    if ( ! FToTreat.empty() )
    {
      current = FToTreat.top();
      FToTreat.pop();
      dangling = true;
    }
    else
    {
      current = cov++;
      dangling = false;
    }

    if ( !isMarked(current, toDelete2) && !isMarked(current, toDelete1) &&
         (dangling || !isMarked(current, treated)) )
    {
      if ( !isFree2(current) )
      {
        // We remove dangling edges and degree two edges.
        if ( (alpha1(current) !=alpha2(current) ||
              alpha01(current)!=alpha02(current)) &&
             alpha23(current)==alpha32(current) &&
             ( dangling ||
               findUnionFindTrees(current, indexFace)!=
               findUnionFindTrees(alpha2(current),indexFace)) )
        {
          // First we mark the current edge.
          CDynamicCoverage02 itEdge(this, current);
          for ( ; itEdge.cont(); ++itEdge )
          {
            setMark( *itEdge, treated );
            setMark( *itEdge, toDelete1 );
            setMark( alpha3(*itEdge), treated );
            setMark( alpha3(*itEdge), toDelete1 );
           }
	      
          // Second, we push in the stack all the neighboors of the current
          // edge that become dangling after the removal.
          // Moreover, we make the removal manually instead of calling
          // remove(current, 1, false) for optimisation reasons.
          for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
          {
            if ( alpha12(*itEdge)==alpha21(*itEdge) &&
                 !isMarked(alpha1(*itEdge), toDelete1) &&
                 !isFree2(alpha1(*itEdge)) )
            {
              FToTreat.push(alpha1(*itEdge));
            }
		  
            // Now we update alpha1
            t1 = alpha(*itEdge, 1);
            if ( !isMarked(t1, toDelete1) )
            {
              t2 = *itEdge;
              while ( isMarked(t2, toDelete1) )
              {
                t2 = alpha21(t2);
              }
			  
              if ( t2 != alpha(t1, 1) )
              {
                unsew1(t1);
                if (!isFree(t2, 1))
                {
                  unsew1(t2);
                }
                if (t1!=t2)
                {
                  sew1(t1, t2);
                }
              }
            }
          }
		  
          if ( !dangling )
            mergeUnionFindTrees(current, alpha2(current), indexFace);
        }
        else
        {
          CDynamicCoverage02 itEdge(this, current);
          for ( ; itEdge.cont(); ++itEdge )
          {
            setMark( *itEdge, treated );
            setMark( alpha3(*itEdge), treated );
          }
        }
      }
      else
      {
        CDynamicCoverage0 itEdge(this, current);
        for ( ; itEdge.cont(); ++itEdge )
        {
          setMark( *itEdge, treated );
          setMark( alpha3(*itEdge), treated );
        }
      }
    }
  }
  negateMaskMark(treated);
  save("after-remove-edges.moka");
  
  // 3) We remove vertices. This is simpler since a vertex can not be dangling.
  cov.reinit();
  while ( cov.cont() )
  {
    current = cov++;
    
    if ( !isMarked(current,treated) && !isMarked(current, toDelete2)
         && !isMarked(current, toDelete1) && !isMarked(current, toDelete0) )
    {
      bool deleteVertex = true;
      CStaticCoverage23 itVertex(this, current);
      for ( ; itVertex.cont(); ++itVertex )
      {
        setMark( *itVertex, treated );
        setMark( alpha1(*itVertex), treated );

        if ( isFree1(*itVertex)                     ||
             alpha1 (*itVertex)==alpha2 (*itVertex) ||
             alpha01(*itVertex)==alpha02(*itVertex) ||
             alpha12(*itVertex)!=alpha21(*itVertex) )
          deleteVertex = false;
      }

      if ( deleteVertex )
      {
        // First we mark the current vertex.
        for ( itVertex.reinit(); itVertex.cont(); ++itVertex )
        {
          setMark( *itVertex, toDelete0 );
          setMark(  alpha1(*itVertex), toDelete0 );
        }
        
        // Second, we make the removal manually instead of calling
        // remove(current, 0, false) for optimisation reasons.
        for ( itVertex.reinit(); itVertex.cont(); ++itVertex )
        {
          t1 = alpha(*itVertex, 0);
          if ( !isMarked(t1, toDelete0) )
          {
            t2 = *itVertex;
            while ( isMarked(t2, toDelete0) )
            {
              t2 = alpha10(t2);
            }
		    
            if ( t2 != alpha(t1, 0) )
            {
              unsew0(t1);
              if (!isFree(t2, 0)) unsew0(t2);
              if (t1!=t2) sew0(t1, t2);
            }
          }
        }
      }
    }
  }
  
  save("after-all-removals.moka");

  // 4) We remove all the darts marked toDelete
  for ( cov.reinit(); cov.cont(); )
  {
    current = cov++;

    unsetMark(current,treated);      
    if ( isMarked(current, toDelete) )
    {
      delMapDart(current);
    }

    if ( isMarked(current, toDelete0) ||
         isMarked(current, toDelete1) ||
         isMarked(current, toDelete2) )
      ++nbRemove;
  }
  
  freeMark(toDelete);
  freeMark(treated);

  freeDirectInfo(indexVol);
  freeDirectInfo(indexFace);

  return nbRemove;
}
//******************************************************************************
unsigned int CGMapVertex::simplify3DObjectRemoval()
{
  // Simplify a 3D map in its minimal form, without use shifting operations,
  // and by keeping each cell homeomorphic to a ball.
  // This method suppose that each cell is initially homeomorphic to a ball,
  // and that there is no dangling cell.
  // First we remove each degree two face, then each degree two edge, last each
  // degree two vertex.
  std::cout<<"simplify3DObjectRemoval()"<<std::endl;
  int  toDelete	  = getNewMark();
  int  treated	  = getNewMark();
  CDart* current  = NULL;
  CDart* t1  = NULL;
  CDart* t2  = NULL;
  std::stack<CDart*> FToTreat;
  bool dangling = false;
  unsigned int nbRemove = 0;

  int indexVol  = getNewDirectInfo();
  int indexFace = getNewDirectInfo();
  if ( indexVol!=-1 && indexFace!=-1 )
    initUnionFindTreesFaceVolume(indexFace, indexVol);
  else
  {
    std::cout<<"Not enough directInfo to use union-find trees."<<std::endl;
    return 0;
  }

  CDart* firstDeleteDart = NULL;
  
  // 1) We remove faces.
  CDynamicCoverageAll cov(this);
  while ( cov.cont() )
  {
    dangling = false;
    if ( ! FToTreat.empty() )
    {      
      do
      {
        current = FToTreat.top();
        FToTreat.pop();
        if ( !isMarked(current, toDelete) && isDanglingFace(current) )
          dangling = true;
      }
      while ( !dangling && ! FToTreat.empty() );
    }

    if ( !dangling )
      current = cov++;

    if ( !isMarked(current,toDelete) &&
         (dangling || !isMarked(current, treated)) )
    {
      if ( !isFree3(current) )
      {
        // We remove dangling faces and degree two faces.
        if ( dangling ||
             findUnionFindTrees(current, indexVol)!=
             findUnionFindTrees(alpha3(current),indexVol) )
        {
          // First we mark the current face.
          CDynamicCoverage01 itFace(this, current);
          for ( ; itFace.cont(); ++itFace )
          {
            setMark( *itFace, treated );
            setMark( alpha3(*itFace), treated );
            setMark( *itFace, toDelete );
            setMark(  alpha3(*itFace), toDelete );
          }

          while ( cov.cont() && isMarked(*cov, treated) )
            ++cov;
          
          // Second we manage vertex attributes, and remove darts
          // from the list of darts.
          for ( itFace.reinit(); itFace.cont(); ++itFace )
          {
            removeDartInList( *itFace );
            removeDartInList( alpha3(*itFace) );
            (*itFace)->setNext(alpha3(*itFace));
            alpha3(*itFace)->setNext(firstDeleteDart);
            firstDeleteDart=*itFace;

            if ( getVertex(*itFace)!=NULL )
            {
              CAttributeVertex * v = removeVertex(*itFace);
              
              if ( !isMarked(alpha2(*itFace), toDelete) )
                setVertex(alpha2(*itFace), v);
              else if (!isMarked(alpha32(*itFace), toDelete) )
                setVertex(alpha32(*itFace), v);
              else if (!isMarked(alpha12(*itFace), toDelete) )
                setVertex(alpha12(*itFace), v);
              else if (!isMarked(alpha312(*itFace), toDelete) )
                setVertex(alpha312(*itFace), v);
              else
                delete v;
            }

            t1=alpha3(*itFace);
            if ( getVertex(t1)!=NULL )
            {
              CAttributeVertex * v = removeVertex(t1);
              
              if ( !isMarked(alpha2(t1), toDelete) )
                setVertex(alpha2(t1), v);
              else if (!isMarked(alpha32(t1), toDelete) )
                setVertex(alpha32(t1), v);
              else if (!isMarked(alpha12(t1), toDelete) )
                setVertex(alpha12(t1), v);
              else if (!isMarked(alpha312(t1), toDelete) )
                setVertex(alpha312(t1), v);
              else
                delete v;
            }
          }
		  
          // Third, we push in the stack all the neighboors of the current
          // face that become dangling after the removal.
          // Moreover, we make the removal manually instead of calling
          // remove(current, 2, false) for optimisation reasons.
          for ( itFace.reinit(); itFace.cont(); ++itFace )
          {
            if (alpha23(*itFace)==alpha32(*itFace) &&
                !isMarked(alpha2(*itFace), toDelete) &&
                !isFree3(alpha2(*itFace)) )
            {
              FToTreat.push(alpha2(*itFace));
            }
		  
            // Now we update alpha2
            t1 = alpha(*itFace, 2);
            if ( !isMarked(t1, toDelete) )
            {
              t2 = *itFace;
              while ( isMarked(t2, toDelete) )
              {
                t2 = alpha32(t2);
              }
			  
              if ( t2 != alpha(t1, 2) )
              {
                unlinkAlpha2(t1);
                if (!isFree(t2, 2)) unlinkAlpha2(t2);
                if (t1!=t2) linkAlpha2(t1,t2);
              }
            }
          }
          
          if ( !dangling )
            mergeUnionFindTrees(current, alpha3(current), indexVol);
        }
        else
        {          
          for ( CDynamicCoverage01 itFace(this, current);
                itFace.cont(); ++itFace )
          {
            setMark( *itFace, treated );
            setMark( alpha3(*itFace), treated );
          }
        }
      }
      else
      {
        for ( CDynamicCoverage01 itFace(this, current);
              itFace.cont(); ++itFace )
        {
          setMark( *itFace, treated );
        }
      }
    }
  }
  negateMaskMark(treated);
  // assert( isWholeMapUnmarked(treated) );
  save("after-remove-faces.moka");
  
  // 2) We remove edges.
  cov.reinit();
  while ( cov.cont() )
  {
    if ( ! FToTreat.empty() )
    {
      current = FToTreat.top();
      FToTreat.pop();
      dangling = true;
    }
    else
    {
      current = cov++;
      dangling = false;
    }

    if ( !isMarked(current, toDelete) &&
         (dangling || !isMarked(current, treated)) )
    {
      if ( !isFree2(current) )
      {
        // We remove dangling edges and degree two edges.
        if ( (alpha1(current) !=alpha2(current) ||
              alpha01(current)!=alpha02(current)) &&
             alpha23(current)==alpha32(current) &&
             ( dangling ||
               findUnionFindTrees(current, indexFace)!=
               findUnionFindTrees(alpha2(current),indexFace)) )
        {
          // First we mark the current edge.
          CDynamicCoverage02 itEdge(this, current);
          for ( ; itEdge.cont(); ++itEdge )
          {
            setMark( *itEdge, treated );
            setMark( *itEdge, toDelete );
            setMark( alpha3(*itEdge), treated );
            setMark( alpha3(*itEdge), toDelete );
          }

          while ( cov.cont() && isMarked(*cov, treated) )
            ++cov;
          
          // Second we manage vertex attributes, and remove darts
          // from the list of darts.
          for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
          {
            removeDartInList( *itEdge );
            if ( !isFree3(*itEdge) )
            {
              removeDartInList( alpha3(*itEdge) );
              (*itEdge)->setNext(alpha3(*itEdge));
              alpha3(*itEdge)->setNext(firstDeleteDart);                          
            }
            else
              (*itEdge)->setNext(firstDeleteDart);
            firstDeleteDart=*itEdge;

            if ( getVertex(*itEdge)!=NULL )
            {
              CAttributeVertex * v = removeVertex(*itEdge);
              
              if ( !isMarked(alpha1(*itEdge), toDelete) )
                setVertex(alpha1(*itEdge), v);
              else if ( !isMarked(alpha21(*itEdge), toDelete) )
                setVertex(alpha21(*itEdge), v);
              else
                delete v;
            }
            
            if ( !isFree3(*itEdge) )
            {
              t1=alpha3(*itEdge);
              if ( getVertex(t1)!=NULL )
              {
                CAttributeVertex * v = removeVertex(t1);
                
                if ( !isMarked(alpha1(t1), toDelete) )
                  setVertex(alpha1(t1), v);
                else if ( !isMarked(alpha21(t1), toDelete) )
                  setVertex(alpha21(t1), v);
                else
                  delete v;
              }              
            }
          }
	      
          // Third, we push in the stack all the neighboors of the current
          // edge that become dangling after the removal.
          // Moreover, we make the removal manually instead of calling
          // remove(current, 1, false) for optimisation reasons.
          for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
          {
            if ( alpha12(*itEdge)==alpha21(*itEdge) &&
                 !isMarked(alpha1(*itEdge), toDelete) &&
                 !isFree2(alpha1(*itEdge)) )
            {
              FToTreat.push(alpha1(*itEdge));
            }
		  
            // Now we update alpha1
            t1 = alpha(*itEdge, 1);
            if ( !isMarked(t1, toDelete) )
            {
              t2 = *itEdge;
              while ( isMarked(t2, toDelete) )
              {
                t2 = alpha21(t2);
              }
			  
              if ( t2 != alpha(t1, 1) )
              {
                unlinkAlpha1(t1);
                if ( !isFree3(t1) ) unlinkAlpha1(alpha3(t1));
                if (!isFree(t2, 1))
                {
                  unlinkAlpha1(t2);
                  if ( !isFree3(t2) ) unlinkAlpha1(alpha3(t2));
                }
                if (t1!=t2)
                {
                  linkAlpha1(t1, t2);
                  if ( !isFree3(t1) ) linkAlpha1(alpha3(t1), alpha3(t2));
                }
              }
            }
          }
		  
          if ( !dangling )
            mergeUnionFindTrees(current, alpha2(current), indexFace);
        }
        else
        {          
          for ( CDynamicCoverage02 itEdge(this, current);
                itEdge.cont(); ++itEdge )
          {
            setMark( *itEdge, treated );
            setMark( alpha3(*itEdge), treated );
          }
        }
      }
      else
      {        
        for ( CDynamicCoverage0 itEdge(this, current);
              itEdge.cont(); ++itEdge )
        {
          setMark( *itEdge, treated );
          setMark( alpha3(*itEdge), treated );
        }
      }
    }
  }
  negateMaskMark(treated);
  // assert( isWholeMapUnmarked(treated) );
  save("after-remove-edges.moka");
   
  // 3) We remove vertices. This is simpler since a vertex can not be dangling.
  cov.reinit();
  while ( cov.cont() )
  {
    current = cov++;
    
    if ( !isMarked(current,treated) )
    {
      bool deleteVertex = true;
      CStaticCoverage23 itVertex(this, current);
      for ( ; itVertex.cont(); ++itVertex )
      {
        setMark( *itVertex, treated );
        setMark( alpha1(*itVertex), treated );

        if ( isFree1(*itVertex)                     ||
             alpha1 (*itVertex)==alpha2 (*itVertex) ||
             alpha01(*itVertex)==alpha02(*itVertex) ||
             alpha12(*itVertex)!=alpha21(*itVertex) )
          deleteVertex = false;
      }

      if ( deleteVertex )
      {
        // First we mark the current vertex todelete.
        for ( itVertex.reinit(); itVertex.cont(); ++itVertex )
        {
          setMark( *itVertex, toDelete );
          setMark(  alpha1(*itVertex), toDelete );
        }

        while ( cov.cont() && isMarked(*cov, treated) )
          ++cov;          
       
        // Second we remove the darts from their list.
        for ( itVertex.reinit(); itVertex.cont(); ++itVertex )
        {
          removeDartInList( *itVertex );
          removeDartInList( alpha1(*itVertex) );
          (*itVertex)->setNext(alpha1(*itVertex));
          alpha1(*itVertex)->setNext(firstDeleteDart);
          firstDeleteDart=*itVertex;
          
          if ( getVertex(*itVertex)!=NULL )
            delVertex(*itVertex);
          else if ( getVertex(alpha1(*itVertex))!=NULL )
            delVertex(alpha1(*itVertex));
        }
        
        // Second, we make the removal manually instead of calling
        // remove(current, 0, false) for optimisation reasons.
        for ( itVertex.reinit(); itVertex.cont(); ++itVertex )
        {
          t1 = alpha(*itVertex, 0);
          if ( !isMarked(t1, toDelete) )
          {
            t2 = *itVertex;
            while ( isMarked(t2, toDelete) )
            {
              t2 = alpha10(t2);
            }
		    
            if ( t2 != alpha(t1, 0) )
            {
              unlinkAlpha0(t1);
              if (!isFree(t2, 0)) unlinkAlpha0(t2);
              if (t1!=t2) linkAlpha0(t1, t2);
            }
          }
        }
      }
    }
  }

  save("after-all-removal.moka");
  
  // 4) We remove all the darts marked toDelete
  negateMaskMark(treated);
  while ( firstDeleteDart!=NULL )
  {
    t1 = firstDeleteDart->getNext();
    delDart(firstDeleteDart);
    firstDeleteDart = t1;
    ++nbRemove;
  }

  assert( isWholeMapUnmarked(toDelete) );
  assert( isWholeMapUnmarked(treated) );
  
  freeMark(toDelete);
  freeMark(treated);

  freeDirectInfo(indexVol);
  freeDirectInfo(indexFace);

  return nbRemove;
}
//******************************************************************************
unsigned int CGMapVertex::simplify3DObjectContraction()
{
  // Simplify a 3D map in its minimal form, without use shifting operations,
  // and by keeping each cell homeomorphic to a ball.
  // This method suppose that each cell is initially homeomorphic to a ball,
  // and that there is no dangling cell.
  // First we contract each codegree two edge, then each codegree two face,
  // last each codegree two volume.
  std::cout<<"simplify3DObjectContraction()"<<std::endl;
  int  toDelete	  = getNewMark();
  int  treated	  = getNewMark();
  CDart* current  = NULL;
  CDart* t1  = NULL;
  CDart* t2  = NULL;
  std::stack<CDart*> FToTreat;
  bool dangling = false;
  unsigned int nbRemove = 0;

  int indexVertex = getNewDirectInfo();
  int indexEdge   = getNewDirectInfo();
  if ( indexVertex!=-1 && indexEdge!=-1 )
    initUnionFindTreesVerticesEdges(indexVertex, indexEdge);
  else
  {
    std::cout<<"Not enough directInfo to use union-find trees."<<std::endl;
    return 0;
  }

  CDart* firstDeleteDart = NULL;

  // 1) We contract edges.
  CDynamicCoverageAll cov(this);
  while ( cov.cont() )
  {
    current = cov++;

    if ( !isMarked(current,toDelete) &&
         /* !isFree0(current) &&
              !isFree1(current) &&
              !isFree2(current) &&
              !isFree1(alpha0(current)) &&
              !isFree1(alpha2(current)) &&
              !isFree1(alpha02(current)) &&
              alpha1(current)!=alpha2(current) &&
              alpha01(current)!=alpha02(current) && */
         !isMarked(current, treated) )
    {      
      // We contract co-degree two edges.
      if ( findUnionFindTrees(current, indexVertex)!=
           findUnionFindTrees(alpha0(current),indexVertex) )
      {
        // First we mark the current edge.
        //CStaticCoverage23 itEdge(this, current);
        CDynamicCoverage23 itEdge(this, current);
        for ( ; itEdge.cont(); ++itEdge)
        {
          assert ( !isFree0(*itEdge) );
          setMark( *itEdge, treated );
          setMark( alpha0(*itEdge), treated );
          setMark( *itEdge, toDelete );
          setMark( alpha0(*itEdge), toDelete );
        }

        while ( cov.cont() && isMarked(*cov, treated) )
          ++cov;

        std::vector<CDart*> vertex;
        for (CDynamicCoverageVertex itvertex(this, current);
             itvertex.cont(); ++itvertex)
        {
          vertex.push_back(*itvertex);
        }
        for (CDynamicCoverageVertex itvertex(this, alpha0(current));
             itvertex.cont(); ++itvertex)
        {
          vertex.push_back(*itvertex);
        }

        std::vector<CDart*> face;
        for (CDynamicCoverageFace itface(this, current);
             itface.cont(); ++itface)
        {
          face.push_back(*itface);
        }

        std::vector<CDart*> volume;
        for (CDynamicCoverageVolume itvol(this, current);
             itvol.cont(); ++itvol)
        {
          volume.push_back(*itvol);
        }

        // We manage attributes before to modify the map; otherwise
        // it is too late.
        // Attribute of the second vertex must be removed.
        // CAttributeVertex* secondvertex = removeVertex(alpha0(current));

        CVertex secondvertex = *findVertex(alpha0(current));

        // Attribute of the first vertex must be placed on a non delete dart
       /* for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
        {
          if ( getVertex(*itEdge)!=NULL )
          {
            CAttributeVertex * v = removeVertex(*itEdge);

            if ( !isMarked(alpha1(*itEdge), toDelete) )
              setVertex(alpha1(*itEdge), v);
            else if (!isMarked(alpha21(*itEdge), toDelete) )
              setVertex(alpha21(*itEdge), v);
            else if (!isMarked(alpha31(*itEdge), toDelete) )
              setVertex(alpha31(*itEdge), v);
            else if (!isMarked(alpha321(*itEdge), toDelete) )
              setVertex(alpha321(*itEdge), v);
            else if (!isMarked(alpha231(*itEdge), toDelete) )
              setVertex(alpha231(*itEdge), v);
            else
            {
              assert(false);
              delete v;
            }
            break; // We can jump out of the for loop as the attribute is on
                   // a safe dart.
          }
        }*/

        std::vector<std::pair<CDart*,CDart*> > sews;
        // Normalement pas la peine std::vector< CDart* > unsews;

        // We push in the stack all the neighboors of the current
        // edge that become co-dangling ??? after the removal.
        // Moreover, we make the removal manually instead of calling
        // contract(current, 1, false) for optimisation reasons.
        //for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
        for ( CDynamicCoverageEdge itEdge(this,current); itEdge.cont(); ++itEdge )
        {
          // Now we update alpha1
          t1 = alpha(*itEdge, 1);
          if ( !isMarked(t1, toDelete) )
          {
            t2 = *itEdge;
            while ( isMarked(t2, toDelete) )
            {
              t2 = alpha01(t2);
            }

            //std::cout<<t1<<"--"<<alpha1(t1)<<"  ";
            //std::cout<<t2<<"--"<<alpha1(t2)<<"  ";
            if ( t2 != alpha(t1, 1) )
            {
              sews.push_back(std::pair<CDart*,CDart*>(t1,alpha1(t1)));
              unsew1(t1); //unlinkAlpha1(t1);
              if (!isFree(t2, 1))
              {
                sews.push_back(std::pair<CDart*,CDart*>(t2,alpha1(t2)));
                unsew1(t2); //unlinkAlpha1(t2);
              }
              if (t1!=t2 && !isMarked(t1, toDelete))
              {
                // Normalement pas la peine unsews.push_back(t1);
                sew1(t1,t2); //linkAlpha1(t1,t2);
                // std::cout<<"link1 "<<t1<<"--"<<t2<<"  ";
              }
            }
          }
        }
        //std::cout<<std::endl;

        // We test if there is a disconnexion or disparition
        std::vector<CDart*>::iterator itcell;
        std::set<CDart*> cellafter;
        bool disconnection = false;
        for (itcell=vertex.begin(); itcell!=vertex.end(); ++itcell)
        {
          if ( !isMarked(*itcell, toDelete) )
          {
            if ( cellafter.empty() )
              for (CDynamicCoverageVertex itcell2(this, *itcell);
                   itcell2.cont(); ++itcell2)
              {
                cellafter.insert(*itcell2);
              }
            else
            {
              if ( cellafter.find(*itcell)==cellafter.end() )
              {
                disconnection = true;
                // std::cout<<"Disconnect vertex\n";
                break;
              }
            }
          }
        }
        vertex.clear();
        if ( cellafter.empty() ) disconnection=true;
        else cellafter.clear();
/*        if ( !disconnection )
        {
          for (itcell=face.begin(); itcell!=face.end(); ++itcell)
          {
            if ( !isMarked(*itcell, toDelete) )
            {
              if ( cellafter.empty() )
                for (CDynamicCoverageFace itcell2(this, *itcell);
                     itcell2.cont(); ++itcell2)
                {
                  cellafter.insert(*itcell2);
                }
              else
              {
                if ( cellafter.find(*itcell)==cellafter.end() )
                {
                  disconnection = true;
                  //std::cout<<"Disconnect face\n";
                  break;
                }
              }
            }
          }
        }
        face.clear();
        if ( cellafter.empty() ) disconnection=true;
        else cellafter.clear();
        if ( !disconnection )
        {
          for (itcell=volume.begin(); itcell!=volume.end(); ++itcell)
          {
            if ( !isMarked(*itcell, toDelete) )
            {
              if ( cellafter.empty() )
                for (CDynamicCoverageVolume itcell2(this, *itcell);
                     itcell2.cont(); ++itcell2)
                {
                  cellafter.insert(*itcell2);
                }
              else
              {
                if ( cellafter.find(*itcell)==cellafter.end() )
                {
                  disconnection = true;
                  // std::cout<<"Disconnect volume\n";
                  break;
                }
              }
            }
          }
        }
        volume.clear();
        if ( cellafter.empty() ) disconnection=true;
        else cellafter.clear();
*/
        if ( !disconnection )
        {
          assert( findUnionFindTrees(current, indexVertex)!=
              findUnionFindTrees(alpha0(current),indexVertex) );

          // We remove darts from the list of darts.
          for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
          {
            removeDartInList( *itEdge );
            removeDartInList( alpha0(*itEdge) );
            (*itEdge)->setNext(alpha0(*itEdge));
            alpha0(*itEdge)->setNext(firstDeleteDart);
            firstDeleteDart=*itEdge;

            //assert( getVertex(*itEdge)==NULL );
            //assert( getVertex(alpha0(*itEdge))==NULL );
          }

          if ( !dangling )
            mergeUnionFindTrees(current, alpha0(current), indexVertex);
        }
        else
        {
          //std::cout<<"Disconnection: we reput the alpha1.\n";
          for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
          {
            unsetMark( *itEdge, toDelete );
            unsetMark( alpha0(*itEdge), toDelete );
          }
          /* Normalement pas la peine!! std::vector<CDart*>::iterator unsewsit;
          for ( unsewsit=unsews.begin(); unsewsit!=unsews.end();
                ++unsewsit )
            unlinkAlpha1(*unsewsit);*/

          std::vector<std::pair<CDart*,CDart*> >::iterator sewsit;
          for ( sewsit=sews.begin(); sewsit!=sews.end(); ++sewsit )
          {
            //std::cout<<(*sewsit).first<<"--"<<(*sewsit).second<<"  ";
            if ( alpha1((*sewsit).first)!=((*sewsit).second) )
            {
              if ( !isFree1((*sewsit).first) ) unsew1((*sewsit).first);
              if ( !isFree1((*sewsit).second) ) unsew1((*sewsit).second);
              /*linkAlpha1*/sew1((*sewsit).first, (*sewsit).second);
            }
          }
          //std::cout<<std::endl;

          // And we reput the vertex attribute
          // setVertex(alpha0(current), secondvertex);
          updateVertex(alpha0(current), secondvertex);
        }

        sews.clear();
 /*       assert(checkTopology());
        assert(checkEmbeddings(ORBIT_VERTEX, ATTRIBUTE_VERTEX, true));
        for( CDynamicCoverageAll cov2(this); cov2.cont(); ++cov2 )
        {
          assert( !isMarked(*cov2, toDelete) );
          assert( !isMarked(alpha0(*cov2), toDelete) );
          assert( !isMarked(alpha1(*cov2), toDelete) );
          assert( !isMarked(alpha2(*cov2), toDelete) );
          assert( !isMarked(alpha3(*cov2), toDelete) );
        }
        save("in-contract-edges.moka");*/
      }
      else
      {
        for ( CDynamicCoverage23 itEdge(this, current);
              itEdge.cont(); ++itEdge )
        {
          setMark( *itEdge, treated );
          setMark( alpha0(*itEdge), treated );
        }
      }
    }
    else
    {
      for ( CDynamicCoverage23 itEdge(this, current);
            itEdge.cont(); ++itEdge )
      {
        setMark( *itEdge, treated );
        setMark( alpha0(*itEdge), treated );
      }
    }
  }
  negateMaskMark(treated);
  //assert( isWholeMapUnmarked(treated) );

  save("after-contract-edges.moka");

  for( cov.reinit(); cov.cont(); ++cov )
  {
    assert( !isMarked(*cov, toDelete) );
    assert( !isMarked(alpha0(*cov), toDelete) );
    assert( !isMarked(alpha1(*cov), toDelete) );
    assert( !isMarked(alpha2(*cov), toDelete) );
    assert( !isMarked(alpha3(*cov), toDelete) );
  }

  // 4) We remove all the darts marked toDelete
  while ( firstDeleteDart!=NULL )
  {
    assert( isMarked(firstDeleteDart, toDelete) );
    t1 = firstDeleteDart->getNext();
    delDart(firstDeleteDart);
    firstDeleteDart = t1;
    ++nbRemove;
  }

  assert(checkTopology());
  assert(checkEmbeddings(ORBIT_VERTEX, ATTRIBUTE_VERTEX, true));


  // 2) We contract faces.
  cov.reinit();
  while ( false ) //cov.cont() )
  {
    /*if ( ! FToTreat.empty() ) TODO co-dangling
    {
      current = FToTreat.top();
      FToTreat.pop();
      dangling = true;
    }
    else*/
    {
      current = cov++;
      dangling = false;
    }

    if ( !isMarked(current, toDelete) &&
         (dangling || !isMarked(current, treated)) )
    {
      if ( !isFree1(current) )
      {
        // We contract faces and co-dangling faces.
        if ( (alpha2(current)!=alpha1(current) &&
              alpha32(current)!=alpha31(current) &&
              alpha02(current)!=alpha01(current) &&
              alpha302(current)!=alpha301(current) ) &&
             alpha10(current)!=current &&
             !isFree0(current) && !isFree1(alpha0(current)) &&
             alpha10(current)==alpha01(current) &&
             !isFree2(current) && !isFree2(alpha1(current)) &&
             ( dangling ||
               (findUnionFindTrees(current, indexEdge)!=
               findUnionFindTrees(alpha1(current),indexEdge)) &&
               findUnionFindTrees(current, indexVertex)!=
               findUnionFindTrees(alpha0(current),indexVertex)) )
        {
          // std::cout<<"Contract one face"<<std::endl;
          // First we mark the current face.
          CDynamicCoverage13 itFace(this, current);
          for ( ; itFace.cont(); ++itFace)
          {
            setMark( *itFace, treated );
            setMark( *itFace, toDelete );
            setMark( alpha0(*itFace), treated );
            setMark( alpha0(*itFace), toDelete );
          }

          /*std::vector<CDart*> vertex1, vertex2;

          std::cout<<"v1=";
          for (CDynamicCoverageVertex itvertex(this, current);
               itvertex.cont(); ++itvertex)
          {
            //std::cout<<*itvertex<<"; ";
            vertex1.push_back(*itvertex);
          }
          std::cout<<vertex1.size()<<"\nv2=";
          for (CDynamicCoverageVertex itvertex(this, alpha0(current));
               itvertex.cont(); ++itvertex)
          {
            //std::cout<<*itvertex<<"; ";
            vertex2.push_back(*itvertex);
          }
          std::cout<<vertex2.size()<<std::endl;

          for (CDynamicCoverage01 tmpit(this,current);
               tmpit.cont(); ++tmpit)
          {
            std::cout<<"Remove dart "<<*tmpit<<"  "<<"a2="<<alpha2(*tmpit)<<" "<<getVertex(*tmpit)<<std::endl;
          }
          if ( !isFree3(current) )
          {
            std::cout<<"alpha3: ";
            for (CDynamicCoverage01 tmpit(this,alpha3(current));
                 tmpit.cont(); ++tmpit)
            {
              std::cout<<"Remove dart "<<*tmpit<<"  "<<"a2="<<alpha2(*tmpit)<<" "<<getVertex(*tmpit)<<std::endl;
            }
          }*/

          while ( cov.cont() && isMarked(*cov, treated) )
            ++cov;

          // Second we manage vertex attributes, and remove darts
          // from the list of darts.
          for ( itFace.reinit(); itFace.cont(); ++itFace )
          {
            removeDartInList( *itFace );
            assert ( !isFree0(*itFace) );
            {
              removeDartInList( alpha0(*itFace) );
              (*itFace)->setNext(alpha0(*itFace));
              alpha0(*itFace)->setNext(firstDeleteDart);
            }
            // else (*itFace)->setNext(firstDeleteDart);
            firstDeleteDart=*itFace;

            //std::cout<<"Remove dart "<<*itFace
            //        <<" and "<<alpha0(*itFace)<<std::endl;

            if ( getVertex(*itFace)!=NULL )
            {
              CAttributeVertex * v = removeVertex(*itFace);
              // std::cout<<"remove vertex 1\n";
              if ( !isMarked(alpha2(*itFace), toDelete) )
                setVertex(alpha2(*itFace), v);
              else if ( !isMarked(alpha12(*itFace), toDelete) )
                setVertex(alpha12(*itFace), v);
              else
                delete v;
            }

            assert ( !isFree0(*itFace) );
            {
              t1=alpha0(*itFace);
              if ( getVertex(t1)!=NULL )
              {
                CAttributeVertex * v = removeVertex(t1);
                // std::cout<<"remove vertex 2\n";
                if ( !isMarked(alpha2(t1), toDelete) )
                  setVertex(alpha2(t1), v);
                else if ( !isMarked(alpha12(t1), toDelete) )
                  setVertex(alpha12(t1), v);
                else
                  delete v;
              }
            }
          }

          // Third, we push in the stack all the neighboors of the current
          // face that become dangling after the removal. TODO
          // Moreover, we make the contraction manually instead of calling
          // contract(current, 2, false) for optimisation reasons.
          for ( itFace.reinit(); itFace.cont(); ++itFace )
          {
           /* TODO if ( alpha12(*itEdge)==alpha21(*itEdge) &&
                 !isMarked(alpha1(*itEdge), toDelete) &&
                 !isFree2(alpha1(*itEdge)) )
            {
              FToTreat.push(alpha1(*itEdge));
            } */

            // Now we update alpha2
            t1 = alpha(*itFace, 2);
            if ( !isMarked(t1, toDelete) )
            {
              t2 = *itFace;
              while ( isMarked(t2, toDelete) )
              {
                t2 = alpha12(t2);
              }

              if ( t2 != alpha(t1, 2) )
              {
                unlinkAlpha2(t1);
                if ( !isFree0(t1) ) unlinkAlpha2(alpha0(t1));
                if (!isFree(t2, 2))
                {
                  unlinkAlpha2(t2);
                  if ( !isFree0(t2) ) unlinkAlpha2(alpha0(t2));
                }
                if (t1!=t2)
                {
                  linkAlpha2(t1, t2);
                  if ( !isFree0(t1) ) linkAlpha2(alpha0(t1), alpha0(t2));
                }
              }
            }
          }

          if ( !dangling )
            mergeUnionFindTrees(current, alpha1(current), indexEdge);

  /*        std::cout<<"After contraction: nv1=";
          int nbv1 = 0;
          for (CDynamicCoverageVertex itvertex(this, current);
               itvertex.cont(); ++itvertex)
          {
            ++nbv1;
          }
          std::cout<<nbv1<<"\nv2=";
          int nbv2 = 0;
          for (CDynamicCoverageVertex itvertex(this, alpha0(current));
               itvertex.cont(); ++itvertex)
          {
            ++nbv2;
          }
          std::cout<<nbv2<<std::endl;

          unsigned nbv1survivants=0;
          for (std::vector<CDart*>::iterator itvector=vertex1.begin();
               itvector!=vertex1.end(); ++itvector)
            if ( !isMarked(*itvector, toDelete) ) ++nbv1survivants;
          unsigned nbv2survivants=0;
          for (std::vector<CDart*>::iterator itvector=vertex2.begin();
               itvector!=vertex2.end(); ++itvector)
            if ( !isMarked(*itvector, toDelete) ) ++nbv2survivants;

          std::cout<<"nv1survivants="<<nbv1survivants<<std::endl;
          std::cout<<"nv2survivants="<<nbv2survivants<<std::endl;*/
        }
        else
        {
          for ( CDynamicCoverage13 itFace(this, current);
                itFace.cont(); ++itFace )
          {
            setMark( *itFace, treated );
            setMark( alpha0(*itFace), treated );
          }
        }
      }
      else
      {
        for ( CDynamicCoverage3 itFace(this, current);
              itFace.cont(); ++itFace )
        {
          setMark( *itFace, treated );
          setMark( alpha0(*itFace), treated );
        }
      }
    }
  }
  //negateMaskMark(treated);
  //assert( isWholeMapUnmarked(treated) );

  save("after-contract-faces.moka");

  assert(checkTopology());
  assert(checkEmbeddings(ORBIT_VERTEX, ATTRIBUTE_VERTEX, true));

  // 3) We contract volumes.
  // Not necessary for 3D objects that is already simplified by removal
  // operations. Indeed, we are sure there are exactly one volume which must
  // not be simplified...
 /*  cov.reinit();
  while ( cov.cont() )
  {
    current = cov++;

    if ( !isMarked(current,treated) )
    {
      bool deleteVol= true;
      CStaticCoverage01 itVol(this, current);
      for ( ; itVol.cont(); ++itVol )
      {
        setMark( *itVol, treated );
        setMark( alpha2(*itVol), treated );

        if ( isFree2(*itVol)                     ||
             alpha2 (*itVol)==alpha1 (*itVol) ||
             alpha32(*itVol)==alpha31(*itVol) ||
             alpha21(*itVol)!=alpha12(*itVol) )
          deleteVol = false;
      }

      if ( deleteVol )
      {
        // First we mark the current vol todelete.
        for ( itVol.reinit(); itVol.cont(); ++itVol )
        {
          setMark( *itVol, toDelete );
          setMark(  alpha2(*itVol), toDelete );
        }

        while ( cov.cont() && isMarked(*cov, treated) )
          ++cov;

        // Second we remove the darts from their list.
        for ( itVol.reinit(); itVol.cont(); ++itVol )
        {
          removeDartInList( *itVol );
          removeDartInList( alpha2(*itVol) );
          (*itVol)->setNext(alpha2(*itVol));
          alpha2(*itVol)->setNext(firstDeleteDart);
          firstDeleteDart=*itVol;

          if ( getVertex(*itVol)!=NULL )
          {
            CAttributeVertex * v = removeVertex(*itVol);

            if ( !isMarked(alpha3(*itVol), toDelete) )
              setVertex(alpha3(*itVol), v);
            else if ( !isMarked(alpha23(*itVol), toDelete) )
              setVertex(alpha23(*itVol), v);
            else
              delete v;
          }
          else if ( getVertex(alpha2(*itVol))!=NULL )
          {
            CAttributeVertex * v = removeVertex(alpha2(*itVol));

            if ( !isMarked(alpha3(*itVol), toDelete) )
              setVertex(alpha3(*itVol), v);
            else if ( !isMarked(alpha23(*itVol), toDelete) )
              setVertex(alpha23(*itVol), v);
            else
              delete v;
          }
        }

        // Second, we make the removal manually instead of calling
        // remove(current, 0, false) for optimisation reasons.
        for ( itVol.reinit(); itVol.cont(); ++itVol )
        {
          t1 = alpha(*itVol, 3);
          if ( !isMarked(t1, toDelete) )
          {
            t2 = *itVol;
            while ( isMarked(t2, toDelete) )
            {
              t2 = alpha23(t2);
            }

            if ( t2 != alpha(t1, 3) )
            {
              unlinkAlpha3(t1);
              if (!isFree(t2, 3)) unlinkAlpha3(t2);
              if (t1!=t2) linkAlpha3(t1, t2);
            }
          }
        }
      }
    }
  }
  */
  negateMaskMark(treated);

  // 4) We remove all the darts marked toDelete  
  while ( firstDeleteDart!=NULL )
  {
    t1 = firstDeleteDart->getNext();
    delDart(firstDeleteDart);
    firstDeleteDart = t1;
    ++nbRemove;
  }

  // temp pour debug : comme on parcours pas toute la carte pour contract
  // edges, on n'a plus tout démarqué.
  for (cov.reinit(); cov.cont(); ++cov)
    unsetMark(*cov, treated);

  assert( isWholeMapUnmarked(toDelete) );
  assert( isWholeMapUnmarked(treated) );

  freeMark(toDelete);
  freeMark(treated);

  freeDirectInfo(indexVertex);
  freeDirectInfo(indexEdge);

  save("after-simplification-3D-contraction.moka");
  assert(checkTopology());
  assert(checkEmbeddings(ORBIT_VERTEX, ATTRIBUTE_VERTEX, true));

  return nbRemove;
}
//******************************************************************************
unsigned int CGMapVertex::simplify3DObject()
{
  unsigned int nb=simplify3DObjectRemoval();
  nb+=simplify3DObjectContraction();
  return nb;
//  simplify3DObjectRemoval();
//  simplify3DObjectContraction();
}

//******************************************************************************
