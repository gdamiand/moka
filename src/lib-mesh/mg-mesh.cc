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
#include "mesh-generic.hh"
#include "g-map-generic.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
bool CMeshGeneric::canMesh1(CDart* ADart)
{
  assert(ADart != NULL);
  return isTopoEdge(ADart);
}
//------------------------------------------------------------------------------
void CMeshGeneric::mesh1(CDart* ADart, int ASx,
			 const CTransformationMatrix* /*AMeshMatrix*/,
			 bool AMeshWithMerges,
			 bool AMeshWithAdjacentSews,
			 bool AMeshAssociatedCells,
			 int /*ADirectInfoAlpha0*/, int ADirectInfoAssoc)
{
  assert(ADart!=NULL);
  assert(canMesh1(ADart));
  assert(ASx>0);

  CDart* extremities[2];

  if (AMeshWithMerges)
    {
      FMap->createTopoMesh1(ASx, extremities, ADart);
      mergeEdgeAndMesh1(ADart, extremities);
    }
  else
    {
      assert(ADirectInfoAssoc >= 0);

      if (AMeshWithAdjacentSews)
	{
	  CDart* assoc = FMap->getDirectInfoAsDart(ADart, ADirectInfoAssoc);
	  FMap->createTopoMesh1(ASx, extremities, assoc);
	  mergeEdgeAndMesh1(assoc, extremities);
	}
      else
	{
	  if (AMeshAssociatedCells)
	    {
	      CDart* assoc = FMap->getDirectInfoAsDart(ADart, ADirectInfoAssoc);

	      FMap->createTopoMesh1(ASx, extremities, assoc);
	      mergeEdgeAndMesh1(assoc, extremities);
	    }
	  else
	    {
	      FMap->createTopoMesh1(ASx, extremities, NULL);
	      FMap->setDirectInfo(   ADart , ADirectInfoAssoc, extremities[0]);
	      FMap->setDirectInfo(FMap->alpha0(ADart), ADirectInfoAssoc, extremities[1]);
	    }
	}
    }
}
//******************************************************************************
bool CMeshGeneric::canMesh2(CDart* ADart,
			    int ASx, int ASy, int AInitialMeshDim)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(0<=AInitialMeshDim && AInitialMeshDim<=1);

  return isTopoSquareIMeshed(AInitialMeshDim, ADart, ASx,ASy,
			     true, ! FMap->isFree3(ADart));
}
//------------------------------------------------------------------------------
void CMeshGeneric::mesh2(CDart* ADart,
			 int ASx, int ASy,
			 const CTransformationMatrix * AMeshMatrix,
			 bool AMeshWithMerges,
			 bool AMeshWithAdjacentSews,
			 bool AMeshAssociatedCells,
			 bool /*AGetAssociatedEmbeddings*/,
			 int AInitialMeshDim, int AFinalMeshDim,
			 int ADirectInfoAlpha0, int ADirectInfoAssoc)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(0<=AInitialMeshDim && AInitialMeshDim<=1);
  assert(1<=AFinalMeshDim   && AFinalMeshDim  <=2);
  assert(AInitialMeshDim < AFinalMeshDim);

  assert(canMesh2(AMeshAssociatedCells
		  ? FMap->getDirectInfoAsDart(ADart, ADirectInfoAssoc) : ADart,
		  ASx,ASy, AInitialMeshDim));

  bool isolated = !AMeshWithMerges && !AMeshWithAdjacentSews;

  // 1-maillage:
  if (AInitialMeshDim==0 && (AFinalMeshDim==1 || !isolated))
    {
      CDart* dart1 = ADart;
      CDart* dart2 = FMap->alpha0101(ADart);

      mesh1( dart1, ASx, AMeshMatrix, AMeshWithMerges, AMeshWithAdjacentSews,
	     AMeshAssociatedCells, ADirectInfoAlpha0, ADirectInfoAssoc);
      mesh1( FMap->alpha1(dart1), ASy, AMeshMatrix, AMeshWithMerges, AMeshWithAdjacentSews,
	     AMeshAssociatedCells, ADirectInfoAlpha0, ADirectInfoAssoc);
      mesh1( dart2, ASx, AMeshMatrix, AMeshWithMerges, AMeshWithAdjacentSews,
	     AMeshAssociatedCells, ADirectInfoAlpha0, ADirectInfoAssoc);
      mesh1( FMap->alpha1(dart2), ASy, AMeshMatrix, AMeshWithMerges, AMeshWithAdjacentSews,
	     AMeshAssociatedCells, ADirectInfoAlpha0, ADirectInfoAssoc);
    }

  // 2-maillage:
  if (AFinalMeshDim==2)
    {
      CDart* first =
	AMeshAssociatedCells
	? FMap->getDirectInfoAsDart(ADart, ADirectInfoAssoc)
	: ADart;

      CDart* mesh = FMap->createTopoMesh2(ASx, ASy, !isolated &&
					  ! FMap->isFree3(first));

      mergeSquare1MeshedAndMesh2(first, mesh, ASx, ASy);
    }
}
//******************************************************************************
bool CMeshGeneric::canMesh3(CDart* ADart,
			    int ASx, int ASy, int ASz, int AInitialMeshDim)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(0<=AInitialMeshDim && AInitialMeshDim<=2);

  return isTopoCubeIMeshed(AInitialMeshDim, ADart, ASx,ASy,ASz);
}
//------------------------------------------------------------------------------
void CMeshGeneric::mesh3(CDart* ADart,
			 int ASx, int ASy, int ASz,
			 const CTransformationMatrix * AMeshMatrix,
			 bool AMeshWithMerges,
			 bool AMeshWithAdjacentSews,
			 bool AMeshAssociatedCells,
			 int AInitialMeshDim, int AFinalMeshDim,
			 int /*ADirectInfoAlpha0*/, int ADirectInfoAssoc)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(0<=AInitialMeshDim && AInitialMeshDim<=2);
  assert(1<=AFinalMeshDim   && AFinalMeshDim  <=3);
  assert(AInitialMeshDim < AFinalMeshDim);
  assert(canMesh3(ADart, ASx,ASy,ASz, AInitialMeshDim));

  // 1-maillage:
  if (AInitialMeshDim==0)
    {
      CDart* dart1 = ADart;
      CDart* dart2 = FMap->alpha21012(FMap->alpha0101(ADart));

      mesh1( FMap->alpha21 (dart1), ASz, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha21 (dart2), ASz, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha021(dart1), ASz, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha021(dart2), ASz, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha01 (dart1), ASy, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha01 (dart2), ASy, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha101(dart1), ASx, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha101(dart2), ASx, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha1  (dart1), ASy, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( FMap->alpha1  (dart2), ASy, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( dart1 , ASx, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
      mesh1( dart2 , ASx, AMeshMatrix, AMeshWithMerges,
	     AMeshWithAdjacentSews, AMeshAssociatedCells, ADirectInfoAssoc);
    }

  if (AFinalMeshDim>=2)
    {
      // 2-maillage:
      if (AInitialMeshDim<2)
	{
	  CDart* dart1 = ADart;
	  CDart* dart2 = ADart;

	  for (int i=0; i<ASx; ++i)
	    dart2 = FMap->alpha01(dart2);
	
	  for (int j=0; j<ASy; ++j)
	    dart2 = FMap->alpha01(dart2);
	
	  dart2 = FMap->alpha21(dart2);

	  for (int k=0; k<ASz; ++k)
	    dart2 = FMap->alpha01(dart2);

	  mesh2( dart1 , ASx,ASy, AMeshMatrix, AMeshWithMerges,
		 AMeshWithAdjacentSews, AMeshAssociatedCells, false,
		 1,2, ADirectInfoAssoc);
	  mesh2( dart2 , ASx,ASy, AMeshMatrix, AMeshWithMerges,
		 AMeshWithAdjacentSews, AMeshAssociatedCells, false,
		 1,2, ADirectInfoAssoc);
	  mesh2(FMap->alpha2 (dart1), ASx,ASz, AMeshMatrix, AMeshWithMerges,
		AMeshWithAdjacentSews, AMeshAssociatedCells, false,
		1,2, ADirectInfoAssoc);
	  mesh2(FMap->alpha2 (dart2), ASx,ASz, AMeshMatrix, AMeshWithMerges,
		AMeshWithAdjacentSews, AMeshAssociatedCells, false,
		1,2, ADirectInfoAssoc);
	  mesh2(FMap->alpha12(dart1), ASy,ASz, AMeshMatrix, AMeshWithMerges,
		AMeshWithAdjacentSews, AMeshAssociatedCells, false,
		1,2, ADirectInfoAssoc);
	  mesh2(FMap->alpha12(dart2), ASy,ASz, AMeshMatrix, AMeshWithMerges,
		AMeshWithAdjacentSews, AMeshAssociatedCells, false,
		1,2, ADirectInfoAssoc);
	}

      // 3-maillage:
      if (AFinalMeshDim==3)
	mergeCube2MeshedAndMesh3(ADart,
				 FMap->createTopoMesh3(ASx, ASy, ASz),
				 ASx, ASy, ASz);
    }
}
//******************************************************************************
