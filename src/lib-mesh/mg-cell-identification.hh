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
 * Teste si l'arête incidente à ADart est constituée de deux sommets.
 *
 * Si AModel est non NULL, la méthode vérifie en plus que ADart et AModel
 * ont bien des orbites 23 similaires.
 *
 * @param ADart Un brin de la carte
 * @param AModel Un brin de la carte
 * @return Un booléen indiquant si le brin ADart est 0-cousu
 */
bool isTopoEdge(CDart* ADart, CDart* AModel = NULL);

/**
 * Teste si la face incidente à ADart est bien constituée de ASx arêtes (au
 * moins) toutes 0-cousues.
 * Attention: ADart doit être situé à l'extrémité de la polyligne.
 *
 * Si AModel est non NULL, la méthode vérifie que les brins de l'orbite 01
 * partant de ADart ont bien des orbites 23 similaires avec celle de
 * AModel.
 * Si AModel est NULL, aucune vérification n'est faite sur les orbites 23.
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions
 * @param AModel Un brin de la carte
 * @return Un booléen indiquant si ADart est à l'extrémité d'un polyligne
 *   constituée de ASx arêtes
 */
bool isTopoEdge1Meshed(CDart* ADart, int ASx, CDart* AModel = NULL);

/**
 * Cette méthode est fournie dans le but de faciliter l'exploitation du noyau.
 * Elle appelle simplement l'une des méthodes 'isTopoEdge' ou
 * 'isTopoEdge1Meshed' en fonction de la valeur de ADim.
 * Si ADim vaut 0, la valeur de AS n'est pas prise en compte.
 *
 * @param ADim Une dimension (0 ou 1)
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions
 * @param AModel Un brin de la carte
 * @return Un booléen indiquant si la face incidente à ADart est un carré
 *   maillé en dimension ADim
 */
bool isTopoEdgeIMeshed(int ADim, CDart* ADart, int AS = 1,
		       CDart* AModel = NULL);

/**
 * Teste si la face incidente à ADart est fermée et constituée de 4 arêtes.
 * Si ACheck3Sews est vrai, tous les brins B de la face doivent vérifier
 * isFree3(B) != A3Sewed.
 * Si ACheck3Sews est faux, la vérification sur les 3-coutures n'est pas
 * faite (la valeur de A3Sewed n'est pas prise en compte).
 *
 * @param ADart Un brin de la carte
 * @param ACheck3Sews Un booléen indiquant s'il faut vérifier l'état des
 *   3-coutures
 * @param A3Sewed Un booléen indiquant si le carré doit être 3-cousu
 * @return Un booléen indiquant si la face incidente à ADart est un carré
 */
bool isTopoSquare(CDart* ADart,
		  bool ACheck3Sews = false, bool A3Sewed = false);

/**
 * Teste si la face incidente à ADart est fermée et constituée de
 * 2*(ASx+ASy) arêtes.
 * Si ACheck3Sews est vrai, alors:
 *   Si A3Sewed vaut 'vrai', la face doit être 3-cousue.
 *     Si A3Sewed vaut 'faux', la face doit ne pas être 3-cousue.
 * Si ACheck3Sews est faux, la vérification sur les 3-coutures n'est pas
 * faite (la valeur de A3Sewed n'est pas prise en compte).
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ACheck3Sews Un booléen indiquant s'il faut vérifier l'état des
 *   3-coutures
 * @param A3Sewed Un booléen indiquant si la face doit être 3-cousue
 * @return Un booléen indiquant si la face incidente à ADart est un carré
 *   maillé 1d
 */
bool isTopoSquare1Meshed(CDart* ADart, int ASx, int ASy,
			 bool ACheck3Sews = false, bool A3Sewed = false);

/**
 * Teste si la grille incidente à ADart est constituée de ASx*ASy carrés
 * au moins.
 *
 * ADart doit être situé dans un coin du maillage et dirigé selon la
 * première dimension (comme l'est le brin AMesh2Corners[0][0] de la
 * méthode 'createTopoMesh2').
 *
 * Si ACheck3Sews est vrai, alors:
 *   Si A3Sewed vaut 'vrai', toutes les faces du maillage doivent être
 *     3-cousues.
 *   Si A3Sewed vaut 'faux', toutes les faces du maillage doivent être
 *     3-libres.
 * Si ACheck3Sews est faux, la vérification sur les 3-coutures n'est pas
 * faite (la valeur de A3Sewed n'est pas prise en compte).
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ACheck3Sews Un booléen indiquant s'il faut vérifier l'état des
 *   3-coutures
 * @param A3Sewed Un booléen indiquant si les faces du maillage doivent
 *   être 3-cousues
 * @return Un booléen indiquant si la face incidente à ADart est un carré
 *   maillé 2d
 */
bool isTopoSquare2Meshed(CDart* ADart, int ASx, int ASy,
			 bool ACheck3Sews = false, bool A3Sewed = false);

/**
 * Teste si la grille incidente à ADart est constituée de ASx*ASy carrés
 * au moins correspondant à une face d'un cube 3-maillé.
 *
 * ADart doit être situé dans un coin du maillage et dirigé selon la
 * première dimension (comme l'est le brin AMesh2Corners[0][0] de la
 * méthode 'createTopoMesh2').
 *
 * Si ACheck3Sews est vrai, alors:
 *   Si A3Sewed vaut 'vrai', toutes les faces du maillage doivent être
 *     3-cousues.
 *   Si A3Sewed vaut 'faux', toutes les faces du maillage doivent être
 *     3-libres.
 * Si ACheck3Sews est faux, la vérification sur les 3-coutures n'est pas
 * faite (la valeur de A3Sewed n'est pas prise en compte).
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ACheck3Sews Un booléen indiquant s'il faut vérifier l'état des
 *   3-coutures
 * @param A3Sewed Un booléen indiquant si les faces du maillage doivent
 *   être 3-cousues
 * @return Un booléen indiquant si la face incidente à ADart est un carré
 *   maillé 3d
 */
bool isTopoSquare3Meshed(CDart* ADart, int ASx, int ASy,
			 bool ACheck3Sews = false, bool A3Sewed = false);

/**
 * Cette méthode permet de tester dans n'importe quelle dimension si un
 * carré est maillé.
 *
 * Elle appelle simplement l'une des méthodes 'isTopoSquare' ou
 * 'isTopoSquare1Meshed' si ADim<2. Sinon, elle teste si le carré est 2
 * ou 3 maillé. L'algorithme est le même que le cube soit 2 ou 3 maillé,
 * sinon que dans un cas on passe d'un carré à son successeur par la
 * fonction alpha0121 (ADim==2) et dans l'autre par la fonction
 * alpha012321 (ADim==3).
 *
 * @param ADim Une dimension (0, 1, 2 ou 3)
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ACheck3Sews Un booléen indiquant s'il faut vérifier l'état des
 *   3-coutures
 * @param A3Sewed Un booléen indiquant si la face doit être 3-cousue
 * @return Un booléen indiquant si la face incidente à ADart est un carré
 *   maillé 2d
 */
bool isTopoSquareIMeshed(int ADim, CDart* ADart, int ASx = 1, int ASy = 1,
			 bool ACheck3Sews = false, bool A3Sewed = false);

/**
 * Cette méthode prend en paramètre un brin incident à un carré maillé en
 * dimension ADim. Le bord de ce carré est composé de 4 polyarêtes.
 * La méthode vérifie qu'aucune arête horizontale n'est marquée avec la
 * marque AMarkNumberNotX1 (ou avec la marque AMarkNumberNotX2 si elle est
 * positive ou nulle) et qu'aucune arête verticale n'est marquée avec la
 * marque AMarkNumberNotY1 (ou avec la marque AMarkNumberNotY2 si elle est
 * positive ou nulle).
 * Pour tester si une arête est marquée ou pas, on ne teste les marques que
 * d'un seul de ses brins. Les arêtes doivent donc être soit totalement
 * marquées soit totalement démarquées.
 *
 * Cette méthode est utilisée par 'meshMarkedSquares', en association avec
 * la méthode 'markTopoSquareIMeshed'.
 *
 * 2==AReturnedDart             AReturnedDart==3
 *              +** --+-- --+-- **+
 *              |     |     |     |
 *              |     |     |     |
 *              +-- --+-- --+-- --+
 *              |     |     |     |
 *              |     |     |     |
 *              +** --+-- --+-- **+
 *            ADart             AReturnedDart==1
 *
 * @param ADim Une dimension (0, 1 ou 2)
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param AMarkNumberNotX1 Un numéro de marque pour les arêtes horizontales
 * @param AMarkNumberNotX2 Un numéro de marque pour les arêtes horizontales
 * @param AMarkNumberNotY1 Un numéro de marque pour les arêtes verticales
 * @param AMarkNumberNotY2 Un numéro de marque pour les arêtes verticales
 * @param AReturnedDart Un entier indiquant quel brin retourner
 * @param AOppositeDart Un brin situé sur un autre coin du maillage,
 *   dépendant de la valeur de AReturnedDart (voir schéma)
 * @return Un booléen indiquant si le carré (orbite 01) est libre
 */
bool isFreeTopoSquareIMeshed(int ADim, CDart* ADart, int ASx, int ASy,
			     int AMarkNumberNotX1, int AMarkNumberNotX2,
			     int AMarkNumberNotY1, int AMarkNumberNotY2,
			     int AReturnedDart = 3,
			     CDart** AOppositeDart = NULL);

/**
 * Cette méthode prend en paramètre un brin incident à un cube maillé en
 * dimension ADim. Le bord de ce cube est composé de 12 polyarêtes.
 * La méthode vérifie qu'aucune arête de la première dimension n'est
 * marquée avec la marque AMarkNumberNotX1 (ou avec la marque
 * AMarkNumberNotX2 si elle est positive ou nulle), qu'aucune arête de la
 * deuxième dimension n'est marquée avec la marque AMarkNumberNotY1 (ou
 * avec la marque AMarkNumberNotY2 si elle est positive ou nulle) et
 * qu'aucune arête de la troisième dimension n'est marquée avec la marque
 * AMarkNumberNotZ1 (ou avec la marque AMarkNumberNotZ2 si elle est
 * positive ou nulle).
 * Pour tester si une arête est marquée ou pas, on ne teste les marques que
 * d'un seul de ses brins. Les arêtes doivent donc être soit totalement
 * marquées soit totalement démarquées.
 *
 * Cette méthode est utilisée par 'meshMarkedCubes', en association avec la
 * méthode 'markTopoCubeIMeshed'.
 *
 * @param ADim Une dimension (0, 1 ou 2)
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @param AMarkNumberNotX1 Un numéro de marque pour les arêtes de la
 *   première dimension
 * @param AMarkNumberNotX2 Un numéro de marque pour les arêtes de la
 *   première dimension
 * @param AMarkNumberNotY1 Un numéro de marque pour les arêtes de la
 *   deuxième dimension
 * @param AMarkNumberNotY2 Un numéro de marque pour les arêtes de la
 *   deuxième dimension
 * @param AMarkNumberNotZ1 Un numéro de marque pour les arêtes de la
 *   troisième dimension
 * @param AMarkNumberNotZ2 Un numéro de marque pour les arêtes de la
 *   troisième dimension
 * @return Un booléen indiquant si le cube est libre
 */
bool isFreeTopoCubeIMeshed(int ADim, CDart* ADart,
			   int ASx, int ASy, int ASz,
			   int AMarkNumberNotX1, int AMarkNumberNotX2,
			   int AMarkNumberNotY1, int AMarkNumberNotY2,
			   int AMarkNumberZ1, int AMarkNumberZ2);

/**
 * Cette méthode teste si les brins ADart1 et ADart2 sont bien de part et
 * d'autre (c'est-à-dire 2-cousus) d'une arête d'un cube maillé.
 * Le test est effectué sur AS arêtes consécutives.
 *
 * @param ADim Une dimension (1 ou 2)
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param AS Le nombre de subdivisions de l'arête
 * @return NULL ou l'extrémité de la polyligne
 */
CDart* isTopoEdgeOfCubeIMeshed(int ADim,
			       CDart* ADart1, CDart* ADart2, int AS);

/**
 * Teste si le volume incident à ADart est fermé et constitué de 6 faces
 * carrées formant un cube simple.
 *
 * @param ADart Un brin de la carte
 * @return Un booléen indiquant si le volume incident à ADart est un cube
 */
bool isTopoCube(CDart* ADart);

/**
 * Teste si le volume incident à ADart est un cube maillé 1d selon les
 * valeurs de ASx, ASy et ASz.
 *
 * ADart doit être situé dans un coin du maillage et dirigé selon la
 * première dimension (comme l'est le brin AMesh3Corners[0][0][0] de la
 * méthode 'createTopoMesh3').
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @return Un booléen indiquant si le volume incident à ADart est un cube
 *   dont les arêtes sont maillées
 */
bool isTopoCube1Meshed(CDart* ADart, int ASx, int ASy, int ASz);

/**
 * Teste si le volume incident à ADart est un cube maillé 2d selon les
 * valeurs de ASx, ASy et ASz.
 *
 * ADart doit être situé dans un coin du maillage et dirigé selon la
 * première dimension (comme l'est le brin AMesh3Corners[0][0][0] de la
 * méthode 'createTopoMesh3').
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @return Un booléen indiquant si le volume incident à ADart est un cube
 *   dont les arêtes sont maillées
 */
bool isTopoCube2Meshed(CDart* ADart, int ASx, int ASy, int ASz);

/**
 * Teste si le volume incident à ADart est un cube maillé 3d selon les
 * valeurs de ASx, ASy et ASz.
 *
 * ADart doit être situé dans un coin du maillage et dirigé selon la
 * première dimension (comme l'est le brin AMesh2Corners[0][0][0] de la
 * méthode 'createTopoMesh3').
 *
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @return Un booléen indiquant si le volume incident à ADart est un cube
 *   dont les arêtes sont maillées
 */
bool isTopoCube3Meshed(CDart* ADart, int ASx, int ASy, int ASz);

/**
 * Teste si le volume incident à ADart est un cube maillé en dimension ADim
 * selon les valeurs de ASx, ASy et ASz.
 *
 * ADart doit être situé dans un coin du maillage et dirigé selon la
 * première dimension (comme l'est le brin AMesh3Corners[0][0][0] de la
 * méthode 'createTopoMesh3').
 *
 * Si ADim vaut 0, les valeurs de ASx, ASy et ASz ne sont pas prises en compte.
 *
 * @param ADim Une dimension (0, 1, 2 ou 3)
 * @param ADart Un brin de la carte
 * @param ASx Le nombre de subdivisions selon la première dimension
 * @param ASy Le nombre de subdivisions selon la deuxième dimension
 * @param ASz Le nombre de subdivisions selon la troisième dimension
 * @return Un booléen indiquant si le volume incident à ADart est un cube
 *   maillé en dimension ADim
 */
bool isTopoCubeIMeshed(int ADim, CDart* ADart, int ASx, int ASy, int ASz);

//******************************************************************************
