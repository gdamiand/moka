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
 * Teste s'il est possible de quadranguler la face incidente à ADart.
 * C'est toujours le cas si AInsertVertices vaut 'vrai'.
 * Si AInsertVertices vaut 'faux', il faut que le nombre d'arêtes de la
 * face soit pair.
 *
 * @param ADart Un brin de la carte
 * @param AInsertVertices Un booléen indiquant s'il faut insérer des
 *   sommets sur les arêtes de la face avant d'effectuer la quadrangulation
 * @return Un booléen indiquant s'il est possible de quadranguler la face
 */
bool canQuadrangulateFace(CDart* ADart, bool AInsertVertices = true);

/**
 * Quadrangule la face incidente à ADart.
 *
 * Si AInsertVertices vaut 'vrai', un sommet est inséré sur chaque arête de
 * la face avant d'effectuer la quadrangulation.
 *
 * Si AInsertVertices vaut 'faux', la quadrangulation est directement
 * effectuée. Dans ce cas, il faut que le nombre d'arêtes qui composent la
 * face soit pair. Un sommet sur deux de la face est modifié. Celui
 * incident à ADart ne l'est pas. Si la face est ouverte, selon la position
 * de ADart sur celle-ci, les arêtes qui sont au bout de la polyligne
 * seront ou non sur une face ouverte.
 *
 * @param ADart Un brin de la carte
 * @param AInsertVertices Un booléen indiquant s'il faut insérer des sommets
 *   sur les arêtes de la face avant d'effectuer la quadrangulation
 * @return Un brin du sommet topologique créé
 */
virtual CDart* quadrangulateFace(CDart* ADart, bool AInsertVertices = true);

/**
 * Quadrangule les faces dont un brin au moins est marqué.
 * Pour plus d'informations sur le paramètre AInsertVertices, se reporter à
 * la méthode 'quadrangulateFace'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param AInsertVertices Un booléen indiquant s'il faut insérer des sommets sur
 *   les arêtes des faces avant d'effectuer la quadrangulation
 * @return Le nombre de quadrangulations effectuées
 */
int quadrangulateMarkedFaces(int AMarkNumber, bool AInsertVertices = true);

//******************************************************************************
