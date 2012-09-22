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
#ifndef G_MAP_VERTEX_HH
#define G_MAP_VERTEX_HH
//******************************************************************************
#include <cmath>
#include <cstdio>
#include <fstream>
#include <set> // pour les méthodes auxiliaires de co-raffinement.

#include "g-map-generic.hh"
#include "dart-vertex.hh"
#include "geometry.hh"
#include "vertex.hh"
#include "vector.hh"
#include "transformation-matrix.hh"
#include "mesh-diver.hh"

#include "alt-stdint.hh"
#include "inline-macro.hh"
//******************************************************************************
namespace GMap3d
{
  enum
  {
    NONE              = 0,
    FACE_REMOVAL      = 1,
    EDGE_REMOVAL      = 2,
    VERTEX_REMOVAL    = 4,
    EDGE_CONTRACTION  = 8,
    FACE_CONTRACTION  = 16,
    VOLUME_CONTRACTION= 32
  };


  /**
   * Définit la classe CGMapVertex (carte plongée sommets).
   *
   * @author MOKA TEAM
   */
  class CGMapVertex : public CGMapGeneric
  {
  public:
    // @name Constructeur, destructeur et méthode d'initialisation
    // @{
#include "gmv-instance.hh"
    // @}

    // @name Accesseurs
    // @{
#include "gmv-inline.hh"
    // @}

    // @name Méthodes permettant de créer et plonger des maillages
    // @{
#include "gmv-primitive-mesh.hh"
    // @}

    // @name Méthodes permettant la manipulation de polylignes
    // @{
#include "gmv-polyline.hh"
    // @}

    // @name Méthodes de création des objets de base
    // @{
#include "gmv-primitive.hh"
    // @}

    // @name Méthodes basiques pour les opérations géométriques
    // @{
#include "gmv-geometry.hh"
    // @}

    // @name Méthodes de calcul d'une droite ou d'un plan de régression
    // @{
#include "gmv-regression.hh"
    // @}

    // @name Méthodes de placage
    // @{
#include "gmv-plating.hh"
    // @}

    // @name Coutures avec placages
    // @{
#include "gmv-geo-sew.hh"
    // @}

    // @name Méthode d'insertion
    // @{
#include "gmv-insertion.hh"
    // @}

    // @name Méthode de suppression des arêtes de longueur nulle
    // @{
#include "gmv-null-length-edge.hh"
    // @}

    // @name Matérialisations
    // @{
#include "gmv-materialization.hh"
    // @}

    // @name Chargement et sauvegarde
    // @{
#include "gmv-save.hh"
    // @}

    // @name Import et export au format off
    // @{
#include "gmv-off.hh"
    // @}

    // Calcul du dual d'une carte, en dimension 2 ou 3.
    // @{
#include "gmv-dual.hh"
    // @}

    // @name Méthodes de mise à jour des plongements éclatés
    // @{
#include "gmv-burstview.hh"
    // @}

    // @name Bouchage
    // @{
#include "gmv-stop-up.hh"
    // @}

    // @name Triangulations et quadrangulations barycentriques
    // @{
#include "gmv-triangulation.hh"
#include "gmv-quadrangulation.hh"
    // @}

    // @name Méthodes permettant l'allocation et la libération d'un paramètre de
    //   pondération pour les déformations
    // @{
#include "gmv-direct-info.hh"
    // @}

    // @name Méthodes permettant de déformer les objets
    // @{
#include "gmv-deformation.hh"
    // @}

    // @name Épaississement
    // @{
#include "gmv-thickening.hh"
    // @}

    // @name Extrusions
    // @{
#include "gmv-extrusion.hh"
    // @}

    // @name Fusion de cellules alignées
    // @{
#include "gmv-merge.hh"
    // @}

    // Différents exports dans des formats plus ou moins exotiques
    // @{
#include "gmv-export-pov.hh"
    // @}

    // Test d'isomorphismes de cartes
    // @{
#include "gmv-isomorphisme.hh"
    // @}

  private:
    // @name Champs privés
    // @{

    /// Coefficients d'éclatement, pour le calcul des plongements éclatés
    TCoordinate FBurstCoef[4];

    /// Méthode de calcul des sommets éclatés
    TBurstMethod FBurstMethod;
    
    // @}
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("gmv-inline.icc")
//******************************************************************************
#endif // G_MAP_VERTEX_HH
//******************************************************************************
