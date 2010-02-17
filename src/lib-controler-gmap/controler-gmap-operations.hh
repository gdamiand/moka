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
// Ce fichier est une partie de "controler-gmap.hh".
//******************************************************************************

public:

// @name Méthodes générales
// @{

/// Return true iff the operation does not modify the map.
bool isNonModifyingOperation(const COperation& AOperation) const;

/// Return true iff the operation is a removal one.
bool isRemovalOperation(const COperation& AOperation) const;

/// Return true iff the operation is a contraction one.
bool isContractionOperation(const COperation& AOperation) const;

/**
 * Retourne si le mode courant permet de faire une opération (exemple couture,
 *  inversion mark...).
 */
virtual bool canApplyOperation(const COperation& AOperation);

/// Exécute l'opération donné en paramètre.
virtual bool applyOperation(const COperation& AOperation);

public:
/**
 * Accesseurs pour récupérer le nombre d'éléments de la carte. 
 */
int getNbDarts() const;
int getNbSelectedDarts() const;
int getNbVertices() const;

//@}

// @name Création d'objets
// @{
#include "operations-creation.hh"
// @}

// @name Couture / Décousure
// @{
#include "operations-sew-unsew.hh"
//@}

// @name Plaquage
// @{
#include "operations-plating.hh"
// @}

// @name Sélection / Matching
// @{
#include "operations-selection.hh"
// @}

// @name Destruction / Copie
// @{
#include "operations-duplication-deletion.hh"
// @}

// @name Fusion / Contraction / Insertion
// @{
#include "operations-merging-insertion-contraction.hh"
// @}

// @name Positionnement
// @{
#include "operations-positioning.hh"
// @}

// @name Matérialisation
// @{
#include "operations-materialization.hh"
// @}

// @name Triangulations / Quadrangulations
// @{
#include "operations-triangulation-quadrangulation.hh"
// @}

#ifdef MODULE_TRIANGULATION
// @name Opérations de triangulation
// @{
#include "operations-triangulation.hh"
// @}
#endif // MODULE_TRIANGULATION

#ifdef MODULE_GEOLOGY
// @name Opérations géologiques
// @{
#include "operations-geology.hh"
// @}
#endif // MODULE_GEOLOGY

#ifdef MODULE_MESH
// @name Maillage / Lissage
// @{
#include "operations-mesh.hh"
// @}
#endif // MODULE_MESH

#ifdef MODULE_ROUNDING
// @name Chanfreinage
// @{
#include "operations-rounding.hh"
// @}
#endif // MODULE_ROUNDING

#ifdef MODULE_COREFINEMENT
// @name Corafinement
// @{
#include "operations-corefinement.hh"
// @}
#endif // MODULE_COREFINEMENT

// @name Dual
// @{
#include "operations-dual.hh"
// @}

// @name Extrusion
// @{
#include "operations-extrusion.hh"
// @}

// @name Caractéristiques topologiques
// @{
#include "operations-topology.hh"
// @}

// @name Sauvegarde / Chargement / Export
// @{
#include "operations-save-load-export.hh"
// @}

// @name Déplacement sélection
// @{
#include "operations-move-selection.hh"
// @}

//******************************************************************************
