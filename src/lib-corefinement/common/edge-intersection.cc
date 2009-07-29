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

#include "edge-intersection.hh"

#include INCLUDE_NON_INLINE("edge-intersection.icc")

using namespace std;
using namespace GMap3d;

//******************************************************************************

ostream & GMap3d::operator << (ostream & AStream, TPositionOnEdge APos)
{
  switch (APos) {
  case EP_OnFirstVertex:
    AStream << "OnFirstVertex";
    break;
  case EP_OnSecondVertex:
    AStream << "OnSecondVertex";
    break;
  case EP_OnEdge:
    AStream << "OnEdge";
    break;
  case EP_OutOfEdge:
    AStream << "OutOfEdge";
    break;
  default:
    AStream << "Unknown";
    break;
  }

  return (AStream);
}

//******************************************************************************

ostream & GMap3d::operator << (ostream & AStream,
				               const CEdgeIntersection & AInter)
{
  AStream << "Cell      : " << AInter.getCell() << endl
	      << "Parameter : " << AInter.getParameter() << endl
	      << "Point     : " << AInter.getPoint() << endl
	      << "Dimension : " << AInter.getCellDimension() << endl
	      << "Position  : " << AInter.getPosition() << endl;

  return AStream;
}

//******************************************************************************
