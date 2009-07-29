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

#include "corefine-2d-propagation.hh"
#include "time.hh"

// #define DEBUG_MESSAGES
#include "message-macros.hh"

#include INCLUDE_NON_INLINE("corefine-2d-propagation.icc")

using namespace std;
using namespace GMap3d;

//******************************************************************************

#define INFO_MESSAGES

#ifdef INFO_MESSAGES
#define INFO(s) (cout << s << endl)
#else // INFO_MESSAGES
#define INFO(s)
#endif // INFO_MESSAGES

//******************************************************************************

#define a0 FMap->alpha0
#define a1 FMap->alpha1
#define a2 FMap->alpha2
#define a3 FMap->alpha3

#define b1(d) ((CDart*)FMap->getDirectInfo(d, FAlpha1DI))

//******************************************************************************

CCorefine2dPropagation::CCorefine2dPropagation(CGMapVertex * AMap,
					       const CPlane & APlane,
					       bool ACalculateOrientation,
					       TCoordinate AEpsilon,
					       int AVertexDI)
  : CCorefine(AMap, AEpsilon), FPlane(APlane), FTools(AMap, AEpsilon),
    FCalculateOrientation(ACalculateOrientation), FNumberOfIntersections(0)
{
  ENTER;

  FBestProj = FPlane.getBestProjection();

  if (AVertexDI < 0) {
    FVertexDI = FMap->getNewDirectInfo();
    MSG("FVertexDI = FMap->getNewDirectInfo() = " << FVertexDI);
    FLocalVertexDirectInfo = true;
  }
  else {
    FVertexDI = AVertexDI;
    FLocalVertexDirectInfo = false;
  }

  FAlpha1DI = FMap->getNewDirectInfo();
  MSG("FAlpha1DI = FMap->getNewDirectInfo() = " << FAlpha1DI);

  EXIT;
}

//******************************************************************************

CCorefine2dPropagation::~CCorefine2dPropagation()
{
  ENTER;

  if (FLocalVertexDirectInfo) {
    MSG("FMap->freeDirectInfo(FVertexDI = " << FVertexDI << ")");
    FMap->freeDirectInfo(FVertexDI);
  }
  
  MSG("FMap->freeDirectInfo(FAlpha1DI = " << FAlpha1DI << ")");
  FMap->freeDirectInfo(FAlpha1DI);

  EXIT;
}

//******************************************************************************

int CCorefine2dPropagation::corefine(CDart *& AMesh1, CDart *& AMesh2,
				     bitset<NB_MARKS> ACopyMarks)
{
  TCoordinate t;
  list<CDart*> vertex_list;
  CEdgeIntersection inter;
  CDart *vertex, *edge, *face, *tmp_edge, *tmp_face;
  int edge_mark = FMap->getNewMark();
  int vertex_mark = FMap->getNewMark();
  CVertex *pt1, *pt2, tmp_pt;
  CTime start, end;

  FCopyMarks = ACopyMarks;

  ENTER;

  INFO("Initialisation des maillages");
  start.updateTime();

  initMesh(AMesh1);
  initMesh(AMesh2);

  end.updateTime();
  FInitialisationTime = end - start;
  INFO("Durée de l'initialisation : " << FInitialisationTime);

  INFO("Orientation des maillages");
  start.updateTime();

  if (FCalculateOrientation) {
    AMesh1 = FTools.findWellOrientedDart(AMesh1, FVertexDI);
    AMesh2 = FTools.findWellOrientedDart(AMesh2, FVertexDI);
  }

  vertex = AMesh1;
  face = AMesh2;

  end.updateTime();
  FOrientationTime = end - start;
  INFO("Durée de l'orientation : " << FOrientationTime);

//   cout << FTools.faceNormalVector(vertex, FVertexDI) << endl;
//   FTools.displayFaceVertices(vertex, FVertexDI);
//   cout << FTools.faceNormalVector(face, FVertexDI) << endl;
//   FTools.displayFaceVertices(face, FVertexDI);

  INFO("Localisation de la première face");
  start.updateTime();

  // Recherche de la face du deuxième maillage contenant le brin de départ du
  // premier maillage
  face = FTools.localizePointInMesh(*getVertex(vertex), face, FVertexDI);
  
//   cout << "\033[0;37m"
//        << "Point de départ sur le premier maillage : "
//        << *getVertex(vertex) << endl
//        << "Sommets de la face du second maillage contenant ce point : "
//        << endl;
//   FTools.displayFaceVertices(face, FVertexDI);
//   cout << "\033[0m";

  end.updateTime();
  FLocalizationTime = end - start;
  INFO("Durée de la localisation : " << FLocalizationTime);

  pt1 = getVertex(face);
  pt2 = getVertex(a0(face));
  tmp_pt = *getVertex(vertex);

  // Si le brin d1 se trouve sur l'arête incidente à d2, on éclate cette arête
  switch(FTools.localizePointOnEdge(tmp_pt, *pt1, *pt2, &t)) {
  case EP_OnEdge:
    face = splitEdge(face, tmp_pt);
    weldVertices(vertex, face);
    FMap->markOrbit(vertex, ORBIT_VERTEX, vertex_mark);
    break;

  case EP_OnSecondVertex:
    face = a2(a0(face));
  case EP_OnFirstVertex:
    modifyVertex(face, tmp_pt);
    weldVertices(vertex, face);
    FMap->markOrbit(vertex, ORBIT_VERTEX, vertex_mark);
    break;

  default:
    break;
  }

  INFO("Parcours des maillages et création des intersections");
  start.updateTime();

  // Ajoute le sommet et la face courants dans la liste
  vertex_list.push_back(vertex);
  vertex_list.push_back(face);

  // Tant qu'il y a des éléments dans la liste, on continue le parcours
  while(!vertex_list.empty()) {

    // Récupération des prochains sommets et faces à traiter
    vertex = vertex_list.front(), vertex_list.pop_front();
    face = vertex_list.front(), vertex_list.pop_front();

    edge = vertex;

    MSG("Parcours du sommet : " << *getVertex(vertex));

    // Parcours des arêtes incidentes au sommet
    do {
      if (!FMap->isMarked(edge, edge_mark)) {

	pt1 = getVertex(edge);
	pt2 = getVertex(a0(edge));
	
#ifdef DEBUG_MESSAGES
  	cout << "Parcours de l'arête : ";
  	FTools.displayEdgeVertices(edge, FVertexDI);
#endif // DEBUG_MESSAGES

	/* Si le sommet d'origine de l'arête est marqué, c'est qu'il possède une
	 * intersection avec une arête ou un sommet de l'autre maillage. Il
	 * faut donc rechercher la face contenant l'arête qu'on longe.
	 */
	if (FMap->isMarked(edge, vertex_mark)) {
// 	  tmp_face = FTools.findSectorOfVector(*pt2 - *pt1, face, FVertexDI);

	  /* On peut éviter de chercher le bon secteur à chaque fois si
	   * l'interclassement fonctionne correctement...
	   */
	  tmp_face = a2(edge);
	  while (b1(tmp_face) == NULL)
	    tmp_face = a2(a1(tmp_face));
	  tmp_face = b1(tmp_face);

	  inter = FTools.findNearestIntersection(*pt1, *pt2, tmp_face, true,
						 FVertexDI);
	}
	else {
	  tmp_face = face;

	  inter = FTools.findNearestIntersection(*pt1, *pt2, tmp_face, false,
						 FVertexDI);
	}

#ifdef DEBUG_MESSAGES
  	cout << "Recherche d'une intersection dans la face :" << endl;
  	FTools.displayFaceVertices(tmp_face, FVertexDI);
#endif

	if (inter.getCell() != NULL &&
	    (inter.getPosition() != EP_OnSecondVertex ||
	     !FMap->isMarked(a2(a0(edge)), vertex_mark))) {
#ifdef DEBUG_MESSAGES
 	  cout << "Une intersection trouvée dans la face avec l'arête :\n";
 	  FTools.displayEdgeVertices(inter.getCell(), FVertexDI);
 	  cout << "Première arête : ";
#endif
	    
	  if (inter.getPosition() == EP_OnEdge) {
 	    MSG("Intersection franche");
	    tmp_edge = splitEdge(edge, inter.getPoint());
	    tmp_pt = inter.getPoint();
	  }
	  else if (inter.getPosition() == EP_OnFirstVertex) {
	    cout << "\033[0;31m"
		 << "Intersection avec le sommet d'origine de l'arête "
		 << *pt1 << " et ";
	    if (inter.getCellDimension() == 0)
	      cout << "le sommet " << *getVertex(inter.getCell());
	    else
	      cout << "l'arête [" << *getVertex(inter.getCell())
		   << "," << *getVertex(a0(inter.getCell())) << "]";
	    cout << "\033[0m\n";
	    
	    tmp_edge = edge;
	    tmp_pt = *pt1;
	  }
	  else {
 	    MSG("Intersection avec le sommet extrémité de l'arête");
	    tmp_edge = a2(a0(edge));
	    tmp_pt = *pt2;
	  }
	  
 	  MSG("Deuxième arête : ");
	    
	  if (inter.getCellDimension() == 1) {
 	    MSG("Intersection franche");
	    tmp_face = splitEdge(inter.getCell(), tmp_pt);
	  }
	  else {
 	    MSG("Intersection avec un sommet");
	    tmp_face = inter.getCell();
	    modifyVertex(inter.getCell(), tmp_pt);
	  }
	  
	  if (inter.getPosition() != EP_OnFirstVertex ||
	      !FMap->isMarked(edge, vertex_mark)) {
	    weldVertices(tmp_edge, tmp_face);
	  }
	  else {
	    cout << "\033[1;31m"
		 << "Interclassement de plus de deux sommets " << *pt1
		 << "\033[0m\n";

	    /* Cas très délicat à gérer !!!
	     * Ici, comme le point d'intersection est proche du sommet
	     * d'origine A de l'arête, il faut faire un interclassement entre
	     * le nouveau sommet et tous les sommets déjà interclassés en A.
	     */
	    //weldMultipleVertices(tmp_edge, tmp_face);
	  }

	  FMap->markOrbit(tmp_edge, ORBIT_VERTEX, vertex_mark);
	}
	else {
	  tmp_edge = a2(a0(edge));
	}

	vertex_list.push_back(tmp_edge);
	vertex_list.push_back(tmp_face);

	FMap->markOrbit(edge, ORBIT_EDGE, edge_mark);
      }

      // Passage à l'arête suivante sur le sommet
      edge = a2(a1(edge));
    }
    while (edge != vertex);
  }

  FMap->unmarkOrbit(AMesh1, ORBIT_CC, edge_mark);
  FMap->unmarkOrbit(AMesh1, ORBIT_CC, vertex_mark);

  FMap->freeMark(edge_mark);
  FMap->freeMark(vertex_mark);

  end.updateTime();
  FPropagationTime = end - start;
  INFO("Durée de la recherche des intersections : " << FPropagationTime);

  if (FNumberOfIntersections != 0) {
    INFO("Assemblage final des maillages");
    start.updateTime();
    applyModifications(AMesh1);
    end.updateTime();
    FAssemblyTime = end - start;
  }
  else {
    INFO("Aucune intersection entre les maillages "
	 << "==> insertion d'une arête fictive");
    start.updateTime();
    tmp_face = FTools.localizePointInMesh(*getVertex(face), AMesh1, FVertexDI);
    FMap->insertEdge(tmp_face, a1(face));
    end.updateTime();
    FAssemblyTime = end - start;
  }
  INFO("Durée de l'assemblage : " << FAssemblyTime);

  INFO("Nettoyage des maillages");
  start.updateTime();

  cleanMesh(AMesh1);

  if (FNumberOfIntersections == 0)
    cleanMesh(AMesh2);

  end.updateTime();
  FCleaningTime = end - start;
  INFO("Durée du nettoyage : " << FCleaningTime);

  if (FNumberOfIntersections != 0) {
    INFO("Suppression des arêtes doubles");
    start.updateTime();

    FTools.removeDoubleEdges(AMesh1, AMesh2, FCopyMarks);

    end.updateTime();
    FDoubleEdgesCleaningTime = end - start;
    INFO("Durée de la suppression : " << FDoubleEdgesCleaningTime);

    INFO("Nombre d'intersections réalisées : "
	 << FNumberOfIntersections);
  }
  else FDoubleEdgesCleaningTime.setTime(0, 0);
  
  EXIT;

  return FNumberOfIntersections;
}

//******************************************************************************

const CTime & CCorefine2dPropagation::getInitialisationTime() const
{
  return FInitialisationTime;
}

//------------------------------------------------------------------------------

const CTime & CCorefine2dPropagation::getOrientationTime() const
{
  return FOrientationTime;
}

//------------------------------------------------------------------------------

const CTime & CCorefine2dPropagation::getLocalizationTime() const
{
  return FLocalizationTime;
}

//------------------------------------------------------------------------------

const CTime & CCorefine2dPropagation::getPropagationTime() const
{
  return FPropagationTime;
}

//------------------------------------------------------------------------------

const CTime & CCorefine2dPropagation::getAssemblyTime() const
{
  return FAssemblyTime;
}

//------------------------------------------------------------------------------

const CTime & CCorefine2dPropagation::getCleaningTime() const
{
  return FCleaningTime;
}

//------------------------------------------------------------------------------

const CTime & CCorefine2dPropagation::getDoubleEdgesCleaningTime() const
{
  return FDoubleEdgesCleaningTime;
}

//******************************************************************************

void CCorefine2dPropagation::initMesh(CDart * AMesh)
{
  ENTER;

  int vert_mark = FMap->getNewMark();
  CAttributeVertex *vert;

  for (CStaticCoverageCC scc2(FMap, AMesh); scc2.cont(); ++scc2)
    if (FMap->isFree2(*scc2))
      FMap->stopUp(*scc2, 2);

  for (CStaticCoverageCC scc1(FMap, AMesh); scc1.cont(); ++scc1)
    if (FMap->isFree1(*scc1))
      FMap->linkAlpha1(*scc1, a2(*scc1));

  for (CDynamicCoverageCC dcc(FMap, AMesh); dcc.cont(); ++dcc) {
    FMap->setDirectInfo(*dcc, FAlpha1DI, NULL);

    if (!FMap->isMarked(*dcc, vert_mark)) {
      vert = FMap->findVertex(*dcc);

      *(CVertex*)vert = FPlane.projectPoint(*vert, FBestProj);

//       if (FLocalVertexDirectInfo) {
	for (CDynamicCoverageVertex dcv(FMap, *dcc) ; dcv.cont() ; dcv++) {
	  FMap->setMark(*dcv, vert_mark);
	  FMap->setDirectInfo(*dcv, FVertexDI, vert);
	}
//       }
//       else {
// 	FMap->markOrbit(*dcc, ORBIT_VERTEX, vert_mark);
//       }
    }
  }

  FMap->unmarkOrbit(AMesh, ORBIT_CC, vert_mark);
  FMap->freeMark(vert_mark);

  EXIT;
}

//******************************************************************************

void CCorefine2dPropagation::cleanMesh(CDart * AMesh)
{
  ENTER;

  int vert_mark = FMap->getNewMark();
  CAttributeVertex *vert;

  for (CDynamicCoverageCC dcc(FMap, AMesh) ; dcc.cont() ; dcc++) {
    FMap->setDirectInfo(*dcc, FAlpha1DI, NULL);

    if (!FMap->isMarked(*dcc, vert_mark)) {
      FMap->markOrbit(*dcc, ORBIT_VERTEX, vert_mark);
      vert = FMap->findVertex(*dcc);

      *(CVertex*)vert = FPlane.unprojectPoint(*vert, FBestProj);
    }
  }

  FMap->unmarkOrbit(AMesh, ORBIT_CC, vert_mark);
  FMap->freeMark(vert_mark);

  EXIT;
}

//******************************************************************************

CDart * CCorefine2dPropagation::splitEdge(CDart * AEdge,
					  const CVertex & AVertex)
{
  ENTER;

  CDart *d = a1(FMap->CGMapGeneric::insertVertex(AEdge));

  CAttributeVertex *att = new CAttributeVertex(AVertex);

  FMap->setVertex(d, att);

  for (CDynamicCoverageVertex dcv(FMap, d) ; dcv.cont() ; dcv++) {
    FMap->setDirectInfo(*dcv, FVertexDI, att);
    FMap->setDirectInfo(*dcv, FAlpha1DI, NULL);

    for (int i = 0; i < NB_MARKS; i++)
      if (FCopyMarks[i] && FMap->isMarked(a0(*dcv), i))
	FMap->setMark(*dcv, i);
  }

  EXIT;

  return d;
}

//******************************************************************************

// void CCorefine2dPropagation::weldVertices(CDart * AVertex1, CDart * AVertex2)
// {
//   list<CDart*> * l = FTools.sortVerticesEdges(AVertex1, AVertex2, FVertexDI);
//   list<CDart*>::iterator it;
//   CDart *d1, *d2;

//   if (l == NULL) {
//     cout << "\033[1;33m"
// 	 << "Erreur lors du tri angulaire => fusion des sommets impossible !"
// 	 << "\033[0m\n";

//     return;
//   }

//   ENTER;

//   it = l->begin();
//   d2 = *it;

//   do {
//     d1 = d2;
//     it++;
    
//     if (it != l->end())
//       d2 = *it;
//     else
//       d2 = *l->begin();

//     if (a1(d1) != a2(d2)) {
//       FMap->setDirectInfo(d1, FAlpha1DI, a2(d2));
//       FMap->setDirectInfo(a2(d2), FAlpha1DI, d1);
//     }
//     else {
//       FMap->setDirectInfo(d1, FAlpha1DI, NULL);
//       FMap->setDirectInfo(a2(d2), FAlpha1DI, NULL);
//     }
//   }
//   while (it != l->end());

//   delete l;

//   FNumberOfIntersections++;

//   EXIT;
// }

//******************************************************************************

class CAngularEdgeComparator
{
 public:
  CAngularEdgeComparator(CGMapVertex * AMap, const CVertex & ACenter,
			 int AVertexDI)
    : FMap(AMap), FCenter(ACenter), FVertexDI(AVertexDI) {}
  
  bool operator () (CDart * AEdge1, CDart * AEdge2)
  {
    CVertex v1 = *getVertex(a0(AEdge1)) - FCenter;
    CVertex v2 = *getVertex(a0(AEdge2)) - FCenter;
    return (v1 * v2).getZ() > 0.0;
  }
  
 private:

  CAttributeVertex * getVertex(CDart * ADart) const
  { return (CAttributeVertex*)FMap->getDirectInfo(ADart, FVertexDI); }

 private:
  CGMapVertex *FMap;
  CVertex FCenter;
  int FVertexDI;
};

void CCorefine2dPropagation::weldVertices(CDart * AVertex1, CDart * AVertex2)
{
  CAngularEdgeComparator comp(FMap, *getVertex(AVertex1), FVertexDI);
  multiset<CDart*, CAngularEdgeComparator> edges(comp);
  multiset<CDart*, CAngularEdgeComparator>::iterator it, tmp_it;
  CDart *d1, *d2;

  // Ajout des premières arêtes
  d1 = AVertex1;
  do {
    edges.insert(d1);
    d1 = a2(FTools.alpha1(d1));
  }
  while (d1 != AVertex1);

  // Ajout des secondes arêtes
  d2 = AVertex2;
  do {
    edges.insert(d2);
    d2 = a2(FTools.alpha1(d2));
  }
  while (d2 != AVertex2);

  for (it = edges.begin(); it != edges.end(); ) {
    tmp_it = it++;

    d1 = *tmp_it;
    d2 = (it != edges.end()) ? *it : *edges.begin();

    if (a1(d1) != a2(d2)) {
      FMap->setDirectInfo(d1, FAlpha1DI, a2(d2));
      FMap->setDirectInfo(a2(d2), FAlpha1DI, d1);
    }
  }

  ++FNumberOfIntersections;
}

//******************************************************************************

void CCorefine2dPropagation::weldMultipleVertices(CDart * AVertex1,
						  CDart * AVertex2)
{
  list<CDart*> vertices;
  list<CDart*>::iterator it;
  CDart *d1, *d2;
  int mark = FMap->getNewMark();

  FMap->markOrbit(AVertex1, ORBIT_VERTEX, mark);

  // Ajout des sommets déjà reliés au premier sommet
  d1 = AVertex1;
  do {
    d2 = b1(d1);

    if (d2 != NULL && !FMap->isMarked(d2, mark)) {
      vertices.push_back(d2);
      FMap->markOrbit(d2, ORBIT_VERTEX, mark);
    }

    d1 = a2(a1(d1));
  }
  while (d1 != AVertex1);

  // Ajout du second sommet
  if (!FMap->isMarked(AVertex2, mark)) {
    vertices.push_back(AVertex2);
    FMap->markOrbit(AVertex2, ORBIT_VERTEX, mark);
  }
  
  // Ajout des sommets déjà reliés au second sommet
  d1 = AVertex2;
  do {
    d2 = b1(d1);

    if (d2 != NULL && !FMap->isMarked(d2, mark)) {
      vertices.push_back(d2);
      FMap->markOrbit(d2, ORBIT_VERTEX, mark);
    }

    d1 = a2(a1(d1));
  }
  while (d1 != AVertex2);

  // Démarquage des sommets
  for (it = vertices.begin() ; it != vertices.end() ; it++)
    FMap->unmarkOrbit(*it, ORBIT_VERTEX, mark);
  FMap->freeMark(mark);

  list<CDart*> * l = FTools.sortMultipleVerticesEdges(AVertex1, vertices,
						      FVertexDI);

  if (l == NULL) {
    cout << "\033[1;33m"
	 << "Erreur lors du tri angulaire => fusion des sommets impossible !"
	 << "\033[0m\n";

    return;
  }

  it = l->begin();
  d2 = *it;

  do {
    d1 = d2;
    it++;
    
    if (it != l->end())
      d2 = *it;
    else
      d2 = *l->begin();

    if (a1(d1) != a2(d2)) {
      FMap->setDirectInfo(d1, FAlpha1DI, a2(d2));
      FMap->setDirectInfo(a2(d2), FAlpha1DI, d1);
    }
    else {
      FMap->setDirectInfo(d1, FAlpha1DI, NULL);
      FMap->setDirectInfo(a2(d2), FAlpha1DI, NULL);
    }
  }
  while (it != l->end());

  delete l;

  FNumberOfIntersections++;
}

//******************************************************************************

// void CCorefine2dPropagation::applyModifications(CDart * AMesh)
// {
//   CDart *d1, *d2;
//   int vertex_mark = FMap->getNewMark();
//   CAttributeVertex *att;
//   CVertex pt;

//   StaticCoverageCC scc(FMap, AMesh);

//   for (scc.reinit() ; scc.cont() ; scc++) {
//     if (!FMap->isMarked(*scc, vertex_mark)) {
//       att = FMap->removeVertex(*scc);
//       pt = *((CVertex*)att);
//       delete att;

//       for (StaticCoverageVertex scv(FMap, *scc) ; scv.cont() ; scv++) {
// 	d1 = *scv;
// 	d2 = b1(d1);
	  
// 	if (d2 != NULL) {
// 	  FMap->uu1(d1);
// 	  FMap->uu1(d2);
// 	  FMap->ll1(d1, d2);
// 	}

// 	FMap->setMark(d1, vertex_mark);
//       }

//       modifyVertex(*scc, pt);
//     }
//   }

//   for (scc.reinit() ; scc.cont() ; scc++)
//     FMap->unsetMark(*scc, vertex_mark);
  
//   FMap->freeMark(vertex_mark);
// }

//******************************************************************************

void CCorefine2dPropagation::applyModifications(CDart * AMesh)
{
  ENTER;

  CDart *d1, *d2;

  for (CStaticCoverageCC scc(FMap, AMesh) ; scc.cont() ; scc++) {
    d1 = *scc;
    d2 = b1(d1);

    if (d2 != NULL) {
      if (!FMap->isFree1(d1))
	FMap->dartUnsew1(d1);

      if (!FMap->isFree1(d2))
	FMap->dartUnsew1(d2);

      FMap->dartSew1(d1, d2);
    }
  }

  EXIT;
}

//******************************************************************************

#undef a0
#undef a1
#undef a2
#undef b1

//******************************************************************************
