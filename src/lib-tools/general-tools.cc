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

#include "general-tools.hh"
#include "plane.hh"
#include "bounding-box.hh"

// #define WARNING_MESSAGES
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
CGeneralTools::CGeneralTools(CGMapVertex * AMap, TCoordinate AEpsilon)
  : FMap(AMap), FEps(AEpsilon)
{
  FEps2 = FEps * FEps;
}
//******************************************************************************
CGeneralTools::~CGeneralTools()
{
}
//******************************************************************************
CVertex CGeneralTools::normalizeVector(const CVertex & AVector)
{
  TCoordinate n = AVector.norm();
  CVertex v;

  if (n <= FEps)
    return ORIGIN;
//   if (n == 0.0)
//     return AVector;

  v.setX(AVector.getX() / n);
  v.setY(AVector.getY() / n);
  v.setZ(AVector.getZ() / n);

  return v;
}
//******************************************************************************
bool CGeneralTools::isVectorNull(const CVertex & AVector)
{
  return AVector.norm() <= FEps;
}
//******************************************************************************
bool CGeneralTools::areVectorsColinear(const CVertex & AVector1,
				       const CVertex & AVector2)
{
  if (AVector1 * AVector2 != ORIGIN)
    return false;
  else
    return AVector1.dot(AVector2) > 0.0;
}
//******************************************************************************
// TCoordinate CGeneralTools::pointParameterOnLine(const CVertex & APoint,
// 						const CVertex & ALineVertex1,
// 						const CVertex & ALineVertex2)
// {
//   CVertex v1 = ALineVertex2 - ALineVertex1;
//   CVertex v2 = APoint - ALineVertex1;
//   TCoordinate n1 = v1.norm();
//   TCoordinate n2 = v2.norm();

// #ifdef WARNING_MESSAGES
//   if (isVectorNull(v1))
//     WARN_BB("<CGeneralTools::pointParameterOnLine> "
// 	    << "Le vecteur est de longueur nulle : " << v1);
// #endif

//   return (v1.dot(v2) >= 0.0) ? n2 / n1 : -n2 / n1;
// }
//******************************************************************************
TCoordinate CGeneralTools::pointParameterOnLine(const CVertex & APoint,
						const CVertex & ALineVertex1,
						const CVertex & ALineVertex2)
{
  CVertex v1 = ALineVertex2 - ALineVertex1;
  CVertex v2 = APoint - ALineVertex1;

#ifdef WARNING_MESSAGES
  if (isVectorNull(v1))
    WARN_BB("<CGeneralTools::pointParameterOnLine> "
	    << "Le vecteur est de longueur nulle : " << v1);
#endif

  return v1.dot(v2) / v1.sqrNorm();
}
//******************************************************************************
TCoordinate CGeneralTools::squareDistanceToLine(const CVertex & APoint,
						const CVertex & ALineVertex1,
						const CVertex & ALineVertex2)
{
  CVertex v = ALineVertex2 - ALineVertex1;

#ifdef WARNING_MESSAGES
  if (isVectorNull(v))
    WARN_BB("<CGeneralTools::squareDistanceToLine> "
	    << "Le vecteur est de longueur nulle : " << v);
#endif

  return (v * (APoint - ALineVertex1)).sqrNorm() / v.sqrNorm();
}
//******************************************************************************
TCoordinate CGeneralTools::squareDistanceToPlane(const CVertex & APoint,
						 const CPlane & APlane)
{
  CVertex v = APlane.getNormal();
  TCoordinate t=0;

#ifdef WARNING_MESSAGES
  if (isVectorNull(v))
    WARN_BB("<CGeneralTools::squareDistanceToPlane> "
	    << "La normale est de longueur nulle : " << v);
#endif

  APlane.getLineIntersection(APoint, v, &t);

  return (t * v).sqrNorm();
}
//******************************************************************************
TCoordinate
CGeneralTools::squareDistanceBetweenLines(const CVertex & ALine1Vertex1,
					  const CVertex & ALine1Vertex2,
					  const CVertex & ALine2Vertex1,
					  const CVertex & ALine2Vertex2,
					  CPlane * AIntersectionPlane)
{
  CVertex v1, v2, n;

  v1 = ALine1Vertex2 - ALine1Vertex1;
  v2 = ALine2Vertex2 - ALine2Vertex1;

  n = v1 * v2;

  if (isVectorNull(n)) {
    // Les droites sont parallèles
    if (AIntersectionPlane != NULL)
      AIntersectionPlane->setPlane(0.0, 0.0, 0.0, 0.0);

    return squareDistanceToLine(ALine2Vertex1, ALine1Vertex1, ALine1Vertex2);
  }
  else {
    // Les droites forment un plan
    CPlane plane(n, ALine1Vertex1);

    if (AIntersectionPlane != NULL)
      *AIntersectionPlane = plane;

    return squareDistanceToPlane(ALine2Vertex1, plane);
  }
}
//******************************************************************************
bool CGeneralTools::getLinesIntersection(CVertex ALine1Point1,
					 CVertex ALine1Point2,
					 CVertex ALine2Point1,
					 CVertex ALine2Point2,
					 TCoordinate * ALine1Param,
					 TCoordinate * ALine2Param,
					 const CPlane & APlane)
{
  TProjection proj = APlane.getBestProjection();

  ALine1Point1 = APlane.projectPoint(ALine1Point1, proj);
  ALine1Point2 = APlane.projectPoint(ALine1Point2, proj);
  ALine2Point1 = APlane.projectPoint(ALine2Point1, proj);
  ALine2Point2 = APlane.projectPoint(ALine2Point2, proj);

  CVertex v[2] = {ALine1Point2 - ALine1Point1, ALine2Point2 - ALine2Point1};
  TCoordinate d = v[0].getY() * v[1].getX() - v[0].getX() * v[1].getY();
  CVertex tmp = ALine1Point1 - ALine2Point1;

  if (d == 0.0)
    return false;

  *ALine1Param = (v[1].getY() * tmp.getX() - v[1].getX() * tmp.getY()) / d;
  *ALine2Param = (v[0].getY() * tmp.getX() - v[0].getX() * tmp.getY()) / d;

//   *ALine1Param = (v[1].getY() * (ALine1Point1.getX() - ALine2Point1.getX()) -
// 		  v[1].getX() * (ALine1Point1.getY() - ALine2Point1.getY())) /d;

//   if (fabs(v[1].getX()) > fabs(v[1].getY()))
//     *ALine2Param = (ALine1Point1.getX() + *ALine1Param * v[0].getX() -
// 		    ALine2Point1.getX()) / v[1].getX();
//   else
//     *ALine2Param = (ALine1Point1.getY() + *ALine1Param * v[0].getY() -
// 		    ALine2Point1.getY()) / v[1].getY();

  return true;
}
//******************************************************************************
bool CGeneralTools::arePointsEqual(const CVertex & APoint1,
				   const CVertex & APoint2)
{
  return fabs((APoint2 - APoint1).norm()) <= FEps;
}
//******************************************************************************
bool CGeneralTools::isPointOnLine(const CVertex & APoint,
				  const CVertex & ALineVertex1,
				  const CVertex & ALineVertex2)
{
  return sqrt(squareDistanceToLine(APoint, ALineVertex1, ALineVertex2)) <= FEps;
}
//******************************************************************************
bool CGeneralTools::isPointOnPlane(const CVertex & APoint,
				   const CPlane & APlane)
{
//   return sqrt(squareDistanceToPlane(APoint, APlane)) <= FEps;
  return abs(APlane.pointDistance(APoint)) <= FEps;
}
//******************************************************************************
bool CGeneralTools::areLinesCrossing(const CVertex & ALine1Vertex1,
				     const CVertex & ALine1Vertex2,
				     const CVertex & ALine2Vertex1,
				     const CVertex & ALine2Vertex2,
				     CPlane * AIntersectionPlane)
{
  return sqrt(squareDistanceBetweenLines(ALine1Vertex1, ALine1Vertex2,
					 ALine2Vertex1, ALine2Vertex2,
					 AIntersectionPlane)) <= FEps;
}
//******************************************************************************
CDart * CGeneralTools::alpha1(CDart * ADart)
{
  if (!FMap->isFree1(ADart))
    return a1(ADart);
  else {
    CDart *d = a2(ADart);

    while (!FMap->isFree1(d)) {
      d = a2(a1(d));
    }

    return d;
  }
}
//******************************************************************************
CDart * CGeneralTools::alpha2(CDart * ADart)
{
  if (!FMap->isFree2(ADart))
    return a2(ADart);
  else {
    CDart *d = a3(ADart);

    while (!FMap->isFree2(d)) {
      d = a3(a2(d));
    }

    return d;
  }
}
//******************************************************************************
CVertex CGeneralTools::edgeVector(CDart * AEdge, int AVertexDI)
{
  return *VTX(FMap->alpha0(AEdge)) - *VTX(AEdge);
}
//******************************************************************************
CVertex CGeneralTools::faceNormalVector(CDart * AFace, int AVertexDI)
{
  CVertex normal(0.0, 0.0, 0.0);
  int mark = FMap->getNewMark();
  CVertex pt;

  pt = *VTX(AFace);
  FMap->halfMarkOrbit(AFace, ORBIT_01, mark);
  FMap->unsetMark(AFace, mark);
  FMap->unsetMark(a0(a1(AFace)), mark);

  for (CDynamicCoverage01 cov(FMap, AFace); cov.cont(); ++cov)
    if (FMap->isMarked(*cov, mark)) {
      FMap->unsetMark(*cov, mark);
      normal += (*VTX(*cov) - pt) * (*VTX(a0(*cov)) - pt);
    }

  FMap->freeMark(mark);

  return normal;
}
//******************************************************************************
// CVertex CGeneralTools::faceNormalVector(CDart * AFace, int AVertexDI)
// {
//   CVertex normal(0.0, 0.0, 0.0);
// //   int n = 0;
//   int vert_mark = FMap->getNewMark();
//   CDart *d1, *d2;
//   CVertex v, v1, v2;
//   TCoordinate fact = 1.0;

//   TCoordinate surf = orbitBoundingBox(AFace, ORBIT_01, AVertexDI).getSurface();

//   if (surf > 0.0 && surf < 1.0)
//     fact /= surf;

//   FMap->halfMarkOrbit(AFace, ORBIT_01, vert_mark);

//   for (CDynamicCoverage01 dc(FMap, AFace); dc.cont(); ++dc) {
//     d1 = *dc;

//     if (FMap->isMarked(d1, vert_mark)) {
//       if (!FMap->isFree1(d1)) {
// 	d2 = a1(d1);

// 	if (!FMap->isFree0(d1) && !FMap->isFree0(d2)) {
// 	  v = *VTX(d1) * fact;
// 	  v1 = *VTX(a0(d1)) * fact;
// 	  v2 = *VTX(a0(d2)) * fact;
// 	  normal += (v1 - v) * (v2 - v);
// // 	  normal.setZ(normal.getZ() + PROD(v1 - v, v2 - v));
// // 	  n++;
// 	}
//       }
//       FMap->unsetMark(d1, vert_mark);
//     }
//   }

//   FMap->freeMark(vert_mark);

// //   if (n > 1)
// //     normal /= n;

//   normal = normalizeVector(normal);

//   return normal;
// }
//******************************************************************************
CVertex CGeneralTools::fastFaceNormalVector(CDart * AFace, int AVertexDI)
{
  CVertex v = *VTX(AFace);
  CVertex normal = (*VTX(a0(AFace)) - v) * (*VTX(a0(a1(AFace))) - v);

//   normal = normalizeVector(normal);

  return normal;
}
//******************************************************************************
CVertex CGeneralTools::vertexNormalVector(CDart * AVertex, int AVertexDI)
{
  int mark = FMap->getNewMark();
  int nb = 0;
  CVertex normal(0.0, 0.0, 0.0);

  FMap->halfMarkOrbit(AVertex, ORBIT_12, mark);

  for (CDynamicCoverage12 dc(FMap, AVertex); dc.cont(); ++dc) {
    if (FMap->isMarked(*dc, mark)) {
      FMap->unsetMark(*dc, mark);

      normal += faceNormalVector(*dc, AVertexDI);
      nb++;
    }
  }

  FMap->freeMark(mark);

  return (nb > 1) ? normal / nb : normal;
}
//******************************************************************************
CVertex CGeneralTools::fastVertexNormalVector(CDart * AVertex, int AVertexDI)
{
  int mark = FMap->getNewMark();
  int nb = 0;
  CVertex normal(0.0, 0.0, 0.0);

  FMap->halfMarkOrbit(AVertex, ORBIT_12, mark);

  for (CDynamicCoverage12 dc(FMap, AVertex); dc.cont(); ++dc) {
    if (FMap->isMarked(*dc, mark)) {
      FMap->unsetMark(*dc, mark);

      normal += fastFaceNormalVector(*dc, AVertexDI);
      nb++;
    }
  }

  FMap->freeMark(mark);

  return (nb > 1) ? normal / nb : normal;
}
//******************************************************************************
CPlane CGeneralTools::facePlane(CDart * AFace, int AVertexDI)
{
  return CPlane(faceNormalVector(AFace, AVertexDI),
		FMap->barycenter(AFace, ORBIT_01, AVertexDI));
}
//******************************************************************************
CBoundingBox CGeneralTools::orbitBoundingBox(CDart * ADart, TOrbit AOrbit,
					     int AVertexDI)
{
  CBoundingBox bb;
  int mark = FMap->getNewMark();
  CCoverage *cov = FMap->getDynamicCoverage(ADart, AOrbit);

  for (cov->reinit(); cov->cont(); ++(*cov))
    if (!FMap->isMarked(**cov, mark)) {
      FMap->markOrbit(**cov, ORBIT_VERTEX & AOrbit, mark);
      bb.addPoint(*VTX(**cov));
    }

  for (cov->reinit(); cov->cont(); ++(*cov))
    FMap->unsetMark(**cov, mark);

  delete cov;

  FMap->freeMark(mark);

  return bb;
}
//******************************************************************************
// CVertex CGeneralTools::vertexRegionNormalVector(CDart * AVertex,
// 						unsigned int ARegionSize,
// 						int ARegionMark, int AVertexDI)
// {
//   int mark = FMap->getNewMark();
//   CVertex normal(0.0, 0.0, 0.0);
//   list<CDart*> next_vtx, old_vtx;
//   unsigned int depth;
//   CDart *d;

//   next_vtx.push_back((CDart*)0);
//   next_vtx.push_back(AVertex);

//   while (!next_vtx.empty()) {
//     depth = (unsigned int)next_vtx.front(), next_vtx.pop_front();
//     d = next_vtx.front(), next_vtx.pop_front();
//     old_vtx.push_back(d);

//     FMap->halfMarkOrbit(a1(d), ORBIT_12, mark);

//     for (CDynamicCoverage12 dc(FMap, d); dc.cont(); ++dc)
//       if (!FMap->isMarked(*dc, mark)) {
// 	FMap->setMark(*dc, mark);
// 	normal += faceNormalVector(*dc, AVertexDI);

// 	if (depth < ARegionSize) {
// 	  d = a1(a0(*dc));

// 	  next_vtx.push_back((CDart*)(depth + 1));
// 	  next_vtx.push_back(d);

// 	  FMap->setMark(d, mark);
// 	  d = a2(a1(d)); FMap->setMark(d, mark);
// 	  d = a2(a1(d)); FMap->setMark(d, mark);
// 	}
//       }
//   }

//   for (list<CDart*>::iterator it = old_vtx.begin(); it != old_vtx.end(); ++it)
//     FMap->unmarkOrbit(*it, ORBIT_12, mark);

//   FMap->freeMark(mark);

//   return normal;
// }
//******************************************************************************
CVertex CGeneralTools::regionNormalVector(CDart * ADart, TOrbit AOrbit,
					  unsigned int ARegionSize,
					  int ARegionMark, int AVertexDI)
{
  CVertex normal(0.0, 0.0, 0.0);
  list<CDart*> *current_faces = new list<CDart*>;
  list<CDart*> *next_faces;
  list<CDart*> old_faces;
  list<CDart*>::iterator it;
  CDart *d;
  int mark = FMap->getNewMark();

  CCoverage *cov = FMap->getDynamicCoverage(ADart, AOrbit);
  int tmp_mark = FMap->getNewMark();
  FMap->halfMarkOrbit(ADart, AOrbit, tmp_mark);
  for (; cov->cont(); ++(*cov))
    if (FMap->isMarked(**cov, tmp_mark)) {
      assert(!FMap->isMarked(**cov, mark));
      FMap->unmarkOrbit(**cov, ORBIT_01, tmp_mark);
      if (ARegionMark < 0 || FMap->isMarked(**cov, ARegionMark)) {
	normal += faceNormalVector(**cov, AVertexDI);
	current_faces->push_back(**cov);
	FMap->halfMarkOrbit(**cov, ORBIT_01, mark);
      }
    }
  delete cov;
//   assert(FMap->isWholeMapUnmarked(tmp_mark));
  FMap->freeMark(tmp_mark);

  for (unsigned int i = 0; i < ARegionSize; ++i) {
    next_faces = new list<CDart*>;
    while (!current_faces->empty()) {
      d = current_faces->front();
      current_faces->pop_front();
      old_faces.push_back(d);
      for (CDynamicCoverage01 cov(FMap, d); cov.cont(); ++cov)
	if (FMap->isMarked(*cov, mark) && !FMap->isFree2(*cov)) {
	  d = a0(a2(*cov));
	  if (!FMap->isMarked(d, mark) &&
	      (ARegionMark < 0 || FMap->isMarked(d, ARegionMark))) {
	    normal += faceNormalVector(d, AVertexDI);
	    next_faces->push_back(d);
	    FMap->halfMarkOrbit(d, ORBIT_01, mark);
	  }
	}
    }
    delete current_faces;
    current_faces = next_faces;
  }

  for (it = old_faces.begin(); it != old_faces.end(); ++it)
    FMap->unmarkOrbit(*it, ORBIT_01, mark);
  for (it = current_faces->begin(); it != current_faces->end(); ++it)
    FMap->unmarkOrbit(*it, ORBIT_01, mark);
  delete current_faces;

//   assert(FMap->isWholeMapUnmarked(mark));
  FMap->freeMark(mark);

  return normal;
}
//******************************************************************************
void CGeneralTools::displayEdgeVertices(CDart * ADart, int AVertexDI)
{
  cout << "Edge : (" << *VTX(ADart) << ";"
       << *VTX(a0(ADart)) << ")\n";
}
//******************************************************************************
void CGeneralTools::displayFaceVertices(CDart * ADart, int AVertexDI)
{
  cout << "Face : (";

  cout << *VTX(ADart);

  for (CDart * d = a1(a0(ADart)) ; d != ADart ; d = a1(a0(d)))
    cout << ";" << *VTX(d);

  cout << ")\n";
}
//******************************************************************************
#define SIDE1(d) ((CDart*)FMap->getDirectInfo(d, Side1DI))
#define SIDE2(d) ((CDart*)FMap->getDirectInfo(d, Side2DI))

CDart * CGeneralTools::thickenVolume(CDart * AVolume, TCoordinate AThick,
				     int ABorderSteps, int AVertexDI)
{
  assert(ABorderSteps >= 0);

  int mark = FMap->getNewMark();
  int Side1DI = FMap->getNewDirectInfo();
  int Side2DI = FMap->getNewDirectInfo();

  CVertex face_normal, pt;
  CDart *d;

  CDynamicCoverageVolume dcv(FMap, AVolume);

  for (dcv.reinit(); dcv.cont(); ++dcv)
    FMap->setMark(*dcv, mark);

  // Epaississement topologique
  FMap->CGMapGeneric::thickenMarkedDarts(mark, Side1DI, Side2DI);

  // Le bord provenant de l'épaississement est toujours orientable
  FMap->halfMarkOrbit(SIDE1(AVolume), ORBIT_CC, mark);

  // Décalage des sommets intérieurs afin de former un coussin
  for (dcv.reinit(); dcv.cont(); ++dcv) {
    if (FMap->isMarked(*dcv, mark) && FMap->isMarked(SIDE1(*dcv), mark)) {
      // Teste si le sommet se trouve sur un bord du maillage et récupération
      // d'un brin 2-libre si c'est le cas
      d = NULL;
      for (CDynamicCoverage12 cov(FMap, *dcv); cov.cont() && d == NULL; ++cov)
	if (FMap->isFree2(SIDE1(*cov)))
	  d = *cov;

      // S'il ne s'agit pas d'un bord, on décale les sommets
      if (d == NULL) {
	FMap->unmarkOrbit(*dcv, ORBIT_12, mark);

	pt = *VTX(*dcv);

	face_normal = vertexNormalVector(*dcv, AVertexDI);
	face_normal = normalizeVector(face_normal);

	FMap->updateVertex(SIDE1(*dcv), pt + face_normal * AThick);
	FMap->updateVertex(SIDE2(*dcv), pt - face_normal * AThick);
      }
      // Sinon, on créer l'arrondi sur le bord du maillage
      else {
	thickenBorder(d, mark, AThick, ABorderSteps,
		      Side1DI, Side2DI, AVertexDI);
      }
    }
  }

  // Démarquage des brins du coussin
  FMap->unmarkOrbit(SIDE1(AVolume), ORBIT_VOLUME, mark);

  d = SIDE1(AVolume);

  // Libération des champs direct info
  for (dcv.reinit() ; dcv.cont() ; dcv++) {
    FMap->setDirectInfo(*dcv, Side1DI, NULL);
    FMap->setDirectInfo(*dcv, Side2DI, NULL);
  }

  FMap->freeDirectInfo(Side1DI);
  FMap->freeDirectInfo(Side2DI);
//   assert(FMap->isWholeMapUnmarked(mark));
  FMap->freeMark(mark);

  return d;
}

#undef SIDE1
#undef SIDE2
//******************************************************************************

void CGeneralTools::thickenBorder(CDart * ABorder, int AMark,
			          TCoordinate AThick, int ASteps,
			          int ASide1DI, int ASide2DI, int AVertexDI)
{
#define SIDE1(d) ((CDart*)FMap->getDirectInfo(d, ASide1DI))
#define SIDE2(d) ((CDart*)FMap->getDirectInfo(d, ASide2DI))

  CVertex face_normal, edge_normal, pt, v, v1, v2;
  CDart *d[2], *tmp_d[2], *old_d[2], *new_d[8];
  int i, j;

  d[0] = ABorder;

  d[1] = a1(d[0]);
  while (!FMap->isFree2(d[1]))
    d[1] = a1(a2(d[1]));

  do {
    FMap->unmarkOrbit(d[0], ORBIT_12, AMark);

    pt = *VTX(d[0]);

    face_normal = vertexNormalVector(d[0], AVertexDI);
    face_normal = normalizeVector(face_normal);

    tmp_d[0] = SIDE1(d[0]);
    tmp_d[1] = a1(tmp_d[0]);
    while (!FMap->isFree2(tmp_d[1]))
      tmp_d[1] = a1(a2(tmp_d[1]));

    /* ATTENTION !
     * Ici, nous ne sommes pas toujours certain d'avoir tmp_d[1] = SIDE1(d[1])
     * Il est possible d'avoir tmp_d[1] = SIDE2(d[1])
     * Il faut donc tester à chaque fois à quel côté appartient le brin !
     */

    old_d[0] = tmp_d[0];
    old_d[1] = tmp_d[1];

    // Création de la topologie de l'arrondi
    for (i=0 ; i<ASteps+1 ; i++) {

      for (j=0 ; j<8 ; j++)
	new_d[j] = FMap->addMapDart();

      FMap->linkAlpha1(new_d[0], new_d[1]);
      FMap->linkAlpha0(new_d[1], new_d[2]);
      FMap->linkAlpha1(new_d[2], new_d[3]);

      FMap->linkAlpha1(new_d[4], new_d[5]);
      FMap->linkAlpha0(new_d[5], new_d[6]);
      FMap->linkAlpha1(new_d[6], new_d[7]);

      FMap->linkAlpha2(new_d[1], new_d[5]);
      FMap->linkAlpha2(new_d[2], new_d[6]);

      FMap->linkAlpha2(old_d[0], new_d[0]);
      FMap->linkAlpha2(old_d[1], new_d[4]);

      old_d[0] = new_d[3];
      old_d[1] = new_d[7];
    }

    FMap->linkAlpha2(old_d[0], SIDE2(d[0]));
    if (tmp_d[1] == SIDE1(d[1]))
      FMap->linkAlpha2(old_d[1], SIDE2(d[1]));
    else
      FMap->linkAlpha2(old_d[1], SIDE1(d[1]));

    // Liaisons avec les arrondis voisins d'un côté
    if (!FMap->isFree2(a0(tmp_d[0])) &&
	FMap->isFree0(a2(tmp_d[0]))) {

      old_d[0] = tmp_d[0];
      old_d[1] = a0(tmp_d[0]);

      for (int i=0 ; i<ASteps+1 ; i++) {
	old_d[0] = a2(old_d[0]);
	old_d[1] = a2(old_d[1]);

	FMap->linkAlpha0(old_d[0], old_d[1]);

	old_d[0] = a1(a0(a1(old_d[0])));
	old_d[1] = a1(a0(a1(old_d[1])));

	FMap->linkAlpha0(old_d[0], old_d[1]);
      }
    }

    // Liaisons avec les arrondis voisins de l'autre côté
    if (!FMap->isFree2(a0(tmp_d[1])) &&
	FMap->isFree0(a2(tmp_d[1]))) {

      old_d[0] = tmp_d[1];
      old_d[1] = a0(tmp_d[1]);

      for (i=0 ; i<ASteps+1 ; i++) {
	old_d[0] = a2(old_d[0]);
	old_d[1] = a2(old_d[1]);

	FMap->linkAlpha0(old_d[0], old_d[1]);

	old_d[0] = a1(a0(a1(old_d[0])));
	old_d[1] = a1(a0(a1(old_d[1])));

	FMap->linkAlpha0(old_d[0], old_d[1]);
      }
    }

    // Plongements des sommets de l'arrondi
    FMap->updateVertex(SIDE1(d[0]), pt + face_normal * AThick);
    FMap->updateVertex(SIDE2(d[0]), pt - face_normal * AThick);

    v1 = (*VTX(a0(d[0])) - pt) * face_normal;
    v2 = face_normal * (*VTX(a0(d[1])) - pt);

    v1 = normalizeVector(v1);
    v2 = normalizeVector(v2);

    edge_normal = v1 + v2;

    edge_normal = normalizeVector(edge_normal);
    if (!isZero(edge_normal.dot(v1)))
      edge_normal /= edge_normal.dot(v1);

    tmp_d[1] = a2(tmp_d[0]);
    for (int i=0 ; i<ASteps ; i++) {
      tmp_d[1] = a2(a1(a0(a1(tmp_d[1]))));

      v = (cos(((i+1) * M_PI) / (ASteps+1)) * face_normal +
	   sin(((i+1) * M_PI) / (ASteps+1)) * edge_normal);

      FMap->setVertex(tmp_d[1], pt + v * AThick);
    }

    d[0] = a0(d[1]);
    d[1] = a1(d[0]);
    while (!FMap->isFree2(d[1]))
      d[1] = a1(a2(d[1]));
  }
  while (d[0] != ABorder);

#undef SIDE1
#undef SIDE2
}

//******************************************************************************

