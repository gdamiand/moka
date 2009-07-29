/*
 * lib-mesh : Opérations de maillage et lissage.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-mesh
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
// Ce fichier fait partie de "mesh-generic.hh"
//******************************************************************************

public:

/**
 * Teste s'il est possible de mailler l'arête incidente à ADart.
 *
 * @param ADart Un brin de la carte
 * @return 'vrai' si l'arête incidente à ADart peut être maillée
 */
bool canMesh1(CDart* ADart);

/**
 * Maille l'arête incidente à ADart.
 * L'arête est donc découpée en ASx arêtes.
 *
 * Si ADirectInfoAlpha0 est positif ou nul, il indique un champ directInfo
 * où sont mémorisés les images par alpha0 des brins tels qu'elles étaient
 * avant le maillage.
 * Ceci est utile lorsqu'on maille plusieurs arêtes en parallèle (méthode
 * 'meshMarkedEdges'). Au moment du plongement (effectué par la méthode
 * 'CGMapVertex::mesh1', cela permet de récupérer facilement les points de
 * contrôle dont on alpha besoin sans avoir à parcourir complètement les
 * arêtes déjà 1-maillées.
 *
 * Si ADirectInfoAssoc est positif ou nul, il indique un champ directInfo
 * où sont mémorisées les extrémités du maillage créé. Ceci permet
 * d'accéder au maillage d'une arête déjà traitée.
 *
 * @param ADart Un brin de l'arête à mailler
 * @param ASx Le nombre de mailles à créer
 * @param AMeshMatrix Matrice utilisée pour le calcul des
 *   plongements des cubiques
 * @param AMeshWithMerges Un booléen indiquant la manière dont sont
 *   effectuées les maillages de cellules. Si vrai, les maillages
 *   créés sont fusionnés aux cellules auquelles ils correspondent.
 *   Sinon, les maillages restent indépendants des cellules à
 *   partir desquelles ils ont été créés.
 * @param AMeshWithAdjacentSews Un booléen indiquant la manière dont
 *   sont effectuées les maillages de cellules. Si 'vrai', les
 *   maillages de cellules adjacentes créés en parallèle sont
 *   cousus entre eux. Sinon les maillages créés restent isolés.
 * @param AMeshAssociatedCells Un booléen indiquant si les cellules
 *   associées sont maillées ou non.
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 * @param ADirectInfoAssoc Un indice de champ directInfo
 *
 * @precondition canMesh1(ADart);
 * @precondition ASx>0
 * @precondition ADirectInfoAlpha0>=0 => FMeshWithMerges
 * @precondition ADirectInfoAssoc >=0 => !FMeshWithMerges
 */
virtual void mesh1(CDart* ADart, int ASx,
		   const CTransformationMatrix* AMeshMatrix = NULL,
		   bool AMeshWithMerges = true,
		   bool AMeshWithAdjacentSews = true,
		   bool AMeshAssociatedCells = false,
		   int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

/**
 * Teste s'il est possible de 2-mailler le carré incident à ADart.
 * Si AInitialMeshDim vaut 0, on teste si la cellule incidente à ADart est
 * un carré (4 arêtes).
 * Si AInitialMeshDim vaut 1, on teste si la cellule incidente à ADart est
 * un carré 1-maillé (2*(ASx+ASy) arêtes).
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions à créer selon la première dimension
 * @param ASy Le nombre de subdivisions à créer selon la deuxième dimension
 * @param AInitialMeshDim La dimension dans laquelle le carré à mailler est
 *   déjà maillé (0 ou 1)
 *
 * @precondition ASx>0
 * @precondition ASy>0
 * @precondition AInitialMeshDim = 0 ou 1
 */
bool canMesh2(CDart* ADart,
	      int ASx, int ASy,
	      int AInitialMeshDim = 0);

/**
 * Maille le carré incident à ADart, de la dimension AInitialMeshDim vers
 * la dimension AFinalMeshDim.
 *
 * Possibilités:
 *  AInitialMeshDim==0, AFinalMeshDim==1 : Maillage 1d des arêtes d'un carré
 *  AInitialMeshDim==0, AFinalMeshDim==2 : Maillage 2d d'un carré
 *  AInitialMeshDim==1, AFinalMeshDim==2 : Maillage 2d d'un carré 1-maillé
 *
 * ADart doit correspondre au brin AMesh2Corners[0][0] de la méthode
 * 'createTopoMesh2'.
 *
 * Pour plus d'informations concernant les champs ADirectInfoAlpha0 et
 * ADirectInfoAssoc, se reporter à la méthode 'mesh1'.
 * Pour plus d'informations concernant les champs AMeshWithMerges,
 * AMeshWithAdjacentSews et AMeshAssociatedCells voir la méthode 'mesh1'.
 *
 * @param ADart Un brin du carré à mailler
 * @param ASx Le nombre de mailles à créer selon la première dimension
 * @param ASy Le nombre de mailles à créer selon la deuxième dimension
 * @param AMeshMatrix Matrice utilisée pour le calcul des
 *   plongements des cubiques
 * @param AMeshWithMerges Un booléen
 * @param AMeshWithAdjacentSews Un booléen
 * @param AMeshAssociatedCells Un booléen
 * @param AGetAssociatedEmbeddings Un booléen
 * @param AInitialMeshDim La dimension du maillage de la cellule de départ
 *   (0 ou 1)
 * @param AFinalMeshDim La dimension dans laquelle la cellule doit être
 *   maillée (1 ou 2)
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 * @param ADirectInfoAssoc Un indice de champ directInfo
 *
 * @precondition ASx>0
 * @precondition ASy>0
 * @precondition AInitialMeshDim = 0 ou 1
 * @precondition AFinalMeshDim   = 1 ou 2
 * @precondition AInitialMeshDim < AFinalMeshDim
 * @precondition canMesh2(ADart, ASx,ASy, AInitialMeshDim)
 */
virtual void mesh2(CDart* ADart,
		   int ASx, int ASy,
		   const CTransformationMatrix * AMeshMatrix = NULL,
		   bool AMeshWithMerges = true,
		   bool AMeshWithAdjacentSews = true,
		   bool AMeshAssociatedCells = false,
		   bool AGetAssociatedEmbeddings = false,
		   int AInitialMeshDim = 0, int AFinalMeshDim = 2,
		   int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

/**
 * Teste s'il est possible de 3-mailler le cube incident à ADart.
 * Si AInitialMeshDim vaut 0, on teste si la cellule incidente à ADart est
 * un cube (6 faces).
 * Si AInitialMeshDim vaut 1, on teste si la cellule incidente à ADart est
 * un cube 1-maillé.
 * Si AInitialMeshDim vaut 2, on teste si la cellule incidente à ADart est
 * un cube 2-maillé.
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions à créer selon la 1ère dimension
 * @param ASy Le nombre de subdivisions à créer selon la 2ème dimension
 * @param ASz Le nombre de subdivisions à créer selon la 3ème dimension
 * @param AInitialMeshDim La dimension dans laquelle le cube à mailler est
 *   déjà maillé (0, 1 ou 2)
 *
 * @precondition ASx>0
 * @precondition ASy>0
 * @precondition ASz>0
 * @precondition AInitialMeshDim = 0, 1 ou 2
 */
bool canMesh3(CDart* ADart,
	      int ASx, int ASy, int ASz,
	      int AInitialMeshDim = 0);

/**
 * Maille le cube incident à ADart, de la dimension AInitialMeshDim vers la
 * dimension AFinalMeshDim.
 *
 * Possibilités:
 *  AInitialMeshDim == 0, AFinalMeshDim == 1 :
 *             Maillage 1d des arêtes d'un cube
 *  AInitialMeshDim == 0, AFinalMeshDim == 2 :
 *             Maillage 2d des faces d'un cube
 *  AInitialMeshDim == 0, AFinalMeshDim == 3 :
 *             Maillage 3d d'un cube
 *  AInitialMeshDim == 1, AFinalMeshDim == 2 :
 *             Maillage 2d des faces d'un cube 1-maillé
 *  AInitialMeshDim == 1, AFinalMeshDim == 3 :
 *             Maillage 3d d'un cube 1-maillé
 *  AInitialMeshDim == 2, AFinalMeshDim == 3 :
 *             Maillage 3d d'un cube 2-maillé
 *
 * ADart doit correspondre au brin AMesh3Corners[0][0][0] de la méthode
 * 'createTopoMesh3'.
 *
 * Pour plus d'informations concernant les champs ADirectInfoAlpha0 et
 * ADirectInfoAssoc, se reporter à la méthode 'mesh1'.
 * Pour plus d'informations concernant les champs AMeshWithMerges,
 * AMeshWithAdjacentSews et AMeshAssociatedCells voir la méthode 'mesh1'.
 *
 * @param ADart Un brin du cube à mailler
 * @param ASx Le nombre de mailles à créer selon la première dimension
 * @param ASy Le nombre de mailles à créer selon la deuxième dimension
 * @param ASz Le nombre de mailles à créer selon la troisième dimension
 * @param AMeshMatrix Matrice utilisée pour le calcul des
 *   plongements des cubiques
 * @param AMeshWithMerges Un booléen
 * @param AMeshWithAdjacentSews Un booléen
 * @param AMeshAssociatedCells Un booléen
 * @param AInitialMeshDim La dimension du maillage de la cellule de départ
 *   (0 , 1 ou 2)
 * @param AFinalMeshDim La dimension dans laquelle la cellule doit être
 *   maillée (1, 2 ou 3)
 * @param ADirectInfoAlpha0 Un indice de champ directInfo
 * @param ADirectInfoAssoc Un indice de champ directInfo
 *
 * @precondition ASx>0
 * @precondition ASy>0
 * @precondition ASz>0
 * @precondition AInitialMeshDim = 0 ou 1
 * @precondition AFinalMeshDim   = 1 ou 2
 * @precondition AInitialMeshDim < AFinalMeshDim
 * @precondition canMesh3(ADart, ASx,ASy,ASz, AInitialMeshDim)
 */
virtual void mesh3(CDart* ADart,
		   int ASx, int ASy, int ASz,
		   const CTransformationMatrix * AMeshMatrix = NULL,
		   bool AMeshWithMerges = true,
		   bool AMeshWithAdjacentSews = true,
		   bool AMeshAssociatedCells = false,
		   int AInitialMeshDim = 0, int AFinalMeshDim = 3,
		   int ADirectInfoAlpha0 = -1, int ADirectInfoAssoc = -1);

//******************************************************************************
