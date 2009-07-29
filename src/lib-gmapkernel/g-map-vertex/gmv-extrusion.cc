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
CDart* CGMapVertex::extrudeByNormal(CDart* ADart, int ADim,
				    bool AExtrusionWithAdjacentSews,
				    TCoordinate AExtrusionCoef,
				    int AMarkToExtrude, int AMarkExtruded)
{
  assert(canExtrudeCell(ADart, ADim));

  TOrbit orbitPieceOfVertex = AND_ORBIT(ORBIT_VERTEX, ORBIT_INF[ADim]);
  CVertex normal = AExtrusionCoef * faceNormalVector(ADart);

  // Topologie:
  CDart* newDart = CGMapGeneric::extrudeByNormal(ADart, ADim,
						 AExtrusionWithAdjacentSews,
						 AMarkToExtrude,
						 AMarkExtruded);

  // Affectation des plongements:
  int treated = getNewMark();

  CCoverage * cov1 = getDynamicCoverage(ADart  , ORBIT_INF[ADim]);
  CCoverage * cov2 = getDynamicCoverage(newDart, ORBIT_INF[ADim]);

  for (; cov1->cont(); ++(*cov1), ++(*cov2))
    if (!isMarked(**cov1, treated))
      {
  	markOrbit(**cov1, orbitPieceOfVertex, treated);

	if (findVertex(**cov2)==NULL)
	  {
	    CVertex vector;

	    if (AExtrusionWithAdjacentSews && ADim==2)
	      vector = AExtrusionCoef *
		regionNormalVector(**cov1, 0, AMarkToExtrude);
	    else
	      vector = normal;
	
	    setVertex(**cov2, *findVertex(**cov1) + vector);
	  }
      }

  delete cov1;
  delete cov2;

  unmarkOrbit(ADart, ORBIT_INF[ADim], treated);
  freeMark(treated);

  return newDart;
}
//******************************************************************************
// Méthode protégée !
void CGMapVertex::markCellsToBeExtrudedByNormal(int ADim,
						int AMarkNumberSrce,
						int AMarkNumberDest)
{
  halfMarkIncidentCells(ORBIT_INF[ADim], AMarkNumberSrce, AMarkNumberDest);

  // Désélection des cellules sélectionnées des deux côtés:
  if (ADim>0)
    {
      int treated = getNewMark();

      for (CDynamicCoverageAll it(this); it.cont(); ++it)
	if (!isMarked(*it, treated))
	  {
	    if (isMarked(*it, AMarkNumberDest) && !isFree0(*it) &&
		isMarked(alpha0(*it), AMarkNumberDest))
	      {
		markOrbit(*it, ORBIT_INF[ADim], treated);
		unmarkOrbit(*it, ORBIT_INF[ADim], AMarkNumberDest);
	      }
	    else
	      setMark(*it, treated);
	  }

      negateMaskMark(treated);
      freeMark(treated);
    }
}
//******************************************************************************
CDart* CGMapVertex::extrudeByPath(CDart* ADart,
				  int     ADim,
				  CDart* APath,
				  bool    AExtrusionWithAdjacentSews,
				  bool    ARotateExtrudedCells,
				  bool    AScaleExtrudedCells,
				  CDart* APonderationPath,
				  int     AMarkExtruded,
				  int     ADirectInfoIndex)
{
  assert(canExtrudeByPath(ADart, ADim, APath));
  assert(APonderationPath==NULL || !isClosedPolyline(APonderationPath));

  int treated = getNewMark();

  bool change =
    !canExtrudeHalfCell(ADart, ADim) &&
    (isFree0(APath) || isFree1(APath) || isFree0(alpha1(APath)));

  // Topologie:
  CDart* returned =
    CGMapGeneric::extrudeByPath(ADart, ADim, APath,
				AExtrusionWithAdjacentSews,
				AMarkExtruded, ADirectInfoIndex);

  CDart* current = change ? alpha(ADart, ADim+1) : ADart;

  // Calcul des coefficients de pondération:
  int pathNbVertices = getNbPolylineVertices(APath);
  int pathInitialPos = 0;

  if (!isClosedPolyline(APath))
    {
      CDart* current = APath;

      while (!isFree1(current) && !isFree0(alpha1(current)))
	{
	  current = alpha10(current);
	  ++pathInitialPos;
	}
    }

  TCoordinate * pathCoefs = new TCoordinate[pathNbVertices];

  if (APonderationPath!=NULL)
    {
      int ponderationNbVertices = getNbPolylineVertices(APonderationPath);

      // On se place au début du chemin:
      while (!isFree1(APonderationPath) && !isFree0(alpha1(APonderationPath)))
	APonderationPath = alpha10(APonderationPath);

      TCoordinate * ponderationCoefs = new TCoordinate[ponderationNbVertices];

      // Récupération des ordonnées:
      {
	int i = 0;
	for (CDynamicCoverage01 cov(this, APonderationPath); cov.cont(); ++cov)
	  if (!isMarked(*cov, treated))
	    {
	      ponderationCoefs[i++] = findVertex(*cov)->getY();
	      markOrbit(*cov, ORBIT_1, treated);
	    }

	assert(i==ponderationNbVertices);
	unmarkOrbit(APonderationPath, ORBIT_01, treated);
      }

      // Interpolation:
      for (int pathI=0; pathI<pathNbVertices; ++pathI)
	if (ponderationNbVertices==1)
	  pathCoefs[pathI] = ponderationCoefs[0];
	else
	  {
	    float t = ((float) pathI*ponderationNbVertices)/pathNbVertices;

	    int i = (int) t;

	    pathCoefs[pathI] = ponderationCoefs[i];

	    if (!isZero(t-i))
	      pathCoefs[pathI] +=
		(t-i)*(ponderationCoefs[i+1]-ponderationCoefs[i]);
	  }

      delete [] ponderationCoefs;
    }

  // Affectation des plongements:
  CDynamicCoverage01 path(this, APath);

  // On se place sur l'extrémité de la première arête:
  if (isFree0(*path))
    ++path;

  if (path.cont())
    ++path;

  int pathIndex     = pathInitialPos;
  int pathDirection = +1;

  while (path.cont())
    {
      current = ADim==1 ? alpha1012(current) : alpha210123(current);

      // Pour chaque arête du chemin:
      if (alpha1(*path)!=APath)
	{
	  assert(!isFree0(*path));

	  // Calcul de la matrice de transformation:
	  CVertex & vertex1 = * findVertex(alpha0(*path));
	  CVertex & vertex2 = * findVertex(*path);

	  CVertex vertex0, vertex3 ;

	  if (isFree1(alpha0(*path)) || isFree0(alpha01(*path)))
	    vertex0 = vertex1 - (vertex2 - vertex1);
	  else
	    vertex0 = * findVertex(alpha010(*path));

	  if (isFree1(*path) || isFree0(alpha1(*path)))
	    vertex3 = vertex2 + (vertex2 - vertex1);
	  else
	    vertex3 = * findVertex(alpha10(*path));

	  CVertex vector0 = vertex1 - vertex0;
	  CVertex vector1 = vertex2 - vertex1;
	  CVertex vector2 = vertex3 - vertex2;

	  CVertex axis0   = vector0 * vector1;
	  CVertex axis1   = vector1 * vector2;

	  if (axis0.isNull()) axis0 = OZ;
	  if (axis1.isNull()) axis1 = OZ;

	  TCoordinate angle0 = CGeometry::getAngle(vector0, vector1);
	  TCoordinate angle1 = CGeometry::getAngle(vector1, vector2);

	  CTransformationMatrix matrix;

	  if (AScaleExtrudedCells)
	    {
	      TCoordinate cos0 = dCos(angle0/2);

	      if (isZero(cos0))
		cos0 = 1;

	      matrix.axialScale(vertex1, axis0, cos0);
	    }

	  if (ARotateExtrudedCells)
	    matrix.rotate(vertex1, axis0, angle0/2);

	  matrix.translate(vector1);

	  if (ARotateExtrudedCells)
	    matrix.rotate(vertex2, axis1, angle1/2);

	  if (AScaleExtrudedCells)
	    {
	      TCoordinate cos1 = dCos(angle1/2);

	      if (isZero(cos1))
		cos1 = 1;

	      matrix.axialScale(vertex2, axis1, 1/cos1);
	    }

	  // Pondération du rayon de la section:
	  if (APonderationPath!=NULL)
	    {
	      TCoordinate coef = 1;

	      if (!isZero(pathCoefs[pathIndex]))
		coef /= pathCoefs[pathIndex];

	      pathIndex += pathDirection;
	      coef *= pathCoefs[pathIndex];
	      matrix.scale(vertex2, coef);
	    }

	  // Utilisation de cette matrice pour plonger les sommets de la face:
	  CCoverage * cov = getDynamicCoverage(current, ORBIT_INF[ADim]);
	
	  for (; cov->cont(); ++(*cov))
	    if (!isMarked(**cov, treated))
	      {
		markOrbit(**cov, AND_ORBIT(ORBIT_VERTEX, ORBIT_INF[ADim]),
			  treated);

		if (findVertex(**cov)==NULL)
		  {
		    CVertex result;
		    CDart* prev =
		      ADim==1 ? alpha2101(**cov) : alpha321012(**cov);

		    matrix.applyOn(* findVertex(prev), result);

		    setVertex(**cov, result);
		  }
	      }

	  delete cov;

	  unmarkOrbit(current, ORBIT_INF[ADim], treated);
	}

      // Passage à l'arête suivante du chemin:
      ++path; if (!path.cont()) break;

      if (isFree0(*path))
	{ ++path; if (!path.cont()) break; }

      if (path.prevOperationType()==OP_JUMP)
	{
	  current = alpha(ADart, ADim+1);

	  pathIndex = pathInitialPos;
	  pathDirection = - pathDirection;
	}

      ++path;
    }

  freeMark(treated);
  delete [] pathCoefs;

  return returned;
}
//******************************************************************************
int CGMapVertex::extrudeByPathMarkedCells(int AMarkNumber, int ADim,
					  CDart* APath,
					  bool AExtrusionWithAdjacentSews,
					  int AInitialTranslateExtrudedCells,
					  int AInitialRotateExtrudedCells,
					  bool ARotateExtrudedCells,
					  bool AScaleExtrudedCells,
					  CDart* APonderationPath)
{
  assert(APath!=NULL);
  assert(1<=ADim && ADim<=2);
  assert(0<=AInitialTranslateExtrudedCells
	 && AInitialTranslateExtrudedCells<=2);
  assert(0<=AInitialRotateExtrudedCells
	 && AInitialRotateExtrudedCells<=2);
  
  
  CVertex pathOrigin , pathDirection ;
  CVertex cellsOrigin, cellsDirection;

  int treated = getNewMark();
  int toExtrude = getNewMark();
  int nbExtruded = 0;

  if (isFree0(APath))
    {
      if (isFree1(APath))
	return 0;

      APath = alpha1(APath);
    }

  CDynamicCoverageAll it(this);

  // Repérage des cellules qui peuvent être extrudées:
  if (AInitialRotateExtrudedCells==0)
    markIncidentCells(ORBIT_INF[ADim], AMarkNumber, toExtrude);
  else
    {
      halfMarkIncidentCells(ORBIT_INF[ADim], AMarkNumber, toExtrude);

      for (it.reinit(); it.cont(); ++it)
	if (!isMarked(*it, treated))
	  {
	    if (isMarked(*it, toExtrude) && !isFree0(*it) &&
		isMarked(alpha0(*it), toExtrude))
	      {
		markOrbit(*it, ORBIT_INF[ADim], treated);
		unmarkOrbit(*it, ORBIT_INF[ADim], toExtrude);
	      }
	    else
	      setMark(*it, treated);
	  }

      negateMaskMark(treated);
    }

  for (it.reinit(); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, toExtrude))
	  {
	    markOrbit(*it, ORBIT_CELL[ADim], treated);

	    if (!canExtrudeByPath(*it, ADim, APath))
	      unmarkOrbit(*it, ORBIT_CELL[ADim], toExtrude);
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);

  // Calcul des positions initiales du chemin et des cellules:
  if (AInitialTranslateExtrudedCells!=0 || AInitialRotateExtrudedCells!=0)
    {
      // Barycentres:
      pathOrigin = *findVertex(APath);

      markIncidentCells(ORBIT_CELL[ADim], toExtrude, treated);
      cellsOrigin = barycenter(treated);
      unmarkAll(treated);

      // Vecteurs directeurs:
      if (AInitialRotateExtrudedCells!=0)
	{
	  // Vecteur directeur du chemin:
	  CVertex vector1 = * findVertex(alpha0(APath)) - pathOrigin;

	  if (vector1.isNull())
	    vector1 = OZ;
	  else
	    vector1 /= vector1.norm();

	  if (isFree1(APath) || isFree0(alpha1(APath)))
	    pathDirection = vector1;
	  else
	    {
	      CVertex vector0 = pathOrigin - * findVertex(alpha10(APath));

	      if (vector0.isNull())
		vector0 = OZ;
	      else
		vector0 /= vector0.norm();

	      pathDirection = (vector0 + vector1) / 2;
	    }

	  if (pathDirection.isNull())
	    pathDirection = OZ;

	  // Vecteur normal des cellules à extruder:
	  int nbCells = 0;

	  for (it.reinit(); it.cont(); ++it)
	    if (!isMarked(*it, treated))
	      {
		if (isMarked(*it, toExtrude))
		  {
		    markOrbit(*it, ORBIT_CELL[ADim], treated);
		    ++nbCells;
		    cellsDirection +=
		      /* cellNormalVector(ADim, *it); */
		      faceNormalVector(*it);
		  }
		else
		  setMark(*it, treated);
	      }

	  if (nbCells>0)
	    cellsDirection /= nbCells;

	  if (cellsDirection.isNull())
	    cellsDirection = ADim==0 ? OX : ADim==1 ? OY : OZ;

	  negateMaskMark(treated);
	}
    }

  // Orientation initiale du chemin par rapport aux cellules:
  if (AInitialRotateExtrudedCells!=0)
    {
      if (AInitialRotateExtrudedCells==1)
	{
	  // Rotation des cellules à extruder:
	  CTransformationMatrix matrix;
	  matrix.orientate(cellsOrigin, cellsDirection, pathDirection);

	  markIncidentCells(ORBIT_CC, toExtrude, treated);
	  applyMatrix(matrix, treated);
	  unmarkAll(treated);
	}
      else
	{
	  // Rotation du chemin d'extrusion:
	  CTransformationMatrix matrix;
	  matrix.orientate(pathOrigin, pathDirection, cellsDirection);
	  applyMatrix(matrix, APath, ORBIT_CC);
	}
    }

  // Positionnement initial du chemin par rapport aux cellules:
  if (AInitialTranslateExtrudedCells!=0)
    {
      if (AInitialTranslateExtrudedCells==1)
	{
	  // Translation des cellules à extruder:
	  markIncidentCells(ORBIT_CC, toExtrude, treated);
	  translate(treated, pathOrigin - cellsOrigin);
	  unmarkAll(treated);
	}
      else
	{
	  // Translation du chemin d'extrusion:
	  markOrbit(APath, ORBIT_CC, treated);
	  translate(treated, cellsOrigin - pathOrigin);
	  unmarkOrbit(APath, ORBIT_CC, treated);
	}
    }

  // Extrusion:
  int extruded = getNewMark();

  int directInfoIndex = getNewDirectInfo();

  for (it.reinit(); it.cont(); ++it)
    setDirectInfo(*it, directInfoIndex, alpha(*it, ADim));

  for (it.reinit(); it.cont(); ++it)
    if (isMarked(*it, toExtrude))
      {
	unmarkOrbit(*it, ORBIT_CELL[ADim], toExtrude);
	extrudeByPath(*it, ADim, APath, AExtrusionWithAdjacentSews,
		      ARotateExtrudedCells, AScaleExtrudedCells,
		      APonderationPath,
		      extruded, directInfoIndex);
	++nbExtruded;
      }

  freeDirectInfo(directInfoIndex);

  freeMark(treated);

  unmarkAll(extruded);
  freeMark(extruded);

  unmarkAll(toExtrude);
  freeMark(toExtrude);

  // Replacement du chemin à sa position initiale:
  if (AInitialTranslateExtrudedCells==2)
    {
      CTransformationMatrix matrix;
      matrix.translate(pathOrigin - cellsOrigin);
      applyMatrix(matrix, APath, ORBIT_CC);
    }

  // Replacement du chemin dans sa direction initiale:
  if (AInitialRotateExtrudedCells==2)
    {
      CTransformationMatrix matrix;
      matrix.orientate(pathOrigin, cellsDirection, pathDirection);
      applyMatrix(matrix, APath, ORBIT_CC);
    }

  return nbExtruded;
}
//******************************************************************************
CDart* CGMapVertex::createRevolutionPath(const CVertex & AAxeVertex,
					 const CVertex & AAxeDirection,
					 const CVertex & AFirstVertex,
					 TCoordinate AAngle, int ANbEdges)
{
  assert(!AAxeDirection.isNull());
  assert(ANbEdges>=3);

  // Topologie:
  TCoordinate nbRounds = fabs(AAngle)/360.0;
  bool closed = isZero(nbRounds - floor(nbRounds));

  CDart* extremity = CGMapGeneric::createRevolutionPath(closed, ANbEdges);

  // Plongements:
  CVertex vertex(AFirstVertex);

  CTransformationMatrix matrix;
  matrix.rotate(AAxeVertex, AAxeDirection, AAngle/ANbEdges);

  int nbVertices = closed ? ANbEdges : ANbEdges + 1;

  CDart* current = extremity;

  for (int i=0; i<nbVertices; ++i)
    {
      setVertex(current, vertex);
      matrix.applyOn(vertex);
      current= alpha01(current);
    }

  return extremity;
}
//******************************************************************************
CDart* CGMapVertex::extrudeByRevolution(CDart* ADart, int ADim,
					const CVertex & AAxeVertex,
					const CVertex & AAxeDirection,
					const CVertex & AFirstVertex,
					TCoordinate AAngle, int ANbEdges,
					bool AExtrusionWithAdjacentSews,
					bool ARotateExtrudedCells,
					bool AScaleExtrudedCells,
					CDart* APonderationPath,
					int AMarkExtruded, int ADirectInfoIndex)
{
#ifndef NDEBUG
  TCoordinate nbRounds = fabs(AAngle)/360.0;
  bool closed = isZero(nbRounds - floor(nbRounds));

  assert(canExtrudeByRevolution(ADart, ADim, closed));
  assert(ANbEdges>=3);
  assert(!AAxeDirection.isNull());
#endif

  // 1) Création du chemin:
  CDart* path = createRevolutionPath(AAxeVertex, AAxeDirection,
				     AFirstVertex, AAngle, ANbEdges);

  // 2) Extrusion:
  CDart* result = extrudeByPath(ADart, ADim, path,
				AExtrusionWithAdjacentSews,
				ARotateExtrudedCells, AScaleExtrudedCells,
				APonderationPath,
				AMarkExtruded, ADirectInfoIndex);

  // 3) Destruction du chemin:
  destroyRevolutionPath(path);

  return result;
}
//******************************************************************************
int CGMapVertex::
extrudeByRevolutionMarkedCells(int AMarkNumber, int ADim,
			       const CVertex & AAxeVertex,
			       const CVertex & AAxeDirection,
			       TCoordinate AAngle, int ANbEdges,
			       bool AExtrusionWithAdjacentSews,
			       int AInitialTranslateExtrudedCells,
			       int AInitialRotateExtrudedCells,
			       bool ARotateExtrudedCells,
			       bool AScaleExtrudedCells,
			       CDart* APonderationPath)
{
  assert(ANbEdges>=3);
  assert(!AAxeDirection.isNull());
  assert(0<=AInitialTranslateExtrudedCells
	 && AInitialTranslateExtrudedCells<=2);
  assert(0<=AInitialRotateExtrudedCells
	 && AInitialRotateExtrudedCells<=2);

  TCoordinate nbRounds = fabs(AAngle)/360.0;
  bool closed = isZero(nbRounds - floor(nbRounds));

  int toExtrude = getNewMark();
  int treated = getNewMark();

  // 1) Repérage des cellules qui peuvent être extrudées:
  if (AInitialRotateExtrudedCells==0)
    markIncidentCells(ORBIT_INF[ADim], AMarkNumber, toExtrude);
  else
    {
      halfMarkIncidentCells(ORBIT_INF[ADim], AMarkNumber, toExtrude);

      for (CDynamicCoverageAll it(this); it.cont(); ++it)
	if (!isMarked(*it, treated))
	  {
	    if (isMarked(*it, toExtrude) && !isFree0(*it) &&
		isMarked(alpha0(*it), toExtrude))
	      {
		markOrbit(*it, ORBIT_INF[ADim], treated);
		unmarkOrbit(*it, ORBIT_INF[ADim], toExtrude);
	      }
	    else
	      setMark(*it, treated);
	  }

      negateMaskMark(treated);
    }

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, toExtrude))
	  {
	    markOrbit(*it, ORBIT_CELL[ADim], treated);

	    if (!canExtrudeByRevolution(*it, ADim, closed))
	      unmarkOrbit(*it, ORBIT_CELL[ADim], toExtrude);
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);

  // 2) Calcul du barycentre des cellules:
  markIncidentCells(ORBIT_CELL[ADim], toExtrude, treated);
  CVertex bary = barycenter(treated);
  unmarkAll(treated);
  freeMark(treated);
  freeMark(toExtrude);

  // 3) Création du chemin:
  CDart* path = createRevolutionPath(AAxeVertex, AAxeDirection,
				     bary, AAngle, ANbEdges);

  // 4) Extrusion:
  int result = extrudeByPathMarkedCells(AMarkNumber, ADim, path,
					AExtrusionWithAdjacentSews,
					0, AInitialRotateExtrudedCells,
					ARotateExtrudedCells,
					AScaleExtrudedCells,
					APonderationPath);

  // 5) Destruction du chemin:
  destroyRevolutionPath(path);

  return result;
}
//******************************************************************************
