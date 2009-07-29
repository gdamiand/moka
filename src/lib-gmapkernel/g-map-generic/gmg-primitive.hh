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
 * Crée un triangle topologique (6 brins 0 et 1-cousus).
 *
 * @return Un brin du triangle
 */
CDart* createTopoTriangle();

/**
 * Crée un quadrilatère topologique (8 brins 0 et 1-cousus).
 *
 * @return Un brin du quadrilatère
 */
CDart* createTopoSquare();

/**
 * Crée un polygone à An côtés.
 *
 * @param An Le nombre d'arêtes du polygone
 * @return Un brin du polygone
 */
CDart* createTopoPolygon(int An);

/**
 * Crée un pôle ouvert composé de An triangles 2-cousus entre eux et avec
 * un sommet central.
 * Le pôle est ouvert, c'est-à-dire que 2 des arêtes incidentes au sommet
 * central ne sont pas 2-cousues.
 *
 * @param An Le nombre de triangles qui composent le pôle
 * @return Un brin 2-libre du sommet central du pôle
 */
CDart* createTopoOpenedPole(int An);

/**
 * Crée un pôle composé de An triangles 2-cousus entre eux et avec un
 * sommet central.
 *
 * @param An Le nombre de triangles qui composent le pôle
 * @return Un brin du sommet central du pôle
 */
CDart* createTopoPole(int An);

/**
 * Crée une grille de dimension AMeridians*AParallels en s'appuyant sur la
 * méthode 'createTopoMesh2', et 2-coud deux côtés de cette grille pour
 * obtenir un tube.
 * Les deux 2-bords du tube obtenu sont composés de AMeridians arêtes chacun.
 *
 * ABorder1 s'il n'est pas NULL est positionné sur un brin 2-libre du
 * premier bord du tube.
 * ABorder2 s'il n'est pas NULL est positionné sur un brin 2-libre du
 * second bord du tube.
 * ABorder1 et ABorder2 sont situés en face l'un de l'autre.
 *
 * Remarque :
 * Les paramètres ABorder- passés peuvent être égaux au pointeur NULL.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du tube
 * @param AParallels Le nombre (non nul) de parallèles du tube
 * @param ABorder1 L'adresse d'un pointeur sur brin
 * @param ABorder2 L'adresse d'un pointeur sur brin
 */
void createTopoTube(int AMeridians, int AParallels,
		    CDart* * ABorder1, CDart* * ABorder2);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoTube' ci-dessus et
 * retourne le brin correspondant à ABorder1.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du tube
 * @param AParallels Le nombre (non nul) de parallèles du tube
 * @return Un brin du tube
 */
CDart* createTopoTube(int AMeridians, int AParallels);

/**
 * Crée un tube de dimension AMeridians*AParallels en s'appuyant sur la
 * méthode 'createTopoTube', et 2-coud un pôle de chaque côté de ce tube
 * afin de fermer ses bords.
 *
 * Si ASouthPole n'est pas NULL, il est positionné sur un brin du sommet
 * situé au pôle sud de la sphère créée.
 * Si ASouthPole n'est pas NULL, il est positionné sur un brin du sommet
 * situé au pôle nord de la sphère créée.
 *
 * Remarque :
 * Les adresses des pointeurs sur brins passés en paramètre peuvent être
 * égales à NULL.
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la sphère
 * @param AParallels Le nombre (non nul) de parallèles de la sphère
 * @param ASouthPole L'adresse d'un pointeur sur brin
 * @param ANorthPole L'adresse d'un pointeur sur brin
 */
void createTopoSphere(int AMeridians, int AParallels,
		      CDart* * ASouthPole, CDart* * ANorthPole);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoSphere' ci-dessus
 * et retourne le brin correspondant à ASouthPole.
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la sphère
 * @param AParallels Le nombre (non nul) de parallèles de la sphère
 * @return Un brin de la sphère
 */
CDart* createTopoSphere(int AMeridians, int AParallels);

/**
 * Crée un tube de dimension AMeridians*AParallels en s'appuyant sur la
 * méthode 'createTopoTube', et 2-coud une face de AMeridians sommets à
 * chaque bout du tube de manière à obtenir un cylindre (selon les valeurs
 * de AClose1 et AClose2, les bords sont fermés ou non).
 *
 * Remarque :
 * Les paramètres ABorder- passés peuvent être égaux au pointeur NULL.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du cylindre
 * @param AParallels Le nombre (non nul) de parallèles du cylindre
 * @param ABorder1 L'adresse d'un pointeur sur brin
 * @param ABorder2 L'adresse d'un pointeur sur brin
 * @param AClose1 Un booléen indiquant s'il faut fermer le bord incident
 *   à ABorder1
 * @param AClose2 Un booléen indiquant s'il faut fermer le bord incident
 *   à ABorder2
 */
void createTopoCylinder(int AMeridians, int AParallels,
			CDart* * ABorder1, CDart* * ABorder2,
			bool AClose1 = true, bool AClose2 = true);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoCylinder' ci-dessus
 * et retourne le brin correspondant à ABorder1.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du cylindre
 * @param AParallels Le nombre (non nul) de parallèles du cylindre
 * @param ABorder1 L'adresse d'un pointeur sur brin
 * @param ABorder2 L'adresse d'un pointeur sur brin
 * @param AClose1 Un booléen indiquant s'il faut fermer le bord incident
 * à ABorder1 (voir méthode 'createTopoCylinder' ci-dessus)
 * @param AClose2 Un booléen indiquant s'il faut fermer le bord incident
 *   à ABorder2 (voir méthode 'createTopoCylinder' ci-dessus)
 * @return Un brin du cylindre
 */
CDart* createTopoCylinder(int AMeridians, int AParallels,
			  bool AClose1 = true, bool AClose2 = true);

/**
 * Crée une pyramide à AMeridians méridiens et AParallels parallèles.
 *
 * ABaseDart, s'il n'est pas NULL est positionné sur un brin 2-libre de la
 * base (ou 2-cousu à un brin de la base si cette base est créée).
 * APoleDart, s'il n'est pas NULL est positionné sur un brin situé sur la
 * pointe de la pyramide.
 *
 * ABaseDart et APoleDart sont situés sur le même méridien.
 *
 * Remarque :
 * Les paramètres ABaseDart et APoleDart passés peuvent être égaux au
 * pointeur NULL.
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la pyramide
 * @param AParallels Le nombre (non nul) de parallèles de la pyramide
 * @param ABaseDart  L'adresse d'un pointeur sur brin
 * @param APoleDart  L'adresse d'un pointeur sur brin
 * @param ACloseBase Un booléen indiquant si la base de la pyramide doit être
 *   fermée
 */
void createTopoPyramid(int AMeridians, int AParallels,
		       CDart* * ABaseDart, CDart* * APoleDart,
		       bool ACloseBase = true);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoPyramid' ci-dessus
 * et retourne le brin correspondant à ABaseDart.
 *
 * @param AMeridians Le nombre (non nul) de méridiens de la pyramide
 * @param AParallels Le nombre (non nul) de parallèles de la pyramide
 * @param ACloseBase Un booléen indiquant si la base de la pyramide doit
 *   être fermée
 */
CDart* createTopoPyramid(int AMeridians, int AParallels,
			 bool ACloseBase = true);

/**
 * Crée un tube de dimension AMeridians*AParallels en s'appuyant sur la
 * méthode 'createTopoTube', et 2-coud les deux 2-bords de ce tube de
 * manière à obtenir un tore.
 *
 * AEquator, s'il n'est pas NULL est positionné sur un brin situé sur
 * l'équateur du tore.
 *
 * Remarque :
 * Le paramètre AEquator passé peut être égal au pointeur NULL.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du tore
 * @param AParallels Le nombre (non nul) de parallèles du tore
 * @param AEquator L'adresse d'un pointeur sur brin
 */
void createTopoTorus(int AMeridians, int AParallels, CDart* * AEquator);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createTopoTorus' ci-dessus
 * et retourne le brin correspondant à AEquator.
 *
 * @param AMeridians Le nombre (non nul) de méridiens du tore
 * @param AParallels Le nombre (non nul) de parallèles du tore
 * @return Un brin du tore
 */
CDart* createTopoTorus(int AMeridians, int AParallels);

/**
 * Crée un carré, éventuellement maillé.
 * Le paramètre ASquareCorners permet de récupérer des brins stratégiques
 * du carré, comme indiqué sur la figure.
 *
 * ASx=3, ASy=2
 *
 * ASquareCorners[0][1]             ASquareCorners[1][1]
 *                  +== --+-- --+-- ==+
 *        	    |                 |
 *        	    |                 |
 *        	    +                 +
 *        	    |                 |
 *        	    |                 |
 *        	    +== --+-- --+-- ==+
 * ASquareCorners[0][0]             ASquareCorners[1][0]
 *
 * Si AMeshDimension==0, les valeurs de ASx et ASy ne sont pas prises en compte.
 *
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param AMeshDimension La dimension du maillage (0 ou 2)
 * @param ASquareCorners Un tableau de dimension 2*2 de pointeurs sur brin
 */

void createTopoSquareIMeshed(int ASx, int ASy,
			     int AMeshDimension,
			     CDart* ASquareCorners[2][2]);

/**
 * Crée un cube, éventuellement maillé.
 * Le paramètre AFacesCorners permet de récupérer des brins stratégiques du
 * cube, comme indiqué sur la figure.
 *
 * ACreatedFaces[0][0] correspond à la face X0.
 * ACreatedFaces[0][1] correspond à la face X1.
 * ACreatedFaces[1][0] correspond à la face Y0.
 * ACreatedFaces[1][1] correspond à la face Y1.
 * ACreatedFaces[2][0] correspond à la face Z0.
 * ACreatedFaces[2][1] correspond à la face Z1.
 *
 * AFacesCorners[0][0] correspond aux coins de la face X0.
 * AFacesCorners[0][1] correspond aux coins de la face X1.
 * AFacesCorners[1][0] correspond aux coins de la face Y0.
 * AFacesCorners[1][1] correspond aux coins de la face Y1.
 * AFacesCorners[2][0] correspond aux coins de la face Z0.
 * AFacesCorners[2][1] correspond aux coins de la face Z1.
 *
 * Les éléments des tableaux AFacesCorners[dim][i] sont des pointeurs qui
 * sont positionnés sur certains brins du cube.
 * Ces brins sont numérotés sur le schéma.
 *
 * Les six booléens donnés par ACreatedFaces indiquent quelles faces du
 * cube doivent être créées.
 *
 *            +----  ----+           +01--  --11+
 *        +   10        11          /          /    +
 *      11|   |          |         /          /   11|
 *      / |      ARRIÈRE              HAUT        / |
 *            |   (Y1)   |       /   (Z1)   /
 *    /   |   00        01      /          /    /   |
 *  01    |   +----  ----+     +00--  --10+   01    |
 *  +  G. +                                   +  D. +
 *  |(X0)10   +01--  --11+     +----  ----+   |(X1)10
 *  |   /    /          /      10        11   |   /
 *          /          /       |          |
 *  | /        BAS                 AVANT      | /
 *  |00   /   (Z0)   /         |   (Y0)   |   |00
 *  +    /          /          00        01   +
 *      +00--  --10+           +----  ----+
 *
 * AMeshDimension indique dans quelle dimension le cube créé doit être maillé.
 * Pour créer un cube 3-maillé, utilisez la méthode createTopoMesh3.
 *
 * Si AMeshDimension==0, les valeurs de ASx, ASy et ASz ne sont pas prises
 * en compte.
 *
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @param AMeshDimension La dimension du maillage (0, 1 ou 2)
 * @param ACreatedFaces Un tableau de dimension 3*2 de booléens
 * @param AFacesCorners Un tableau de dimension 3*2*2*2 de pointeurs sur
 *   brin
 */
void createTopoCubeIMeshed(int ASx, int ASy, int ASz,
			   int AMeshDimension,
			   bool ACreatedFaces[3][2],
			   CDart* AFacesCorners[3][2][2][2]);

//******************************************************************************
