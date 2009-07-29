/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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

//******************************************************************************
#include "g-map-vertex.hh"
#include "geometry.hh"
using namespace GMap3d;
//******************************************************************************
void CGMapVertex::applyMatrix(const CTransformationMatrix & AMatrix, CDart * ADart,
			      TOrbit AOrbit, int ADirectInfoVertex)
{
  assert(ADart!=NULL);

  int mark= getNewMark();

  markOrbit(ADart, AOrbit, mark);
  applyMatrix(AMatrix, mark, ADirectInfoVertex);
  unmarkOrbit(ADart, AOrbit, mark);

  freeMark(mark);
}
//******************************************************************************
void CGMapVertex::applyMatrix(const CTransformationMatrix & AMatrix,
			      int AMarkNumber, int ADirectInfoVertex)
{
  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);
	
	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * Vdest = getVertex(*cov);
		  assert(Vdest!=NULL);

		  CVertex * Vsrce =
		    ADirectInfoVertex<0 ? Vdest :
		    getDirectInfoAsVertex(*cov, ADirectInfoVertex);
		  assert(Vsrce!=NULL);

		  AMatrix.applyOn(*Vsrce, *Vdest);
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
#define SET_VALUE(DART, INDEX, VALUE) \
((* getDirectInfoAsCoord(DART, INDEX)) = (VALUE))
//------------------------------------------------------------------------------
void CGMapVertex::computeDistanceToVertex(int AMarkNumber, int ADirectInfoIndex,
					  const CVertex & AVertex)
{
  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);
	
	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * vertex = getVertex(*cov);
		  assert(vertex!=NULL);
		
		  SET_VALUE(*cov, ADirectInfoIndex,
			    CGeometry::distanceToVertex(*vertex, AVertex));
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::computeDistanceToLine(int AMarkNumber, int ADirectInfoIndex,
					const CVertex & ALineVertex,
					const CVertex & ALineDirection)
{
  assert(!ALineDirection.isNull());

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);
	
	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * vertex = getVertex(*cov);
		  assert(vertex!=NULL);
		
		  SET_VALUE(*cov, ADirectInfoIndex,
			    CGeometry::distanceToLine(*vertex,
						      ALineVertex,
						      ALineDirection));
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::computeDistanceToPlane(int AMarkNumber, int ADirectInfoIndex,
					 const CVertex & APlaneVertex,
					 const CVertex & APlaneNormal)
{
  assert(!APlaneNormal.isNull());

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);
	
	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * vertex = getVertex(*cov);
		  assert(vertex!=NULL);
		
		  SET_VALUE(*cov, ADirectInfoIndex,
			    CGeometry::distanceToPlane(*vertex,
						       APlaneVertex,
						       APlaneNormal));
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//------------------------------------------------------------------------------
#undef SET_VALUE
//******************************************************************************
void CGMapVertex::normalizeParameter(int AMarkNumber, int ADirectInfoIndex,
				     TCoordinate AMin, TCoordinate AMax)
{
  int treated = getNewMark();

  bool first = true;
  /* Les variables suivantes sont initialisées uniquement pour éviter un warning
   * à la compilation.
   */
  TCoordinate oldMin = 0, oldMax = 0;

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  TCoordinate & value =
		    * getDirectInfoAsCoord(*cov,ADirectInfoIndex);

		  if (first)
		    {
		      first = false;
		      oldMin = oldMax = value;
		    }
		  else
		    {
		      if (value < oldMin) oldMin = value;
		      if (value > oldMax) oldMax = value;
		    }
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);

  for (it.reinit(); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  TCoordinate & value =
		    * getDirectInfoAsCoord(*cov,ADirectInfoIndex);

		  if (oldMax==oldMin)
		    value = (AMin+AMax)/2;
		  else
		    value = AMin + (AMax-AMin)*(value-oldMin)/(oldMax-oldMin);
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::applyFunctionOnParameter(int AMarkNumber,
					   int ADirectInfoIndex,
					   TFunctionType AFunctionType)
{
  if (AFunctionType==FUNCTION_LIN)
    return;

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);
	
	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  TCoordinate & value =
		    * getDirectInfoAsCoord(*cov, ADirectInfoIndex);

		  switch (AFunctionType)
		    {
		    case FUNCTION_QUAD: value = value*value              ; break;
		    case FUNCTION_EXP : value = exp(value)               ; break;
		    case FUNCTION_LOG : value = value<=0 ? 0 : log(value); break;
		    case FUNCTION_SIN : value = sin(value)               ; break;
		    case FUNCTION_COS : value = cos(value)               ; break;
		    default:
		      assert(false);
		    }
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::translate(int AMarkNumber, const CVertex & AVector,
			    int ADirectInfoVertex, int ADirectInfoParam)
{

  if (ADirectInfoParam<0)
    {
      CTransformationMatrix translationMatrix(CTransformationMatrix::IdentityMatrix);
      translationMatrix.translate(AVector);
      applyMatrix(translationMatrix, AMarkNumber, ADirectInfoVertex);
      return;
    }

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * Vdest = getVertex(*cov);
		  assert(Vdest!=NULL);

		  CVertex * Vsrce =
		    ADirectInfoVertex<0 ? Vdest :
		    getDirectInfoAsVertex(*cov,ADirectInfoVertex);
		  assert(Vsrce!=NULL);
	
		  TCoordinate param =
		    * getDirectInfoAsCoord(*cov,ADirectInfoParam);

		  CTransformationMatrix translationMatrix(CTransformationMatrix::IdentityMatrix);
		  translationMatrix.translate(param*AVector);

		  translationMatrix.applyOn(*Vsrce, *Vdest);
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::rotate(int AMarkNumber,
			 const CVertex & AAxeVertex,
			 const CVertex & AAxeDirection,
			 TCoordinate AAngle,
			 int ADirectInfoVertex, int ADirectInfoParam)
{
  if (ADirectInfoParam<0)
    {
      CTransformationMatrix
	rotationMatrix(CTransformationMatrix::IdentityMatrix);
      rotationMatrix.rotate(AAxeVertex, AAxeDirection, AAngle);
      applyMatrix(rotationMatrix, AMarkNumber, ADirectInfoVertex);
      return;
    }

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * Vdest = getVertex(*cov);
		  assert(Vdest!=NULL);

		  CVertex * Vsrce =
		    ADirectInfoVertex<0 ? Vdest :
		    getDirectInfoAsVertex(*cov,ADirectInfoVertex);
		  assert(Vsrce!=NULL);
	
		  TCoordinate param =
		    * getDirectInfoAsCoord(*cov, ADirectInfoParam);

		  CTransformationMatrix rotationMatrix(CTransformationMatrix::IdentityMatrix);
		  rotationMatrix.rotate(AAxeVertex, AAxeDirection,
					param*AAngle);
		
		  rotationMatrix.applyOn(*Vsrce, *Vdest);
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::scale(int AMarkNumber,
			const CVertex & ACenter, const CVertex & ACoef,
			int ADirectInfoVertex, int ADirectInfoParam)
{
  if (ADirectInfoParam<0)
    {
      CTransformationMatrix scaleMatrix(CTransformationMatrix::IdentityMatrix);
      scaleMatrix.scale(ACenter, ACoef);
      applyMatrix(scaleMatrix, AMarkNumber, ADirectInfoVertex);
      return;
    }

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * Vdest = getVertex(*cov);
		  assert(Vdest!=NULL);

		  CVertex * Vsrce =
		    ADirectInfoVertex<0 ? Vdest :
		    getDirectInfoAsVertex(*cov, ADirectInfoVertex);
		  assert(Vsrce!=NULL);
	
		  TCoordinate param =
		    * getDirectInfoAsCoord(*cov, ADirectInfoParam);

		  CTransformationMatrix scaleMatrix(CTransformationMatrix::IdentityMatrix);
		  scaleMatrix.scale(ACenter, param*ACoef);
		
		  scaleMatrix.applyOn(*Vsrce, *Vdest);
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::scale(int AMarkNumber,
			const CVertex & ACenter, TCoordinate ACoef,
			int ADirectInfoVertex, int ADirectInfoParam)
{
  scale(AMarkNumber,
	ACenter, CVertex(ACoef, ACoef, ACoef),
	ADirectInfoVertex, ADirectInfoParam);
}
//******************************************************************************
void CGMapVertex::axialScale(int AMarkNumber,
			     const CVertex & AAxeVertex,
			     const CVertex & AAxeDirection,
			     TCoordinate ACoef,
			     int ADirectInfoVertex, int ADirectInfoParam)
{
  assert(!AAxeDirection.isNull());

  if (ADirectInfoParam<0)
    {
      CTransformationMatrix scaleMatrix(CTransformationMatrix::IdentityMatrix);
      scaleMatrix.axialScale(AAxeVertex, AAxeDirection, ACoef);
      applyMatrix(scaleMatrix, AMarkNumber, ADirectInfoVertex);
      return;
    }

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * Vdest = getVertex(*cov);
		  assert(Vdest!=NULL);

		  CVertex * Vsrce =
		    ADirectInfoVertex<0 ? Vdest :
		    getDirectInfoAsVertex(*cov, ADirectInfoVertex);
		  assert(Vsrce!=NULL);
	
		  TCoordinate param =
		    * getDirectInfoAsCoord(*cov, ADirectInfoParam);

		  CTransformationMatrix scaleMatrix(CTransformationMatrix::IdentityMatrix);
		  scaleMatrix.axialScale(AAxeVertex, AAxeDirection,
					 param*ACoef);
		
		  scaleMatrix.applyOn(*Vsrce, *Vdest);
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::planarScale(int AMarkNumber,
			      const CVertex & APlaneVertex,
			      const CVertex & APlaneNormal,
			      TCoordinate ACoef,
			      int ADirectInfoVertex, int ADirectInfoParam)
{
  assert(!APlaneNormal.isNull());

  if (ADirectInfoParam<0)
    {
      CTransformationMatrix scaleMatrix(CTransformationMatrix::IdentityMatrix);
      scaleMatrix.planarScale(APlaneVertex, APlaneNormal, ACoef);
      applyMatrix(scaleMatrix, AMarkNumber, ADirectInfoVertex);
      return;
    }

  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  for (CDynamicCoverageVertex cov(this, *it); cov.cont(); ++cov)
	    {
	      setMark(*cov, treated);

	      if (isOrbitUsed(*cov, ORBIT_VERTEX))
		{
		  CVertex * Vdest = getVertex(*cov);
		  assert(Vdest!=NULL);

		  CVertex * Vsrce =
		    ADirectInfoVertex<0 ? Vdest :
		    getDirectInfoAsVertex(*cov, ADirectInfoVertex);
		  assert(Vsrce!=NULL);
	
		  TCoordinate param =
		    * getDirectInfoAsCoord(*cov, ADirectInfoParam);

		  CTransformationMatrix scaleMatrix(CTransformationMatrix::IdentityMatrix);
		  scaleMatrix.planarScale(APlaneVertex, APlaneNormal,
					  param*ACoef);
		
		  scaleMatrix.applyOn(*Vsrce, *Vdest);
		}
	    }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
void CGMapVertex::orthoProjectOnPlane(int AMarkNumber,
				      TCoordinate AA,
				      TCoordinate AB,
				      TCoordinate AC,
				      TCoordinate AD)
{
  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (! isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    CVertex & v = * findVertex(*it);

	    v = CGeometry::orthoProjectVertexOnPlane(v, AA, AB, AC, AD);
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
