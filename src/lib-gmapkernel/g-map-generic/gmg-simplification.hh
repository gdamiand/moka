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
 * Supprime toute les faces plates (cad composée seulement de 2 arêtes) de
 * la carte.
 *
 * @return Le nombre de faces plates détruites.
 */
int deleteFlatFaces();

/**
 * Supprime les faces plates dont un brin au moins est marqué.
 *
 * @return Le nombre de faces plates détruites.
 */
int deleteMarkedFlatFaces(int AMarkNumber);

/**
 * Supprime tous les volumes plats (cad composée seulement de 2 faces) de
 * la carte.
 *
 * @return Le nombre de volumes plats détruits.
 */
int deleteFlatVolumes();

/**
 * Supprime les volumes plats dont un brin au moins est marqué.
 *
 * @return Le nombre de volumes plats détruits.
 */
int deleteMarkedFlatVolumes(int AMarkNumber);

/**
 * Supprime toutes les arêtes pendantes de la carte.
 *
 * @return Le nombre d'arêtes pendantes détruites.
 */
int removeDanglingEdges();

/**
 * Supprime toutes les arêtes pendantes  dont un brin au moins est marqué.
 *
 * @return Le nombre d'arêtes pendantes détruites.
 */
int removeMarkedDanglingEdges(int AMarkNumber);

//******************************************************************************
