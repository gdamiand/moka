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
bool CMeshGeneric::canMergeEdgeAndMesh1(CDart* AEdgeDart,
					CDart* AMesh1Corners[2])
{
  assert(AEdgeDart!=NULL);
  assert(AMesh1Corners!=NULL);

  return
    ! FMap->isFree0(AMesh1Corners[0]) && ! FMap->isFree0(AMesh1Corners[1]) &&
    ! FMap->isSameOrbit(AMesh1Corners[0], AMesh1Corners[1], ORBIT_23) &&
    isTopoEdge(AEdgeDart, AMesh1Corners[0]) &
    isTopoEdge(AEdgeDart, AMesh1Corners[1]);
}
//------------------------------------------------------------------------------
void CMeshGeneric::mergeEdgeAndMesh1(CDart* AEdgeDart,
				     CDart* AMesh1Extremities[2])
{
  assert(canMergeEdgeAndMesh1(AEdgeDart, AMesh1Extremities));

  CDart* edges[2] = { AEdgeDart, FMap->alpha0(AEdgeDart) };

  // Ouverture:
  FMap->unsew0(AEdgeDart);

  // Fusion:
  for (int i=0; i<2; ++i)
    {
      CDart* mesh = FMap->alpha0(AMesh1Extremities[i]);

      FMap->unsew0(AMesh1Extremities[i]);

      for (CStaticCoverage23 cov(FMap, AMesh1Extremities[i]); cov.cont(); ++cov)
	FMap->delMapDart(*cov);

      FMap->topoSew0(edges[i], mesh);
    }
}
//******************************************************************************
bool CMeshGeneric::canMergeSquare1MeshedAndMesh2(CDart* ASquareDart,
						 CDart* AMesh2Dart,
						 int ASx, int ASy)
{
  assert(ASquareDart!=NULL);
  assert(AMesh2Dart!=NULL);
  assert(ASx>0);
  assert(ASy>0);

  bool is3Sewed = ! FMap->isFree3(ASquareDart);

  return
    isTopoSquare1Meshed(ASquareDart, ASx,ASy, true, is3Sewed) &&
    isTopoSquare2Meshed(AMesh2Dart , ASx,ASy, true, is3Sewed);
}
//------------------------------------------------------------------------------
void CMeshGeneric::mergeSquare1MeshedAndMesh2(CDart * ASquareDart,
					      CDart * AMesh2Dart,
					      int ASx, int ASy)
{
  assert(canMergeSquare1MeshedAndMesh2(ASquareDart, AMesh2Dart, ASx,ASy));

  int nbRounds = FMap->isFree3(ASquareDart) ? 1 : 2;

  CDart * current1 = ASquareDart;
  CDart * current2 = AMesh2Dart;

  while (nbRounds > 0)
    {
      CDart * tmp;

      FMap->unlinkAlpha1(current1);

      for (int i=2*(ASx+ASy); i>0; --i)
	{
	  tmp = FMap->alpha1(current2);
	
	  FMap->unlinkAlpha1(tmp); FMap->linkAlpha1(tmp, current1);
	
	  tmp = current2;
	  current2 = FMap->alpha01(current2);
	
	  FMap->unlinkAlpha1(current2);
	  FMap->delMapDart(FMap->alpha0(tmp));
	  FMap->delMapDart(   tmp );
	
	  tmp = FMap->alpha0(current1);
	  current1 = FMap->alpha1(tmp);
	
	  FMap->unlinkAlpha1(tmp); FMap->linkAlpha1(tmp, current2);
	
	  if (! FMap->isFree2(current2))
	    current2 = FMap->alpha21(current2);
	}

      if (nbRounds-- == 2)
	{
	  current1 = FMap->alpha3(ASquareDart);
	  current2 = FMap->alpha3(AMesh2Dart );
	}
    }
}
//******************************************************************************
bool CMeshGeneric::canMergeSquare2MeshedAndMesh3Side(CDart* ASquareDart,
						     CDart* AMesh3Dart,
						     int ASx, int ASy)
{
  assert(ASquareDart!=NULL);
  assert(AMesh3Dart!=NULL);
  assert(ASx>0);
  assert(ASy>0);

  return
    isTopoSquare2Meshed(ASquareDart, ASx,ASy) &&
    isTopoSquare3Meshed(AMesh3Dart , ASx,ASy);
}
//------------------------------------------------------------------------------
void CMeshGeneric::mergeSquare2MeshedAndMesh3Side(CDart* ASquareDart,
						  CDart* AMesh3Dart,
						  int ASx, int ASy,
						  CDart** ANextSquare,
						  CDart** ANextMesh3Side)
{
  assert(canMergeSquare2MeshedAndMesh3Side(ASquareDart, AMesh3Dart, ASx,ASy));

  CDart * currentX1, * currentX2, * currentY1=NULL, * currentY2=NULL;
  CDart * nextX1   , * nextX2   , * nextY1   , * nextY2   ;
  CDart * tmp1     , * tmp2     ;

  // Brins permettant de se déplacer selon la première dimension:
  currentX1 = ASquareDart;
  currentX2 = AMesh3Dart;

  // Pour chaque colonne:
  for (int i=0; i<ASx; ++i)
    {
      // Mémorisation du début de la colonne suivante:
      nextX1 = FMap->alpha0121  (currentX1);
      nextX2 = FMap->alpha012321(currentX2);

      // Brins permettant de se déplacer selon la deuxième dimension:
      currentY1 = FMap->alpha1(currentX1);
      currentY2 = FMap->alpha1(currentX2);

      // Pour chaque ligne:
      for (int j=0; j<ASy; ++j)
	{
	  // Mémorisation de la cellule suivante dans la colonne:
	  nextY1 = FMap->alpha0121(currentY1);
	  nextY2 = j<ASy-1 ? FMap->alpha012321(currentY2) : FMap->alpha0121(currentY2);

	  // Fusion:
	  for (int n=0; n<4; ++n)
	    {
	      tmp1 = currentY1;
	      tmp2 = currentY2;

	      currentY1 = FMap->alpha01(currentY1);
	      currentY2 = FMap->alpha01(currentY2);

	      if ((n==0 && i==0) || n==1 || n==2 || (n==3 && j==0))
		{
		  FMap->unlinkAlpha2(tmp1);
		  FMap->unlinkAlpha2(FMap->alpha0(tmp1));
		}

	      CDart* tmp2Bis = FMap->alpha2(tmp2);

	      FMap->unlinkAlpha2(tmp2);
	      FMap->unlinkAlpha2(FMap->alpha0(tmp2));

	      FMap->topoSew2(tmp1, tmp2Bis);

	      FMap->delMapDart(FMap->alpha0(tmp2));
	      FMap->delMapDart(   tmp2 );
	    }

	  // Passage à la ligne suivante:
	  currentY1 = nextY1;
	  currentY2 = nextY2;
	}

      // Passage à la colonne suivante:
      currentX1 = nextX1;
      currentX2 = nextX2;
    }

  // Paramètres en sortie:
  if (ANextSquare!=NULL)
    * ANextSquare = FMap->alpha10(currentY1);

  if (ANextMesh3Side!=NULL)
    * ANextMesh3Side = FMap->alpha10(currentY2);
}
//******************************************************************************
bool CMeshGeneric::canMergeCube2MeshedAndMesh3(CDart* ACubeDart,
					       CDart* AMesh3Dart,
					       int ASx, int ASy, int ASz)
{
  assert(ACubeDart!=NULL);
  assert(AMesh3Dart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);

  return
    isTopoCube2Meshed(ACubeDart , ASx,ASy,ASz) &&
    isTopoCube3Meshed(AMesh3Dart, ASx,ASy,ASz);
}
//------------------------------------------------------------------------------
void CMeshGeneric::mergeCube2MeshedAndMesh3(CDart* ACubeDart, CDart* AMesh3Dart,
					    int ASx, int ASy, int ASz)
{
  assert(canMergeCube2MeshedAndMesh3(ACubeDart, AMesh3Dart, ASx,ASy,ASz));

  int s[3] = { ASx, ASy, ASz };

  CDart * current1 = ACubeDart;
  CDart * current2 = AMesh3Dart;

  CDart * next1;
  CDart * next2;

  for (int i=0; i<3; ++i)
    {
      next1 = FMap->alpha12(current1);
      next2 = FMap->alpha12(current2);

      CDart * current1Bis;
      CDart * current2Bis;

      mergeSquare2MeshedAndMesh3Side(current1, current2,
				     s[i], s[(i+1)%3],
				     &current1Bis, &current2Bis);

      mergeSquare2MeshedAndMesh3Side(current1Bis, current2Bis,
				     s[i], s[(i+2)%3]);

      current1 = next1;
      current2 = next2;
    }
}
//******************************************************************************
