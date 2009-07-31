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

#ifndef COREFINE_2D_PROPAGATION_HH
#define COREFINE_2D_PROPAGATION_HH

#include "inline-macro.hh"
#include "g-map-vertex.hh"
#include "corefine.hh"
#include "corefine-2d-tools.hh"
#include "plane.hh"
#include "edge-intersection.hh"
#include "time.hh"

namespace GMap3d
{

  class CCorefine2dPropagation : public CCorefine
  {
  public:

    CCorefine2dPropagation(CGMapVertex * AMap,
			   const CPlane & APlane = CPlane(OZ, ORIGIN),
			   bool ACalculateOrientation = true,
			   TCoordinate AEpsilon = 1E-4, int AVertexDI = -1);
    virtual ~CCorefine2dPropagation();

    virtual int corefine(CDart *& AMesh1, CDart *& AMesh2,
			 std::bitset<NB_MARKS> ACopyMarks = 0);

    const CTime & getInitialisationTime() const;
    const CTime & getOrientationTime() const;
    const CTime & getLocalizationTime() const;
    const CTime & getPropagationTime() const;
    const CTime & getAssemblyTime() const;
    const CTime & getCleaningTime() const;
    const CTime & getDoubleEdgesCleaningTime() const;

  protected:

    virtual void initMesh(CDart * AMesh);
    virtual void cleanMesh(CDart * AMesh);

    virtual CVertex * getVertex(CDart * ADart);
    virtual void modifyVertex(CDart * ADart, const CVertex & AVertex);

    virtual CDart * splitEdge(CDart * AEdge, const CVertex & AVertex);

    virtual void weldVertices(CDart * AVertex1, CDart * AVertex2);

    virtual void weldMultipleVertices(CDart * AVertex1, CDart * AVertex2);

    virtual void applyModifications(CDart * AMesh);

  protected:

    CPlane FPlane;
    TProjection FBestProj;

    CCorefine2dTools FTools;

    int FVertexDI;
    int FAlpha1DI;

    bool FCalculateOrientation;
    bool FLocalVertexDirectInfo;

    int FNumberOfIntersections;

    CTime FInitialisationTime;
    CTime FOrientationTime;
    CTime FLocalizationTime;
    CTime FPropagationTime;
    CTime FAssemblyTime;
    CTime FCleaningTime;
    CTime FDoubleEdgesCleaningTime;
  };

} // namespace GMap3d

#include INCLUDE_INLINE("corefine-2d-propagation.icc")

#endif
