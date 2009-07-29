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
#include "g-map-generic.hh"
using namespace GMap3d;
//******************************************************************************
bool CGMapGeneric::canExtrudeHalfCell(CDart* ADart, int ADim,
				      int AMarkToExtrude)
{
  assert(ADart!=NULL);
  assert(ADim>=0 && ADim<=2);

  // Il faut vérifier que tous les brins de l'orbite (ADart, ORBIT_INF[ADim])
  // sont ADim-libres:

  switch (ADim)
    {
    case 0:
      return
	isFree0(ADart) ||
	(AMarkToExtrude>=0 && isMarked(alpha0(ADart), AMarkToExtrude));

    case 1:
      return
	(isFree1(ADart) ||
	 (AMarkToExtrude>=0 && isMarked(alpha1 (ADart), AMarkToExtrude))) &&
	(isFree1(alpha0(ADart)) ||
	 (AMarkToExtrude>=0 && isMarked(alpha01(ADart), AMarkToExtrude))) ;

    case 2:
      for (CDynamicCoverage01 it(this, ADart); it.cont(); ++it)
	if (!isFree2(*it) &&
	    (AMarkToExtrude<0 || !isMarked(alpha2(*it), AMarkToExtrude)))
	  return false;

      return true;
    }

  return false; // Pour éviter un warning à la compilation...
}
//******************************************************************************
bool CGMapGeneric::canExtrudeCell(CDart* ADart, int ADim)
{
  assert(ADart!=NULL);
  assert(ADim>=0 && ADim<=2);

  return isFree(ADart, ADim+1) || canExtrudeHalfCell(ADart, ADim);
}
//******************************************************************************
int CGMapGeneric::getExtrusionDimension(CDart* ADart)
{
  assert(ADart!=NULL);

  for (int dim=0; dim<=2; ++dim)
    if (canExtrudeHalfCell(ADart, dim))
      return dim;

  if (canExtrudeCell(ADart, 2))
    return 2;

  return 3;
}
//******************************************************************************
CDart* CGMapGeneric::topoExtrude(CDart* ADart, int ADim,
				 bool AExtrusionWithAdjacentSews,
				 int AMarkExtruded, int ADirectInfoIndex)
{
  assert(ADart!=NULL);
  assert(ADim>=0 && ADim<=2);
  assert(canExtrudeCell(ADart, ADim));

  CDart* initial = ADart;
  CDart* halfCell;
  CDart* opposite=NULL;

  // Duplication si nécessaire de la cellule à extruder:
  if (canExtrudeHalfCell(ADart, ADim))
    halfCell = ADart;
  else
    {
      switch (ADim)
	{
	case 0:
	  {
	    CDynamicCoverage3 it(this, ADart);

	    // Duplication sommet:
	    for (; it.cont(); ++it)
	      linkAlpha1(*it, addMapDart());

	    for (it.reinit(); it.cont(); ++it)
	      {
		if (!isFree3(*it) && isFree3(alpha1(*it)))
		  linkAlpha3(alpha1(*it), alpha31(*it));
	      }
	  }
	  break;
	
	case 1:
	  {
	    CDynamicCoverage0 it(this, ADart);

	    // Duplication arête:
	    for (; it.cont(); ++it)
	      linkAlpha2(*it, addMapDart());

	    for (it.reinit(); it.cont(); ++it)
	      {
		if (!isFree0(*it) && isFree0(alpha2(*it)))
		  linkAlpha0(alpha2(*it), alpha02(*it));
	      }
	  }
	  break;
	
	case 2:
	  {
	    CDynamicCoverage01 it(this, ADart);

	    // Duplication face:
	    for (; it.cont(); ++it)
	      linkAlpha3(*it, addMapDart());
	
	    for (it.reinit(); it.cont(); ++it)
	      {
		if (!isFree0(*it) && isFree0(alpha3(*it)))
		  linkAlpha0(alpha3(*it), alpha03(*it));
	
		if (!isFree1(*it) && isFree1(alpha3(*it)))
		  linkAlpha1(alpha3(*it), alpha13(*it));
	      }
	  }
	  break;
	}

      halfCell = alpha(ADart, ADim+1);
    }

  // Extrusion:
  assert(canExtrudeHalfCell(halfCell, ADim));

  switch (ADim)
    {
    case 0:
      {
	CDynamicCoverage3 it(this, halfCell);

	// Extrusion sommet:
	for (; it.cont(); ++it)
	  linkAlpha0(*it, addMapDart());

	for (it.reinit(); it.cont(); ++it)
	  if (!isFree3(*it) && isFree3(alpha0(*it)))
	    linkAlpha3(alpha0(*it), alpha30(*it));

	opposite = alpha0(halfCell);
      }
      break;

    case 1:
      {
	// Extrusion arête:
	for (CDynamicCoverage0 it(this, halfCell); it.cont(); ++it)
	  {
	    CDart* d1 = addMapDart();
	    CDart* d2 = addMapDart();
	    CDart* opposite = addMapDart();

	    linkAlpha0(d1, d2);
	    linkAlpha1(*it, d1);
	    linkAlpha1(d2, opposite);

	    if (it.prevOperationType()!=OP_NONE)
	      linkAlpha0(opposite, alpha0101(*it));
	  }

	opposite = alpha101(halfCell);
      }
      break;

    case 2:
      {
	// Extrusion face:
	for (CDynamicCoverage01 it(this, halfCell); it.cont(); ++it)
	  {
	    CDart* d0 = addMapDart();
	    CDart* d1 = addMapDart();
	    CDart* d2 = addMapDart();
	    CDart* d3 = addMapDart();
	    CDart* opposite = addMapDart();

	    linkAlpha0(d1, d2);
	    linkAlpha1(d1, d0); linkAlpha2(d0, *it);
	    linkAlpha1(d2, d3); linkAlpha2(d3, opposite);

	    if (!isFree0(*it) && !isFree2(alpha0(*it)))
	      {
		linkAlpha0(d0, alpha02(*it));
		linkAlpha0(d3, alpha02101(*it));
		linkAlpha0(opposite, alpha02(d3));
	      }

	    if (!isFree1(*it) && !isFree2(alpha1(*it)))
	      {
		linkAlpha2(d1, alpha121(*it));
		linkAlpha2(d2, alpha20(d1));
		linkAlpha1(opposite, alpha212(d2));
	      }
	  }

	opposite = alpha21012(halfCell);
      }
      break;
    }

  if (AExtrusionWithAdjacentSews && ADim>0 && AMarkExtruded>=0)
    {
      // Couture avec les cellules voisines:
      CCoverage * cov = getDynamicCoverage(initial, ORBIT_INF[ADim]);

      for (; cov->cont(); ++(*cov))
	{
	  CDart* first = **cov;
	  CDart* semiFirst = initial==halfCell ? first : alpha(first, ADim+1);
	
	  if (isFree(alpha(semiFirst, ADim), ADim+1))
	    {
	      CDart* second =
		ADirectInfoIndex < 0
		? alpha(**cov, ADim)
		: (CDart*) getDirectInfo(**cov, ADirectInfoIndex);

	      // Orbite ADim+1 o ADim:
	      bool finished = first==second;

	      while (!finished && !isMarked(second, AMarkExtruded))
		{
		  if (isFree(second, ADim+1))
		    finished = true;
		  else
		    {
		      second = alpha(second, ADim+1);

		      CDart* next =
			ADirectInfoIndex < 0
			? alpha(second, ADim)
			: (CDart*) getDirectInfo(second, ADirectInfoIndex);

		      if (next==second)
			finished = true;
		      else
			second = next;
		    }
		}
		
	      // ADim+1 Couture:
	      if (!finished)
		{
		  CDart* semiSecond =
		    initial==halfCell ? second : alpha(second, ADim+1);

		  assert(!isFree(semiSecond, ADim));
		  sew(alpha(semiFirst, ADim), alpha(semiSecond, ADim), ADim+1);
		}
	    }
	}
	
      delete cov;

      // Marquage de la cellule pour indiquer qu'elle alpha été extrudée:
      markOrbit(initial, ORBIT_INF[ADim], AMarkExtruded);
    }

  return opposite;
}
//******************************************************************************
CDart* CGMapGeneric::extrudeByNormal(CDart* ADart, int ADim,
				     bool AExtrusionWithAdjacentSews,
				     TCoordinate /*AExtrusionCoef*/,
				     int /*AMarkToExtrude*/, int AMarkExtruded)
{
  return topoExtrude(ADart, ADim, AExtrusionWithAdjacentSews, AMarkExtruded);
}
//******************************************************************************
int CGMapGeneric::extrudeByNormalMarkedCells(int AMarkNumber, int ADim,
					     bool AExtrusionWithAdjacentSews,
					     TCoordinate AExtrusionCoef)
{
  int selected = getNewMark();

  markCellsToBeExtrudedByNormal(ADim, AMarkNumber, selected);

  int nb = extrudeByNormalMarkedCells(selected, ADim, -1,
				      AExtrusionWithAdjacentSews,
				      AExtrusionCoef);

  unmarkAll(selected);
  freeMark(selected);

  return nb;
}
//******************************************************************************
int CGMapGeneric::
intuitiveExtrudeByNormalMarkedCells(int AMarkNumber,
				    bool AExtrusionWithAdjacentSews,
				    TCoordinate AExtrusionCoef)
{
  int nb = 0;
  int treated = getNewMark();
  int selected = getNewMark();


  markCellsToBeExtrudedByNormal(2, AMarkNumber, selected);

  nb+= extrudeByNormalMarkedCells(selected, 0, treated,
				  AExtrusionWithAdjacentSews,
				  AExtrusionCoef);
  nb+= extrudeByNormalMarkedCells(selected, 1, treated,
				  AExtrusionWithAdjacentSews,
				  AExtrusionCoef);
  nb+= extrudeByNormalMarkedCells(selected, 2, treated,
				  AExtrusionWithAdjacentSews,
				  AExtrusionCoef);

  unmarkAll(treated);
  freeMark(treated);

  unmarkAll(selected);
  freeMark(selected);

  return nb;
}
//******************************************************************************
// Méthode protégée!
void CGMapGeneric::markCellsToBeExtrudedByNormal(int ADim,
						 int AMarkNumberSrce,
						 int AMarkNumberDest)
{
  markIncidentCells(ORBIT_INF[ADim], AMarkNumberSrce, AMarkNumberDest);
}
//******************************************************************************
// Méthode protégée!
int CGMapGeneric::extrudeByNormalMarkedCells(int AMarkNumber, int ADim,
					     int AMarkTreated,
					     bool AExtrusionWithAdjacentSews,
					     TCoordinate AExtrusionCoef)
{
  int nbExtruded = 0;
  int treated = getNewMark();
  int extruded = getNewMark();

  for (CDynamicCoverageAll it(this); it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber) &&
	    (AMarkTreated<0 || !isMarked(*it, AMarkTreated)))
	  {
	    if ((AMarkTreated<0 && canExtrudeCell(*it, ADim)) ||
		(AMarkTreated>=0 &&
		 (canExtrudeHalfCell(*it, ADim) ||
		  (ADim==2 && canExtrudeCell(*it, ADim)))))
	      {
		bool free = isFree(*it, ADim+1);

		extrudeByNormal(*it, ADim, 
				AExtrusionWithAdjacentSews,
				AExtrusionCoef,
				AMarkNumber, extruded);
		++nbExtruded;

		if (free && !isFree(*it, ADim+1))
		  {
		    markOrbit(*it, ORBIT_INF[ADim], treated);
		    markOrbit(alpha(*it, ADim+1), ORBIT_CELL[ADim+1], treated);

		    if (AMarkTreated>=0)
		      markOrbit(alpha(*it, ADim+1),
				ORBIT_CELL[ADim+1], AMarkTreated);
		  }
		else
		  {
		    markOrbit(*it, ORBIT_CELL[ADim+1], treated);

		    if (AMarkTreated>=0)
		      markOrbit(*it, ORBIT_CELL[ADim+1], AMarkTreated);
		  }
	      }
	    else
	      markOrbit(*it, ORBIT_CELL[ADim], treated);
	  }
	else
	  setMark(*it, treated);
      }

  unmarkAll(extruded);
  freeMark(extruded);

  negateMaskMark(treated);
  freeMark(treated);

  return nbExtruded;
}
//******************************************************************************
bool CGMapGeneric::canExtrudeByPath(CDart* ADart, int ADim, CDart* APath)
{
  assert(ADart!=NULL);
  assert(1<=ADim && ADim<=2);
  assert(APath!=NULL);

  bool extremity = isFree0(APath) || isFree1(APath) || isFree0(alpha1(APath));

  // Cas où le chemin est ouvert:
  if (extremity)
    return canExtrudeCell(ADart, ADim);

  // Cas où le chemin est fermé:
  return isFree(ADart, ADim+1);
}
//******************************************************************************
CDart* CGMapGeneric::extrudeByPath(CDart* ADart, int ADim, CDart* APath,
				   bool AExtrusionWithAdjacentSews,
				   int AMarkExtruded, int ADirectInfoIndex)
{
  assert(canExtrudeByPath(ADart, ADim, APath));

  CDart* current = ADart;
  bool jumped = isFree0(APath) || isFree1(APath) || isFree0(alpha1(APath));

  // Déconnexion de la cellule dans le cas où le chemin est fermé:
  // (ou si on est au milieu du chemin)
  if (!jumped)
    {
      CCoverage * cov = getDynamicCoverage(ADart, ORBIT_INF[ADim]);

      for (; cov->cont(); ++(*cov))
	if (!isFree(**cov, ADim))
	  unsew(**cov, ADim);

      delete cov;
    }

  // On se place sur l'extrémité de la première arête:
  CDynamicCoverage01 path(this, APath);

  if (isFree0(*path))
    ++path;

  if (path.cont())
    ++path;

  while (path.cont())
    {
      // Extrusion:
      assert(!isFree0(*path));
      current = topoExtrude(current, ADim, AExtrusionWithAdjacentSews,
			    AMarkExtruded,
			    current==ADart ? ADirectInfoIndex : -1);

      // Passage à l'arête suivante:
      ++path; if (!path.cont()) break;

      if (isFree0(*path))
	{ ++path; if (!path.cont()) break; }

      if (path.prevOperationType()==OP_JUMP)
	{
	  current = ADart;
	  jumped = true;
	}

      ++path;
    }

  // Fermeture (dans le cas où le chemin est un circuit):
  if (!jumped)
    topoSew(current, ADart, ADim+1);

  return current;
}
//******************************************************************************
int CGMapGeneric::extrudeByPathMarkedCells(int AMarkNumber, int ADim,
					   CDart* APath,
					   bool AExtrusionWithAdjacentSews)
{
  assert(APath!=NULL);
  assert(1<=ADim && ADim<=2);

  int nbExtruded = 0;
  int treated = getNewMark();
  int toExtrude = getNewMark();
  int extruded = getNewMark();

  int directInfoIndex = getNewDirectInfo();

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    setDirectInfo(*it, directInfoIndex, alpha(*it, ADim));

  markIncidentCells(ORBIT_CELL[ADim], AMarkNumber, toExtrude);

  for (it.reinit(); it.cont(); ++it)
    if (!isMarked(*it, treated) && isMarked(*it, AMarkNumber))
      {
	markOrbit(*it, ORBIT_CELL[ADim], treated);

	if (canExtrudeByPath(*it, ADim, APath))
	  {
	    extrudeByPath(*it, ADim, APath, AExtrusionWithAdjacentSews,
			  extruded, directInfoIndex);
	    ++nbExtruded;
	  }
      }

  freeDirectInfo(directInfoIndex);

  unmarkAll(treated);
  freeMark(treated);

  unmarkAll(toExtrude);
  freeMark(toExtrude);

  unmarkAll(extruded);
  freeMark(extruded);

  return nbExtruded;
}
//******************************************************************************
bool CGMapGeneric::canExtrudeByRevolution(CDart* ADart, int ADim, bool AClosed)
{
  assert(ADart!=NULL);
  assert(1<=ADim && ADim<=2);

  // Cas où le chemin est ouvert:
  if (!AClosed)
    return canExtrudeCell(ADart, ADim);

  // Cas où le chemin est fermé:
  return isFree(ADart, ADim+1);
}
//******************************************************************************
CDart* CGMapGeneric::createRevolutionPath(bool AClosed, int ANbEdges)
{
  assert(ANbEdges>=3);

  CDart* extremity = createTopoPolygon(ANbEdges);

  if (!AClosed)
    topoUnsew1(extremity);

  return extremity;
}
//******************************************************************************
void CGMapGeneric::destroyRevolutionPath(CDart* ADart)
{
  assert(ADart!=NULL);
  assert(isIsolatedPolyline(ADart));

  for (CStaticCoverage01 cov(this, ADart); cov.cont(); ++cov)
    delMapDart(*cov);
}
//******************************************************************************
CDart* CGMapGeneric::extrudeByRevolution(CDart* ADart, int ADim,
					 bool AClosed, int ANbEdges,
					 bool AExtrusionWithAdjacentSews,
					 int AMarkExtruded /*-1*/,
					 int ADirectInfoIndex /*-1*/)
{
  assert(canExtrudeByRevolution(ADart, ADim, AClosed));
  assert(ANbEdges>=3);

  // 1) Création du chemin:
  CDart* path = CGMapGeneric::createRevolutionPath(AClosed, ANbEdges);

  // 2) Appel à la méthode 'extrudeByPath':
  CDart* result = CGMapGeneric::extrudeByPath(ADart, ADim, path,
					      AExtrusionWithAdjacentSews,
					      AMarkExtruded, ADirectInfoIndex);

  // 3) Destruction du chemin:
  destroyRevolutionPath(path);

  return result;
}
//******************************************************************************
int CGMapGeneric::extrudeByRevolutionMarkedCells(int AMarkNumber, int ADim,
						 bool AClosed, int ANbEdges,
						 bool AExtrusionWithAdjacentSews)
{
  assert(1<=ADim && ADim<=2);
  assert(ANbEdges>=3);

  // 1) Création du chemin:
  CDart* path = CGMapGeneric::createRevolutionPath(AClosed, ANbEdges);

  // 2) Extrusion:
  int nbExtruded = extrudeByPathMarkedCells(AMarkNumber, ADim, path,
					    AExtrusionWithAdjacentSews);

  // 3) Destruction du chemin:
  destroyRevolutionPath(path);

  return nbExtruded;
}
//******************************************************************************
