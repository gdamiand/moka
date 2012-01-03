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
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param AOrbit Une orbite quelconque
 *
 * @return true si ADart1 et ADart2 appartiennent à la même orbite orientée.
 */
bool belongToSameOrientedOrbit(CDart* ADart1, CDart* ADart2, TOrbit AOrbit);

/**
 * Teste si l'objet déterminé par ADart et AOrbit est orientable.
 * Remarque: L'objet peut ne pas être orientable uniquement pour les
 * orbites ORBIT_VERTEX, ORBIT_VOLUME et ORBIT_CC.
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite (ORBIT_VOLUME ou ORBIT_CC)
 * @return 'vrai' si l'objet est orientable
 *
 * @precondition AOrbit n'est pas une orbite de bord
 */
bool isOrientable(CDart* ADart, TOrbit AOrbit);

/**
 * Compte le nombre de bords incidents aux brins marqués avec la marque
 * AMarkNumber. Pour ne compter que certains bords, mettre à NULL les
 * paramêtres ANbi correspondant à une dimension non voulue.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ANbi Le nombre de i-bords
 * @param ANb0Coupled Le nombre de faces ayant deux 0-bords
 */
void countBorders(int AMarkNumber,
		  int* ANb0, int* ANb1, int* ANb2, int* ANb3);

/**
 * Compte le nombre de bords incidents à l'orbite (ADart, AOrbit).
 * Pour ne compter que certains bords, mettre à NULL les paramètres ANb-
 * correspondant à une dimension non voulue.
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @param ANbi Le nombre de i-bords
 */
void countBorders(CDart* ADart, TOrbit AOrbit,
		  int* ANb0, int* ANb1, int* ANb2, int* ANb3);

/** Count the number of edges with local degree different from two.
 * Remark: if this number is equal to zero, the map corresponds to a
 * 2 dimensional quasi-manifols.
 * @return the number of edges with local degree different from two.
 */
unsigned int countNonLocalDegreeTwoEdges();

/**
 * Compte le nombre de cellules incidentes à un brin marqué avec la marque
 * AMarkNumber. Pour ne compter que certains types de cellules, mettre à
 * NULL les paramètres ANb- correspondant à une dimension non voulue.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ANbi Les nombres de i-cellules
 */
void countCells(int AMarkNumber,
		int* ANb0, int* ANb1, int* ANb2, int* ANb3, int* ANb4,
		int* ANbDarts = NULL);

/**
 * Compte le nombre de cellules incidentes ou incluses dans l'orbite
 * (ADart, AOrbit). Pour ne compter que certains types de cellules, mettre
 * à NULL les paramètres ANb- correspondant à une dimension non voulue.
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @param ANbi Les nombres de i-cellules
 */
void countCells(CDart* ADart, TOrbit AOrbit,
		int* ANb0, int* ANb1, int* ANb2, int* ANb3, int* ANb4,
		int* ANbDarts = NULL);

/**
 * Calcule les caractéristiques globales de la carte (nombre de brins,
 * de cellules, de bords). Pour ne pas calculer certaines caractéristiques,
 * mettre à NULL les paramètres correspondants.
 *
 * @param ANbDarts Le nombre de brins de la carte
 * @param ANb{Vertices,...,CC} Le nombre de cellules de la carte
 * @param ANbiBorders Le nombre de i-bords de la carte
 */
void getGlobalCharacteristics(int* ANbDarts,
			      int* ANbVertices, int* ANbEdges,
			      int* ANbFaces, int* ANbVolumes, int* ANbCC,
			      int* ANb0Borders, int* ANb1Borders,
			      int* ANb2Borders, int* ANb3Borders);

/** Display all the characteristics of the map (number of darts and
 *   number of cells.
 */
std::ostream& displayCharacteristics(std::ostream &AOs);

/**
 * Calcule les caractéristiques d'une surface dont un brin est donné
 * (nombre de brins, de cellules, de bords, caractéristique d'Euler,
 * orientabilité, genre). Pour ne pas calculer certaines caractéristiques,
 * mettre à NULL les paramètres correspondants.
 *
 * @param ADart Un brin du volume
 * @param ANbDarts Le nombre de brins du volume
 * @param ANb{Vertices,...,CC} Le nombre de cellules du volume
 * @param ANbiBorders Le nombre de i-bords du volume
 * @param ANb2BordersWhenClosed Le nombre total de 2-bords qu'on aurait si
 *   le volume incident à ADart était 0-fermé puis 1-fermé.
 * @param AEuler La caractéristique d'Euler-Poincaré du volume
 * @param AOrient Le coefficient d'orientabilité du volume
 * @param AGenus Le genre du volume
 */
void getSurfaceCharacteristics(CDart* ADart,
			       int* ANbDarts,
			       int* ANbVertices, int* ANbEdges,
			       int* ANbFaces,
			       int* ANb0Borders, int* ANb1Borders,
			       int* ANb2Borders,
			       int* ANb2BordersWhenClosed,
			       int* AEuler, int* AOrient, int* AGenus);

/**
 * Détermine à partir des caractéristiques topologiques d'une surface son
 * nom usuel.
 *
 * @param AB2 Le nombre de 2-bords de la surface
 * @param AQ Le coefficient d'orientabilité de la surface
 * @param AG Le genre de la surface
 * @return Le nom de la surface (par exemple, "tore à 2 trous", "bouteille
 *   de Klein", etc)
 */
std::string getSurfaceName(int AB2, int AQ, int AG);

/**
 * Teste si la carte ne contient qu'une seule composante connexe.
 * Si c'est le cas, la valeur 'vrai' est retournée.
 *
 * @return 'vrai' si la carte est connexe
 */
bool isConnex();

/**
 * Vérifie l'intégrité de la carte.
 * La carte est cohérente si les orbites 0, 1, 2 et 3 sont des permutations
 * et si les orbites 02, 03 et 13 sont des involutions.
 *
 * @return Un booléen indiquant si la topologie de la carte est cohérente
 */
bool checkTopology();

/**
 * Vérifie si pour chaque cellule d'orbite AOrbit de la carte il existe un
 * seul brin de la cellule qui possède le plongement identifié par
 * AAttributeIdentity.
 * Si AEmbeddingMustExist vaut 'vrai', une cellule non plongée est
 * considérée comme un problème d'intégrité de la carte.
 *
 * @param AOrbit l'orbite plongée à tester
 * @param AAttributeIdentity L'identificateur de l'attribut à vérifier
 * @param AEmbeddingMustExist Un booléen indiquant si la détection d'une
 *   orbite non plongée doit provoquer une erreur
 * @return Un booléen indiquant si les plongements sont cohérents
 *
 * @precondition isOrbitUsed(AOrbit);
 */
bool checkEmbeddings(TOrbit AOrbit, int AAttributeIdentity,
		     bool AEmbeddingMustExist = true);

/**
 * Teste si tous les brins de l'orbite (ADart, AOrbit) sont cousus
 * par alpha ADimension.
 * Si c'est le cas, la méthode retourne vrai.
 *
 * @param ADart Un brin de la carte
 * @param ADimension Une dimension quelconque (0, 1, 2 ou 3)
 * @param AOrbit Une orbite quelconque
 * @return Un booléen
 */
bool isIClosedOrbit(CDart* ADart, int ADimension, TOrbit AOrbit);

/**
 * Teste si tous les brins de l'orbite (ADart, AOrbit) sont non cousus
 * par alpha ADimension.
 * Si c'est le cas, la méthode retourne vrai.
 *
 * @param ADart Un brin de la carte
 * @param ADimension Une dimension quelconque (0, 1, 2 ou 3)
 * @param AOrbit Une orbite quelconque
 * @return Un booléen
 */
bool isIFreeOrbit(CDart* ADart, int ADimension, TOrbit AOrbit);

/**
 * @return la dimension de la carte, c'est à dire le plus grand i tel qu'il 
 *         existe un brin non i-libre. Retourne -1 si tout les brins sont libre
 *         pour tout les i. 
 */
int getMapDimension();

/**
 * @return true iff the face containing dart ADart is dangling.
 */
bool isDanglingFace(CDart* ADart);
//******************************************************************************
