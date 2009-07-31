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

#ifndef COREFINE_3D_TOOLS_HH
#define COREFINE_3D_TOOLS_HH

#include "g-map-vertex.hh"
#include "plane.hh"
#include "corefine-2d-tools.hh"
#include "attribute-bounding-box.hh"

namespace GMap3d {

  class CCorefine3dTools : public CCorefine2dTools
  {
  public:

    CCorefine3dTools(CGMapVertex * AMap, TCoordinate AEpsilon = 1E-4);

    bool isVectorInSector(const CVertex & AVector,
			  const CVertex & ASectorVector1,
			  const CVertex & ASectorVector2,
			  const CPlane & APlane,
			  bool AThickVector1 = false,
			  bool AThickVector2 = false);

    bool isVectorInSector(const CVertex & AVector, CDart * ASector,
			  const CPlane & APlane,
			  bool ATestBorders = false,
			  int AVertexDI = -1);
  
    virtual bool areVectorsColinear(const CVertex & AVector1,
				    const CVertex & AVector2);

    virtual TCoordinate pointParameterOnLine(const CVertex & APoint,
					     const CVertex & ALineVertex1,
					     const CVertex & ALineVertex2);

    CDart * findSectorOfVector(const CVertex & AVector, CDart * AVertex,
			       const CPlane & APlane, int AVertexDI = -1);

    bool areFacesCoplanar(const CVertex & AVertex1,
			  const CPlane & APlane1,
			  const CVertex & AVertex2,
			  const CPlane & APlane2);

    virtual CDart * findWellOrientedDart(CDart * AVertex, int AVertexDI = -1);

    virtual TPositionOnEdge localizeEdgesIntersection(const CVertex & ALinePoint1,
						      const CVertex & ALinePoint2,
						      const CVertex & AEdgePoint1,
						      const CVertex & AEdgePoint2,
						      TCoordinate * ALineParam,
						      TCoordinate * AEdgeParam);

    TPositionOnEdge localizeEdgeAndPlaneIntersection(CVertex AEdgePoint1,
						     CVertex AEdgePoint2,
						     const CPlane & APlane,
						     TCoordinate * AEdgeParam);

    CEdgeIntersection findNearestIntersectionInFace(const CVertex & AOrigin,
						    const CVertex & ADirection,
						    CDart * AFace,
						    const CPlane & APlane,
						    bool AExcludeOrigin = true,
						    int AVertexDI = -1);

    CEdgeIntersection findNearestIntersectionInFace(const CVertex & AOrigin,
						    const CVertex & ADirection,
						    CDart * AFace,
						    const CPlane & AFacePlane,
						    const CPlane & ACutPlane,
						    bool AExcludeOrigin = true,
						    int AVertexDI = -1);

    CEdgeIntersection findNearestIntersectionInOrbit(const CVertex & AOrigin,
						     const CVertex & ADirection,
						     CDart * ADart,
						     TOrbit AOrbit,
						     int AVertexDI = -1);

    virtual bool isPointInFace(const CVertex & APoint, CDart * AFace,
			       const CPlane * APlane = NULL,
			       int AVertexDI = -1);

    void sortFacesAroundEdges_Naive(CDart * AEdge1, CDart * AEdge2,
				    int ALinkDirectInfo, int AVertexDI = -1);

    void sortFacesAroundEdges_NaiveBis(CDart * AEdge1, CDart * AEdge2,
				       int ALinkDirectInfo, int AVertexDI = -1);
  
    bool sortFacesAroundEdges_Optimized(CDart * AEdge1, CDart * AEdge2,
					int ALinkDirectInfo,
					int AVertexDI = -1);

    void sortFacesAroundEdges_SuperNaive(CDart * AEdge1, CDart * AEdge2,
					 int ALinkDirectInfo,
					 int AVertexDI = -1);

    TCoordinate matrixDet(TCoordinate AMat[3][3]);

    void vectProduct4d(const CVertex & AV1, const CVertex & AV2,
		       const CVertex & AV3, TCoordinate ANormal[4]);

    void volumeNormalVector(CDart * AVolume, TCoordinate ANormal[4],
			    int AVertexDI = -1);

  protected:
  };

} // namespace GMap3d

#endif
