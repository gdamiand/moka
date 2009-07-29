/*
 * lib-tools : Divers outils autour de lib-gmapkernel.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-tools
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

#include "bounding-box.hh"
using namespace std;
//******************************************************************************
// Epsilon utilisé pour les tests d'appartenance et de comparaison
static TCoordinate BB_EPS = 0;
//******************************************************************************
CBoundingBox::CBoundingBox()
  : FMin(CVertex(0, 0, 0)), FMax(CVertex(0, 0, 0)), FIsEmpty(true)
{
}
//******************************************************************************
CBoundingBox::CBoundingBox(const CVertex & APoint)
  : FMin(APoint), FMax(APoint), FIsEmpty(false)
{
}
//******************************************************************************
CBoundingBox::CBoundingBox(const CVertex & APoint1, const CVertex & APoint2)
  : FIsEmpty(false)
{
  if (APoint1.getX() < APoint2.getX()) {
    FMin.setX(APoint1.getX());
    FMax.setX(APoint2.getX());
  }
  else {
    FMin.setX(APoint2.getX());
    FMax.setX(APoint1.getX());
  }

  if (APoint1.getY() < APoint2.getY()) {
    FMin.setY(APoint1.getY());
    FMax.setY(APoint2.getY());
  }
  else {
    FMin.setY(APoint2.getY());
    FMax.setY(APoint1.getY());
  }

  if (APoint1.getZ() < APoint2.getZ()) {
    FMin.setZ(APoint1.getZ());
    FMax.setZ(APoint2.getZ());
  }
  else {
    FMin.setZ(APoint2.getZ());
    FMax.setZ(APoint1.getZ());
  }
}
//******************************************************************************
TCoordinate CBoundingBox::getEpsilon()
{
  return BB_EPS;
}
//******************************************************************************
void CBoundingBox::setEpsilon(TCoordinate AEpsilon)
{
  BB_EPS = AEpsilon;
}
//******************************************************************************
void CBoundingBox::addPoint(const CVertex & APoint)
{
  if (FIsEmpty)
    FMin = FMax = APoint;
  else {
    if (APoint.getX() < FMin.getX())
      FMin.setX(APoint.getX());
    if (APoint.getY() < FMin.getY())
      FMin.setY(APoint.getY());
    if (APoint.getZ() < FMin.getZ())
      FMin.setZ(APoint.getZ());
    if (APoint.getX() > FMax.getX())
      FMax.setX(APoint.getX());
    if (APoint.getY() > FMax.getY())
      FMax.setY(APoint.getY());
    if (APoint.getZ() > FMax.getZ())
      FMax.setZ(APoint.getZ());
  }
  FIsEmpty = false;
}
//******************************************************************************
void CBoundingBox::clear()
{
  FMin = FMax = CVertex(0, 0, 0);
  FIsEmpty = true;
}
//******************************************************************************
bool CBoundingBox::isEmpty() const
{
  return FIsEmpty;
}
//******************************************************************************
bool CBoundingBox::isInBox(const CVertex & APoint) const
{
  if (FIsEmpty)
    return false;
  else
    return (APoint.getX() >= FMin.getX() - BB_EPS &&
	    APoint.getX() <= FMax.getX() + BB_EPS &&
	    APoint.getY() >= FMin.getY() - BB_EPS &&
	    APoint.getY() <= FMax.getY() + BB_EPS &&
	    APoint.getZ() >= FMin.getZ() - BB_EPS &&
	    APoint.getZ() <= FMax.getZ() + BB_EPS);
}
//******************************************************************************
bool CBoundingBox::isInIntersectionWith(const CBoundingBox & ABB) const
{
  if (FIsEmpty || ABB.FIsEmpty)
    return false;
  else
    return (FMin.getX() - BB_EPS <= ABB.FMax.getX() + BB_EPS &&
	    FMin.getY() - BB_EPS <= ABB.FMax.getY() + BB_EPS &&
	    FMin.getZ() - BB_EPS <= ABB.FMax.getZ() + BB_EPS &&
	    FMax.getX() + BB_EPS >= ABB.FMin.getX() - BB_EPS &&
	    FMax.getY() + BB_EPS >= ABB.FMin.getY() - BB_EPS &&
	    FMax.getZ() + BB_EPS >= ABB.FMin.getZ() - BB_EPS);
}
//******************************************************************************
TCoordinate CBoundingBox::getVolume() const
{
  CVertex vol = FMax - FMin;

  return vol.getX() * vol.getY() * vol.getZ();
}
//******************************************************************************
TCoordinate CBoundingBox::getSurface() const
{
  CVertex vol = FMax - FMin;
  TCoordinate surf;

  surf = vol.getX() * vol.getY();
  surf += vol.getX() * vol.getZ();
  surf += vol.getY() * vol.getZ();

  return surf * 2.0;
}
//******************************************************************************
const CVertex & CBoundingBox::getMinBound() const
{
  return FMin;
}
//******************************************************************************
const CVertex & CBoundingBox::getMaxBound() const
{
  return FMax;
}
//******************************************************************************
CVertex CBoundingBox::getEpsMinBound() const
{
  return FMin - CVertex(BB_EPS, BB_EPS, BB_EPS);
}
//******************************************************************************
CVertex CBoundingBox::getEpsMaxBound() const
{
  return FMax + CVertex(BB_EPS, BB_EPS, BB_EPS);
}
//******************************************************************************
CVertex CBoundingBox::getCenter() const
{
  return (FMin + FMax) / 2.0;
}
//******************************************************************************
bool CBoundingBox::operator * (const CBoundingBox & AOther) const
{
  return isInIntersectionWith(AOther);
}
//******************************************************************************
CBoundingBox CBoundingBox::operator + (const CBoundingBox & AOther) const
{
  CBoundingBox box = *this;
  box.addPoint(AOther.getMinBound());
  box.addPoint(AOther.getMaxBound());
  return box;
}
//******************************************************************************
ostream & operator << (ostream & AStream, const CBoundingBox & ABB)
{
  if (ABB.isEmpty())
    AStream << "[empty]";
  else
    AStream << "[" << ABB.getMinBound() << "|" << ABB.getMaxBound() << "]";

  return AStream;
}
//******************************************************************************
