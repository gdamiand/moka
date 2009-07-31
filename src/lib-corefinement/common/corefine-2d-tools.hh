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

#ifndef COREFINE_2D_TOOLS_HH
#define COREFINE_2D_TOOLS_HH

#include <bitset>
#include "general-tools.hh"
#include "edge-intersection.hh"

class CBoundingBox;

namespace GMap3d {

  class CCorefine2dTools : public CGeneralTools
  {
  public:

    CCorefine2dTools(CGMapVertex * AMap, TCoordinate AEpsilon = 1E-4);

    bool isVectorInSector(const CVertex & AVector,
			  const CVertex & ASectorVector1,
			  const CVertex & ASectorVector2);
  
    bool isVectorInSector(const CVertex & AVector, CDart * ASector,
			  int AVertexDI = -1);

    virtual bool areVectorsColinear(const CVertex & AVector1,
				    const CVertex & AVector2);

    virtual TCoordinate pointParameterOnLine(const CVertex & APoint,
					     const CVertex & ALineVertex1,
					     const CVertex & ALineVertex2);

    virtual CDart * findSectorOfVector(const CVertex & AVector, CDart * AVertex,
				       int AVertexDI = -1);

    std::list<CDart*> * sortVerticesEdges(CDart * AVertex1, CDart * AVertex2,
					  int AVertexDI = -1);

    std::list<CDart*> * sortMultipleVerticesEdges(CDart * AVertex1,
						  const std::list<CDart*> & AVertices,
						  int AVertexDI = -1);

    virtual CDart * findWellOrientedDart(CDart * AVertex, int AVertexDI = -1);

    virtual TPositionOnEdge localizePointOnEdge(const CVertex & APoint,
						const CVertex & AVertex1,
						const CVertex & AVertex2,
						TCoordinate * AParam);

    virtual TPositionOnEdge localizeEdgesIntersection(const CVertex & ALinePoint1,
						      const CVertex & ALinePoint2,
						      const CVertex & AEdgePoint1,
						      const CVertex & AEdgePoint2,
						      TCoordinate * ALineParam,
						      TCoordinate * AEdgeParam);
    
    CEdgeIntersection findNearestIntersection(const CVertex & AVertex1,
					      const CVertex & AVertex2,
					      CDart * AFace,
					      bool AFaceIsVertex1,
					      int AVertexDI = -1);
    
    CDart * localizePointInMesh(const CVertex & APoint, CDart * AMesh,
				int AVertexDI = -1);
    
    void removeDoubleEdges(CDart *& AMesh1, CDart *& AMesh2, std::bitset<NB_MARKS> ACopyMarks, int AVertexDI = -1);

  protected:
  };

} // namespace GMap3d

#endif
