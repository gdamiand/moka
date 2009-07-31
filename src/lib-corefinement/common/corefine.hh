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

#ifndef COREFINE_HH
#define COREFINE_HH

#include "g-map-vertex.hh"

namespace GMap3d
{

  class CCorefine
  {
  public:

    CCorefine(CGMapVertex * AMap, TCoordinate AEpsilon = 1E-4);
    virtual ~CCorefine();

    virtual int corefine(CDart *& AMesh1, CDart *& AMesh2,
			 std::bitset<NB_MARKS> ACopyMarks = 0) = 0;

  protected:
  
    CGMapVertex *FMap;
    TCoordinate FEps;
    std::bitset<NB_MARKS> FCopyMarks;
  };

} // namespace GMap3d;

#endif
