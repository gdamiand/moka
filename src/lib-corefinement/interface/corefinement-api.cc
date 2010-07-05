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
#include "corefinement-api.hh"
#include "g-map-vertex.hh"
#include "time.hh"

// 2D Meshes:
#include "corefine-2d-sweeping.hh"
#include "corefine-2d-propagation.hh"
#include "boolean-operations-2d.hh"
// 3D:
#include "corefine-3d.hh"
#include "corefine-3d-face-face.hh"
#include "boolean-operations-3d.hh"
using namespace std;
using namespace GMap3d;
//******************************************************************************
CCorefinementAPI::CCorefinementAPI(CGMapVertex * AMap)
{
  assert(AMap != NULL);

  FMap = AMap;
}
//******************************************************************************
CCorefinementAPI::~CCorefinementAPI()
{
}
//******************************************************************************
void CCorefinementAPI::corefine2dMeshesSweeping(CDart * ADart1, CDart * ADart2,
						const CVertex& ANormal)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);

  CCorefineSegmentsSweeping(FMap).corefine(ADart1, ADart2, ANormal);
}
//******************************************************************************
void CCorefinementAPI::corefine2dMeshesPropagation(CDart * ADart1,
						   CDart * ADart2,
						   const CVertex & ANormal,
						   bool /*useBBox*/)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);

  CTime start, end;

  cout << endl << "Co-raffinement des maillages" << endl;
  start.updateTime();

  CVertex * vertex = FMap->findVertex(ADart1);
  CPlane plane(ANormal, *vertex);

  CCorefine2dPropagation(FMap, plane).corefine(ADart1, ADart2);

  end.updateTime();
  cout << "Durée du co-raffinement : " << end - start << endl;
}
//******************************************************************************
void CCorefinementAPI::corefine3dMeshes(CDart * ADart1, CDart * ADart2)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);

  CTime start, end;

  start.updateTime();

  cout << endl << "Co-raffinement 3D des maillages" << endl;
//   CCorefine3d(FMap, false).corefine(ADart1, ADart2);

  CCorefine3dFF coref(FMap, false);
  coref.splitMeshes(ADart1, ADart2);

  end.updateTime();
  cout << "Durée du co-raffinement : " << end - start << endl;
}
//******************************************************************************
void CCorefinementAPI::corefineMarked3dFaces(int AMark1, int AMark2)
{
  CTime start, end;

  cout << endl << "Co-raffinement 3D des faces" << endl;
  start.updateTime();

  CCorefine3dFF coref(FMap, false);
  coref.corefine(AMark1, AMark2);

  end.updateTime();
  cout << "Durée du co-raffinement : " << end - start << endl;
}
//******************************************************************************
void CCorefinementAPI::corefineMarked3dMeshesWith(int AMark, CDart * ADart)
{
  assert(ADart != NULL);

  int vertex_di = FMap->getNewDirectInfo();
  CCorefine3dFF coref(FMap, false, 1E-4, vertex_di);
  CCorefine3dTools tools(FMap);
  list<CDart*> objects;
  CDart *d1, *d2;
  CTime start, end;

  start.updateTime();

  FMap->pointDirectInfoToAttributeVertex(vertex_di);

  int tmp_mark = FMap->getNewMark();

  d2 = tools.findWellOrientedDart(ADart, vertex_di);
  FMap->markOrbit(d2, ORBIT_CC, tmp_mark);

  for (CDynamicCoverageAll dca(FMap); dca.cont(); ++dca) {
    if (FMap->isMarked(*dca, AMark) && !FMap->isMarked(*dca, tmp_mark)) {
      d1 = tools.findWellOrientedDart(*dca, vertex_di);
      FMap->markOrbit(d1, ORBIT_CC, tmp_mark);
      objects.push_back(d1);
    }
  }
  
  FMap->unmarkAll(tmp_mark);
  FMap->freeMark(tmp_mark);

  while (!objects.empty()) {
    d1 = objects.front(), objects.pop_front();

    coref.corefine(d1, d2);
  }
  
  FMap->freeDirectInfo(vertex_di);

  end.updateTime();
  cout << "Durée total du calcul : " << end - start << endl;
}
//******************************************************************************
void CCorefinementAPI::booleanOperations2d(CDart * ADart1,
					   CDart * ADart2,
					   const CVertex & /*ANormal*/,
					   int ADifference1Mark,
					   int ADifference2Mark,
					   int AIntersectionMark,
					   int AUnionMark)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);

  CTime start, end;

  cout << endl << "Co-raffinement des maillages" << endl;
  start.updateTime();

  CBooleanOperations2d ops(FMap, ADart1, ADart2, true);

  ops.computeResults();

  end.updateTime();
  cout << "Durée du co-raffinement : " << end - start << endl;

  cout << "Extraction des opérations booléennes" << endl;
  start.updateTime();

  if (ADifference1Mark >= 0) {
    FMap->unmarkAll(ADifference1Mark);
    ops.markResult(BO_Difference1, ADifference1Mark);
  }

  if (ADifference2Mark >= 0) {
    FMap->unmarkAll(ADifference2Mark);
    ops.markResult(BO_Difference2, ADifference2Mark);
  }

  if (AIntersectionMark >= 0) {
    FMap->unmarkAll(AIntersectionMark);
    ops.markResult(BO_Intersection, AIntersectionMark);
  }

  if (AUnionMark >= 0) {
    FMap->unmarkAll(AUnionMark);
    ops.markResult(BO_Union, AUnionMark);
  }

  end.updateTime();
  cout << "Durée de l'extraction : " << end - start << endl;
}
//******************************************************************************
void CCorefinementAPI::booleanOperations3d(CDart * ADart1, CDart * ADart2,
					   int ADifference1Mark,
					   int ADifference2Mark,
					   int AIntersectionMark,
					   int AUnionMark)
{
  assert(ADart1 != NULL);
  assert(ADart2 != NULL);

  CTime start, end, total;

  cout << endl << "Co-raffinement des maillages" << endl;
  start.updateTime();
  total.updateTime();

  CBooleanOperations3d ops(FMap, ADart1, ADart2, false);

  ops.computeResults();

  end.updateTime();
  cout << "Durée du co-raffinement : " << end - start << endl;

  cout << "Extraction des opérations booléennes" << endl;
  start.updateTime();

  if (ADifference1Mark >= 0) {
    FMap->unmarkAll(ADifference1Mark);
    ops.markResult(BO_Difference1, ADifference1Mark);
  }

  if (ADifference2Mark >= 0) {
    FMap->unmarkAll(ADifference2Mark);
    ops.markResult(BO_Difference2, ADifference2Mark);
  }

  if (AIntersectionMark >= 0) {
    FMap->unmarkAll(AIntersectionMark);
    ops.markResult(BO_Intersection, AIntersectionMark);
  }

  if (AUnionMark >= 0) {
    FMap->unmarkAll(AUnionMark);
    ops.markResult(BO_Union, AUnionMark);
  }

  end.updateTime();
  cout << "Durée de l'extraction : " << end - start << endl;
  cout << "Durée totale : " << end - total << endl;
}
//******************************************************************************
