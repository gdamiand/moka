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
using namespace GMap3d;
//******************************************************************************
#define SIDE(DART) ((((CGMapGeneric*) FMap)->*side)(DART))
#define TURN(DART) ((((CGMapGeneric*) FMap)->*turn)(DART))
#define SUCC(DART) ((((CGMapGeneric*) FMap)->*succ)(DART))
//******************************************************************************
bool CMeshGeneric::isTopoEdge(CDart* ADart, CDart* AModel)
{
  assert(ADart!=NULL);
  return isTopoEdge1Meshed(ADart, 1, AModel);
}
//******************************************************************************
bool CMeshGeneric::isTopoEdge1Meshed(CDart* ADart, int ASx, CDart* AModel)
{
  assert(ADart!=NULL);

  int n = 0;

  for (CDynamicCoverage01 cov(FMap, ADart); n<ASx && cov.cont(); ++cov)
    {
      if (FMap->isFree0(*cov))
	return false;

      if (AModel!=NULL)
	{
	  CDynamicCoverage23 covDart (FMap, *cov  );
	  CDynamicCoverage23 covModel(FMap, AModel);

	  while (covDart.cont() && covModel.cont() &&
		 covDart.prevOperationType()==covModel.prevOperationType())
	    {
	      ++covDart;
	      ++covModel;
	    }

	  if (covDart.cont() || covModel.cont())
	    return false;
	}

      ++cov;
      ++n;

      if (n<ASx && FMap->isFree1(*cov))
	return false;
    }

  return true;
}
//******************************************************************************
bool CMeshGeneric::isTopoEdgeIMeshed(int ADim, CDart* ADart, int AS,
				     CDart* AModel)
{
  assert(ADim>=0 && ADim<=1);
  assert(ADart!=NULL);
  assert(AS>0);

  if (ADim==1)
    return isTopoEdge1Meshed(ADart, AS, AModel);

  return isTopoEdge(ADart, AModel);
}
//******************************************************************************
bool CMeshGeneric::isTopoSquare(CDart* ADart, bool ACheck3Sews, bool A3Sewed)
{
  assert(ADart!=NULL);
  return isTopoSquare1Meshed(ADart, 1,1, ACheck3Sews, A3Sewed);
}
//******************************************************************************
bool CMeshGeneric::isTopoSquare1Meshed(CDart* ADart, int ASx, int ASy,
				       bool ACheck3Sews, bool A3Sewed)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);

  if (ACheck3Sews && A3Sewed==FMap->isFree3(ADart))
    return false;

  int nbDarts = 0;
  int nbToObtain = 4*(ASx+ASy);

  for (CDynamicCoverage01 cov(FMap, ADart); cov.cont(); ++cov)
    {
      if (FMap->isFree0(*cov) || FMap->isFree1(*cov))
	return false;

      if (++nbDarts > nbToObtain)
	return false;
    }

  return nbDarts == nbToObtain;
}
//******************************************************************************
bool CMeshGeneric::isTopoSquare2Meshed(CDart* ADart, int ASx, int ASy,
				       bool ACheck3Sews, bool A3Sewed)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);

  return isTopoSquareIMeshed(2, ADart, ASx,ASy, ACheck3Sews, A3Sewed);
}
//******************************************************************************
bool CMeshGeneric::isTopoSquare3Meshed(CDart* ADart, int ASx, int ASy,
				       bool ACheck3Sews, bool A3Sewed)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);

  return isTopoSquareIMeshed(3, ADart, ASx,ASy, ACheck3Sews, A3Sewed);
}
//******************************************************************************
bool CMeshGeneric::isTopoSquareIMeshed(int ADim, CDart* ADart, int ASx, int ASy,
				       bool ACheck3Sews, bool A3Sewed)
{
  assert(ADim>=0 && ADim<=3);
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);

  if (ADim==0)
    return isTopoSquare(ADart, ACheck3Sews, A3Sewed);

  if (ADim==1)
    return isTopoSquare1Meshed(ADart, ASx, ASy, ACheck3Sews, A3Sewed);

  // Cas où ADim==2 ou 3:

  CDart* (CGMapGeneric::* succ) (CDart*) const
    = ADim==2 ? & CGMapGeneric::alpha0121 : & CGMapGeneric::alpha012321;

  CDart* (CGMapGeneric::* side) (CDart*) const
    = ADim==2 ? & CGMapGeneric::alpha01 : & CGMapGeneric::alpha012;

  CDart* (CGMapGeneric::* turn) (CDart*) const
    = ADim==2 ? & CGMapGeneric::alpha12 : & CGMapGeneric::alpha1232;

  CDart* currentX = ADart;

  for (int i=0; i<ASx; ++i)
    {
      CDart* currentY = FMap->alpha1(currentX);

      for (int j=0; j<ASy; ++j)
	{
	  if (!isTopoSquare(currentY, ACheck3Sews, A3Sewed))
	    return false;

	  if (i<ASx-1)
	    if (FMap->isFree(SIDE(FMap->alpha1(currentY)), ADim))
	      return false;

	  if (j<ASy-1)
	    if (FMap->isFree(SIDE(   currentY ), ADim))
	      return false;

	  if (i>0 && j>0)
	    {
	      CDart* tmp = currentY;

	      for (int k=0; k<4; ++k)
		{
		  tmp = TURN(tmp);

		  if ((k!=3 && tmp==currentY) || (k==3 && tmp!=currentY))
		    return false;
		}
	    }

	  if (j<ASy-1)
	    currentY = SUCC(currentY);
	}

      if (i<ASx-1)
	currentX = SUCC(currentX);
    }

  return true;
}
//******************************************************************************
bool CMeshGeneric::isFreeTopoSquareIMeshed(int ADim, CDart* ADart,
					   int ASx, int ASy,
					   int AMarkNumberNotX1,
					   int AMarkNumberNotX2,
					   int AMarkNumberNotY1,
					   int AMarkNumberNotY2,
					   int AReturnedDart,
					   CDart** AOppositeDart)
{
  assert(ADim>=0 && ADim<=2);
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(isTopoSquareIMeshed(ADim, ADart, ASx,ASy));
  assert(AMarkNumberNotX1>0);
  assert(AMarkNumberNotY1>0);
  assert(AReturnedDart>=1 && AReturnedDart<=3);

  if (ADim==0)
    ASx = ASy = 1;

  CDart* (CGMapGeneric::* succ) (CDart*) const
    = ADim<2 ? & CGMapGeneric::alpha01 : & CGMapGeneric::alpha0121;

  CDart* current = ADart;

  for (int round=0; round<2; ++round)
    {
      for (int i=0; i<ASx; ++i)
	{
	  if (FMap->isMarked(current, AMarkNumberNotX1) ||
	      (AMarkNumberNotX2>=0 &&
	       FMap->isMarked(current, AMarkNumberNotX2)))
	    return false;

	  if (FMap->isMarked(FMap->alpha0(current), AMarkNumberNotX1) ||
	      (AMarkNumberNotX2>=0 &&
	       FMap->isMarked(FMap->alpha0(current), AMarkNumberNotX2)))
	    return false;

	  if (i<ASx-1)
	    current = SUCC(current);
	}

      current = FMap->alpha0(current);

      if (AOppositeDart!=NULL)
	{
	  if (round==0 && AReturnedDart==1)
	    * AOppositeDart = current;

	  if (round==1 && AReturnedDart==2)
	    * AOppositeDart = current;
	}

      current = FMap->alpha1(current);

      for (int j=0; j<ASy; ++j)
	{
	  if (FMap->isMarked(current, AMarkNumberNotY1) ||
	      (AMarkNumberNotY2>=0 &&
	       FMap->isMarked(current, AMarkNumberNotY2)))
	    return false;

	  if (FMap->isMarked(FMap->alpha0(current), AMarkNumberNotY1) ||
	      (AMarkNumberNotY2>=0 &&
	       FMap->isMarked(FMap->alpha0(current), AMarkNumberNotY2)))
	    return false;

	  if (j<ASy-1)
	    current = SUCC(current);
	}

      current = FMap->alpha01(current);

      if (AOppositeDart!=NULL)
	{
	  if (round==0 && AReturnedDart==3)
	    * AOppositeDart = current;
	}
    }

  return true;
}
//******************************************************************************
CDart* CMeshGeneric::isTopoEdgeOfCubeIMeshed(int ADim,
					     CDart* ADart1, CDart* ADart2,
					     int AS)
{
  assert(ADim==1 || ADim==2);
  assert(ADart1!=NULL);
  assert(ADart2!=NULL);

  CDart* (CGMapGeneric::* succ) (CDart*) const
    = ADim==1 ? & CGMapGeneric::alpha01 : & CGMapGeneric::alpha0121;

  CDart* current1 = ADart1;
  CDart* current2 = ADart2;

  for (int i=0; i<AS; ++i)
    {
      if (FMap->alpha2(current1)!=current2)
	return NULL;

      if (i<AS-1)
	{
	  current1 = SUCC(current1);
	  current2 = SUCC(current2);
	}
    }

  return FMap->alpha0(current1);
}
//******************************************************************************
bool CMeshGeneric::isTopoCube(CDart* ADart)
{
  assert(ADart!=NULL);

  return isTopoCube1Meshed(ADart, 1,1,1);
}
//******************************************************************************
bool CMeshGeneric::isTopoCube1Meshed(CDart* ADart, int ASx, int ASy, int ASz)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);

  return isTopoCubeIMeshed(1, ADart, ASx,ASy,ASz);
}
//******************************************************************************
bool CMeshGeneric::isTopoCube2Meshed(CDart* ADart, int ASx, int ASy, int ASz)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);

  return isTopoCubeIMeshed(2, ADart, ASx,ASy,ASz);
}
//******************************************************************************
bool CMeshGeneric::isTopoCube3Meshed(CDart* ADart, int ASx, int ASy, int ASz)
{
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);

  CDart* currentX = ADart;

  for (int i=0; i<ASx; ++i)
    {
      CDart* currentY = FMap->alpha12(currentX);

      for (int j=0; j<ASy; ++j)
	{
	  CDart* currentZ = FMap->alpha12(currentY);

	  for (int k=0; k<ASz; ++k)
	    {
	      if (!isTopoCube(currentZ))
		return false;

	      if (k<ASz-1)
		{
		  currentZ = FMap->alpha012(currentZ);

		  if (FMap->isFree3(currentZ))
		    return false;

		  currentZ = FMap->alpha321(currentZ);
		}
	    }

	  if (j<ASy-1)
	    {
	      currentY = FMap->alpha012(currentY);

	      if (FMap->isFree3(currentY))
		return false;

	      currentY = FMap->alpha321(currentY);
	    }
	}

      if (i<ASx-1)
	{
	  currentX = FMap->alpha012(currentX);

	  if (FMap->isFree3(currentX))
	    return false;

	  currentX = FMap->alpha321(currentX);
	}
    }

  return true;
}
//******************************************************************************
bool CMeshGeneric::isTopoCubeIMeshed(int ADim,
				     CDart* ADart, int ASx, int ASy, int ASz)
{
  assert(ADim>=0 && ADim<=3);
  assert(ADart!=NULL);

  if (ADim==0) return isTopoCube(ADart);
  if (ADim==3) return isTopoCube3Meshed(ADart, ASx,ASy,ASz);

  // Cas où ADim==1 ou 2:

  CDart* memo[4];
  CDart* current = ADart;
  int i;

  // Face du bas:
  if (!isTopoSquareIMeshed(ADim, current, ASx, ASy))
    return false;

  for (i=0; i<4; ++i)
    {
      memo[i] = current;

      int len = i%2 == 0 ? ASx : ASy;

      // Face latérale:
      if (FMap->isFree2(current) ||
	  ! isTopoSquareIMeshed(ADim, FMap->alpha2(current), len, ASz))
	return false;

      // Coutures bas/latérale:
      current = isTopoEdgeOfCubeIMeshed(ADim, current,
					FMap->alpha2(current), len);

      if (current==NULL)
	return false;

      current = FMap->alpha1(current);
    }

  // On vérifie que les 4 coins du bas sont bien de degré 3:
  for (i=0; i<4; ++i)
    if (FMap->degree(memo[i], 0, 3) != 3)
      return false;

  // Coutures d'arêtes verticales:
  for (i=0; i<4; ++i)
    {
      memo[i] = FMap->alpha21(memo[i]);

      if (FMap->isFree2(memo[i]))
	return false;

      memo[i] = isTopoEdgeOfCubeIMeshed(ADim, memo[i],
					FMap->alpha2(memo[i]), ASz);

      if (memo[i]==NULL || FMap->isFree2(FMap->alpha1(memo[i])))
	return false;

      memo[i] = FMap->alpha12(memo[i]);
    }

  // Face du haut:
  if (! isTopoSquareIMeshed(ADim, memo[0], ASx, ASy))
    return false;

  // On vérifie que les 4 coins du haut sont bien de degré 3:
  for (i=0; i<4; ++i)
    if (FMap->degree(memo[i], 0, 3) != 3)
      return false;

  // Coutures haut/latérale:
  for (i=0; i<4; ++i)
    {
      int len = i%2 == 0 ? ASx : ASy;

      if (isTopoEdgeOfCubeIMeshed(ADim, memo[i],
				  FMap->alpha2(memo[i]), len) == NULL)
	return false;
    }

  return true;
}
//******************************************************************************
bool CMeshGeneric::isFreeTopoCubeIMeshed(int ADim, CDart* ADart,
					 int ASx, int ASy, int ASz,
					 int AMarkNumberNotX1,
					 int AMarkNumberNotX2,
					 int AMarkNumberNotY1,
					 int AMarkNumberNotY2,
					 int AMarkNumberNotZ1,
					 int AMarkNumberNotZ2)
{
  assert(ADim>=0 && ADim<=3);
  assert(ADart!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(isTopoCubeIMeshed(ADim, ADart, ASx,ASy,ASz));
  assert(AMarkNumberNotX1>0);
  assert(AMarkNumberNotY1>0);
  assert(AMarkNumberNotZ1>0);

  if (ADim==0)
    ASx = ASy = ASz = 1;

  int s[3]    = { ASx, ASy, ASz };
  int m[3][2] =
  {
    { AMarkNumberNotX1, AMarkNumberNotX2 },
    { AMarkNumberNotY1, AMarkNumberNotY2 },
    { AMarkNumberNotZ1, AMarkNumberNotZ2 },
  };

  CDart* current = ADart;

  for (int dim=0; dim<3; ++dim)
    {
      CDart* next;

      if (! isFreeTopoSquareIMeshed(ADim, current, s[dim], s[(dim+1)%3],
				    m[dim][0], m[dim][1],
				    m[(dim+1)%3][0], m[(dim+1)%3][1],
				    3, &next))
	return false;

      if (! isFreeTopoSquareIMeshed(ADim, FMap->alpha12(next),
				    s[(dim+1)%3], s[(dim+2)%3],
				    m[(dim+1)%3][0], m[(dim+1)%3][1],
				    m[(dim+2)%3][0], m[(dim+2)%3][1]))
	return false;

      current = FMap->alpha12(current);
    }

  return true;
}
//******************************************************************************
