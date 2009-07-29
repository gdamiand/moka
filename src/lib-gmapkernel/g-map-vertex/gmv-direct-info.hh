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
 * Alloue et si nécessaire initialise (dans le cas où la librairie GMP est
 * utilisée) l'espace mémoire nécessaire pour stocker une valeur de type
 * TCoordinate, ceci pour le champ directInfo[ADirectInfoIndex] des brins
 * marqués avec la marque AMarkNumber.
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est marqué.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param AOrbitUsed Une orbite quelconque
 *
 */
void allocCoordToDirectInfo(int AMarkNumber, int ADirectInfoIndex,
			    TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Alloue et si nécessaire initialise (dans le cas où la librairie GMP est
 * utilisée) l'espace mémoire nécessaire pour stocker une valeur de type
 * TCoordinate, ceci pour le champ directInfo[ADirectInfoIndex] des brins
 * correspondant à l'orbite donnée par le couple (ADart, AOrbit).
 *
 * Si AOrbitUsed est différent de ORBIT_NONE, l'allocation n'est faite que
 * pour le brin porteur du plongement AOrbitUsed de chaque cellule d'orbite
 * AOrbitUsed dont un brin au moins est sur l'orbite (ADart, AOrbit).
 *
 * @param ADart Un brin quelconque de la carte
 * @param AOrbit Une orbite
 * @param ADirectInfoIndex L'indice du champ directInfo à utiliser
 * @param AOrbitUsed Une orbite quelconque
 */
void allocCoordToDirectInfo(CDart* ADart, TOrbit AOrbit, int ADirectInfoIndex,
			    TOrbit AOrbitUsed = ORBIT_NONE);

/**
 * Duplique le plongement sommet des sommets sélectionnés.
 * Le sommet dupliqué est rattaché à un brin de l'orbite sommet, qui est le
 * même que celui qui porte l'attribut original.
 *
 * Un sommet est sélectionné si au moins un de ses brins est marqué avec la
 * marque AMarkNumber.
 *
 * L'espace alloué par cette méthode peut être libéré à l'aide de la méthode
 * 'deleteDuplicatedVertex'.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADirectInfoNumber Un indice indiquant quel directInfo utiliser
 */
void duplicateVertexToDirectInfo(int AMarkNumber, int ADirectInfoNumber);

/**
 * Libère l'espace mémoire alloué par la méthode 'duplicateVertexToDirectInfo'.
 * Les champs directInfo libérés ne sont pas positionnés à NULL.
 *
 * Attention: les brins sélectionnés devraient être les mêmes qu'au moment de
 * l'appel à duplicateVertexToDirectInfo!
 *
 * @param ADirectInfoNumber Un indice indiquant quel directInfo utiliser
 */
void deleteDuplicatedVertex(int ADirectInfoNumber);

/**
 * Copie les attributs sommets vers le champ directInfo spécifié, pour les
 * brins dont ce champ est non NULL.
 *
 * Attention: ces brins sélectionnés devraient être les mêmes qu'au moment de
 * l'appel à duplicateVertexToDirectInfo!
 *
 * @param ADirectInfoNumber Un indice indiquant quel directInfo utiliser
 */
void updateDirectInfoWithVertex(int ADirectInfoNumber);

/**
 * Pour chaque brin b de la carte, fait pointer le champ directInfo numéro
 * ADirectInfoNumber vers le plongement sommet du sommet auquel b appartient.
 *
 * @param ADirectInfoIndex Un indice indiquant quel directInfo utiliser
 */
void pointDirectInfoToAttributeVertex(int ADirectInfoIndex);

/**
 * Pour chaque brin b de l'orbite sommet correspondant à ADart, fait pointer
 * le champ directInfo numéro ADirectInfoNumber vers le plongement sommet du
 * sommet auquel b appartient.
 *
 * @param ADirectInfoIndex Un indice indiquant quel directInfo utiliser
 * @param ADart Un brin de la carte
 */
void pointDirectInfoToAttributeVertex(int ADirectInfoIndex, CDart* ADart);

/**
 * Pour chaque brin b de l'orbite AOrbit correspondant à ADart, fait pointer
 * le champ directInfo numéro ADirectInfoNumber de tous les brins du sommet
 * vers le plongement de ce sommet.
 *
 * @param ADirectInfoIndex Un indice indiquant quel directInfo utiliser
 * @param ADart  Un brin de la carte
 * @param AOrbit Une orbite
 */
void pointDirectInfoToAttributeVertex(int ADirectInfoIndex, CDart* ADart,
				      TOrbit AOrbit);

//******************************************************************************
