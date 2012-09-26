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
 * Fusionne lorsque cela est possible (avec une arête qui leur est incidente)
 * les arêtes dont un brin au moins est marqué, lorsqu'elles sont alignées.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADeleteDarts Un booléen indiquant si la fusion doit se faire en
 *   supprimant des brins
 * @return Le nombre de fusions effectuées
 */
int mergeMarkedColinearEdges(int AMarkNumber, bool ADeleteDarts);

/**
 * Fusionne lorsque cela est possible (avec une face qui leur est incidente)
 * les faces dont un brin au moins est marqué, lorsqu'elles sont coplanaires.
 *
 * @param AMarkNumber Un numéro de marque
 * @param ADeleteDarts Un booléen indiquant si la fusion doit se faire en
 *   supprimant des brins
 * @return Le nombre de fusions effectuées
 */
int mergeMarkedCoplanarFaces(int AMarkNumber, bool ADeleteDarts);

/**
 * Fusionne lorsque cela est possible les arêtes ou les faces alignées.
 *
 * @param ADim La dimension (1 ou 2) des cellules à fusionner
 * @param AMarkNumber Un numéro de marque
 * @param ADeleteDarts Un booléen indiquant si la fusion doit se faire en
 *   supprimant des brins
 * @return Le nombre de fusions effectuées
 */
int mergeMarkedAlignedCells(int ADim, int AMarkNumber, bool ADeleteDarts);

/** Simplify the map by removing each degree two face, each dangling face,
 *  then each degree two edge, each dangling edge, and then
 *  each degree two vertex. There is no fictive face or edge shifting.
 *  AMarki is a mark used to mark darts deleted by i-removal.
 *  If AMarki==-1, corresponding darts are removed from the map. Otherwise,
 *  they are not removed but only marked.
 *  @return the number of "removed" darts.
 */
unsigned int simplify3DObject(int AMark0, int AMark1, int AMark2=-1);

/** Simplify the 2G-map by removing each degree two edge, each dangling edge, and
 *  then each degree two vertex. There is no fictive edge shifting.
 *  AMarki is a mark used to mark darts deleted by i-removal.
 *  If AMarki==-1, corresponding darts are removed from the map. Otherwise,
 *  they are not removed but only marked.
 *  @return the number of removed darts.
 */
unsigned int simplify2DObject(int AMark0, int AMark1);

/** Simplify the 2G-map.
  * optosimplify contains a value giving which cells will be simplified:
  * a or between NONE,EDGE_REMOVAL,VERTEX_REMOVAL,
  * EDGE_CONTRACTION, FACE_CONTRACTION.
  */
unsigned int simplify2DObject(unsigned int optosimplify = EDGE_REMOVAL |
    VERTEX_REMOVAL | EDGE_CONTRACTION | FACE_CONTRACTION );

/** Simplify the map using removal operations only.
 */
unsigned int simplify2DObjectRemoval(unsigned int optosimplify);

/** Simplify the map using removal operations only.
 */
unsigned int simplify2DObjectContraction(unsigned int optosimplify);

/** Simplify the 3G-map.
  * Equivalent to simplify3DObject(-1,-1,-1) but optimized.
  * optosimplify contains a value giving which cells will be simplified:
  * a or between NONE,FACE_REMOVAL,EDGE_REMOVAL,VERTEX_REMOVAL,
  * EDGE_CONTRACTION, FACE_CONTRACTION, VOLUME_CONTRACTION.
  */
unsigned int simplify3DObject(unsigned int optosimplify = FACE_REMOVAL |
    EDGE_REMOVAL | VERTEX_REMOVAL | EDGE_CONTRACTION |
    FACE_CONTRACTION | VOLUME_CONTRACTION );

/** Simplify the map using removal operations only.
 */
unsigned int simplify3DObjectRemoval(unsigned int optosimplify);

/** Simplify the map using removal operations only.
 */
unsigned int simplify3DObjectContraction(unsigned int optosimplify);

//******************************************************************************
