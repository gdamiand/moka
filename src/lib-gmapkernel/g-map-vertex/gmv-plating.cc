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
using namespace GMap3d;
//******************************************************************************
bool CGMapVertex::canPlate(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1!=NULL && ADart2!=NULL);
  return !isSameOrbit(ADart1,ADart2, ORBIT_CC);
}
//******************************************************************************
int CGMapVertex::getPlateDimension(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1!=NULL && ADart2!=NULL);

  if (!canPlate(ADart1,ADart2))
    return -1;

  if (isFree1(ADart1) && isFree1(ADart2)) return 0;
  if (isFree2(ADart1) && isFree2(ADart2)) return 1;
  if (isFree3(ADart1) && isFree3(ADart2)) return 2;

  return -1;
}
//******************************************************************************
void CGMapVertex::plate(CDart * ADart1, CDart * ADart2, int ADim,
			bool ARotateCells,
			bool AScaleCells,
			bool ATranslateCells)
{
  assert(ADart1!=NULL && ADart2!=NULL);
  assert(ADim>=0 && ADim<3);
  assert(canPlate(ADart1,ADart2));

  CVertex origin1, origin2;
  CVertex U1,V1, U2,V2;
  TCoordinate  kHomothety;

  // Calcul des origines:
  origin1= barycenter(ADart1, ORBIT_INF[ADim]);

  if (ATranslateCells)
    origin2= barycenter(ADart2, ORBIT_INF[ADim]);
  else
    origin2= origin1;

  // Calcul des vecteurs de chaque repère:
  if (ADim==0 || !ARotateCells)
    {
      U1= U2= OZ;
      V1= V2= OX;
    }
  else
    {
      U1= faceNormalVector(ADart1); if (U1.isNull()) U1= OZ;
      U2= faceNormalVector(ADart2); if (U2.isNull()) U2= OZ;

      V1= barycenter(ADart1, ORBIT_EDGE) - barycenter(ADart1, ORBIT_FACE);
      if (V1.isNull()) V1= OX;
      V2= barycenter(ADart2, ORBIT_EDGE) - barycenter(ADart2, ORBIT_FACE);
      if (V2.isNull()) V2= OX;

      if ((U1*V1).isNull() || (U2*V2).isNull())
	{ U1= U2= OZ; V1= V2= OX; }
      else
	if (ADim==1)
	  { U2= -U2; V2= -V2; }
    }

  // Calcul du coefficient d'homothétie:
  if (ADim==0 || !AScaleCells)
    kHomothety = 1;
  else
    {
      TCoordinate length1=
	ADim==1 ? edgeLength(ADart1) : facePerimeter(ADart1);

      TCoordinate length2=
	ADim==1 ? edgeLength(ADart2) : facePerimeter(ADart2);

      kHomothety = isZero(length1) ? 1 : length2/length1;
    }

  {
    // Initialisation de la matrice de placage:
    CTransformationMatrix platingMatrix(CTransformationMatrix::IdentityMatrix);

    platingMatrix.transform(origin1, U1, V1,
			    origin2, U2, V2,
			    CVertex(kHomothety, kHomothety, kHomothety));

    // Application de la transformation globale sur l'objet:
    applyMatrix(platingMatrix, ADart1,ORBIT_CC);
  }
}
//******************************************************************************
bool CGMapVertex::intuitivePlate(CDart * ADart1, CDart * ADart2,
				 bool ARotateCells, bool AScaleCells,
				 bool ATranslateCells)
{
  assert(ADart1!=NULL && ADart2!=NULL);

  int dim= getPlateDimension(ADart1,ADart2);

  if (dim>0)
    plate(ADart1,ADart2, dim, ARotateCells, AScaleCells, ATranslateCells);

  return dim>0;
}
//******************************************************************************
void CGMapVertex::borderPlate(CDart * ADart1, CDart * ADart2, int ADim,
			      bool ARotateCells, bool AScaleCells,
			      bool ATranslateCells)
{
  assert(ADart1!=NULL && ADart2!=NULL);
  assert(ADim>=0 && ADim<3);

  assert(canPlate(ADart1,ADart2));

  CVertex origin1, origin2;
  CVertex U1,V1, U2,V2;
  TCoordinate  kHomothety;

  // Calcul des origines:
  origin1= barycenter(ADart1, ORBIT_BORDER[ADim]);

  if (ATranslateCells)
    origin2= barycenter(ADart2, ORBIT_BORDER[ADim]);
  else
    origin2= origin1;

  // Calcul des vecteurs de chaque repère:
  if (ADim==0 || !ARotateCells)
    {
      U1= U2= OZ;
      V1= V2= OX;
    }
  else
    {
      if (ADim==1)
	{
	  U1= faceNormalVector(ADart1);
	  U2= faceNormalVector(ADart2);
	  V1= barycenter(ADart1, ORBIT_BORDER_1)
	    - barycenter(ADart1, ORBIT_FACE);
	  V2= barycenter(ADart2, ORBIT_BORDER_1)
	    - barycenter(ADart2, ORBIT_FACE);
	}
      else
	{
	  U1= border2NormalVector(ADart1);
	  U2= border2NormalVector(ADart2);
	  V1= barycenter(ADart1, ORBIT_EDGE)
	    - barycenter(ADart1, ORBIT_BORDER[ADim]);
	  V2= barycenter(ADart2, ORBIT_EDGE)
	    - barycenter(ADart2, ORBIT_BORDER[ADim]);
	}

      if (U1.isNull()) U1= OZ;
      if (U2.isNull()) U2= OZ;

      if (V1.isNull()) V1= OX;
      if (V2.isNull()) V2= OX;

      if ((U1*V1).isNull() || (U2*V2).isNull())
	{ U1= U2= OZ; V1= V2= OX; }
      else
	if (ADim==1)
	  { U2= -U2; V2= -V2; }
    }

  // Calcul du coefficient d'homothétie:
  if (ADim==0 || !AScaleCells)
    kHomothety = 1;
  else
    {
      TCoordinate length1= orbitLength(ADart1, ORBIT_BORDER[ADim]);
      TCoordinate length2= orbitLength(ADart2, ORBIT_BORDER[ADim]);

      kHomothety = isZero(length1) ? 1 : length2/length1;
    }

  {
    // Initialisation de la matrice de placage:
    CTransformationMatrix platingMatrix(CTransformationMatrix::IdentityMatrix);

    platingMatrix.transform(origin1, U1, V1,
			    origin2, U2, V2,
			    CVertex(kHomothety, kHomothety, kHomothety));

    // Application de la transformation globale sur l'objet:
    applyMatrix(platingMatrix, ADart1,ORBIT_CC);
  }
}
//******************************************************************************
