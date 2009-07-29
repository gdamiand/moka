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
// Fusion de 2 i-cellules incidentes à deux brins donnés.
//
// dim == 3: fusion de volumes ie 2-décousures et 2-coutures
// dim == 2: fusion de faces   ie 1-décousures et 1-coutures
// dim == 1: fusion d' arêtes  ie 0-décousures et 0-coutures
//
//******************************************************************************
int CGMapGeneric::getMergeDimension(CDart * ADart)
{
  assert(ADart!=NULL);

  if (!isFree3(ADart)) return 3;
  if (!isFree2(ADart)) return 2;
  if (!isFree1(ADart)) return 1;
  if (!isFree0(ADart)) return 0;

  return -1;
}
//******************************************************************************
bool CGMapGeneric::canMerge(CDart * ADart1, CDart * ADart2, int ADim)
{
  assert(ADart1!=NULL && ADart2!=NULL);
  assert(ADim>=1 || ADim<=3);

  // Il faut que les cellules de dimension ADim-1 incidentes à ADart1 et ADart2
  // soient de degré 1 ou 2, c'est-à-dire incidentes à 1 ou 2 cellules de
  // dimension ADim:

  if (isSameOrbit(ADart1, ADart2, ORBIT_CELL[ADim-1]))
    {
      if (degree(ADart1, ADim-1) > 2)
	return false;
    }
  else
    {
      if (degree(ADart1, ADim-1) + degree(ADart2, ADim-1) > 2)
	return false;
    }

  // Il faut vérifier que les brins des deux cellules peuvent être fusionnés
  // deux-à-deux:
  TOrbit halfCell = SUB_ORBIT(ORBIT_CC, ORBIT_I_IP1[ADim-1]);

  CCoverage * cov1 = getDynamicCoverage(ADart1, halfCell);
  CCoverage * cov2 = getDynamicCoverage(ADart2, halfCell);

  while (cov1->cont() && cov2->cont() &&
	 cov1->prevOperationType()==cov2->prevOperationType())
    {
      ++(*cov1);
      ++(*cov2);
    }

  bool ok = !cov1->cont() && !cov2->cont();

  delete cov1;
  delete cov2;

  return ok;
}
//******************************************************************************
void CGMapGeneric::merge(CDart * ADart1, CDart * ADart2, int ADim,
			 bool ADeleteDarts)
{
  assert(ADart1!=NULL && ADart2!=NULL);
  assert(ADim>=1 && ADim<=3);
  assert(canMerge(ADart1, ADart2, ADim));

  TOrbit halfCell = SUB_ORBIT(ORBIT_CC, ORBIT_I_IP1[ADim-1]);

  CCoverage * it1 = getStaticCoverage(ADart1, halfCell);
  CCoverage * it2 = getStaticCoverage(ADart2, halfCell);

  for (; it1->cont(); ++(*it1), ++(*it2))
    {
      bool bothLinked = !isFree(**it1,ADim-1) && !isFree(**it2,ADim-1);

      CDart * t1 = alpha(**it1, ADim-1);
      CDart * t2 = alpha(**it2, ADim-1);

      if (!isFree(t1,ADim-1)) unsew(t1,ADim-1);
      if (!isFree(t2,ADim-1)) unsew(t2,ADim-1);

      if (bothLinked)
	sew(t1,t2,ADim-1);
    }

  if (ADeleteDarts)
    for (it1->reinit(), it2->reinit(); it1->cont(); )
      {
  	delMapDart((*it1)++);
  	delMapDart((*it2)++);
      }

  delete it1;
  delete it2;
}
//******************************************************************************
// Méthode protégée!
int CGMapGeneric::isolateMarkedCells(int AMarkNumber, int ADim,
				     bool ADeleteDarts, bool AVerif,
				     int AMarkTreated)
{
  assert(ADim>=0 && ADim<=2);
  assert(AMarkTreated!=AMarkNumber);

  int nbIsolated = 0;
  int treated = getNewMark();
  int toDelete = 0;

  if (ADeleteDarts)
    toDelete = getNewMark();

  if (AMarkTreated>=0)
    markCopy(AMarkTreated, treated);

  CDynamicCoverageAll it(this);

  for (; it.cont(); ++it)
    if (!isMarked(*it, treated))
      {
	if (isMarked(*it, AMarkNumber))
	  {
	    bool possible =
	      !AVerif ||
	      (!isFree(*it, ADim+1) && canMerge(*it, alpha(*it, ADim+1), ADim+1));
	
	    markOrbit(*it, ORBIT_CELL[ADim], treated);
	
	    if (possible)
	      {
		if (ADeleteDarts)
		  markOrbit(*it, ORBIT_CELL[ADim], toDelete);

		if (AMarkTreated>=0)
		  for (int i = 0; i<2; ++i)
		    {
		      CDart * dart = i==0 ? *it : alpha(*it, ADim+1);

		      CCoverage * cov =
			getDynamicCoverage(dart, ORBIT_CELL[ADim+1]);

		      for (; cov->cont(); ++(*cov))
			if (!isMarked(**cov, AMarkTreated))
			  markOrbit(**cov, ORBIT_CELL[ADim], AMarkTreated);

		      delete cov;
		    }
	
		merge(*it, alpha(*it,ADim+1), ADim+1, false);
		++nbIsolated;
	      }
	  }
	else
	  setMark(*it, treated);
      }

  negateMaskMark(treated);
  freeMark(treated);

  // Suppression des cellules isolées:
  if (ADeleteDarts)
    {
      if (nbIsolated>0)
	for (it.reinit(); it.cont(); )
	  {
	    CDart * dart = it++;
	
	    if (isMarked(dart, toDelete))
	      delMapDart(dart);
	  }

      freeMark(toDelete);
    }

  return nbIsolated;
}
//******************************************************************************
int CGMapGeneric::isolateMarkedCells(int AMarkNumber, int ADim,
				     bool ADeleteDarts)
{
  assert(ADim>=0 && ADim<=2);
  return isolateMarkedCells(AMarkNumber, ADim, ADeleteDarts, true);
}
//******************************************************************************
int CGMapGeneric::intuitiveIsolateMarkedCells(int AMarkNumber,
					       bool ADeleteDarts)
{
  int treated = getNewMark();
  int nbIsolated = 0;

  for (int dim=3; dim>=1; --dim)
    nbIsolated +=
      isolateMarkedCells(AMarkNumber, dim, ADeleteDarts, true, treated);

  unmarkAll(treated);
  freeMark(treated);

  return nbIsolated;
}
//******************************************************************************
// Méthode protégée!
void CGMapGeneric::markCellsToBeIsolated(int AMarkSource, int ADim,
					 int AMarkDestination, int AMarkTreated)
{
  assert(ADim>=0 && ADim<=2);

  // Orbite "demi-cellule":
  // ADim == 0 => Cellules d'orbite 23,
  // ADim == 1 => Cellules d'orbite 03,
  // ADim == 2 => Cellules d'orbite 01.
  TOrbit halfCell = SUB_ORBIT(ORBIT_CC, ORBIT_I_IP1[ADim]);

  CDynamicCoverageAll it(this);

  // On sélectionne les demi-cellules de dimension ADim marquées:
  markIncidentCells(halfCell, AMarkSource, AMarkDestination);

  // ... qui sont ADim+1-cousues:
  for (; it.cont(); ++it)
    if (isMarked(*it, AMarkDestination) && isFree(*it, ADim+1))
      unsetMark(*it, AMarkDestination);

  // Lorsque plusieurs cellules adjacentes de dimension ADim+1 sont
  // sélectionnées, il faut désélectionner le bord de l'objet correspondant à
  // l'union de ces cellules:
  int treated = getNewMark();

  if (AMarkTreated>=0)
    markCopy(AMarkTreated, treated);

  for (it.reinit(); it.cont(); ++it)
    if (!isMarked(*it, treated))
      // Pour chaque cellule de dimension ADim dont les 2 demi-cellules sont
      // marquées,
      if (isMarked(  *it         , AMarkDestination) &&
	  isMarked(alpha(*it, ADim+1), AMarkDestination))
	// Traiter les 2 cellules de dimension ADim+1 incidentes à cette cellule
	// de dimension ADim:
	for (int i=0; i<=1; ++i)
	  {
	    // Première cellule: incidente à *it,
	    // Deuxième cellule: incidente à alpha(*it, ADim+1).
	    CCoverage * cov = getDynamicCoverage(i==0 ? *it : alpha(*it, ADim+1),
						  ORBIT_CELL[ADim+1]);
	    for (; cov->cont(); ++(*cov))
	      {
		// Démarquer tous les brins de la cellule qui ne sont pas sur
		// une cellule de dimension ADim marquée des 2 côtés:
		setMark(**cov, treated);
		if (!isFree(**cov, ADim+1) &&
		    !isMarked(alpha(**cov, ADim+1), AMarkDestination))
		  unsetMark(**cov, AMarkDestination);

		// Si **cov est ADim+1-libre, il alpha déjà été démarqué plus haut.

		// Si alpha(**cov, ADim+1) est marqué, il ne faut pas marquer **cov
		// car si alpha(**cov, ADim+1) est traité par l'itérateur it après
		// **cov, il y aura des démarquages non voulus.
	      }

	    delete cov;
	  }

  // À ce stade, les cellules de dimension ADim sont soit totalement marquées,
  // soit totalement démarquées.

  // Démarquage des cellules impossible à fusionner:
  if (AMarkTreated>=0)
    markCopy(AMarkTreated, treated);
  else
    unmarkAll(treated);

  for (it.reinit(); it.cont(); ++it)
    if (isMarked(*it, AMarkDestination) && !isMarked(*it, treated))
      {
  	markOrbit(*it, ORBIT_CELL[ADim], treated);
	
	if (!canMerge(*it, alpha(*it,ADim+1), ADim+1))
	  unmarkOrbit(*it, ORBIT_CELL[ADim], AMarkDestination);
      }

  // Si à ce stade une cellule de dimension ADim+2 (ou ADim+3) est totalement
  // sélectionnée, il faut désélectionner les cellules de dimension ADim qui la
  // composent, sinon elle va disparaître par fusions successives :)
  if (ADim<2)
    {
      if (AMarkTreated>=0)
	markCopy(AMarkTreated, treated);
      else
	unmarkAll(treated);

      for (it.reinit(); it.cont(); ++it)
	if (isMarked(*it, AMarkDestination) && !isMarked(*it, treated))
	  {
	    bool whole = isWholeCellMarked(*it, ORBIT_CELL[ADim+2],
					   AMarkDestination);

	    CCoverage * cov = getDynamicCoverage(*it, ORBIT_CELL[ADim+2]);

	    for (; cov->cont(); ++(*cov))
	      if (!isMarked(**cov, treated))
	      {
		markOrbit(**cov, ORBIT_CELL[ADim], treated);
		
		if (whole)
		  unmarkOrbit(**cov, ORBIT_CELL[ADim], AMarkDestination);

		// Remarque: Démarquer les demi-cellules n'est pas suffisant
		// dans le cas d'un volume dont on alpha bouché le 3-bord...
	      }

	    delete cov;
	  }
    }

  unmarkAll(treated);
  freeMark(treated);
}
//******************************************************************************
int CGMapGeneric::mergeMarkedCells(int AMarkNumber, int ADim,
				   bool ADeleteDarts)
{
  assert(ADim>=1 && ADim<=3);

  // Sélection des cellules de dimension ADim-1 qui doivent être isolées:
  int selected = getNewMark();

  markCellsToBeIsolated(AMarkNumber, ADim-1, selected);

  // Fusion effective:
  int nbMerged = isolateMarkedCells(selected, ADim-1, ADeleteDarts, false);

  unmarkAll(selected);
  freeMark(selected);

  return nbMerged;
}
//******************************************************************************
int CGMapGeneric::intuitiveMergeMarkedCells(int AMarkNumber, bool ADeleteDarts)
{
  int nbMerged = 0;
  int treated = getNewMark();

  int selected = getNewMark();

  for (int dim=3; dim>=1; --dim)
    {
      // Sélection des cellules qui doivent être isolées:
      markCellsToBeIsolated(AMarkNumber, dim-1, selected, treated);

      // Fusion effective:
      nbMerged +=
    	isolateMarkedCells(selected, dim-1, ADeleteDarts, false, treated);

      unmarkAll(selected);
    }

  freeMark(selected);

  unmarkAll(treated);
  freeMark(treated);

  return nbMerged;
}
//******************************************************************************
