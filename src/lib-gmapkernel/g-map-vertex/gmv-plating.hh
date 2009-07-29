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
// Ce fichier fait partie de "g-map-vertex.hh".
//******************************************************************************

public:

/**
 * Teste si les deux cellules incidentes aux brins passés en paramètres
 * peuvent être plaquées (c'est le cas si elles ne sont pas dans la même
 * composante connexe).
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param Un booléen indiquant si les deux cellules incidentes aux brins
 *   donnés peuvent être plaquées
 */
bool canPlate(CDart* ADart1, CDart* ADart2);

/**
 * Calcule la dimension maximum des cellules incidentes aux brins donnés qui
 * peuvent être plaquées, en tenant compte de l'état des coutures de chacun
 * de ces brins (par exemple, pour faire un placage de faces, il faut qu'au
 * moins un des brins soit 3-cousu).
 * Si le placage est impossible, la valeur -1 est retournée, sinon la valeur
 * retournée vaut 0, 1 ou 2.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @return Un entier indiquant la dimension des cellules incidentes aux brins
 *   donnés pouvant être plaquées
 */
int getPlateDimension(CDart* ADart1, CDart* ADart2);

/**
 * Plaque les cellules de dimension ADim incidentes aux brins ADart1 et ADart2.
 * La cellule incidentes à ADart1 voit son plongement modifié pour se plaquer
 * à la cellule incidente à ADart2. Le plongement de la cellule incidente à
 * ADart2 n'est pas modifié.
 * Le placage tient compte de l'état des booléen 'orientation', 'scale' et
 * 'position'.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ADim Une dimension
 * @param ARotateCells Un booléen pour modifier ou non l'orientation
 * @param AScaleCells Un booléen pour modifier ou non la taille
 * @param ATranslateCells Un booléen pour modifier ou non la position
 */
void plate(CDart* ADart1, CDart* ADart2, int ADimbool,
	   bool ARotateCells = true, bool AScaleCells = true,
	   bool ATranslateCells = true);

/**
 * Effectue un placage comme la méthode 'plate'.
 * La dimension du placage est déterminée par la méthode 'getPlateDimension'.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ARotateCells Un booléen pour modifier ou non l'orientation
 * @param AScaleCells Un booléen pour modifier ou non la taille
 * @param ATranslateCells Un booléen pour modifier ou non la position
 * @return true si le placage alpha été possible
 */
bool intuitivePlate(CDart* ADart1, CDart* ADart2,
		    bool ARotateCells = true, bool AScaleCells = true,
		    bool ATranslateCells = true);

/**
 * Effectue un placage des bords de dimension ADim incidents aux brins ADart1
 * et ADart2. Tout comme pour la méthode 'plate', c'est le bord incident à
 * ADart1 qui est déplacé vers le bord incident à ADart2.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ADim Une dimension
 * @param ARotateCells Un booléen pour modifier ou non l'orientation
 * @param AScaleCells Un booléen pour modifier ou non la taille
 * @param ATranslateCells Un booléen pour modifier ou non la position
 */
void borderPlate(CDart* ADart1, CDart* ADart2, int ADim,
		 bool ARotateCells = true, bool AScaleCells = true,
		 bool ATranslateCells = true);

//******************************************************************************
