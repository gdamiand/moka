/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
// Ce fichier fait partie de "controler-gmap-operations.hh".
//******************************************************************************

public:

/** Calcule le nombre d'arêtes non degré 2 local.
 * Si ce nombre vaut zéro, la carte représente une 2 quasi-variété.
 * @return le nombre d'arêtes non degré 2 local.
 */
unsigned int countNonLocalDegreeTwoEdges();

/**
 *  Calcule les caractéristiques globales de la carte (nombre de brins,
 *  de cellules, de bords). Pour ne pas calculer certaines caractéristiques,
 *  mettre à NULL les paramètres correspondants.
 *
 *  @param ANbDarts Le nombre de brins de la carte
 *  @param ANb{Vertices,...,CC} Le nombre de cellules de la carte
 *  @param ANbiBorders Le nombre de i-bords de la carte
 */
void getMapGlobalCharacteristics(int* ANbDarts,
				 int* ANbVertices, int* ANbEdges,
				 int* ANbFaces, int* ANbVolumes, int* ANbCC,
				 int* ANb0Borders, int* ANb1Borders,
				 int* ANb2Borders, int* ANb3Borders);

/**
 *  Calcule les caractéristiques d'une surface sélectionnée (nombre de brins,
 *  de cellules, de bords, caractéristique d'Euler, orientabilité, genre).
 *  Pour ne pas calculer certaines caractéristiques, mettre à NULL les
 *  paramètres correspondants.
 *
 *  @param AName Le nom de la surface (sphère, tore, plan projectif, ...)
 *  @param ANbDarts Le nombre de brins du volume
 *  @param ANb{Vertices,...,CC} Le nombre de cellules du volume
 *  @param ANbiBorders Le nombre de i-bords du volume
 *  @param ANb2BordersWhenClosed Le nombre total de 2-bords qu'on aurait si
 *                               le volume était 0-fermé puis 1-fermé.
 *  @param AEuler La caractéristique d'Euler-Poincaré du volume
 *  @param AOrient Le coefficient d'orientabilité du volume
 *  @param AGenus Le genre du volume
 *
 *  @return true si la sélection est correcte
 */
bool getSurfaceCharacteristics(std::string* AName,
			       int* ANbDarts,
			       int* ANbVertices, int* ANbEdges, int* ANbFaces,
			       int* ANb0Borders, int* ANb1Borders,
			       int* ANb2Borders,
			       int* ANb2BordersWhenClosed,
			       int* AEuler, int* AOrient, int* AGenus);


//******************************************************************************
