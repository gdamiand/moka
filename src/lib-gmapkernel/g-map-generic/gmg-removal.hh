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
// Ce fichier fait partie de "g-map-generic.hh".
//******************************************************************************

public:

/**
 * @param ADart Un pointeur sur un brin
 * @param ADim Une dimension
 * @return Un booléen indiquant si la cellule peut être supprimée.
 */
bool canRemove(CDart* ADart, int ADim);

/**
 * Supprime la cellule de dimension ADim désignées par le brin ADart.
 *
 * @param ADart Un pointeur sur brin
 * @param ADim Une dimension
 * @param ADeleteDarts Un booléen indiquant s'il faut supprimer les brins
 *   correspondant à la cellule supprimée.
 *
 * @precondition canRemove(ADart, ADim)
 */
void remove(CDart* ADart, int ADim, bool ADeleteDarts = false);

/**
 * Supprime lorsque cela est possible les cellules dont un brin au moins
 * est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (0, 1 ou 2)
 * @param ADeleteDarts Un booléen indiquant si la suppression doit se faire
 *   en supprimant des brins
 * @return Le nombre de suppressions effectuées
 */
int removeMarkedCells(int AMarkNumber, int ADim,
		      bool ADeleteDarts = false);

/**
 * Supprime lorsque cela est possible les sommets dont un brin au moins
 * est marqué, tout en décallant les arêtes fictives.
 *
 * @param AMarkNumber Un numéro de marque
 * @return Le nombre de suppressions effectuées
 */
int removeMarkedVerticesWithFictiveEdgeShifting(int AMarkNumber);

/**
 * Supprime lorsque cela est possible les arêtes dont un brin au moins
 * est marqué, tout en évitant les déconnexions.
 * Marche uniquement pour une surface (tout les brins 3-libres)
*/
int removeMarkedEdgesWithoutDisconnectionForSurface(int AMarkNumber);

/**
 * Supprime lorsque cela est possible les arêtes dont un brin au moins
 * est marqué, tout en évitant les déconnexions, sans se préoccuper des faces
 * fictives éventuelles.
 *
 * @param AMarkNumber Un numéro de marque
 * @return Le nombre de suppressions effectuées
 */
int removeMarkedEdgesWithoutDisconnection(int AMarkNumber);

/**
 * Supprime lorsque cela est possible les arêtes dont un brin au moins
 * est marqué, tout en évitant les déconnexions.
 * Marche pour une surface avec des faces fictives, en décalant les faces
 * fictives pour obtenir l'objet minimal. TODO vérifier ?
 * ATTENTION : Pour le moment, version en O(n²) avec n nombre d'arêtes...
 * TODO : Améliorer la complexité (par exemple avec des arbre union-find, mais
 *        aussi en décallant les arêtes fictives ?)
 *
 * @param AMarkNumber Un numéro de marque
 * @return Le nombre de suppressions effectuées
 */
int removeMarkedEdgesWithoutDisconnectionWithFaceShifting(int AMarkNumber);

/**
 * Supprime lorsque cela est possible les faces dont un brin au moins est
 * marqué, tout en évitant de créer des volumes non homéomorphes à des boules.
 * A la fin, démarque tout, sauf les arêtes de degré supérieur à 2.
 * ATTENTION : Pour le moment, version en O(n²) avec n nombre de faces...
 * TODO : Améliorer la complexité (par exemple avec des arbre union-find, mais
 *        aussi en décallant les faces fictives ?)
 *
 * @param AMarkNumber Un numéro de marque
 * @return Le nombre de suppressions effectuées
 */
int removeMarkedFacesButKeepBalls(int AMarkNumber);

/* Simplify2DObject and Simplify3DObject are defined in
 * file g-map-vertex/gmv-merge.hh because vertex attributes must be updates.
 */
//******************************************************************************
