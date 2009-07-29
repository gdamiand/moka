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
 * Calcule le degré d'une cellule de dimension ACellDim, c'est-à-dire le
 * nombre de cellules de dimension ACellDim+1 qui lui sont incidentes.
 * Cette méthode est utile pour tester si une fusion est possible
 * (méthode 'canMerge').
 *
 * La valeur de ABoundingCellDim indique la dimension de la cellule
 * englobante.
 * Par exemple, si ACellDim==0 et ABoundingCellDim==3, la méthode calcule
 * le nombre d'arêtes incidentes au sommet désigné par ADart dont un brin
 * au moins appartient au volume incident à ADart.
 *
 * @param ADart Un brin de la carte
 * @param ACellDim Une dimension
 * @param ABoundingCellDim La dimension de la cellule englobante
 * @return La dimension de la cellule de dimension ACellDim incidente à ADart.
 *
 * @precondition 0 <= ACellDim < 3
 * @precondition ACellDim < ABoundingCell <= 4
 */
int degree(CDart* ADart, int ACellDim, int ABoundingCellDim = 4);

/**
 * Vérifie si la condition alpha_i+1 o alpha_i+2 est une involution pour
 * chaque brin de la cellule.
 *
 * @param ADart Un brin de la carte
 * @param ACellDim Une dimension
 * @return vrai ssi la cellule de dimension ACellDim incidente à
 *   ADart est de "degré local supérieur" égal à 2.
 *
 * @precondition 0 <= ACellDim < 3
 * @precondition ADart!=NULL
 */
bool isLocalDegreeTwoSup(CDart* ADart, int ACellDim);

/**
 * Vérifie si la condition alpha_i-1 o alpha_i-2 est une involution pour
 * chaque brin de la cellule.
 *
 * @param ADart Un brin de la carte
 * @param ACellDim Une dimension
 * @return vrai ssi la cellule de dimension ACellDim incidente à
 *   ADart est de "degré local inférieur" égal à 2.
 *
 * @precondition 0 <= ACellDim < 3
 * @precondition ADart!=NULL
 */
bool isLocalDegreeTwoInf(CDart* ADart, int ACellDim);

//******************************************************************************
