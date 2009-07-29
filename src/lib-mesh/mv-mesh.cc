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
#include "mesh-vertex.hh"
#include "g-map-vertex.hh"
#include "inter-smooth-diver.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
void CMeshVertex::mesh1(CDart * ADart, int ASx,
			const CTransformationMatrix * AMeshMatrix,
			bool AMeshWithMerges,
			bool AMeshWithAdjacentSews,
			bool AMeshAssociatedCells,		   
			int ADirectInfoAlpha0, int ADirectInfoAssoc)
{
  assert(ADart!=NULL);
  assert(canMesh1(ADart));
  assert(ASx>0);
  assert(!AMeshWithMerges || ADirectInfoAlpha0>=0);
  assert( AMeshWithMerges || ADirectInfoAssoc >=0);

  // Récupération des points de contrôle:
  CMesh1Diver* diver;

  if (AMeshMatrix == &COONS || AMeshMatrix == NULL)
    {
      CVertex & V1 = * FMap->findVertex(ADart);
      CVertex & V2 = * FMap->findVertex(FMap->alpha0(ADart));
      diver = new CMesh1LinearDiver(ASx, V1,V2);
    }
  else
    {
      CVertex CP[4];

      get4ControlPoints(ADart, CP, ADirectInfoAlpha0);
      diver = new CMesh1SmoothingDiver(ASx, *AMeshMatrix, CP);
    }

  // Topologie:
  CMeshGeneric::mesh1(ADart, ASx, AMeshMatrix, AMeshWithMerges,
		      AMeshWithAdjacentSews, AMeshAssociatedCells,
		      ADirectInfoAlpha0, ADirectInfoAssoc);

  // Plongements:
  CDart * first = AMeshWithMerges
    ? ADart : FMap->getDirectInfoAsDart(ADart, ADirectInfoAssoc);

  FMap->diveMesh1(first, *diver, CGMapVertex::GMV_CHECK);

  delete diver;
}
//******************************************************************************
void CMeshVertex::mesh2(CDart* ADart,
			int ASx, int ASy,
			const CTransformationMatrix * AMeshMatrix,
			bool AMeshWithMerges,
			bool AMeshWithAdjacentSews,
			bool AMeshAssociatedCells,
			bool AGetAssociatedEmbeddings,
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

  assert(FMap->findVertex(ADart)!=NULL);

  // Topologie:
  CMeshGeneric::mesh2(ADart, ASx,ASy, AMeshMatrix, AMeshWithMerges,
		      AMeshWithAdjacentSews, AMeshAssociatedCells,
		      AGetAssociatedEmbeddings,
		      AInitialMeshDim, AFinalMeshDim,
		      ADirectInfoAlpha0, ADirectInfoAssoc);

  assert(FMap->findVertex(ADart)!=NULL);

  // Plongements:
  if (AFinalMeshDim==2)
    {
      CMesh2Diver* diver;

      CDart* geo =
	AGetAssociatedEmbeddings
	? FMap->getDirectInfoAsDart(ADart, ADirectInfoAssoc)
	: ADart;

      assert(FMap->findVertex(geo)!=NULL);

      if (AMeshMatrix == &COONS || AMeshMatrix == NULL)
	{
	  CDart * current;

	  const CVertex ** bottom = new const CVertex * [ASx+1];
	  const CVertex ** top    = new const CVertex * [ASx+1];
	  const CVertex ** left   = new const CVertex * [ASy+1];
	  const CVertex ** right  = new const CVertex * [ASy+1];

	  bool already2Meshed = AInitialMeshDim!=1 || AMeshWithMerges;

	  current = getMesh1Dive(geo, ASx, bottom, already2Meshed);
	  getMesh1Dive(FMap->alpha1(current), ASy, right, already2Meshed);

	  current = getMesh1Dive(FMap->alpha1(geo), ASy, left, already2Meshed);
	  getMesh1Dive(FMap->alpha1(current), ASx, top, already2Meshed);

	  diver = new CMesh2LinearDiver(ASx,ASy, bottom,top,left,right);

	  delete [] bottom;
	  delete [] top;
	  delete [] left;
	  delete [] right;
	}
      else
	{
	  CVertex** CP = allocVertexArray2(4,4);
	
	  get16ControlPoints(ADart, CP, ADirectInfoAlpha0);

	  diver = new CMesh2SmoothingDiver(ASx,ASy, *AMeshMatrix,
					   (const CVertex **) CP);

	  freeVertexArray2(CP, 4,4);
	}

      CDart * first =
	AMeshAssociatedCells
	? FMap->getDirectInfoAsDart(ADart, ADirectInfoAssoc)
	: ADart;

      FMap->diveMesh2(first, *diver, CGMapVertex::GMV_CHECK);

      delete diver;
    }
}
//******************************************************************************
void CMeshVertex::mesh3(CDart * ADart,
			int ASx, int ASy, int ASz,
			const CTransformationMatrix * AMeshMatrix,
			bool AMeshWithMerges,
			bool AMeshWithAdjacentSews,
			bool AMeshAssociatedCells,
			int AInitialMeshDim, int AFinalMeshDim,
			int /*ADirectInfoAlpha0*/, int ADirectInfoAssoc)
{
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);

  assert(0 <= AInitialMeshDim);
  assert(AInitialMeshDim < AFinalMeshDim);
  assert(AFinalMeshDim <= 3);

  assert(canMesh3(ADart, ASx,ASy,ASz, AInitialMeshDim));

  // Topologie:
  CMeshGeneric::mesh3(ADart, ASx,ASy,ASz, AMeshMatrix, AMeshWithMerges,
		      AMeshWithAdjacentSews, AMeshAssociatedCells,
		      AInitialMeshDim,AFinalMeshDim, ADirectInfoAssoc);

  // Plongements:
  if (AFinalMeshDim==3)
    {
      const CVertex
	*** left   = allocVertexPtrArray2(ASy+1, ASz+1),
	*** right  = allocVertexPtrArray2(ASy+1, ASz+1),
	*** front  = allocVertexPtrArray2(ASz+1, ASx+1),
	*** back   = allocVertexPtrArray2(ASz+1, ASx+1),
	*** bottom = allocVertexPtrArray2(ASx+1, ASy+1),
	*** top    = allocVertexPtrArray2(ASx+1, ASy+1);

      // Récupération des plongements du cube 2-maillé:
      CDart * other;

      other = getMesh2Dive(                ADart , ASx,ASy, bottom, true, 1);
      /* - */ getMesh2Dive(FMap->alpha12  (other), ASy,ASz, right , true   );
      other = getMesh2Dive(FMap->alpha12  (ADart), ASy,ASz, left  , true, 1);
      /* - */ getMesh2Dive(FMap->alpha12  (other), ASz,ASx, back  , true   );
      other = getMesh2Dive(FMap->alpha1212(ADart), ASz,ASx, front , true, 1);
      /* - */ getMesh2Dive(FMap->alpha12  (other), ASx,ASy, top   , true   );

      // Affectation:
      CMesh3LinearDiver diver(ASx,ASy,ASz, left,right,front,back,bottom,top);
      FMap->diveMesh3(ADart, diver, CGMapVertex::GMV_CHECK);

      // Libération:
      freeVertexPtrArray2(left  , ASy+1, ASz+1);
      freeVertexPtrArray2(right , ASy+1, ASz+1);
      freeVertexPtrArray2(front , ASz+1, ASx+1);
      freeVertexPtrArray2(back  , ASz+1, ASx+1);
      freeVertexPtrArray2(bottom, ASx+1, ASy+1);
      freeVertexPtrArray2(top   , ASx+1, ASy+1);
    }
}
//******************************************************************************
