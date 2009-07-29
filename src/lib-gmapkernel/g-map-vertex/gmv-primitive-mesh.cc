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
#define SUCC(DART) ((((CGMapGeneric*) this)->*succ)(DART))
//------------------------------------------------------------------------------
void CGMapVertex::diveMesh1(CDart * AFirstDart, CMesh1Diver & ADiver,
			    TEmbedMethod ADiveBorders, int AMeshDimension)
{
  assert(1 <= AMeshDimension && AMeshDimension <= 3);

  CDart * (CGMapGeneric::* succ) (CDart *) const = NULL;

  switch (AMeshDimension)
    {
    case 1: succ = & CGMapGeneric::alpha01    ; break;
    case 2: succ = & CGMapGeneric::alpha0121  ; break;
    case 3: succ = & CGMapGeneric::alpha012321; break;
    }

  CDart * currentX =
    ADiveBorders==GMV_NO ? SUCC(AFirstDart) : AFirstDart;

  int iMin = ADiveBorders==GMV_NO ? 1 : 0;
  int iMax = ADiveBorders==GMV_NO ? ADiver.getSx() - 1 : ADiver.getSx();

  for (int i=iMin; i<=iMax; ++i, currentX = SUCC(currentX))
    {
      ADiver.setIx(i);

      if (ADiveBorders==GMV_CHECK && (i==0 || i==ADiver.getSx()))
	updateVertex(currentX, ADiver.computeCurrentVertex());
      else
	setVertex(currentX, ADiver.computeCurrentVertex());
    }
}
//******************************************************************************
void CGMapVertex::diveMesh2(CDart * AFirstDart, CMesh2Diver & ADiver,
			    TEmbedMethod ADiveBorders, int AMeshDimension)
{
  assert(2 <= AMeshDimension && AMeshDimension <= 3);

  CDart * (CGMapGeneric::* succ) (CDart *) const = NULL;

  switch (AMeshDimension)
    {
    case 2: succ = & CGMapGeneric::alpha0121  ; break;
    case 3: succ = & CGMapGeneric::alpha012321; break;
    }

  CDart * currentY =
    ADiveBorders==GMV_NO ? SUCC(alpha1(SUCC(AFirstDart))) : alpha1(AFirstDart);

  int iMin = ADiveBorders==GMV_NO ? 1 : 0;
  int iMax = ADiveBorders==GMV_NO ? ADiver.getSx() - 1 : ADiver.getSx();

  int jMin = ADiveBorders==GMV_NO ? 1 : 0;
  int jMax = ADiveBorders==GMV_NO ? ADiver.getSy() - 1 : ADiver.getSy();

  for (int j=jMin; j<=jMax;
       ++j, currentY = j==jMax ? alpha0(currentY) : SUCC(currentY))
    {
      ADiver.setIy(j);

      bool borderY = j==0 || j==ADiver.getSy();

      CDart * currentX = alpha1(currentY);

      for (int i=iMin; i<=iMax; ++i, currentX = SUCC(currentX))
	{
	  ADiver.setIx(i);

	  bool borderX = i==0 || i==ADiver.getSx();

	  if (ADiveBorders==GMV_CHECK && (borderX || borderY))
	    updateVertex(currentX, ADiver.computeCurrentVertex());
	  else
	    setVertex(currentX, ADiver.computeCurrentVertex());
	}
    }
}
//******************************************************************************
void CGMapVertex::diveMesh3(CDart * AFirstDart, CMesh3Diver & ADiver,
			    TEmbedMethod ADiveBorders)
{
  CDart * currentZ =
    ADiveBorders==GMV_NO
    ? alpha210123(alpha101232(alpha012321(AFirstDart)))
    : AFirstDart;

  currentZ = alpha21(currentZ);

  int iMin = ADiveBorders==GMV_NO ? 1 : 0;
  int iMax = ADiveBorders==GMV_NO ? ADiver.getSx() - 1 : ADiver.getSx();

  int jMin = ADiveBorders==GMV_NO ? 1 : 0;
  int jMax = ADiveBorders==GMV_NO ? ADiver.getSy() - 1 : ADiver.getSy();

  int kMin = ADiveBorders==GMV_NO ? 1 : 0;
  int kMax = ADiveBorders==GMV_NO ? ADiver.getSz() - 1 : ADiver.getSz();

  for (int k=kMin; k<=kMax;
       ++k, currentZ = k==kMax ? alpha0(currentZ) : alpha012321(currentZ))
    {
      ADiver.setIz(k);

      bool borderZ = k==0 || k==ADiver.getSz();

      CDart * currentY = alpha21(currentZ);

      for (int j=jMin; j<=jMax;
	   ++j, currentY = j==jMax ? alpha0(currentY) : alpha012321(currentY))
	{
	  ADiver.setIy(j);

	  bool borderY = j==0 || j==ADiver.getSy();

	  CDart * currentX = alpha21(currentY);

	  for (int i=iMin; i<=iMax;
	       ++i, currentX = i==iMax ? alpha0(currentX) : alpha012321(currentX))
	    {
	      ADiver.setIx(i);
	
	      bool borderX = i==0 || i==ADiver.getSx();

	      if (ADiveBorders==GMV_CHECK && (borderX || borderY || borderZ))
		updateVertex(currentX, ADiver.computeCurrentVertex());
	      else
		setVertex(currentX, ADiver.computeCurrentVertex());
	    }
	}
    }
}
//******************************************************************************
void CGMapVertex::createMesh1(int ASx, CDart * AMesh1Corners[2],
			      CMesh1Diver & ADiver)
{
  assert(ASx == ADiver.getSx());
  assert(AMesh1Corners!=NULL);

  createTopoMesh1(ASx, AMesh1Corners);
  diveMesh1(AMesh1Corners[0], ADiver);
}
//******************************************************************************
CDart * CGMapVertex::createMesh1(int ASx, CMesh1Diver & ADiver)
{
  CDart * mesh1Corners[2];

  createMesh1(ASx, mesh1Corners, ADiver);

  return mesh1Corners[0];
}
//******************************************************************************
CDart * CGMapVertex::createMesh1(int ASx)
{
  CMesh1BasicDiver diver(ASx, -OX/2,OX);
  return createMesh1(ASx, diver);
}
//******************************************************************************
void CGMapVertex::createMesh2(int ASx, int ASy,
			 CDart * AMesh2Corners[2][2],
			 CMesh2Diver & ADiver)
{
  assert(ASx == ADiver.getSx());
  assert(ASy == ADiver.getSy());
  assert(AMesh2Corners!=NULL);

  createTopoMesh2(ASx,ASy, AMesh2Corners);
  diveMesh2(AMesh2Corners[0][0], ADiver);
}
//******************************************************************************
CDart * CGMapVertex::createMesh2(int ASx, int ASy, CMesh2Diver & ADiver)
{
  CDart * mesh2Corners[2][2];

  createMesh2(ASx,ASy, mesh2Corners, ADiver);

  return mesh2Corners[0][0];
}
//******************************************************************************
CDart * CGMapVertex::createMesh2(int ASx, int ASy)
{
  CMesh2BasicDiver diver(ASx,ASy, -(OX+OY)/2, OX,OY);
  return createMesh2(ASx,ASy, diver);
}
//******************************************************************************
void CGMapVertex::createMesh3(int ASx, int ASy, int ASz,
			 CDart * AMesh3Corners[2][2][2],
			 CMesh3Diver & ADiver)
{
  assert(ASx == ADiver.getSx());
  assert(ASy == ADiver.getSy());
  assert(ASz == ADiver.getSz());
  assert(AMesh3Corners!=NULL);

  createTopoMesh3(ASx,ASy,ASz, AMesh3Corners);
  diveMesh3(AMesh3Corners[0][0][0], ADiver);
}
//******************************************************************************
CDart * CGMapVertex::createMesh3(int ASx, int ASy, int ASz, CMesh3Diver & ADiver)
{
  CDart * mesh3Corners[2][2][2];

  createMesh3(ASx,ASy,ASz, mesh3Corners, ADiver);

  return mesh3Corners[0][0][0];
}
//******************************************************************************
CDart * CGMapVertex::createMesh3(int ASx, int ASy, int ASz)
{
  CMesh3BasicDiver diver(ASx,ASy,ASz, -(OX+OY+OZ)/2, OX,OY,OZ);
  return createMesh3(ASx,ASy,ASz, diver);
}
//------------------------------------------------------------------------------
#undef SUCC
//******************************************************************************
