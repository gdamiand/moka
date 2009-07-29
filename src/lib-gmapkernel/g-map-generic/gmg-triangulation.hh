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
 * Triangule l'arête incidente à ADart (la "coupe" en deux).
 *
 * @param ADart Un brin de la carte
 * @return Un brin du sommet topologique créé
 */
virtual CDart* triangulateEdge(CDart* ADart);

/**
 * Triangule la face incidente à ADart.
 *
 * @param ADart Un brin de la carte
 * @return Un brin du sommet topologique créé
 */
virtual CDart* triangulateFace(CDart* ADart);

/**
 * Triangule le volume incident à ADart.
 *
 * @param ADart Un brin de la carte
 * @return Un brin du sommet topologique créé
 */
virtual CDart* triangulateVolume(CDart* ADart);

/**
 * Triangule la cellule de dimension ADim incidente à ADart.
 *
 * @param ADart Un brin de la carte
 * @return Un brin du sommet topologique créé
 *
 * @precondition ADim>1 et ADim<=3
 */
CDart* triangulate(CDart* ADart, int ADim);

/**
 * Triangule les cellules de dimension ADim dont un brin au moins est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (1, 2 ou 3)
 * @return Le nombre de triangulations effectuées
 */
int triangulateMarkedCells(int AMarkNumber, int ADim);

//******************************************************************************
