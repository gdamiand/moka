/*
 * lib-spamod : Visualisation des objets en discret.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-spamod
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
#ifndef SPAMOD_TYPES_HH
#define SPAMOD_TYPES_HH
//******************************************************************************

namespace GMap3d
{

//@name Types de vues :
typedef int TSpamodViewMode;

enum
{
  SPAMOD_NONE = -1,
  SPAMOD_TOPOLOGICAL,
  SPAMOD_EUCLIDIAN,
  SPAMOD_ANALYTIC,
  SPAMOD_VOXEL,
  SPAMOD_K2
};

} // namespace GMap3d

#endif // SPAMOD_TYPES_HH
