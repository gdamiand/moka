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
#ifndef MESH_GENERIC_HH
#define MESH_GENERIC_HH
//******************************************************************************
#include <cstdlib>
//******************************************************************************
class CTransformationMatrix;

namespace GMap3d
{
  class CDart;
  class CGMapGeneric;

  class CMeshGeneric
  {
  public:
    // @name Instanciantion de la classe
    // @{
#include "mg-instance.hh"
    //@}

    // @name Méthodes pour mailler une cellule
    // @{
#include "mg-mesh.hh"
    // @}

    /**
     * @name Méthodes de reconnaissance de cellules simples
     *
     * Ces méthodes servent à tester s'il est possible de fusionner un maillage
     * avec une cellule.
     */
#include "mg-cell-identification.hh"
    // @}

    // @name Méthodes de sélection de cellules maillées
    // @{
#include "mg-selection.hh"
    // @}

    // @name Méthodes permettant d'assembler un maillage avec une cellule
    // @{
#include "mg-merge.hh"
    // @}

  private:
    CGMapGeneric* FMap;
  };
}
//******************************************************************************
#endif // MESH_GENERIC_HH
//******************************************************************************
