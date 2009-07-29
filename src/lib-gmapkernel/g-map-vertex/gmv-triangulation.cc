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
#include "plane.hh"
#include <list>
using namespace std;
using namespace GMap3d;
//******************************************************************************
CDart * CGMapVertex::triangulateEdge(CDart * ADart)
{
  assert(ADart!=NULL);
  // Inutile d'affecter un plongement au nouveau sommet créé, car
  // CGMapGeneric::triangulateEdge fait appel à la méthode
  // CGMapGeneric::insertVertex qui est virtuelle.
  // La méthode CGMapVertex::insertVertex est donc appelée.
  // C'est elle qui se charge de plonger le nouveau sommet.
  return CGMapGeneric::triangulateEdge(ADart);
}
//******************************************************************************
CDart * CGMapVertex::triangulateFace(CDart * ADart)
{
  assert(ADart!=NULL);

  CVertex bary = barycenter(ADart, ORBIT_01);
  CDart * center= CGMapGeneric::triangulateFace(ADart);
  setVertex(center, bary);

  return center;
}
//******************************************************************************
CDart * CGMapVertex::triangulateVolume(CDart * ADart)
{
  assert(ADart!=NULL);

  CVertex bary = barycenter(ADart, ORBIT_012);
  CDart * center= CGMapGeneric::triangulateVolume(ADart);
  setVertex(center, bary);

  return center;
}
//******************************************************************************
#define VTX(d) (AVertexDI < 0 ? findVertex(d) : \
		(CAttributeVertex*)getDirectInfo(d, AVertexDI))
//------------------------------------------------------------------------------
void CGMapVertex::triangulateGeoFace(CDart * AFace, int ANewEdgesMark,
				     int AVertexDI)
{
  if ( alpha101(AFace) == alpha010(AFace) )
    return;

  CVertex v1, v2, v3, pt, normal;
  CDart *face, *d1, *d2, *d3, *iter, *best;
  TCoordinate coef, best_coef;
  int mark = getNewMark();
  list<CDart*> new_edges;

  normal = faceNormalVector(AFace);

  face = AFace;
  while (alpha101(face) != alpha010(face)) {
    // Recherche du meilleur triangle
    d1 = face;
    best = NULL;
    best_coef = 0;
    do {
      d2 = alpha01(d1);
      d3 = alpha10(d1);

      v1 = *VTX(d1);
      v2 = *VTX(d2);
      v3 = *VTX(d3);
      
      if (((v2 - v1) * (v3 - v1)).dot(normal) > 0.0) {
	markOrbit(d1, ORBIT_VERTEX, mark);
	markOrbit(d2, ORBIT_VERTEX, mark);
	markOrbit(d3, ORBIT_VERTEX, mark);
	iter = alpha01(d2);
	while (iter != d3 &&
	       (isMarked(iter, mark) ||
		!isPointInTriangle(*VTX(iter), v1, v2, v3, normal)))
	  iter = alpha01(iter);
	unmarkOrbit(d1, ORBIT_VERTEX, mark);
	unmarkOrbit(d2, ORBIT_VERTEX, mark);
	unmarkOrbit(d3, ORBIT_VERTEX, mark);
	if (iter == d3) {
	  coef = getTriangleCoef(v1, v2, v3);
	  if (coef > best_coef) {
	    best_coef = coef;
	    best = d1;
	  }
	}
      }

      d1 = d2;
    }
    while(d1 != face);

    if (!best) {
      unmarkAll(1);
      setMark(face, 1);
      save("triangulateGeoFace.moka", BinaryFormat);
    }

    assert(best != NULL);

    // Ajout d'une arête
    d1 = best;
    d2 = alpha01(d1);
    d3 = alpha10(d1);
    insertEdge(alpha1(d2), d3);

    new_edges.push_back(alpha1(d3));
    if (ANewEdgesMark >= 0)
      markOrbit(alpha1(d3), ORBIT_EDGE, ANewEdgesMark);
    if (AVertexDI >= 0) {
      pointDirectInfoToAttributeVertex(AVertexDI, d2);
      pointDirectInfoToAttributeVertex(AVertexDI, d3);
    }
    face = d2;
    normal = faceNormalVector(face);
  }

  while (!new_edges.empty()) {
    if (shouldSwapEdge(new_edges.front(), AVertexDI))
      swapEdge(new_edges.front(), AVertexDI);
    new_edges.pop_front();
  }

  freeMark(mark);
}
//******************************************************************************
void CGMapVertex::triangulateMarkedFaces(int AMark,
					 int ANewEdgesMark,
					 int AVertexDI)
{
  bool closed;
  int mark = getNewMark();

  for (CDynamicCoverageAll dca(this); dca.cont(); ++dca)
    {
      if (isMarked(*dca, AMark) )
	{
	  if (!isMarked(*dca, mark))
	    {
	      closed = true;
	      
	      for (CDynamicCoverageFace dcf(this, *dca); dcf.cont(); ++dcf)
		{
		  setMark(*dcf, mark);
		  if (isFree0(*dcf) || isFree1(*dcf))
		    closed = false;
		}
	      
	      if (closed)
		{
		  triangulateGeoFace(*dca, ANewEdgesMark, AVertexDI);
		}
	    }
	}
      else
	setMark(*dca, mark);
    }

  unmarkAll(mark);
  freeMark(mark);
}
//******************************************************************************
bool CGMapVertex::shouldSwapEdge(CDart * AEdge, int AVertexDI)
{
  CVertex p[4];
  bool result = false;

  p[0] = *VTX(AEdge);
  p[1] = *VTX(alpha210(AEdge));
  p[2] = *VTX(alpha0(AEdge));
  p[3] = *VTX(alpha10(AEdge));

  CVertex v01 = p[1] - p[0];
  CVertex v03 = p[3] - p[0];
  CVertex v23 = p[3] - p[2];
  CVertex v21 = p[1] - p[2];

  CVertex v013 = v01 * v03;
  CVertex v231 = v23 * v21;

  CVertex n013 = CGeometry::getNormalVector(p[0],p[1], p[3]);
  CVertex n123 = CGeometry::getNormalVector(p[1], p[2], p[3]);

  if (v013.dot(v231) > 0 && !n013.isNull() && !n123.isNull()) {
    CVertex v120 = v21 * v01;
    CVertex v302 = v03 * v23;
    
    TCoordinate diff1 = fabs(v013.norm() - v231.norm());
    TCoordinate diff2 = fabs(v120.norm() - v302.norm());

    result = diff2 > diff1;
  }

  return result;
}
//******************************************************************************
bool CGMapVertex::swapEdge(CDart * AEdge, int AVertexDI)
{
  if ( isFree2(AEdge) || isFree1(AEdge) || isFree1(alpha0(AEdge)) ||
       isFree1(alpha2(AEdge)) || isFree1(alpha20(AEdge)) )
    return false;

  CDart *d[2] = {AEdge, alpha20(AEdge)};
  CDart *old[2] = {alpha1(d[0]), alpha1(d[1])};
  CDart *d1, *d2;

  for (int i = 0; i < 2; i++) {
    d1 = alpha1(d[i]);
    d2 = alpha21(d[i]);

    unsew1(d1);
    unsew1(d2);
    sew1(d1, d2);
    
    d1 = alpha0(d2);
    d2 = alpha1(d1);
    
    unsew1(d1);
    sew1(d[i], d1);
    sew1(alpha2(d[i]), d2);
  }

  if (AVertexDI >= 0)
    for (int i = 0; i < 2; i++)
      {
	pointDirectInfoToAttributeVertex(AVertexDI, d[i]);
	pointDirectInfoToAttributeVertex(AVertexDI, old[i]);
      }

  return true;
}
//******************************************************************************
bool CGMapVertex::isPointInTriangle(const CVertex & APoint,
				    const CVertex & AVertex1,
				    const CVertex & AVertex2,
				    const CVertex & AVertex3,
				    const CVertex & ANormal)
{
  CPlane plane1(ANormal * (AVertex2 - AVertex1), AVertex1);
  CPlane plane2(ANormal * (AVertex3 - AVertex2), AVertex2);
  CPlane plane3(ANormal * (AVertex1 - AVertex3), AVertex3);
  return (isPositive(plane1.pointDistance(APoint)) &&
	  isPositive(plane2.pointDistance(APoint)) &&
	  isPositive(plane3.pointDistance(APoint)));
}
//******************************************************************************
TCoordinate CGMapVertex::getTriangleCoef(const CVertex & AVertex1,
					 const CVertex & AVertex2,
					 const CVertex & AVertex3)
{
  if ( CGeometry::areColinear(AVertex1-AVertex2, AVertex2-AVertex3) )
    return 0.0;

  CVertex AB = AVertex2 - AVertex1;
  CVertex AC = AVertex3 - AVertex1;
  CVertex BC = AVertex3 - AVertex2;

  TCoordinate a = BC.norm();
  TCoordinate b = AC.norm();
  TCoordinate c = AB.norm();

  CPlane plane(AB, (AVertex1 + AVertex2) / 2.0);
  CVertex line = (AB * AC) * BC;
  CVertex pt;

  if (!plane.getLineIntersection((AVertex2 + AVertex3) / 2.0, line, &pt)) {
    cerr << "CTriangulationTools::getTriangleCoef: "
	 << "Erreur dans le calcul du coefficient !" << endl
	 << AVertex1 << endl
	 << AVertex2 << endl
	 << AVertex3 << endl;
    assert(false);
  }

  TCoordinate R = (AVertex1 - pt).norm();
  TCoordinate coef = (a*b*c) / (4 * M_PI * R*R*R);
  assert(coef >= 0.0 && coef < 1.0);
  return coef;
}
//------------------------------------------------------------------------------
#undef VTX
//******************************************************************************

