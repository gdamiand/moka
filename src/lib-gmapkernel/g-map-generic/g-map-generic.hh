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
#ifndef G_MAP_GENERIC_HH
#define G_MAP_GENERIC_HH
//******************************************************************************
#include "g-map.hh"
#include "math-extension.hh"
#include "transformation-matrix.hh"
//******************************************************************************

namespace GMap3d
{
  /**
   * Définit la classe CGMapGeneric qui représente une 3-G-carte.
   * Les opérations implémentées dans cette classe sont toutes topologiques.
   * Celles qui modifient la carte ne mettent pas à jour les
   * plongements (exemple: triangulation, qui ne duplique pas
   * les plongements 'face' lors de la triangulation d'une face).
   * La mise à jour des plongements doit donc être faite dans les
   * méthodes des classes qui dérivent de CGMapGeneric.
   *
   * @author MOKA TEAM
   */
  class CGMapGeneric: public CGMap
  {
    // @name Constructeur et destructeur
    // @{
#include "gmg-instance.hh"
    // @}

    // @name Accesseurs inlinés
    // @{
#include "gmg-inline.hh"
    // @}

    // @name Méthodes de marquage
    // @{
#include "gmg-selection.hh"
#include "gmg-exploration.hh"
    // @}

    /**
     * @name Méthodes permettant d'allouer et de libérer de la mémoire aux
     * champs directInfo des brins, ainsi que d'effectuer la sauvegarde de
     * coutures.
     */
    // @{
#include "gmg-direct-info.hh"
    // @}

    // @name Primitives
    // @{
#include "gmg-primitive.hh"
#include "gmg-primitive-mesh.hh"
    // @}

    // @name Manipulation de polylignes
    // @{
#include "gmg-polyline.hh"
    // @}

    // @name Informations topologiques
    // @{
#include "gmg-topology.hh"
#include "gmg-degree.hh"
    // @}

    // @name Coutures / décousures
    // @{
#include "gmg-sew.hh"
#include "gmg-unsew.hh"
    // @}

    // @name Duplication et suppression de brins
    // @{
#include "gmg-duplication.hh"
#include "gmg-deletion.hh"
    // @}

    // @name Méthode de manipulation des bords
    // @{
#include "gmg-border.hh"
#include "gmg-stop-up.hh"
    // @}

    // @name Insertions (splits)
    // @{
#include "gmg-insertion.hh"
    // @}

    // @name Fusions (merges) / contractions / suppression de cellules "plates"
    // @{
#include "gmg-merge.hh"
#include "gmg-removal.hh"
#include "gmg-contraction.hh"
#include "gmg-contract-and-remove.hh"
#include "gmg-simplification.hh"
    // @}

    // @name Triangulations et quadrangulations barycentriques
    // @{
#include "gmg-triangulation.hh"
#include "gmg-quadrangulation.hh"
    // @}

    // @name Extrusions et épaississement
    // @{
#include "gmg-extrusion.hh"
#include "gmg-thickening.hh"
    // @}

    // @name Méthodes de traitement des arêtes fictives.
    // @{
#include "gmg-fictive-edge.hh"
    // @}

    // @name Méthodes de traitement des faces fictives.
    // @{
#include "gmg-fictive-face.hh"
    // @}

  };
  
} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("gmg-inline.icc")
//******************************************************************************
#endif // G_MAP_GENERIC_HH
//******************************************************************************
