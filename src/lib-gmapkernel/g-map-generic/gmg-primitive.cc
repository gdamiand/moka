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
using namespace GMap3d;
//******************************************************************************
CDart * CGMapGeneric::createTopoTriangle()
{
  CDart * dart[6];

  for (int i=0; i<6; ++i)
    dart[i] = addMapDart();

  linkAlpha0(dart[0], dart[1]); linkAlpha1(dart[1], dart[2]);
  linkAlpha0(dart[2], dart[3]); linkAlpha1(dart[3], dart[4]);
  linkAlpha0(dart[4], dart[5]); linkAlpha1(dart[5], dart[0]);

  return dart[0];
}
//******************************************************************************
CDart * CGMapGeneric::createTopoSquare()
{
  CDart * dart[8];

  for (int i=0; i<8; ++i)
    dart[i] = addMapDart();

  linkAlpha0(dart[0], dart[1]); linkAlpha1(dart[1], dart[2]);
  linkAlpha0(dart[2], dart[3]); linkAlpha1(dart[3], dart[4]);
  linkAlpha0(dart[4], dart[5]); linkAlpha1(dart[5], dart[6]);
  linkAlpha0(dart[6], dart[7]); linkAlpha1(dart[7], dart[0]);

  return dart[0];
}
//******************************************************************************
CDart * CGMapGeneric::createTopoPolygon(int An)
{
  assert(An>0);

  CDart * dart0, * current = NULL, * current0, * current01;

  linkAlpha0(dart0 = addMapDart(), current01 = addMapDart());

  for (int i=1; i<An; ++i)
    {
      current  = addMapDart();
      current0 = addMapDart();

      linkAlpha0(current , current0 );
      linkAlpha1(current0, current01);

      current01 = current;
    }

  linkAlpha1(current, dart0);

  return dart0;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoOpenedPole(int An)
{
  assert(An>0);

  CDart * dart0, * current = NULL, * current21;

  dart0 = current21 = createTopoTriangle();

  for (int i=1; i<An; ++i)
    {
      current = createTopoTriangle();
      topoSew2(current, alpha1(current21));
      current21 = current;
    }

  /* On n'effectue pas la dernière couture, comme c'est fait dans la méthode
     createTopoPole qui suit... */

  return dart0;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoPole(int An)
{
  assert(An>0);

  CDart * dart0, * current=NULL, * current21;

  dart0 = current21 = createTopoTriangle();

  for (int i=1; i<An; ++i)
    {
      current = createTopoTriangle();
      topoSew2(current, alpha1(current21));
      current21 = current;
    }

  topoSew2(alpha1(current), dart0);

  return dart0;
}
//******************************************************************************
void CGMapGeneric::createTopoTube(int AMeridians, int AParallels,
				  CDart * * ABorder1, CDart * * ABorder2)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * mesh2Corners[2][2];

  createTopoMesh2(AMeridians,AParallels, mesh2Corners);
  border2TopoSew(alpha1(mesh2Corners[0][0]), alpha1(mesh2Corners[1][0]), AParallels);

  if (ABorder1!=NULL) * ABorder1 = mesh2Corners[0][0];
  if (ABorder2!=NULL) * ABorder2 = mesh2Corners[0][1];
}
//******************************************************************************
CDart * CGMapGeneric::createTopoTube(int AMeridians, int AParallels)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * border1;

  createTopoTube(AMeridians,AParallels, &border1,NULL);

  return border1;
}
//******************************************************************************
void CGMapGeneric::createTopoSphere(int AMeridians, int AParallels,
				    CDart * * ASouthPole, CDart * * ANorthPole)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * southPole = createTopoPole(AMeridians);
  CDart * northPole = createTopoPole(AMeridians);

  if (AParallels==1)
    border2TopoSew(alpha01(southPole), alpha01(northPole), AMeridians);
  else
    {
      CDart * top, * bottom;

      createTopoTube(AMeridians,AParallels-1, &bottom, &top);

      border2TopoSew(alpha01(southPole), bottom, AMeridians);
      border2TopoSew(alpha01(northPole), top   , AMeridians);
    }

  if (ASouthPole!=NULL) * ASouthPole = southPole;
  if (ANorthPole!=NULL) * ANorthPole = northPole;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoSphere(int AMeridians, int AParallels)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * southPole;

  createTopoSphere(AMeridians,AParallels, &southPole,NULL);

  return southPole;
}
//******************************************************************************
void CGMapGeneric::createTopoCylinder(int AMeridians, int AParallels,
				      CDart * * ABorder1, CDart * * ABorder2,
				      bool AClose1, bool AClose2)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * border1;
  CDart * border2;

  createTopoTube(AMeridians,AParallels, &border1,&border2);

  if (AClose1)
    border2TopoSew(border1, createTopoPolygon(AMeridians), AMeridians);

  if (AClose2)
    border2TopoSew(border2, createTopoPolygon(AMeridians), AMeridians);

  if (ABorder1!=NULL) * ABorder1 = border1;
  if (ABorder2!=NULL) * ABorder2 = border2;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoCylinder(int AMeridians, int AParallels,
					bool AClose1, bool AClose2)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * border1;

  createTopoCylinder(AMeridians,AParallels, &border1,NULL, AClose1,AClose2);

  return border1;
}
//******************************************************************************
void CGMapGeneric::createTopoPyramid(int AMeridians, int AParallels,
				     CDart * * ABaseDart, CDart * * APoleDart,
				     bool ACloseBase)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * pole = createTopoPole(AMeridians);
  CDart * base;

  if (AParallels==1)
    base = alpha01(pole);
  else
    {
      CDart * top;

      createTopoTube(AMeridians, AParallels-1, &base,&top);
      border2TopoSew(top, alpha01(pole), AMeridians);
    }

  if (ACloseBase)
    border2TopoSew(base, createTopoPolygon(AMeridians), AMeridians);

  if (ABaseDart!=NULL) * ABaseDart = base;
  if (APoleDart!=NULL) * APoleDart = pole;
}
//******************************************************************************
CDart * CGMapGeneric::createTopoPyramid(int AMeridians, int AParallels,
				       bool ACloseBase)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * baseDart;

  createTopoPyramid(AMeridians,AParallels, &baseDart,NULL, ACloseBase);

  return baseDart;
}
//******************************************************************************
void CGMapGeneric::createTopoTorus(int AMeridians, int AParallels,
				   CDart * * AEquator)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * border1;
  CDart * border2;

  // Attention: Les méridiens d'un tore correspondent aux parallèles du tube à
  // partie duquel on le construit.

  createTopoTube(AParallels, AMeridians, &border1, &border2);
  border2TopoSew(border1, border2, AParallels);

  if (AEquator!=NULL) * AEquator = alpha1(border1);
}
//******************************************************************************
CDart * CGMapGeneric::createTopoTorus(int AMeridians, int AParallels)
{
  assert(AMeridians>0);
  assert(AParallels>0);

  CDart * equator;

  createTopoTorus(AMeridians,AParallels, &equator);

  return equator;
}
//******************************************************************************
void CGMapGeneric::createTopoSquareIMeshed(int ASx, int ASy,
					   int AMeshDimension,
					   CDart * ASquareCorners[2][2])
{
  assert(ASx>0);
  assert(ASy>0);
  assert(AMeshDimension>=0 && AMeshDimension<=2);

  if (AMeshDimension==0)
    ASx = ASy = 1;

  if (AMeshDimension==2)
    createTopoMesh2(ASx, ASy, ASquareCorners);
  else
    {
      CDart * corners[2];

      createTopoMesh1(ASx, corners);
      ASquareCorners[0][0] = corners[0];
      ASquareCorners[1][0] = corners[1];

      createTopoMesh1(ASx, corners);
      ASquareCorners[0][1] = corners[0];
      ASquareCorners[1][1] = corners[1];

      createTopoMesh1(ASy, corners);
      linkAlpha1(corners[0], ASquareCorners[0][0]);
      linkAlpha1(corners[1], ASquareCorners[0][1]);

      createTopoMesh1(ASy, corners);
      linkAlpha1(corners[0], ASquareCorners[1][0]);
      linkAlpha1(corners[1], ASquareCorners[1][1]);
    }
}
//******************************************************************************
void CGMapGeneric::createTopoCubeIMeshed(int ASx, int ASy, int ASz,
					 int AMeshDimension,
					 bool ACreatedFaces[3][2],
					 CDart * AFacesCorners[3][2][2][2])
{
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(AMeshDimension>=0 && AMeshDimension<=2);
  assert(ACreatedFaces!=NULL);
  assert(AFacesCorners!=NULL);

  if (AMeshDimension==0)
    ASx = ASy = ASz = 1;

  int dim, dim1, dim2;
  int s[3] = {ASx, ASy, ASz};

  // Créations:
  for (dim=0; dim<3; ++dim)
    {
      dim1 = (dim+1) % 3;
      dim2 = (dim+2) % 3;

      for (int side=0; side<2; ++side)
	if (ACreatedFaces[dim][side])
	  createTopoSquareIMeshed(s[dim1], s[dim2],
				  AMeshDimension, AFacesCorners[dim][side]);
	else
	  for (int n=0; n<4; ++n)
	    AFacesCorners[dim][side][n/2][n%2] = NULL;
    }

  // Coutures:
  for (dim=0; dim<3; ++dim)
    {
      dim1 = (dim+1) % 3;
      dim2 = (dim+2) % 3;

      for (int dim1Side=0; dim1Side<2; ++dim1Side)
	for (int dim2Side=0; dim2Side<2; ++dim2Side)
	  if (ACreatedFaces[dim1][dim1Side] && ACreatedFaces[dim2][dim2Side])
	    {
	      CDart * dartA = AFacesCorners[dim1][dim1Side][dim2Side][0];
	      CDart * dartB = AFacesCorners[dim2][dim2Side][0][dim1Side];

	      border2TopoSew(alpha1(dartA), dartB, s[dim]);
	    }
    }
}
//******************************************************************************
