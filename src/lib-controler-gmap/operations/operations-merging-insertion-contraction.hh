/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
// Ce fichier fait partie de "controler-gmap-operations.hh".
//******************************************************************************

public:

/// In simplification mode, update the removed marks after having
/// ADim-removed some darts.
void updateDartAfterRemovals(unsigned int ADim);

/// In simplification mode, update the removed marks after having
/// removed some darts, each mark AMarki corresponds to darts i-removed .
void updateDartAfterRemovals(int AMark0, int AMark1, int AMark2);

bool merge(int ADimension);
bool intuitiveMerge();

bool mergeColinearEdges();
bool mergeCoplanarFaces();
bool delNullEdges();
bool delFlatFaces();
bool delFlatVolumes();

bool removeMarkedEdgesWithoutDisconnection();
bool shiftAllEdgesIncidentToVertex();
bool removeDanglingEdges();
bool simplify2DObject();
bool simplify3DObject();

bool removeMarkedFacesButKeepBalls();

bool contract(int ADimension);
bool contextContract();

bool insertVertex();
bool insertEdge();
bool insertFace();

bool stopUp(int ADimension);
bool intuitiveStopUp();

//******************************************************************************
