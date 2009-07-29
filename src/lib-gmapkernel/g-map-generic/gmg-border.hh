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
 * Calcule la dimension du bord incident au brin ADart.
 *
 * @param ADart Un brin quelconque
 * @return La dimension du bord de plus petite dimension incident au brin
 *
 * @postcondition La valeur retournée vaut 0, 1, 2, 3 ou 4.
 */
int getBorderDimension(CDart* ADart);

/**
 * Marque les bords de dimension ADim incidents aux brins sélectionnés.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADim Une dimension (0, 1, 2 ou 3)
 * @return Un booléen indiquant si des bords ont été sélectionnés
 */
bool markBorders(int AMarkNumber, int ADim);

/**
 * Marque les bords incidents aux brins sélectionnés.
 * La dimension des bord est automatiquement déterminée
 * avec la méthode 'getBorderDimension'.
 *
 * @param AMarkNumber Un numéro de marque
 * @return Un booléen indiquant si des bords ont été sélectionnés
 */
bool markBorders(int AMarkNumber);

/**
 * Teste si les deux bords de dimension ADim désignés par les brins
 * ADart1 et ADart2 peuvent être cousus.
 * Si An est strictement positif, on teste si la couture est possible sur
 * les An premiers brins parcourus sur chaque bord seulement.
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param ADim Une dimension
 * @param An Le nombre d'itérations à effectuer
 * @return Un booléen indiquant si les deux bords peuvent être cousus.
 */
bool canSewBorders(CDart* ADart1, CDart* ADart2, int ADim, int An = 0);

/**
 * Coud les deux bords de dimension ADim désignés par les brins
 * ADart1 et ADart2.
 * Si An est strictement positif, sa valeur indique le nombre de
 * coutures à effectuer (nombre d'itérations à effectuer sur
 * l'itérateur de bord).
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param ADim Une dimension
 * @param An Le nombre d'itérations à effectuer
 *
 * @precondition canSewBorders(ADart1, ADart2, ADim, An)
 */
void borderTopoSew(CDart* ADart1, CDart* ADart2, int ADim, int An = 0);

/**
 * Coud les deux bords de dimension ADim désignés par les brins
 * ADart1 et ADart2.
 * Si An est strictement positif, sa valeur indique le nombre de
 * coutures à effectuer (nombre d'itérations à effectuer sur
 * l'itérateur de bord).
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param ADim Une dimension
 * @param An Le nombre d'itérations à effectuer
 *
 * @precondition canSewBorders(ADart1, ADart2, ADim, An)
 */
void borderSew(CDart* ADart1, CDart* ADart2, int ADim, int An = 0);

/**
 * Coud les deux bords désignés par les brins ADart1 et ADart2.
 * La dimension des bords est déterminée par la méthode
 * 'getBorderDimension' appliquée sur ADart1.
 *
 * Si An est strictement positif, sa valeur indique le nombre de
 * coutures à effectuer (nombre d'itérations à effectuer sur
 * l'itérateur de bord).
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param An Le nombre d'itérations à effectuer
 * @return Un booléen indiquant si la couture alpha été possible
 */
bool intuitiveBorderSew(CDart* ADart1, CDart* ADart2, int An = 0);

/**
 * Coud les deux 2-bords désignés par les brins ADart1 et ADart2.
 * Si An est strictement positif, sa valeur indique le nombre d'ARÊTES à coudre.
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param An Le nombre d'arêtes à coudre
 *
 * @precondition canSewBorders(ADart1, ADart2, 2, 2*An)
 */
void border2TopoSew(CDart* ADart1, CDart* ADart2, int An = 0);

/**
 * Coud les deux 2-bords désignés par les brins ADart1 et ADart2.
 * Si An est strictement positif, sa valeur indique le nombre d'ARÊTES à coudre.
 *
 * @param ADart1 Un pointeur sur un premier brin
 * @param ADart2 Un pointeur sur un deuxième brin
 * @param An Le nombre d'arêtes à coudre
 *
 * @precondition canSewBorders(ADart1, ADart2, 2, 2*An)
 */
void border2Sew(CDart* ADart1, CDart* ADart2, int An = 0);

//******************************************************************************
