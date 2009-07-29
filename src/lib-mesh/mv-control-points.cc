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
#include "g-map-vertex.hh"
#include "mesh-vertex.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
#define A0(DART) (ADirectInfoAlpha0<0 \
	? FMap->alpha0(DART) \
	: FMap->getDirectInfoAsDart((DART), ADirectInfoAlpha0))

#define A1(DART) (FMap->alpha1(DART))
//------------------------------------------------------------------------------
#define SUCC(DART) ((((CGMapGeneric*) FMap)->*succ)(DART))
//------------------------------------------------------------------------------
void CMeshVertex::get4ControlPoints(CDart* ADart, CVertex* AVertexArray,
				    int ADirectInfoAlpha0)
{
  assert(ADart!=NULL);
  assert(AVertexArray!=NULL);

  CDart* dart0 = A0(ADart);

  AVertexArray[1] = * FMap->findVertex(ADart);
  AVertexArray[2] = * FMap->findVertex(dart0);

  /**/ if (FMap->isFree2(ADart) &&
	   FMap->isFree2(A1(ADart)) &&
	   FMap->isFree2(A1(dart0)))
    {
      // meshDim = 1
      if (FMap->isFree1(ADart))
	AVertexArray[0] = AVertexArray[1];
      else
	AVertexArray[0] = * FMap->findVertex(A0(A1(ADart)));

      if (FMap->isFree1(dart0))
	AVertexArray[3] = AVertexArray[2];
      else
	AVertexArray[3] = * FMap->findVertex(A0(A1(dart0)));
    }
  else if (FMap->isFree3(ADart) &&
	   FMap->isFree3(FMap->alpha12(ADart)) &&
	   FMap->isFree3(FMap->alpha12(dart0)))
    {
      // meshDim = 2
      if (FMap->isFree2(A1(ADart)))
	AVertexArray[0] = AVertexArray[1];
      else
	AVertexArray[0] = * FMap->findVertex(A0(FMap->alpha121(ADart)));

      if (FMap->isFree2(A1(dart0)))
	AVertexArray[3] = AVertexArray[2];
      else
	AVertexArray[3] = * FMap->findVertex(A0(FMap->alpha121(dart0)));
    }
  else
    {
      // meshDim = 3;
      if (FMap->isFree3(FMap->alpha12(ADart)))
	AVertexArray[0] = AVertexArray[1];
      else
	AVertexArray[0] = * FMap->findVertex(A0(FMap->alpha12321(ADart)));

      if (FMap->isFree3(FMap->alpha12(dart0)))
	AVertexArray[3] = AVertexArray[1];
      else
	AVertexArray[3] = * FMap->findVertex(A0(FMap->alpha12321(dart0)));
    }
}
//------------------------------------------------------------------------------
void CMeshVertex::get16ControlPoints(CDart* ADart, CVertex** AVertexArray,
				     int ADirectInfoAlpha0)
{
  assert(ADart!=NULL);
  assert(AVertexArray!=NULL);

  CDart* dart0 = A1(A0(ADart));

  ADart = A1(ADart);

  if (FMap->isFree3(FMap->alpha12(ADart)) &&
      FMap->isFree3(FMap->alpha12(dart0)))
    {
      // meshDim = 2

//        static int x[4][2][2] =
//        {
//  	{ { 0 , 0 } , { 1 , 1 } },
//  	{ { 0 , 1 } , { 0 , 1 } },
//  	{ { 3 , 3 } , { 2 , 2 } },
//  	{ { 3 , 2 } , { 3 , 2 } }
//        };
//        static int y[4][2][2] =
//        {
//  	{ { 0 , 1 } , { 0 , 1 } },
//  	{ { 3 , 3 } , { 2 , 2 } },
//  	{ { 3 , 2 } , { 3 , 2 } },
//  	{ { 0 , 0 } , { 1 , 1 } }
//        };

//  #define V(I,J) (AVertexArray[x[time][I][J]][y[time][I][J]])

//        for (int time=0; time<4; ++time)
//  	{
//  	  // Sommets faciles à retrouver:
//  	  V(1,1) = * findVertex(ADart);

//  	  V(0,1) = isFree2(   ADart ) ? V(1,1) : * findVertex(alpha210(   ADart ));
//  	  V(1,0) = isFree2(alpha1(ADart)) ? V(1,1) : * findVertex(alpha210(alpha1(ADart)));

//  	  // Quatrième sommet:
//      	  int deg = degree(ADart, 0, 3);
//      	  int mid = deg / 2;
//      	  CDart* d = ADart;

//      	  for (int i=0; i<mid; ++i)
//      	    d = alpha21(d);

//      	  V(0,0) = * findVertex((deg % 2) ? alpha0(d) : alpha010(d));

//  	  // Passage au sommet central suivant:
//  	  ADart = alpha01(ADart);
//  	}
      
      get4ControlPoints(A1(A0(FMap->alpha21(ADart))), AVertexArray[0], ADirectInfoAlpha0);
      get4ControlPoints(                    ADart   , AVertexArray[1], ADirectInfoAlpha0);
      get4ControlPoints(                    dart0   , AVertexArray[2], ADirectInfoAlpha0);
      get4ControlPoints(A1(A0(FMap->alpha21(dart0))), AVertexArray[3], ADirectInfoAlpha0);
//  #undef V

      return;
    }

  // meshDim = 3
  get4ControlPoints(A1(A0(FMap->alpha2321(ADart))), AVertexArray[0], ADirectInfoAlpha0);
  get4ControlPoints(                      ADart   , AVertexArray[1], ADirectInfoAlpha0);
  get4ControlPoints(                      dart0   , AVertexArray[2], ADirectInfoAlpha0);
  get4ControlPoints(A1(A0(FMap->alpha2321(dart0))), AVertexArray[3], ADirectInfoAlpha0);
}
//------------------------------------------------------------------------------
void CMeshVertex::get64ControlPoints(CDart* ADart, CVertex*** AVertexArray,
				     int ADirectInfoAlpha0)
{
  assert(ADart!=NULL);
  assert(AVertexArray!=NULL);

  CDart* currentX =
    A0(FMap->alpha12321(FMap->alpha1(A0(FMap->alpha2321(FMap->alpha12(A0(FMap->alpha321(ADart))))))));

  for (int i=0; i<4; ++i)
    {
      CDart* currentY = FMap->alpha1(currentX);

      for (int j=0; j<4; ++j)
	{
	  CDart* currentZ = FMap->alpha21(currentY);
	
	  for (int k=0; k<4; ++k)
	    {
	      AVertexArray[i][j][k] = * FMap->findVertex(currentZ);
	
	      currentZ = k<3 ? FMap->alpha12321(A0(currentZ)) : A0(currentZ);
	    }
	
	  currentY = j<3 ? FMap->alpha12321(A0(currentY)) : A0(currentY);
	}

      currentX = i<3 ? FMap->alpha12321(A0(currentX)) : A0(currentX);
    }
}
//------------------------------------------------------------------------------
#undef A0
#undef A1
//******************************************************************************
CDart* CMeshVertex::getMesh1Dive(CDart* ADart, int ASx,
				 const CVertex** AVertexTable,
				 bool AAlready2MeshedSquare)
{
  assert(ADart!=NULL);
  assert(AVertexTable!=NULL);
  assert(ASx>0);

  CDart* currentX = ADart;

  CDart* (CGMapGeneric::* succ) (CDart*) const
    = AAlready2MeshedSquare ?
    & CGMapGeneric::alpha0121 : & CGMapGeneric::alpha01;

  for (int i=0; i<=ASx; ++i)
    {
      assert(FMap->findVertex(currentX) != NULL);

      AVertexTable[i] = FMap->findVertex(currentX);

      if (i<=ASx-2)
	currentX = SUCC(currentX);
      else
	if (i==ASx-1)
	  currentX = FMap->alpha0(currentX);
    }

  return currentX;
}
//------------------------------------------------------------------------------
CDart* CMeshVertex::getMesh2Dive(CDart* ADart, int ASx, int ASy,
				 const CVertex*** AVertexTable,
				 bool AAlready3MeshedCube,
				 int AReturnedDart)
{
  assert(ADart!=NULL);
  assert(AVertexTable!=NULL);
  assert(ASx>0);
  assert(ASy>0);
  assert(1<=AReturnedDart && AReturnedDart<=3);

  CDart* currentX, * currentY=NULL, * returned;

  CDart* (CGMapGeneric::* succ) (CDart*) const
    = AAlready3MeshedCube ?
    & CGMapGeneric::alpha012321 : & CGMapGeneric::alpha0121;

  currentX = ADart;

  for (int i=0; i<=ASx; ++i)
    {
      if (i==ASx && AReturnedDart==1)
	returned = currentX;

      currentY = FMap->alpha1(currentX);

      for (int j=0; j<=ASy; ++j)
	{
	  assert(FMap->findVertex(currentY) != NULL);

	  AVertexTable[i][j] = FMap->findVertex(currentY);
	
	  if (j<=ASy-2)
	    currentY = SUCC(currentY);
	  else
	    if (j==ASy-1)
	      currentY = FMap->alpha0(currentY);
	}
	
      if (i==0 && AReturnedDart==2)
	returned = FMap->alpha1(currentY);

      if (i<=ASx-2)
	currentX = SUCC(currentX);
      else
	if (i==ASx-1)
	  currentX = FMap->alpha0(currentX);
    }

  if (AReturnedDart==1)
    returned = currentX;
  else
    returned = FMap->alpha1(currentY);

  return returned;
}
//------------------------------------------------------------------------------
#undef SUCC
//******************************************************************************
