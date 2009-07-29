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

#include "face-intersection-tools.hh"

using namespace std;

namespace GMap3d {

//******************************************************************************

ostream & operator << (ostream & AStream, TPositionInFace APos)
{
  switch (APos) {
  case FP_Outside:          AStream << "Outside";          break;
  case FP_Inside:           AStream << "Inside";           break;
  case FP_OnBorder:         AStream << "OnBorder";         break;
  case FP_OnReversedBorder: AStream << "OnReversedBorder"; break;
  }
  return AStream;
}

//******************************************************************************

}
