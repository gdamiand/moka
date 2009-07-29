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
 * Teste si le maillage 1d d'extrémités AMesh1Corners[0] et
 * AMesh1Corners[1] peut être fusionné avec l'arête donnée par le brin
 * AEdgeDart.
 *
 * @param AEdgeDart Un brin de l'arête à mailler
 * @param AMesh1Corners Un tableau de 2 pointeurs sur les brins
 *   correspondant aux extrémités du maillage 1d à fusionner
 * @return 'vrai' si la fusion est possible
 */
bool canMergeEdgeAndMesh1(CDart* AEdgeDart,
			  CDart* AMesh1Corners[2]);

/**
 * Fusionne un maillage 1d avec une arête.
 * Les brins supprimés dans la fusion sont ceux situés aux extrémités du
 * maillage 1d. Aucun brin de l'arête n'est supprimé.
 *
 * @param AEdgeDart Un brin de l'arête à mailler
 * @param AMesh1Corners Un tableau de 2 pointeurs sur les brins
 *   correspondant aux extrémités du maillage 1d à fusionner avec l'arête
 *
 * @precondition canMergeEdgeAndMesh1(AEdgeDart, AMesh1Corners)
 */
void mergeEdgeAndMesh1(CDart* AEdgeDart, CDart* AMesh1Corners[2]);

/**
 * Teste si le maillage 2d incident à AMesh2Dart peut être fusionné avec le
 * carré maillé 1d incident à ASquareDart.
 * AMesh2Dart doit correspondre au brin AMesh2Corners[0][0] de la méthode
 * 'createTopoMesh2'.
 * Le principe est le même pour ASquareDart.
 *
 * @param ASquareDart Un brin du carré maillé 1d
 * @param AMesh2Dart Un brin du maillage 2d
 * @param ASx Le nombre de subdivisions du maillage 2d selon la première
 *   dimension
 * @param ASy Le nombre de subdivisions du maillage 2d selon la deuxième
 *   dimension
 * @return 'vrai' si la fusion est possible
 */
bool canMergeSquare1MeshedAndMesh2(CDart* ASquareDart, CDart* AMesh2Dart,
				   int ASx, int ASy);

/**
 * Fusionne un maillage 2d avec une face carrée 1-maillée.
 * Les brins supprimés dans la fusion sont ceux situés au bord du
 * maillage 2d. Aucun brin du carré 1-maillé n'est supprimé.
 *
 * @param ASquareDart Un brin du carré maillé 1d
 * @param AMesh2Dart Un brin du maillage 2d
 * @param ASx Le nombre de subdivisions du maillage 2d selon la première
 *   dimension
 * @param ASy Le nombre de subdivisions du maillage 2d selon la deuxième
 *   dimension
 *
 * @precondition canMergeSquare1MeshedAndMesh2(ASquareDart, AMesh2Dart, ASx,ASy)
 */
void mergeSquare1MeshedAndMesh2(CDart* ASquareDart, CDart* AMesh2Dart,
				int ASx, int ASy);

/**
 * Teste si les ASx*ASy faces (correspondant à une face du maillage 3d)
 * incidentes à AMesh3Dart peuvent être fusionnées avec le carré 2-maillé
 * incident à ASquareDart.
 * AMesh3Dart doit correspondre au brin AMesh3Corners[0][0][0] de la
 * méthode 'createTopoMesh3' et ASquareDart au brin AMesh2Corners[0][0] de
 * la méthode 'createTopoMesh2'.
 *
 * @param ASquareDart Un brin du carré maillé 2d
 * @param AMesh3Dart Un brin du maillage 3d
 * @param ASx Le nombre de subdivisions du maillage 2d selon la première
 *   dimension
 * @param ASy Le nombre de subdivisions du maillage 2d selon la deuxième
 *   dimension
 */
bool canMergeSquare2MeshedAndMesh3Side(CDart* ASquareDart,
				       CDart* AMesh3Dart,
				       int ASx, int ASy);

/**
 * Fusionne un côté d'un maillage 3d avec une face carrée 2-maillée.
 * Les brins supprimés dans la fusion sont ceux situés au bord du maillage
 * 3d. Aucun brin du carré 2-maillé n'est supprimé.
 *
 * ASx=3, ASy=2
 *
 *           |     |     |     |
 *           +-- --+-- --+-- ==+ ANextSquare
 *           +-- --+-- --+-- --+
 *           |     |     |     |
 *           |     |     |     |
 *           +-- --+-- --+-- --+
 *           |     |     |     |
 *           |     |     |     |
 *           +== --+-- --+-- --+
 * ASquareDart
 *
 * Remarque: Après la fusion, toutes les faces du maillage 2d se retrouvent
 * sur des volumes (des cubes) différents.
 *
 * Si ANextSquare n'est pas égal à NULL, il est positionné sur un brin
 * correspondant initialement à l'alpha2 du brin situé à l'autre extrémité
 * de ASquareDart (voir schéma).
 * Si ANextMesh3Side n'est pas égal à NULL, il est positionné sur
 * l'homologue de ANextSquare sur le maillage 3d.
 *
 * Ces paramètres en sortie permettent de poursuivre la fusion sur d'autres
 * versants du maillage 3d (voir la méthode 'mergeCube2MeshedAndMesh3').
 *
 * @param ASquareDart Un brin du carré maillé 2d
 * @param AMesh3Dart Un brin du maillage 3d
 * @param ASx Le nombre de subdivisions du maillage 2d selon la première
 *   dimension
 * @param ASy Le nombre de subdivisions du maillage 2d selon la deuxième
 *   dimension
 * @param ANextSquare L'alpha2 du brin situé dans le coin opposé à
 *   ASquareDart sur le carré 2-maillé (paramètre en sortie)
 * @param ANextMesh3Side L'homologue de ANextSquare sur le maillage 3d
 *   (paramètre en sortie)
 *
 * @precondition canMergeSquare2MeshedAndMesh3(ASquareDart, AMesh3Dart,
 *                                             ASx,ASy)
 */
void mergeSquare2MeshedAndMesh3Side(CDart* ASquareDart, CDart* AMesh3Dart,
				    int ASx, int ASy,
				    CDart** ANextSquare = NULL,
				    CDart** ANextMesh3Side = NULL);

/**
 * Teste si le maillage 3d incident à AMesh3Dart peut être fusionné avec le
 * cube maillé 2d incident à ACubeDart.
 * AMesh3Dart doit correspondre au brin AMesh3Corners[0][0][0] de la
 * méthode 'createTopoMesh3'.
 * Le principe est le même pour ACubeDart.
 *
 * @param ACubeDart Un brin du cube maillé 2d
 * @param AMesh3Dart Un brin du maillage 3d
 * @param ASx Le nombre de subdivisions du maillage 3d selon la première
 *   dimension
 * @param ASy Le nombre de subdivisions du maillage 3d selon la deuxième
 *   dimension
 * @param ASz Le nombre de subdivisions du maillage 3d selon la troisième
 *   dimension
 * @return 'vrai' si la fusion est possible
 */
bool canMergeCube2MeshedAndMesh3(CDart* ACubeDart, CDart* AMesh3Dart,
				 int ASx, int ASy, int ASz);

/**
 * Fusionne un maillage 3d avec un cube maillé 2d.
 * Les brins supprimés dans la fusion sont ceux situés au bord du maillage 3d.
 * Aucun brin du cube 2-maillé n'est supprimé.
 *
 * @param ACubeDart Un brin du cube maillé 2d
 * @param AMesh3Dart Un brin du maillage 3d
 * @param ASx Le nombre de subdivisions du maillage 3d selon la première
 *   dimension
 * @param ASy Le nombre de subdivisions du maillage 3d selon la deuxième
 *   dimension
 * @param ASz Le nombre de subdivisions du maillage 3d selon la troisième
 *   dimension
 * @return 'vrai' si la fusion est possible
 *
 * @precondition canMergeCubeMeshed2AndMesh3(ACubeDart, AMesh3Dart,
 *                                           ASx,ASy,ASz)
 */
void mergeCube2MeshedAndMesh3(CDart* ACubeDart, CDart* AMesh3Dart,
			      int ASx, int ASy, int ASz);

//******************************************************************************
