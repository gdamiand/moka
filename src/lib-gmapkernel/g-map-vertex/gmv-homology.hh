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
 * Compute the incidence number of cell c=(ADart,ADim), relatively to the cell
 * c2=(ADart2,ADim+1). Incidence number is the number of times c is incident to
 * c2. This number is signed depending if the two cells have the same
 * orientation (+) or not (-).
 *
 * @param ADart A dart of the cell c
 * @param ADim The dimension of the cell c
 * @param ADart A adart of the cell c2
 * @return The incidence number of c in c2
 *
 * @precondition (ADim >= 0) && (ADim < 3)
 */
int computeIncidenceNumber(CDart* ADart, int ADim, CDart* ADart2);

//******************************************************************************
