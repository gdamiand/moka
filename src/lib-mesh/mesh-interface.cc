/*
 * lib-mesh : Opérations de maillage et lissage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-mesh
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
#include "mesh-interface.hh"
#include "g-map-vertex.hh"
#include <cstdlib>
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CMesh::CMesh(CGMapVertex* AMap) :
  FMap(AMap),
  FMesh(AMap)
{
  assert(FMap != NULL);
}
//------------------------------------------------------------------------------
CMesh::~CMesh()
{
}
//******************************************************************************
int CMesh::meshMarkedEdges(int AMarkNumber, int ASx,
			   const CTransformationMatrix* AMeshMatrix,
			   bool AMeshWithMerges,
			   bool AMeshWithAdjacentSews,
			   bool AMeshAssociatedCells,
			   int ADirectInfoAlpha0, int ADirectInfoAssoc)
{
  assert(ASx>0);

  CDynamicCoverageAll it(FMap);
  int nbMeshed = 0;
  int treated = FMap->getNewMark();

  int direct0, assoc;
  int toMesh;

  // Initialisation:
  if (ADirectInfoAlpha0>=0 || ADirectInfoAssoc>=0)
    {
      direct0 = ADirectInfoAlpha0;
      assoc = ADirectInfoAssoc;
      toMesh = AMarkNumber;
    }
  else
    {
      if (AMeshWithMerges)
	{
	  direct0 = FMap->getNewDirectInfo();
	  assoc = -1;
	}
      else
	{
	  direct0 = -1;
	  assoc = FMap->getNewDirectInfo();
	}

      toMesh = FMap->getNewMark();

      // Marquage des arêtes qui peuvent être maillées:
      for (it.reinit(); it.cont(); ++it)
	if (!FMap->isMarked(*it, treated))
	  {
	    if (FMap->isMarked(*it, AMarkNumber))
	      {		
		if (FMesh.canMesh1(*it))
		  {
		    for (CDynamicCoverageEdge cov(FMap, *it); cov.cont(); ++cov)
		      {
			if (FMap->isMarked(             *cov , AMarkNumber) ||
			    FMap->isMarked(FMap->alpha0(*cov), AMarkNumber))
			  FMap->markOrbit(*cov, ORBIT_0, toMesh);
			
			FMap->markOrbit(*cov, ORBIT_0, treated);
		      }

		    ++nbMeshed;
		  }
	      }
	    else
	      FMap->setMark(*it, treated);
	  }

      FMap->negateMaskMark(treated);

      if (AMeshWithMerges)
	// Sauvegarde des 0-coutures:
	FMap->saveAlphaInDirectInfo(0, direct0);
      else
	// Duplication des cellules:
	if (AMeshWithAdjacentSews)
	  FMap->duplicateMarkedDarts(toMesh, FMap, assoc, false);
    }

  // Maillage:
  for (it.reinit(); it.cont(); ++it)
    if (!FMap->isMarked(*it, treated))
      {
	if (FMap->isMarked(*it, toMesh))
	  {
	    FMap->markOrbit(*it, ORBIT_EDGE, treated);

	    if (AMeshWithMerges)
	      {
		FMesh.mesh1(*it, ASx, AMeshMatrix, AMeshWithMerges, AMeshWithAdjacentSews,
			    AMeshAssociatedCells, direct0, assoc);

		CDart* current = FMap->alpha0(*it);
		
		for (int i=1; i<ASx; ++i)
		  {
		    FMap->markOrbit(current, ORBIT_23, treated);
		    current = FMap->alpha1(current);
		    FMap->markOrbit(current, ORBIT_23, treated);
		    current = FMap->alpha0(current);
		  }
	      }
	    else
	      {
		if (AMeshWithAdjacentSews)
		  {
		    CDynamicCoverage23 cov(FMap, *it);

		    for (; cov.cont(); ++cov)
		      if (FMap->isMarked(*cov, toMesh))
			FMap->unsetMark(FMap->getDirectInfoAsDart(*cov, assoc), treated);

		    for (cov.reinit(); cov.cont(); ++cov)
		      if (FMap->isMarked(*cov, toMesh))
			{
			  CDart* current =
			    FMap->getDirectInfoAsDart(*cov, assoc);
			
			  if (!FMap->isMarked(current, treated))
			    {
			      FMesh.mesh1(*cov, ASx, AMeshMatrix,
					  AMeshWithMerges,
					  AMeshWithAdjacentSews,
					  AMeshAssociatedCells,
					  direct0, assoc);
			
			      for (int i=0; i<ASx; ++i)
				{
				  FMap->markOrbit(current, ORBIT_23, treated);
				  current = FMap->alpha0(current);
				  FMap->markOrbit(current, ORBIT_23, treated);
				  current = FMap->alpha1(current);
				}
			    }
			}
		  }
		else
		  {
		    FMap->markOrbit(*it, ORBIT_EDGE, treated);

		    FMesh.mesh1(*it, ASx, AMeshMatrix, AMeshWithMerges,
				AMeshWithAdjacentSews, AMeshAssociatedCells,
				direct0, assoc);
			
		    CDart* current = FMap->getDirectInfoAsDart(*it, assoc);

		    for (int i=0; i<ASx; ++i)
		      {
			FMap->markOrbit(current, ORBIT_23, treated);
			current = FMap->alpha0(current);
			FMap->markOrbit(current, ORBIT_23, treated);
			current = FMap->alpha1(current);
		      }
		  }
	      }
	  }
	else
	  FMap->setMark(*it, treated);
      }

  FMap->negateMaskMark(treated);
  FMap->freeMark(treated);

  if (ADirectInfoAlpha0<0 && ADirectInfoAssoc<0)
    {
      if (direct0>=0)
	FMap->freeDirectInfo(direct0);

      if (assoc>=0)
	FMap->freeDirectInfo(assoc);

      FMap->unmarkAll(toMesh);
      FMap->freeMark(toMesh);
    }

  return nbMeshed;
}
//------------------------------------------------------------------------------
int CMesh::meshMarkedSquares(int AMarkNumber, int ASx, int ASy,
			     const CTransformationMatrix* AMeshMatrix,
			     bool AMeshWithMerges,
			     bool AMeshWithAdjacentSews,
			     bool AMeshAssociatedCells,
			     int AInitialMeshDim, int AFinalMeshDim,
			     int ADirectInfoAlpha0, int ADirectInfoAssoc)
{
  assert(ASx>0);
  assert(ASy>0);
  assert(0<=AInitialMeshDim && AInitialMeshDim<=1);
  assert(1<=AFinalMeshDim   && AFinalMeshDim  <=2);
  assert(AInitialMeshDim < AFinalMeshDim);

  CDynamicCoverageAll it(FMap);
  int nbMeshed = 0;
  int treated  = FMap->getNewMark();

  int toMesh2 = AFinalMeshDim==2 ? FMap->getNewMark() : -1;
  int toMeshX = FMap->getNewMark();
  int toMeshY = ASx==ASy ? toMeshX : FMap->getNewMark();

  int direct0, assoc;

  if (ADirectInfoAlpha0>=0 || ADirectInfoAssoc>=0)
    {
      direct0 = ADirectInfoAlpha0;
      assoc = ADirectInfoAssoc;
    }
  else
    {
      if (AMeshWithMerges)
	{
	  direct0 = AInitialMeshDim==0 ? FMap->getNewDirectInfo() : -1;
	  assoc = -1;
	}
      else
	{
	  direct0 = -1;
	  assoc = FMap->getNewDirectInfo();
	}

      // Marquage des faces qui peuvent être maillées:
      for (it.reinit(); it.cont(); ++it)
	if (!FMap->isMarked(*it, treated))
	  {
	    if (FMap->isMarked(*it, AMarkNumber))
	      {
		CDart* current = *it;

		if (FMesh.canMesh2(current, ASx,ASy, AInitialMeshDim))
		  {
		    bool canMesh = true;
		
		    if (AInitialMeshDim==0)
		      {
			if (ASx!=ASy)
			  {
			    canMesh = false;
			
			    for (CDynamicCoverage01 fac(FMap, current);
				 !canMesh && fac.cont(); ++fac)
			      if (FMap->isMarked(*fac, AMarkNumber) &&
				  FMesh.isFreeTopoSquareIMeshed(0, *fac,
								ASx,ASy,
								toMeshY,-1,
								toMeshX,-1))
				{
				  current = *fac;
				  canMesh = true;
				}
			  }
			
			if (canMesh)
			  FMesh.markTopoSquareIMeshed(0, current, ASx,ASy,
						      toMeshX, toMeshY);
		      }
		
		    if (canMesh)
		      {
			++nbMeshed;
			
			if (AFinalMeshDim==2)
			  FMap->setMark(current, toMesh2);
		      }
		  }

		FMap->markOrbit(current, ORBIT_FACE, treated);
	      }
	    else
	      FMap->setMark(*it, treated);
	  }

      FMap->negateMaskMark(treated);

      if (AMeshWithMerges)
	{
	  if (AInitialMeshDim==0)
	    FMap->saveAlphaInDirectInfo(0, direct0);
	}
      else
	{
	  int toDuplicate = FMap->getNewMark();

	  if (AInitialMeshDim==0)
	    {
	      int markedEdges = FMap->getNewMark();
	
	      FMap->markCopy(toMeshX, markedEdges);
	      FMap->markAdd (toMeshY, markedEdges);
	
	      FMap->negateMaskMark(markedEdges);
	      FMap->markIncidentCells(ORBIT_FACE, markedEdges);
	      FMap->negateMaskMark(markedEdges);

	      FMap->markCopy(AMarkNumber, toDuplicate);
	      FMap->markAnd (markedEdges, toDuplicate);
	      FMap->markIncidentCells(ORBIT_FACE, toDuplicate);
	
	      FMap->markAnd(toDuplicate, toMeshX);
	      FMap->markAnd(toDuplicate, toMeshY);

	      FMap->unmarkAll(markedEdges);
	      FMap->freeMark(markedEdges);
	    }
	  else
	    FMap->markIncidentCells(AMeshWithAdjacentSews ? ORBIT_FACE : ORBIT_01,
				    toMesh2, toDuplicate);
	
	  if (AInitialMeshDim!=0 || AMeshWithAdjacentSews)
	    FMap->duplicateMarkedDarts(toDuplicate, FMap, assoc, false,
				       true, true, AMeshWithAdjacentSews, true);

	  FMap->unmarkAll(toDuplicate);
	  FMap->freeMark(toDuplicate);
	}
    }

  FMap->freeMark(treated);

  // 1) Maillage 1d:
  if (AInitialMeshDim==0)
    {
      if (AMeshWithMerges || AMeshWithAdjacentSews)
	{
	  meshMarkedEdges(toMeshX, ASx, AMeshMatrix, AMeshWithMerges,
			  AMeshWithAdjacentSews, AMeshAssociatedCells,
			  direct0, assoc);
	
	  if (ASy!=ASx)
	    meshMarkedEdges(toMeshY, ASy, AMeshMatrix, AMeshWithMerges,
			    AMeshWithAdjacentSews, AMeshAssociatedCells,
			    direct0, assoc);
	}
      else
	{
	  AMeshAssociatedCells = true;

	  int mark = AFinalMeshDim == 2 ? toMesh2 : toMeshX;

	  for (it.reinit(); it.cont(); ++it)
	    if (FMap->isMarked(*it, mark))
	      {
		CDart* square = FMap->duplicateDarts(*it, ORBIT_01, NULL, assoc);
		
		FMap->setDirectInfo(*it, assoc, square);
		
		FMesh.mesh2(*it, ASx, ASy, AMeshMatrix, AMeshWithMerges,
			    AMeshWithAdjacentSews, AMeshAssociatedCells, false,
			    0,1, direct0, assoc);
		
		FMap->unmarkOrbit(*it, ORBIT_FACE, toMeshX);
		FMap->unmarkOrbit(*it, ORBIT_FACE, toMeshY);
	      }
	
	  AMeshAssociatedCells = false;
	}

      FMap->unmarkAll(toMeshX);
      FMap->freeMark(toMeshX);

      if (toMeshY != toMeshX)
	{
	  FMap->unmarkAll(toMeshY);
	  FMap->freeMark(toMeshY);
	}
    }

  // 2) Maillage 2d:
  if (AFinalMeshDim==2)
    {
      AMeshAssociatedCells = ! AMeshWithMerges;
      bool getAssociatedEmbeddings = AMeshAssociatedCells && AInitialMeshDim!=1;

      for (it.reinit(); it.cont(); ++it)
	if (FMap->isMarked(*it, toMesh2))
	  {
  	    FMesh.mesh2(*it, ASx,ASy, AMeshMatrix, AMeshWithMerges,
			AMeshWithAdjacentSews, AMeshAssociatedCells,
			getAssociatedEmbeddings,
			1,2, direct0, assoc);
	    FMap->unsetMark(*it, toMesh2);
	  }

      AMeshAssociatedCells = false;

      FMap->freeMark(toMesh2);
    }

  if (ADirectInfoAlpha0<0 && direct0>=0)
    FMap->freeDirectInfo(direct0);

  if (ADirectInfoAssoc<0 && assoc>=0)
    FMap->freeDirectInfo(assoc);

  return nbMeshed;
}
//------------------------------------------------------------------------------
int CMesh::meshMarkedCubes(int AMarkNumber, int ASx, int ASy, int ASz,
			   const CTransformationMatrix* AMeshMatrix,
			   bool AMeshWithMerges,
			   bool AMeshWithAdjacentSews,
			   bool AMeshAssociatedCells,
			   int AInitialMeshDim, int AFinalMeshDim,
			   int ADirectInfoAlpha0, int ADirectInfoAssoc)
{
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(0<=AInitialMeshDim && AInitialMeshDim<=2);
  assert(1<=AFinalMeshDim   && AFinalMeshDim  <=3);
  assert(AInitialMeshDim < AFinalMeshDim);

  CDynamicCoverageAll it(FMap);
  int nbMeshed = 0;
  int treated  = FMap->getNewMark();
  int selected = FMap->getNewMark();

  int toMesh3 = AFinalMeshDim==3 ? FMap->getNewMark() : -1;
  int toMeshX = FMap->getNewMark();
  int toMeshY = ASy==ASx ? toMeshX : FMap->getNewMark();
  int toMeshZ = ASz==ASx ? toMeshX : ASz==ASy ? toMeshY : FMap->getNewMark();

  int s[3] = { ASx, ASy, ASz };
  int m[3] = { toMeshX, toMeshY, toMeshZ };

  int direct0, assoc;

  if (ADirectInfoAssoc >= 0)
    {
      direct0 = ADirectInfoAlpha0;
      assoc = ADirectInfoAssoc;
    }
  else
    {
      if (AMeshWithMerges)
	{
	  direct0 = AInitialMeshDim==0 ? FMap->getNewDirectInfo() : -1;
	  assoc = -1;
	}
      else
	{
	  direct0 = -1;
	  assoc = FMap->getNewDirectInfo();
	}

      // 0) Repérage des cubes sélectionnés qui peuvent être maillés:
      for (it.reinit(); it.cont(); ++it)
	if (!FMap->isMarked(*it, treated))
	  {
	    if (FMap->isMarked(*it, AMarkNumber))
	      {
		CDart* current = *it;
		
		if (FMesh.canMesh3(current, ASx,ASy,ASz, AInitialMeshDim))
		  {
		    bool canMesh = true;
		
		    if (AInitialMeshDim<=1)
		      {
			if (ASx!=ASy || ASy!=ASz)
			  {
			    int alt[3][2];
			
			    for (int dim=0; dim<3; ++dim)
			      if (s[(dim+1)%3]==s[dim])
				{
				  alt[dim][0] = m[(dim+2)%3];
				  alt[dim][1] = -1;
				}
			      else
				{
				  alt[dim][0] = m[(dim+1)%3];
				
				  if (s[(dim+2)%3]==s[dim] ||
				      s[(dim+2)%3]==s[(dim+1)%3])
				    alt[dim][1] = -1;
				  else
				    alt[dim][1] = m[(dim+2)%3];
				}
			
			    canMesh =
			      FMesh.isFreeTopoCubeIMeshed(AInitialMeshDim,
							  current,
							  ASx,ASy,ASz,
							  alt[0][0], alt[0][1],
							  alt[1][0], alt[1][1],
							  alt[2][0], alt[2][1]);
			  }
			
			if (canMesh)
			  {
			    FMesh.markTopoCubeIMeshed(AInitialMeshDim, current,
						      ASx,ASy,ASz,
						      toMeshX, toMeshY, toMeshZ);
			    FMap->markOrbit(current, ORBIT_VOLUME, selected);
			  }
		      }

		    if (canMesh)
		      {
			++nbMeshed;
			
			if (AFinalMeshDim==3)
			  FMap->setMark(current, toMesh3);
			
			FMap->markOrbit(current, ORBIT_VOLUME, treated);
		      }
		  }
	      }
	    else
	      FMap->setMark(*it, treated);
	  }

      FMap->negateMaskMark(treated);

      if (AMeshWithMerges)
	{
	  if (AInitialMeshDim==0)
	    FMap->saveAlphaInDirectInfo(0, direct0);
	}
      else
	{
	  int toDuplicate = FMap->getNewMark();

	  if (AInitialMeshDim<=1)
	    {
	      int markedEdges = FMap->getNewMark();
	
	      FMap->markCopy(toMeshX, markedEdges);
	      FMap->markAdd (toMeshY, markedEdges);
	      FMap->markAdd (toMeshZ, markedEdges);
	

	      FMap->markCopy(markedEdges, 1);
	      //if (AInitialMeshDim!=0)
	      // FMap->markIncidentCells(ORBIT_EDGE, markedEdges);

	      FMap->negateMaskMark(markedEdges);
	      FMap->markIncidentCells(ORBIT_VOLUME, markedEdges);
	      FMap->negateMaskMark(markedEdges);
	
	      FMap->markCopy(AMarkNumber, toDuplicate);
	      FMap->markAnd (markedEdges, toDuplicate);
	      FMap->markIncidentCells(ORBIT_VOLUME, toDuplicate);
	
	      FMap->markAnd(toDuplicate, toMeshX);
	      FMap->markAnd(toDuplicate, toMeshY);
	      FMap->markAnd(toDuplicate, toMeshZ);
	
	      FMap->unmarkAll(markedEdges);
	      FMap->freeMark(markedEdges);
	    }
	  else
	    FMap->markIncidentCells(ORBIT_VOLUME, toMesh3, toDuplicate);

	  if (AInitialMeshDim!=0 || AMeshWithAdjacentSews)
	    FMap->duplicateMarkedDarts(toDuplicate, FMap, assoc, false,
				       true, true, true, AMeshWithAdjacentSews);
	
	  FMap->unmarkAll(toDuplicate);
	  FMap->freeMark(toDuplicate);
	}
    }

  // 1) Maillage 1d:
  if (AInitialMeshDim==0)
    {
      if (AMeshWithMerges || AMeshWithAdjacentSews)
	{
	  meshMarkedEdges(toMeshX, ASx, AMeshMatrix, AMeshWithMerges,
			  AMeshWithAdjacentSews, AMeshAssociatedCells,
			  direct0, assoc);

	  if (ASy!=ASx)
	    meshMarkedEdges(toMeshY, ASy, AMeshMatrix, AMeshWithMerges,
			    AMeshWithAdjacentSews, AMeshAssociatedCells,
			    direct0, assoc);

	  if (ASz!=ASx && ASz!=ASy)
	    meshMarkedEdges(toMeshZ, ASz, AMeshMatrix, AMeshWithMerges,
			    AMeshWithAdjacentSews, AMeshAssociatedCells,
			    direct0, assoc);
	}
      else
	{
	  AMeshAssociatedCells = true;

	  int mark = AFinalMeshDim == 3 ? toMesh3 : toMeshX;

	  for (it.reinit(); it.cont(); ++it)
	    if (FMap->isMarked(*it, mark))
	      {
		CDart* cube = FMap->duplicateDarts(*it, ORBIT_012, NULL, assoc);
		
		FMap->setDirectInfo(*it, assoc, cube);
		
		FMesh.mesh3(*it, ASx,ASy,ASz, AMeshMatrix,
			    AMeshWithMerges, AMeshWithAdjacentSews,
			    AMeshAssociatedCells, 0,1, direct0, assoc);
		
		FMap->unmarkOrbit(*it, ORBIT_VOLUME, toMeshX);
		FMap->unmarkOrbit(*it, ORBIT_VOLUME, toMeshY);
		FMap->unmarkOrbit(*it, ORBIT_VOLUME, toMeshZ);
	      }
	
	  AMeshAssociatedCells = false;
	}
    }

  // 2) Maillage 2d:
  if (AInitialMeshDim<=1 && AFinalMeshDim>=2)
    {
      AMeshAssociatedCells = !AMeshWithMerges;

      for (it.reinit(); it.cont(); ++it)
	if (!FMap->isMarked(*it, treated))
	  {
	    if (FMap->isMarked(*it, toMeshX))
	      {
		if (FMap->isMarked(FMap->alpha1(*it), toMeshY))
		  {
		    FMap->markOrbit(*it, ORBIT_FACE, treated);
		    FMesh.mesh2(*it, ASx,ASy, AMeshMatrix, AMeshWithMerges,
				AMeshWithAdjacentSews, AMeshAssociatedCells,
				false, 1,2, direct0, assoc);
		  }
		else if (FMap->isMarked(FMap->alpha1(*it), toMeshZ))
		  {
		    FMap->markOrbit(*it, ORBIT_FACE, treated);
		    FMesh.mesh2(*it, ASx,ASz, AMeshMatrix, AMeshWithMerges,
				AMeshWithAdjacentSews, AMeshAssociatedCells,
				1,2, direct0, assoc);
		  }
	      }
	    else if (FMap->isMarked(*it, toMeshY))
	      {
		if (FMap->isMarked(FMap->alpha1(*it), toMeshZ))
		  {
		    FMap->markOrbit(*it, ORBIT_FACE, treated);
		    FMesh.mesh2(*it, ASy,ASz, AMeshMatrix, AMeshWithMerges,
				AMeshWithAdjacentSews, AMeshAssociatedCells,
				false, 1,2, direct0, assoc);
		  }
	      }
	  }

      AMeshAssociatedCells = false;
    }

  FMap->unmarkAll(treated);
  FMap->freeMark(treated);

  // 3) Maillage 3d:

  if (AFinalMeshDim==3)
    {
      for (it.reinit(); it.cont(); ++it)
	if (FMap->isMarked(*it, toMesh3))
	  {
	    FMesh.mesh3(*it, ASx,ASy,ASz, AMeshMatrix,
			AMeshWithMerges, AMeshWithAdjacentSews,
			AMeshAssociatedCells, 2,3, direct0, assoc);
	    FMap->unsetMark(*it, toMesh3);
	  }

      FMap->freeMark(toMesh3);
    }

  // Libération des marques:
  FMap->unmarkAll(toMeshX);
  FMap->freeMark(toMeshX);

  if (ASy!=ASx)
    {
      FMap->unmarkAll(toMeshY);
      FMap->freeMark(toMeshY);
    }

  if (ASz!=ASx && ASz!=ASy)
    {
      FMap->unmarkAll(toMeshZ);
      FMap->freeMark(toMeshZ);
    }

  FMap->unmarkAll(selected);
  FMap->freeMark(selected);

  if (ADirectInfoAlpha0<0 && direct0>=0)
    FMap->freeDirectInfo(direct0);

  if (ADirectInfoAssoc<0 && assoc>=0)
    FMap->freeDirectInfo(assoc);

  return nbMeshed;
}
//******************************************************************************
