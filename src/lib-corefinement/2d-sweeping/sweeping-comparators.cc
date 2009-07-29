/*
 * lib-corefinement : Opérations de corafinement.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-corefinement
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
#include "inline-macro.hh"
#include "sweeping-comparators.hh"
#include "vertex.hh"
#include "dart-vertex.hh"
#include "geometry.hh"
#include "g-map-vertex.hh"
#include <cassert>
using namespace GMap3d;
//******************************************************************************
CDartCompare::CDartCompare(CGMapVertex * AMap,
			   int ADirectVertex, int AExtremity1,
			   const CVertex & AZVector)
{
  assert(AMap!=NULL);
  assert(0 <= ADirectVertex);
  assert(0 <= AExtremity1);
  assert(!AZVector.isNull());

  FMap = AMap;

  FDirectVertex = ADirectVertex;
  FExtremity1 = AExtremity1;

  if (fabs(AZVector.getX()) > fabs(AZVector.getY()))
    {
      if (fabs(AZVector.getX()) > fabs(AZVector.getZ()))
	{
	  FCoord1 = 1; // OY
	  FCoord2 = 2; // OZ
	}
      else
	{
	  FCoord1 = 0; // OX
	  FCoord2 = 1; // OY
	}
    }
  else
    {
      if (fabs(AZVector.getY()) > fabs(AZVector.getZ()))
	{
	  FCoord1 = 0; // OX
	  FCoord2 = 2; // OZ
	}
      else
	{
	  FCoord1 = 0; // OX
	  FCoord2 = 1; // OY
	}
    }
}
//******************************************************************************
CDartLexicoCompare::CDartLexicoCompare(CGMapVertex * AMap,
				       int ADirectVertex, int AExtremity1,
				       const CVertex & AZVector,
				       bool AExactComparison)
  : CDartCompare(AMap, ADirectVertex, AExtremity1, AZVector)
{
  FExactComparison = AExactComparison;
}
//******************************************************************************
CDartVertexerticalCompare::CDartVertexerticalCompare(CGMapVertex * AMap,
					   int ADirectVertex, int AExtremity1,
					   const CVertex & AZVector)
  : CDartCompare(AMap, ADirectVertex, AExtremity1, AZVector)
{
}
//******************************************************************************
TCoordinate CDartVertexerticalCompare::FX = 0 ;
//******************************************************************************
CDartAngularCompare::CDartAngularCompare(CGMapVertex * AMap,
					 int ADirectVertex,
					 const CVertex & AZVector)
  : CDartCompare(AMap, ADirectVertex, 0, AZVector)
{
  FZVector = AZVector;
}
//******************************************************************************
CIntersection::CIntersection(const TCoordinate & ALambda, int AIdentity)
{
  FLambda = ALambda;
  FIdentity = AIdentity;
}
//******************************************************************************
CDartLinearCompare::CDartLinearCompare()
{
}
//******************************************************************************
#define GET_VERTEX(DART) (FMap-> \
                          getDirectInfoAsAttributeVertex(DART, FDirectVertex))
//******************************************************************************
void CDartCompare::project(const CVertex & AVertex,
			   TCoordinate & AX, TCoordinate & AY) const
{
  AX = AVertex.getCoord(FCoord1);
  AY = AVertex.getCoord(FCoord2);
}
//******************************************************************************
bool CDartLexicoCompare::operator()
  (CDartVertex * ADart1, CDartVertex * ADart2) const
{
  if (ADart1==ADart2)
    return false;

  // Ces variables sont déclarées statiques pour éviter que le constructeur par
  // défaut de TCoordinate (quand GMP est utilisé) soit appelé à chaque appel à
  // la méthode:
  static TCoordinate x1, y1, x2, y2;

  project(* GET_VERTEX(ADart1), x1, y1);
  project(* GET_VERTEX(ADart2), x2, y2);

  // Comparaison lexicographique:
  if (FExactComparison)
    {
      if (x1 != x2)
	return x1 < x2;

      if (y1 != y2)
	return y1 < y2;
    }
  else
    {
      if (!areEqual(x1, x2))
	return x1 < x2;

      if (!areEqual(y1, y2))
	return y1 < y2;
    }

  // Cas où les extrémités sont confondues:
  bool in1 = FMap->isMarked(ADart1, FExtremity1);
  bool in2 = FMap->isMarked(ADart2, FExtremity1);

  // Si une extrémité est sortante et l'autre entrante,
  // l'extrémité entrante alpha priorité sur l'extrémité sortante
  // (pour éviter des problèmes sur les arêtes de longueur nulle):
  if (in1 != in2)
    return in1;

  // Les deux extrémités sont confondues et de même type.
  // On compare les pointeurs!!
  return ADart1 < ADart2;
}
//******************************************************************************
bool CDartVertexerticalCompare::operator()
  (CDartVertex * ADart1, CDartVertex * ADart2) const
{
  if (ADart1==ADart2)
    return false;

  // Ces variables sont déclarées statiques pour éviter que le constructeur par
  // défaut de TCoordinate soit appelé à chaque appel à la méthode:
  static TCoordinate xA,yA, xB,yB, xC,yC, xD,yD;

  project(* GET_VERTEX(         ADart1 ), xA, yA);
  project(* GET_VERTEX(FMap->alpha0(ADart1)), xB, yB);
  project(* GET_VERTEX(         ADart2 ), xC, yC);
  project(* GET_VERTEX(FMap->alpha0(ADart2)), xD, yD);

  // Delta X:
  TCoordinate dxAB = xB - xA;
  TCoordinate dxCD = xD - xC;

  // Delta Y:
  TCoordinate dyAB = yB - yA;
  TCoordinate dyCD = yD - yC;

  // t:
  TCoordinate tAB = isZero(dxAB) ? 0.0 : (FX - xA) / dxAB;
  TCoordinate tCD = isZero(dxCD) ? 0.0 : (FX - xC) / dxCD;

  TCoordinate yAB = yA + tAB * dyAB;
  TCoordinate yCD = yC + tCD * dyCD;

  // Cas où les ordonnées sont distinctes:
  if (!areEqual(yAB, yCD))
    return yAB < yCD;

  // Cas où les ordonnées sont égales:
  bool in1 = FMap->isMarked(ADart1, FExtremity1);
  bool in2 = FMap->isMarked(ADart2, FExtremity1);

  // Si une extrémité est sortante et l'autre entrante,
  // l'extrémité entrante alpha priorité sur l'extrémité sortante:
  if (in1 != in2)
    return in1;

  // Sinon on regarde l'inclinaison des segments.
  assert(areEqual(tAB, 0.0) || areEqual(tAB, 1.0) ||
	 areEqual(tCD, 0.0) || areEqual(tCD, 1.0) );

  bool inverseAB = areEqual(tAB, 1.0);
  bool inverseCD = areEqual(tCD, 1.0);

  bool verticalAB = isZero(dxAB);
  bool verticalCD = isZero(dxCD);

  if (verticalAB || verticalCD)
    {
      if (!verticalAB)
	return true;

      if (!verticalCD)
	return false;

      return ADart1 < ADart2;
    }

  TCoordinate tanAB = dyAB / dxAB;
  TCoordinate tanCD = dyCD / dxCD;

  if (inverseAB) tanAB = - tanAB;
  if (inverseCD) tanCD = - tanCD;

  if (areEqual(tanAB, tanCD))
    return ADart1 < ADart2;

  return tanAB < tanCD;
}
//******************************************************************************
void CDartVertexerticalCompare::setCurrentPoint(const CVertex & AVertex)
{
  // Cette variable est déclarée statique pour éviter que le constructeur par
  // défaut de TCoordinate soit appelé à chaque appel à la méthode:
  static TCoordinate y;
  project(AVertex, FX, y);
}
//******************************************************************************
bool CDartAngularCompare::operator()
  (CDartVertex * ADart1, CDartVertex * ADart2) const
{
  if (ADart1==ADart2)
    return false;

  const CVertex O =
    (* GET_VERTEX(ADart1) + * GET_VERTEX(ADart2)) / 2;

  const CVertex & A = * GET_VERTEX(FMap->alpha0(ADart1));
  const CVertex & B = * GET_VERTEX(FMap->alpha0(ADart2));

  TCoordinate angle = CGeometry::getAngle(A-O, B-O, FZVector);

  // if (isZero(angle))
  //   return ADart1 < ADart2;

  return angle < 0;
}
//******************************************************************************
#undef GET_VERTEX
//******************************************************************************
bool CDartLinearCompare::operator()(CIntersection * AIntersection1,
				    CIntersection * AIntersection2) const
{
  assert(AIntersection1->FIdentity != AIntersection2->FIdentity);

  if (areEqual(AIntersection1->FLambda, AIntersection2->FLambda))
    return AIntersection1->FIdentity < AIntersection2->FIdentity;

  return AIntersection1->FLambda < AIntersection2->FLambda;
}
//******************************************************************************
