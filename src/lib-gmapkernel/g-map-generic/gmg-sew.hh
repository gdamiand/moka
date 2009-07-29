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
 * Retourne la plus petite dimension dans laquelle ADart1 et ADart2 peuvent
 * être cousus.
 *
 * @param ADart1 Un brin quelconque
 * @param ADart2 Un brin quelconque
 * @return La plus petite dimension dans laquelle ADart1 et ADart2 peuvent
 *   être cousus
 *
 * @postcondition La valeur retournée vaut 0, 1, 2, 3 ou 4.
 */
int getSewDimension(CDart* ADart1, CDart* ADart2);

/**
 * Essaie de coudre ADart1 et ADart2 en déterminant la dimension de la
 * couture à l'aide de la méthode 'getSewDimension'.
 *
 * @param ADart1 Un brin quelconque
 * @param ADart2 Un brin quelconque
 * @return Un booléen indiquant si la couture alpha pu se faire
 */
bool intuitiveSew(CDart* ADart1, CDart* ADart2);

/**
 * Essaie de coudre par alpha-ADimension des cellules de dimension
 * ADimension, à partir des brins ADart1 et ADart2. Le bord de dimension
 * ADimension incident à chaque brin de départ est parcouru en parallèle en
 * se limitant du premier côté à la région délimitée par la marque.
 *
 * @param ADart1 Un brin de la carte
 * @param AMarkNumber1 Un numéro de marque
 * @param ADart2 Un brin de la carte
 * @param ADimension Une dimension de couture (2 ou 3)
 * @return Le nombre de coutures qui ont été effectuées
 */
int multiSew(CDart* ADart1, int AMarkNumber1,
	     CDart* ADart2, int ADimension);

//******************************************************************************
