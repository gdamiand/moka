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

#ifndef FACE_INTERSECTION_TOOLS_HH
#define FACE_INTERSECTION_TOOLS_HH

#include "message-macros.hh"

#include "g-map-vertex.hh"
#include "general-tools.hh"
#include "plane.hh"

namespace GMap3d {

  enum TPositionInFace {FP_Outside, FP_Inside,
			FP_OnBorder, FP_OnReversedBorder};

  std::ostream & operator << (std::ostream & AStream, TPositionInFace APos);

  class CIntersectionPoint
  {
  public:
    CIntersectionPoint(const CVertex & APoint = ORIGIN, int ADim = 0,
		       TPositionInFace APos = FP_Outside)
      : FPoint(APoint), FEnteringSector(NULL), FLeavingSector(NULL),
	FCellDim(ADim), FPos(APos) {}

    const CVertex & getPoint         () const { return FPoint;          }
    CDart *         getEnteringSector() const { return FEnteringSector; }
    CDart *         getLeavingSector () const { return FLeavingSector;  }
    int             getCellDim       () const { return FCellDim;        }
    TPositionInFace getPosition      () const { return FPos;            }
    
    void setPoint         (const CVertex & APoint){ FPoint = APoint;           }
    void setEnteringSector(CDart * ASector)       { FEnteringSector = ASector; }
    void setLeavingSector (CDart * ASector)       { FLeavingSector  = ASector; }
    void setCellDim       (int ADim)              { FCellDim = ADim;           }
    void setPosition      (TPositionInFace APos)  { FPos = APos;               }

  private:
    CVertex FPoint;
    CDart *FEnteringSector;
    CDart *FLeavingSector;
    int FCellDim;
    TPositionInFace FPos;
  };

  class CIntersectionPointComparator
  {
  public:
    CIntersectionPointComparator(CGeneralTools * ATools,
				 const CVertex & AIntersectionLine = ORIGIN)
      : FTools(ATools), FInterLine(AIntersectionLine) {}

    const CVertex & getIntersectionLine() const     { return FInterLine;  }
    void setIntersectionLine(const CVertex & ALine) { FInterLine = ALine; }

    bool operator () (const CIntersectionPoint * APoint1,
		      const CIntersectionPoint * APoint2) const
    {
      assert(APoint1->getPoint().getX() != APoint2->getPoint().getX() ||
	     APoint1->getPoint().getY() != APoint2->getPoint().getY() ||
	     APoint1->getPoint().getZ() != APoint2->getPoint().getZ());
//       if (FTools->arePointsEqual(APoint1->getPoint(), APoint2->getPoint())) {
// 	std::cerr << "CIntersectionPointComparator: " << APoint1->getPoint()
// 		  << " = " << APoint2->getPoint() << std::endl;
// 	exit(0);
//       }
      return (APoint2->getPoint() -
	      APoint1->getPoint()).dot(FInterLine) > 0.0;
    }

  private:
    CGeneralTools *FTools;
    CVertex FInterLine;
  };

  class CAngularFaceComparator
  {
  public:
    CAngularFaceComparator(CGMapVertex * AMap, CGeneralTools * ATools,
			   const CVertex & AAxis, const CVertex & ARef,
			   int AVertexDI, int AFacePlaneDI, int ANegativeMark)
      : FMap(AMap), FTools(ATools), FAxis(AAxis), FRefX(ARef),
	FVertexDI(AVertexDI), FFacePlaneDI(AFacePlaneDI),
	FNegativeMark(ANegativeMark)
    {
      FRefY = FAxis * FRefX;
    }
    
    int getArea(const CVertex & AVector)
    {
      if (AVector.dot(FRefX) > 0.0) {
	if (AVector.dot(FRefY) > 0.0) return 0;
	else return 3;
      }
      else {
	if (AVector.dot(FRefY) > 0.0) return 1;
	else return 2;
      }
    }

    bool operator () (CDart * AFace1, CDart * AFace2)
    {
      CVertex n1, n2;
      CPlane *plane;

      plane = (CPlane*)FMap->getDirectInfo(AFace1, FFacePlaneDI);
      if (plane)
	n1 = (FMap->isMarked(AFace1, FNegativeMark) ?
	      -plane->getNormal() : plane->getNormal());
      else {
	FUNC_WARN("Calcul de la normale à une face non sélectionnée !");
	n1 = FTools->faceNormalVector(AFace1, FVertexDI);
      }

      plane = (CPlane*)FMap->getDirectInfo(AFace2, FFacePlaneDI);
      if (plane)
	n2 = (FMap->isMarked(AFace2, FNegativeMark) ?
	      -plane->getNormal() : plane->getNormal());
      else {
	FUNC_WARN("Calcul de la normale à une face non sélectionnée !");
	n2 = FTools->faceNormalVector(AFace2, FVertexDI);
      }

      int area1 = getArea(n1);
      int area2 = getArea(n2);

      if (area1 == area2)
	return (n1 * n2).dot(FAxis) > 0.0;
      else
	return
	  area1 < area2;
    }
    
  private:
    CGMapVertex *FMap;
    CGeneralTools *FTools;
    CVertex FAxis, FRefX, FRefY;
    int FVertexDI;
    int FFacePlaneDI;
    int FNegativeMark;
  };

}

#endif // FACE_INTERSECTION_TOOLS_HH
