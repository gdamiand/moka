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

#ifndef COREFINE_3D_FACE_FACE_HH
#define COREFINE_3D_FACE_FACE_HH

#include "corefine.hh"
#include "g-map-vertex.hh"
#include "corefine-3d-tools.hh"
#include "face-intersection-tools.hh"
#include "grid-3d.hh"
#include "time.hh"

namespace GMap3d {

  typedef CIntersectionPoint                       TInterPt;
  typedef CIntersectionPointComparator             TInterPtCmp;
  typedef std::set<TInterPt*, TInterPtCmp>         TInterPtSet;
//   typedef std::list<CDart*>                        TFaceSet;
  typedef std::multiset<CDart*, CAngularFaceComparator> TFaceSet;

  struct SCorefFace
  {
    SCorefFace() {}
    SCorefFace(CDart * AFace, const CPlane & APlane, const CBoundingBox & ABox)
      : face(AFace), plane(APlane), box(ABox) {}
    CDart *face;
    CPlane plane;
    CBoundingBox box;
  };

  typedef std::list<SCorefFace> TCorefFaceList;

  typedef CGrid3d< std::pair< std::list<CDart*>,
			      std::list<CDart*> > >         TCorefFaceGrid;
  typedef CGrid3dIterator< std::pair< std::list<CDart*>,
				      std::list<CDart*> > > TCorefFaceGridIter;

  class CCorefine3dFF : public CCorefine
  {
  public:
    CCorefine3dFF(CGMapVertex * AMap, bool ACalculateOrientation = true,
		  TCoordinate AEpsilon = 1E-4, int AVertexDI = -1);
    virtual ~CCorefine3dFF();

    void calculateOrientation(bool AState)   { FCalculateOrientation = AState; }
    void optimizeSearch(bool AState)         { FOptimizeSearch = AState;       }
    void setOptimizationGridResolution(int ARes) { FGridResolution = ARes;     }
    void setDisplayMessagesLevel(int ALevel) { FDisplayMessages = ALevel;      }

    const CTime & getInitialisationTime() const    { return FInitialisationTime;    }
    const CTime & getFaceListCreationTime() const  { return FFaceListCreationTime;  }
    const CTime & getFaceListReductionTime() const { return FFaceListReductionTime; }
    const CTime & getSplitTime() const             { return FSplitTime;             }
    const CTime & getMergeTime() const             { return FMergeTime;             }

    virtual int corefine(CDart *& AMesh1, CDart *& AMesh2,
			 std::bitset<NB_MARKS> ACopyMarks = 0);
    virtual int corefine(int AMark1, int AMark2,
			 std::bitset<NB_MARKS> ACopyMarks = 0);

    virtual void splitMeshes(CDart * AMesh1, CDart * AMesh2,
			     std::bitset<NB_MARKS> ACopyMarks = 0);
    virtual void splitMeshes(int AMark1, int AMark2,
			     std::bitset<NB_MARKS> ACopyMarks = 0);
    virtual void mergeMeshes();
    virtual void removeDoubleFaces(CDart **ADarts = NULL, int ANbDarts = 0);
    virtual void linkCompounds(CDart * AMesh1, CDart * AMesh2,
			       std::bitset<NB_MARKS> ACopyMarks = 0);

    virtual void markIntersectionEdges(int AMark, int AObjectsToMark = 3);
    virtual void markDoubleFaces(int AMark);
    virtual void spreadMarksWithoutMerging(std::bitset<NB_MARKS> AMarks);

    virtual void clear();

    const std::list<CDart*> & getInterEdgesList() const  {return FInterEdges; }
    const std::list<CDart*> & getDoubleFacesList() const {return FDoubleFaces;}

  protected:

    virtual void splitMeshes(std::bitset<NB_MARKS> ACopyMarks = 0);

    bool checkEdges();

    void pointDirectInfoToData(int ADirectInfo, CDart * ADart,
			       TOrbit AOrbit, void * AData);

    virtual CBoundingBox initMesh(CDart * AMesh);
    virtual void cleanMesh(CDart * AMesh);
    virtual CBoundingBox initFaces(int AMark);

    SCorefFace computeFaceInfos(CDart * AFace, int AMark);
    TCorefFaceList * buildFaceList(CDart * AMesh);
    TCorefFaceList * buildFaceList(int AMark);
    void reduceFaceLists(TCorefFaceList * AList1, TCorefFaceList * AList2,
			 const CBoundingBox & ABox);
    void updateFaceInList(TCorefFaceList * AList, TCorefFaceList::iterator AElt,
			  std::list<CDart*> * AFaces);

    bool isVertexLinkedWithFace(CDart * AVertex, int AFaceMark);
    void markFaceVertex(CDart * AVertex, int AFaceMark, int AMark);
    CDart * getTwinDartOnFace(CDart * ADart, int AFaceMark);
    CDart * findFaceSector(CDart * AFaceVertex, const CPlane & AFacePlane,
			   int AFaceMark, const CVertex & AVector);
    CDart * findNearestFaceVertex(CDart * AFace,
				  const CPlane & ARefPlane,
				  const CPlane & AClipPlane1,
				  const CPlane & AClipPlane2);
    CVertex edgeInsideVector(CDart * AEdge, const CPlane & AFacePlane);
    CVertex vertexInsideVector(CDart * AVertex, const CPlane & AFacePlane);

    void classifyFaceVertices(CDart * AFace, const CPlane & APlane,
			      int APositiveMark, int ANegativeMark,
			      int AFacesMark, bool AUseVerticesLinks = true,
			      int * ANbVertices = NULL,
			      int * ANbPositiveVertices = NULL,
			      int * ANbNegativeVertices = NULL);
    void getFaceFictiveElements(CDart * AFace, int AFaceMark,
				std::list<CDart*> * AFictVertices,
				std::list<CDart*> * AFictEdges);

//     TInterPt * testVertexIntersection(CDart * AVertex, const CPlane & APlane,
// 				      int APositiveMark, int ANegativeMark,
// 				      int AFacesMark);
    void testPointInside(CDart * AVertex, TInterPt * APoint, int AFaceMark,
			 const CVertex & AInterLine, const CPlane & AFacePlane);
    void testPointBorder(CDart * AVertex,
			 TInterPt * ANewPoint, TInterPt * AOldPoint,
			 const TInterPtCmp & AComparator);
    TInterPtSet * findIntersectionPoints(CDart * AFace,
					 const CPlane & AFacePlane,
					 const CPlane & AInterPlane,
					 const CVertex & AInterLine,
					 int APositiveMark, int ANegativeMark,
					 int AFacesMark,
					 const TInterPtCmp & AComparator);
//     TInterPtSet * findIntersectionPoints(CDart * AFace,
// 					 const CPlane & AFacePlane,
// 					 const CPlane & AInterPlane,
// 					 const CVertex & AInterLine,
// 					 int AFacesMark,
// 					 const TInterPtCmp & AComparator,
// 					 std::list<CDart*> * AFictVertices,
// 					 std::list<CDart*> * AFictEdges);

    virtual CDart * createEdge();
    virtual CDart * insertVertexInFace(CDart * AFace, const CVertex & APoint);
    virtual CDart * insertEdgeInFace(CDart * AVertex1,
				     const CVertex & AVertex2);
    virtual CDart * splitEdge(CDart * AEdge, const CVertex & APoint,
			      std::list<CDart*> * AFictiveEdges = NULL);
    virtual CDart * splitFace(CDart * AVertex1, CDart * AVertex2);

    bool isUselessFictiveVertex(CDart * AVertex);
    bool isUselessFictiveEdge(CDart * AEdge);
    bool isWholeFaceMarked(CDart * AFace, int AMark);
    virtual CDart * copyDoubleFaceData(CDart * AFace);
    virtual CDart * removeFictiveVertex(CDart * AVertex, int ADeleteMark = -1);
    virtual CDart * removeFictiveEdge(CDart * AEdge, int ADeleteMark = -1);
    virtual CDart * removeDoubleFace(CDart * AFace);

    void linkVertices(CDart * AVertex1, CDart * AVertex2);
    void addIntersectionPoint(CDart *AVertices[2], int AFaceNumber,
			      int AFacesMark, int AInterMark,
			      std::list<CDart*> * AInterList);

    virtual bool intersectFaces(CDart * AFace1, CDart * AFace2,
				const CPlane & APlane1,
				const CPlane & APlane2,
				std::list<CDart*> * AFaceList1,
				std::list<CDart*> * AFaceList2);

    virtual void intersectSecantFaces(CDart * AFace1, CDart * AFace2,
				      const CPlane & APlane1,
				      const CPlane & APlane2,
				      int APositiveMark, int ANegativeMark,
				      int AFacesMark,
				      std::list<CDart*> * AFictiveVertices,
				      std::list<CDart*> * AFictiveEdges);

    virtual void intersectCoplanarFaces(CDart * AFace1, CDart * AFace2,
					const CPlane & APlane1,
					const CPlane & APlane2,
					int APositiveMark, int ANegativeMark,
					int AFacesMark,
					std::list<CDart*> * AFictiveVertices,
					std::list<CDart*> * AFictiveEdges);

//     virtual bool intersectSecantFaces(CDart * AFace1, CDart * AFace2,
// 				      const CPlane & APlane1,
// 				      const CPlane & APlane2,
// 				      std::list<CDart*> * AFaceList1,
// 				      std::list<CDart*> * AFaceList2);

    void assignFacesPlaneInfo(int ADirectInfo, int ANegativeMark,
			      TCorefFaceList * AList);
    void removeFacesPlaneInfo(int ADirectInfo, int ANegativeMark,
			      TCorefFaceList * AList);
    void sortFacesAroundEdges(int AFacePlaneDI, int ANegativeMark);
    TFaceSet * sortFacesAroundEdges(CDart * AEdge1, CDart * AEdge2,
				    int AFacePlaneDI, int ANegativeMark);
    void linkSortedFaces(TFaceSet * AFaceSet);
    void spreadMarksAroundEdges(TFaceSet * AFaceSet,
				std::bitset<NB_MARKS> AMarks);

    bool isPointInFace(const CVertex & APoint, CDart * AFace,
		       const CPlane & APlane);

  protected:
    CCorefine3dTools *FTools;
    bool FCalculateOrientation;
    bool FLocalVertexDI;
    bool FOptimizeSearch;
    int FGridResolution;
    int FDisplayMessages;

    CTime FInitialisationTime;
    CTime FFaceListCreationTime;
    CTime FFaceListReductionTime;
    CTime FSplitTime;
    CTime FMergeTime;

    int FVertexDI;
    int FInterPointDI;
    int FLinkedVertexDI;

    int FInterEdgeMark;
    int FFictiveVertexMark;
    int FFictiveEdgeMark;
    int FFaceMark;
    int FDoubleFaceMark;

    TCorefFaceList *FMesh1, *FMesh2;
    std::list<CDart*> FInterEdges;
    std::list<CDart*> FDoubleFaces;
  };

}

#endif // COREFINE_3D_FACE_FACE_HH
