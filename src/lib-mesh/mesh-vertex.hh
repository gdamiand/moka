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
#ifndef MESH_VERTEX_HH
#define MESH_VERTEX_HH
//******************************************************************************
#include "mesh-generic.hh"
//******************************************************************************
class CTransformationMatrix;
class CVertex;

namespace GMap3d
{
  class CGMapVertex;

  class CMeshVertex : public CMeshGeneric
  {
    // @name Instanciation de la classe
    // @{
#include "mv-instance.hh"
    // @}

    /** @name Allocation et libération de tableaux de points
     * (pour la récupération des plongements nécessaires à une interpolation)
     */
    // @{
#include "mv-array-allocation.hh"
    // @}

    // @name Méthodes permettant la récupération de points de contrôle
    // @{
#include "mv-control-points.hh"
    // @}

    // @name Méthodes permettant le maillage de cellules
    // @{
#include "mv-mesh.hh"
    // @}

  private:
    CGMapVertex* FMap;
  };
}
//******************************************************************************
#endif // MESH_VERTEX_HH
//******************************************************************************
