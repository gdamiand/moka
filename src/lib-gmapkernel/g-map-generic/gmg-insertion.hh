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
 * Insère un sommet topologique sur l'arête incidente au brin ADart.
 *
 * @param ADart Un pointeur sur un brin
 * @return Un brin du sommet topologique inséré
 *
 * @precondition canInsertVertex(ADart)
 */
virtual CDart* insertVertex(CDart* ADart);

/**
 * Insère un sommet sur chaque arête dont un brin au moins est marqué,
 * lorsque cela est possible.
 *
 * @param AMarkNumber Un numéro de marque
 * @return Le nombre d'insertions effectuées
 */
int insertVertexOnMarkedEdges(int AMarkNumber);

/**
 * Insère une arête topologique entre les sommets incidents aux brins
 * ADart1 et ADart2.
 *
 * @param ADart1 Un pointeur sur un brin
 * @param ADart2 Un pointeur sur un brin
 * @return Un brin de l'arête topologique insérée
 *
 * @precondition canInsertEdge(ADart1, ADart2)
 */
virtual CDart* insertEdge(CDart* ADart1, CDart* ADart2);

/**
 * Insère une arête sur chaque face dont deux sommets sont sélectionnés,
 * lorsque cela est possible.
 * Si ANoCrossedFace est faux, une arête n'est insérée sur une face que si
 * exactement 2 brins de cette face sont marqués.
 * Si ANoCrossedFace est vrai, une arête n'est insérée sur une face que si
 * 2 sommets de cette face sont marqués (2, 3 ou 4 brins marqués).
 *
 * @param AMarkNumber Un numéro de marque
 * @param ANoCrossedFace Un booléen indiquant s'il faut vérifier qu'on ne
 *   va pas créer une face 'croisée', c'est-à-dire une face en forme
 *   de 8, avec deux arêtes 2-cousues qui se croisent
 * @param ANoTwoEdgesFace Un booléen indiquant s'il faut vérifier qu'on ne
 *   va pas créer de face fermée composée de seulement deux arêtes
 * @return Le nombre d'arêtes insérées
 */
int insertEdgeOnMarkedFaces(int AMarkNumber,
			    bool ANoCrossedFace = false,
			    bool ANoTwoEdgesFace = false);

/**
 * Teste s'il est possible d'insérer une face topologique dans un volume.
 * Si ANoCrossedVolume est vrai, au moins un brin de chaque demi-arête du
 * volume (orbite 0) correspondant à la face à créer doit être sélectionné.
 * Si ANoCrossedVolume est faux, au moins un brin de chaque arête du volume
 * (orbite 02) correspondant à la face à créer doit être sélectionné.
 *
 * @param ADart Un brin marqué du volume dans lequel insérer la face
 * @param AMarkNumber Un numéro de marque
 * @param ANoCrossedVolume Un booléen indiquant s'il faut vérifier qu'on ne
 *   va pas créer un volume 'croisé', c'est-à-dire un volume en forme
 *   de 8, avec deux faces 3-cousues qui se croisent
 * @param ANoTwoEdgesFace Un booléen indiquant s'il faut vérifier qu'on ne
 *   va pas créer de face fermée composée de seulement deux arêtes
 * @param ANoTwoFacesVolume Un booléen indiquant s'il faut vérifier qu'on
 *   ne va pas créer de volume fermé composé de seulement deux faces
 * @return Un booléen
 */
bool canInsertFace(CDart* ADart, int AMarkNumber,
		   bool ANoCrossedVolume = false,
		   bool ANoTwoEdgesFace = false,
		   bool ANoTwoFacesVolume = false);

/**
 * Insère une face le long des arêtes sélectionnées du volume donné.
 *
 * @param ADart Un brin marqué du volume dans lequel insérer la face
 * @param AMarkNumber Un numéro de marque
 * @param ANoCrossedVolume Un booléen indiquant s'il faut vérifier qu'on ne
 *   va pas créer un volume 'croisé', c'est-à-dire un volume en forme
 *   de 8, avec deux faces 3-cousues qui se croisent
 * @return Un brin de la face créée
 *
 * @precondition canInsertFace(ADart, AMarkNumber)
 */
virtual CDart* insertFace(CDart* ADart, int AMarkNumber,
			  bool ANoCrossedVolume = false);

/**
 * Insère une face sur chaque volume dont plusieurs arêtes sont
 * sélectionnées, lorsque cela est possible.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ANoCrossedVolume Un booléen indiquant s'il faut vérifier qu'on ne
 *   va pas créer un volume 'croisé', c'est-à-dire un volume en forme
 *   de 8, avec deux faces 3-cousues qui se croisent
 * @param ANoTwoEdgesFace Un booléen indiquant s'il faut vérifier qu'on ne
 *   va pas créer de face fermée composée de seulement deux arêtes
 * @param ANoTwoFacesVolume Un booléen indiquant s'il faut vérifier qu'on
 *   ne va pas créer de volume fermé composé de seulement deux faces
 * @return Le nombre de faces insérées
 */
int insertFaceOnMarkedVolumes(int AMarkNumber,
			      bool ANoCrossedVolume = false,
			      bool ANoTwoEdgesFace = false,
			      bool ANoTwoFacesVolume = false);


protected:

/**
 * Cette méthode est utilisée par les méthodes 'canInsertFace' et 'insertFace'.
 * Elle permet de se déplacer le long des arêtes marquées d'une volume.
 * Partant d'un brin donné elle tourne autour d'un sommet (orbite 12)
 * jusqu'à trouver s'il existe un brin marqué d'une autre arête que celle
 * sur laquelle se trouve ADart.
 * Les deux derniers paramètres sont des paramètres en sortie.
 *
 * @param ADart Un brin de la carte, par lequel on est arrivé sur ce sommet
 * @param ANoCrossedVolume Un booléen indiquant si l'insertion de face doit
 *   ne pas entraîner l'apparition d'un volume croisé (voir la méthode
 *   'insertFace')
 * @param AMarkNumber Un numéro de marque permettant de repérer les brins
 *   sélectionnés
 * @param ANext Un pointeur sur un brin (paramètre en sortie uniquement)
 * @param ACrossed Un booléen indiquant si le brin ANext est tel que la
 *   face à insérer est croisée (paramètre en sortie uniquement)
 * @return Un booléen indiquant si le sommet est valide, c'est-à-dire si au
 *   plus de 2 des arêtes qui lui sont incidentes sont sélectionnées
 */
bool turnAroundVertex(CDart* ADart,
		      bool ANoCrossedVolume, int AMarkNumber,
		      CDart* & ANext, bool & ACrossed);

//******************************************************************************
