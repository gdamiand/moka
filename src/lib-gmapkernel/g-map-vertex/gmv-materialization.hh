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
 * Insère un brin isolé dans la carte dont les coordonnées sont données par
 * le barycentre des sommets incidents au brins de la carte marqués avec la
 * marque AMarkNumber.
 *
 * @param AMarkNumber
 * @return Le brin ayant pour plongement le barycentre
 */
CDart* materializeBarycenter(int AMarkNumber);

/**
 * Matérialise l'axe déterminé par les deux brins donnés (qui ne doivent pas
 * être sur la même orbite sommet) en créant une nouvelle arête.
 * L'arête créée est constituée de deux brins 0-cousus.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @return Le brin de l'axe créé correspondant à ADart2
 */
CDart* materializeAxe(CDart* ADart1, CDart* ADart2);

/**
 * Matérialise le plan déterminé par les trois sommets incidentes aux brins
 * donnés. L'objet créé est un triangle.
 *
 * @param ADart1 Un brin de la carte
 * @param ADart2 Un brin de la carte
 * @param ADart3 Un brin de la carte
 * @return Un brin du plan créé
 */
CDart* materializePlane(CDart* ADart1, CDart* ADart2, CDart* ADart3);

/**
 * Matérialise le vecteur normal de la face incidente au brin donné.
 *
 * @param ADart un brin de la carte
 * @return Le brin correspondant à la pointe du vecteur normal lorsque ce
 * vecteur "est posé" sur le barycentre de la face incidente à ADart.
 */
CDart* materializeNormalVector(CDart* ADart);

/**
 * Matérialise le plan normal du vecteur donné.
 *
 * @param ADart1 un brin de la carte
 * @param ADart2 un brin de la carte
 * @return Un brin appartenant au plan créé (un triangle)
 */
CDart* materializeNormalPlane(CDart* ADart1, CDart* ADart2);

/**
 * Matérialise le référentiel (OX,OY,OZ) avec 3 faces perpendiculaires
 * 2-cousues. Chaque face est ouverte et n'est composée que de deux arêtes.
 *
 * @return Un brin du référentiel
 */
CDart* materializeReferential();

//******************************************************************************
