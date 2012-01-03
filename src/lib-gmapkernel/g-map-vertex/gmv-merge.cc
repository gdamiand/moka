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
        if ( isDanglingFace(current) )
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

            if ( getVertex(*itFace)!=NULL )
            {
              CAttributeVertex * v = removeVertex(*itFace);
              
              if ( !isMarked(alpha2(*itFace), toDelete2) )
                setVertex(alpha2(*itFace), v);
              else if (!isMarked(alpha32(*itFace), toDelete2) )
                setVertex(alpha32(*itFace), v);
              else if (!isMarked(alpha12(*itFace), toDelete2) )
                setVertex(alpha12(*itFace), v);
              else if (!isMarked(alpha312(*itFace), toDelete2) )
                setVertex(alpha312(*itFace), v);
              else
                delete v;
            }

            t1=alpha3(*itFace);
            if ( getVertex(t1)!=NULL )
            {
              CAttributeVertex * v = removeVertex(t1);
              
              if ( !isMarked(alpha2(t1), toDelete2) )
                setVertex(alpha2(t1), v);
              else if (!isMarked(alpha32(t1), toDelete2) )
                setVertex(alpha32(t1), v);
              else if (!isMarked(alpha12(t1), toDelete2) )
                setVertex(alpha12(t1), v);
              else if (!isMarked(alpha312(t1), toDelete2) )
                setVertex(alpha312(t1), v);
              else
                delete v;
            }
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
          }

          for ( itEdge.reinit(); itEdge.cont(); ++itEdge )
          {
            if ( getVertex(*itEdge)!=NULL )
            {
              CAttributeVertex * v = removeVertex(*itEdge);
              
              if ( !isMarked(alpha1(*itEdge), toDelete1) )
                setVertex(alpha1(*itEdge), v);
              else if ( !isMarked(alpha21(*itEdge), toDelete1) )
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
                
                if ( !isMarked(alpha1(t1), toDelete1) )
                  setVertex(alpha1(t1), v);
                else if ( !isMarked(alpha21(t1), toDelete1) )
                  setVertex(alpha21(t1), v);
                else
                  delete v;
              }
            }
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
          if ( !isMarked(t1, toDelete0) )
          {
            t2 = *itVertex;
            while ( isMarked(t2, toDelete0) )
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
unsigned int CGMapVertex::simplify3DObject()
{
  // Simplify a 3D map in its minimal form, without use shifting operations, and
  // by keeping each cell homeomorphic to a ball.
  // This method suppose that each cell is initially homeomorphic to a ball, and
  // that there is no dangling cell.
  // First we remove each degree two face, then each degree two edges, last each
  // degree two vertex.
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
  // save("after-remove-faces.moka");
  
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

            if ( !isFree3(*itEdge) )
            {
              setMark( alpha3(*itEdge), treated );
              setMark( alpha3(*itEdge), toDelete );
            }
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
  // save("after-remove-edges.moka");
    
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
