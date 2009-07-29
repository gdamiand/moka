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
 * Plaque la cellule de dimension ADim incidente à ADart1 sur la cellule de
 * dimension ADim incidente à ADart2 (si cela est possible) avant de les
 * coudre entre elles.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ADim La dimension dans laquelle doit se faire la couture
 * @param ARotateCells Un booléen pour modifier ou non l'orientation
 * @param AScaleCells Un booléen pour modifier ou non la taille
 * @param ATranslateCells Un booléen pour modifier ou non la position
 *
 * @precondition 0 <= ADim <= 3
 * @precondition canSew(ADart1, ADart2, ADim)
 */
void geoSew(CDart* ADart1, CDart* ADart2, int ADim,
	    bool ARotateCells = true, bool AScaleCells = true,
	    bool ATranslateCells = true);

/**
 * Appelle la méthode 'geoSew' après avoir déterminé la dimension dans
 * laquelle doit se faire la couture à l'aide de la méthode 'getSewDimension'.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ARotateCells Un booléen pour modifier ou non l'orientation
 * @param AScaleCells Un booléen pour modifier ou non la taille
 * @param ATranslateCells Un booléen pour modifier ou non la position
 * @return Un booléen indiquant si la couture alpha pu se faire
 */
bool intuitiveGeoSew(CDart* ADart1, CDart* ADart2,
		     bool ARotateCells = true, bool AScaleCells = true,
		     bool ATranslateCells = true);

/**
 * Plaque le bord dimension ADim incident à ADart1 sur le bord de dimension
 * ADim incident à ADart2 (si cela est possible) avant de les coudre entre eux.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ADim La dimension dans laquelle doit se faire la couture
 * @param ARotateCells Un booléen pour modifier ou non l'orientation
 * @param AScaleCells Un booléen pour modifier ou non la taille
 * @param ATranslateCells Un booléen pour modifier ou non la position
 *
 * @precondition 0 <= ADim <= 3
 * @precondition canSew(ADart1, ADart2, ADim)
 */
void geoBorderSew(CDart* ADart1, CDart* ADart2, int ADim,
		  bool ARotateCells = true, bool AScaleCells = true,
		  bool ATranslateCells = true);

/**
 * Appelle la méthode 'geoSewBorders' après avoir déterminé la dimension dans
 * laquelle doit se faire la couture à l'aide de la méthode
 * 'getBorderDimension'.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ARotateCells Un booléen pour modifier ou non l'orientation
 * @param AScaleCells Un booléen pour modifier ou non la taille
 * @param ATranslateCells Un booléen pour modifier ou non la position
 * @return Un booléen indiquant si la couture alpha pu se faire
 */
bool intuitiveGeoBorderSew(CDart* ADart1, CDart* ADart2,
			   bool ARotateCells = true, bool AScaleCells = true,
			   bool ATranslateCells = true);

//******************************************************************************
