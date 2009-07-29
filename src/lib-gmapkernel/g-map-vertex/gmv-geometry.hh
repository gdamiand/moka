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
 * Calcule le barycentre d'un ensemble de sommets déterminé par un brin ADart
 * et une orbite AOrbit.
 * Les brins marqués ne sont pas démarqués à la fin du parcours.
 *
 * La méthode utilise un itérateur qui marque les brins avec les marques
 * ATreatedMark et AOrbitMark et qui n'effectue pas de démarquage à la fin.
 *
 * Si ADirectInfoVertex est positif ou nul, le plongement des sommets n'est
 * pas recherché à l'aide de la méthode 'findVertex' mais est accédé
 * directement à l'aide du champ directInfo[ADirectInfoVertex].
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @param AOrbitMark Le numéro de marque utilisé par l'itérateur pour le
 *   marquage des brins atteints
 * @param ATreatedMark Le numéro de marque utilisé pour le marquage des brins
 *   déjà traités
 * @param ADirectInfoVertex Un indice correspondant à un champ directInfo
 *
 * @precondition AOrbit==ORBIT012 || AOrbit==ORBIT123 || AOrbit==ORBIT0123
 */
CVertex basicBarycenter(CDart* ADart, TOrbit AOrbit,
			int AOrbitMark, int ATreatedMark,
			int ADirectInfoVertex = -1);

/**
 * Calcule le barycentre d'un ensemble de sommets déterminé par un brin ADart
 * et une orbite AOrbit.
 *
 * Si ADirectInfoVertex est positif ou nul, le plongement des sommets n'est
 * pas recherché à l'aide de la méthode 'findVertex' mais est accédé
 * directement à l'aide du champ directInfo[ADirectInfoVertex].
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @param ADirectInfoVertex Un indice correspondant à un champ directInfo
 */
CVertex barycenter(CDart* ADart, TOrbit AOrbit, int ADirectInfoVertex = -1);

/**
 * Calcule le barycentre d'un ensemble de sommets marqués avec la marque
 * AMarkNumber.
 *
 * @param AMarkNumber Un numéro de marque
 */
CVertex barycenter(int AMarkNumber);

/**
 * Calcule le barycentre d'un ensemble de sommets se trouvant dans le
 * champ directInfo d'indice ADirectInfoVertex.
 * Les champs directinfo doivent être initialisés avec la méthode
 * duplicateVertexToDirectInfo.
 *
 * @param ADirectInfoVertex Un indice correspondant à un champ directInfo
 */
CVertex directInfoBarycenter(int ADirectInfoVertex);

/**
 * Calcule la boîte englobante d'un ensemble de sommets dont un brin au
 * moins est marqué avec la marque AMarkNumber.
 *
 * @param AMarkNumber Un numéro de marque
 * @param AMin Le point minimum de la boîte englobante
 * @param AMax Le point maximum de la boîte englobante
 */
void boundingBox(int AMarkNumber, CVertex & AMin, CVertex & AMax);

/**
 * Calcule la boîte englobante d'un ensemble de sommets dont un brin au
 * moins est marqué avec la marque AMarkNumber puis retourne le centre de
 * cette boîte englobante.
 *
 * @param AMarkNumber Un numéro de marque
 * @return Le centre de la boîte englobante
 */
CVertex centerOfBoundingBox(int AMarkNumber);

/**
 * Calcule la boîte englobante d'un ensemble de sommets situés sur l'orbite
 * (ADart, AOrbit).
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @param AMin Le point minimum de la boîte englobante
 * @param AMax Le point maximum de la boîte englobante
 */
void boundingBox(CDart* ADart, TOrbit AOrbit, CVertex & AMin, CVertex & AMax);

/**
 * Calcule la boîte englobante d'un ensemble de sommets situés sur l'orbite
 * (ADart, AOrbit) puis retourne le centre de cette boîte englobante.
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite quelconque
 * @return Le centre de la boîte englobante
 */
CVertex centerOfBoundingBox(CDart* ADart, TOrbit AOrbit);

/**
 * Calcule la longueur d'une arête.
 *
 * @param ADart Un brin de la carte
 * @return La longueur de l'arête incidente à ADart
 */
TCoordinate edgeLength(CDart* ADart);

/**
 * Calcule le périmètre d'une face.
 *
 * @param ADart Un brin de la carte
 * @return Le périmètre de la face incidente à ADart
 */
TCoordinate facePerimeter(CDart* ADart);

/**
 * Calcule la longueur d'un 1-bord, c'est-à-dire la longueur de l'arête qui
 * permettrait de boucher ce 1-bord.
 *
 * @param ADart Un brin de la carte
 * @return La longueur du 1-bord incident à ADart
 */
TCoordinate border1Length(CDart* ADart);

/**
 * Calcule la longueur d'une 2-bord, c'est-à-dire la somme de la longueur de
 * toutes les arêtes qui le constituent.
 *
 * @param ADart Un brin de la carte
 * @return La longueur du 2-bord incident à ADart
 */
TCoordinate border2Length(CDart* ADart);

/**
 * Calcule * findVertex(alpha0(ADart)) - * findVertex(ADart).
 *
 * @param ADart Un brin de la carte
 * @return Le vecteur correpondant à l'arête incidente à ADart
 */
CVertex edgeVector(CDart* ADart);

/**
 * Calcule le vecteur normal d'une arête. Ce vecteur n'est jamais nul.
 * Si l'arête est de longueur nulle, la méthode retourne OX, sinon
 * elle appelle 'CGeometry::getNormalVector'.
 *
 * @param ADart Un brin de la carte
 * @return Le vecteur normal de l'arête incidente à ADart
 */
CVertex edgeNormalVector(CDart* ADart);

/**
 * Calcule le vecteur normal d'une face. Ce vecteur n'est jamais nul.
 * La face n'est pas forcément plane. Elle peut être ouverte.
 * Elle peut comporter moins de 3 sommets (le résultat est alors OZ).
 *
 * @param ADart Un brin de la carte
 * @return Le vecteur normal de la face incidente à ADart
 */
CVertex faceNormalVector(CDart* ADart);

/**
 * Calcule le vecteur normal d'une cellule en s'appuyant sur les méthodes
 * 'edgeNormalVector' et 'faceNormalVector'.
 *
 * @param ADim La dimension de la cellule (0, 1 ou 2)
 * @param ADart Un brin incident à la cellule
 * @return Le vecteur normal de la cellule
 */
CVertex cellNormalVector(int ADim, CDart* ADart);

/**
 * Calcule le vecteur normal d'un 2-bord. Ce vecteur n'est jamais nul.
 * Le 2-bord n'est pas forcément plan. Il peut être ouvert.
 * Il peut comporter moins de 3 sommets (le résultat est alors OZ).
 *
 * @param ADart Un brin de la carte
 * @return Le vecteur normal du 2-bord incident à ADart
 */
CVertex border2NormalVector(CDart* ADart);

/**
 * Calcule la moyenne des vecteurs normaux des faces (restreintes au volume
 * auquel appartient ADart) incidentes à la cellule de dimension ADim à
 * laquelle ADart appartient.
 * Les faces 3-cousues ne sont pas prises en compte dans le calcul.
 * Le vecteur calculé peut être nul.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (0, 1 ou 2)
 * @return La moyenne des vecteurs normaux des faces de la région donnée
 */
CVertex regionNormalVector(CDart* ADart, int ADim);

/**
 * Calcule la moyenne des vecteurs normaux des faces incidentes à la cellule
 * de dimension ADim à laquelle ADart appartient.
 * Seules les faces dont un brin commun avec l'orbite
 * (ADart, ORBIT_INF[ADart]) est marqué sont prises en compte.
 * Le vecteur calculé peut être nul.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (0, 1 ou 2)
 * @param AMarkNumber Un numéro de marque
 * @return La moyenne des vecteurs normaux des faces de la région donnée
 */
CVertex regionNormalVector(CDart* ADart, int ADim, int AMarkNumber);

/**
 * Calcule la moyenne des vecteurs normaux des faces de la cellule de
 * dimension ADim incidente à ADart.
 * Si la cellule n'est pas orientable, le vecteur retourné est nul.
 *
 * @param ADart Un brin de la carte
 * @param ADim Une dimension (3 ou 4)
 * @return Un vecteur (éventuellement nul)
 */
CVertex cellDimensionNormalVector(CDart* ADart, int ADim);

protected:

/**
 * Calcule la longueur de la circonférence d'une orbite,
 * c'est-à-dire la somme de la longueur de toutes les arêtes qui la
 * constituent.
 * Ce calcul n'alpha de sens que si AOrbit==ORBIT01 ou si AOrbit==ORBIT_BORDER1
 * ou si AOrbit==ORBIT_BORDER2.
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite
 * @return La longueur de la circonférence de l'orbite
 */
TCoordinate orbitLength(CDart* ADart, TOrbit AOrbit);

/**
 * Calcule le vecteur normal d'une orbite.
 * Ce calcul n'alpha de sens que si AOrbit==ORBIT01 ou si AOrbit==ORBIT_BORDER1
 * ou si AOrbit==ORBIT_BORDER2.
 *
 * @param ADart Un brin de la carte
 * @param AOrbit Une orbite
 * @return Le vecteur normal de l'orbite
 */
CVertex orbitNormalVector(CDart* ADart, TOrbit AOrbit);

//******************************************************************************
