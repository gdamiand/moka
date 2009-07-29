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

// #define DEBUG_MESSAGES
#include "message-macros.hh"

#include "boolean-operations-3d.hh"
#include "corefine-3d-face-face.hh"

using namespace std;
using namespace GMap3d;

//******************************************************************************

CBooleanOperations3d::CBooleanOperations3d(CGMapVertex * AMap,
					   CDart * AObject1,
					   CDart * AObject2,
					   bool ACalculateOrientation,
					   int AVertexDI)
  : CBooleanOperations(AMap, AObject1, AObject2,
		       ACalculateOrientation, AVertexDI),
    FDisplayMessages(2)
{
}

//******************************************************************************

CBooleanOperations3d::~CBooleanOperations3d()
{
}

//******************************************************************************

bool CBooleanOperations3d::corefineObjects(bitset<NB_MARKS> ACopyMarks)
{
  DEBUG_FUNCTION;

  CDart *d1 = getObject1(), *d2 = getObject2();
  CCorefine3dFF c(getMap(), calculateOrientation(), 1E-4, getVertexDI());

  c.setDisplayMessagesLevel(FDisplayMessages);
  int nb = c.corefine(d1, d2, ACopyMarks);
  if (!nb) c.linkCompounds(d1, d2, ACopyMarks);

  setObject1(d1);
  setObject2(d2);
  
  return (nb > 0);
}

//******************************************************************************
#define a0(d) (getMap()->alpha0(d))
#define a1(d) (getMap()->alpha1(d))
#define a2(d) (getMap()->alpha2(d))
#define a3(d) (getMap()->alpha3(d))

void CBooleanOperations3d::extendMarks()
{
  DEBUG_FUNCTION;

//   getMap()->markIncidentCells(ORBIT_012, getObject1Mark());
//   getMap()->markIncidentCells(ORBIT_012, getObject2Mark());

  list<CDart*> dart_list[2];
  int mark[2] = {getObject1Mark(), getObject2Mark()};
  CDart *d;

  for (CDynamicCoverageCC dcc(getMap(), getObject1()); dcc.cont(); ++dcc) {
    for (int i = 0; i < 2; i++)
      if (getMap()->isMarked(*dcc, mark[i]) &&
	  !getMap()->isMarked(a2(*dcc), mark[i]))
	dart_list[i].push_back(a2(*dcc));
  }

  for (int i = 0; i < 2; i++)
    while (!dart_list[i].empty()) {
      d = dart_list[i].front(), dart_list[i].pop_front();
      
      if (!getMap()->isMarked(d, mark[i])) {
	getMap()->setMark(d, mark[i]);
	
	if (!getMap()->isMarked(a0(d), mark[i]))
	  dart_list[i].push_back(a0(d));
	
	if (!getMap()->isMarked(a1(d), mark[i]))
	  dart_list[i].push_back(a1(d));
	
	if (!getMap()->isMarked(a2(d), mark[i]))
	  dart_list[i].push_back(a2(d));
	
	if ((!getMap()->isMarked(d, mark[(i + 1) % 2]) ||
	     getMap()->isMarked(a3(d), mark[(i + 1) % 2])) &&
	    !getMap()->isMarked(a3(d), mark[i]))
	  dart_list[i].push_back(a3(d));
      }
    }
}

#undef a0
#undef a1
#undef a2
#undef a3
//******************************************************************************
