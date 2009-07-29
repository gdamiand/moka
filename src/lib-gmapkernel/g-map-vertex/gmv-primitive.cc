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
using namespace std;
using namespace GMap3d;
//******************************************************************************
CDart * CGMapVertex::createPolyline(const vector<CVertex> & AVector)
{
  CMesh1VectorDiver ADiver(AVector);
  return createMesh1(AVector.size()-1, ADiver);
}
//******************************************************************************
CDart * CGMapVertex::createPole(int An, const CVertex & AVertex)
{
  assert(An>0);

  CDart * pole = createTopoPole(An);
  setVertex(pole, AVertex);
  return pole;
}
//******************************************************************************
CDart * CGMapVertex::createOpenedPole(int An, const CVertex & AVertex)
{
  assert(An>0);

  CDart * pole = createTopoOpenedPole(An);
  setVertex(pole, AVertex);
  return pole;
}
//******************************************************************************
CDart * CGMapVertex::createRegularPolygon(int An)
{
  assert(An>=3);

  CTransformationMatrix matrix;
  CVertex vertex(OX/2);
  matrix.rotate(OZ, 360.0/An);

  CDart * current = createTopoPolygon(An);

  for (int i=0; i<An; ++i)
    {
      setVertex(current, vertex);
      matrix.applyOn(vertex);
      current= alpha01(current);
    }

  return current;
}
//******************************************************************************
void CGMapVertex::createSphere(int AMeridians, int AParallels,
			       CDart * * ASouthPole, CDart * * ANorthPole)
{
  assert(AMeridians>=3);
  assert(AParallels>=1);

  CDart * southPole;
  CDart * northPole;

  // Topologie:
  CGMapGeneric::createTopoSphere(AMeridians,AParallels, &southPole, &northPole);

  // Géométrie:
  CTransformationMatrix rotationMatrix;
  rotationMatrix.rotate(OZ, 360.0/AMeridians);

  setVertex(southPole, -OZ/2);
  setVertex(northPole, +OZ/2);

  CDart * currentY = alpha0121(southPole);

  for (int j=0; j<AParallels; ++j, currentY = alpha0121(currentY))
    {
      CVertex vertex;

      TCoordinate alpha = 180.0*(j+1)/(AParallels+1) - 90.0;

      vertex.setX(dCos(alpha)/2);
      vertex.setZ(dSin(alpha)/2);

      CDart * currentX = alpha1(currentY);

      for (int i=0; i<AMeridians; ++i, currentX = alpha0121(currentX))
	{
	  setVertex(currentX, vertex);
	  rotationMatrix.applyOn(vertex);
	}
    }

  // Paramètres en sortie:
  if (ASouthPole!=NULL) * ASouthPole = southPole;
  if (ANorthPole!=NULL) * ANorthPole = northPole;
}
//******************************************************************************
CDart * CGMapVertex::createSphere(int AMeridians, int AParallels)
{
  assert(AMeridians>=3);
  assert(AParallels>=1);

  CDart * southPole;

  createSphere(AMeridians,AParallels, &southPole,NULL);

  return southPole;
}
//******************************************************************************
void CGMapVertex::createCylinder(int AMeridians, int AParallels,
				 CDart * * ABorder1, CDart * * ABorder2,
				 bool AClose1, bool AClose2)
{
  assert(AMeridians>=3);
  assert(AParallels>=1);

  CDart * border1, * border2;

  // Topologie:
  createTopoCylinder(AMeridians,AParallels, &border1,&border2, AClose1,AClose2);

  // Géométrie:
  CTransformationMatrix rotationMatrix;
  rotationMatrix.rotate(OZ, 360.0/AMeridians);
  CVertex vertex((OX-OZ)/2);

  CDart * currentY = border1;

  for (int i=0; i<AMeridians; ++i, currentY = alpha0121(currentY))
    {
      CDart * currentX = alpha1(currentY);

      for (int j=0; j<=AParallels; ++j, currentX = alpha0121(currentX))
	{
	  setVertex(currentX, vertex);

	  if (j<AParallels)
	    vertex += OZ/AParallels;
	}

      vertex -= OZ;

      rotationMatrix.applyOn(vertex);
    }

  // Paramètres en sortie:
  if (ABorder1!=NULL) * ABorder1 = border1;
  if (ABorder2!=NULL) * ABorder2 = border2;
}
//******************************************************************************
CDart * CGMapVertex::createCylinder(int AMeridians, int AParallels,
				    bool AClose1, bool AClose2)
{
  assert(AMeridians>=3);
  assert(AParallels>=1);

  CDart * border1;

  createCylinder(AMeridians,AParallels, &border1,NULL, AClose1,AClose2);

  return border1;
}
//******************************************************************************
void CGMapVertex::createPyramid(int AMeridians, int AParallels,
				CDart * * ABaseDart, CDart * * APoleDart,
				bool ACloseBase)
{
  assert(AMeridians>=3);
  assert(AParallels>=1);

  CDart * baseDart, * poleDart;

  // Topologie:
  createTopoPyramid(AMeridians,AParallels, &baseDart,&poleDart, ACloseBase);

  // Géométrie:
  setVertex(poleDart, OZ/2);

  CTransformationMatrix rotationMatrix;
  rotationMatrix.rotate(OZ, 360.0/AMeridians);

  CDart * currentX = alpha1(baseDart);

  for (int i=0; i<AParallels; ++i, currentX = alpha0121(currentX))
    {
      CVertex vertex;

      vertex.setX(0.5*(AParallels-i)/AParallels);
      vertex.setZ(((float) i)/AParallels-0.5);

      CDart * currentY = alpha1(currentX);

      for (int j=0; j<AMeridians; ++j, currentY = alpha0121(currentY))
	{
	  setVertex(currentY, vertex);
	  rotationMatrix.applyOn(vertex);
	}
    }

  // Paramètres en sortie:
  if (ABaseDart!=NULL) * ABaseDart = baseDart;
  if (APoleDart!=NULL) * APoleDart = poleDart;
}
//******************************************************************************
CDart * CGMapVertex::createPyramid(int AMeridians, int AParallels,
				   bool ACloseBase)
{
  assert(AMeridians>=3);
  assert(AParallels>=1);

  CDart * baseDart;

  createPyramid(AMeridians,AParallels, &baseDart,NULL, ACloseBase);

  return baseDart;
}
//******************************************************************************
void CGMapVertex::createTorus(int AMeridians, int AParallels,
			      TCoordinate ARadiusProportion,
			      CDart * * AEquator)
{
  assert(AMeridians>=3);
  assert(AParallels>=3);

  CDart * equator;

  // Topologie:
  createTopoTorus(AMeridians,AParallels, &equator);

  // Géométrie:
  TCoordinate R = 0.5/(1+ARadiusProportion);
  TCoordinate r = 0.5 - R;

  CTransformationMatrix matrix1; matrix1.rotate(OY, 360.0/AParallels);
  CTransformationMatrix matrix2; matrix2.translate(R*OX);
  CTransformationMatrix matrix3; matrix3.rotate(OZ, 360.0/AMeridians);

  CVertex vertex1(r*OX);

  CDart * currentY = alpha1(equator);

  for (int j=0; j<AParallels; ++j, currentY = alpha0121(currentY))
    {
      CVertex vertex2(vertex1);
      matrix2.applyOn(vertex2);

      CDart * currentX = alpha1(currentY);

      for (int i=0; i<AMeridians; ++i, currentX = alpha0121(currentX))
	{
	  setVertex(currentX, vertex2);
	  matrix3.applyOn(vertex2);
	}

      matrix1.applyOn(vertex1);
    }

  // Paramètre en sortie:
  if (AEquator!=NULL) * AEquator = equator;
}
//******************************************************************************
CDart * CGMapVertex::createTorus(int AMeridians, int AParallels,
				 TCoordinate ARadiusProportion)
{
  assert(AMeridians>=3);
  assert(AParallels>=3);

  CDart * equator;

  createTorus(AMeridians,AParallels, ARadiusProportion, &equator);

  return equator;
}
//******************************************************************************
void CGMapVertex::createSquareIMeshed(int ASx, int ASy, int AMeshDimension,
				      CDart * ASquareCorners[2][2])
{
  assert(ASx>0);
  assert(ASy>0);
  assert(AMeshDimension>=0 && AMeshDimension<=2);
  assert(ASquareCorners!=NULL);

  if (AMeshDimension==0)
    ASx = ASy = 1;

  createTopoSquareIMeshed(ASx,ASy, AMeshDimension, ASquareCorners);

  if (AMeshDimension==2)
    {
      CMesh2BasicDiver diver(ASx,ASy, (-OX-OY)/2, OX,OY);

      diveMesh2(ASquareCorners[0][0], diver, GMV_YES);
    }
  else
    {
      CMesh1BasicDiver bottom(ASx, (-OX-OY)/2, OX);
      CMesh1BasicDiver top   (ASx, (-OX+OY)/2, OX);
      CMesh1BasicDiver left  (ASy, (-OX-OY)/2, OY);
      CMesh1BasicDiver right (ASy, (+OX-OY)/2, OY);

      diveMesh1(       ASquareCorners[0][0] , bottom, GMV_YES);
      diveMesh1(       ASquareCorners[0][1] , top   , GMV_YES);
      diveMesh1(alpha1(ASquareCorners[0][0]), left  , GMV_NO );
      diveMesh1(alpha1(ASquareCorners[1][0]), right , GMV_NO );
    }
}
//******************************************************************************
CDart * CGMapVertex::createSquareIMeshed(int ASx, int ASy, int AMeshDimension)
{
  CDart * squareCorners[2][2];

  createSquareIMeshed(ASx,ASy, AMeshDimension, squareCorners);

  return squareCorners[0][0];
}
//******************************************************************************
void CGMapVertex::createCubeIMeshed(int ASx, int ASy, int ASz,
				    int AMeshDimension,
				    bool ACreatedFaces[3][2],
				    CDart * AFacesCorners[3][2][2][2])
{
  assert(ASx>0);
  assert(ASy>0);
  assert(ASz>0);
  assert(AMeshDimension>=0 && AMeshDimension<=2);
  assert(AFacesCorners!=NULL);

  // Topologie:
  createTopoCubeIMeshed(ASx, ASy, ASz, AMeshDimension,
			ACreatedFaces, AFacesCorners);

  // Plongements:
  if (AMeshDimension==0)
    ASx = ASy = ASz = 1;

  int s[3] = { ASx, ASy, ASz};

  // Sommets:
  for (int i=0; i<2; ++i)
    for (int j=0; j<2; ++j)
      for (int k=0; k<2; ++k)
	{
	  CDart * corner;

	  /**/ if (ACreatedFaces[0][i]) corner = AFacesCorners[0][i][j][k];
	  else if (ACreatedFaces[1][j]) corner = AFacesCorners[1][j][k][i];
	  else if (ACreatedFaces[2][k]) corner = AFacesCorners[2][k][i][j];
	  else corner = NULL;

	  if (corner!=NULL)
	    setVertex(corner, CVertex(i-0.5, j-0.5, k-0.5));
	}

  // Arêtes:
  if (AMeshDimension>=1)
    for (int dim=0; dim<3; ++dim)
      {
	int dim1 = (dim+1) % 3;
	int dim2 = (dim+2) % 3;

	for (int side1=0; side1<2; ++side1)
	  for (int side2=0; side2<2; ++side2)
	    if (ACreatedFaces[dim1][side1] || ACreatedFaces[dim2][side2])
	      {
		CVertex origin =
		  (side1-0.5)*BASE[dim1] + (side2-0.5)*BASE[dim2] - BASE[dim]/2;

		CMesh1BasicDiver diver(s[dim], origin, BASE[dim]);

		CDart * first =
		  ACreatedFaces[dim2][side2]
		  ?    AFacesCorners[dim2][side2][    0][side1]
		  : alpha1(AFacesCorners[dim1][side1][side2][    0]);

		diveMesh1(first, diver, GMV_NO, AMeshDimension);
	      }
      }

  // Faces:
  if (AMeshDimension==2)
    for (int dim=0; dim<3; ++dim)
      {
	int dim1 = (dim+1) % 3;
	int dim2 = (dim+2) % 3;

	for (int side=0; side<2; ++side)
	  if (ACreatedFaces[dim][side])
	    {
	      CVertex origin = (side-0.5)*BASE[dim] - (BASE[dim1]+BASE[dim2])/2;

	      CMesh2BasicDiver diver(s[dim1],s[dim2],
				     origin, BASE[dim1], BASE[dim2]);

	      CDart * first = AFacesCorners[dim][side][0][0];

	      diveMesh2(first, diver, GMV_NO);
	    }
      }
}
//******************************************************************************
void CGMapVertex::createCubeIMeshed(int ASx, int ASy, int ASz,
				    int AMeshDimension,
				    bool ACreatedFaces[3][2],
				    CDart ** ADart1, CDart ** ADart2)
{
  CDart * facesCorners[3][2][2][2];

  createCubeIMeshed(ASx, ASy, ASz, AMeshDimension, ACreatedFaces, facesCorners);

  int dim;

  for (dim=0; dim<3; ++dim)
    if (ACreatedFaces[dim][0] && ACreatedFaces[dim][1] &&
	!ACreatedFaces[(dim+1)%3][0] && !ACreatedFaces[(dim+1)%3][1] &&
	!ACreatedFaces[(dim+2)%3][0] && !ACreatedFaces[(dim+2)%3][1])
      {
	* ADart1 = facesCorners[dim][0][0][0];
	* ADart2 = facesCorners[dim][1][0][0];
	return;
      }

  * ADart2 = NULL;

  for (dim=0; dim<3; ++dim)
    for (int side=0; side<2; ++side)
      if (ACreatedFaces[dim][side])
	{
	  * ADart1 = facesCorners[dim][side][0][0];
	  return;
	}

  assert(false);
}
//******************************************************************************
