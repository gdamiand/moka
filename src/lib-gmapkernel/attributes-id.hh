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
#ifndef ATTRIBUTES_ID_HH
#define ATTRIBUTES_ID_HH
//******************************************************************************
/** Définition des id des attributs
 *
 *  @author MOKA TEAM
 */

enum
{
  // Attribut principal : sommet pour g-map-vertex
  ATTRIBUTE_VERTEX = 1,

  // Attributs pour le corafinement
  ATTRIBUTE_COREFINE_BBOX,
  ATTRIBUTE_QUADRIC_ERROR,
  
  // Attributs spamod
  ATTRIBUTE_SPAMOD_INT,
  ATTRIBUTE_SPAMOD_COLOR_TABLE,
  ATTRIBUTE_SPAMOD_INEQ_TABLE2,
  ATTRIBUTE_SPAMOD_INEQ_TABLE6,
  ATTRIBUTE_SPAMOD_INEQ_USE,
  ATTRIBUTE_SPAMOD_MATRIX,
  ATTRIBUTE_SPAMOD_POINT_LIST,
  ATTRIBUTE_SPAMOD_VOXEL_LIST,

  // Attributs geology
  ATTRIBUTE_GEO_MACRO_FACE
};

//******************************************************************************
#endif // ATTRIBUTES_ID_HH
//******************************************************************************
