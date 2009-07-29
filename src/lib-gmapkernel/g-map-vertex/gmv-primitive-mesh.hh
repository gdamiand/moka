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

//******************************************************************************
typedef enum { GMV_NO = 0, GMV_YES, GMV_CHECK } TEmbedMethod;

/**
 * Plonge les sommets d'un maillage 1d généralement créé avec la méthode
 * 'CGMapGeneric::createTopoMesh1'.
 *
 * Si AMeshDimension > 1, le plongement est effectué le long du bord d'un
 * maillage 2d ou 3d.
 *
 * Le passage d'une arête à la suivante se fait de manières différentes en
 * fonction de la valeur de AMeshDimension:
 * Si AMeshDimension == 1 : alpha01
 * Si AMeshDimension == 2 : alpha0121
 * Si AMeshDimension == 3 : alpha012321
 *
 * @param AFirstDart Un brin situé à l'origine du maillage 1d
 * @param ADiver Un calculateur des plongements du maillage 1d
 * @param ADiveBorders Indique s'il faut plonger le bord du maillage 1d
 *   (2 sommets)
 * @param AMeshDimension Un entier indiquant la dimension du maillage
 *   à plonger (1, 2 ou 3)
 */
void diveMesh1(CDart* AFirstDart, CMesh1Diver& ADiver,
	       TEmbedMethod ADiveBorders = GMV_YES, int AMeshDimension = 1);

/**
 * Plonge les sommets d'un maillage 2d généralement créé avec la méthode
 * 'CGMapGeneric::createTopoMesh2'.
 *
 * Si AMeshDimension == 3, le plongement est effectué le long du bord d'un
 * maillage 3d.
 *
 * Le passage d'une face à la suivante se fait de manières différentes en
 * fonction de la valeur de AMeshDimension:
 * Si AMeshDimension == 2 : alpha0121
 * Si AMeshDimension == 3 : alpha012321 
 *
 * @param AFirstDart Un brin situé à l'origine du maillage 2d
 * @param ADiver Un calculateur des plongements du maillage 2d
 * @param ADiveBorders Indique s'il faut plonger le bord du maillage 2d
 *   (4 arêtes)
 * @param AMeshDimension Un entier indiquant la dimension du maillage
 *   à plonger (2 ou 3)
*/
void diveMesh2(CDart* AFirstDart, CMesh2Diver& ADiver,
	       TEmbedMethod ADiveBorders = GMV_YES, int AMeshDimension = 2);

/**
 * Plonge les sommets d'un maillage 3d généralement créé avec la méthode
 * 'CGMapGeneric::createTopoMesh3'.
 *
 * @param AFirstDart Un brin situé à l'origine du maillage 3d
 * @param ADiver Un calculateur des plongements du maillage 3d
 * @param ADiveBorders Indique s'il faut plonger le bord du maillage 3d
 *   (6 faces)
 */
void diveMesh3(CDart* AFirstDart, CMesh3Diver& ADiver,
	       TEmbedMethod ADiveBorders = GMV_YES);

//******************************************************************************

/**
 * Appelle la méthode CGMapGeneric::createMesh1 et plonge la polyligne
 * obtenue en utilisant ADiver.
 *
 * @param ASx Le nombre d'arêtes de la polyligne
 * @param AMesh1Corners Un tableau de 2 pointeurs sur brin
 * @param AExtremity2 L'adresse d'un pointeur sur brin
 * @param ADiver Un calculateur des plongements du maillage 1d
 */
void createMesh1(int ASx, CDart* AMesh1Corners[2], CMesh1Diver& ADiver);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createMesh1' ci-dessus et
 * retourne le brin correspondant à AExtremity1.
 *
 * @param ASx Le nombre d'arêtes de la polyligne
 * @param ADiver Un calculateur des plongements du maillage 1d
 * @return Un brin 1-libre de la polyligne
 */
CDart* createMesh1(int ASx, CMesh1Diver& ADiver);

/**
 * Cette méthode appelle 'createMesh1' ci dessus.
 * Le plongement est déterminé par le plongeur:
 * CMesh1BasicDiver(-OX/2, OX, ASx).
 *
 * @param ASx Le nombre d'arêtes de la polyligne
 * @return Un brin 1-libre de la polyligne
 */
CDart* createMesh1(int ASx);

/**
 * Appelle la méthode CGMapGeneric::createMesh2 et plonge la grille
 * obtenue en utilisant ADiver.
 *
 * @param ASx Le nombre (non nul) de subdivisions de la grille sur l'axe OX
 * @param ASy Le nombre (non nul) de subdivisions de la grille sur l'axe OY
 * @param AMesh2Corners Un tableau de taille 2*2 de pointeurs sur brin
 *   correspondant aux coins du maillage 2d
 * @param ADiver Un calculateur des plongements de la grille
 */
void createMesh2(int ASx, int ASy,
		 CDart* AFaceCorners[2][2], CMesh2Diver& ADiver);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createMesh2' ci-dessus et
 * retourne le brin correspondant à ACornerBL.
 *
 * @param ASx Le nombre (non nul) de subdivisions de la grille sur l'axe OX
 * @param ASy Le nombre (non nul) de subdivisions de la grille sur l'axe OY
 * @param ADiver Un calculateur des plongements de la grille
 * @return Un brin de la grille
 */
CDart* createMesh2(int ASx, int ASy, CMesh2Diver& ADiver);

/**
 * Cette méthode appelle 'createMesh2' ci dessus.
 * Le plongement est déterminé par le plongeur:
 * CMesh2BasicDiver(-(OX+OY)/2, OX, ASx, OY, ASy).
 *
 * @param ASx Le nombre (non nul) de subdivisions de la grille sur l'axe OX
 * @param ASy Le nombre (non nul) de subdivisions de la grille sur l'axe OY
 * @return Un brin 1-libre de la grille
 */
CDart* createMesh2(int ASx, int ASy);

/**
 * Appelle la méthode CGMapGeneric::createMesh3 et plonge le maillage 3d
 * obtenu en utilisant ADiver.
 *
 * @param ASx Le nombre (non nul) de subdivisions du maillage 3d sur l'axe OX
 * @param ASy Le nombre (non nul) de subdivisions du maillage 3d sur l'axe OY
 * @param ASz Le nombre (non nul) de subdivisions du maillage 3d sur l'axe OZ
 * @param AMesh3Corners Un tableau de taille 2*2*2 de pointeurs sur brin
 *   correspondant aux coins du maillage 3d
 * @param ADiver Un calculateur des plongements du maillage 3d
 */
void createMesh3(int ASx, int ASy, int ASz,
		 CDart* AMesh3Corners[2][2][2],
		 CMesh3Diver& ADiver);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du
 * noyau. Elle appelle simplement la méthode 'createMesh3' ci-dessus et
 * retourne le brin correspondant à AMesh3Corners[0][0][0].
 *
 * @param ASx Le nombre (non nul) de subdivisions du maillage 3d sur l'axe OX
 * @param ASy Le nombre (non nul) de subdivisions du maillage 3d sur l'axe OY
 * @param ASz Le nombre (non nul) de subdivisions du maillage 3d sur l'axe OZ
 * @param ADiver Un calculateur des plongements du maillage 3d
 * @return Un brin du maillage 3d
 */
CDart* createMesh3(int ASx, int ASy, int ASz, CMesh3Diver& ADiver);

/**
 * Cette méthode appelle 'createMesh3' ci dessus.
 * Le plongement est déterminé par le plongeur:
 * CMesh3BasicDiver(-(OX+OY+OZ)/2, OX, ASx, OY, ASy, OZ, ASz).
 *
 * @param ASx Le nombre (non nul) de subdivisions de la grille sur l'axe OX
 * @param ASy Le nombre (non nul) de subdivisions de la grille sur l'axe OY
 * @param ASz Le nombre (non nul) de subdivisions de la grille sur l'axe OY
 * @return Un brin 1-libre de la grille
 */
CDart* createMesh3(int ASx, int ASy, int ASz);

//******************************************************************************
