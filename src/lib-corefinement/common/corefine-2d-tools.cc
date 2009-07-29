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

#include "corefine-2d-tools.hh"
#include "bounding-box.hh"

// #define WARNING_MESSAGES
// #define DEBUG_MESSAGES
#include "message-macros.hh"

using namespace std;
using namespace GMap3d;

//******************************************************************************

#define PROD(V1, V2) ((V1).getX() * (V2).getY() - (V1).getY() * (V2).getX())

#define a0 FMap->alpha0
#define a1 FMap->alpha1
#define a2 FMap->alpha2
#define a3 FMap->alpha3

#define VTX(d) (AVertexDI < 0 ? FMap->findVertex(d) : \
		(CAttributeVertex*)FMap->getDirectInfo(d, AVertexDI))

//******************************************************************************

CCorefine2dTools::CCorefine2dTools(CGMapVertex * AMap, TCoordinate AEpsilon)
  : CGeneralTools(AMap,AEpsilon)
{
}

//******************************************************************************

bool CCorefine2dTools::isVectorInSector(const CVertex & AVector,
					const CVertex & ASectorVector1,
					const CVertex & ASectorVector2)
{
  if (PROD(ASectorVector1, ASectorVector2) < 0.0)
    return (PROD(ASectorVector1, AVector) >= 0 ||
	    PROD(ASectorVector2, AVector) <= 0);

  return (PROD(ASectorVector1, AVector) >= 0 &&
	  PROD(ASectorVector2, AVector) <= 0);
}

//******************************************************************************

bool CCorefine2dTools::isVectorInSector(const CVertex & AVector,
					CDart * ASector, int AVertexDI)
{
  CVertex center = *VTX(ASector);

  CVertex v1 = *VTX(FMap->alpha0(ASector)) - center;
  CVertex v2 = *VTX(FMap->alpha0(FMap->alpha1(ASector))) - center;

  return isVectorInSector(AVector, v1, v2);
}

//******************************************************************************

bool CCorefine2dTools::areVectorsColinear(const CVertex & AVector1,
					  const CVertex & AVector2)
{
//   if (PROD(AVector1, AVector2) != 0.0)
//     return false;
//   else
//     return AVector1.dot(AVector2) > 0.0;

  CVertex v1 = normalizeVector(AVector1);
  CVertex v2 = normalizeVector(AVector2);

  assert(v1 != ORIGIN);
  assert(v2 != ORIGIN);

  return arePointsEqual(v1, v2);
}

//******************************************************************************

TCoordinate CCorefine2dTools::pointParameterOnLine(const CVertex & APoint,
						   const CVertex & ALineVertex1,
						   const CVertex & ALineVertex2)
{
  TCoordinate res;
  CVertex v = ALineVertex2 - ALineVertex1;

#ifdef WARNING_MESSAGES
  if (isVectorNull(v))
    WARN_BB("<CGeneralTools::pointParameterOnLine> "
	    << "Le vecteur est de longueur nulle : " << v);
#endif

  if (fabs(v.getX()) > fabs(v.getY()))
    res = (APoint.getX() - ALineVertex1.getX()) / v.getX();
  else
    res = (APoint.getY() - ALineVertex1.getY()) / v.getY();

  return res;
}

//******************************************************************************

CDart * CCorefine2dTools::findSectorOfVector(const CVertex & AVector,
					    CDart * AVertex, int AVertexDI)
{
  CDart *d = AVertex;

  if (a1(d) != a2(d))
    while (!isVectorInSector(AVector, d, AVertexDI))
      d = a2(a1(d));

  return d;
}

//******************************************************************************

// list<CDart*> * CCorefine2dTools::sortVerticesEdges(CDart * AVertex1,
// 						  CDart * AVertex2)
// {
//   AVertex2 = findSectorOfVector(getEdgeVector(AVertex1), AVertex2);
  
//   CDart *d1 = a2(a1(AVertex1));
//   CDart *d2 = AVertex2;
//   CVertex v1 = getEdgeVector(a2(a1(AVertex1)));

//   list<CDart*> * l = new list<CDart*>;

//   l->push_back(AVertex2);
//   l->push_back(AVertex1);

//   while (d1 != AVertex1) {
//     if (isVectorInSector(v1, d2)) {
//       l->push_back(d1);
      
//       d1 = a2(a1(d1));
//       v1 = getEdgeVector(d1);
//     }
//     else {
//       d2 = a2(a1(d2));

//       l->push_back(d2);
//     }
//   }

//   d2 = a2(a1(d2));

//   while (d2 != AVertex2) {
//     l->push_back(d2);

//     d2 = a2(a1(d2));
//   }

//   return l;
// }

//******************************************************************************

list<CDart*> * CCorefine2dTools::sortVerticesEdges(CDart * AVertex1,
						   CDart * AVertex2,
						   int AVertexDI)
{
  DEBUG_FUNCTION;

  CDart *d;
  list<CDart*> * l = new list<CDart*>;
  list<CDart*>::iterator it;

  // Cas ou le premier sommet est une extrémité d'un segment seul
  if (a1(AVertex1) == a2(AVertex1)) {
    l->push_back(AVertex1);
    AVertex2 = a2(a1(findSectorOfVector(edgeVector(AVertex1, AVertexDI),
					AVertex2, AVertexDI)));

    d = AVertex2;
    do {
      l->push_back(d);
      d = a2(a1(d));
    }
    while (d != AVertex2);
  }
  // Cas ou le second sommet est une extrémité d'un segment seul
  else if (a1(AVertex2) == a2(AVertex2)) {
    l->push_back(AVertex2);
    AVertex1 = a2(a1(findSectorOfVector(edgeVector(AVertex2, AVertexDI),
					AVertex1, AVertexDI)));

    d = AVertex1;
    do {
      l->push_back(d);
      d = a2(a1(d));
    }
    while (d != AVertex1);
  }
  // Cas restants
  else {
    int mark = FMap->getNewMark();
    CVertex center = *VTX(AVertex1);
    
    d = AVertex1;
    CVertex v = *VTX(a0(d)) - center;
    
    AVertex2 = findSectorOfVector(v, AVertex2, AVertexDI);
    
    CDart *d1 = a2(a1(AVertex1));
    CDart *d2 = a2(a1(AVertex2));
    
    CVertex v1 = *VTX(a0(d1)) - center;
    CVertex v2 = *VTX(a0(d2)) - center;

#define INSERT(dart) (l->push_back(dart), FMap->setMark(dart, mark))
    
    do {
      INSERT(d);

      bool test1 = isVectorInSector(v1, v, v2);
      bool test2 = isVectorInSector(v2, v, v1);

      MSG("v  = "  << center << " --> " << center + v);
      MSG("v1 = "  << center << " --> " << center + v1);
      MSG("v2 = "  << center << " --> " << center + v2);

      MSG("tests :");
      MSG("v1 entre v et v2 : " << test1);
      MSG("v2 entre v et v1 : " << test2);
      
      if (areVectorsColinear(v, v2)) {
	MSG("v et v2 sont colinéaires");
	d = d2;
	v = v2;
	d2 = a2(a1(d2));
	v2 = *VTX(a0(d2)) - center;
      }
      else if (areVectorsColinear(v1, v2)) {
//       else if (test1 == test2 && v1.dot(v2) > 0.0) {
	MSG("v1 et v2 sont colinéaires");

	d = d1;
	v = v1;
	
	if (!FMap->isMarked(d2, mark)) {
	  INSERT(d2);
	}
	
	d2 = a2(a1(d2));
	d1 = a2(a1(d1));
	
	v1 = *VTX(a0(d1)) - center;
	v2 = *VTX(a0(d2)) - center;
      }
      else if (test1 && !test2) {
	MSG("v1 est entre v et v2");

	d = d1;
	v = v1;
	d1 = a2(a1(d1));
	v1 = *VTX(a0(d1)) - center;
      }
      else if (!test1 && test2) {
	MSG("v2 est entre v et v1");

	d = d2;
	v = v2;
	d2 = a2(a1(d2));
	v2 = *VTX(a0(d2)) - center;
      }
      else {
	assert(test1 != test2);
      }
    }
    while (!FMap->isMarked(d, mark));
    
#undef INSERT
    
    bool problem = false;
    
    // Démarquage des brins du deuxième sommet
    d2 = AVertex2;
    do {
      if (FMap->isMarked(d2, mark))
	FMap->unsetMark(d2, mark);
      else {
	WARN_BR("<CCorefine2dTools::sortVerticesEdges> "
		<< "Brin du deuxième sommet non marqué !");
	problem = true;
      }
      
      d2 = a2(a1(d2));
    }
    while (d2 != AVertex2);
    
    // Démarquage des brins du premier sommet
    d1 = AVertex1;
    do {
      if (FMap->isMarked(d1, mark))
	FMap->unsetMark(d1, mark);
      else {
	WARN_BR("<CCorefine2dTools::sortVerticesEdges> "
		<< "Brin du premier sommet non marqué !");
	problem = true;
      }
      
      d1 = a2(a1(d1));
    }
    while (d1 != AVertex1);
    
    FMap->freeMark(mark);
    
    if (problem) {
      cout << "\033[0;33m"
	   << "Tri angulaire autour du sommet " << center << endl;
      for (it = l->begin() ; it != l->end() ; it++) {
	cout << *VTX(a0(*it)) << endl;
      }
      cout << "\033[0m";
      
      delete l;
      l = NULL;
    }
  }

  return l;
}

//******************************************************************************

list<CDart*> *
CCorefine2dTools::sortMultipleVerticesEdges(CDart * AVertex1,
					    const list<CDart*> & AVertices,
					    int AVertexDI)
{
  int mark = FMap->getNewMark();
  list<CDart*> * l = new list<CDart*>;
  list<CDart*>::const_iterator it;
  int size = AVertices.size();
  CDart **darts = new CDart*[size];
  CDart **tmp_darts = new CDart*[size];
  CVertex *vectors = new CVertex[size];
  int i = 0, j;

  CVertex center = *VTX(AVertex1);

  CDart *d = AVertex1;
  CVertex v = *VTX(a0(d)) - center;

  CDart *d1 = a2(a1(AVertex1));
  CVertex v1 = *VTX(a0(d1)) - center;

  for (it = AVertices.begin() ; it != AVertices.end() ; it++) {
    darts[i] = findSectorOfVector(v, *it, AVertexDI);
    tmp_darts[i] = a2(a1(darts[i]));
    vectors[i] = *VTX(a0(tmp_darts[i])) - center;
    i++;
  }

#define INSERT(dart) (l->push_back(dart), FMap->setMark(dart, mark))

  do {
    INSERT(d);

    j = 0;

    // Recherche du vecteur le plus proche
    for (i = 1 ; i < size ; i++)
      if (isVectorInSector(vectors[i], v, vectors[j]))
	j = i;

    if (areVectorsColinear(v1, vectors[j])) {
      d = d1;
      v = v1;

      for (i = 0 ; i < size ; i++)
	if (areVectorsColinear(vectors[i], v1)) {
	  if(!FMap->isMarked(tmp_darts[i], mark)) {
	    INSERT(tmp_darts[i]);
	  }

	  tmp_darts[i] = a2(a1(tmp_darts[i]));
	  vectors[i] = *VTX(a0(tmp_darts[i])) - center;
	}

      d1 = a2(a1(d1));
      v1 = *VTX(a0(d1)) - center;
    }
    else if (isVectorInSector(vectors[j], v, v1)) {
      d = tmp_darts[j];
      v = vectors[j];

      for (i = 0 ; i < size ; i++)
	if (i != j && areVectorsColinear(vectors[i], v)) {
	  if(!FMap->isMarked(tmp_darts[i], mark)) {
	    INSERT(tmp_darts[i]);
	  }

	  tmp_darts[i] = a2(a1(tmp_darts[i]));
	  vectors[i] = *VTX(a0(tmp_darts[i])) - center;
	}

      tmp_darts[j] = a2(a1(tmp_darts[j]));
      vectors[j] = *VTX(a0(tmp_darts[j])) - center;
    }
    else {
      d = d1;
      v = v1;
      d1 = a2(a1(d1));
      v1 = *VTX(a0(d1)) - center;
    }
  }
  while ( /*d != AVertex1*/ !FMap->isMarked(d, mark));

#undef INSERT

  bool problem = false;

  // Démarquage des brins des sommets
  for (i = 0 ; i < size ; i++) {
    tmp_darts[i] = darts[i];

    do {
      if (FMap->isMarked(tmp_darts[i], mark))
	FMap->unsetMark(tmp_darts[i], mark);
      else {
	WARN_BR("<CCorefine2dTools::sortMultipleVerticesEdges> "
		<< "Brin du sommet n°" << i << " non marqué !");
	problem = true;
    }

      tmp_darts[i] = a2(a1(tmp_darts[i]));
    }
    while (tmp_darts[i] != darts[i]);
  }

  // Démarquage des brins du premier sommet
  d1 = AVertex1;
  do {
    if (FMap->isMarked(d1, mark))
      FMap->unsetMark(d1, mark);
    else {
      WARN_BR("<CCorefine2dTools::sortMultipleVerticesEdges> "
	      << "Brin du premier sommet non marqué !");
      problem = true;
    }

    d1 = a2(a1(d1));
  }
  while (d1 != AVertex1);

  FMap->freeMark(mark);

  delete [] darts;
  delete [] tmp_darts;
  delete [] vectors;

  if (problem) {
#ifdef WARNING_MESSAGES
    cout << "\033[1;34m" << "Tri angulaire autour du sommet " << center << endl;
    for (it = l->begin() ; it != l->end() ; it++)
      cout << *VTX(a0(*it)) << endl;
    cout << "\033[0m";
#endif

    delete l;

    return NULL;
  }

  return l;
}

//******************************************************************************

CDart * CCorefine2dTools::findWellOrientedDart(CDart * AVertex, int AVertexDI)
{
  CVertex v1, v2;

  v1 = faceNormalVector(AVertex, AVertexDI);
  v2 = faceNormalVector(a2(AVertex), AVertexDI);

  if ((v1.getZ() < 0.0 && v2.getZ() < 0.0) ||
      (v1.getZ() > 0.0 && v2.getZ() > 0.0)) {
    /* Nous sommes ici dans le cas où une des deux faces est une face
     * extérieure et nous devons donc comparer la taille des deux faces.
     */
    CBoundingBox bb1, bb2;

    bb1 = orbitBoundingBox(AVertex, ORBIT_FACE, AVertexDI);
    bb2 = orbitBoundingBox(a2(AVertex), ORBIT_FACE, AVertexDI);

    if (bb1.getSurface() <= bb2.getSurface()) {
      // AVertex n'est pas la face extérieure, sa normale doit donc être > 0
      if (v1.getZ() < 0.0) {
	AVertex = a1(AVertex);
      }
    }
    else {
      // AVertex est la face extérieure, sa normale doit donc être < 0
      if (v1.getZ() > 0.0) {
	AVertex = a1(AVertex);
      }
    }
  }
  else if (v1.getZ() < 0.0) {
    AVertex = a1(AVertex);
  }

  return AVertex;
}

//******************************************************************************

TPositionOnEdge
CCorefine2dTools::localizePointOnEdge(const CVertex & APoint,
				      const CVertex & AVertex1,
				      const CVertex & AVertex2,
				      TCoordinate * AParam)
{
  assert(AParam != NULL);

  if (arePointsEqual(APoint, AVertex2)) {
    *AParam = 1.0;
    return EP_OnSecondVertex;
  }

  if (arePointsEqual(APoint, AVertex1)) {
    *AParam = 0.0;
    return EP_OnFirstVertex;
  }

  if (isPointOnLine(APoint, AVertex1, AVertex2)) {
    *AParam = pointParameterOnLine(APoint, AVertex1, AVertex2);

    if (*AParam >= 0.0 && *AParam <= 1.0)
      return EP_OnEdge;
  }

  return EP_OutOfEdge;
}

//******************************************************************************

TPositionOnEdge
CCorefine2dTools::localizeEdgesIntersection(const CVertex & ALinePoint1,
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

  CVertex v[2] = {ALinePoint2 - ALinePoint1, AEdgePoint2 - AEdgePoint1};
  TCoordinate d = v[0].getY() * v[1].getX() - v[0].getX() * v[1].getY();

#ifdef WARNING_MESSAGES
  if (isVectorNull(v[0]) || isVectorNull(v[1]))
    WARN_BB("<localizeEdgesIntersection>"
	    << "Une des arêtes est de longueur nulle !");
#endif

  if (d == 0.0)
    return EP_OutOfEdge;

  *ALineParam = (v[1].getY() * (ALinePoint1.getX() - AEdgePoint1.getX()) -
		 v[1].getX() * (ALinePoint1.getY() - AEdgePoint1.getY())) / d;

  if (fabs(v[1].getX()) > fabs(v[1].getY()))
    *AEdgeParam = (ALinePoint1.getX() + *ALineParam * v[0].getX() -
		   AEdgePoint1.getX()) / v[1].getX();
  else
    *AEdgeParam = (ALinePoint1.getY() + *ALineParam * v[0].getY() -
		   AEdgePoint1.getY()) / v[1].getY();

//   CVertex pt = AEdgePoint1 + *AEdgeParam * v[1];

//   if (arePointsEqual(pt, AEdgePoint2))
//     return EP_OnSecondVertex;

//   if (arePointsEqual(pt, AEdgePoint1))
//     return EP_OnFirstVertex;
  
  if (*AEdgeParam >= 0.0 && *AEdgeParam <= 1.0)
    return EP_OnEdge;

  return EP_OutOfEdge;
}

//******************************************************************************

CEdgeIntersection
CCorefine2dTools::findNearestIntersection(const CVertex & AVertex1,
					  const CVertex & AVertex2,
					  CDart * AFace,
					  bool AFaceIsVertex1,
					  int AVertexDI)
{
  CDart *d = AFace;
  CEdgeIntersection inter;
  int edge_mark = FMap->getNewMark();
  TPositionOnEdge pos;
  TCoordinate t1 = 0.0, t2 = 0.0, best_t2 = 1.0;
  CVertex *pt1, *pt2;

  //if (arePointsEqual(AVertex1, AVertex2))
  if (AVertex1 == AVertex2) {
    FMap->freeMark(edge_mark);
    return inter;
  }

  // Parcours des sommets de la face
  do {
    pos = localizePointOnEdge(*VTX(d), AVertex1, AVertex2, &t2);

    if ((pos == EP_OnEdge && t2 < best_t2) ||
	(pos == EP_OnSecondVertex && t2 <= best_t2) ||
	(pos == EP_OnFirstVertex && AFaceIsVertex1 && d != AFace) ||
	(pos == EP_OnFirstVertex && !AFaceIsVertex1)) {
      inter.setCell(d);
      inter.setCellDimension(0);
      inter.setPosition(pos);

      best_t2 = t2;

      FMap->setMark(d, edge_mark);
      FMap->setMark(a0(d), edge_mark);
      FMap->setMark(a1(d), edge_mark);
      FMap->setMark(a0(a1(d)), edge_mark);
    }

    d = a1(a0(d));
  }
  while (d != AFace);

  // Parcours des arêtes de la face
  do {
    pt1 = VTX(d);
    pt2 = VTX(a0(d));

    if (FMap->isMarked(d, edge_mark)) {
      FMap->unsetMark(d, edge_mark);
      FMap->unsetMark(a0(d), edge_mark);
    }
    else if (fabs((*pt2 - *pt1).norm()) > FEps / 2.0) {
      pos = localizeEdgesIntersection(*pt1, *pt2, AVertex1, AVertex2, &t1, &t2);

      if ((t1 >= 0.0 && t1 <= 1.0) &&
	  ((pos == EP_OnEdge && t2 < best_t2) ||
	   (pos == EP_OnSecondVertex && t2 <= best_t2) ||
	   (pos == EP_OnFirstVertex && AFaceIsVertex1 &&
	    d != AFace && d != a0(a1(AFace))) ||
	   (pos == EP_OnFirstVertex && !AFaceIsVertex1))) {
	inter.setCell(d);
	inter.setCellDimension(1);
	inter.setPosition(pos);

	best_t2 = t2;
      }
    }

    d = a1(a0(d));
  }
  while (d != AFace);

  FMap->freeMark(edge_mark);

  if (inter.getCell() != NULL)
    inter.setPoint(AVertex1 + best_t2 * (AVertex2 - AVertex1));

  return inter;
}

//******************************************************************************

CDart * CCorefine2dTools::localizePointInMesh(const CVertex & APoint,
					      CDart * AMesh, int AVertexDI)
{
  CDart *face = NULL;
  CEdgeIntersection inter;
  CVertex pt;

  inter.setCell(AMesh);
  inter.setCellDimension(0);

  do {
    switch (inter.getCellDimension()) {
    case 0:
      pt = *VTX(inter.getCell());
      face = findSectorOfVector(APoint - pt, inter.getCell(), AVertexDI);
      break;

    case 1:
      pt = inter.getPoint();
      face = a2(a0(inter.getCell()));
      /* boucle infinie dans certains cas !!! :( */
      break;

    default:
      break;
    }

//     cout << "Dim = " << inter.getCellDimension() << endl;

    inter = findNearestIntersection(pt, APoint, face, true, AVertexDI);
  }
  while (inter.getCell() != NULL);

  return face;
}

//******************************************************************************

void CCorefine2dTools::removeDoubleEdges(CDart *& AMesh1, CDart *& AMesh2, bitset<NB_MARKS> ACopyMarks, int AVertexDI)
{
  CDart *d, *to_link[4], *edge[4], *to_remove[4];
  int remove_mark = FMap->getNewMark();
  CVertex pt1, pt2;

  for (CStaticCoverageCC scc(FMap, AMesh1) ; scc.cont() ; scc++) {
    d = *scc;

    if (!FMap->isMarked(a1(d), remove_mark)) {
      if (a1(a0(d)) == a0(a1(d)) || a0(a2(a1(d))) == a1(a2(a0(d)))) {
	edge[0] = d;
	edge[1] = a0(edge[0]);
	edge[2] = a2(edge[1]);
	edge[3] = a0(edge[2]);

	to_remove[0] = a2(a1(d));
	to_remove[1] = a0(to_remove[0]);
	to_remove[2] = a2(to_remove[1]);
	to_remove[3] = a0(to_remove[2]);

	if (a1(a0(d)) == a0(a1(d))) {
	  to_link[0] = d;
	  to_link[1] = a0(d);
	  to_link[2] = a1(to_remove[0]);
	  to_link[3] = a1(to_remove[1]);
	}
	else { // a0(a2(a1(d))) == a1(a2(a0(d)))
	  to_link[0] = d;
	  to_link[1] = a0(a2(d));
	  to_link[2] = a1(to_remove[0]);
	  to_link[3] = a1(to_remove[2]);
	}

	for (int i=0 ; i<4 ; i++) {
	  if (!FMap->isFree1(to_link[i]))
	    FMap->dartUnsew1(to_link[i]);

	  FMap->setMark(to_remove[i], remove_mark);

	  if (AMesh1 == to_remove[i])
	    AMesh1 = edge[i];
	  if (AMesh2 == to_remove[i])
	    AMesh2 = edge[i];

	  for (int j = 0; j < NB_MARKS; j++)
	    if (ACopyMarks[j] && FMap->isMarked(to_remove[i], j))
	      FMap->setMark(edge[i], j);
	}
	
	FMap->dartSew1(to_link[0], to_link[2]);
	FMap->dartSew1(to_link[1], to_link[3]);

	if (AVertexDI >= 0) {
	  FMap->pointDirectInfoToAttributeVertex(AVertexDI, to_link[0]);
	  FMap->pointDirectInfoToAttributeVertex(AVertexDI, to_link[1]);
	}
      }
    }
  }

  FMap->deleteMarkedDarts(remove_mark);

  FMap->freeMark(remove_mark);
}

//******************************************************************************
