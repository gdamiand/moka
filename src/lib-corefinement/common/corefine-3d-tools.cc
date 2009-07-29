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

#include "corefine-3d-tools.hh"

// #define DEBUG_MESSAGES
#include "message-macros.hh"

using namespace std;
using namespace GMap3d;

//******************************************************************************

#define a0 FMap->alpha0
#define a1 FMap->alpha1
#define a2 FMap->alpha2
#define a3 FMap->alpha3

#define VTX(d) (AVertexDI < 0 ? FMap->findVertex(d) : \
		(CAttributeVertex*)FMap->getDirectInfo(d, AVertexDI))

//******************************************************************************

CCorefine3dTools::CCorefine3dTools(CGMapVertex * AMap,
				   TCoordinate AEpsilon)
  : CCorefine2dTools(AMap, AEpsilon)
{
}

//******************************************************************************

bool CCorefine3dTools::isVectorInSector(const CVertex & AVector,
					const CVertex & ASectorVector1,
					const CVertex & ASectorVector2,
					const CPlane & APlane,
					bool AThickVector1,
					bool AThickVector2)
{
  TCoordinate pv = (ASectorVector1 * ASectorVector2).dot(APlane.getNormal());
  TCoordinate pv1 = (ASectorVector1 * AVector).dot(APlane.getNormal());
  TCoordinate pv2 = (ASectorVector2 * AVector).dot(APlane.getNormal());
  bool result = pv < 0.0 ? pv1 >= 0.0 || pv2 <= 0.0 : pv1 >= 0.0 && pv2 <= 0.0;

  if (!result && AThickVector1 && pv1 >= -FEps &&
      AVector.dot(ASectorVector1) > 0.0) result = true;
  if (!result && AThickVector2 && pv2 <= FEps &&
      AVector.dot(ASectorVector2) > 0.0) result = true;

  return result;
}

//******************************************************************************

bool CCorefine3dTools::isVectorInSector(const CVertex & AVector,
					CDart * ASector,
					const CPlane & APlane,
					bool ATestBorders,
					int AVertexDI)
{
  CVertex pt = *VTX(ASector);

  if (a2(ASector) != a1(ASector))
    if (!ATestBorders)
      return isVectorInSector(AVector,
			      *VTX(a0(ASector)) - pt,
			      *VTX(a0(a1(ASector))) - pt,
			      APlane);
    else
      return (isVectorInSector(AVector,
			       *VTX(a0(ASector)) - pt,
			       *VTX(a0(a1(ASector))) - pt,
			       APlane) ||
	      isPointOnLine(*VTX(a0(ASector)), pt,
			    pt + AVector) ||
	      isPointOnLine(*VTX(a0(a1(ASector))), pt,
			    pt + AVector));
  else
    return true;
}

//******************************************************************************

bool CCorefine3dTools::areVectorsColinear(const CVertex & AVector1,
					  const CVertex & AVector2)
{
  return CGeneralTools::areVectorsColinear(AVector1, AVector2);
}

//******************************************************************************

TCoordinate CCorefine3dTools::pointParameterOnLine(const CVertex & APoint,
						   const CVertex & ALineVertex1,
						   const CVertex & ALineVertex2)
{
  return CGeneralTools::pointParameterOnLine(APoint, ALineVertex1,
					     ALineVertex2);
}

//******************************************************************************

CDart * CCorefine3dTools::findSectorOfVector(const CVertex & AVector,
					     CDart * AVertex,
					     const CPlane & APlane,
					     int AVertexDI)
{
  CDart *d = AVertex;
  CDart *best = NULL;

  do {
    if (isVectorInSector(AVector, d, APlane, false, AVertexDI))
      best = d;

    d = a2(a1(d));
  }
  while (d != AVertex && best == NULL);

  if (best == NULL || !FMap->isSameOrbit(best, AVertex, ORBIT_01)) {
    best = AVertex;
    WARN_BB("<findSectorOfVector> Aucun secteur valide sur la face !");
  }

  return best;
}

//******************************************************************************

bool CCorefine3dTools::areFacesCoplanar(const CVertex & AVertex1,
					const CPlane & APlane1,
					const CVertex & AVertex2,
					const CPlane & APlane2)
{
  CVertex normal1 = APlane1.getNormal();
  CVertex normal2 = APlane2.getNormal();

  normal1 = normalizeVector(normal1);
  normal2 = normalizeVector(normal2);

  return (isVectorNull(normal1 * normal2) &&
	  isPointOnPlane(AVertex2, APlane1) &&
	  isPointOnPlane(AVertex1, APlane2));
}

//******************************************************************************

CDart * CCorefine3dTools::findWellOrientedDart(CDart * AVertex, int AVertexDI)
{
  TCoordinate n1[4], n2[4];
  volumeNormalVector(AVertex, n1, AVertexDI);
  volumeNormalVector(a3(AVertex), n2, AVertexDI);

  if ((n1[3] < 0.0 && n2[3] < 0.0) ||
      (n1[3] > 0.0 && n2[3] > 0.0)) {
    /* Nous sommes ici dans le cas où un des deux volume est un volume
     * extérieur et nous devons donc comparer la taille des deux volumes.
     */
    CBoundingBox bb1, bb2;

    bb1 = orbitBoundingBox(AVertex, ORBIT_VOLUME, AVertexDI);
    bb2 = orbitBoundingBox(a3(AVertex), ORBIT_VOLUME, AVertexDI);

    if (bb1.getSurface() <= bb2.getSurface()) {
      // AVertex n'est pas le volume extérieur, sa normale doit donc être > 0
      if (n1[3] < 0.0) {
	AVertex = a1(AVertex);
      }
    }
    else {
      // AVertex est le volume extérieur, sa normale doit donc être < 0
      if (n1[3] > 0.0) {
	AVertex = a1(AVertex);
      }
    }
  }
  else if (n1[3] < 0.0) {
    AVertex = a1(AVertex);
  }

  return AVertex;
}

//******************************************************************************

TPositionOnEdge
CCorefine3dTools::localizeEdgesIntersection(const CVertex & ALinePoint1,
					    const CVertex & ALinePoint2,
					    const CVertex & AEdgePoint1,
					    const CVertex & AEdgePoint2,
					    TCoordinate * ALineParam,
					    TCoordinate * AEdgeParam)
{
  assert(AEdgeParam != NULL && ALineParam != NULL);

  if (localizePointOnEdge(AEdgePoint2, ALinePoint1,
			  ALinePoint2, ALineParam) != EP_OutOfEdge) {

    *AEdgeParam = 1.0;
    return EP_OnSecondVertex;
  }

  if (localizePointOnEdge(AEdgePoint1, ALinePoint1,
			  ALinePoint2, ALineParam) != EP_OutOfEdge) {
    *AEdgeParam = 0.0;
    return EP_OnFirstVertex;
  }

  CPlane plane;

  if (sqrt(squareDistanceBetweenLines(ALinePoint1, ALinePoint2,
				      AEdgePoint1, AEdgePoint2, &plane)) > FEps)
    return EP_OutOfEdge;

  if (isVectorNull(plane.getNormal()))
    return EP_OutOfEdge;

  if (!getLinesIntersection(ALinePoint1, ALinePoint2, AEdgePoint1, AEdgePoint2,
			    ALineParam, AEdgeParam, plane))
    return EP_OutOfEdge;

  if (*AEdgeParam >= 0.0 && *AEdgeParam <= 1.0)
    return EP_OnEdge;

  return EP_OutOfEdge;
}

//******************************************************************************

TPositionOnEdge
CCorefine3dTools::localizeEdgeAndPlaneIntersection(CVertex AEdgePoint1,
						   CVertex AEdgePoint2,
						   const CPlane & APlane,
						   TCoordinate * AEdgeParam)
{
  assert(AEdgeParam != NULL);

  CVertex v = AEdgePoint2 - AEdgePoint1;

  if (isPointOnPlane(AEdgePoint2, APlane)) {
    *AEdgeParam = 1.0;
    return EP_OnSecondVertex;
  }

  if (isPointOnPlane(AEdgePoint1, APlane)) {
    *AEdgeParam = 0.0;
    return EP_OnFirstVertex;
  }

  if (!APlane.getLineIntersection(AEdgePoint1, v, AEdgeParam))
    return EP_OutOfEdge;

//   if (*AEdgeParam >= 0.0 && *AEdgeParam <= 1.0 &&
//       isPointInFace(AFace, AEdgePoint1 + *AEdgeParam * v, &plane))
//     return EP_OnEdge;

  if (*AEdgeParam >= 0.0 && *AEdgeParam <= 1.0)
    return EP_OnEdge;

  return EP_OutOfEdge;
}

//******************************************************************************

CEdgeIntersection
CCorefine3dTools::findNearestIntersectionInFace(const CVertex & AOrigin,
						const CVertex & ADirection,
						CDart * AFace,
						const CPlane & APlane,
						bool AExcludeOrigin,
						int AVertexDI)
{
  ENTER;

  TProjection proj = APlane.getBestProjection();

  CVertex projOrigin = APlane.projectPoint(AOrigin, proj);
  CVertex projDirection = APlane.projectPoint(ADirection, proj);

  CDart *d = AFace;
  CEdgeIntersection inter;
  TPositionOnEdge pos;
  int edge_mark = FMap->getNewMark();
  TCoordinate t1 = 0.0, t2 = 0.0;
  CVertex pt1, pt2, pt3;
  CVertex extremity = projOrigin + projDirection;

  if (projOrigin != extremity) {
    MSG("Parcours des sommets de la face");
    do {
      pt1 = APlane.projectPoint(*VTX(d), proj);

      if (CCorefine2dTools::isPointOnLine(pt1, projOrigin, extremity)) {
	if (!AExcludeOrigin || !arePointsEqual(projOrigin, pt1)) {
	  t1 = CCorefine2dTools::pointParameterOnLine(pt1, projOrigin,
						      extremity);

	  if (t1 >= 0.0 && (t1 <= inter.getParameter() ||
			    inter.getCell() == NULL)) {
	    inter.setCell(d);
	    inter.setCellDimension(0);
	    inter.setPosition(EP_OnEdge);
	    inter.setParameter(t1);
	    inter.setPoint(*VTX(d));

	    MSG("une intersection trouvée :" << endl << inter);
	  }
	}

	// Marquage des arêtes adjacentes pour ne pas les tester
	FMap->setMark(d, edge_mark);
	FMap->setMark(a0(d), edge_mark);
	FMap->setMark(a1(d), edge_mark);
	FMap->setMark(a0(a1(d)), edge_mark);
      }
      
      d = a1(a0(d));
    }
    while (d != AFace);
    
    MSG("Parcours des arêtes de la face");
    do {
      pt1 = APlane.projectPoint(*VTX(d), proj);
      pt2 = APlane.projectPoint(*VTX(a0(d)), proj);

      if (FMap->isMarked(d, edge_mark)) {
	FMap->unsetMark(d, edge_mark);
	FMap->unsetMark(a0(d), edge_mark);
      }
      else if (fabs((pt2 - pt1).norm()) > FEps / 2.0 &&
	       (!AExcludeOrigin ||
		!CCorefine2dTools::isPointOnLine(projOrigin, pt1, pt2))) {
	pos = CCorefine2dTools::localizeEdgesIntersection(projOrigin, extremity,
							  pt1, pt2, &t1, &t2);

// 	pt3 = pt1 + t2 * (pt2 - pt1);

	if (pos != EP_OutOfEdge &&
// 	    !arePointsEqual(projOrigin, pt3) &&
	    t1 >= 0.0 && (t1 <= inter.getParameter() ||
			  inter.getCell() == NULL)) {
	  inter.setCell(d);
	  inter.setCellDimension(1);
	  inter.setPosition(EP_OnEdge);
	  inter.setParameter(t1);
	  inter.setPoint(*VTX(d) + t2 * (*VTX(a0(d)) - *VTX(d)));

	  MSG("une intersection trouvée :" << endl << inter);
	}
      }
      
      d = a1(a0(d));
    }
    while (d != AFace);

//     if (inter.getCell() != NULL)
//       inter.setPoint(AOrigin + inter.getParameter() * ADirection);
  }
  
  FMap->freeMark(edge_mark);

  EXIT;

  return inter;
}

//******************************************************************************

CEdgeIntersection
CCorefine3dTools::findNearestIntersectionInFace(const CVertex & AOrigin,
						const CVertex & ADirection,
						CDart * AFace,
						const CPlane & AFacePlane,
						const CPlane & ACutPlane,
						bool AExcludeOrigin,
						int AVertexDI)
{
  DEBUG_FUNCTION;

  CDart *d = AFace;
  CEdgeIntersection inter;
  TCoordinate dist, t;
  CVertex pt1, pt2, pt3, dir, extremity = AOrigin + ADirection;

  int pos_mark = FMap->getNewMark();
  int neg_mark = FMap->getNewMark();
  int face_mark = FMap->getNewMark();

  FMap->markOrbit(AFace, ORBIT_01, face_mark);

  MSG("Parcours des sommets de la face");
  do {
    pt1 = *VTX(d);
    MSG("sommet testé : " << pt1);
    dist = ACutPlane.pointDistance(pt1);
    if (dist < -FEps) {
      MSG("le sommet se trouve du côté négatif");
      for (CDynamicCoverageVertex dcv(FMap, d); dcv.cont(); ++dcv)
	if (FMap->isMarked(*dcv, face_mark)) FMap->setMark(*dcv, neg_mark);
    }
    else if (dist > FEps) {
      MSG("le sommet se trouve du côté positif");
      for (CDynamicCoverageVertex dcv(FMap, d); dcv.cont(); ++dcv)
	if (FMap->isMarked(*dcv, face_mark)) FMap->setMark(*dcv, pos_mark);
    }
    else if (!AExcludeOrigin || !arePointsEqual(pt1, AOrigin)) {
      t = pointParameterOnLine(pt1, AOrigin, extremity);
      if (t >= 0.0 && (t <= inter.getParameter() || !inter.getCell())) {
	MSG("le sommet se trouve sur la ligne de coupe");
	inter.setCell(d);
	inter.setCellDimension(0);
	inter.setPosition(EP_OnEdge);
	inter.setParameter(t);
	inter.setPoint(pt1);
      }
    }
    d = FMap->alpha01(d);
  }
  while (d != AFace);

  MSG("Parcours des arêtes de la face");
  do {
    pt1 = *VTX(d);
    pt2 = *VTX(a0(d));
    MSG("arête testée : [" << pt1 << ";" << pt2 << "]");
    dir = pt2 - pt1;
    if ((FMap->isMarked(d, pos_mark) && FMap->isMarked(a0(d), neg_mark)) ||
	(FMap->isMarked(d, neg_mark) && FMap->isMarked(a0(d), pos_mark))) {
      if (!AExcludeOrigin ||
	  !(abs(CPlane(AFacePlane.getNormal() * (pt2-pt1), pt1).
		pointDistance(AOrigin)) <= FEps)) {
	if (ACutPlane.getLineIntersection(pt1, dir, &t)) {
	  assert(t > 0.0 && t < 1.0);
	  pt3 = pt1 + dir * t;
	  t = pointParameterOnLine(pt3, AOrigin, extremity);
	  if (t >= 0.0 && (t <= inter.getParameter() || !inter.getCell())) {
	    MSG("la ligne de coupe intersecte l'arête en " << pt3);
	    inter.setCell(d);
	    inter.setCellDimension(1);
	    inter.setPosition(EP_OnEdge);
	    inter.setParameter(t);
	    inter.setPoint(pt3);
	  }
	}
	else
	  assert(false);
      }
    }
    FMap->unsetMark(d, face_mark);
    FMap->unsetMark(a0(d), face_mark);
    for (CDynamicCoverageEdge dce(FMap, d); dce.cont(); ++dce) {
      FMap->unsetMark(*dce, pos_mark);
      FMap->unsetMark(*dce, neg_mark);
    }
    d = FMap->alpha01(d);
  }
  while (d != AFace);

//   assert(FMap->isWholeMapUnmarked(pos_mark));
//   assert(FMap->isWholeMapUnmarked(neg_mark));
//   assert(FMap->isWholeMapUnmarked(face_mark));
  FMap->freeMark(pos_mark);
  FMap->freeMark(neg_mark);
  FMap->freeMark(face_mark);

  return inter;
}

//******************************************************************************

CEdgeIntersection
CCorefine3dTools::findNearestIntersectionInOrbit(const CVertex & AOrigin,
						 const CVertex & ADirection,
						 CDart * ADart, TOrbit AOrbit,
						 int AVertexDI)
{
  CEdgeIntersection inter;
  TCoordinate t1 = 0.0, t2 = 0.0;
  CVertex *pt1, *pt2, pt, extremity = AOrigin + ADirection;
  CPlane plane;
  CDart *face;
  int mark = FMap->getNewMark();
  int edge_mark = FMap->getNewMark();
  int face_mark = FMap->getNewMark();
  int orient_mark = FMap->getNewMark();

  TOrbit orbit_vertex = AND_ORBIT(ORBIT_VERTEX, AOrbit);
  TOrbit orbit_edge = AND_ORBIT(ORBIT_EDGE, AOrbit);
  TOrbit orbit_face = AND_ORBIT(ORBIT_FACE, AOrbit);

  CCoverage *cov = FMap->getDynamicCoverage(ADart, AOrbit);
  CCoverage *dcf, *tmp_cov;

  FMap->halfMarkOrbit(ADart, AOrbit, orient_mark);

  inter.setCell(NULL);

  // Recherche d'une éventuelle intersection avec les sommets
  for (cov->reinit(); cov->cont(); ++(*cov)) {
    dcf = FMap->getDynamicCoverage(**cov, orbit_face);

    for (; dcf->cont(); ++(*dcf)) {
      if (!FMap->isMarked(**dcf, mark) && FMap->isMarked(**dcf, orient_mark)) {
	// Marquage du sommet comme traité
	FMap->markOrbit(**dcf, orbit_vertex, mark);
	
	pt = *VTX(**dcf);
	
	if (isPointOnLine(pt, AOrigin, extremity)) {
	  t2 = pointParameterOnLine(pt, AOrigin, extremity);
	  
	  if (t2 > 0.0) {
	    if (t2 < inter.getParameter() || inter.getCell() == NULL) {
	      inter.setCell(**dcf);
	      inter.setCellDimension(0);
	      inter.setPoint(pt);
	      inter.setParameter(t2);
	    }
	  
	    tmp_cov = FMap->getDynamicCoverage(**dcf, orbit_vertex);
	    
	    for (; tmp_cov->cont(); ++(*tmp_cov)) {
	      // Marquage des arêtes incidentes pour ne pas les tester
	      if (!FMap->isMarked(**tmp_cov, edge_mark))
		FMap->markOrbit(**tmp_cov, orbit_edge, edge_mark);
	      
	      // Marquage des faces incidentes pour ne pas les tester
	      if (!FMap->isMarked(**tmp_cov, face_mark))
		FMap->markOrbit(**tmp_cov, orbit_face, face_mark);
	    }
	    
	    delete tmp_cov;
	  }
	}
      }
    }

    delete dcf;
  }

  // Recherche d'une éventuelle intersection avec les arêtes
  for (cov->reinit(); cov->cont(); ++(*cov)) {
    dcf = FMap->getDynamicCoverage(**cov, orbit_face);

    for (; dcf->cont(); ++(*dcf)) {
      if (FMap->isMarked(**dcf, edge_mark)) {
	FMap->unmarkOrbit(**dcf, orbit_edge, edge_mark);
	FMap->unmarkOrbit(**dcf, orbit_edge, mark);
      }
      else if (FMap->isMarked(**dcf, mark) &&
	       FMap->isMarked(**dcf, orient_mark)) {
	FMap->unmarkOrbit(**dcf, orbit_edge, mark);
	  
	pt1 = VTX(**dcf);
	pt2 = VTX(a0(**dcf));
	  
	if (sqrt(squareDistanceBetweenLines(*pt1, *pt2, AOrigin, extremity,
					    &plane)) <= FEps) {

	  if (!isVectorNull(plane.getNormal()) &&
	      getLinesIntersection(*pt1, *pt2, AOrigin, extremity,
				   &t1, &t2, plane)) {
	    
	    pt = AOrigin + t2 * ADirection;
	    
	    if (t1 >= 0.0 && t1 <= 1.0 && t2 > 0) {
	      if (t2 < inter.getParameter() || inter.getCell() == NULL) {
		inter.setCell(**dcf);
		inter.setCellDimension(1);
		inter.setPoint(pt);
		inter.setParameter(t2);
	      }
	    
	      tmp_cov = FMap->getDynamicCoverage(**dcf, orbit_edge);
	      
	      for (; tmp_cov->cont(); ++(*tmp_cov)) {
		// Marquage des faces incidentes pour ne pas les tester
		if (!FMap->isMarked(**tmp_cov, face_mark))
		  FMap->markOrbit(**tmp_cov, orbit_face, face_mark);
	      }

	      delete tmp_cov;
	    }
	  }
	}
      }
    }

    delete dcf;
  }
  
  // Recherche d'une éventuelle intersection avec les faces
  for (cov->reinit(); cov->cont(); ++(*cov)) {
    if (FMap->isMarked(**cov, face_mark)) {
      FMap->unmarkOrbit(**cov, orbit_face, face_mark);
      FMap->markOrbit(**cov, orbit_face, mark);
    }
    else if (!FMap->isMarked(**cov, mark)) {
      face = (FMap->isMarked(**cov, orient_mark)) ? **cov : a3(**cov);
      FMap->markOrbit(face, orbit_face, mark);
	
      plane = facePlane(face, AVertexDI);
      
      if (plane.getLineIntersection(AOrigin, ADirection, &t2)) {
	if (t2 > 0.0 &&
	    (t2 < inter.getParameter() || inter.getCell() == NULL)) {
	  pt = AOrigin + t2 * ADirection;
	  
	  if (isPointInFace(pt, face, &plane, AVertexDI)) {
	    inter.setCell(face);
	    inter.setCellDimension(2);
	    inter.setPoint(pt);
	    inter.setParameter(t2);
	  }
	}
      }
    }
  }

  for (cov->reinit(); cov->cont(); ++(*cov)) {
    FMap->unsetMark(**cov, mark);
    FMap->unsetMark(**cov, orient_mark);
  }
  
  delete cov;

  FMap->freeMark(mark);
  FMap->freeMark(edge_mark);
  FMap->freeMark(face_mark);
  FMap->freeMark(orient_mark);

  return inter;
}

//******************************************************************************

bool CCorefine3dTools::isPointInFace(const CVertex & APoint, CDart * AFace,
				     const CPlane * APlane, int AVertexDI)
{
  bool result;
  CPlane plane;
  
  if (APlane == NULL)
    plane = facePlane(AFace, AVertexDI);
  else
    plane = *APlane;

  if (APoint == *VTX(AFace))
    result = true;
  else {
    CVertex direction = *VTX(AFace) - APoint;
    CEdgeIntersection inter;

    inter = findNearestIntersectionInFace(APoint, direction, AFace, plane,
					  false, AVertexDI);

    if (inter.getCell() == NULL) {
      result = false;
    }
    else {
      if (inter.getCellDimension() == 0) {
	result = isVectorInSector(-direction, inter.getCell(), plane, false,
				  AVertexDI);
      }
      else {
	CVertex v = edgeVector(inter.getCell(), AVertexDI);
	result = (plane.getNormal() * v).dot(direction) < 0.0;
      }
    }
  }

  return result;
}

//******************************************************************************
#define LINK(d1, d2) (FMap->setDirectInfo(d1, ALinkDirectInfo, d2), \
		      FMap->setDirectInfo(d2, ALinkDirectInfo, d1), \
		      FMap->setDirectInfo(a0(d1), ALinkDirectInfo, a0(d2)), \
		      FMap->setDirectInfo(a0(d2), ALinkDirectInfo, a0(d1)))

#define UNLINK(d) (FMap->setDirectInfo(d, ALinkDirectInfo, NULL), \
		   FMap->setDirectInfo(a0(d), ALinkDirectInfo, NULL))

#define ALPHA2(d) ((FMap->getDirectInfo(d, ALinkDirectInfo) != NULL) ? \
		   (CDart*)FMap->getDirectInfo(d, ALinkDirectInfo) : \
		   alpha2(d))

void CCorefine3dTools::sortFacesAroundEdges_Naive(CDart * AEdge1,
						  CDart * AEdge2,
						  int ALinkDirectInfo,
						  int AVertexDI)
{
  CPlane plane(edgeVector(AEdge1, AVertexDI), *VTX(AEdge1));

  CDart *d1, *d2;
  CVertex v, v1, v2;
  CDart *best, *tmp1, *tmp2;

  d1 = AEdge1;
  do {
    v = faceNormalVector(d1, AVertexDI);

    best = NULL;
    d2 = AEdge2;
    v2 = faceNormalVector(d2, AVertexDI);
    do {
      tmp2 = a3(alpha2(d2));

      v1 = v2;
      v2 = faceNormalVector(tmp2, AVertexDI);

      if (isVectorInSector(v, v1, v2, plane))
	best = d2;

      d2 = tmp2;
    }
    while (d2 != AEdge2 && best == NULL);

    if (best == NULL) {
      WARN_BB("<sortFacesAroundEdges_Naive> Aucun secteur trouvé !");
      d2 = AEdge2;
    }
    else
      d2 = best;

    // Liaison avec la face précédente
    tmp1 = alpha2(d1);
    if (FMap->getDirectInfo(tmp1, ALinkDirectInfo) == d2) {
      UNLINK(tmp1);
      UNLINK(d2);
    }
    if (FMap->getDirectInfo(d2, ALinkDirectInfo) == NULL)
      LINK(d2, a3(d1));

    // Liaison avec la face suivante
    tmp1 = alpha2(a3(d1));
    tmp2 = alpha2(d2);
    if (FMap->getDirectInfo(tmp1, ALinkDirectInfo) == tmp2) {
      UNLINK(tmp1);
      UNLINK(tmp2);
    }
    if (FMap->getDirectInfo(tmp2, ALinkDirectInfo) == NULL)
      LINK(d1, tmp2);

    d1 = a3(alpha2(d1));
  }
  while (d1 != AEdge1);
}

//******************************************************************************

void CCorefine3dTools::sortFacesAroundEdges_NaiveBis(CDart * AEdge1,
						     CDart * AEdge2,
						     int ALinkDirectInfo,
						     int AVertexDI)
{
  CPlane plane(edgeVector(AEdge1, AVertexDI), *VTX(AEdge1));

  CDart *d1, *d2;
  CVertex v, v1, v2;
  CDart *best, *tmp;
  int mark = FMap->getNewMark();

  for (CDynamicCoverageEdge dce1(FMap, AEdge1); dce1.cont(); dce1++)
    FMap->setDirectInfo(*dce1, ALinkDirectInfo, NULL);
  for (CDynamicCoverageEdge dce2(FMap, AEdge2); dce2.cont(); dce2++)
    FMap->setDirectInfo(*dce2, ALinkDirectInfo, NULL);

  d1 = AEdge1;
  d2 = AEdge2;
  do {
    v = faceNormalVector(d1, AVertexDI);

    best = NULL;
    v2 = faceNormalVector(d2, AVertexDI);
    do {
      FMap->setMark(d2, mark);

//       tmp = a3(alpha2(d2));
      tmp = a3(ALPHA2(d2));

      v1 = v2;
      v2 = faceNormalVector(tmp, AVertexDI);

      if (isVectorInSector(v, v1, v2, plane))
	best = d2;

      d2 = tmp;
    }
    while (best == NULL && !FMap->isMarked(d2, mark));

    FMap->unmarkOrbit(AEdge1, ORBIT_23, mark);
    FMap->unmarkOrbit(AEdge2, ORBIT_23, mark);

    if (best == NULL) {
      WARN_BB("<sortFacesAroundEdges_NaiveBis> Aucun secteur trouvé !");
      d2 = AEdge2;
    }
    else
      d2 = best;

//     if (isVectorInSector(calculateFaceNormal(d2), getAlpha2(a3(d1)), plane)) {
//       if (FMap->getDirectInfo(d2, ALinkDirectInfo) != NULL)
// 	UNLINK((Dart*)FMap->getDirectInfo(d2, ALinkDirectInfo));
//       LINK(d2, a3(d1));
//     }

//     tmp = getAlpha2(d2);
//     if (isVectorInSector(calculateFaceNormal(a3(tmp)), d1, plane)) {
//       if (FMap->getDirectInfo(tmp, ALinkDirectInfo) != NULL)
// 	UNLINK((Dart*)FMap->getDirectInfo(tmp, ALinkDirectInfo));
//       LINK(d1, tmp);
//     }

    if (FMap->getDirectInfo(d2, ALinkDirectInfo) != NULL) {
      tmp = (CDart*)FMap->getDirectInfo(d2, ALinkDirectInfo);
      UNLINK(tmp);
      UNLINK(d2);
    }
    else
      tmp = alpha2(d2);

    LINK(d2, a3(d1));
    LINK(d1, tmp);

    d1 = a3(alpha2(d1));
  }
  while (d1 != AEdge1);

  FMap->freeMark(mark);
}

//******************************************************************************

bool CCorefine3dTools::sortFacesAroundEdges_Optimized(CDart * AEdge1,
						      CDart * AEdge2,
						      int ALinkDirectInfo,
						      int AVertexDI)
{
  int mark = FMap->getNewMark();
    
  CPlane plane(edgeVector(AEdge1, AVertexDI), *VTX(AEdge1));

  CDart *d = AEdge1;
  CVertex v = faceNormalVector(d, AVertexDI);
  
  CDart *tmp = a3(alpha2(AEdge2));
  CVertex v1 = faceNormalVector(AEdge2, AVertexDI);
  CVertex v2 = faceNormalVector(tmp, AVertexDI);

//   cout << endl << "Arête : [" << *VTX(AEdge1) << ","
//        << *VTX(a0(AEdge1)) << "]" << endl;

  while (!isVectorInSector(v, v1, v2, plane)) {
//     cout << v << endl << v1 << endl << v2 << endl;
    v1 = v2;
    AEdge2 = tmp;

    tmp = a3(alpha2(AEdge2));
    v2 = faceNormalVector(tmp, AVertexDI);
  }

  CDart *d1 = a3(alpha2(AEdge1));
  CDart *d2 = a3(alpha2(AEdge2));
    
  v1 = faceNormalVector(d1, AVertexDI);
  v2 = faceNormalVector(d2, AVertexDI);

  do {
    FMap->setMark(d, mark);

    if (areVectorsColinear(v1, v2)) {
      if (!FMap->isMarked(d2, mark)) {
	LINK(d, a3(d2));
	FMap->setMark(d2, mark);
	d = d2;
      }
      
      LINK(d, a3(d1));

      d = d1;
      v = v1;
      
      d1 = a3(alpha2(d1));
      d2 = a3(alpha2(d2));
      
      v1 = faceNormalVector(d1, AVertexDI);
      v2 = faceNormalVector(d2, AVertexDI);
    }
    else if (isVectorInSector(v2, v, v1, plane)) { // v2 est entre v et v1
      LINK(d, a3(d2));
      
      d = d2;
      v = v2;
      
      d2 = a3(alpha2(d2));
      v2 = faceNormalVector(d2, AVertexDI);
    }
    else { // v1 est entre v et v2
      LINK(d, a3(d1));
      
      d = d1;
      v = v1;
      
      d1 = a3(alpha2(d1));
      v1 = faceNormalVector(d1, AVertexDI);
    }
  }
  while (!FMap->isMarked(d, mark));

  bool problem = false;
    
  // Démarquage des brins de la deuxième arête
  d2 = AEdge2;
  do {
    if (FMap->isMarked(d2, mark))
      FMap->unsetMark(d2, mark);
    else {
      WARN_BR("<sortFacesAroundEdges> Brin de la deuxième arête non marqué !");
      problem = true;
    }
      
    d2 = a3(alpha2(d2));
  }
  while (d2 != AEdge2);
    
  // Démarquage des brins de la première arête
  d1 = AEdge1;
  do {
    if (FMap->isMarked(d1, mark))
      FMap->unsetMark(d1, mark);
    else {
      WARN_BR("<sortFacesAroundEdges> Brin de la première arête non marqué !");
      problem = true;
    }
      
    d1 = a3(alpha2(d1));
  }
  while (d1 != AEdge1);
  
  FMap->freeMark(mark);
    
  if (problem) {
    d1 = AEdge1;
    do {
      FMap->setDirectInfo(d1, ALinkDirectInfo, NULL);
      d1 = a3(alpha2(d1));
    }
    while (d1 != AEdge1);
    
    d2 = AEdge2;
    do {
      FMap->setDirectInfo(d2, ALinkDirectInfo, NULL);
      d2 = a3(alpha2(d2));
    }
    while (d2 != AEdge2);
  }

  return !problem;
}

//******************************************************************************

class CFaceComparator
{
public:
  CFaceComparator(CCorefine3dTools & ATools, const CVertex & AAxis,
		  int AVertexDI = -1)
    : FTools(ATools), FAxis(AAxis), FVertexDI(AVertexDI) {}

  bool operator () (CDart * AFace1, CDart * AFace2)
  {
    CVertex v1 = FTools.faceNormalVector(AFace1, FVertexDI);
    CVertex v2 = FTools.faceNormalVector(AFace2, FVertexDI);
    return (v1 * v2).dot(FAxis) > 0.0;
  }

private:
  CCorefine3dTools & FTools;
  CVertex FAxis;
  int FVertexDI;
};

//******************************************************************************

void CCorefine3dTools::sortFacesAroundEdges_SuperNaive(CDart * AEdge1,
						       CDart * AEdge2,
						       int ALinkDirectInfo,
						       int AVertexDI)
{
  CDart *d1, *d2;
  CVertex axis = edgeVector(AEdge1, AVertexDI);
  CFaceComparator comp(*this, axis, AVertexDI);
  multiset<CDart*, CFaceComparator> faces(comp);
  multiset<CDart*, CFaceComparator>::iterator it, tmp_it;

  for (CDynamicCoverageEdge dce1(FMap, AEdge1); dce1.cont(); ++dce1)
    FMap->setDirectInfo(*dce1, ALinkDirectInfo, NULL);
  for (CDynamicCoverageEdge dce2(FMap, AEdge2); dce2.cont(); ++dce2)
    FMap->setDirectInfo(*dce2, ALinkDirectInfo, NULL);

  d1 = AEdge1;
  do {
    faces.insert(d1);
    d1 = a3(alpha2(d1));
  }
  while (d1 != AEdge1);

  d2 = AEdge2;
  do {
    faces.insert(d2);
    d2 = a3(alpha2(d2));
  }
  while (d2 != AEdge2);

  for (it = faces.begin(); it != faces.end(); ) {
    tmp_it = it++;

    d1 = *tmp_it;
    d2 = (it != faces.end()) ? *it : *faces.begin();

    LINK(d1, a3(d2));
  }
}

#undef LINK
#undef UNLINK
#undef ALPHA2
//******************************************************************************

TCoordinate CCorefine3dTools::matrixDet(TCoordinate AMat[3][3])
{
  return (AMat[0][0] * (AMat[1][1] * AMat[2][2] - AMat[2][1] * AMat[1][2]) -
	  AMat[0][1] * (AMat[1][0] * AMat[2][2] - AMat[2][0] * AMat[1][2]) -
	  AMat[0][2] * (AMat[1][0] * AMat[2][1] - AMat[2][0] * AMat[1][1]));
}

//******************************************************************************

void CCorefine3dTools::vectProduct4d(const CVertex & AV1,
				     const CVertex & AV2,
				     const CVertex & AV3,
				     TCoordinate ANormal[4])
{
  TCoordinate mat[3][3];

//   mat[0][0] = AV1.getY();
//   mat[0][1] = AV1.getZ();
//   mat[0][2] = 0;

//   mat[1][0] = AV2.getY();
//   mat[1][1] = AV2.getZ();
//   mat[1][2] = 0;

//   mat[2][0] = AV3.getY();
//   mat[2][1] = AV3.getZ();
//   mat[2][2] = 0;

//   ANormal[0] = matrixDet(mat);

//   mat[0][0] = AV1.getX();
//   mat[0][1] = AV1.getZ();
//   mat[0][2] = 0;

//   mat[1][0] = AV2.getX();
//   mat[1][1] = AV2.getZ();
//   mat[1][2] = 0;

//   mat[2][0] = AV3.getX();
//   mat[2][1] = AV3.getZ();
//   mat[2][2] = 0;

//   ANormal[1] = matrixDet(mat);

//   mat[0][0] = AV1.getX();
//   mat[0][1] = AV1.getY();
//   mat[0][2] = 0;

//   mat[1][0] = AV2.getX();
//   mat[1][1] = AV2.getY();
//   mat[1][2] = 0;

//   mat[2][0] = AV3.getX();
//   mat[2][1] = AV3.getY();
//   mat[2][2] = 0;

//   ANormal[2] = matrixDet(mat);

  mat[0][0] = AV1.getX();
  mat[0][1] = AV1.getY();
  mat[0][2] = AV1.getZ();

  mat[1][0] = AV2.getX();
  mat[1][1] = AV2.getY();
  mat[1][2] = AV2.getZ();

  mat[2][0] = AV3.getX();
  mat[2][1] = AV3.getY();
  mat[2][2] = AV3.getZ();

  ANormal[3] = matrixDet(mat);
}

//******************************************************************************

void CCorefine3dTools::volumeNormalVector(CDart * AVolume,
					  TCoordinate ANormal[4],
					  int AVertexDI)
{
  int orient_mark = FMap->getNewMark();
  CVertex v1, v2, v3;
  TCoordinate tmp[4];

  ANormal[0] = ANormal[1] = ANormal[2] = ANormal[3] = 0.0;

  FMap->halfMarkOrbit(AVolume, ORBIT_VOLUME, orient_mark);

  for (CDynamicCoverageVolume dcv(FMap, AVolume); dcv.cont(); dcv++) {
    if (FMap->isMarked(*dcv, orient_mark)) {
      FMap->unsetMark(*dcv, orient_mark);

      if (!FMap->isFree2(*dcv)) {
	v1 = edgeVector(a1(a2(*dcv)), AVertexDI);
	v2 = edgeVector(*dcv, AVertexDI);
	v3 = edgeVector(a1(*dcv), AVertexDI);

	vectProduct4d(v1, v2, v3, tmp);

// 	for (int i = 0; i < 4; i++)
// 	  ANormal[i] += tmp[i];
	ANormal[3] += tmp[3];
      }
    }
  }

  FMap->freeMark(orient_mark);
}

//******************************************************************************

