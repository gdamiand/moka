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
 * Crée un maillage 1d composé de ASx arêtes.
 * Si ASx est nul, un seul sommet est créé.
 *
 * AMesh1Corners[0] est positionné sur le brin 1-libre situé à la première
 * extrémité de la polyligne créée.
 * AMesh1Corners[1] est positionné sur le brin 1-libre situé à la deuxième
 * extrémité de la polyligne créée.
 *
 * Si AModel est NULL, le maillage créé est une polyligne, sinon c'est un
 * faisceau calqué sur l'orbite 23 de AModel.
 *
 * @param ASx Le nombre d'arêtes de la polyligne
 * @param AMesh1Corners Un tableau de 2 pointeurs sur brin
 * @param AModel Un brin de la carte
 */
void createTopoMesh1(int ASx,
		     CDart* AMesh1Corners[2],
		     CDart* AModel = NULL);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoMesh1' ci-dessus et
 * retourne le brin correspondant à AMesh1Corners[0].
 *
 * @param ASx Le nombre d'arêtes de la polyligne
 * @param AModel Un brin de la carte
 * @return Un brin 1-libre de la polyligne
 */
CDart* createTopoMesh1(int ASx, CDart* AModel = NULL);

/**
 * Crée une grille de dimension ASx*ASy.
 * Les éléments de AFaceCorner sont positionnés sur des brins stratégiques
 * de la grille, comme indiqué sur le schéma.
 *
 * ASx=3, ASy=2
 *
 * AMesh2Corners[0][1]                 AMesh2Corners[1][1]
 *       	     +== --+-- --+-- ==+
 *        	     |     |     |     |
 *        	     |     |     |     |
 *        	     +-- --+-- --+-- --+
 *        	     |     |     |     |
 *        	     |     |     |     |
 *        	     +== --+-- --+-- ==+
 * AMesh2Corners[0][0]                 AMesh2Corners[1][0]
 *
 * Si A3Sewed est vrai, le maillage est doublé, c'est-à-dire que 2 grilles
 * identiques sont créées et chaque brin qui constitue la première est
 * 3-cousu à son homologue sur la deuxième.
 *
 * @param ASx Le nombre (non nul) de subdivisions de la grille selon la
 *   première dimension
 * @param ASy Le nombre (non nul) de subdivisions de la grille selon la
 *   deuxième dimension
 * @param AMesh2Corners Un tableau de taille 2*2 de pointeurs sur brin
 *   correspondant aux coins du maillage 2d
 * @param A3Sewed Un booléen indiquant si le maillage est doublé
 */
void createTopoMesh2(int ASx, int ASy,
		     CDart* AMesh2Corners[2][2], bool A3Sewed = false);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoMesh2' ci-dessus et
 * retourne le brin correspondant à AFaceCorners[0][0].
 *
 * @param ASx Le nombre (non nul) de subdivisions de la grille selon la
 *   première dimension
 * @param ASy Le nombre (non nul) de subdivisions de la grille selon la
 *   deuxième dimension
 * @param A3Sewed Un booléen indiquant si le maillage est doublé
 * @return Un brin de la grille
 */
CDart* createTopoMesh2(int ASx, int ASy, bool A3Sewed = false);

/**
 * Crée un maillage 3d de dimension ASx*ASy*ASz.
 * Les éléments de AMesh3Corners sont positionnés sur des brins
 * stratégiques du maillage, comme indiqué sur le schéma.
 *
 * ASx=3, ASy=2, ASz=1
 *
 *            AMesh3Corners[0][1][1]             AMesh3Corners[1][1][1]
 *                               +== --+-- --+-- ==+
 *                              /     /     /     /+
 *                             /     /     /     //|
 *                            +-- --+-- --+-- --+/ |
 *                           /     /     /     /+  +
 *                          /     /     /     //| /+ AMesh3Corners[1][1][0]
 *  AMesh3Corners[0][0][1] +== --+-- --+-- ==+/ AMesh3Corners[1][0][1]
 *                         +-- --+-- --+-- --+  +/
 *                         |     |     |     | /+
 *                         |     |     |     |//
 *                         +-- --+-- --+-- --+/
 *                         +== --+-- --+-- ==+
 *      AMesh3Corners[0][0][0]             AMesh3Corners[1][0][0]
 *
 *
 * Remarque: AMesh3Corners[0][1][0] n'est pas visible.
 *
 * @param ASx Le nombre (non nul) de subdivisions du maillage 3d selon la
 *   première dimension
 * @param ASy Le nombre (non nul) de subdivisions du maillage 3d selon la
 *   deuxième dimension
 * @param ASz Le nombre (non nul) de subdivisions du maillage 3d selon la
 *   troisième dimension
 * @param AMesh3Corners Un tableau de taille 2*2*2 de pointeurs sur brin
 *   correspondant aux coins du maillage 3d
 */
void createTopoMesh3(int ASx, int ASy, int ASz,
		     CDart* AMesh3Corners[2][2][2]);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoMesh3' ci-dessus et
 * retourne le brin correspondant à AMesh3Corners[0][0][0].
 *
 * @param ASx Le nombre (non nul) de subdivisions du maillage 3d selon la
 *   première dimension
 * @param ASy Le nombre (non nul) de subdivisions du maillage 3d selon la
 *   deuxième dimension
 * @param ASz Le nombre (non nul) de subdivisions du maillage 3d selon la
 *   troisième dimension
 * @return Un brin du maillage 3d
 */
CDart* createTopoMesh3(int ASx, int ASy, int ASz);

//******************************************************************************
