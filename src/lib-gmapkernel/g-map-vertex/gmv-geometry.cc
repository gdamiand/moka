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
CVertex CGMapVertex::basicBarycenter(CDart * ADart, TOrbit AOrbit,
				     int AOrbitMark, int ATreatedMark,
				     int ADirectInfoVertex)
{
  assert(ADart!=NULL);
  assert(AOrbit==ORBIT_012 || AOrbit==ORBIT_123 || AOrbit==ORBIT_0123);
  assert(AOrbitMark  >=0 && AOrbitMark  <NB_MARKS);
  assert(ATreatedMark>=0 && ATreatedMark<NB_MARKS);
  assert(ADirectInfoVertex<NB_DIRECT_INFO);

  CVertex bary(ORIGIN);
  int n = 0;

  CCoverage * cov = getBasicDynamicCoverage(ADart, AOrbit, AOrbitMark);
  assert(cov!=NULL);

  TOrbit orbit = AND_ORBIT(ORBIT_VERTEX, AOrbit);

  for (; cov->cont(); ++(*cov))
    if (!isMarked(**cov, ATreatedMark))
      {
	bary +=
	  ADirectInfoVertex>=0
	  ? * getDirectInfoAsAttributeVertex(**cov, ADirectInfoVertex)
	  : * findVertex(**cov);
	
	++n;
	markOrbit(**cov, orbit, ATreatedMark);
      }

  delete cov;

  return n==0 ? ORIGIN : bary/n;
}
//******************************************************************************
CVertex CGMapVertex::barycenter(CDart * ADart, TOrbit AOrbit,
				int ADirectInfoVertex)
{
  assert(ADart!=NULL);

  CVertex bary(ORIGIN);
  int n = 0;
  int treated = getNewMark();

  CCoverage * cov;
  TOrbit orbit;

  if (AOrbit>=ORBIT_BORDER_0 && AOrbit<=ORBIT_BORDER_3)
    {
      cov = getStaticCoverage(ADart, AOrbit);
      orbit = ORBIT_VERTEX;
    }
  else
    {
      cov = getDynamicCoverage(ADart, AOrbit);
      orbit = AND_ORBIT(AOrbit, ORBIT_VERTEX);
    }

  assert(cov!=NULL);

  for (; cov->cont(); ++(*cov))
    if (!isMarked(**cov, treated))
      {
	bary +=
	  ADirectInfoVertex>=0
	  ? * getDirectInfoAsAttributeVertex(**cov, ADirectInfoVertex)
	  : * findVertex(**cov);
	
	++n;
	markOrbit(**cov, orbit, treated);
      }

  // Démarquage:
  if (AOrbit>=ORBIT_BORDER_0 && AOrbit<=ORBIT_BORDER_3)
    {
      for (cov->reinit(); cov->cont(); ++(*cov))
	if (isMarked(**cov, treated))
	  unmarkOrbit(**cov, ORBIT_VERTEX, treated);
    }
  else
    unmarkOrbit(ADart, AOrbit, treated);

  delete cov;

  freeMark(treated);

  return n==0 ? ORIGIN : bary/n;
}
//******************************************************************************
CVertex CGMapVertex::barycenter(int AMarkNumber)
{
  CVertex bary(ORIGIN);
  int n=0;

  int treated= getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    {
      if (!isMarked(*it, treated))
	{
	  if (isMarked(*it, AMarkNumber))
	    {
	      bary += * findVertex(*it);
	      ++n;
	      markOrbit(*it, ORBIT_VERTEX, treated);
	    }
	  else
	    setMark(*it, treated);
	}
    }

  negateMaskMark(treated);
  freeMark(treated);

  return n==0 ? ORIGIN : bary/n;
}
//******************************************************************************
CVertex CGMapVertex::directInfoBarycenter(int ADirectInfoVertex)
{
  CVertex bary(ORIGIN);
  int n=0;

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (getDirectInfo(*it, ADirectInfoVertex) != NULL)
      {
	bary += * getDirectInfoAsVertex(*it, ADirectInfoVertex);
	++n;
      }

  return n==0 ? ORIGIN : bary/n;
}
//******************************************************************************
void CGMapVertex::boundingBox(int AMarkNumber, CVertex & AMin, CVertex & AMax)
{
  bool first = true;
  int treated = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (! isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    markOrbit(*it, ORBIT_VERTEX, treated);

	    CVertex & vertex = * findVertex(*it);
	
	    if (first)
	      {
		first = false;
		AMin = AMax = vertex;
	      }
	    else
	      {
		if (vertex.getX() < AMin.getX()) AMin.setX(vertex.getX());
		if (vertex.getY() < AMin.getY()) AMin.setY(vertex.getY());
		if (vertex.getZ() < AMin.getZ()) AMin.setZ(vertex.getZ());
		
		if (vertex.getX() > AMax.getX()) AMax.setX(vertex.getX());
		if (vertex.getY() > AMax.getY()) AMax.setY(vertex.getY());
		if (vertex.getZ() > AMax.getZ()) AMax.setZ(vertex.getZ());
	      }
	  }
	else
	  setMark(*it, treated);
      }

  if (first == true)
    AMin = AMax = ORIGIN;

  negateMaskMark(treated);
  freeMark(treated);
}
//******************************************************************************
CVertex CGMapVertex::centerOfBoundingBox(int AMarkNumber)
{
  CVertex min, max;

  boundingBox(AMarkNumber, min, max);

  return (min + max) / 2;
}
//******************************************************************************
void CGMapVertex::boundingBox(CDart * ADart, TOrbit AOrbit,
			      CVertex & AMin, CVertex & AMax)
{
  assert(ADart != NULL);

  bool first = true;
  int treated = getNewMark();
  CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

  for (; cov->cont(); ++(*cov))
    if (! isMarked(**cov, treated))
      {
	markOrbit(**cov, AND_ORBIT(AOrbit, ORBIT_VERTEX), treated);

	CVertex & vertex = * findVertex(**cov);

	if (first)
	  {
	    first = false;
	    AMin = AMax = vertex;
	  }
	else
	  {
	    if (vertex.getX() < AMin.getX()) AMin.setX(vertex.getX());
	    if (vertex.getY() < AMin.getY()) AMin.setY(vertex.getY());
	    if (vertex.getZ() < AMin.getZ()) AMin.setZ(vertex.getZ());

	    if (vertex.getX() > AMax.getX()) AMax.setX(vertex.getX());
	    if (vertex.getY() > AMax.getY()) AMax.setY(vertex.getY());
	    if (vertex.getZ() > AMax.getZ()) AMax.setZ(vertex.getZ());
	  }
      }

  assert(first == false);

  unmarkOrbit(ADart, AOrbit, treated);
  freeMark(treated);
  delete cov;
}
//******************************************************************************
CVertex CGMapVertex::centerOfBoundingBox(CDart * ADart, TOrbit AOrbit)
{
  assert(ADart != NULL);
  CVertex min, max;

  boundingBox(ADart, AOrbit, min, max);

  return (min + max) / 2;
}
//******************************************************************************
#define PUSH(STACK,V) \
( \
  (STACK)[1]= (STACK)[0], \
  (STACK)[0]= (V) \
)

#define DISTANCE(STACK) \
( \
  (*(STACK)[0] - *(STACK)[1]).norm() \
)
//******************************************************************************
// Méthode protégée!
TCoordinate CGMapVertex::orbitLength(CDart * ADart, TOrbit AOrbit)
{
  assert(ADart!=NULL);
  assert(AOrbit==ORBIT_0  || AOrbit==ORBIT_BORDER_1 ||
	 AOrbit==ORBIT_01 || AOrbit==ORBIT_BORDER_2);

  TCoordinate length=0;

  // Dans le cas d'une orbite ouverte (face ouverte, 2-bord ouvert),
  // on commence à partir du premier brin 0-cousu.
  {
    CCoverage * firstIt = getDynamicCoverage(ADart, AOrbit);

    while (firstIt->cont() && firstIt->prevOperationType()!=OP_JUMP)
      ADart= (*firstIt)++;

    delete firstIt;
  }

  {
    CCoverage * it = getDynamicCoverage(ADart, AOrbit);
    CVertex * stack[2] = { NULL, NULL };

    if (isFree0(**it))
      ++(*it);

    // On récupère le premier sommet et on avance d'un brin seulement
    // (pour à la fin du parcours pouvoir récupérer le dernier sommet)
    PUSH(stack, findVertex((*it)++));

    // On parcourt le reste de l'orbite en avançant de 2 brins à chaque fois:
    while (it->cont())
      {
	PUSH(stack, findVertex((*it)++));
	length+= DISTANCE(stack);
	
	if (it->cont()) ++(*it);
      }

    delete it;
  }

  return length;
}
//******************************************************************************
#undef DISTANCE
#undef PUSH
//******************************************************************************
TCoordinate CGMapVertex::edgeLength(CDart * ADart)
{
  assert(ADart!=NULL);

  if (isFree0(ADart))
    return 0;

  return (*findVertex(ADart) - *findVertex(alpha0(ADart))).norm();
}
//******************************************************************************
TCoordinate CGMapVertex::facePerimeter(CDart * ADart)
{
  assert(ADart!=NULL);
  return orbitLength(ADart, ORBIT_01);
}
//******************************************************************************
TCoordinate CGMapVertex::border1Length(CDart * ADart)
{
  assert(ADart!=NULL);
  return orbitLength(ADart, ORBIT_BORDER_1);
}
//******************************************************************************
TCoordinate CGMapVertex::border2Length(CDart * ADart)
{
  assert(ADart!=NULL);
  return orbitLength(ADart, ORBIT_BORDER_2);
}
//******************************************************************************
#define PUSH(STACK,V) \
( \
  (STACK)[2]=(STACK)[1], \
  (STACK)[1]=(STACK)[0], \
  (STACK)[0]=(V) \
)

#define NORMAL(STACK) \
( \
  CGeometry::getNormalVector(*(STACK)[0] - *(STACK)[1] , \
                             *(STACK)[2] - *(STACK)[1] ) \
)
//******************************************************************************
// Méthode protégée!
CVertex CGMapVertex::orbitNormalVector(CDart * ADart, TOrbit AOrbit)
{
  assert(ADart!=NULL);
  assert(AOrbit==ORBIT_01 || AOrbit==ORBIT_BORDER_2);

  CVertex normal(ORIGIN);
  int nbEdges = 0;
  bool closed;

  // On teste d'abord si l'orbite est ouverte ou fermée:
  // (Sensé pour un 2-bord ou une face seulement...)
  if (isFree1(ADart))
    {
      if (isFree0(ADart))
	return OX;

      closed = false;
    }
  else
    {
      // On place ADart sur l'extrémité qui lui est opposée:
      CVertex * vertex0 = findVertex(ADart);
      CDart * current=NULL;
      CCoverage * cov = getDynamicCoverage(ADart, AOrbit);

      while (cov->cont())
	current = (*cov)++;

      closed = vertex0==findVertex(current);
      ADart = closed ? ADart : current;

      delete cov;
    }

  // Dans le cas d'une orbite ouverte (face ouverte, 2-bord ouvert),
  // on commence à partir du premier brin 0-cousu.
  // Si l'orbite est fermée, le brin de départ est quelconque.
  {
    CCoverage * it = getDynamicCoverage(ADart, AOrbit);
    CVertex * stack[3] = { NULL, NULL, NULL };
    CVertex * second;

    if (isFree0(**it))
      ++(*it);

    // On récupère le premier sommet et on avance d'un brin seulement
    // (pour à la fin du parcours pouvoir récupérer le dernier sommet)
    PUSH(stack, findVertex((*it)++));

    if (!it->cont())
      {
	delete it;
	return OX;
      }

    // On récupère le deuxième sommet:
    // (mémorisé pour le calcul du dernier produit vectoriel)
    PUSH(stack, second = findVertex((*it)++));

    if (it->cont())
      ++(*it);

    if (!it->cont())
      {
	delete it;
	return CGeometry::getNormalVector(*stack[0] - *stack[1]);
      }

    // On parcourt le reste de l'orbite en avançant de 2 brins à chaque fois:
    while (it->cont())
      {
	PUSH(stack, findVertex((*it)++));
	normal+= NORMAL(stack);
	++nbEdges;

	if (it->cont())
	  ++(*it);
      }

    delete it;

    // Si l'orbite est fermée, il reste à calculer le dernier produit vectoriel:
    if (closed)
      {
	PUSH(stack, second);
	normal+= NORMAL(stack);
	++nbEdges;
      }
  }

  if (normal.isNull())
    return OZ;

  assert(nbEdges>0);

  // Formule telle qu'un carré de côté 1 ait une normale de norme 1:
  return normal/(2*sqrt(static_cast<double>(nbEdges)));
}
//******************************************************************************
#undef NORMAL
#undef PUSH
//******************************************************************************
CVertex CGMapVertex::edgeVector(CDart * ADart)
{
  assert(ADart!=NULL);

  return * findVertex(alpha0(ADart)) - * findVertex(ADart);
}
//******************************************************************************
CVertex CGMapVertex::edgeNormalVector(CDart * ADart)
{
  assert(ADart!=NULL);

  CVertex vector = edgeVector(ADart);

  if (vector.isNull())
    return OX;

  return CGeometry::getNormalVector(vector);
}
//******************************************************************************
CVertex CGMapVertex::faceNormalVector(CDart * ADart)
{
  assert(ADart!=NULL);
  return orbitNormalVector(ADart, ORBIT_01);
}
//******************************************************************************
CVertex CGMapVertex::cellNormalVector(int ADim, CDart * ADart)
{
  assert(0<=ADim && ADim<=2);

  if (ADim==2)
    return faceNormalVector(ADart);

  if (ADim==1)
    return edgeNormalVector(ADart);

  return OX;
}
//******************************************************************************
CVertex CGMapVertex::border2NormalVector(CDart * ADart)
{
  assert(ADart!=NULL);
  return orbitNormalVector(ADart, ORBIT_BORDER_2);
}
//******************************************************************************
CVertex CGMapVertex::regionNormalVector(CDart * ADart, int ADim)
{
  assert(ADart!=NULL);
  assert(ADim>=0 && ADim<=2);

  /* Attention: dans tous les calculs on ne prend pas en compte les faces
   * 3-cousues car il faudrait comptabiliser un vecteur normal par demi-face, or
   * ces deux vecteurs sont opposés...
   */

  TOrbit orbitCell = AND_ORBIT(ORBIT_CELL[ADim], NEG_ORBIT(ORBIT_3));

  /* Si ADim==0, orbitCell = ORBIT_12
   * (Moyenne des vecteurs normaux des faces incidentes au sommet)
   *
   * Si ADim==1, orbitCell = ORBIT_02
   * (Moyenne des vecteurs normaux des faces incidentes à l'arête)
   *
   * Si ADim==3, orbtiCell = ORBIT_01
   * (Moyenne des vecteurs normaux des faces incidentes à la face)
   */

  CVertex normal = ORIGIN;
  int n = 0;

  int toTreat = getNewMark();

  halfMarkOrbit(ADart, orbitCell, toTreat);

  CCoverage * cov = getDynamicCoverage(ADart, orbitCell);

  for (; cov->cont(); ++(*cov))
    if (isMarked(**cov, toTreat))
      {
	if (ADim==2)
	  {
	    if (!isFree2(**cov))
	      {
		normal -= faceNormalVector(alpha(**cov, ADim));
		++n;
	      }
	  }
	else
	  {
	    normal += faceNormalVector(**cov);
	    ++n;
	  }

	unsetMark(**cov, toTreat);
      }

  delete cov;

  freeMark(toTreat);

  return normal/(n==0 ? 1 : n);
}
//******************************************************************************
CVertex CGMapVertex::regionNormalVector(CDart * ADart, int ADim,
					int AMarkNumber)
{
  assert(ADart!=NULL);
  assert(ADim>=0 && ADim<=2);
  assert(AMarkNumber>=0);

  CVertex normal = ORIGIN;
  int n = 0;

  int toTreat = getNewMark();

  markCopy(AMarkNumber, toTreat, ADart, ORBIT_CELL[ADim]);

  CCoverage * cov = getDynamicCoverage(ADart, ORBIT_CELL[ADim]);

  for (; cov->cont(); ++(*cov))
    if (isMarked(**cov, toTreat))
      {
	if (ADim==2)
	  {
	    if (!isFree2(**cov))
	      {
		normal -= faceNormalVector(alpha(**cov, ADim));
		++n;
	      }
	  }
	else
	  {
	    normal += faceNormalVector(**cov);
	    ++n;
	  }

	unsetMark(**cov, toTreat);
      }

  delete cov;

  freeMark(toTreat);

  return normal/(n==0 ? 1 : n);
}
//******************************************************************************
CVertex CGMapVertex::cellDimensionNormalVector(CDart * ADart, int ADim)
{
  assert(ADart!=NULL);
  assert(3 <= ADim && ADim <= 4);

  if (!isOrientable(ADart, ORBIT_CELL[ADim]))
    return ORIGIN;

  TOrbit pieceOfFace = AND_ORBIT(ORBIT_CELL[2], ORBIT_CELL[ADim]);

  int treated = getNewMark();
  int out = getNewMark();

  halfMarkOrbit(ADart, ORBIT_CELL[ADim], out);

  CCoverage * cov = getDynamicCoverage(ADart, ORBIT_CELL[ADim]);

  CVertex normal = ORIGIN;
  int nbFaces = 0;

  for (; cov->cont(); ++(*cov))
    if (!isMarked(**cov, treated) && isMarked(**cov, out))
      {
	markOrbit(**cov, pieceOfFace, treated);
	
	if (ADim==3 || isFree3(**cov))
	  {
	    normal += faceNormalVector(**cov);
	    ++nbFaces;
	  }
      }

  for (cov->reinit(); cov->cont(); ++(*cov))
    {
      unsetMark(**cov, treated);
      unsetMark(**cov, out);
    }

  delete cov;

  freeMark(treated);
  freeMark(out);

  if (nbFaces>0)
    normal /= nbFaces;

  return normal;
}
//******************************************************************************
