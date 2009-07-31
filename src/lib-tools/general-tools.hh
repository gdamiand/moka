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

#ifndef GENERAL_TOOLS_HH
#define GENERAL_TOOLS_HH

#include "g-map-vertex.hh"

class CPlane;
class CBoundingBox;

namespace GMap3d {

  /** Classe définissant des outils divers
   *
   * @author Nicolas Guiard
   */

  class CGeneralTools
  {
  public:

    /** @name Constructeurs et destructeur
     */

    //@{

    /** Constructeur par défaut
     *
     * @return Une nouvelle instance de la classe
     *
     * Construction d'une nouvelle instance de la classe
     */
    CGeneralTools(CGMapVertex * AMap, TCoordinate AEpsilon = 1E-4);

    /** Destructeur
     *
     * Destruction de l'instance.
     */
    virtual ~CGeneralTools();

    //@}

    /** @name Méthodes publiques
     */

    //@{

    CVertex normalizeVector(const CVertex & AVector);

    bool isVectorNull(const CVertex & AVector);
    bool areVectorsColinear(const CVertex & AVector1, const CVertex & AVector2);

    TCoordinate pointParameterOnLine(const CVertex & APoint,
				     const CVertex & ALineVertex1,
				     const CVertex & ALineVertex2);

    TCoordinate squareDistanceToLine(const CVertex & APoint,
				     const CVertex & ALineVertex1,
				     const CVertex & ALineVertex2);
    TCoordinate squareDistanceToPlane(const CVertex & APoint,
				      const CPlane & APlane);
    TCoordinate squareDistanceBetweenLines(const CVertex & ALine1Vertex1,
					   const CVertex & ALine1Vertex2,
					   const CVertex & ALine2Vertex1,
					   const CVertex & ALine2Vertex2,
					   CPlane * AIntersectionPlane = NULL);

    bool getLinesIntersection(CVertex ALine1Point1, CVertex ALine1Point2,
			      CVertex ALine2Point1, CVertex ALine2Point2,
			      TCoordinate * ALine1Param,
			      TCoordinate * ALine2Param,
			      const CPlane & APlane);

    virtual bool arePointsEqual(const CVertex & APoint1,
				const CVertex & APoint2);
    virtual bool isPointOnLine(const CVertex & APoint,
			       const CVertex & ALineVertex1,
			       const CVertex & ALineVertex2);
    virtual bool isPointOnPlane(const CVertex & APoint, const CPlane & APlane);
    virtual bool areLinesCrossing(const CVertex & ALine1Vertex1,
				  const CVertex & ALine1Vertex2,
				  const CVertex & ALine2Vertex1,
				  const CVertex & ALine2Vertex2,
				  CPlane * AIntersectionPlane = NULL);

    CDart * alpha1(CDart * ADart);
    CDart * alpha2(CDart * ADart);

    CVertex edgeVector(CDart * AEdge, int AVertexDI = -1);
    virtual CVertex faceNormalVector(CDart * AFace, int AVertexDI = -1);
    virtual CVertex fastFaceNormalVector(CDart * AFace, int AVertexDI = -1);
    virtual CVertex vertexNormalVector(CDart * AVertex, int AVertexDI = -1);
    virtual CVertex fastVertexNormalVector(CDart * AVertex, int AVertexDI = -1);
    virtual CPlane facePlane(CDart * AFace, int AVertexDI = -1);
    virtual CBoundingBox orbitBoundingBox(CDart * ADart, TOrbit AOrbit,
					  int AVertexDI = -1);

    virtual CVertex regionNormalVector(CDart * ADart, TOrbit AOrbit,
				       unsigned int ARegionSize,
				       int ARegionMark = -1,
				       int AVertexDI = -1);

    void displayEdgeVertices(CDart * ADart, int AVertexDI = -1);
    void displayFaceVertices(CDart * ADart, int AVertexDI = -1);

    CDart * thickenVolume(CDart * AMesh, TCoordinate AThick = 1.0,
			  int ABorderSteps = 5, int AVertexDI = -1);

    //@}

  private:
    void thickenBorder(CDart * ABorder, int AMark,
		       TCoordinate AThick, int ASteps,
		       int ASide1DI, int ASide2DI, int AVertexDI = -1);

  protected:

    /** Attributs protégés
     */

    //@{

    CGMapVertex * FMap;

    TCoordinate FEps, FEps2;

    //@}
  };

} // namespace GMap3d

#endif
