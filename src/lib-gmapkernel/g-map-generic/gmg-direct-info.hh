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
 * Alloue ABytes octets au champ directInfo[ADirectInfoIndex] des brins
 * marqués avec la marque AMarkNumber.
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param ABytes Le nombre d'octets à allouer
 * @param AOrbitUsed Une orbite quelconque
 */
void allocMemoryToDirectInfo(int AMarkNumber, int ADirectInfoIndex,
			     int ABytes, TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Alloue ABytes octets au champ directInfo[ADirectInfoIndex] des brins
 * correspondant à l'orbite donnée par le couple (ADart, AOrbit).
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est sur l'orbite (ADart, AOrbit).
 *
 * @param ADart Un brin quelconque de la carte
 * @param AOrbit Une orbite
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param ABytes Le nombre d'octets à allouer
 * @param AOrbitUsed Une orbite quelconque
 */
void allocMemoryToDirectInfo(CDart* ADart, TOrbit AOrbit,
			     int ADirectInfoIndex,
			     int ABytes, TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Libère l'espace mémoire affecté au champ directInfo[ADirectInfoIndex]
 * des brins marqués avec la marque AMarkNumber.
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param AOrbitUsed Une orbite quelconque
 */
void freeMemoryFromDirectInfo(int AMarkNumber, int ADirectInfoIndex,
			      TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Libère l'espace mémoire affecté au champ directInfo[ADirectInfoIndex]
 * des brins marqués avec la marque AMarkNumber.
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, la libération n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est marqué.
 *
 * @param ADart Un brin quelconque de la carte
 * @param AOrbit Une orbite
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param AOrbitUsed Une orbite quelconque
 */
void freeMemoryFromDirectInfo(CDart* ADart, TOrbit AOrbit,
			      int ADirectInfoIndex,
			      TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Pour chaque brin b de la carte, copie la valeur de l'image par alpha
 * AAlphaIndex de b dans son champ direcInfo d'indice ADirectInfoIndex.
 *
 * Remarque: La méthode 'getDirectInfoAsDart' permet de récupérer aisément
 * les valeurs sauvées.
 *
 * @param AAlphaIndex Une dimension (0, 1, 2 ou 3)
 * @param ADirectInfoIndex Un indice de champ directInfo
 */
void saveAlphaInDirectInfo(int AAlphaIndex, int ADirectInfoIndex);

/** Point each dart of a same AOrbit to the first dart of this orbit. This
 *  dart point to itself. Allows to use union-find trees on this orbit.
 *  @param AIndex the direct info index to use.
 *  @param AOrbit the orbit for which initialize union find trees.
 */
void initUnionFindTrees(int AIndex, TOrbit AOrbit);

/** Init union find trees for two direct info, and two orbits.
 *  
 *  @param AIndex1 the first direct info index to use.
 *  @param AOrbit1 the first orbit for which initialize union find trees.
 *  @param AIndex2 the second direct info index to use.
 *  @param AOrbit2 the second orbit for which initialize union find trees.
 */
void initTwoUnionFindTrees(int AIndex1, TOrbit AOrbit1,
			   int AIndex2, TOrbit AOrbit2);

/** Init union find trees for two direct info, for faces and volumes.
 *  This method is equivalent to calling initTwoUnionFindTrees with
 *  ORBIT_FACE and ORBIT_VOLUME, but it is optimized.
 *  
 *  @param AIndexFace the direct info index for faces.
 *  @param AIndexVol the direct info index for volumes.
 */
void initUnionFindTreesFaceVolume(int AIndexFace, int AIndexVol);

/** Init union find trees for two direct info, for vertices and edges.
 *  This method is equivalent to calling initTwoUnionFindTrees with
 *  ORBIT_VERTEX and ORBIT_EDGE, but it is optimized.
 *
 *  @param AIndexVertex the direct info index for vertices.
 *  @param AIndexEdge the direct info index for edges.
 */
void initUnionFindTreesVerticesEdges(int AIndexVertex, int AIndexEdge);

/** Return the root of the uf-tree containing ADart.
 *  @param ADart the dart to find the root.
 *  @param AIndex the direct info index to use.
 */
CDart* findUnionFindTrees(CDart* ADart,int AIndex);

/** Merge the two uf-trees containing ADart1 and ADart2.
 *  @param ADart1 the dart of a first uf-tree.
 *  @param ADart2 the dart of a second uf-tree.
 *  @param AIndex the direct info index to use.
 *  @todo we can improve this by counting the size of the tree and putting
 *  the small one under the big one. 
 */
void mergeUnionFindTrees(CDart* ADart1, CDart* ADart2, int AIndex);

//******************************************************************************
