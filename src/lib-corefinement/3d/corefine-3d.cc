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

#include <cmath>
#include <sstream>
#include "corefine-3d.hh"
#include "time.hh"

#include INCLUDE_NON_INLINE("corefine-3d.icc")

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

// Résolutions miminale et maximale de la grille selon une direction
#define MIN_GRID_RES 2
#define MAX_GRID_RES 512

// La grille est de taille fixe si l'option est activée
// #define FIXED_GRID_RES 32

// Les sous-grilles sont de tailles fixes si l'option est activée
// #define FIXED_SUB_GRID_RES 8

// Version du co-raffinement (optionnel)
#define COREFINEMENT_VERSION "Co-raffinement v11"

//******************************************************************************

/* Permet d'extraire les lignes de coupes.
 */
// #define EXTRACT_LINES

/* Permet de sauver les points d'intersection dans un fichier nommé
 * intersections.dat
 */
// #define SAVE_INTERSECTION_POINTS

/* Permet de sauvegarder la progression des arêtes d'intersection dans des
 * fichiers portant comme nom le numéro de l'arête.
 */
// #define FOLLOW_DEBUG_MODE

/* Permet d'afficher dans la console le détail des calculs et la progression
 * de l'algorithme. Très utile lorsque ça plante pour connaître l'endroit exact
 * de l'erreur.
 */
// #define DEBUG_MESSAGES
#define WARNING_MESSAGES
#include "message-macros.hh"

//******************************************************************************

#ifdef SAVE_INTERSECTION_POINTS

FILE * point_file = NULL;

#define SAVE_POINT(p) (fprintf(point_file ? point_file : stdout, \
			       "%.6f # %.6f # %.6f\n", \
			       (p).getX()*10, (p).getY()*10, (p).getZ()*10))

#else  // SAVE_INTERSECTION_POINTS

#define SAVE_POINT(p)

#endif // SAVE_INTERSECTION_POINTS

//******************************************************************************

CCorefine3d::CCorefine3d(CGMapVertex * AMap, bool ACalculateOrientation,
			 TCoordinate AEpsilon, int AVertexDI)
  : CCorefine(AMap, AEpsilon), FCalculateOrientation(ACalculateOrientation),
    FComputeOnlyFirstIntersection(false), FGridResolution(70),
    FTools(AMap, AEpsilon)
{
  ENTER;

  if (FEps > 0)
    CBoundingBox::setEpsilon(2*FEps);

  if (AVertexDI < 0) {
    FVertexDI = FMap->getNewDirectInfo();
    FLocalVertexDirectInfo = true;
  }
  else {
    FVertexDI = AVertexDI;
    FLocalVertexDirectInfo = false;
  }

  EXIT;
}

//******************************************************************************

CCorefine3d::~CCorefine3d()
{
  ENTER;

  if (FLocalVertexDirectInfo)
    FMap->freeDirectInfo(FVertexDI);

  EXIT;
}

//******************************************************************************

void CCorefine3d::computeOnlyFirstIntersection(bool ABoolean)
{
  FComputeOnlyFirstIntersection = ABoolean;
}

//******************************************************************************

void CCorefine3d::setGridResolution(int ARes)
{
  FGridResolution = ARes;
}

//******************************************************************************

int CCorefine3d::corefine(CDart *& AMesh1, CDart *& AMesh2,
			  bitset<NB_MARKS> ACopyMarks)
{
  ENTER;

  list<CDart*> vertex_list;
  CEdgeIntersection inter;
  unsigned long nb_faces;
  CDart *vertex=NULL, *edge=NULL, *tmp_edge=NULL, *tmp_face=NULL;
  int edge_mark = FMap->getNewMark();
  int vertex_mark = FMap->getNewMark();
  CVertex *pt1, *pt2, tmp_pt;
  CPlane plane;
  CTime start, end;

  FFaceDI = FMap->getNewDirectInfo();
  FAlpha2DI = FMap->getNewDirectInfo();

  FOrientMark = FMap->getNewMark();
  FFictiveMark = FMap->getNewMark();
  FIntersectionMark = FMap->getNewMark();

  FCopyMarks = ACopyMarks;

  FNumberOfIntersectionLines = 0;
  FNumberOfIntersectionEdges = 0;

//   if (!FTools.isMeshOk(AMesh1) || !FTools.isMeshOk(AMesh2)) {
//     cout << "\033[1;31m"
// 	 << "Co-raffinement impossible !\n"
// 	 << "Les maillages contiennent des arêtes de longueur nulle."
// 	 << "\033[0m\n";

//     FMap->freeMark(edge_mark);
//     FMap->freeMark(vertex_mark);
//     return;
//   }

#ifdef COREFINEMENT_VERSION
  cout << COREFINEMENT_VERSION << endl;
#endif

#ifdef SAVE_INTERSECTION_POINTS
  point_file = fopen("intersections.dat", "w");
#endif

  cout << "Initialisation des maillages" << endl;
  start.updateTime();

  nb_faces = initMesh(AMesh1);
  cout << "Nombre de faces sur le premier maillage : " << nb_faces << endl;

  nb_faces = initMesh(AMesh2);
  cout << "Nombre de faces sur le second maillage : " << nb_faces << endl;

  end.updateTime();
  FInitialisationTime = end - start;
  cout << "Durée de l'initialisation : " << FInitialisationTime << endl;

  cout << "Orientation des maillages" << endl;
  start.updateTime();

  // Recherche d'un brin correctement orienté sur chacun des maillages
  if (FCalculateOrientation) {
    AMesh1 = FTools.findWellOrientedDart(AMesh1, FVertexDI);
    AMesh2 = FTools.findWellOrientedDart(AMesh2, FVertexDI);
  }
  
  // Orientation des maillages
  FMap->halfMarkOrbit(AMesh1, ORBIT_CC, FOrientMark);
  FMap->halfMarkOrbit(AMesh2, ORBIT_CC, FOrientMark);
  
  end.updateTime();
  cout << "Durée de l'orientation : " << end - start << endl;
  
  cout << "Création de la grille régulière pour l'optimisation" << endl;
  start.updateTime();

  createGrid(AMesh2, nb_faces);

//   int depth = 0;
//   while (depth < 5 && refineGrid(MAX_GRID_RES / 16, FNumberOfFacesPerCell) > 0)
//     depth++;
//   cout << "Profondeur de la grille : " << depth << endl;
  
  end.updateTime();
  FGridCreationTime = end - start;
  cout << "Durée de la création : " << FGridCreationTime << endl;

  FLineCreationTime.setTime(0);
  
  cout << "Traitement de la première intersection" << endl;
  start.updateTime();

  // Gestion de la première intersection :
  int tmp_dim;
  tmp_pt = *getVertex(AMesh1);
  tmp_face = findFirstIntersectionInGrid(tmp_pt, &tmp_dim);

  if (tmp_face != NULL) {
    switch (tmp_dim) {
    case 0:
      *getVertex(AMesh1) = *getVertex(tmp_face);
      break;
      
    case 1:
      tmp_face = splitEdge(tmp_face, tmp_pt);
      break;
      
    case 2:
      tmp_face = insertVertexInFace(tmp_face, tmp_pt);
      break;
    }
    
    followIntersection(AMesh1, tmp_face, vertex_mark);
  }

  cout << "Parcours des maillages et création des intersections" << endl;

  // Ajoute le sommet courant dans la liste
  vertex_list.push_back(AMesh1);

  // Tant qu'il y a des éléments dans la liste, on continue le parcours
  while(!vertex_list.empty()) {

    // Récupération du prochain sommet à traiter
    vertex = vertex_list.front(), vertex_list.pop_front();

    // Parcours des arêtes incidentes au sommet
    for (CStaticCoverageVertex dcv(FMap, vertex); dcv.cont(); ++dcv) {
      edge = *dcv;

      if (!FMap->isMarked(edge, edge_mark) &&
	  FMap->isMarked(edge, FOrientMark)) {
	pt1 = getVertex(edge);
	pt2 = getVertex(a0(edge));
	
	MSG("Arête parcourue = [" << *pt1 << "," << *pt2 << "]");

	if (!FMap->isMarked(edge, FFictiveMark))
	  inter = findNearestIntersectionInGrid(*pt1, *pt2);
	else
	  inter.setCell(NULL);

	if (inter.getCell() != NULL && // FNumberOfIntersectionLines == 0 &&
// 	    (inter.getPosition() != EP_OnSecondVertex ||
	    ((*pt2 - inter.getPoint()).norm() > 2.0 * FEps ||
	     !FMap->isMarked(a0(edge), vertex_mark))) {

	  assert(inter.getPosition() != EP_OnFirstVertex);
	  
// 	  cout << "Une intersection trouvée sur l'arête ["
// 	       << *pt1 << "," << *pt2 << "] de longueur "
// 	       << (*pt2 - *pt1).norm() << " avec la face :" << endl;
// 	  FTools.displayFaceVertices(inter.getCell(), FVertexDI);
// 	  cout << "Propriétés de l'intersection :" << endl << inter;
	    
	  switch (inter.getPosition()) {
	  case EP_OnFirstVertex:
 	    tmp_edge = edge;
	    *pt1 = inter.getPoint();
	    break;

	  case EP_OnSecondVertex:
	    tmp_edge = a3(a0(edge));
	    *pt2 = inter.getPoint();
	    break;

	  case EP_OnEdge:
	    tmp_edge = splitEdge(edge, inter.getPoint());
	    break;

	  default:
	    break;
	  }

	  switch (inter.getCellDimension()) {
	  case 0:
	    tmp_face = inter.getCell();
	    *getVertex(inter.getCell()) = inter.getPoint();
	    break;

	  case 1:
	    tmp_face = splitEdge(inter.getCell(), inter.getPoint());
	    break;

	  case 2:
	    tmp_face = insertVertexInFace(inter.getCell(), inter.getPoint());
	    break;

	  default:
	    tmp_face = NULL;
	    break;
	  }

	  followIntersection(tmp_edge, tmp_face, vertex_mark);
	}
	else {
	  tmp_edge = a3(a0(edge));
	}

	if (!FMap->isMarked(tmp_edge, vertex_mark) ||
	    !FComputeOnlyFirstIntersection)
	  vertex_list.push_back(tmp_edge);

	FMap->markOrbit(edge, ORBIT_EDGE, edge_mark);
      }
    }
  }

  FMap->unmarkOrbit(AMesh1, ORBIT_CC, edge_mark);
  FMap->unmarkOrbit(AMesh1, ORBIT_CC, vertex_mark);

  if (FNumberOfIntersectionEdges == 0)
    FMap->unmarkOrbit(AMesh2, ORBIT_CC, vertex_mark);

  FMap->freeMark(edge_mark);
  FMap->freeMark(vertex_mark);

  end.updateTime();
  FResearchTime = end - start;
  cout << "Durée de la recherche des intersections : "
       << FResearchTime << endl;

  FResearchTime -= FLineCreationTime;

  cout << "Assemblage des maillages" << endl;
  start.updateTime();

  applyModifications(AMesh1);

  end.updateTime();
  FUpdateTime = end - start;
  cout << "Durée de l'assemblage : " << FUpdateTime << endl;

#ifdef EXTRACT_LINES
  cout << "Extraction des lignes de coupes" << endl;
  extractIntersectionLines(AMesh1);
  FMap->unmarkOrbit(AMesh1, ORBIT_CC, FIntersectionMark);
#endif // EXTRACT_LINES

  cout << "Destruction de la grille" << endl;
  start.updateTime();

  destroyGrid();

  end.updateTime();
  cout << "Durée de la destruction : " << end - start << endl;

  cout << "Nettoyage des maillages" << endl;
  start.updateTime();

  cleanMesh(AMesh1);

  if (FNumberOfIntersectionEdges == 0)
    cleanMesh(AMesh2);

  end.updateTime();
  cout << "Durée du nettoyage : " << end - start << endl;

  cout << "Il y a eu " << FNumberOfIntersectionLines << " ligne(s) de coupe et "
       << FNumberOfIntersectionEdges << " arête(s) d'intersection" << endl;

#ifdef SAVE_INTERSECTION_POINTS
  if (point_file != NULL) {
    fclose(point_file);
    point_file = NULL;
  }
#endif

  FMap->freeDirectInfo(FFaceDI);
  FMap->freeDirectInfo(FAlpha2DI);

  FMap->freeMark(FOrientMark);
  FMap->freeMark(FFictiveMark);
  FMap->freeMark(FIntersectionMark);
  
  EXIT;

  return FNumberOfIntersectionLines;
}

//******************************************************************************

unsigned long CCorefine3d::initMesh(CDart * AMesh)
{
  ENTER;

  int mark = FMap->getNewMark();
  unsigned long int nb_faces = 0;

  MSG("mark = " << mark);

  // Fermeture des 3-bords
  for (CStaticCoverageCC scc(FMap, AMesh); scc.cont(); ++scc) {
    if (FMap->isFree1(*scc))
      cerr << "<initMesh> Le brin " << *scc
	   << " n'est pas lié par alpha1 !!!" << endl;

    if (FMap->isFree3(*scc))
      FMap->stopUp(*scc, 3);
  }

  // Fermeture des 2-bords
//   for (CStaticCoverageCC scc(FMap, AMesh); scc.cont(); ++scc) {
//     if (FMap->isFree2(*scc) && FMap->canSew2(*scc, a3(*scc)))
//       FMap->sew2(*scc, a3(*scc));
//   }

  CDynamicCoverageCC dcc(FMap, AMesh);

//   if (FLocalVertexDirectInfo) {
    // Mise à jour des pointeurs vers les plongements sommets
    for (dcc.reinit(); dcc.cont(); ++dcc) {
      if (!FMap->isMarked(*dcc, mark)) {
	FMap->markOrbit(*dcc, ORBIT_VERTEX, mark);
	updateVertexLinks(*dcc);
      }
    }
//   }
//   else {
//     for (dcc.reinit(); dcc.cont(); ++dcc)
//       FMap->setMark(*dcc, mark);
//   }

  // Mise à jour des pointeurs vers un brin des faces
  for (dcc.reinit(); dcc.cont(); ++dcc) {
      FMap->setDirectInfo(*dcc, FAlpha2DI, NULL);
      
    if (FMap->isMarked(*dcc, mark)) {
      nb_faces++;
      FMap->unmarkOrbit(*dcc, ORBIT_FACE, mark);
      updateFaceLinks(*dcc);
    }
  }

  FMap->freeMark(mark);

  EXIT;

  return nb_faces;
}

//******************************************************************************

void CCorefine3d::cleanMesh(CDart * /*AMesh*/)
{
  ENTER;

  int delete_mark = FMap->getNewMark();

  MSG("delete_mark = " << delete_mark);

  for (CDynamicCoverageAll dca(FMap); dca.cont(); ++dca) {
    if (FMap->isMarked(*dca, FOrientMark)) {
      FMap->unmarkOrbit(*dca, ORBIT_EDGE, FOrientMark);
      
      if (FMap->isMarked(*dca, FFictiveMark)) {
	if (getFace(*dca) != getFace(a2(*dca))) {
	  removeEdge(*dca, delete_mark);
	}
	else
	  FMap->unmarkOrbit(*dca, ORBIT_EDGE, FFictiveMark);
      }
    }
  }
  FMap->deleteMarkedDarts(delete_mark);
  FMap->freeMark(delete_mark);

  EXIT;
}

//******************************************************************************

// void CCorefine3d::createGrid(CDart * AMesh, int AMaxDiv)
// {
//   ENTER;

//   int face_mark = FMap->getNewMark();
//   CBoundingBox bb = FTools.orbitBoundingBox(AMesh, ORBIT_CC, FVertexDI);
//   CVertex size;
//   TCoordinate max;
//   int size_i, size_j, size_k;

//   // Calcul de la résolution de la grille
//   size = bb.getEpsMaxBound() - bb.getEpsMinBound();
//   max = (size.getX() > size.getY() ?
// 	 (size.getX() > size.getZ() ? size.getX() : size.getZ()) :
// 	 (size.getY() > size.getZ() ? size.getY() : size.getZ()));
//   size /= max;
//   size_i = (int)(size.getX() * AMaxDiv);
//   size_j = (int)(size.getY() * AMaxDiv);
//   size_k = (int)(size.getZ() * AMaxDiv);
//   if (size_i == 0) size_i = 1;
//   if (size_j == 0) size_j = 1;
//   if (size_k == 0) size_k = 1;

//   cout << "Résolution de la grille = "
//        << size_i << "x" << size_j << "x" << size_k << endl;

//   FGrid = new CGrid3d<list<CDart*>*>(size_i, size_j, size_k, bb);
//   for (CGrid3dIterator<list<CDart*>*> gi(*FGrid) ; gi.cont() ; ++gi)
//     FGrid->setCell(gi, new list<CDart*>);

//   for (CDynamicCoverageCC dcc(FMap, AMesh); dcc.cont(); ++dcc) {
//     if (!FMap->isMarked(*dcc, face_mark) &&
// 	FMap->isMarked(*dcc, FOrientMark)) {
//       FMap->markOrbit(*dcc, ORBIT_FACE, face_mark);
//       addFaceToGrid(getFace(*dcc));
//     }
//   }

//   FMap->unmarkOrbit(AMesh, ORBIT_CC, face_mark);
//   FMap->freeMark(face_mark);

// //   for (CGrid3dIterator<list<CDart*>*> gi(*FGrid) ; gi.cont() ; ++gi)
// //     cout << gi << " : " << (*gi)->size() << endl;

//   EXIT;
// }

//******************************************************************************

// void CCorefine3d::destroyGrid()
// {
//   ENTER;

//   for (CGrid3dIterator<list<CDart*>*> gi(*FGrid) ; gi.cont() ; ++gi)
//     delete FGrid->getCell(gi);
//   delete FGrid;

//   EXIT;
// }

//******************************************************************************

void CCorefine3d::createGrid(CDart * AMesh, unsigned long ANbFaces)
{
  ENTER;

  int face_mark = FMap->getNewMark();
  CBoundingBox bb = FTools.orbitBoundingBox(AMesh, ORBIT_CC, FVertexDI);
  CVertex size = bb.getEpsMaxBound() - bb.getEpsMinBound();
  unsigned int size_i, size_j, size_k;

  MSG("face_mark = " << face_mark);

  // Calcul de la résolution de la grille
  if (FGridResolution > 0)
    getGridResolution(normalizeGridSize(size), 1, FGridResolution,
		      &size_i, &size_j, &size_k);
  else
    computeGridResolution(normalizeGridSize(size),
			  ANbFaces, 0.05,
			  &size_i, &size_j, &size_k,
			  MIN_GRID_RES, MAX_GRID_RES);

  cout << "Résolution de la grille = "
       << size_i << "x" << size_j << "x" << size_k << endl;

  FGrid = new TFaceGrid(size_i, size_j, size_k, bb);
  for (TFaceGridIter gi(*FGrid); gi.cont(); ++gi)
    FGrid->setCell(gi, new list<CDart*>);

  for (CDynamicCoverageCC dcc(FMap, AMesh); dcc.cont(); ++dcc) {
    if (!FMap->isMarked(*dcc, face_mark) &&
	FMap->isMarked(*dcc, FOrientMark)) {
      FMap->markOrbit(*dcc, ORBIT_FACE, face_mark);
      addFaceToGrid(FGrid, *dcc);
    }
  }

  FMap->unmarkOrbit(AMesh, ORBIT_CC, face_mark);
  FMap->freeMark(face_mark);

//   for (CGrid3dIterator<list<CDart*>*> gi(*FGrid); gi.cont(); ++gi)
//     cout << gi << " : " << (*gi)->size() << endl;

  EXIT;
}

//******************************************************************************

void CCorefine3d::destroyGrid()
{
  ENTER;

  for (TFaceGridIter gi(*FGrid); gi.cont(); ++gi)
    delete *gi;
  delete FGrid;

  EXIT;
}

//******************************************************************************

// void CCorefine3d::addFaceToGrid(CDart * AFace)
// {
//   ENTER;

//   CBoundingBox bb = FTools.orbitBoundingBox(AFace, ORBIT_01, FVertexDI);

// //   int x, y, z;
// //   FGrid->getCellPosition(bb->getEpsMinBound(), &x, &y, &z);
// //   cout << "min = (" << x << "," << y << "," << z << ")" << endl;
// //   FGrid->getCellPosition(bb->getEpsMaxBound(), &x, &y, &z);
// //   cout << "max = (" << x << "," << y << "," << z << ")" << endl;

//   for (CGrid3dIterator<list<CDart*>*> gi(*FGrid, &bb); gi.cont(); ++gi) {
//     (*gi)->push_back(AFace);
//   }

//   EXIT;
// }

//******************************************************************************

// void CCorefine3d::removeFaceFromGrid(CDart * AFace)
// {
//   ENTER;

//   CBoundingBox bb = FTools.orbitBoundingBox(AFace, ORBIT_01, FVertexDI);
//   list<CDart*> *dart_list;
//   list<CDart*>::iterator li, old_li;

//   for (CGrid3dIterator<list<CDart*>*> gi(*FGrid, &bb); gi.cont(); ++gi) {
//     dart_list = *gi;

//     for (li = dart_list->begin() ; li != dart_list->end() ; ) {
//       old_li = li;
//       ++li;
//       if (*old_li == AFace)
// 	dart_list->erase(old_li);
//     }
//   }

//   EXIT;
// }

//******************************************************************************

unsigned int CCorefine3d::getMaxVerticesDegree(list<CDart*> * AList)
{
  ENTER;

  list<CDart*>::iterator it;
  unsigned int max_deg = 0, current_deg;

  for (it = AList->begin(); it != AList->end(); it++) {
    for (CDynamicCoverage01 dcf(FMap, *it); dcf.cont(); dcf++) {
      if (FMap->isMarked(*dcf, FOrientMark)) {
	current_deg = 0;

	for (CDynamicCoverageVertex dcv(FMap, *dcf); dcv.cont(); dcv++)
	  current_deg++;

	if (current_deg > max_deg)
	  max_deg = current_deg;
      }
    }
  }

  EXIT;

  return max_deg / 4;
}

//******************************************************************************

CVertex CCorefine3d::normalizeGridSize(const CVertex & AGridSize)
{
  TCoordinate max;

  max = (AGridSize.getX() > AGridSize.getY() ?
	 (AGridSize.getX() > AGridSize.getZ() ?
	  AGridSize.getX() : AGridSize.getZ()) :
	 (AGridSize.getY() > AGridSize.getZ() ?
	  AGridSize.getY() : AGridSize.getZ()));

  return AGridSize / max;
}

//******************************************************************************

void CCorefine3d::getGridResolution(const CVertex & AGridSize,
				    unsigned int AMinRes,
				    unsigned int AMaxRes,
				    unsigned int * AResX,
				    unsigned int * AResY,
				    unsigned int * AResZ)
{
  assert(AGridSize.getX() >= 0.0 && AGridSize.getX() <= 1.0 &&
	 AGridSize.getY() >= 0.0 && AGridSize.getY() <= 1.0 &&
	 AGridSize.getZ() >= 0.0 && AGridSize.getZ() <= 1.0);

  *AResX = (int)(AGridSize.getX() * AMaxRes + 0.5);
  *AResY = (int)(AGridSize.getY() * AMaxRes + 0.5);
  *AResZ = (int)(AGridSize.getZ() * AMaxRes + 0.5);
  if (*AResX < AMinRes) *AResX = AMinRes;
  if (*AResY < AMinRes) *AResY = AMinRes;
  if (*AResZ < AMinRes) *AResZ = AMinRes;
}

//******************************************************************************

void CCorefine3d::computeGridResolution(const CVertex & AGridSize,
					unsigned long ANbFaces,
					TCoordinate ANbFacesPerCell,
					unsigned int * AResX,
					unsigned int * AResY,
					unsigned int * AResZ,
					unsigned int AMinRes,
					unsigned int /*AMaxRes*/)
{
  assert(AGridSize.getX() >= 0.0 && AGridSize.getX() <= 1.0 &&
	 AGridSize.getY() >= 0.0 && AGridSize.getY() <= 1.0 &&
	 AGridSize.getZ() >= 0.0 && AGridSize.getZ() <= 1.0);

  double res = pow((double)ANbFaces /
		   (double)(ANbFacesPerCell * AGridSize.getX() *
			    AGridSize.getY() * AGridSize.getZ()),
		   1.0/3.0);

  getGridResolution(AGridSize, AMinRes, (int)(res+0.5), AResX, AResY, AResZ);
}

//******************************************************************************

unsigned int CCorefine3d::refineGrid(unsigned int AMaxSubDiv,
				     unsigned int AMaxNumberOfFaces)
{
  ENTER;

  list<CDart*> *l;
  list<CDart*>::iterator it;
  TFaceGrid *sub_grid;
  unsigned int max_deg, size, result = 0;
  unsigned int div_x, div_y, div_z;

  for (TFaceGridIter gi(*FGrid); gi.cont(); ++gi) {
     l = *gi;
     size = l->size();

    if (size > AMaxNumberOfFaces) {
      max_deg = getMaxVerticesDegree(l);

      if (max_deg <= AMaxNumberOfFaces || size > 2 * max_deg) {
	result++;

#ifdef FIXED_SUB_GRID_RES
	div_x = div_y = div_z = FIXED_SUB_GRID_RES;
#else
	computeGridResolution(CVertex(1.0, 1.0, 1.0),
			      size, 0.05,
			      &div_x, &div_y, &div_z,
			      MIN_GRID_RES, AMaxSubDiv);
#endif

// 	cout << "Cellule " << gi << " éclatée en une grille de taille "
// 	     << sub_div << "^3" << endl;
	
	sub_grid = FGrid->splitCellInGrid(gi, div_x, div_y, div_z);
	
	for (TFaceGridIter gi2(*sub_grid); gi2.cont(); ++gi2)
	  sub_grid->setCell(gi2, new list<CDart*>);
	
	for (it = l->begin(); it != l->end(); ++it)
	  addFaceToGrid(sub_grid, *it);
	
	delete l;
      }
    }
  }

  EXIT;

  return result;
}

//******************************************************************************

void CCorefine3d::addFaceToGrid(TFaceGrid * AGrid, CDart * AFace)
{
  ENTER;

  assert(AGrid != NULL);
  assert(AFace != NULL);

  CBoundingBox bb = FTools.orbitBoundingBox(AFace, ORBIT_01, FVertexDI);

  for (TFaceGridIter gi(*AGrid, bb); gi.cont(); ++gi)
    (*gi)->push_back(getFace(AFace));

  EXIT;
}

//******************************************************************************

void CCorefine3d::removeFaceFromGrid(TFaceGrid * AGrid,
				     CDart * AFace)
{
  ENTER;

  AFace = getFace(AFace);

  CBoundingBox bb = FTools.orbitBoundingBox(AFace, ORBIT_01, FVertexDI);
  list<CDart*> *dart_list;
  list<CDart*>::iterator li, old_li;

  for (TFaceGridIter gi(*AGrid, bb); gi.cont(); ++gi) {
    dart_list = *gi;

    for (li = dart_list->begin(); li != dart_list->end(); ) {
      old_li = li;
      ++li;
      if (*old_li == AFace)
	dart_list->erase(old_li);
    }
  }

  EXIT;
}

//******************************************************************************

void CCorefine3d::updateVertexLinks(CDart * ADart, CAttributeVertex * AVertex)
{
  ENTER;

  for (CDynamicCoverageVertex dcv(FMap, ADart); dcv.cont(); ++dcv) {
    FMap->setDirectInfo(*dcv, FVertexDI, AVertex);
  }

  EXIT;
}

//******************************************************************************

void CCorefine3d::updateVertexLinks(CDart * ADart)
{
  ENTER;

  CAttributeVertex * att = FMap->findVertex(ADart);

  for (CDynamicCoverageVertex dcv(FMap, ADart); dcv.cont(); ++dcv) {
    FMap->setDirectInfo(*dcv, FVertexDI, att);
  }

  EXIT;
}

//******************************************************************************

void CCorefine3d::updateFaceLinks(CDart * AFace)
{
  ENTER;

  for (CDynamicCoverageFace dcf(FMap, AFace); dcf.cont(); ++dcf) {
    FMap->setDirectInfo(*dcf, FFaceDI, AFace);
  }

  EXIT;
}

//******************************************************************************

CDart * CCorefine3d::createEdge()
{
  ENTER;

  CDart *d[8];

  for (int i=0; i<8; ++i) {
    d[i] = FMap->addMapDart();
    FMap->setDirectInfo(d[i], FAlpha2DI, NULL);
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

  FMap->halfMarkOrbit(d[0], ORBIT_EDGE, FOrientMark);

//   FMap->setMark(d[0], FOrientMark);
//   FMap->setMark(d[3], FOrientMark);
//   FMap->setMark(d[5], FOrientMark);
//   FMap->setMark(d[6], FOrientMark);

  EXIT;

  return d[0];
}

//******************************************************************************

CDart * CCorefine3d::insertVertexInFace(CDart * AFace, const CVertex & APoint)
{
  ENTER;

  assert(FMap->isMarked(AFace, FOrientMark));

  CDart *vertex = a1(a0(insertEdgeInFace(AFace, APoint)));

  FMap->markOrbit(vertex, ORBIT_EDGE, FFictiveMark);

  EXIT;

  return vertex;
}

//******************************************************************************

CDart * CCorefine3d::insertEdgeInFace(CDart * AVertex1,
				      const CVertex & AVertex2)
{
  ENTER;

  assert(FMap->isMarked(AVertex1, FOrientMark));

  CDart *edge;

//   if (FTools.isPointOnLine(AVertex2, *getVertex(AVertex1),
// 			   *getVertex(a0(AVertex1)))) {
//     edge = AVertex1;
//     splitEdge(edge, AVertex2);
//     if (FMap->isMarked(edge, FFictiveMark))
//       FMap->unmarkOrbit(edge, ORBIT_EDGE, FFictiveMark);
//   }
//   else if (FTools.isPointOnLine(AVertex2, *getVertex(AVertex1),
// 				*getVertex(a0(a1(AVertex1))))) {
//     edge = a3(a1(AVertex1));
//     splitEdge(edge, AVertex2);
//     if (FMap->isMarked(edge, FFictiveMark))
//       FMap->unmarkOrbit(edge, ORBIT_EDGE, FFictiveMark);
//   }
//   else {
    CDart *old_a1, *face = getFace(AVertex1);

    CPlane plane = FTools.facePlane(AVertex1, FVertexDI);

    assert(FTools.isPointOnPlane(AVertex2, plane));

    if (FTools.isVectorNull(plane.getNormal())) {
      cout << "\033[1;34m";
      FTools.displayFaceVertices(AVertex1, FVertexDI);
      cout << "<insertEdgeInFace> La normale à la face est nulle : "
	   << plane.getNormal() << "\033[0m\n";
    }

    // Ici, on s'assure d'insérer l'arête au bon endroit afin de ne pas obtenir
    // une topologie en désaccord avec la géométrie.
    AVertex1 = FTools.findSectorOfVector(AVertex2 - *getVertex(AVertex1),
					 AVertex1, plane, FVertexDI);

    AVertex1 = a1(AVertex1);

    edge = createEdge();

    assert(!FMap->isFree1(AVertex1));

    old_a1 = a1(AVertex1);
    FMap->unsew1(AVertex1);

    FMap->sew1(AVertex1, edge);
    FMap->sew1(old_a1, a2(edge));
    FMap->sew1(a0(edge), a0(a2(edge)));

    updateVertexLinks(AVertex1);

    FMap->setVertex(a0(edge), AVertex2);

    for (CDynamicCoverage23 dc(FMap, edge); dc.cont(); ++dc) {
      FMap->setDirectInfo(*dc, FFaceDI, face);
      FMap->setDirectInfo(a0(*dc), FFaceDI, face);

      for (int m = 0; m < NB_MARKS; m++)
	if (FCopyMarks[m] && FMap->isMarked(a1(*dc), m)) {
	  FMap->setMark(*dc, m);
	  FMap->setMark(a0(*dc), m);
	}
    }

    updateVertexLinks(a0(edge));
//   }
  
    EXIT;

  return edge;
}

//******************************************************************************

CDart * CCorefine3d::splitFace(CDart * AVertex1, CDart * AVertex2)
{
  ENTER;

  assert(AVertex1 != AVertex2);
  assert(FMap->isMarked(AVertex1, FOrientMark));
  assert(FMap->isMarked(AVertex2, FOrientMark));

  CDart *edge = NULL;

  // Gestion du cas ou l'arête est déjà existante
  for (CDynamicCoverage12 dc1(FMap, AVertex1); dc1.cont() && !edge; ++dc1)
    for (CDynamicCoverage12 dc2(FMap, AVertex2); dc2.cont() && !edge; ++dc2)
      if (a0(*dc1) == *dc2) {
	if (FMap->isMarked(*dc1, FOrientMark))
	  edge = *dc1;
	else
	  edge = a3(*dc1);
	
	if (FMap->isMarked(edge, FFictiveMark))
	  FMap->unmarkOrbit(edge, ORBIT_EDGE, FFictiveMark);
      }
  
  // Cas où les deux sommets ne sont pas reliés par une arête
  if (edge == NULL) {
    CVertex v = *getVertex(AVertex2) - *getVertex(AVertex1);
    
    MSG("Création d'une arête entre " << *getVertex(AVertex1)
	<< " et " << *getVertex(AVertex2));
    
    CPlane plane = FTools.facePlane(AVertex1, FVertexDI);
    
    if (FTools.isVectorNull(plane.getNormal())) {
      cout << "\033[1;34m";
      FTools.displayFaceVertices(AVertex1, FVertexDI);
      cout << "<splitFace> La normale à la face est nulle : "
	   << plane.getNormal() << "\033[0m\n";
    }

    AVertex1 = FTools.findSectorOfVector(v, AVertex1, plane, FVertexDI);
    AVertex2 = FTools.findSectorOfVector(-v, AVertex2, plane, FVertexDI);
    
    CDart *old_a1[2], *face = getFace(AVertex1);
    
    edge = createEdge();
    
    // On prend le brin voisin qui est orienté dans le mauvais sens car le
    // premier brin de la nouvelle arête est orienté dans le bon sens
    AVertex1 = a1(AVertex1);
    
    old_a1[0] = a1(AVertex1);
    old_a1[1] = a1(AVertex2);
    
    assert(!FMap->isFree1(AVertex1));
    FMap->unsew1(AVertex1);

    assert(!FMap->isFree1(AVertex2));
    FMap->unsew1(AVertex2);
    
    FMap->sew1(AVertex1, edge);
    FMap->sew1(old_a1[0], a2(edge));
    
    FMap->sew1(AVertex2, a0(edge));
    FMap->sew1(old_a1[1], a2(a0(edge)));
    
    updateVertexLinks(edge);
    updateVertexLinks(a0(edge));
    
    if (!FMap->isSameOrbit(edge, a2(edge), ORBIT_01)) {
      updateFaceLinks(edge);
      updateFaceLinks(a2(edge));
    }
    else {
      for (CDynamicCoverageEdge dce(FMap, edge); dce.cont(); ++dce)
	FMap->setDirectInfo(*dce, FFaceDI, face);
    }

//     cleanFictiveEdges(edge);
//     if (!FMap->is_same_orbit(edge, a2(edge), ORBIT_01))
//       cleanFictiveEdges(a2(edge));
  }

  for (CDynamicCoverageEdge dce(FMap, edge); dce.cont(); ++dce) {
    for (int m = 0; m < NB_MARKS; m++)
      if (FCopyMarks[m] && FMap->isMarked(a1(*dce), m)) {
	FMap->setMark(*dce, m);
      }
  }

  EXIT;
    
  return edge;
}

//******************************************************************************

CDart * CCorefine3d::splitEdge(CDart * AVertex, const CVertex & APoint)
{
  ENTER;

  assert(FMap->isMarked(AVertex, FOrientMark));
//   assert(FTools.isPointOnLine(APoint, *getVertex(AVertex),
// 			      *getVertex(a0(AVertex))));

  MSG("Arête [" << *getVertex(AVertex) << "," << *getVertex(a0(AVertex))
      << "] éclatée en " << APoint);

  CDart *d, *face = getFace(AVertex);

  FMap->CGMapGeneric::insertVertex(AVertex);

  d = a1(a0(AVertex));

  FMap->halfMarkOrbit(d, ORBIT_VERTEX, FOrientMark);

  if (FMap->isMarked(AVertex, FFictiveMark))
    FMap->markOrbit(d, ORBIT_VERTEX, FFictiveMark);

  FMap->setVertex(d, APoint);
  updateVertexLinks(d);

  for (CDynamicCoverageVertex dcv(FMap, d); dcv.cont(); ++dcv) {
    FMap->setDirectInfo(*dcv, FFaceDI, face);
    FMap->setDirectInfo(*dcv, FAlpha2DI, NULL);

    for (int m = 0; m < NB_MARKS; m++)
      if (FCopyMarks[m] && FMap->isMarked(a0(*dcv), m)) {
	FMap->setMark(*dcv, m);
      }
  }

  EXIT;

  return d;
}

//******************************************************************************

CDart * CCorefine3d::removeEdge(CDart * AEdge, int ADeleteMark)
{
  CDart *d1 = a1(AEdge);
  CDart *d2 = a1(a0(AEdge));

  if (ADeleteMark < 0)
    FMap->merge(AEdge, a2(AEdge), 2, true);
  else {
    FMap->markOrbit(AEdge, ORBIT_EDGE, ADeleteMark);
    FMap->merge(AEdge, a2(AEdge), 2, false);
  }

  updateVertexLinks(d1);
  updateVertexLinks(d2);

  return d1;
}

//******************************************************************************

CVertex CCorefine3d::getProjectionOnPlane(CDart * AVertex,
					  const CPlane & APlane)
{
  ENTER;

  CVertex pt, v1, v2;
  TCoordinate t=0;

  pt = *getVertex(AVertex);
  v1 = *getVertex(a0(AVertex)) - pt;
  v2 = *getVertex(a0(a1(AVertex))) - pt;
  
  APlane.getLineIntersection(pt, v1, &t);

  if (t > 0)
    return pt + t * v1;
  
  APlane.getLineIntersection(pt, v2, &t);

  EXIT;

  return pt + t * v2;
}

//******************************************************************************

bool CCorefine3d::isSameEdge_Naive(CDart * AEdge1, CDart * AEdge2)
{
  ENTER;

  assert(FMap->isMarked(AEdge1, FOrientMark));
  assert(FMap->isMarked(AEdge2, FOrientMark));

  bool found = false;

  for (CDynamicCoverage23 dc1(FMap, AEdge1); dc1.cont() && !found; ++dc1)
    for (CDynamicCoverage23 dc2(FMap, AEdge2); dc2.cont() && !found; ++dc2)
      found = (FMap->getDirectInfo(*dc1, FAlpha2DI) == *dc2);

  EXIT;

  return found;
}

//******************************************************************************

bool CCorefine3d::isSameEdge_Optimized(CDart * AEdge1, CDart * AEdge2)
{
  ENTER;

  assert(FMap->isMarked(AEdge1, FOrientMark));
  assert(FMap->isMarked(AEdge2, FOrientMark));

  CDart *d = AEdge1;
  bool same_edge = false;

  do {
    if (FMap->getDirectInfo(d, FAlpha2DI) != NULL)
      d = a3((CDart*)FMap->getDirectInfo(d, FAlpha2DI));
    else
      d = a3(FTools.alpha2(d));

    same_edge = (d == AEdge2) || (d == a3(a0(AEdge2)));
  }
  while (d != AEdge1 && !same_edge);

  EXIT;

  return same_edge;
}

//******************************************************************************
#define isSameEdge isSameEdge_Optimized

bool CCorefine3d::areFacesLinked(CDart * AFace1, CDart * AFace2)
{
  assert(FMap->isMarked(AFace1, FOrientMark));
  assert(FMap->isMarked(AFace2, FOrientMark));

  return (isSameEdge(AFace1, AFace2) ||
	  isSameEdge(AFace1, a3(a1(AFace2))) ||
	  isSameEdge(a3(a1(AFace1)), AFace2) ||
	  isSameEdge(a3(a1(AFace1)), a3(a1(AFace2))));
}

//******************************************************************************

CEdgeIntersection
CCorefine3d::findNearestIntersectionInGrid(const CVertex & AVertex1,
					   const CVertex & AVertex2)
{
  ENTER;

  CEdgeIntersection inter;
  TPositionOnEdge pos;
  TCoordinate t1 = 0.0, t2 = 0.0;
  CVertex *pt1, *pt2, pt;
  CPlane plane;
  CBoundingBox line_bb(AVertex1, AVertex2);
  CDart *face;
  int mark = FMap->getNewMark();
  int edge_mark = FMap->getNewMark();
  int face_mark = FMap->getNewMark();

  MSG("mark = " << mark);
  MSG("edge_mark = " << edge_mark);
  MSG("face_mark = " << face_mark);

  TFaceGridIter gi(*FGrid, line_bb);
  list<CDart*>::iterator li;

  inter.setCell(NULL);
  inter.setPosition(EP_OutOfEdge);
  inter.setParameter(1.0);

  MSG("Recherche d'une éventuelle intersection avec les sommets");

  for (gi.reinit(); gi.cont(); ++gi) {
    for (li = (*gi)->begin(); li != (*gi)->end(); ++li) {
      for (CDynamicCoverageFace dcf(FMap, *li); dcf.cont(); ++dcf) {
	if (!FMap->isMarked(*dcf, mark) && FMap->isMarked(*dcf, FOrientMark)) {
	  // Marquage du sommet comme traité
	  FMap->markOrbit(*dcf, ORBIT_VERTEX, mark);
	  
	  pt = *getVertex(*dcf);
	  
	  if (line_bb.isInBox(pt)) {
	    pos = FTools.localizePointOnEdge(pt, AVertex1, AVertex2, &t2);
	    
	    if ((pos == EP_OnEdge && t2 < inter.getParameter()) ||
		(pos == EP_OnSecondVertex && t2 <= inter.getParameter())) {
	      MSG("Une intersection trouvée avec le sommet " << pt);
	      
	      inter.setCell(*dcf);
	      inter.setCellDimension(0);
	      inter.setPosition(pos);
	      inter.setPoint(pt);
	      inter.setParameter(t2);
	    }
	    
	    if (pos != EP_OutOfEdge) {
	      for (CDynamicCoverageVertex dcv(FMap, *dcf); dcv.cont(); ++dcv) {
		// Marquage des arêtes incidentes pour ne pas les tester
		if (!FMap->isMarked(*dcv, edge_mark))
		  FMap->markOrbit(*dcv, ORBIT_EDGE, edge_mark);
		
		// Marquage des faces incidentes pour ne pas les tester
		if (!FMap->isMarked(*dcv, face_mark))
		  FMap->markOrbit(*dcv, ORBIT_FACE, face_mark);
	      }
	    }
	  }
	}
      }
    }
  }

  MSG("Recherche d'une éventuelle intersection avec les arêtes");

  for (gi.reinit(); gi.cont(); ++gi) {
    for (li = (*gi)->begin(); li != (*gi)->end(); ++li) {
      for (CDynamicCoverageFace dcf(FMap, *li); dcf.cont(); ++dcf) {
	if (FMap->isMarked(*dcf, edge_mark)) {
	  FMap->unmarkOrbit(*dcf, ORBIT_EDGE, edge_mark);
	  FMap->unmarkOrbit(*dcf, ORBIT_EDGE, mark);
	}
	else if (FMap->isMarked(*dcf, mark) &&
// 	       !FMap->isMarked(*dcf, FFictiveMark) &&
		 FMap->isMarked(*dcf, FOrientMark)) {
	  FMap->unmarkOrbit(*dcf, ORBIT_EDGE, mark);
	  
	  pt1 = getVertex(*dcf);
	  pt2 = getVertex(a0(*dcf));
	  
	  if (CBoundingBox(*pt1, *pt2) * line_bb) {
	    pos = FTools.localizeEdgesIntersection(*pt1, *pt2,
						   AVertex1, AVertex2,
						   &t1, &t2);
	
	    pt = *pt1 + t1 * (*pt2 - *pt1);
	    
	    if (/*FGrid->getCellBoundingBox(gi.getI(),
		  gi.getJ(),
		  gi.getK()).isInBox(pt) &&*/
		(t1 >= 0.0 && t1 <= 1.0)) {
	      if ((pos == EP_OnEdge && t2 < inter.getParameter()) ||
		  (pos == EP_OnSecondVertex && t2 <= inter.getParameter())) {
		MSG("Une intersection trouvée avec l'arête "
		    << "[" << *pt1 << "," << *pt2 << "]");
		
		inter.setCell(*dcf);
		inter.setCellDimension(1);
		inter.setPosition(pos);
		inter.setPoint(pt);
		inter.setParameter(t2);
	      }
	    
	      if (pos != EP_OutOfEdge) {
		for (CDynamicCoverageEdge dce(FMap, *dcf); dce.cont(); ++dce) {
		  // Marquage des faces incidentes pour ne pas les tester
		  if (!FMap->isMarked(*dce, face_mark))
		    FMap->markOrbit(*dce, ORBIT_FACE, face_mark);
		}
	      }
	    }
	  }
	}
      }
    }
  }
  
  MSG("Recherche d'une éventuelle intersection avec les faces");

  for (gi.reinit(); gi.cont(); ++gi) {
    for (li = (*gi)->begin(); li != (*gi)->end(); ++li) {
      if (FMap->isMarked(*li, face_mark)) {
	FMap->unmarkOrbit(*li, ORBIT_FACE, face_mark);
	FMap->markOrbit(*li, ORBIT_FACE, mark);
      }
      else if (!FMap->isMarked(*li, mark)) {
	face = (FMap->isMarked(*li, FOrientMark)) ? *li : a3(*li);
	FMap->markOrbit(face, ORBIT_FACE, mark);
	
// 	if (FTools.orbitBoundingBox(face, ORBIT_01, FVertexDI) * line_bb) {
	  plane = FTools.facePlane(face, FVertexDI);
	  
	  pos = FTools.localizeEdgeAndPlaneIntersection(AVertex1, AVertex2,
							plane, &t2);
	  
	  pt = AVertex1 + t2 * (AVertex2 - AVertex1);
	  
	  if (/*FGrid->getCellBoundingBox(gi.getI(),
		gi.getJ(),
		gi.getK()).isInBox(pt) &&*/
	      FTools.isPointInFace(pt, face, &plane, FVertexDI) &&
	      ((pos == EP_OnEdge && t2 < inter.getParameter()) ||
	       (pos == EP_OnSecondVertex && t2 <= inter.getParameter()))) {
	    MSG("Une intersection avec une face trouvée");
	    
	    inter.setCell(face);
	    inter.setCellDimension(2);
	    inter.setPosition(pos);
	    inter.setPoint(pt);
	    inter.setParameter(t2);
	  }
// 	}
      }
    }
  }

  MSG("Démarquage des faces");

  for (gi.reinit(); gi.cont(); ++gi)
    for (li = (*gi)->begin(); li != (*gi)->end(); ++li)
      for (CDynamicCoverageFace dcf(FMap, *li); dcf.cont(); ++dcf)
	if (FMap->isMarked(*dcf, mark))
	  FMap->unmarkOrbit(*dcf, ORBIT_VERTEX, mark);
  
  FMap->freeMark(mark);
  FMap->freeMark(edge_mark);
  FMap->freeMark(face_mark);

  EXIT;

  return inter;
}

//******************************************************************************

CDart *
CCorefine3d::findFirstIntersectionInGrid(const CVertex & AVertex, int * ADim)
{
  ENTER;

  CVertex pt1, pt2, pt;
  CDart *face, *result = NULL;
  CPlane plane;
  TCoordinate t;
  int mark = FMap->getNewMark();
  
  MSG("mark = " << mark);

  TFaceGridIter gi(*FGrid, CBoundingBox(AVertex));
  list<CDart*>::iterator li;

  // Recherche d'une éventuelle intersection avec les sommets
  for (gi.reinit(); gi.cont() && !result; ++gi)
    for (li = (*gi)->begin(); li != (*gi)->end() && !result; ++li)
      for (CDynamicCoverageFace dcf(FMap, *li); dcf.cont() && !result; ++dcf)
	if (!FMap->isMarked(*dcf, mark) && FMap->isMarked(*dcf, FOrientMark)) {
	  // Marquage du sommet comme traité
	  FMap->markOrbit(*dcf, ORBIT_VERTEX, mark);
	  
	  pt = *getVertex(*dcf);
	  
	  if (FTools.arePointsEqual(AVertex, pt)) {
	    MSG("Une intersection trouvée avec le sommet " << pt);
	    result = *dcf;
	    *ADim = 0;
	  }
	}

  // Recherche d'une éventuelle intersection avec les arêtes
  for (gi.reinit(); gi.cont() && !result; ++gi)
    for (li = (*gi)->begin(); li != (*gi)->end() && !result; ++li)
      for (CDynamicCoverageFace dcf(FMap, *li); dcf.cont() && !result; ++dcf)
	if (FMap->isMarked(*dcf, mark) && FMap->isMarked(*dcf, FOrientMark)) {
	  FMap->unmarkOrbit(*dcf, ORBIT_EDGE, mark);
	  
	  pt1 = *getVertex(*dcf);
	  pt2 = *getVertex(a0(*dcf));
	  
	  if (FTools.isPointOnLine(AVertex, pt1, pt2)) {
	    t = FTools.pointParameterOnLine(AVertex, pt1, pt2);
	    
	    if (t >= 0.0 && t <= 1.0) {
	      MSG("Une intersection trouvée avec l'arête "
		  << "[" << pt1 << "," << pt2 << "]");
	      result = *dcf;
	      *ADim = 1;
	    }
	  }
	}
  
  // Recherche d'une éventuelle intersection avec les faces
  for (gi.reinit(); gi.cont() && !result; ++gi)
    for (li = (*gi)->begin(); li != (*gi)->end() && !result; ++li)
      if (!FMap->isMarked(*li, mark)) {
	face = (FMap->isMarked(*li, FOrientMark)) ? *li : a3(*li);
	FMap->markOrbit(face, ORBIT_FACE, mark);
	
	plane = FTools.facePlane(face, FVertexDI);

	if (FTools.isPointOnPlane(AVertex, plane) &&
	    FTools.isPointInFace(AVertex, face, &plane, FVertexDI)) {
	  MSG("Une intersection avec une face trouvée");
	  result = face;
	  *ADim = 2;
	}
      }

  for (gi.reinit(); gi.cont(); ++gi)
    for (li = (*gi)->begin(); li != (*gi)->end(); ++li)
      for (CDynamicCoverageFace dcf(FMap, *li); dcf.cont(); ++dcf)
	if (FMap->isMarked(*dcf, mark))
	  FMap->unmarkOrbit(*dcf, ORBIT_VERTEX, mark);
  
  FMap->freeMark(mark);

  EXIT;

  return result;
}

//******************************************************************************

void CCorefine3d::followIntersection(CDart * AVertex1, CDart * AVertex2,
				     int AMark)
{
  ENTER;

  /* Le marquage n'est pas encore au point car il teste plusieurs fois des
   * intersections déjà traitées. Cependant, l'algorithme fonctionne quand
   * même car il se contente de traiter l'intersection une deuxième fois comme
   * s'il s'agissait d'arêtes confondues...
   */

  assert(FMap->isMarked(AVertex1, FOrientMark));
  assert(FMap->isMarked(AVertex2, FOrientMark));

  list<CDart*> inter_list;
  CDart *d1, *d2, *current, *first, *last;
  bool ok;
  int mark = FMap->getNewMark();
  CTime start, end;
  int nb_vertices = 0;
  CPlane plane1, plane2;

  MSG("mark = " << mark);

  FNumberOfIntersectionLines++;

  cout << "--> Suivi d'une ligne de coupe" << endl;
  start.updateTime();

  SAVE_POINT(*getVertex(AVertex1));

  inter_list.push_back(AVertex1);
  inter_list.push_back(AVertex2);
  while (!inter_list.empty()) {
    d1 = inter_list.front(), inter_list.pop_front();
    d2 = inter_list.front(), inter_list.pop_front();

    nb_vertices++;

    for (CStaticCoverageVertex dcv1(FMap, d1); dcv1.cont(); ++dcv1)
      if (!FMap->isMarked(*dcv1, mark) &&
	  FMap->isMarked(*dcv1, FOrientMark)) {
	FMap->markOrbit(*dcv1, ORBIT_13, mark);

	first = *dcv1;
	last = FTools.alpha2(a1(*dcv1));
	
	for (CStaticCoverageVertex dcv2(FMap, d2); dcv2.cont(); ++dcv2)
	  if (!FMap->isMarked(*dcv2, mark) &&
	      FMap->isMarked(*dcv2, FOrientMark)) {
	    FMap->markOrbit(*dcv2, ORBIT_13, mark);
	    
	    /* Comme la première face peut être éclatée par chacune des
	     * autres faces testées, il faut à chaque tour parcourir les
	     * sous-faces de la première face
	     */
	    ok = false;
	    current = first;
	    do {
	      plane1 = FTools.facePlane(current, FVertexDI);
	      plane2 = FTools.facePlane(*dcv2, FVertexDI);
	      
	      if (!areFacesLinked(current, *dcv2)) {
		if ((plane1.getNormal() * plane2.getNormal()).norm() > FEps) {
		  ok = manageFacesIntersection(current, plane1,
					       *dcv2, plane2,
					       AMark, &inter_list);
		}
		else {
		  WARN_BB("<followIntersection> Faces coplanaires !");
		}
	      }

	      current = FTools.alpha2(a1(current));
	    }
	    while (current != last && !ok);
	  }
	
	FMap->unmarkOrbit(d2, ORBIT_VERTEX, mark);
      }

    FMap->markOrbit(d1, ORBIT_VERTEX, AMark);
    FMap->unmarkOrbit(d1, ORBIT_VERTEX, mark);
  }

  FMap->freeMark(mark);

  end.updateTime();
//   cout << "    Il y a eu " << nb_vertices << " étape(s) lors du suivi\n";
  cout << "    Durée du suivi : " << end - start << endl;
  FLineCreationTime += end - start;

//   cout << "    Mise à jour de la grille" << endl;
//   start.updateTime();
//   int nb_cells = refineGrid(MAX_GRID_RES / 16 , MAX_NUMBER_OF_FACES);
//   end.updateTime();
//   cout << "    " << nb_cells << " mise(s) à jour effectuée(s) en "
//        << end - start << endl;

  EXIT;
}

//******************************************************************************

bool CCorefine3d::manageFacesIntersection(CDart * AFace1,
					  const CPlane & APlane1,
					  CDart * AFace2,
					  const CPlane & APlane2,
					  int AMark,
					  list<CDart*> * AList)
{
  ENTER;

  assert(FMap->isMarked(AFace1, FOrientMark));
  assert(FMap->isMarked(AFace2, FOrientMark));

  CEdgeIntersection inter1, inter2;
  CVertex pt, dir;
  bool result = false;

#ifdef DEBUG_MESSAGES
  FTools.displayFaceVertices(AFace1, FVertexDI);
  FTools.displayFaceVertices(AFace2, FVertexDI);
#endif

  if (FTools.isVectorNull(APlane1.getNormal())) {
    cout << "\033[1;34m";
    FTools.displayFaceVertices(AFace1, FVertexDI);
    cout << "<manageFacesIntersection> La normale à la face 1 est nulle : "
	 << APlane1.getNormal() << "\033[0m\n";
  }
  if (FTools.isVectorNull(APlane2.getNormal())) {
    cout << "\033[1;34m";
    FTools.displayFaceVertices(AFace2, FVertexDI);
    cout << "<manageFacesIntersection> La normale à la face 2 est nulle : "
	 << APlane2.getNormal() << "\033[0m\n";
  }

  pt = *getVertex(AFace1);

  // Calcul de la ligne de coupe
  dir = APlane1.getNormal() * APlane2.getNormal();

//   dir = FTools.normalizeVector(dir);
  
  if (!FTools.isVectorInSector(dir, AFace1, APlane1, true, FVertexDI) ||
      !FTools.isVectorInSector(dir, AFace2, APlane2, true, FVertexDI))
    dir = -dir;
  
  if (FTools.isVectorInSector(dir, AFace1, APlane1, true, FVertexDI) &&
      FTools.isVectorInSector(dir, AFace2, APlane2, true, FVertexDI)) {
    MSG("Ligne de coupe = (" << pt << "," << pt + dir << ")");
    
    // On cherche la plus proche intersection dans chaque face
    inter1 = FTools.findNearestIntersectionInFace(pt, dir, AFace1, APlane1,
						  true, FVertexDI);
    inter2 = FTools.findNearestIntersectionInFace(pt, dir, AFace2, APlane2,
						  true, FVertexDI);
    
    if (inter1.getCell() != AFace1 && inter2.getCell() != AFace2) {
      if (inter1.getCell() != NULL && inter2.getCell() != NULL) {
	createIntersectionEdge(AFace1, AFace2, APlane1, APlane2,
			       inter1, inter2, AMark, AList);
	result = true;
      }
    }
    else {
      WARN_BB("<manageFacesIntersection> Intersections trouvées invalides !");
    }
  }

  EXIT;
  
  return result;
}

//******************************************************************************
#define sortFacesAroundEdges sortFacesAroundEdges_SuperNaive

void CCorefine3d::createIntersectionEdge(CDart * AFace1, CDart * AFace2,
					 const CPlane & APlane1,
					 const CPlane & APlane2,
					 const CEdgeIntersection & AInter1,
					 const CEdgeIntersection & AInter2,
					 int AMark, list<CDart*> * AList)
{
  ENTER;

  CDart *d1, *d2, *edge1, *edge2;
  bool add_to_list = true;
  CVertex pt;

  removeFaceFromGrid(FGrid, AFace2);

  // L'intersection se trouve à la même distance sur chacune des faces
  if (FTools.arePointsEqual(AInter1.getPoint(), AInter2.getPoint())) {
    MSG("L'intersection sur chaque face se trouve à égale distance");
    
    pt = (AInter1.getPoint() + AInter2.getPoint()) / 2.0;

    if (AInter1.getCellDimension() == 0) {
      d1 = AInter1.getCell();
      *getVertex(AInter1.getCell()) = pt;
    }
    else
      d1 = splitEdge(AInter1.getCell(), pt);
      
    if (AInter2.getCellDimension() == 0) {
      d2 = AInter2.getCell();
      *getVertex(AInter2.getCell()) = pt;
    }
    else
      d2 = splitEdge(AInter2.getCell(), pt);
      
    if (FMap->isMarked(d1, AMark))
      add_to_list = false;
      
    edge1 = splitFace(AFace1, d1);
    edge2 = splitFace(AFace2, d2);
      
//     for (CDynamicCoverage23 dc(FMap, a0(edge1)); dc.cont(); ++dc) {
//       FMap->setMark(*dc, AMark);
//       FMap->setMark(a1(*dc), AMark);
//       FMap->setMark(a0(*dc), AMark);
//       FMap->setMark(a1(a0(*dc)), AMark);
//     }
    
    SAVE_POINT(pt);
  }
  // L'intersection la plus proche se trouve sur la même face
  else if (AInter1.getParameter() < AInter2.getParameter()) {
    MSG("L'intersection la plus proche se trouve sur la même face");
      
    if (AInter1.getCellDimension() == 0)
      d1 = AInter1.getCell();
    else
      d1 = splitEdge(AInter1.getCell(), AInter1.getPoint());

    pt = getProjectionOnPlane(d1, APlane2);
    *getVertex(d1) = pt;
      
    if (FMap->isMarked(d1, AMark))
      add_to_list = false;
      
    edge1 = splitFace(AFace1, d1);
    edge2 = insertEdgeInFace(AFace2, pt);

    d2 = a1(a0(edge2));

//     for (CDynamicCoverage23 dc(FMap, a0(edge1)); dc.cont(); ++dc) {
//       FMap->setMark(*dc, AMark);
//       FMap->setMark(a1(*dc), AMark);
//       FMap->setMark(a0(*dc), AMark);
//       FMap->setMark(a1(a0(*dc)), AMark);
//     }

    SAVE_POINT(pt);
  }
  // L'intersection la plus proche se trouve sur l'autre face
  else { // (AInter2.getParameter() < AInter1.getParameter())
    MSG("L'intersection la plus proche se trouve sur l'autre face");
      
    if (AInter2.getCellDimension() == 0)
      d2 = AInter2.getCell();
    else
      d2 = splitEdge(AInter2.getCell(), AInter2.getPoint());
      
    pt = getProjectionOnPlane(d2, APlane1);
    *getVertex(d2) = pt;
      
    edge1 = insertEdgeInFace(AFace1, pt);
    edge2 = splitFace(AFace2, d2);

    d1 = a1(a0(edge1));
      
//     for (CDynamicCoverage23 dc(FMap, edge1); dc.cont(); ++dc) {
//       FMap->setMark(*dc, AMark);
//       FMap->setMark(a1(*dc), AMark);
//     }

    SAVE_POINT(pt);
  }
    
  FMap->markOrbit(a0(edge1), ORBIT_VERTEX, AMark);

  assert(FMap->isMarked(edge1, FOrientMark));
  assert(FMap->isMarked(edge2, FOrientMark));
  
  if (!isSameEdge(edge1, edge2)) {
    FNumberOfIntersectionEdges++;
    
    MSG("Tri des faces autour de l'arête d'intersection n°"
	<< FNumberOfIntersectionEdges);
    
    FTools.sortFacesAroundEdges(edge1, edge2, FAlpha2DI, FVertexDI);
  
#ifdef EXTRACT_LINES //---------------------------------------------------------
    FMap->markOrbit(edge1, ORBIT_EDGE, FIntersectionMark);
    FMap->markOrbit(edge2, ORBIT_EDGE, FIntersectionMark);
#endif // EXTRACT_LINES --------------------------------------------------------
  }

  if (getFace(edge2) == getFace(a2(edge2))) {
    addFaceToGrid(FGrid, edge2);
  }
  else {
    addFaceToGrid(FGrid, edge2);
    addFaceToGrid(FGrid, a2(edge2));
  }
  
  if (add_to_list) {
    AList->push_back(d1);
    AList->push_back(d2);
  }

#ifdef FOLLOW_DEBUG_MODE //-----------------------------------------------------
  CGMapVertex * tmp_map = new CGMapVertex;
  char file_name[1024] = "\0";

  stringstream s; 
  s<<"edge";
  s.width(4); s.fill(0); s.flags (ios::right);
  s<<FNumberOfIntersectionEdges;
  s.width(0);
  s<<".map";
  
  d1 = tmp_map->addMapDart();
  d2 = tmp_map->addMapDart();

  tmp_map->linkAlpha0(d1, d2);

  tmp_map->setVertex(d1, *getVertex(edge1));
  tmp_map->setVertex(d2, *getVertex(a0(edge1)));

  tmp_map->save(s.str(), AsciiFormat);

  delete tmp_map;
#endif // FOLLOW_DEBUG_MODE ----------------------------------------------------

  EXIT;
}

#undef sortFacesAroundEdges
//******************************************************************************

void CCorefine3d::applyModifications(CDart * AMesh)
{
  ENTER;

  CDart *d;

  for (CStaticCoverageCC scc(FMap, AMesh); scc.cont(); ++scc) {
    d = (CDart*)FMap->getDirectInfo(*scc, FAlpha2DI);

    if (d != NULL) {
      if (!FMap->isFree2(*scc))
	FMap->unsew2(*scc);
      if (!FMap->isFree2(d))
	FMap->unsew2(d);

      FMap->sew2(*scc, d);

      FMap->setDirectInfo(*scc, FAlpha2DI, NULL);
      FMap->setDirectInfo(a0(*scc), FAlpha2DI, NULL);
      FMap->setDirectInfo(d, FAlpha2DI, NULL);
      FMap->setDirectInfo(a0(d), FAlpha2DI, NULL);
    }
  }

  EXIT;
}

//******************************************************************************
#define COPY(d) ((CDart*)FMap->getDirectInfo(d, copy_direct_info))

void CCorefine3d::extractIntersectionLines(CDart * AMesh)
{
  ENTER;

  int copy_direct_info = FMap->getNewDirectInfo();
  CDart *d;
  CDynamicCoverageCC dcc(FMap, AMesh);

  // Duplication des brins appartenant aux lignes de coupe
  for (dcc.reinit(); dcc.cont(); ++dcc) {
    if (FMap->isMarked(*dcc, FIntersectionMark)) {
      FMap->setDirectInfo(*dcc, copy_direct_info, FMap->addMapDart());
      FMap->setMark(COPY(*dcc), FIntersectionMark);
//       FMap->setVertex(COPY(*dcc), *FMap->findVertex(*dcc));
    }
  }

  // Créations des liaisons entre ces brins
  for (dcc.reinit(); dcc.cont(); ++dcc) {
    if (FMap->isMarked(*dcc, FIntersectionMark)) {
      if (FMap->canSew0(COPY(*dcc), COPY(a0(*dcc))))
	FMap->sew0(COPY(*dcc), COPY(a0(*dcc)));

      d = a1(*dcc);
      while (!FMap->isMarked(d, FIntersectionMark))
	d = a1(a2(d));

      if (FMap->canSew1(COPY(*dcc), COPY(d)))
	FMap->sew1(COPY(*dcc), COPY(d));
    }
  }

  // Créations des liaisons entre ces brins
  for (dcc.reinit(); dcc.cont(); ++dcc) {
    if (FMap->isMarked(*dcc, FIntersectionMark)) {
      if (FMap->canSew2(COPY(*dcc), COPY(a2(*dcc))))
	FMap->sew2(COPY(*dcc), COPY(a2(*dcc)));
      if (FMap->canSew3(COPY(*dcc), COPY(a3(*dcc))))
	FMap->sew3(COPY(*dcc), COPY(a3(*dcc)));
    }
  }

  // Copie des plongements
  for (dcc.reinit(); dcc.cont(); ++dcc) {
    if (FMap->isMarked(COPY(*dcc), FIntersectionMark)&&
	FMap->isMarked(COPY(*dcc), FIntersectionMark)) {
      FMap->unmarkOrbit(COPY(*dcc), ORBIT_VERTEX, FIntersectionMark);
      FMap->setVertex(COPY(*dcc), *FMap->findVertex(*dcc));
    }
  }

  FMap->freeDirectInfo(copy_direct_info);

  EXIT;
}

#undef COPY
//******************************************************************************

#undef a0
#undef a1
#undef a2
#undef a3

//******************************************************************************
