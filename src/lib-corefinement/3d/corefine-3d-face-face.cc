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

#include <set>
#include <cfloat>
#include <sstream>

// #define DEBUG_MESSAGES
// #define DEBUG_MARKS
// #define SAVE_STEPS_SINCE 11090
// #define SAVE_STEPS_UNTIL 200
#include "message-macros.hh"

#include "corefine-3d-face-face.hh"

using namespace std;
using namespace GMap3d;

//******************************************************************************

#define a0 FMap->alpha0
#define a1 FMap->alpha1
#define a2 FMap->alpha2
#define a3 FMap->alpha3

// #define VTX(d)  (FMap->findVertex(d))
#define VTX(d)  ((CAttributeVertex*)FMap->getDirectInfo(d, FVertexDI))
#define LINK(d) ((CDart*)FMap->getDirectInfo(d, FLinkedVertexDI))

#ifdef DEBUG_MARKS
#define freeMark(m) (assert(FMap->isWholeMapUnmarked(m)), FMap->freeMark(m))
#else  // DEBUG_MARKS
#define freeMark(m) (FMap->freeMark(m))
#endif // DEBUG_MARKS

//******************************************************************************

struct SFaceListIterators
{
  SFaceListIterators() {}
  SFaceListIterators(TCorefFaceList::iterator f1, TCorefFaceList::iterator l1,
		     TCorefFaceList::iterator f2, TCorefFaceList::iterator l2)
    : first1(f1), last1(l1), first2(f2), last2(l2) {}
  TCorefFaceList::iterator first1, last1, first2, last2;
};

//******************************************************************************

CCorefine3dFF::CCorefine3dFF(CGMapVertex * AMap, bool ACalculateOrientation,
			     TCoordinate AEpsilon, int AVertexDI)
  : CCorefine(AMap, AEpsilon), FCalculateOrientation(ACalculateOrientation),
    FOptimizeSearch(true), FGridResolution(64), FDisplayMessages(2)
{
  FTools = new CCorefine3dTools(FMap, AEpsilon);
  CBoundingBox::setEpsilon(AEpsilon);

  if (AVertexDI < 0) {
    FLocalVertexDI = true;
    FVertexDI = FMap->getNewDirectInfo();
  }
  else {
    FLocalVertexDI = false;
    FVertexDI = AVertexDI;
  }
  FMesh1 = FMesh2 = NULL;
}

//******************************************************************************

CCorefine3dFF::~CCorefine3dFF()
{
  if (FLocalVertexDI)
    FMap->freeDirectInfo(FVertexDI);
  clear();
  delete FTools;
}

//******************************************************************************

int CCorefine3dFF::corefine(CDart *& AMesh1, CDart *& AMesh2,
			    bitset<NB_MARKS> ACopyMarks)
{
  CDart *meshes[2] = {AMesh1, AMesh2};

  splitMeshes(AMesh1, AMesh2, ACopyMarks);
  mergeMeshes();
  removeDoubleFaces(meshes, 2);

  AMesh1 = meshes[0];
  AMesh2 = meshes[1];

  return FInterEdges.size() / 2;
}

//******************************************************************************

int CCorefine3dFF::corefine(int AMark1, int AMark2,
			    bitset<NB_MARKS> ACopyMarks)
{
  splitMeshes(AMark1, AMark2, ACopyMarks);
  mergeMeshes();
  removeDoubleFaces();

  return FInterEdges.size() / 2;
}

//******************************************************************************

void CCorefine3dFF::splitMeshes(CDart * AMesh1, CDart * AMesh2,
				bitset<NB_MARKS> ACopyMarks)
{
  DEBUG_FUNCTION;

  CTime start, end;

  FInterPointDI = FMap->getNewDirectInfo();
  FLinkedVertexDI = FMap->getNewDirectInfo();
  FFictiveVertexMark = FMap->getNewMark();
  FFictiveEdgeMark = FMap->getNewMark();

  clear();

  if (FDisplayMessages) cout << "Initialisation des maillages" << endl;
  start.updateTime();
  CBoundingBox box1 = initMesh(AMesh1);
  CBoundingBox box2 = initMesh(AMesh2);
  end.updateTime();
  FInitialisationTime = end - start;
  if (FDisplayMessages) cout << "Durée de l'initialisation : "
			     << FInitialisationTime << endl;

  if (FCalculateOrientation) {
    if (FDisplayMessages) cout << "Orientation des maillages" << endl;
    start.updateTime();
    AMesh1 = FTools->findWellOrientedDart(AMesh1, FVertexDI);
    AMesh2 = FTools->findWellOrientedDart(AMesh2, FVertexDI);
    end.updateTime();
    if (FDisplayMessages) cout << "Durée de l'orientation : "
			       << end - start << endl;
  }

  if (FDisplayMessages) cout << "Construction des listes de faces" << endl;
  start.updateTime();
  FMesh1 = buildFaceList(AMesh1);
  FMesh2 = buildFaceList(AMesh2);
  end.updateTime();
  FFaceListCreationTime = end - start;
  if (FDisplayMessages) cout << "Durée de la construction des listes : "
			     << FFaceListCreationTime << endl;
  if (FDisplayMessages)
    cout << "Le premier objet possède " << FMesh1->size()
	 << (FMesh1->size() > 1 ? " faces" : " face") << endl
	 << "Le second objet possède " << FMesh2->size()
	 << (FMesh2->size() > 1 ? " faces" : " face")<< endl;

  if (FOptimizeSearch) {
    if (FDisplayMessages) cout << "Réduction de la taille des listes" << endl;
    start.updateTime();
    reduceFaceLists(FMesh1, FMesh2, box1 + box2);
    end.updateTime();
    FFaceListReductionTime = end - start;
    if (FDisplayMessages) cout << "Durée de la réduction : "
			       << FFaceListReductionTime << endl;
  }
  
  splitMeshes(ACopyMarks);

  if (FDisplayMessages) cout << "Nettoyage des maillages" << endl;
  start.updateTime();
  cleanMesh(AMesh1);
  cleanMesh(AMesh2);
  end.updateTime();
  if (FDisplayMessages) cout << "Durée du nettoyage : " << end - start << endl;

  FMap->freeDirectInfo(FInterPointDI);
  FMap->freeDirectInfo(FLinkedVertexDI);
  freeMark(FFictiveVertexMark);
  freeMark(FFictiveEdgeMark);
}

//******************************************************************************

void CCorefine3dFF::splitMeshes(int AMark1, int AMark2,
				bitset<NB_MARKS> ACopyMarks)
{
  DEBUG_FUNCTION;

  CTime start, end;

  FInterPointDI = FMap->getNewDirectInfo();
  FLinkedVertexDI = FMap->getNewDirectInfo();
  FFictiveVertexMark = FMap->getNewMark();
  FFictiveEdgeMark = FMap->getNewMark();

  clear();

  if (FDisplayMessages) cout << "Initialisation des faces" << endl;
  start.updateTime();
  CBoundingBox box1 = initFaces(AMark1);
  CBoundingBox box2 = initFaces(AMark2);
  end.updateTime();
  if (FDisplayMessages) cout << "Durée de l'initialisation : "
			     << end - start << endl;

  if (FDisplayMessages) cout << "Construction des listes de faces" << endl;
  start.updateTime();
  FMesh1 = buildFaceList(AMark1);
  FMesh2 = buildFaceList(AMark2);
  end.updateTime();
  if (FDisplayMessages) cout << "Durée de la construction des listes : "
			     << end - start << endl;
  if (FDisplayMessages)
    cout << "Le premier objet possède " << FMesh1->size()
	 << (FMesh1->size() > 1 ? " faces" : " face") << endl
	 << "Le second objet possède " << FMesh2->size()
	 << (FMesh2->size() > 1 ? " faces" : " face")<< endl;

  if (FOptimizeSearch) {
    if (FDisplayMessages) cout << "Réduction de la taille des listes" << endl;
    start.updateTime();
    reduceFaceLists(FMesh1, FMesh2, box1 + box2);
    end.updateTime();
    if (FDisplayMessages) cout << "Durée de la réduction : "
			       << end - start << endl;
  }
  
  splitMeshes(ACopyMarks);

  if (FDisplayMessages) cout << "Nettoyage des faces" << endl;
  start.updateTime();
  for (CDynamicCoverageAll dca(FMap); dca.cont(); ++dca) {
    FMap->unsetMark(*dca, FFictiveVertexMark);
    FMap->unsetMark(*dca, FFictiveEdgeMark);
  }
  end.updateTime();
  if (FDisplayMessages) cout << "Durée du nettoyage : " << end - start << endl;

  FMap->freeDirectInfo(FInterPointDI);
  FMap->freeDirectInfo(FLinkedVertexDI);
  freeMark(FFictiveVertexMark);
  freeMark(FFictiveEdgeMark);
}

//******************************************************************************

void CCorefine3dFF::mergeMeshes()
{
  if (!FInterEdges.empty()) {
    CTime start, end;

    if (FDisplayMessages) cout << "Mise à jour de le topologie" << endl;
    start.updateTime();

    int face_plane_di = FMap->getNewDirectInfo();
    int negative_mark = FMap->getNewMark();

    for (CDynamicCoverageAll dca(FMap); dca.cont(); ++dca)
      FMap->setDirectInfo(*dca, face_plane_di, NULL);

    assignFacesPlaneInfo(face_plane_di, negative_mark, FMesh1);
    assignFacesPlaneInfo(face_plane_di, negative_mark, FMesh2);

    sortFacesAroundEdges(face_plane_di, negative_mark);

    removeFacesPlaneInfo(face_plane_di, negative_mark, FMesh1);
    removeFacesPlaneInfo(face_plane_di, negative_mark, FMesh2);

    FMap->freeDirectInfo(face_plane_di);
    freeMark(negative_mark);

    end.updateTime();
    FMergeTime = end - start;
    if (FDisplayMessages) cout << "Durée de la mise à jour : "
			       << FMergeTime << endl;
  }
}

//******************************************************************************

void CCorefine3dFF::removeDoubleFaces(CDart **ADarts, int ANbDarts)
{
  DEBUG_FUNCTION;

  if (!FDoubleFaces.empty()) {
    int mark = FMap->getNewMark();
    int i;
    CTime start, end;

    if (FDisplayMessages) cout << "Suppression des faces doubles" << endl;
    start.updateTime();

    markDoubleFaces(mark);

    for (i = 0; i < ANbDarts; ++i)
      if (FMap->isMarked(ADarts[i], mark)) {
	CDart *d = ADarts[i];
	do d = a3(FTools->alpha2(d));
	while (d != ADarts[i] && FMap->isMarked(d, mark));
	ADarts[i] = (d == ADarts[i] ? NULL : d);
      }

    TCorefFaceList::iterator fit, tmp_fit;
    TCorefFaceList *face_list[2] = {FMesh1, FMesh2};
    for (i = 0; i < 2; ++i)
      for (fit = face_list[i]->begin(); fit != face_list[i]->end(); ) {
	tmp_fit = fit++;
	if (FMap->isMarked(tmp_fit->face, mark))
	  face_list[i]->erase(tmp_fit);
      }

    list<CDart*>::iterator it;
    for (it = FInterEdges.begin(); it != FInterEdges.end(); ++it)
      if (FMap->isMarked(*it, mark))
	*it = NULL;

    while (!FDoubleFaces.empty()) {
      removeDoubleFace(FDoubleFaces.front());
      FDoubleFaces.pop_front();
    }

    freeMark(mark);

    end.updateTime();
    if (FDisplayMessages) cout << "Durée de la suppression : "
			       << end - start << endl;
  }
}

//******************************************************************************

// void CCorefine3dFF::linkCompounds(CDart * AMesh1, CDart * AMesh2,
// 				  std::bitset<NB_MARKS> ACopyMarks)
// {
//   DEBUG_FUNCTION;

//   if (FInterEdges.empty()) {
//     CDart *d[2] = {AMesh1, AMesh2};
//     CVertex line = *VTX(d[1]) - *VTX(d[0]);

//     if (FTools->isVectorNull(line)) {
//       d[0] = a1(a0(d[0]));
//       line = *VTX(d[1]) - *VTX(d[0]);
//       assert(!FTools->isVectorNull(line));
//     }

//     CEdgeIntersection inter[2];
//     int mark = FMap->getNewMark();
//     CVertex normal;

//     MSG("Recherche d'une arête sur le premier objet");
//     for (int i = 0; i < 2; ++i) {
//       inter[i] = FTools->findNearestIntersectionInOrbit(*VTX(d[(i+1)%2]),
// 							-line, d[i],
// 							ORBIT_CC, FVertexDI);
//       assert(inter[i].getCell());
//       d[i] = NULL;
//       switch (inter[i].getCellDimension()) {
//       case 0:
// 	FMap->halfMarkOrbit(inter[i].getCell(), ORBIT_VERTEX, mark);
// 	for (CDynamicCoverageVertex dcv(FMap, inter[i].getCell());
// 	     dcv.cont(); ++dcv)
// 	  if (FMap->isMarked(*dcv, mark)) {
// 	    normal = ORIGIN;
// 	    for (CDynamicCoverage12 cov(FMap, *dcv); cov.cont(); ++cov)
// 	      if (FMap->isMarked(*cov, mark)) {
// 		FMap->unsetMark(*dcv, mark);
// 		normal += FTools->faceNormalVector(*cov, FVertexDI);
// 	      }
// 	    if (normal.dot(line) > 0.0)
// 	      d[i] = *dcv;
// 	  }
// 	break;
//       case 1:
// 	FMap->halfMarkOrbit(inter[i].getCell(), ORBIT_EDGE, mark);
// 	for (CDynamicCoverageEdge dce(FMap, inter[i].getCell());
// 	     dce.cont(); ++dce)
// 	  if (FMap->isMarked(*dce, mark)) {
// 	    FMap->unsetMark(*dce, mark);
// 	    normal = FTools->faceNormalVector(*dce, FVertexDI);
// 	    if (!FMap->isFree2(*dce)) {
// 	      FMap->unsetMark(a0(a2(*dce)), mark);
// 	      normal += FTools->faceNormalVector(a0(a2(*dce)), FVertexDI);
// 	    }
// 	    if (normal.dot(line) > 0.0)
// 	      d[i] = *dce;
// 	  }
// 	break;
//       case 2:
// 	normal = FTools->faceNormalVector(inter[i].getCell(), FVertexDI);
// 	if (normal.dot(line) > 0.0)
// 	  d[i] = inter[i].getCell();
// 	else {
// 	  d[i] = a1(a3(inter[i].getCell()));
// 	  assert(FTools->faceNormalVector(d[i], FVertexDI).dot(line) > 0.0);
// 	}
//       }
//       assert(d[i] != NULL);
//       line = -line;
//     }

//     assert(FMap->isWholeMapUnmarked(mark));
//     freeMark(mark);

//     CDart *square = FMap->createTopoSquare();
//     FMap->topoSew3(square, FMap->createTopoSquare());
// //     FMap->topoSew2(a1(square), a3(a1(square)));
// //     FMap->topoSew2(a1(a0(square)), a3(a1(a0(square))));

//     CDart *a2d[2] = {FTools->alpha2(d[0]), FTools->alpha2(d[1])};
    
//     for (int i = 0; i < 2; ++i) {
//       if (!FMap->isFree2(d[i]))
// 	FMap->unsew2(d[i]);
//       FMap->sew2(square, d[i]);
//       FMap->sew2(a3(square), a2d[i]);
//       square = a1(a0(a1(a0(square))));
//     }

//     for (int m = 0; m < NB_MARKS; ++m)
//       if (ACopyMarks[m])
// 	for (int i = 0; i < 2; ++i) {
// 	  if (FMap->isMarked(d[i], m))
// 	    FMap->markOrbit(a2(d[i]), ORBIT_01, m);
// 	  if (FMap->isMarked(a2d[i], m))
// 	    FMap->markOrbit(a2(a2d[i]), ORBIT_01, m);
// 	}

//     for (int i = 0; i < 2; ++i) {
//       FMap->pointDirectInfoToAttributeVertex(FVertexDI, d[i]);
//       FMap->pointDirectInfoToAttributeVertex(FVertexDI, a0(d[i]));
//     }
//   }
// }

//******************************************************************************

void CCorefine3dFF::linkCompounds(CDart * AMesh1, CDart * AMesh2,
				  std::bitset<NB_MARKS> ACopyMarks)
{
  DEBUG_FUNCTION;

  if (FInterEdges.empty()) {
    CDart *d1 = AMesh1, *d2 = AMesh2;
    CVertex line = *VTX(d2) - *VTX(d1);

    if (line.getX() == 0.0 && line.getY() == 0.0 && line.getZ() == 0.0) {
      d1 = a1(a0(d1));
      line = *VTX(d2) - *VTX(d1);
    }

    CPlane ref_plane(line, *VTX(d1));
    TCoordinate dist, best_dist = 0.0;
    CVertex normal;
    int mark = FMap->getNewMark();

    MSG("Recherche d'un sommet de l'enveloppe convexe du premier objet");
    FMap->halfMarkOrbit(d1, ORBIT_CC, mark);
    FMap->unmarkOrbit(d1, ORBIT_VERTEX, mark);
    for (CDynamicCoverageCC dcc(FMap, d1); dcc.cont(); ++dcc)
      if (FMap->isMarked(*dcc, mark)) {
	FMap->unmarkOrbit(*dcc, ORBIT_VERTEX, mark);
	dist = ref_plane.pointDistance(*VTX(*dcc));
	if (dist > best_dist) {
	  d1 = *dcc;
	  best_dist = dist;
	}
      }

    MSG("Calcul d'un nouveau vecteur directeur");
    line = *VTX(d2) - *VTX(d1);
    if (line.getX() == 0.0 && line.getY() == 0.0 && line.getZ() == 0.0) {
      d2 = a1(a0(d2));
      line = *VTX(d2) - *VTX(d1);
    }

    MSG("Recherche d'une arête de l'enveloppe convexe du premier objet");
    ref_plane.setPlane(line, *VTX(d1));
    best_dist = -FLT_MAX;
    FMap->halfMarkOrbit(d1, ORBIT_VERTEX, mark);
    for (CDynamicCoverageVertex dcv(FMap, d1); dcv.cont(); ++dcv)
      if (FMap->isMarked(*dcv, mark)) {
	FMap->unsetMark(*dcv, mark);
	normal = FTools->faceNormalVector(*dcv, FVertexDI);
	if (!FMap->isFree2(*dcv))
	  normal += FTools->faceNormalVector(a0(a2(*dcv)), FVertexDI);
	if (normal.dot(line) > 0.0) {
	  dist = ref_plane.pointDistance(*VTX(a0(*dcv)));
	  if (dist > best_dist) {
	    d1 = *dcv;
	    best_dist = dist;
	  }
	}
      }

    MSG("Recherche de la plus proche cellule du deuxième objet");
    CEdgeIntersection inter;
    inter = FTools->findNearestIntersectionInOrbit(*VTX(d1), line, d2,
						   ORBIT_CC, FVertexDI);
    assert(inter.getCell());
    d2 = inter.getCell();

    MSG("Recherche d'une arête de l'enveloppe convexe du deuxième objet");
    TOrbit cell_orbit = ORBIT_VERTEX;
    switch (inter.getCellDimension()) {
    case 1: cell_orbit = ORBIT_EDGE;
    case 2: cell_orbit = ORBIT_FACE;
    }
    ref_plane.setPlane(-line, *VTX(d2));
    best_dist = -FLT_MAX;
    FMap->halfMarkOrbit(d2, cell_orbit, mark);
    CCoverage *cov = FMap->getDynamicCoverage(d2, cell_orbit);
    for (; cov->cont(); ++(*cov))
      if (FMap->isMarked(**cov, mark)) {
	FMap->unsetMark(**cov, mark);
	normal = FTools->faceNormalVector(**cov, FVertexDI);
	if (!FMap->isFree2(**cov))
	  normal += FTools->faceNormalVector(a0(a2(**cov)), FVertexDI);
	if (normal.dot(-line) > 0.0) {
	  dist = ref_plane.pointDistance(*VTX(a0(**cov)));
	  if (dist > best_dist) {
	    d2 = **cov;
	    best_dist = dist;
	  }
	}
      }
    delete cov;

    if ((*VTX(a0(d1)) - *VTX(d1)).dot(*VTX(a0(d2)) - *VTX(d2)) > 0.0)
      d2 = FTools->alpha2(a0(d2));

    freeMark(mark);

    CDart *square = FMap->createTopoSquare();
    FMap->topoSew3(square, FMap->createTopoSquare());
//     FMap->topoSew2(a1(square), a3(a1(square)));
//     FMap->topoSew2(a1(a0(square)), a3(a1(a0(square))));

    CDart *a2d1 = FTools->alpha2(d1);
    CDart *a2d2 = FTools->alpha2(d2);
    
    if (!FMap->isFree2(d1)) FMap->unsew2(d1);
    if (!FMap->isFree2(d2)) FMap->unsew2(d2);

    FMap->sew2(square, d1);
    FMap->sew2(a3(square), a2d1);
    square = a3(a1(a0(a1(a0(square)))));
    FMap->sew2(square, d2);
    FMap->sew2(a3(square), a2d2);

    for (int m = 0; m < NB_MARKS; ++m)
      if (ACopyMarks[m]) {
	if (FMap->isMarked(d1, m))   FMap->markOrbit(a2(d1),   ORBIT_01, m);
	if (FMap->isMarked(a2d1, m)) FMap->markOrbit(a2(a2d1), ORBIT_01, m);
	if (FMap->isMarked(d2, m))   FMap->markOrbit(a2(d2),   ORBIT_01, m);
	if (FMap->isMarked(a2d2, m)) FMap->markOrbit(a2(a2d2), ORBIT_01, m);
      }

    FMap->pointDirectInfoToAttributeVertex(FVertexDI, d1);
    FMap->pointDirectInfoToAttributeVertex(FVertexDI, a0(d1));
    FMap->pointDirectInfoToAttributeVertex(FVertexDI, d2);
    FMap->pointDirectInfoToAttributeVertex(FVertexDI, a0(d2));
  }
}

//******************************************************************************

void CCorefine3dFF::markIntersectionEdges(int AMark, int AObjectsToMark)
{
  CDart *edge1, *edge2;
  for (list<CDart*>::iterator it = FInterEdges.begin();
       it != FInterEdges.end(); ) {
    edge1 = *it++;
    edge2 = *it++;
    if (AObjectsToMark & 1 && edge1 && !FMap->isMarked(edge1, AMark))
      FMap->markOrbit(edge1, ORBIT_EDGE, AMark);
    if (AObjectsToMark & 2 && edge2 && !FMap->isMarked(edge2, AMark))
      FMap->markOrbit(edge2, ORBIT_EDGE, AMark);
  }
}

//******************************************************************************

void CCorefine3dFF::markDoubleFaces(int AMark)
{
  for (list<CDart*>::iterator it = FDoubleFaces.begin();
       it != FDoubleFaces.end(); ++it)
    if (!FMap->isMarked(*it, AMark))
      FMap->markOrbit(*it, ORBIT_FACE, AMark);
}

//******************************************************************************

void CCorefine3dFF::spreadMarksWithoutMerging(bitset<NB_MARKS> AMarks)
{
  int face_plane_di = FMap->getNewDirectInfo();
  int negative_mark = FMap->getNewMark();
  CDart *edge1, *edge2;
  TFaceSet *face_set;

  assignFacesPlaneInfo(face_plane_di, negative_mark, FMesh1);
  assignFacesPlaneInfo(face_plane_di, negative_mark, FMesh2);

  for (list<CDart*>::iterator it = FInterEdges.begin();
       it != FInterEdges.end(); ) {
    edge1 = *it++;
    edge2 = *it++;
    if (edge1 && edge2) {
      face_set = sortFacesAroundEdges(edge1, edge2,
				      face_plane_di, negative_mark);
      spreadMarksAroundEdges(face_set, AMarks);
      delete face_set;
    }
  }

  removeFacesPlaneInfo(face_plane_di, negative_mark, FMesh1);
  removeFacesPlaneInfo(face_plane_di, negative_mark, FMesh2);
  
  FMap->freeDirectInfo(face_plane_di);
  freeMark(negative_mark);
}

//******************************************************************************

void CCorefine3dFF::clear()
{
  if (FMesh1) delete FMesh1;
  if (FMesh2) delete FMesh2;
  FInterEdges.clear();
}

//******************************************************************************

void CCorefine3dFF::splitMeshes(bitset<NB_MARKS> ACopyMarks)
{
  CTime start, end;

  FInterEdgeMark = FMap->getNewMark();
  FFaceMark = FMap->getNewMark();
  FDoubleFaceMark = FMap->getNewMark();
  FCopyMarks = ACopyMarks;
  FCopyMarks[FFaceMark] = true;

  list<SFaceListIterators> inter_list;
  SFaceListIterators iters, next_iters;
  TCorefFaceList::iterator it1, it2;
  list<CDart*> face_list1, face_list2;
  bool found;

  inter_list.push_back(SFaceListIterators(FMesh1->begin(), FMesh1->end(),
					  FMesh2->begin(), FMesh2->end()));

  if (FDisplayMessages) {
    cout << "Création des intersections..." << endl;
    if (FDisplayMessages > 2)
      cout << "+ intersection franche" << endl
	   << "# faces coplanaires" << endl;
  }
  start.updateTime();
  while (!inter_list.empty()) {
    iters = inter_list.front();
    inter_list.pop_front();

//     found = false;
//     for (it1 = iters.first1; it1 != iters.last1 && !found; ++it1) {
//       for (it2 = iters.first2; it2 != iters.last2 && !found; ++it2) {
// 	if (it1->box * it2->box && intersectFaces(it1->face, it2->face,
// 						  it1->plane, it2->plane,
// 						  &face_list1, &face_list2)) {
// 	  next_iters.first1 = it1;
// 	  next_iters.last1 = it1; ++next_iters.last1;
// 	  next_iters.first2 = it2; ++next_iters.first2;
// 	  next_iters.last2 = iters.last2;
// 	  if (next_iters.first2 != next_iters.last2)
// 	    inter_list.push_front(next_iters);

// 	  next_iters.first1 = next_iters.last1;
// 	  next_iters.last1 = iters.last1;
// 	  next_iters.first2 = iters.first2;
// 	  next_iters.last2 = iters.last2;
// 	  if (next_iters.first1 != next_iters.last1)
// 	    inter_list.push_back(next_iters);

// 	  if (face_list1.size() > 1) updateFaceInList(FMesh1, it1, &face_list1);
// 	  if (face_list2.size() > 1) updateFaceInList(FMesh2, it2, &face_list2);
// 	  found = true;
// 	}
//       }
//     }

    for (it1 = iters.first1; it1 != iters.last1;) {
      found = false;
      for (it2 = iters.first2; it2 != iters.last2 && !found; ++it2) {
	if (it1->box * it2->box && intersectFaces(it1->face, it2->face,
						  it1->plane, it2->plane,
						  &face_list1, &face_list2)) {
	  next_iters.first1 = it1;
	  next_iters.last1 = it1; ++next_iters.last1;
	  next_iters.first2 = it2; ++next_iters.first2;
	  next_iters.last2 = iters.last2;
	  if (face_list1.size() > 1) updateFaceInList(FMesh1, it1, &face_list1);
	  if (face_list2.size() > 1) updateFaceInList(FMesh2, it2, &face_list2);
	  if (next_iters.first2 != next_iters.last2)
	    inter_list.push_back(next_iters);
	  found = true;
	  it1 = next_iters.last1;
	}
      }
      if (!found) ++it1;
    }
  }
  end.updateTime();
  FSplitTime = end - start;
  if (FDisplayMessages) {
    if (FDisplayMessages > 1) cout << endl;
    cout << "Durée de la création des intersections : "
	 << FSplitTime << endl;
  }

  FCopyMarks[FFaceMark] = false;
  freeMark(FFaceMark);

  if (FDisplayMessages)
    cout << "Il y a eu " << FInterEdges.size() / 2
	 << (FInterEdges.size() / 2 > 1 ? " arêtes" : " arête")
	 << " d'intersection" << endl;

  list<CDart*>::iterator it;
  for (it = FInterEdges.begin(); it != FInterEdges.end(); ++it)
    FMap->unmarkOrbit(*it, ORBIT_EDGE, FInterEdgeMark);
  freeMark(FInterEdgeMark);

  for (it = FDoubleFaces.begin(); it != FDoubleFaces.end(); ++it)
    FMap->unmarkOrbit(*it, ORBIT_FACE, FDoubleFaceMark);
  freeMark(FDoubleFaceMark);
}

//******************************************************************************

bool CCorefine3dFF::checkEdges()
{
  int mark = FMap->getNewMark();
  bool result = true;

  for (CDynamicCoverageAll dca(FMap); dca.cont(); ++dca)
    if (!FMap->isMarked(*dca, mark)) {
      FMap->markOrbit(*dca, ORBIT_EDGE, mark);
      if (FTools->arePointsEqual(*VTX(*dca), *VTX(a0(*dca)))) {
	cout << "Arête de longueur nulle trouvée : [" << *VTX(*dca)
	     << ", " << *VTX(a0(*dca)) << "]" << endl;
	result = false;
      }
    }
  FMap->negateMaskMark(mark);
  freeMark(mark);

  return result;
}

//******************************************************************************

void CCorefine3dFF::pointDirectInfoToData(int ADirectInfo, CDart * ADart,
					  TOrbit AOrbit, void * AData)
{
  CCoverage *cov = FMap->getDynamicCoverage(ADart, AOrbit);
  for (; cov->cont(); ++(*cov))
    FMap->setDirectInfo(**cov, ADirectInfo, AData);
  delete cov;
}

//******************************************************************************

CBoundingBox CCorefine3dFF::initMesh(CDart * AMesh)
{
  DEBUG_FUNCTION;

  MSG("Fermeture des 3-bords");
  for (CStaticCoverageCC scc(FMap, AMesh); scc.cont(); ++scc)
    if (FMap->isFree3(*scc))
      FMap->stopUp(*scc, 3);

  CBoundingBox box;
  int mark = FMap->getNewMark();
  CDynamicCoverageCC dcc(FMap, AMesh);
  CAttributeVertex *vtx;

  MSG("Mise à jour des champs FVertexDI");
//   if (FLocalVertexDI) {
  for (; dcc.cont(); ++dcc)
    if (!FMap->isMarked(*dcc, mark)) {
      vtx = FMap->findVertex(*dcc);
      assert(vtx != NULL);
      box.addPoint(*vtx);
      for (CDynamicCoverageVertex dcv(FMap, *dcc); dcv.cont(); ++dcv) {
	FMap->setMark(*dcv, mark);
	FMap->setDirectInfo(*dcv, FVertexDI, vtx);
      }
    }
//   }
//   else {
//     for (; dcc.cont(); ++dcc)
//       if (!FMap->isMarked(*dcc, mark)) {
// 	vtx = FMap->findVertex(*dcc);
// 	box.addPoint(*vtx);
// 	FMap->markOrbit(*dcc, ORBIT_VERTEX, mark);
//       }
//   }
    
  MSG("Initialisation des champs FInterPointDI et FLinkedVertexDI");
  for (dcc.reinit(); dcc.cont(); ++dcc) {
    FMap->unsetMark(*dcc, mark);
    FMap->setDirectInfo(*dcc, FInterPointDI, NULL);
    FMap->setDirectInfo(*dcc, FLinkedVertexDI, NULL);
  }

  freeMark(mark);

  return box;
}

//******************************************************************************

CBoundingBox CCorefine3dFF::initFaces(int AMark)
{
  DEBUG_FUNCTION;
  
  CDynamicCoverageAll dca(FMap);

  MSG("Fermeture des 3-bords");
  for (; dca.cont(); ++dca)
    if (FMap->isMarked(*dca, AMark) && FMap->isFree3(*dca))
      FMap->stopUp(*dca, 3);

  CBoundingBox box;
  int mark = FMap->getNewMark();
  CAttributeVertex *vtx;

  FMap->markCopy(AMark, mark);
  FMap->markIncidentCells(ORBIT_FACE, mark);
  FMap->markIncidentCells(ORBIT_VERTEX, mark);
  FMap->markIncidentCells(ORBIT_FACE, mark);

  MSG("Mise à jour des champs FVertexDI, FInterPointDI et FLinkedVertexDI");
  for (dca.reinit(); dca.cont(); ++dca)
    if (FMap->isMarked(*dca, mark)) {
      vtx = FMap->findVertex(*dca);
      assert(vtx != NULL);
      box.addPoint(*vtx);
      for (CDynamicCoverageVertex dcv(FMap, *dca); dcv.cont(); ++dcv) {
	FMap->unsetMark(*dcv, mark);
	FMap->setDirectInfo(*dcv, FVertexDI, vtx);
	FMap->setDirectInfo(*dcv, FInterPointDI, NULL);
	FMap->setDirectInfo(*dcv, FLinkedVertexDI, NULL);
      }
    }
    
  freeMark(mark);

  return box;
}

//******************************************************************************

void CCorefine3dFF::cleanMesh(CDart * AMesh)
{
  DEBUG_FUNCTION;

  for (CDynamicCoverageCC dcc(FMap, AMesh); dcc.cont(); ++dcc) {
    FMap->unsetMark(*dcc, FFictiveVertexMark);
    FMap->unsetMark(*dcc, FFictiveEdgeMark);
  }
}

//******************************************************************************

// void CCorefine3dFF::cleanMesh(CDart * AMesh)
// {
//   DEBUG_FUNCTION;

//   int delete_mark = FMap->getNewMark();
//   int nb;
//   CCoverage *cov = FMap->getStaticCoverage(AMesh, ORBIT_CC);

//   // Suppression des arêtes fictives
//   nb = 0;
//   for (; cov->cont(); ++(*cov))
//     if (FMap->isMarked(**cov, FFictiveEdgeMark)) {
//       if (!FMap->isSameOrbit(**cov, a2(**cov), ORBIT_01)) {
// 	++nb;
// 	removeFictiveEdge(**cov, delete_mark);
//       }
//       FMap->unmarkOrbit(**cov, ORBIT_EDGE, FFictiveEdgeMark);
//     }
//   if (nb && FDisplayMessages)
//     cout << "Suppression de " << nb << " arête(s) fictive(s)" << endl;

//   // Suppression des sommets fictifs
//   nb = 0;
//   for (cov->reinit(); cov->cont(); ++(*cov))
//     if (FMap->isMarked(**cov, FFictiveVertexMark)) {
//       if (!isFictiveVertexUsed(**cov)) {
// 	++nb;
// 	removeFictiveVertex(**cov, delete_mark);
//       }
//       FMap->unmarkOrbit(**cov, ORBIT_VERTEX, FFictiveVertexMark);
//     }
//   if (nb && FDisplayMessages)
//     cout << "Suppression de " << nb << " sommet(s) fictif(s)" << endl;
  
//   delete cov;
//   FMap->deleteMarkedDarts(delete_mark);
//   freeMark(delete_mark);
// }

//******************************************************************************

SCorefFace CCorefine3dFF::computeFaceInfos(CDart * AFace, int AMark)
{
  SCorefFace infos;
  CVertex *vtx, pt, center, normal(0.0, 0.0, 0.0);
  int nb = 2;
  CDart *d;

  infos.face = AFace;

//   d = AFace;     FMap->unsetMark(d, AMark);
//   d = a3(AFace); FMap->unsetMark(a0(d), AMark);
//   d = a1(d);     FMap->unsetMark(d, AMark);
//   d = a3(a0(d)); FMap->unsetMark(d, AMark);

  d = AFace; FMap->unsetMark(d, AMark);
  d = a0(d); FMap->unsetMark(d, AMark);
  d = a3(d); FMap->unsetMark(d, AMark);
  d = a0(d); FMap->unsetMark(d, AMark);
  d = a1(d); FMap->unsetMark(d, AMark);
  d = a3(d); FMap->unsetMark(d, AMark);
  d = a0(d); FMap->unsetMark(d, AMark);
  d = a3(d); FMap->unsetMark(d, AMark);

  pt = *VTX(AFace);
  infos.box.addPoint(pt);
  infos.box.addPoint(*VTX(d));
  center = pt;
  center += *VTX(d);

  for (CDynamicCoverage01 cov(FMap, AFace); cov.cont(); ++cov)
    if (FMap->isMarked(*cov, AMark)) {
      vtx = VTX(*cov);
      d = *cov;  FMap->unsetMark(d, AMark);
      d = a3(d); FMap->unsetMark(d, AMark);
      d = a0(d); FMap->unsetMark(d, AMark);
      d = a3(d); FMap->unsetMark(d, AMark);
      normal += (*vtx - pt) * (*VTX(d) - pt);
      center += *vtx;
      infos.box.addPoint(*vtx);
      ++nb;
    }
  
  infos.plane.setPlane(normal, center / nb);

  return infos;
}

//******************************************************************************

TCorefFaceList * CCorefine3dFF::buildFaceList(CDart * AMesh)
{
  DEBUG_FUNCTION;

  TCorefFaceList *faces = new TCorefFaceList;

  int mark = FMap->getNewMark();
  FMap->halfMarkOrbit(AMesh, ORBIT_CC, mark);
  for (CDynamicCoverageCC dcc(FMap, AMesh); dcc.cont(); ++dcc)
    if (FMap->isMarked(*dcc, mark))
      faces->push_back(computeFaceInfos(*dcc, mark));
  freeMark(mark);

  return faces;
}

//******************************************************************************

TCorefFaceList * CCorefine3dFF::buildFaceList(int AMark)
{
  DEBUG_FUNCTION;

  TCorefFaceList *faces = new TCorefFaceList;

  int mark = FMap->getNewMark();
  FMap->markCopy(AMark, mark);
  FMap->halfMarkIncidentCells(ORBIT_FACE, mark);
  for (CDynamicCoverageAll dca(FMap); dca.cont(); ++dca)
    if (FMap->isMarked(*dca, mark))
      faces->push_back(computeFaceInfos(*dca, mark));
  freeMark(mark);

  return faces;
}

//******************************************************************************

void CCorefine3dFF::reduceFaceLists(TCorefFaceList * AList1,
				    TCorefFaceList * AList2,
				    const CBoundingBox & ABox)
{
  CVertex size;
  TCoordinate max;
  long size_i, size_j, size_k;
  long nb_faces = (AList1->size() > AList2->size() ?
		   AList1->size() : AList2->size());

  if (nb_faces < 50) {
    if (FDisplayMessages)
      cout << "Nombre de faces trop faible => aucune réduction !" << endl;
    return;
  }

  // Calcul de la résolution de la grille
  size = ABox.getEpsMaxBound() - ABox.getEpsMinBound();
  max = (size.getX() > size.getY() ?
	 (size.getX() > size.getZ() ? size.getX() : size.getZ()) :
	 (size.getY() > size.getZ() ? size.getY() : size.getZ()));
  size /= max;

//   double grid_res = pow((double)nb_faces /
// 			(double)(ANbFacesPerCell *
// 				 size.getX() * size.getY() * size.getZ()),
// 			1.0/3.0);

  size_i = (int)(size.getX() * FGridResolution + 0.5);
  size_j = (int)(size.getY() * FGridResolution + 0.5);
  size_k = (int)(size.getZ() * FGridResolution + 0.5);
  if (size_i <= 0) size_i = 1;
  if (size_j <= 0) size_j = 1;
  if (size_k <= 0) size_k = 1;

  if (FDisplayMessages)
    cout << "Résolution de la grille = "
	 << size_i << "x" << size_j << "x" << size_k << endl;
  
  TCorefFaceGrid grid(size_i, size_j, size_k, ABox);
  TCorefFaceList::iterator it, tmp_it;
  list<CDart*>::iterator li;
  int mark = FMap->getNewMark();
  int nb;

  for (it = AList1->begin(); it != AList1->end(); ++it)
    for (TCorefFaceGridIter gi(grid, it->box); gi.cont(); ++gi)
      gi->first.push_back(it->face);
  for (it = AList2->begin(); it != AList2->end(); ++it)
    for (TCorefFaceGridIter gi(grid, it->box); gi.cont(); ++gi)
      gi->second.push_back(it->face);

  for (TCorefFaceGridIter gi(grid); gi.cont(); ++gi) {
    if (gi->first.size() > 0 && gi->second.size() > 0) {
      for (li = gi->first.begin(); li != gi->first.end(); ++li)
	FMap->setMark(*li, mark);
      for (li = gi->second.begin(); li != gi->second.end(); ++li)
	FMap->setMark(*li, mark);
    }
  }

  nb = 0;
  for (it = AList1->begin(); it != AList1->end(); ) {
    tmp_it = it++;
    if (FMap->isMarked(tmp_it->face, mark))
      FMap->unsetMark(tmp_it->face, mark);
    else {
      ++nb;
      AList1->erase(tmp_it);
    }
  }
  if (FDisplayMessages)
    cout << nb << (nb == 1 ?
		   " face a été supprimée" :
		   " faces ont été supprimées")
	 << " de la première liste" << endl;

  nb = 0;
  for (it = AList2->begin(); it != AList2->end(); ) {
    tmp_it = it++;
    if (FMap->isMarked(tmp_it->face, mark))
      FMap->unsetMark(tmp_it->face, mark);
    else {
      ++nb;
      AList2->erase(tmp_it);
    }
  }
  if (FDisplayMessages)
    cout << nb << (nb == 1 ?
		   " face a été supprimée" :
		   " faces ont été supprimées")
	 << " de la seconde liste" << endl;

  freeMark(mark);
}

//******************************************************************************

void CCorefine3dFF::updateFaceInList(TCorefFaceList * AList,
				     TCorefFaceList::iterator AElt,
				     list<CDart*> * AFaces)
{
  DEBUG_FUNCTION;

  TCorefFaceList::iterator pos = AElt;
  list<CDart*>::iterator it = AFaces->begin();

  MSG("La face à été éclatée en " << AFaces->size() << " faces");

  *AElt = SCorefFace(*it, AElt->plane,
		     FTools->orbitBoundingBox(*it, ORBIT_01, FVertexDI));
  ++pos; ++it;

  for (; it != AFaces->end(); ++it)
    AList->insert(pos, SCorefFace(*it, AElt->plane,
				  FTools->orbitBoundingBox(*it, ORBIT_01,
							   FVertexDI)));
}

//******************************************************************************

bool CCorefine3dFF::isVertexLinkedWithFace(CDart * AVertex, int AFaceMark)
{
  DEBUG_FUNCTION;
  if (LINK(AVertex) == NULL)
    return false;

  bool result = false;
  for (CDynamicCoverageVertex dcv(FMap, LINK(AVertex));
       dcv.cont() && !result; ++dcv)
    if (FMap->isMarked(*dcv, AFaceMark))
      result = true;

  return result;
}

//******************************************************************************

void CCorefine3dFF::markFaceVertex(CDart * AVertex, int AFaceMark, int AMark)
{
  for (CDynamicCoverageVertex dcv(FMap, AVertex); dcv.cont(); ++dcv)
    if (FMap->isMarked(*dcv, AFaceMark))
      FMap->setMark(*dcv, AMark);
}

//******************************************************************************

CDart * CCorefine3dFF::getTwinDartOnFace(CDart * ADart, int AFaceMark)
{
  CDart *d = NULL;
  for (CDynamicCoverage23 cov(FMap, ADart); cov.cont() && !d; ++cov)
    if (*cov != ADart && FMap->isMarked(*cov, AFaceMark)) d = *cov;
  return d;
}

//******************************************************************************

CDart * CCorefine3dFF::findFaceSector(CDart * AFaceVertex,
				      const CPlane & AFacePlane,
				      int AFaceMark, const CVertex & AVector)
{
  DEBUG_FUNCTION;

  int orient_mark = FMap->getNewMark();
  CDart *d = NULL;
  CVertex pt = *VTX(AFaceVertex);

  MSG("AVector = " << AVector);
  MSG("sommet testé : " << pt);

  FMap->halfMarkOrbit(AFaceVertex, ORBIT_VERTEX, orient_mark);
  for (CDynamicCoverageVertex dcv(FMap, AFaceVertex); dcv.cont(); ++dcv)
    if (FMap->isMarked(*dcv, orient_mark)) {
      FMap->unsetMark(*dcv, orient_mark);
      if (FMap->isMarked(*dcv, AFaceMark)) {
	if (a1(*dcv) == getTwinDartOnFace(*dcv, AFaceMark) ||
	    FTools->isVectorInSector(AVector,
				     *VTX(a0(*dcv)) - pt,
				     *VTX(a0(a1(*dcv))) - pt,
				     AFacePlane)) {
	  assert(d == NULL);
	  d = *dcv;
	  MSG("secteur trouvé : " << *VTX(a0(*dcv))
	      << ", " << *VTX(a0(a1(*dcv))));
	}
      }
    }
  freeMark(orient_mark);

  return d;
}

//******************************************************************************

CDart * CCorefine3dFF::findNearestFaceVertex(CDart * AFace,
					     const CPlane & ARefPlane,
					     const CPlane & AClipPlane1,
					     const CPlane & AClipPlane2)
{
  CDart *d, *nearest = NULL;
  TCoordinate best_dist = 0, dist;
  CVertex pt;

  d = AFace;
  do {
    pt = *VTX(d);

    if (AClipPlane1.pointDistance(pt) >= -FEps &&
	AClipPlane2.pointDistance(pt) >= -FEps) {
      dist = abs(ARefPlane.pointDistance(pt));
      if (!nearest || dist < best_dist) {
	nearest = d;
	best_dist = dist;
      }
    }

    d = a1(a0(d));
  }
  while (d != AFace);

  return nearest;
}

//******************************************************************************

CVertex CCorefine3dFF::edgeInsideVector(CDart * AEdge,
					const CPlane & AFacePlane)
{
  return AFacePlane.getNormal() * (*VTX(a0(AEdge)) - *VTX(AEdge));
}

//******************************************************************************

CVertex CCorefine3dFF::vertexInsideVector(CDart * AVertex,
					  const CPlane & AFacePlane)
{
  return (edgeInsideVector(AVertex, AFacePlane) +
	  edgeInsideVector(a0(a1(AVertex)), AFacePlane)) / 2.0;
}

//******************************************************************************

void CCorefine3dFF::classifyFaceVertices(CDart * AFace, const CPlane & APlane,
					 int APositiveMark, int ANegativeMark,
					 int AFacesMark, bool AUseVerticesLinks,
					 int * ANbVertices,
					 int * ANbPositiveVertices,
					 int * ANbNegativeVertices)
{
  CDart *d = AFace;
  TCoordinate dist;
  int treated_mark = FMap->getNewMark();
  int nb = 0, nb_pos = 0, nb_neg = 0;
  
  MSG("Classement des sommets de AFace par rapport à APlane");
  do {
    if (!FMap->isMarked(d, treated_mark)) {
      markFaceVertex(d, AFacesMark, treated_mark);
      ++nb;
      if (!AUseVerticesLinks || !isVertexLinkedWithFace(d, AFacesMark)) {
	dist = APlane.pointDistance(*VTX(d));
	if (dist < -FEps) { // Le point est au dessous du plan
	  markFaceVertex(d, AFacesMark, ANegativeMark);
	  ++nb_neg;
	}
	else if (dist > FEps) { // Le point est au dessus du plan
	  markFaceVertex(d, AFacesMark, APositiveMark);
	  ++nb_pos;
	}
      }
    }

    d = a1(a0(d));
  }
  while (d != AFace);

  MSG("Démarquage des brins de la face");
  do {
    FMap->unsetMark(d, treated_mark);
    d = a0(d);
    FMap->unsetMark(d, treated_mark);
    d = a1(d);
  }
  while (d != AFace);

  freeMark(treated_mark);

  MSG("La face comporte " << nb << " sommets dont :");
  MSG("  " << nb_pos << " du côté positif");
  MSG("  " << nb_neg << " du côté negatif");

  if (ANbVertices) *ANbVertices = nb;
  if (ANbPositiveVertices) *ANbPositiveVertices = nb_pos;
  if (ANbNegativeVertices) *ANbNegativeVertices = nb_neg;
}

//******************************************************************************

void CCorefine3dFF::getFaceFictiveElements(CDart * AFace, int AFaceMark,
					   std::list<CDart*> * AFictVertices,
					   std::list<CDart*> * AFictEdges)
{
  int fict_mark = FMap->getNewMark();
  CDart *d = AFace;

  // Récupération des sommets fictifs
  do {
    if (!FMap->isMarked(d, fict_mark) &&
	!FMap->isMarked(d, FFictiveEdgeMark)) {
      markFaceVertex(d, AFaceMark, fict_mark);
      if (FMap->isMarked(d, FFictiveVertexMark)) {
	MSG("Récupération du sommet fictif " << *VTX(d));
	AFictVertices->push_back(d);
      }
    }
    d = a1(a0(d));
  }
  while (d != AFace);

  // Récupération des arêtes fictives
  do {
    if (FMap->isMarked(d, fict_mark)) {
      FMap->unmarkOrbit(d, ORBIT_EDGE, fict_mark);
      if (FMap->isMarked(d, FFictiveEdgeMark))
	AFictEdges->push_back(d);
    }
    d = a1(a0(d));
  }
  while (d != AFace);

  freeMark(fict_mark);
}

//******************************************************************************

void CCorefine3dFF::testPointInside(CDart * AVertex, TInterPt * APoint,
				    int AFaceMark,
				    const CVertex & AInterLine,
				    const CPlane & AFacePlane)
{
  MSG("sector = " << *VTX(a0(a1(AVertex))) << ","
      << *VTX(AVertex) << "," << *VTX(a0(AVertex)));
  if (APoint->getPosition() == FP_Outside &&
//       APoint->getPosition() != FP_OnBorder &&
//       APoint->getPosition() != FP_OnReversedBorder &&
      (a1(AVertex) == getTwinDartOnFace(AVertex, AFaceMark) ||
       FTools->isVectorInSector(AInterLine,
				*VTX(a0(AVertex)) - *VTX(AVertex),
				*VTX(a0(a1(AVertex))) - *VTX(AVertex),
				AFacePlane))) {
    MSG("On entre dans la face");
    APoint->setPosition(FP_Inside);
    APoint->setEnteringSector(AVertex);
  }
  if (FTools->isVectorInSector(-AInterLine,
			       *VTX(a0(AVertex)) - *VTX(AVertex),
			       *VTX(a0(a1(AVertex))) - *VTX(AVertex),
			       AFacePlane)) {
    MSG("On sort de la face");
    APoint->setLeavingSector(AVertex);
  }
}

//******************************************************************************

void CCorefine3dFF::testPointBorder(CDart * AVertex,
				    TInterPt * ANewPoint, TInterPt * AOldPoint,
				    const TInterPtCmp & AComparator)
{
  if (AOldPoint) {
    MSG("Il y a une intersetion avec le bord");
    if (AComparator(AOldPoint, ANewPoint)) {
      MSG("AOldPoint < ANewPoint ==> OnBorder");
      AOldPoint->setPosition(FP_OnBorder);
      AOldPoint->setEnteringSector(a0(a1(AVertex)));
      if (!AOldPoint->getLeavingSector())
	AOldPoint->setLeavingSector(AOldPoint->getEnteringSector());
    }
    else {
      MSG("ANewPoint < AOldPoint ==> OnReversedBorder");
      ANewPoint->setPosition(FP_OnReversedBorder);
      ANewPoint->setEnteringSector(a0(a1(AVertex)));
      if (!ANewPoint->getLeavingSector())
	ANewPoint->setLeavingSector(AVertex);
    }
  }
}

//******************************************************************************
#define GET_IP(d) ((TInterPt*)FMap->getDirectInfo(d, FInterPointDI))

TInterPtSet *
CCorefine3dFF::findIntersectionPoints(CDart * AFace,
				      const CPlane & AFacePlane,
				      const CPlane & AInterPlane,
				      const CVertex & AInterLine,
				      int APositiveMark, int ANegativeMark,
				      int AFacesMark,
				      const TInterPtCmp & AComparator)
{
  DEBUG_FUNCTION;

  TInterPtSet *inter_set;
  TInterPt *inter_point, *old_point;
  CAttributeVertex *vtx1, *vtx2, pt;
  CVertex dir;
  TCoordinate t;
  CDart *d, *d2;

  inter_set = new TInterPtSet(AComparator);
  
  MSG("normale au plan : " << AFacePlane.getNormal());

  // Traitement du dernier sommet pour gérer le cas d'intersection avec le bord
  // entre le dernier et le premier sommet
  inter_point = NULL;
  d = a0(a1(AFace));
  if (!FMap->isMarked(d, APositiveMark) && !FMap->isMarked(d, ANegativeMark)) {
    inter_point = new TInterPt(*VTX(d), 0);
    inter_point->setLeavingSector(d);
    pointDirectInfoToData(FInterPointDI, d, ORBIT_VERTEX, inter_point);
    MSG("première intersection avec le sommet " << inter_point->getPoint());
    inter_set->insert(inter_point);
    testPointInside(d, inter_point, AFacesMark, AInterLine, AFacePlane);
  }

  MSG("Recherche des sommets de AFace se trouvant sur AInterPlane");
  d = AFace;
  do {
    old_point = inter_point;
    inter_point = GET_IP(d);

    // Si le point d'intersection existe déjà, on le met à jour si besoin
    if (inter_point) {
      MSG("intersection déjà existante avec un sommet");
      MSG(inter_point->getPoint());
      testPointInside(d, inter_point, AFacesMark, AInterLine, AFacePlane);
      testPointBorder(d, inter_point, old_point, AComparator);
    }
    else {
      if (!FMap->isMarked(d, APositiveMark) &&
	  !FMap->isMarked(d, ANegativeMark)) {
	inter_point = new TInterPt(*VTX(d), 0);
	inter_point->setLeavingSector(d);
	pointDirectInfoToData(FInterPointDI, d, ORBIT_VERTEX, inter_point);
	MSG("nouvelle intersection avec un sommet");
	MSG(inter_point->getPoint());
	inter_set->insert(inter_point);
	testPointInside(d, inter_point, AFacesMark, AInterLine, AFacePlane);
	testPointBorder(d, inter_point, old_point, AComparator);
      }
    }

    d = a1(a0(d));
  }
  while (d != AFace);

  MSG("Recherche des arêtes de AFace intersectant AInterPlane");
  do {
    // Effacement des pointeurs sur les points d'intersections
    if (GET_IP(d)) pointDirectInfoToData(FInterPointDI, d, ORBIT_VERTEX, NULL);

    if ( (FMap->isMarked(d, APositiveMark) &&
	  FMap->isMarked(a0(d), ANegativeMark)) ||
	 (FMap->isMarked(d, ANegativeMark) &&
	  FMap->isMarked(a0(d), APositiveMark))) {
      vtx1 = VTX(d);
      vtx2 = VTX(a0(d));
      dir = *vtx2 - *vtx1;

      MSG("dir = " << dir);

      if (AInterPlane.getLineIntersection(*vtx1, dir, &t)) {
	assert(t >= 0.0 && t <= 1.0);

	pt = *vtx1 + t * dir;
	inter_point = new TInterPt(pt, 1);
	inter_set->insert(inter_point);
	MSG("intersection avec l'arête [" << *vtx1 << "," << *vtx2 << "] en "
	    << pt);

	if ((AFacePlane.getNormal() * dir).dot(AInterLine) > 0.0) {
	  inter_point->setPosition(FP_Inside);
	  inter_point->setEnteringSector(d);
	}
	else {
	  inter_point->setPosition(FP_Outside);
	  inter_point->setLeavingSector(d);
	}

	// On regarde si l'arête courante est une arête pendante
	if (!FMap->isFree2(d) || !FMap->isFree2(a3(d))) {
	  d2 = NULL;
	  for (CDynamicCoverage23 cov(FMap, d); cov.cont() && !d2; ++cov)
	    if (*cov != d && FMap->isMarked(*cov, AFacesMark)) d2 = *cov;
	  if (d2) {
	    d2 = a0(d2);
	    // Comme c'est une arête pendante, on reste forcément dans la face
	    if (inter_point->getPosition() == FP_Outside) {
	      inter_point->setPosition(FP_Inside);
	      inter_point->setEnteringSector(d2);
	    }
	    else {
	      inter_point->setLeavingSector(d2);
	    }
	  }
	}

	MSG("pos = " << inter_point->getPosition());
      }
    }

    // Démarquage de l'arête
    for (CDynamicCoverageEdge dce(FMap, d); dce.cont(); ++dce) {
      FMap->unsetMark(*dce, APositiveMark);
      FMap->unsetMark(*dce, ANegativeMark);
    }
    
    d = a1(a0(d));
  }
  while (d != AFace);

  return inter_set;
}

//******************************************************************************

CDart * CCorefine3dFF::createEdge()
{
  CDart *d[8];

  for (int i=0; i<8; ++i) {
    d[i] = FMap->addMapDart();
    FMap->setDirectInfo(d[i], FInterPointDI, NULL);
    FMap->setDirectInfo(d[i], FLinkedVertexDI, NULL);
  }

  FMap->linkAlpha0(d[0], d[1]);
  FMap->linkAlpha0(d[2], d[3]);
  FMap->linkAlpha0(d[4], d[5]);
  FMap->linkAlpha0(d[6], d[7]);

  FMap->linkAlpha2(d[0], d[2]);
  FMap->linkAlpha2(d[1], d[3]);
  FMap->linkAlpha2(d[4], d[6]);
  FMap->linkAlpha2(d[5], d[7]);

  FMap->linkAlpha3(d[0], d[4]);
  FMap->linkAlpha3(d[1], d[5]);
  FMap->linkAlpha3(d[2], d[6]);
  FMap->linkAlpha3(d[3], d[7]);

  return d[0];
}

//******************************************************************************

CDart * CCorefine3dFF::insertVertexInFace(CDart * AFace, const CVertex & APoint)
{
  DEBUG_FUNCTION;

  CDart *vertex = a1(a0(insertEdgeInFace(AFace, APoint)));
  FMap->markOrbit(vertex, ORBIT_EDGE, FFictiveEdgeMark);

  return vertex;
}

//******************************************************************************

CDart * CCorefine3dFF::insertEdgeInFace(CDart * AVertex1,
					const CVertex & AVertex2)
{
  DEBUG_FUNCTION;

  CDart *link = LINK(AVertex1);
  CDart *edge = createEdge();

  CDart *old_a1 = a1(AVertex1);
  FMap->unsew1(AVertex1);
  FMap->sew1(AVertex1, a2(edge));
  FMap->sew1(old_a1, edge);
  FMap->sew1(a0(edge), a0(a2(edge)));

  if (FMap->isMarked(AVertex1, FFictiveVertexMark))
    FMap->markOrbit(edge, ORBIT_23, FFictiveVertexMark);

  for (CDynamicCoverage23 dc(FMap, edge); dc.cont(); ++dc) {
    FMap->setDirectInfo(*dc, FLinkedVertexDI, link);
    for (int m = 0; m < NB_MARKS; m++)
      if (FCopyMarks[m] && FMap->isMarked(a1(*dc), m)) {
	FMap->setMark(*dc, m);
	FMap->setMark(a0(*dc), m);
      }
  }
  FMap->pointDirectInfoToAttributeVertex(FVertexDI, AVertex1);

  CAttributeVertex *vtx = new CAttributeVertex(AVertex2);
  FMap->setVertex(a0(edge), vtx);

  for (CDynamicCoverageVertex dcv(FMap, a0(edge)); dcv.cont(); ++dcv) {
    FMap->setDirectInfo(*dcv, FVertexDI, vtx);
    FMap->setDirectInfo(*dcv, FLinkedVertexDI, NULL);
  }

  MSG("arête insérée : [" << *VTX(AVertex1) << ";" << AVertex2 << "]");

  return edge;
}

//******************************************************************************

CDart * CCorefine3dFF::splitEdge(CDart * AEdge, const CVertex & APoint,
				 list<CDart*> * AFictiveEdges)
{
  DEBUG_FUNCTION;

  FMap->CGMapGeneric::insertVertex(AEdge);

  CDart *d = a1(a0(AEdge));

  if (FMap->isMarked(AEdge, FFictiveEdgeMark)) {
    FMap->markOrbit(d, ORBIT_VERTEX, FFictiveEdgeMark);
//     FMap->markOrbit(d, ORBIT_VERTEX, FFictiveVertexMark);

    if (AFictiveEdges) {
      CDart *edge = a2(a3(AEdge));
      bool found = false;
      for (list<CDart*>::iterator it = AFictiveEdges->begin();
	   it != AFictiveEdges->end() && !found; ++it)
	if (*it == AEdge || *it == edge)
	  found = true;
      AFictiveEdges->push_back(found ? d : AEdge);
    }
  }

  CAttributeVertex *vtx = new CAttributeVertex(APoint);
  FMap->setVertex(d, vtx);

  for (CDynamicCoverageVertex dcv(FMap, d); dcv.cont(); ++dcv) {
    FMap->setDirectInfo(*dcv, FInterPointDI, NULL);
    FMap->setDirectInfo(*dcv, FLinkedVertexDI, NULL);
    FMap->setDirectInfo(*dcv, FVertexDI, vtx);
    for (int m = 0; m < NB_MARKS; m++)
      if (FCopyMarks[m] && FMap->isMarked(a0(*dcv), m))
	FMap->setMark(*dcv, m);
  }

  MSG("sommet inséré : " << APoint);

  return d;
}

//******************************************************************************

CDart * CCorefine3dFF::splitFace(CDart * AVertex1, CDart * AVertex2)
{
  DEBUG_FUNCTION;

  assert(AVertex1 != AVertex2);

  CDart *edge = createEdge();
  CDart *link;

  CDart *old_a1[2] = {a1(AVertex1), a1(AVertex2)};
  FMap->unsew1(AVertex1);
  FMap->sew1(AVertex1, a2(edge));
  FMap->sew1(old_a1[0], edge);
  FMap->unsew1(AVertex2);
  FMap->sew1(AVertex2, a0(edge));
  FMap->sew1(old_a1[1], a2(a0(edge)));
    
  if (FMap->isMarked(AVertex1, FFictiveVertexMark))
    FMap->markOrbit(edge, ORBIT_23, FFictiveVertexMark);
  if (FMap->isMarked(AVertex2, FFictiveVertexMark))
    FMap->markOrbit(a0(edge), ORBIT_23, FFictiveVertexMark);

  link = LINK(AVertex1);
  for (CDynamicCoverage23 dc1(FMap, edge); dc1.cont(); ++dc1) {
    FMap->setDirectInfo(*dc1, FLinkedVertexDI, link);
    for (int m = 0; m < NB_MARKS; m++)
      if (FCopyMarks[m] && FMap->isMarked(a1(*dc1), m))
	FMap->setMark(*dc1, m);
  }
  FMap->pointDirectInfoToAttributeVertex(FVertexDI, AVertex1);
  
  link = LINK(AVertex2);
  for (CDynamicCoverage23 dc2(FMap, a0(edge)); dc2.cont(); ++dc2) {
    FMap->setDirectInfo(*dc2, FLinkedVertexDI, link);
    for (int m = 0; m < NB_MARKS; m++)
      if (FCopyMarks[m] && FMap->isMarked(a1(*dc2), m))
	FMap->setMark(*dc2, m);
  }
  FMap->pointDirectInfoToAttributeVertex(FVertexDI, AVertex2);
  
  MSG("arête insérée : [" << *VTX(AVertex1) << ";" << *VTX(AVertex2) << "]");

  return edge;
}

//******************************************************************************

bool CCorefine3dFF::isUselessFictiveVertex(CDart * AVertex)
{
  bool result = true;

  for (CDynamicCoverageVertex dcv(FMap, AVertex); dcv.cont() && result; ++dcv)
    if (FMap->isMarked(*dcv, FFictiveEdgeMark))
      result = false;

  return result;
}

//******************************************************************************

bool CCorefine3dFF::isUselessFictiveEdge(CDart * AEdge)
{
  return (a1(AEdge) == a2(AEdge) || a1(a0(AEdge)) == a2(a0(AEdge)) ||
	  !FMap->isSameOrbit(AEdge, a2(AEdge), ORBIT_01));
}

//******************************************************************************

bool CCorefine3dFF::isWholeFaceMarked(CDart * AFace, int AMark)
{
  bool result = true;
  for (CDynamicCoverageFace dcf(FMap, AFace); dcf.cont() && result; ++dcf)
    if (!FMap->isMarked(*dcf, AMark))
      result = false;
  return result;
}

//******************************************************************************

CDart * CCorefine3dFF::copyDoubleFaceData(CDart * AFace)
{
  DEBUG_FUNCTION;

  int edge_mark = FMap->getNewMark();
  CDart *d1, *d2, *other;
  int m;

  MSG("Marquage des arêtes de la face");
  d1 = AFace;
  do {
    FMap->markOrbit(d1, ORBIT_EDGE, edge_mark);
    d1 = a1(a0(d1));
  }
  while (d1 != AFace);

  MSG("Recherche de la face jumelle");
  if (isWholeFaceMarked(a2(AFace), edge_mark)) {
    other = a3(a2(AFace));
  }
  else {
    other = a2(a3(AFace));
    assert(isWholeFaceMarked(other, edge_mark));
  }
//   other = AFace;
//   do {
//     other = a3(a2(other));
//   }
//   while (!isWholeFaceMarked(other, edge_mark));
//   assert(other != AFace);

  MSG("Copie des marques se trouvant sur la face à supprimer");
  d2 = other;
  do {
    FMap->unmarkOrbit(d1, ORBIT_EDGE, edge_mark);
    for (m = 0; m < NB_MARKS; ++m) {
      if (FCopyMarks[m] && FMap->isMarked(d1, m))
	FMap->setMark(d2, m);
      if (FCopyMarks[m] && FMap->isMarked(a3(d1), m))
	FMap->setMark(a3(d2), m);
    }
    d1 = a0(d1);
    d2 = a0(d2);
    for (m = 0; m < NB_MARKS; ++m) {
      if (FCopyMarks[m] && FMap->isMarked(d1, m))
	FMap->setMark(d2, m);
      if (FCopyMarks[m] && FMap->isMarked(a3(d1), m))
	FMap->setMark(a3(d2), m);
    }
    d1 = a1(d1);
    d2 = a1(d2);
  }
  while (d1 != AFace);
  assert(d2 == other);

  freeMark(edge_mark);

  return other;
}

//******************************************************************************

CDart * CCorefine3dFF::removeFictiveVertex(CDart * AVertex, int ADeleteMark)
{
  DEBUG_FUNCTION;

  assert(FMap->isMarked(AVertex, FFictiveVertexMark));
  assert(LINK(AVertex) == NULL);

  CDart *d = a0(AVertex);
  
  if (ADeleteMark < 0)
    FMap->merge(AVertex, a1(AVertex), 1, true);
  else {
    FMap->markOrbit(AVertex, ORBIT_VERTEX, ADeleteMark);
    FMap->merge(AVertex, a1(AVertex), 1, false);
  }

  return d;
}

//******************************************************************************

CDart * CCorefine3dFF::removeFictiveEdge(CDart * AEdge, int ADeleteMark)
{
  DEBUG_FUNCTION;

  assert(FMap->isMarked(AEdge, FFictiveEdgeMark));
  assert(!FMap->isFree2(AEdge));
  assert(a3(a2(AEdge)) == a2(a3(AEdge)));

  CDart *d1 = a1(AEdge);
  CDart *d2 = a1(a0(AEdge));
  CDart *l1, *l2;

  if (a2(AEdge) == d1) d1 = NULL;
  if (a2(a0(AEdge)) == d2) d2 = NULL;

  l2 = LINK(AEdge);
  if (l2) {
    l1 = LINK(l2);
    if (AEdge == l1 || a3(a2(AEdge)) == l1 ||
	a2(AEdge) == l1 || a3(AEdge) == l1) {
      MSG("Déplacement des liens sur le premier sommet");
      pointDirectInfoToData(FLinkedVertexDI, l2, ORBIT_VERTEX, a1(a3(AEdge)));
    }
  }
  l2 = LINK(a0(AEdge));
  if (l2) {
    l1 = LINK(l2);
    if (a2(a0(AEdge)) == l1 || a3(a0(AEdge)) == l1 ||
	a0(AEdge) == l1 || a3(a2(a0(AEdge))) == l1) {
      MSG("Déplacement des liens sur le second sommet");
      pointDirectInfoToData(FLinkedVertexDI, l2, ORBIT_VERTEX, a1(a0(AEdge)));
    }
  }

  MSG("Démarquage des sommets de l'arête");
  if (FMap->isMarked(AEdge, FFictiveVertexMark))
    FMap->unmarkOrbit(AEdge, ORBIT_23, FFictiveVertexMark);
  if (FMap->isMarked(a0(AEdge), FFictiveVertexMark))
    FMap->unmarkOrbit(a0(AEdge), ORBIT_23, FFictiveVertexMark);

  MSG("Suppression de l'arête");
  if (ADeleteMark < 0)
    FMap->merge(AEdge, a2(AEdge), 2, true);
  else {
    FMap->markOrbit(AEdge, ORBIT_EDGE, ADeleteMark);
    FMap->merge(AEdge, a2(AEdge), 2, false);
  }

  MSG("Mise à jour des champs FVertexDI");
  if (d1) FMap->pointDirectInfoToAttributeVertex(FVertexDI, d1);
  if (d2) FMap->pointDirectInfoToAttributeVertex(FVertexDI, d2);

  return d1;
}

//******************************************************************************

CDart * CCorefine3dFF::removeDoubleFace(CDart * AFace)
{
  DEBUG_FUNCTION;

  CDart *other = copyDoubleFaceData(AFace);

  MSG("Suppression de la face en trop");
  FMap->merge(AFace, a3(AFace), 3, true);

  MSG("Mise à jour des champs FVertexDI");
  CDart *d = other;
  do {
    FMap->pointDirectInfoToAttributeVertex(FVertexDI, d);
    d = a1(a0(d));
  }
  while (d != other);

  return other;
}

//******************************************************************************

void CCorefine3dFF::linkVertices(CDart * AVertex1, CDart * AVertex2)
{
  DEBUG_FUNCTION;
  MSG("AVertex1 = " << *VTX(AVertex1));
  MSG("AVertex2 = " << *VTX(AVertex2));
  assert(FTools->arePointsEqual(*VTX(AVertex1), *VTX(AVertex2)));
  for (CDynamicCoverageVertex dcv1(FMap, AVertex1); dcv1.cont(); ++dcv1)
    FMap->setDirectInfo(*dcv1, FLinkedVertexDI, AVertex2);
  for (CDynamicCoverageVertex dcv2(FMap, AVertex2); dcv2.cont(); ++dcv2)
    FMap->setDirectInfo(*dcv2, FLinkedVertexDI, AVertex1);
}

//******************************************************************************

void CCorefine3dFF::addIntersectionPoint(CDart *AVertices[2], int AFaceNumber,
					 int AFacesMark, int AInterMark,
					 std::list<CDart*> * AInterList)
{
  MSG("Ajout du point d'intersection " << *VTX(AVertices[0]));
  if (FMap->isMarked(AVertices[0], FFictiveVertexMark))
    FMap->unmarkOrbit(AVertices[0], ORBIT_VERTEX, FFictiveVertexMark);
  if (FMap->isMarked(AVertices[1], FFictiveVertexMark))
    FMap->unmarkOrbit(AVertices[1], ORBIT_VERTEX, FFictiveVertexMark);
  linkVertices(AVertices[0], AVertices[1]);
  AInterList->push_back(AVertices[AFaceNumber]);
  markFaceVertex(AVertices[0], AFacesMark, AInterMark);
  markFaceVertex(AVertices[1], AFacesMark, AInterMark);
}

//******************************************************************************

bool CCorefine3dFF::intersectFaces(CDart * AFace1, CDart * AFace2,
				   const CPlane & APlane1,
				   const CPlane & APlane2,
				   list<CDart*> * AFaceList1,
				   list<CDart*> * AFaceList2)
{
  DEBUG_FUNCTION;

  list<CDart*> fict_vertices, fict_edges;
  list<CDart*>::iterator it, last_edge = FInterEdges.begin();
  int positive_mark = FMap->getNewMark();
  int negative_mark = FMap->getNewMark();
  int nb1, nb_pos1, nb_neg1, nb2, nb_pos2, nb_neg2;
  char inter_type = 0;

  FMap->markOrbit(AFace1, ORBIT_01, FFaceMark);
  FMap->markOrbit(AFace2, ORBIT_01, FFaceMark);
  
#ifdef DEBUG_MESSAGES
  static int nb = 0;
  MSG("**** ETAPE N°" << nb << " ****");
#ifdef SAVE_STEPS_SINCE
#ifdef SAVE_STEPS_UNTIL
  if (nb >= SAVE_STEPS_SINCE && nb <= SAVE_STEPS_UNTIL) {
#else
  if (nb >= SAVE_STEPS_SINCE) {
#endif

    stringstream s; 
    s<<"CCorefine3DFF_intersectFaces";
    s.width(4); s.fill(0); s.flags (ios::right);
    s<<nb;
    s.width(0);
    s<<".map";

    FMap->markCopy(FFictiveEdgeMark, 0);
    FMap->markCopy(FInterEdgeMark, 1);
    FMap->markCopy(FFaceMark, 2);
    FMap->save(s.str(), AsciiFormat);
  }
#endif
  ++nb;
#endif

  classifyFaceVertices(AFace1, APlane2, positive_mark, negative_mark,
		       FFaceMark, true, &nb1, &nb_pos1, &nb_neg1);
  classifyFaceVertices(AFace2, APlane1, positive_mark, negative_mark,
		       FFaceMark, true, &nb2, &nb_pos2, &nb_neg2);

  MSG("Récupération des sommets fictifs et des arêtes fictives sur les faces");
  getFaceFictiveElements(AFace1, FFaceMark, &fict_vertices, &fict_edges);
  getFaceFictiveElements(AFace2, FFaceMark, &fict_vertices, &fict_edges);

  if ((nb_pos1 == 0 && nb_neg1 == 0) || (nb_pos2 == 0 && nb_neg2 == 0)) {
    inter_type = '#';
    if (nb_pos1 > 0) FMap->unmarkOrbit(AFace1, ORBIT_01, positive_mark);
    if (nb_neg1 > 0) FMap->unmarkOrbit(AFace1, ORBIT_01, negative_mark);
    if (nb_pos2 > 0) FMap->unmarkOrbit(AFace2, ORBIT_01, positive_mark);
    if (nb_neg2 > 0) FMap->unmarkOrbit(AFace2, ORBIT_01, negative_mark);
    intersectCoplanarFaces(AFace1, AFace2, APlane1, APlane2,
			   positive_mark, negative_mark, FFaceMark,
			   &fict_vertices, &fict_edges);
  }
  else if (nb_pos1 < nb1 && nb_neg1 < nb1 && nb_pos2 < nb2 && nb_neg2 < nb2) {
    inter_type = '+';
    intersectSecantFaces(AFace1, AFace2, APlane1, APlane2,
			 positive_mark, negative_mark, FFaceMark,
			 &fict_vertices, &fict_edges);
  }
  else {
    for (CDynamicCoverage01 cov1(FMap, AFace1); cov1.cont(); ++cov1) {
      FMap->unsetMark(*cov1, positive_mark);
      FMap->unsetMark(*cov1, negative_mark);
    }
    for (CDynamicCoverage01 cov2(FMap, AFace2); cov2.cont(); ++cov2) {
      FMap->unsetMark(*cov2, positive_mark);
      FMap->unsetMark(*cov2, negative_mark);
    }
  }

  MSG("Suppression des arêtes fictives inutiles");
  for (it = fict_edges.begin(); it != fict_edges.end(); ++it)
    if (FMap->isMarked(*it, FFictiveEdgeMark) &&
	isUselessFictiveEdge(*it))
      removeFictiveEdge(*it);

  MSG("Suppression des sommets fictifs inutiles");
  for (it = fict_vertices.begin(); it != fict_vertices.end(); ++it)
    if (FMap->isMarked(*it, FFictiveVertexMark) &&
	isUselessFictiveVertex(*it))
      removeFictiveVertex(*it);

  MSG("Récupération des faces résultantes");
  assert(AFaceList1 != NULL);
  assert(AFaceList2 != NULL);
  AFaceList1->clear();
  AFaceList2->clear();

  bool result = last_edge != FInterEdges.begin();

  if (result) {
    if (FDisplayMessages > 1 && inter_type) {
      cout << inter_type;
      cout.flush();
    }

    list<CDart*> *face_list[2] = {AFaceList1, AFaceList2};
    CDart *edge;
    int i;

    for (it = FInterEdges.begin(); it != last_edge; ) {
      for (i = 0; i < 2; ++i) {
	edge = *it++;
	if (FMap->isMarked(edge, FFaceMark)) {
	  FMap->unmarkOrbit(edge, ORBIT_01, FFaceMark);
	  face_list[i]->push_back(edge);
	}
	if (FMap->isMarked(a2(edge), FFaceMark)) {
	  FMap->unmarkOrbit(a2(edge), ORBIT_01, FFaceMark);
	  face_list[i]->push_back(a0(a2(edge)));
	}
      }
    }
  }
  else {
    FMap->unmarkOrbit(AFace1, ORBIT_01, FFaceMark);
    FMap->unmarkOrbit(AFace2, ORBIT_01, FFaceMark);
  }

  freeMark(positive_mark);
  freeMark(negative_mark);

  return result;
}

//******************************************************************************

void CCorefine3dFF::intersectSecantFaces(CDart * AFace1, CDart * AFace2,
					 const CPlane & APlane1,
					 const CPlane & APlane2,
					 int APositiveMark, int ANegativeMark,
					 int AFacesMark,
					 list<CDart*> * /*AFictiveVertices*/,
					 list<CDart*> * AFictiveEdges)
{
  DEBUG_FUNCTION;

//   assert(!FMap->isSameOrbit(AFace1, AFace2, ORBIT_CC));

  CPlane plane[2] = {APlane1, APlane2};
  CVertex line;

  line = APlane1.getNormal() * APlane2.getNormal();

  MSG("line = " << line);

  assert(line.getX() != 0.0 || line.getY() != 0.0 || line.getZ() != 0.0);
//   if (FTools->isVectorNull(line))
//     cerr << "<CCorefine3dFF::intersectSecantFaces> "
// 	 << "Vecteur de coupe de longueur quasi-nulle : " << line << endl;

  // Calcul des points d'intersection sur chaque face
  TInterPtCmp comp(FTools, line);
  TInterPtSet *inter_set[2];

  MSG("Recherche des intersections sur la première face");
  inter_set[0] = findIntersectionPoints(AFace1, APlane1, APlane2, line,
					APositiveMark, ANegativeMark,
					AFacesMark, comp);

  MSG("Recherche des intersections sur la deuxième face");
  inter_set[1] = findIntersectionPoints(AFace2, APlane2, APlane1, line,
					APositiveMark, ANegativeMark,
					AFacesMark, comp);

  TInterPtSet::iterator it[2] = {inter_set[0]->begin(), inter_set[1]->begin()};
  TInterPt *ip[2];
  TPositionInFace pos[2], prev_pos[2];
  int prev_point, next_point;
  CDart *vtx[2], *edge[2], *last_vertex[2];
  int mark = FMap->getNewMark();
  int i;

  ip[0] = ip[1] = NULL;
  prev_pos[0] = prev_pos[1] = pos[0] = pos[1] = FP_Outside;
  prev_point = next_point = 0;
  last_vertex[0] = last_vertex[1] = NULL;

  // Parcours des ensembles et construction des segments d'intersection
  while (it[0] != inter_set[0]->end() && it[1] != inter_set[1]->end()) {
    // Les points se trouvent au même endroit sur les deux faces
    if (FTools->arePointsEqual((*it[0])->getPoint(), (*it[1])->getPoint()))
      next_point = 3;
    // Le point le plus proche se trouve sur la face 1
    else if (comp(*it[0], *it[1]))
      next_point = 1;
    // Le point le plus proche se trouve sur la face 2
    else
      next_point = 2;

    MSG("prev_point = " << prev_point);
    MSG("next_point = " << next_point);
    MSG("pos1 = " << pos[0]);
    MSG("pos2 = " << pos[1]);
    
    if (pos[0] != FP_Outside && pos[1] != FP_Outside) {
      for (i = 0; i < 2; ++i) {
	BEGIN("Création d'une arête d'intersection sur la face " << i);
	MSG("prev dim  = " << ip[i]->getCellDim());
	MSG("prev point = " << ip[i]->getPoint());
	MSG("next dim = " << (*it[i])->getCellDim());
	MSG("next point = " << (*it[i])->getPoint());

	// On teste si la topologie est en cohérence avec la géométrie
	if (pos[i] == FP_OnBorder || pos[i] == FP_OnReversedBorder) {
	  MSG("test d'erreur numérique");
	  CDart *d;
	  // vtx[0] = pointeur sur le premier sommet du segment courant
	  // vtx[1] = pointeur sur le sommet d'intersection suivant
	  // d = opposé (par a1 o a0) de vtx[0] sur le segment courant
	  if (pos[i] == FP_OnBorder) {
	    if (!(prev_point & (i+1)) && prev_pos[(i+1)%2] != FP_Outside) {
	      assert(last_vertex[i] != NULL);
	      vtx[0] = last_vertex[i];
	    }
	    else
	      vtx[0] = ip[i]->getEnteringSector();
	    d = a1(a0(vtx[0]));
	  }
	  else {
	    d = ip[i]->getEnteringSector();
	    vtx[0] = a1(a0(d));
	  }
	  vtx[1] = ((*it[i])->getPosition() == FP_Inside ?
		    (*it[i])->getEnteringSector() :
		    (*it[i])->getLeavingSector());
	  markFaceVertex(vtx[1], AFacesMark, mark);
	  MSG("vtx[0] = " << *VTX(vtx[0]));
	  MSG("vtx[1] = " << *VTX(vtx[1]));
	  // si d n'est pas marqué, c'est qu'il est différent de vtx[1] !!!
	  if (!FMap->isMarked(d, mark)) {
	    cerr << "Erreur numérique détectée : " << pos[i] << " ==> ";
	    CDart *d_on = NULL;
	    // Recherche d'un éventuel segment commun entre vtx[0] et vtx[1]
	    markFaceVertex(vtx[0], AFacesMark, mark);
	    d = vtx[0];
	    do {
	      if (FMap->isMarked(d, mark) && FMap->isMarked(a0(d), mark))
		d_on = d;
	      FMap->unsetMark(d, mark);
	      d = a0(d);
	      FMap->unsetMark(d, mark);
	      d = a1(d);
	    }
	    while (d != vtx[0]);
	    if (d_on) {
	      ip[i]->setEnteringSector(d_on);
	      if ((*VTX(a0(d_on)) - *VTX(d_on)).dot(line) > 0.0)
		ip[i]->setPosition(FP_OnBorder);
	      else
		ip[i]->setPosition(FP_OnReversedBorder);
	    }
	    else {
	      // S'il n'y a pas de segment commun, on recherche le bon secteur
	      CDart *d_in = findFaceSector(vtx[0], plane[i], AFacesMark, line);
	      ip[i]->setEnteringSector(d_in);
	      ip[i]->setPosition(d_in ? FP_Inside : FP_Outside);
	    }
	    pos[i] = ip[i]->getPosition();
	    cerr << pos[i] << endl;
	  }
	  else {
	    FMap->unmarkOrbit(vtx[0], ORBIT_VERTEX, mark);
	    FMap->unmarkOrbit(vtx[1], ORBIT_VERTEX, mark);
	  }
	}

	// On se trouve sur une arête orientée dans le même sens que la ligne
	if (pos[i] == FP_OnBorder) {
	  vtx[0] = ip[i]->getEnteringSector();
	  if (!(prev_point & (i+1))) {
	    if (prev_pos[(i+1)%2] != FP_Outside) {
	      assert(last_vertex[i] != NULL);
	      vtx[0] = last_vertex[i];
	    }
	    else {
	      vtx[0] = splitEdge(vtx[0], ip[(i+1)%2]->getPoint(),
				 AFictiveEdges);
	      ip[i]->setEnteringSector(vtx[0]);
	    }
	  }
	  if (!(next_point & (i+1)))
	    splitEdge(vtx[0], (*it[(i+1)%2])->getPoint(), AFictiveEdges);
	  edge[i] = vtx[0];
	  last_vertex[i] = vtx[1] = a1(a0(edge[i]));
	}
	// On se trouve sur une arête orientée dans le sens contraire
	else if (pos[i] == FP_OnReversedBorder) {
	  last_vertex[i] = ip[i]->getEnteringSector();
	  if (!(prev_point & (i+1))) {
	    if (prev_pos[(i+1)%2] == FP_Outside)
	      splitEdge(last_vertex[i], ip[(i+1)%2]->getPoint(), AFictiveEdges);
	  }
	  if (!(next_point & (i+1)))
	    last_vertex[i] = splitEdge(last_vertex[i],
				       (*it[(i+1)%2])->getPoint(),
				       AFictiveEdges);
	  vtx[0] = edge[i] = last_vertex[i];
	  vtx[1] = a1(a0(edge[i]));
	}
	// On se trouve dans la face
	else {
	  // Récupération du premier sommet
	  if (prev_point & (i+1)) {
	    MSG("Le premier sommet se trouve sur la face courante");
	    if (ip[i]->getCellDim() == 0)
	      vtx[0] = ip[i]->getEnteringSector();
	    else
	      vtx[0] = splitEdge(ip[i]->getEnteringSector(),
				 ip[i]->getPoint(), AFictiveEdges);
	    MSG("sommet trouvé : " << *VTX(vtx[0]));
	  }
	  else {
	    MSG("Le premier sommet se trouve sur l'autre face");
	    if (prev_pos[(i+1)%2] != FP_Outside) {
	      assert(last_vertex[i] != NULL);
	      vtx[0] = last_vertex[i];
	    }
	    else if (!(next_point & (i+1))) {
	      if (!last_vertex[i]) {
		if (ip[i]->getCellDim() == 1) {
// 		  vtx[0] = splitEdge(ip[i]->getEnteringSector(),
// 				     ip[i]->getPoint(), AFictiveEdges);
// 		  FMap->markOrbit(vtx[0], ORBIT_VERTEX, FFictiveVertexMark);
// 		  AFictiveVertices->push_back(vtx[0]);
		  vtx[0] = ip[i]->getEnteringSector();
		  CPlane clip_plane1(edgeInsideVector(vtx[0], plane[i]),
				     ip[i]->getPoint());
		  CPlane clip_plane2(-line, ip[(i+1)%2]->getPoint());
		  vtx[0] = findNearestFaceVertex(vtx[0], plane[(i+1)%2],
						 clip_plane1, clip_plane2);
		  vtx[0] = findFaceSector(vtx[0], plane[i], AFacesMark,
					  ip[(i+1)%2]->getPoint() -
					  *VTX(vtx[0]));
		  assert(vtx[0]);
		}
		else
		  vtx[0] = ip[i]->getEnteringSector();
	      }
	      else
		vtx[0] = last_vertex[i];
	      vtx[0] = insertVertexInFace(vtx[0], ip[(i+1)%2]->getPoint());
	      AFictiveEdges->push_back(vtx[0]);
	    }
	    else
	      vtx[0] = NULL;
	  }

	  // Récupération du second sommet et création de l'arête
	  if (next_point & (i+1)) {
	    MSG("Le second sommet se trouve sur la face courante");
	    MSG("next_point = " << (*it[i])->getPoint());
	    if ((*it[i])->getCellDim() == 0)
	      vtx[1] = ((*it[i])->getLeavingSector() ?
			(*it[i])->getLeavingSector() :
			(*it[i])->getEnteringSector());
	    else
	      vtx[1] = splitEdge((*it[i])->getLeavingSector(),
				 (*it[i])->getPoint(), AFictiveEdges);

	    if (vtx[0])
	      edge[i] = splitFace(vtx[0], vtx[1]);
	    else {
	      edge[i] = a2(a0(insertEdgeInFace(vtx[1],
					       ip[(i+1)%2]->getPoint())));
	      vtx[0] = edge[i];
	    }

	    if ((*it[i])->getPosition() == FP_Inside) {
	      if ((*it[i])->getCellDim() == 1) {
		(*it[i])->setCellDim(0);
		(*it[i])->setEnteringSector(a1(a0((*it[i])->getEnteringSector())));
	      }
	      else if (((*VTX(a0((*it[i])->getEnteringSector())) -
			 *VTX((*it[i])->getEnteringSector()))
			* line).dot(plane[i].getNormal()) < 0.0)
		(*it[i])->setEnteringSector(a2(a0(edge[i])));
	    }
	    last_vertex[i] = NULL;
	  }
	  else {
	    MSG("Le second sommet se trouve sur l'autre face");
	    edge[i] = insertEdgeInFace(vtx[0], (*it[(i+1)%2])->getPoint());
	    vtx[1] = last_vertex[i] = a1(a0(edge[i]));
	  }
	}
	if (FMap->isMarked(vtx[0], FFictiveVertexMark))
	  FMap->unmarkOrbit(vtx[0], ORBIT_VERTEX, FFictiveVertexMark);
	if (FMap->isMarked(vtx[1], FFictiveVertexMark))
	  FMap->unmarkOrbit(vtx[1], ORBIT_VERTEX, FFictiveVertexMark);
	MSG("arête d'intersection trouvée : [" << *VTX(edge[i]) << ";"
	    << *VTX(a0(edge[i])) << "]");

	END("");
      }

      if (!FMap->isMarked(edge[0], FInterEdgeMark) &&
	  !FMap->isMarked(edge[1], FInterEdgeMark)) {
	MSG("Marquage et stockage des arêtes d'intersection");
	FInterEdges.push_front(edge[1]);
	FInterEdges.push_front(edge[0]);

// 	if (!FMap->isMarked(edge[0], FInterEdgeMark))
	  FMap->markOrbit(edge[0], ORBIT_EDGE, FInterEdgeMark);
// 	else
// 	  cerr << "Topologie de l'objet 2 incorrecte !" << endl;
// 	if (!FMap->isMarked(edge[1], FInterEdgeMark))
	  FMap->markOrbit(edge[1], ORBIT_EDGE, FInterEdgeMark);
// 	else
// 	  cerr << "Topologie de l'objet 1 incorrecte !" << endl;

	if (FMap->isMarked(edge[0], FFictiveEdgeMark))
	  FMap->unmarkOrbit(edge[0], ORBIT_EDGE, FFictiveEdgeMark);
	if (FMap->isMarked(edge[1], FFictiveEdgeMark))
	  FMap->unmarkOrbit(edge[1], ORBIT_EDGE, FFictiveEdgeMark);

	if (pos[0] == FP_OnReversedBorder) edge[0] = a0(edge[0]);
	if (pos[1] == FP_OnReversedBorder) edge[1] = a0(edge[1]);
	linkVertices(edge[0], edge[1]);
	linkVertices(a1(a0(edge[0])), a1(a0(edge[1])));

// 	if (!FTools->arePointsEqual(*VTX(edge[0]), *VTX(edge[1]))) {
// 	  cerr << "intersectSecantFaces: "
// 	       <<  *VTX(edge[0]) << " != " << *VTX(edge[1]) << endl;
// 	  exit(0);
// 	}
// 	if (!FTools->arePointsEqual(*VTX(a0(edge[0])), *VTX(a0(edge[1])))) {
// 	  cerr << "intersectSecantFaces: "
// 	       << *VTX(a0(edge[0])) << " != " << *VTX(a0(edge[1])) << endl;
// 	  exit(0);
// 	}
      }
      else if (FMap->isMarked(edge[0], FInterEdgeMark) !=
	       FMap->isMarked(edge[1], FInterEdgeMark)) {
	cerr << "Intersection incohérante ignorée !" << endl;
      }
    }

    // Passage aux points suivants
    for (i = 0; i < 2; ++i) {
      if (next_point & (i+1)) {
	ip[i] = *it[i]++;
	prev_pos[i] = pos[i];
	pos[i] = ip[i]->getPosition();
	last_vertex[i] = NULL;
      }
    }
    prev_point = next_point;
  }

  freeMark(mark);

  MSG("Destruction des ensembles de points d'intersection");
  for (i = 0; i < 2; ++i) {
    for (it[i] = inter_set[i]->begin(); it[i] != inter_set[i]->end(); ++it[i])
      delete *it[i];
    delete inter_set[i];
  }
}

//******************************************************************************

void CCorefine3dFF::intersectCoplanarFaces(CDart * AFace1, CDart * AFace2,
					   const CPlane & APlane1,
					   const CPlane & APlane2,
					   int APositiveMark, int ANegativeMark,
					   int AFacesMark,
					   list<CDart*> * /*AFictiveVertices*/,
					   list<CDart*> * AFictiveEdges)
{
  DEBUG_FUNCTION;

  CDart *d, *face[2], *vtx[2], *edge[2];
  CPlane plane[2], edge_plane;
  CVertex edge_vector, pt1, pt2;
  int i, j, nb, nb_pos, nb_neg;
  int inter_mark = FMap->getNewMark();
  list<CDart*> inter_points;
  bool local_inside, inside;
//   TInterPt nearest_ip;

  face[0] = AFace1; plane[0] = APlane1;
  face[1] = AFace2; plane[1] = APlane2;
  for (i = 0; i < 2 && inter_points.empty(); ++i) {
    MSG("Recherche des points d'intersection entre les arêtes de la face "
	<< i << " et les arêtes de la face " << (i+1)%2);
    inside = true;
    d = face[i];
    do {
      vtx[0] = d;
      d = a1(a0(d));

      pt1 = *VTX(vtx[0]);
      pt2 = *VTX(a0(vtx[0]));
      edge_vector = pt2 - pt1;
      edge_plane.setPlane(plane[i].getNormal() * edge_vector, pt1);
      local_inside = false;

      MSG("arête testée : [" << pt1 << " ; " << pt2 << "]");

      classifyFaceVertices(face[(i+1)%2], edge_plane,
			   APositiveMark, ANegativeMark, AFacesMark,
			   false, &nb, &nb_pos, &nb_neg);
      if (nb_pos == nb)
	FMap->unmarkOrbit(face[(i+1)%2], ORBIT_01, APositiveMark);
      else if (nb_neg == nb)
	FMap->unmarkOrbit(face[(i+1)%2], ORBIT_01, ANegativeMark);
      else {
	TInterPtCmp comp(FTools, edge_vector);
	TInterPtSet *inter_set[2];
	TInterPtSet::iterator it[2];
	TInterPt *ip[2];
	TPositionInFace pos[2];
	int prev_point, next_point;
	
	inter_set[0] = new TInterPtSet(comp);
	ip[0] = new TInterPt(pt1, 0, FP_OnBorder);
	ip[0]->setEnteringSector(vtx[0]);
	ip[0]->setLeavingSector(vtx[0]);
	inter_set[0]->insert(ip[0]);
	ip[0] = new TInterPt(pt2, 0, FP_Outside);
	ip[0]->setLeavingSector(d);
	inter_set[0]->insert(ip[0]);
	
	inter_set[1] = findIntersectionPoints(face[(i+1)%2], plane[(i+1)%2],
					      edge_plane, edge_vector,
					      APositiveMark, ANegativeMark,
					      AFacesMark, comp);

	it[0] = inter_set[0]->begin();
	it[1] = inter_set[1]->begin();
	ip[0] = ip[1] = NULL;
	pos[0] = pos[1] = FP_Outside;
	prev_point = next_point = 0;

	// Parcours des ensembles et construction des segments d'intersection
	while (it[0] != inter_set[0]->end() && it[1] != inter_set[1]->end()) {
	  // Les points se trouvent au même endroit
	  if (FTools->arePointsEqual((*it[0])->getPoint(),
				     (*it[1])->getPoint()))
	    next_point = 3;
	  // Le point le plus proche se trouve sur l'arête
	  else if (comp(*it[0], *it[1]))
	    next_point = 1;
	  // Le point le plus proche se trouve sur la face
	  else
	    next_point = 2;

	  MSG("prev_point = " << prev_point);	
	  MSG("next_point = " << next_point);
	  MSG("pos[0] = " << pos[0]);
	  MSG("pos[1] = " << pos[1]);

	  // On se trouve entre les deux sommets de l'arête
	  if (pos[0] != FP_Outside) {
	    // On regarde s'il existe une intersection sur l'arête
	    if (local_inside && (prev_point != 1 || next_point != 1))
	      local_inside = false;

	    /* On regarde si le premier sommet de l'arête est à l'intérieur
	       de l'autre face */
	    if (prev_point & 1 && pos[1] != FP_Outside) {
	      if (pos[1] == FP_Inside ||
		  edgeInsideVector(vtx[0], plane[i])
		  .dot(edgeInsideVector(ip[1]->getEnteringSector(),
				       plane[(i+1)%2])) > 0.0) {
		MSG("Le premier sommet est à l'intérieur");
		FMap->setMark(vtx[0], FDoubleFaceMark);
	      }
	    }

	    if (pos[1] == FP_OnBorder || pos[1] == FP_OnReversedBorder) {
	      if (prev_point == 1) {
		vtx[1] = splitEdge(ip[1]->getEnteringSector(), pt1,
				   AFictiveEdges);
		if (pos[1] == FP_OnBorder)
		  ip[1]->setEnteringSector(vtx[1]);
	      }
	      else
		vtx[1] = ip[1]->getLeavingSector();
	      if (prev_point & 1 && !FMap->isMarked(vtx[0], inter_mark))
		addIntersectionPoint(vtx, i, AFacesMark, inter_mark,
				     &inter_points);

	      edge[0] = vtx[0];

	      if (next_point == 1) {
		vtx[0] = a1(a0(vtx[0]));
		vtx[1] = splitEdge(ip[1]->getEnteringSector(), pt2,
				   AFictiveEdges);
		if (pos[1] == FP_OnReversedBorder)
		  ip[1]->setEnteringSector(vtx[1]);
	      }
	      else {
		if (next_point == 2) {
		  vtx[0] = splitEdge(vtx[0], (*it[1])->getPoint(),
				     AFictiveEdges);
		}
		else if (next_point == 3) {
		  vtx[0] = a1(a0(vtx[0]));
		}
		vtx[1] = ((*it[1])->getPosition() == FP_Inside ?
			  (*it[1])->getEnteringSector() :
			  (*it[1])->getLeavingSector());
	      }

	      edge[1] = ip[1]->getEnteringSector();

	      if (!FMap->isMarked(vtx[0], inter_mark))
		addIntersectionPoint(vtx, i, AFacesMark, inter_mark,
				     &inter_points);
	      
	      MSG("edge[0] = [" << *VTX(edge[0]) << ";"
		  << *VTX(a0(edge[0])) << "]");
	      MSG("edge[1] = [" << *VTX(edge[1]) << ";"
		  << *VTX(a0(edge[1])) << "]");

	      if (!FMap->isMarked(edge[0], FInterEdgeMark) ||
		  !FMap->isMarked(edge[1], FInterEdgeMark)) {
		MSG("Une arête d'intersection avec un bord trouvée");
		FInterEdges.push_front(edge[(i+1)%2]);
		FInterEdges.push_front(edge[i]);

		if (!FMap->isMarked(edge[0], FInterEdgeMark))
		  FMap->markOrbit(edge[0], ORBIT_EDGE, FInterEdgeMark);
		else
		  cerr << "topologie de l'objet " << (i+2)%2
		       << " incorrecte !" << endl;
		if (!FMap->isMarked(edge[1], FInterEdgeMark))
		  FMap->markOrbit(edge[1], ORBIT_EDGE, FInterEdgeMark);
		else
		  cerr << "topologie de l'objet " << i+1
		       << " incorrecte !" << endl;

		if (FMap->isMarked(edge[0], FFictiveEdgeMark))
		  FMap->unmarkOrbit(edge[0], ORBIT_EDGE, FFictiveEdgeMark);
		if (FMap->isMarked(edge[1], FFictiveEdgeMark))
		  FMap->unmarkOrbit(edge[1], ORBIT_EDGE, FFictiveEdgeMark);
	      }
	      else {
		assert(FMap->isMarked(edge[1], FInterEdgeMark));
	      }
	    }
	    else {
	      if (prev_point == 3 && ip[1]->getCellDim() == 1) {
		vtx[1] = splitEdge(pos[1] == FP_Outside ?
				   ip[1]->getLeavingSector() :
				   ip[1]->getEnteringSector(), pt1,
				   AFictiveEdges);
		if (!FMap->isMarked(vtx[0], inter_mark))
		  addIntersectionPoint(vtx, i, AFacesMark, inter_mark,
				       &inter_points);
	      }

	      if (next_point != 1) {
		if (next_point == 2) {
		  vtx[0] = splitEdge(vtx[0], (*it[1])->getPoint(),
				     AFictiveEdges);
		  vtx[1] = ((*it[1])->getPosition() == FP_Inside ?
			    (*it[1])->getEnteringSector() :
			    (*it[1])->getLeavingSector());
		  if ((*it[1])->getCellDim() == 1)
		    vtx[1] = splitEdge(vtx[1], (*it[1])->getPoint(),
				       AFictiveEdges);
		}
		else {
		  vtx[0] = a1(a0(vtx[0]));
		  vtx[1] = ((*it[1])->getPosition() == FP_Inside ?
			    (*it[1])->getEnteringSector() :
			    (*it[1])->getLeavingSector());
		  if ((*it[1])->getCellDim() == 1)
		    vtx[1] = splitEdge(vtx[1], pt2, AFictiveEdges);
		}
		if (!FMap->isMarked(vtx[0], inter_mark))
		  addIntersectionPoint(vtx, i, AFacesMark, inter_mark,
				       &inter_points);
	      }
	    }
	    
	    /* On regarde si l'intersection suivante permet de rentrer dans
	       l'autre face */
	    if (next_point == 2 && (*it[1])->getPosition() != FP_Outside) {
	      if ((*it[1])->getPosition() == FP_Inside ||
		  edgeInsideVector(vtx[0], plane[i])
		  .dot(edgeInsideVector((*it[1])->getEnteringSector(),
				       plane[(i+1)%2])) > 0.0) {
		MSG("Le nouveau sommet est à l'intérieur");
		FMap->setMark(vtx[0], FDoubleFaceMark);
	      }
	    }
	  }
	  // On se trouve avant le premier sommet de l'arête
	  else if (!ip[0]) {
	    local_inside = (pos[1] == FP_Inside);
// 	    if (local_inside)
// 	      nearest_ip = *ip[1];
	  }

	  // Passage aux points suivants
	  for (j = 0; j < 2; ++j) {
	    if (next_point & (j+1)) {
	      ip[j] = *it[j]++;
	      pos[j] = ip[j]->getPosition();
	    }
	  }
	  prev_point = next_point;
	}

	for (j = 0; j < 2; ++j) {
	  for (it[j] = inter_set[j]->begin(); it[j] != inter_set[j]->end();
	       ++it[j]) delete *it[j];
	  delete inter_set[j];
	}
      }
      MSG("local_inside = " << local_inside);
      if (!local_inside) inside = false;
    }
    while (d != face[i]);

    MSG(inter_points.size() << " points d'intersection trouvés");

//     if (inside) {
//       MSG("La première face se trouve à l'intérieur de l'autre");
//       assert(inter_points.empty());
//       vtx[1] = nearest_ip.getEnteringSector();
//       if (nearest_ip.getCellDim() == 1) {
// 	vtx[1] = splitEdge(vtx[1], nearest_ip.getPoint(), AFictiveEdges);
// 	FMap->markOrbit(vtx[1], ORBIT_VERTEX, FFictiveVertexMark);
// 	AFictiveVertices->push_back(vtx[1]);
//       }
//       vtx[1] = insertVertexInFace(vtx[1], pt1);
//       AFictiveEdges->push_back(vtx[1]);
//       addIntersectionPoint(vtx, i, AFacesMark, inter_mark, &inter_points);
//     }
    if (inside) {
      MSG("La première face se trouve à l'intérieur de l'autre");
      assert(inter_points.empty());

      TCoordinate dist, best_dist = 0;
      CPlane ref_plane(vertexInsideVector(d, plane[i]), *VTX(d));
      
      MSG("Recherche d'un sommet de l'enveloppe convexe de la face");
      vtx[0] = d;
      d = a1(a0(d));
      do {
	dist = ref_plane.pointDistance(*VTX(d));
	if (dist < best_dist) {
	  best_dist = dist;
	  vtx[0] = d;
	}
	d = a1(a0(d));
      }
      while (d != face[i]);

      edge_vector = -vertexInsideVector(vtx[0], plane[i]);
      edge_plane.setPlane(plane[i].getNormal() * edge_vector, *VTX(vtx[0]));

      MSG("Calcul des points d'intersection entre un vecteur et l'autre face");
      TInterPtCmp comp(FTools, edge_vector);
      TInterPtSet *inter_set;
      TInterPtSet::iterator it;
      inter_set = findIntersectionPoints(face[(i+1)%2], plane[(i+1)%2],
					 edge_plane, edge_vector,
					 APositiveMark, ANegativeMark,
					 AFacesMark, comp);

      MSG("Recherche du point d'intersection le plus proche du sommet");
      it = inter_set->begin();
      while (it != inter_set->end() &&
	     (*VTX(vtx[0]) - (*it)->getPoint()).dot(edge_vector) > 0.0) ++it;

      MSG("Recherche d'un sommet de l'autre face pour insérer une arête");
      vtx[1] = ((*it)->getPosition() == FP_Inside ?
		(*it)->getEnteringSector() : (*it)->getLeavingSector());
      if ((*it)->getCellDim() == 1) {
	CPlane clip_plane1(edgeInsideVector(vtx[1], plane[(i+1)%2]),
			   (*it)->getPoint());
	CPlane clip_plane2(edge_vector, *VTX(vtx[0]));
	vtx[1] = findNearestFaceVertex(vtx[1], edge_plane,
				       clip_plane1, clip_plane2);
	assert(vtx[1]);
      }
      MSG("Insertion de l'arête");
      vtx[1] = insertVertexInFace(vtx[1], *VTX(vtx[0]));
      AFictiveEdges->push_back(vtx[1]);
      addIntersectionPoint(vtx, i, AFacesMark, inter_mark, &inter_points);
    }
  }

  MSG("Création des arêtes d'intersection");
  int orient_mark = FMap->getNewMark();
  list<CDart*>::iterator it;

  for (it = inter_points.begin(); it != inter_points.end(); ++it) {
    CDart *d1, *d2;

    vtx[0] = *it;
    vtx[1] = LINK(*it);

    for (i = 0; i < 2; ++i) {
      MSG("Parcours des arêtes de la face " << i+1 << " autour du sommet "
	  << *VTX(vtx[i]));
      FMap->halfMarkOrbit(vtx[i], ORBIT_VERTEX, orient_mark);
      for (CDynamicCoverageVertex dcv(FMap, vtx[i]); dcv.cont(); ++dcv)
	if (FMap->isMarked(*dcv, orient_mark)) {
	  FMap->unsetMark(*dcv, orient_mark);
	  if (FMap->isMarked(*dcv, inter_mark)) {
	    FMap->unsetMark(*dcv, inter_mark);
	    d = *dcv;
	    do {
	      if (!FMap->isMarked(d, FInterEdgeMark)) {
		MSG("L'arête courante n'est pas une arête d'intersection");
		assert(LINK(d));
		edge_vector = *VTX(a0(d)) - *VTX(d);
		d1 = findFaceSector(LINK(d), plane[(i+1)%2],
				    AFacesMark, edge_vector);
		if (d1) {
		  MSG("L'arête est à l'intérieur de l'autre face");
		  d2 = LINK(a0(d));
		  if (d2) {
		    d2 = findFaceSector(d2, plane[(i+1)%2],
					AFacesMark, -edge_vector);
		    assert(d2);
		    d1 = splitFace(d1, d2);
		  }
		  else
		    d1 = insertEdgeInFace(d1, *VTX(a0(d)));
		  linkVertices(a1(a0(d)), a1(a0(d1)));
		  if (FMap->isMarked(d, FFictiveEdgeMark))
		    FMap->unmarkOrbit(d, ORBIT_EDGE, FFictiveEdgeMark);
		  FMap->markOrbit(d, ORBIT_EDGE, FInterEdgeMark);
		  FMap->markOrbit(d1, ORBIT_EDGE, FInterEdgeMark);
		  if (i == 0) {
		    FInterEdges.push_front(d1);
		    FInterEdges.push_front(d);
		  }
		  else {
		    FInterEdges.push_front(d);
		    FInterEdges.push_front(d1);
		  }
		}
		else
		  FMap->setMark(a0(d), inter_mark);
	      }
	      d = a1(a0(d));
	    }
	    while (!FMap->isMarked(a1(d), inter_mark));
	    FMap->unsetMark(a1(d), inter_mark);
	  }
	}
    }
  }
  for (it = inter_points.begin(); it != inter_points.end(); ++it) {
    vtx[0] = *it;
    vtx[1] = LINK(*it);
    for (i = 0; i < 2; ++i)
      for (CDynamicCoverageVertex dcv(FMap, vtx[i]); dcv.cont(); ++dcv) {
	FMap->unsetMark(*dcv, inter_mark);
	if (FMap->isMarked(*dcv, FDoubleFaceMark) &&
	    !FMap->isMarked(a1(*dcv), FDoubleFaceMark)) {
	  FMap->markOrbit(*dcv, ORBIT_FACE, FDoubleFaceMark);
	  FDoubleFaces.push_back(*dcv);
	}
      }
  }

  freeMark(inter_mark);
  freeMark(orient_mark);
}

//******************************************************************************

void CCorefine3dFF::assignFacesPlaneInfo(int ADirectInfo, int ANegativeMark,
					 TCorefFaceList * AList)
{
  DEBUG_FUNCTION;

  for (TCorefFaceList::iterator it = AList->begin(); it != AList->end(); ++it)
    for (CDynamicCoverage01 cov(FMap, it->face); cov.cont(); ++cov) {
      FMap->setDirectInfo(*cov, ADirectInfo, &it->plane);
      FMap->setDirectInfo(a3(*cov), ADirectInfo, &it->plane);
      FMap->setMark(a3(*cov), ANegativeMark);
    }
}

//******************************************************************************

void CCorefine3dFF::removeFacesPlaneInfo(int /*ADirectInfo*/, int ANegativeMark,
					 TCorefFaceList * AList)
{
  DEBUG_FUNCTION;

  for (TCorefFaceList::iterator it = AList->begin(); it != AList->end(); ++it)
    for (CDynamicCoverage01 cov(FMap, a3(it->face)); cov.cont(); ++cov)
      FMap->unsetMark(*cov, ANegativeMark);
}

//******************************************************************************

void CCorefine3dFF::sortFacesAroundEdges(int AFacePlaneDI, int ANegativeMark)
{
  DEBUG_FUNCTION;

  list<CDart*>::iterator it;
  TFaceSet *face_set;
  CDart *edge1, *edge2;

  for (it = FInterEdges.begin(); it != FInterEdges.end();) {
    edge1 = *it++;
    edge2 = *it++;
    if (edge1 && edge2) {
      face_set = sortFacesAroundEdges(edge1, edge2,
				      AFacePlaneDI, ANegativeMark);
      linkSortedFaces(face_set);
      delete face_set;
    }
  }
}

//******************************************************************************
#define VECT(d) (FMap->isMarked(d, ANegativeMark) ?\
                 -((CPlane*)FMap->getDirectInfo(d, AFacePlaneDI))->getNormal():\
                  ((CPlane*)FMap->getDirectInfo(d, AFacePlaneDI))->getNormal())

TFaceSet * CCorefine3dFF::sortFacesAroundEdges(CDart * AEdge1, CDart * AEdge2,
					       int AFacePlaneDI,
					       int ANegativeMark)
{
  DEBUG_FUNCTION;

  CDart *d1, *d2;
  CVertex axis = *VTX(a0(AEdge1)) - *VTX(AEdge1);
  CAngularFaceComparator comp(FMap, FTools, axis, VECT(AEdge1),
			      FVertexDI, AFacePlaneDI, ANegativeMark);
  TFaceSet *faces = new TFaceSet(comp);

  // Ajout des premières faces
  d1 = AEdge1;
  do {
    faces->insert(d1);
    d1 = a3(FTools->alpha2(d1));
  }
  while (d1 != AEdge1);

  // Ajout des secondes faces
  if ((*VTX(a0(AEdge2)) - *VTX(AEdge2)).dot(axis) < 0.0)
    AEdge2 = a3(a0(AEdge2));
  d2 = AEdge2;
  do {
    faces->insert(d2);
    d2 = a3(FTools->alpha2(d2));
  }
  while (d2 != AEdge2);

  return faces;
}

//******************************************************************************

// TFaceSet * CCorefine3dFF::sortFacesAroundEdges(CDart * AEdge1, CDart * AEdge2,
// 					       int AFacePlaneDI,
// 					       int ANegativeMark)
// {
//   DEBUG_FUNCTION;

//   CDart *d1, *d2;
//   CVertex axis = *VTX(a0(AEdge1)) - *VTX(AEdge1);
//   TFaceSet *faces = new TFaceSet;
//   CPlane axis_plane(axis, *VTX(AEdge1));
//   CVertex v, v1, v2;

//   if (FTools->edgeVector(AEdge2, FVertexDI).dot(axis) < 0.0)
//     AEdge2 = a3(a0(AEdge2));

//   if (FMap->isFree2(AEdge1) && FMap->isFree2(a3(AEdge1))) {
//     CDart *tmp = AEdge1;
//     AEdge1 = AEdge2;
//     AEdge2 = tmp;
//   }

//   if (FMap->isFree2(AEdge2) && FMap->isFree2(a3(AEdge2))) {
//     faces->push_back(AEdge2);

//     if (FMap->isFree2(AEdge1) && FMap->isFree2(a3(AEdge1))) {
//       faces->push_back(AEdge1);
//     }
//     else {
//       v = VECT(d2);
//       v1 = VECT(AEdge1);
//       v2 = VECT(a3(FTools->alpha2(AEdge1)));
//       while (!FTools->isVectorInSector(v, v1, v2, axis_plane, false, true)) {
// 	AEdge1 = a3(FTools->alpha2(AEdge1));
// 	v1 = v2;
// 	v2 = VECT(a3(FTools->alpha2(AEdge1)));
//       }
//       d1 = AEdge1;
//       do {
// 	d1 = a3(FTools->alpha2(d1));
// 	faces->push_back(d1);
//       }
//       while (d1 != AEdge1);
//     }
//   }
//   else {
//     int mark = FMap->getNewMark();

//     d1 = AEdge1;
//     v1 = VECT(d1);
//     v2 = VECT(a3(FTools->alpha2(d1)));

//     while (!FTools->isVectorInSector(VECT(AEdge2), v1, v2,
// 				     axis_plane, false, true))
//       AEdge2 = a3(FTools->alpha2(AEdge2));

//     d2 = AEdge2;
//     do {
//       v = VECT(d2);

//       while (!FTools->isVectorInSector(v, v1, v2, axis_plane, false, true)) {
// 	d1 = a3(FTools->alpha2(d1));
// 	v1 = v2;
// 	v2 = VECT(a3(FTools->alpha2(d1)));
// 	assert(!FMap->isMarked(d1, mark));
// 	faces->push_back(d1);
// 	FMap->setMark(d1, mark);
//       }

//       faces->push_back(d2);
//       d2 = a3(FTools->alpha2(d2));
//     }
//     while (d2 != AEdge2);

//     if (d1 != AEdge1 || !FMap->isMarked(d1, mark))
//       do {
// 	d1 = a3(FTools->alpha2(d1));
// 	assert(!FMap->isMarked(d1, mark));
// 	faces->push_back(d1);
// 	FMap->setMark(d1, mark);
//       }
//       while (d1 != AEdge1);

//     do {
//       assert(FMap->isMarked(d1, mark));
//       FMap->unsetMark(d1, mark);
//       d1 = a3(FTools->alpha2(d1));
//     }
//     while (d1 != AEdge1);

//     freeMark(mark);
//   }

//   return faces;
// }

#undef VECT
//******************************************************************************

void CCorefine3dFF::linkSortedFaces(TFaceSet * AFaceSet)
{
  DEBUG_FUNCTION;

  CDart *d1, *d2;
  TFaceSet::iterator it;

  for (it = AFaceSet->begin(); it != AFaceSet->end(); ) {
    d1 = *it++;
    d2 = (it != AFaceSet->end()) ? *it : *AFaceSet->begin();

    if (a2(d1) != a3(d2)) {
      if (!FMap->isFree2(d1)) FMap->unsew2(d1);
      if (!FMap->isFree2(a3(d2))) FMap->unsew2(a3(d2));
      FMap->sew2(d1, a3(d2));
      FMap->pointDirectInfoToAttributeVertex(FVertexDI, d1);
      FMap->pointDirectInfoToAttributeVertex(FVertexDI, a0(d1));
    }
  }
}

//******************************************************************************

void CCorefine3dFF::spreadMarksAroundEdges(TFaceSet * AFaceSet,
					   bitset<NB_MARKS> AMarks)
{
  DEBUG_FUNCTION;

  CDart *d1, *d2;
  TFaceSet::iterator it;
  int m;

  for (it = AFaceSet->begin(); it != AFaceSet->end(); ) {
    d1 = *it++;
    d2 = (it != AFaceSet->end()) ? *it : *AFaceSet->begin();

    for (m = 0; m < NB_MARKS; ++m)
      if (AMarks[m] && a2(d1) != a3(d2)) {
	if (FMap->isMarked(d1, m)) FMap->setMark(a3(d2), m);
	else if (FMap->isMarked(a3(d2), m)) FMap->setMark(d1, m);
	if (FMap->isMarked(a0(d1), m)) FMap->setMark(a0(a3(d2)), m);
	else if (FMap->isMarked(a0(a3(d2)), m)) FMap->setMark(a0(d1), m);
      }
  }
}

//******************************************************************************

bool CCorefine3dFF::isPointInFace(const CVertex & APoint, CDart * AFace,
				  const CPlane & APlane)
{
  CVertex line = *VTX(AFace) - APoint;

  if (FTools->isVectorNull(line))
    return true;

  TInterPtCmp comp(FTools, line);
  CPlane plane(APlane.getNormal() * line, *VTX(AFace));
  int face_mark = FMap->getNewMark();
  int positive_mark = FMap->getNewMark();
  int negative_mark = FMap->getNewMark();
  TInterPtSet *inter_set;
  
  FMap->markOrbit(AFace, ORBIT_01, face_mark);
  classifyFaceVertices(AFace, plane, positive_mark, negative_mark,
		       face_mark, false);
  inter_set = findIntersectionPoints(AFace, APlane, plane, line,
				     positive_mark, negative_mark,
				     face_mark, comp);
  assert(!inter_set->empty());

  TInterPtSet::iterator it;
  TPositionInFace pos = FP_Outside;
  for (it = inter_set->begin(); it != inter_set->end(); ++it) {
    if (FTools->arePointsEqual((*it)->getPoint(), APoint)) {
      pos = FP_Inside;
      break;
    }
    else if ((APoint - (*it)->getPoint()).dot(line) > 0.0)
      pos = (*it)->getPosition();
    else
      break;
  }

  for (it = inter_set->begin(); it != inter_set->end(); ++it)
    delete *it;
  delete inter_set;

  return pos == FP_Inside;
}

//******************************************************************************
