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

#ifndef COREFINE_3D_HH
#define COREFINE_3D_HH

#include "inline-macro.hh"
#include "corefine.hh"
#include "attribute-bounding-box.hh"
#include "corefine-3d-tools.hh"
#include "edge-intersection.hh"
#include "grid-3d.hh"
#include "grid-tree-3d.hh"
#include "time.hh"

namespace GMap3d {

  typedef CGridTree3d<std::list<CDart*>*> TFaceGrid;
  typedef CGridTree3dIterator<std::list<CDart*>*> TFaceGridIter;

  class CCorefine3d : public CCorefine
  {
  public:

    CCorefine3d(CGMapVertex * AMap, bool ACalculateOrientation = true,
		TCoordinate AEpsilon = 1E-4, int AVertexDI = -1);
    ~CCorefine3d();

    void computeOnlyFirstIntersection(bool ABoolean);
    void setGridResolution(int ARes);

    virtual int corefine(CDart *& AMesh1, CDart *& AMesh2,
			 std::bitset<NB_MARKS> ACopyMarks = 0);

    const CTime & getInitialisationTime() const { return FInitialisationTime; }
    const CTime & getGridCreationTime() const   { return FGridCreationTime;   }
    const CTime & getResearchTime() const       { return FResearchTime;       }
    const CTime & getLineCreationTime() const   { return FLineCreationTime;   }
    const CTime & getUpdateTime() const         { return FUpdateTime;         }

  protected:

    virtual unsigned long initMesh(CDart * AMesh);
    virtual void cleanMesh(CDart * AMesh);

    void createGrid(CDart * AMesh, unsigned long ANbFaces);
    void destroyGrid();

    unsigned int getMaxVerticesDegree(std::list<CDart*> * AList);
    CVertex normalizeGridSize(const CVertex & AGridSize);
    void getGridResolution(const CVertex & AGridSize,
			   unsigned int AMinRes,
			   unsigned int AMaxRes,
			   unsigned int * AResX,
			   unsigned int * AResY,
			   unsigned int * AResZ);
    void computeGridResolution(const CVertex & AGridSize,
			       unsigned long ANbFaces,
			       TCoordinate ANbFacesPerCell,
			       unsigned int * AResX,
			       unsigned int * AResY,
			       unsigned int * AResZ,
			       unsigned int AMinRes = 1,
			       unsigned int AMaxRes = 512);
    unsigned int refineGrid(unsigned int AMaxSubDiv,
			    unsigned int AMaxNumberOfFaces);

    void addFaceToGrid(TFaceGrid * AGrid, CDart * AFace);
    void removeFaceFromGrid(TFaceGrid * AGrid, CDart * AFace);

    void updateVertexLinks(CDart * ADart, CAttributeVertex * AVertex);
    void updateVertexLinks(CDart * ADart);
    CVertex * getVertex(CDart * ADart);

    void updateFaceLinks(CDart * AFace);
    CDart * getFace(CDart * ADart);

    CDart * createEdge();

    virtual CDart * insertVertexInFace(CDart * AFace, const CVertex & APoint);
    virtual CDart * insertEdgeInFace(CDart * AVertex1, const CVertex & AVertex2);
    virtual CDart * splitFace(CDart * AVertex1, CDart * AVertex2);
    virtual CDart * splitEdge(CDart * AVertex, const CVertex & APoint);
    virtual CDart * removeEdge(CDart * AEdge, int ADeleteMark = -1);
    CVertex getProjectionOnPlane(CDart * AVertex, const CPlane & APlane);

    bool isSameEdge_Naive(CDart * AEdge1, CDart * AEdge2);
    bool isSameEdge_Optimized(CDart * AEdge1, CDart * AEdge2);
    bool areFacesLinked(CDart * AFace1, CDart * AFace2);

    CEdgeIntersection findNearestIntersectionInGrid(const CVertex & AVertex1,
						    const CVertex & AVertex2);

    CDart * findFirstIntersectionInGrid(const CVertex & AVertex, int * ADim);

    void followIntersection(CDart * AVertex1, CDart * AVertex2, int AMark);

    bool manageFacesIntersection(CDart * AFace1, const CPlane & APlane1,
				 CDart * AFace2, const CPlane & APlane2,
				 int AMark, std::list<CDart*> * AList);

    void createIntersectionEdge(CDart * AFace1, CDart * AFace2,
				const CPlane & APlane1,
				const CPlane & APlane2,
				const CEdgeIntersection & AInter1,
				const CEdgeIntersection & AInter2,
				int AMark, std::list<CDart*> * AList);

    void applyModifications(CDart * AMesh);

    void extractIntersectionLines(CDart * AMesh);

  private:

    int FVertexDI;
    int FFaceDI;

    int FAlpha2DI;

    int FOrientMark;
    int FFictiveMark;
    int FIntersectionMark;

    int FNumberOfIntersectionLines;
    int FNumberOfIntersectionEdges;

    bool FLocalVertexDirectInfo;
    bool FCalculateOrientation;
    bool FComputeOnlyFirstIntersection;
    int FGridResolution;

    CTime FInitialisationTime;
    CTime FGridCreationTime;
    CTime FResearchTime;
    CTime FLineCreationTime;
    CTime FUpdateTime;

    CCorefine3dTools FTools;
  
    TFaceGrid * FGrid;
  };

} // namespace GMap3d

#include INCLUDE_INLINE("corefine-3d.icc")

#endif
