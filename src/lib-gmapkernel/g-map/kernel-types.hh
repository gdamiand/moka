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
#ifndef KERNEL_TYPES_HH
#define KERNEL_TYPES_HH
//******************************************************************************
#include "attributes-id.hh"
#include "alt-stdint.hh"

#include <bitset>
#include <string>
//******************************************************************************
/**
 * @name Définition des types de base du noyau (orbites, involutions et
 *   masques)
 *
 * @author MOKA TEAM
 */

//******************************************************************************
// @name Macros
// @{

/**
 * Nombre de marques associées aux brins.
 */
#define NB_MARKS (32)

/**
 * Nombre de champs direct-info associés aux brins.
 */
#define NB_DIRECT_INFO (4)

// @}

//******************************************************************************
/// Type Orbite
typedef int8_t TOrbit;

// @name Orbites de base
// @{
enum
{
  ORBIT_NONE = -1,
  ORBIT_SELF,	
  ORBIT_0,
  ORBIT_1,
  ORBIT_01,
  ORBIT_2,
  ORBIT_02,
  ORBIT_12,
  ORBIT_012,
  ORBIT_3,
  ORBIT_03,
  ORBIT_13,
  ORBIT_013,
  ORBIT_23,
  ORBIT_023,
  ORBIT_123,
  ORBIT_0123,

  ORBIT_BORDER_0,
  ORBIT_BORDER_1,
  ORBIT_BORDER_2,
  ORBIT_BORDER_3
};

// @}
//******************************************************************************
// @name Types d'attributs
// @{

typedef int TAttributeId;

/// Les différents identifiants des attributs sont définis dans le fichier
/// attributes-id.hh

// @}
//******************************************************************************
/**
 * @name Opérations sur les orbites.
 * Une orbite est définie par un ensemble d'involutions.
 * Une orbite peut donc être manipulée comme un ensemble.
 */
// @{

/**
 * Complémentaire d'une orbite.
 *
 * @param O1 Une orbite.
 * @return L'orbite définie par le complémentaire de l'ensemble d'involutions
 *   définissant O1.
 */
#define NEG_ORBIT(O1) \
	(static_cast<TOrbit> (0xF ^ (static_cast<unsigned int>(O1))))

/**
 * Union de 2 orbites.
 *
 * @param O1 Une orbite.
 * @param O2 Une orbite.
 * @return L'orbite définie par l'union des ensembles d'involutions
 *   définissant O1 et O2.
 */
#define ADD_ORBIT(O1,O2) \
	(static_cast<TOrbit>( \
	 static_cast<unsigned int>(O1) | \
	 static_cast<unsigned int>(O2)))

/**
 * Intersection de 2 orbites.
 *
 * @param O1 Une orbite.
 * @param O2 Une orbite.
 * @return L'orbite définie par l'intersection des ensembles d'involutions
 *   définissant O1 et O2.
 */
#define AND_ORBIT(O1,O2) \
	(static_cast<TOrbit>( \
	 static_cast<unsigned int>(O1) & \
	 static_cast<unsigned int>(O2)))

/**
 * Soustraction d'orbites.
 *
 * @param O1 Une orbite.
 * @param O2 Une orbite.
 * @return L'orbite définie par l'ensemble d'involutions définissant O1 privé
 *   de l'ensemble d'involutions définissant O2.
 */
#define SUB_ORBIT(O1,O2) \
	(static_cast<TOrbit>( \
	 static_cast<unsigned int>(O1) & \
	 static_cast<unsigned int>(NEG_ORBIT(O2))))

/**
 * Test d'inclusion d'orbites.
 *
 * @param O1 Une orbite.
 * @param O2 Une orbite.
 * @return Un booléen indiquant si l'orbite O1 est incluse dans l'orbite O2.
 */
#define IS_ORBIT_INCLUDED(O1,O2) \
	(static_cast<TOrbit>( \
	 static_cast<unsigned int>(O1) & \
	 static_cast<unsigned int>(O2)) == O1)

// @}
//****************************************************************************
// @name Orbites ayant une sémantique géométrique.
// @{

const TOrbit ORBIT_VERTEX = ORBIT_123 ; //@< Orbite sommet.
const TOrbit ORBIT_EDGE   = ORBIT_023 ; //@< Orbite arête.
const TOrbit ORBIT_FACE   = ORBIT_013 ; //@< Orbite face.
const TOrbit ORBIT_VOLUME = ORBIT_012 ; //@< Orbite volume.
const TOrbit ORBIT_CC     = ORBIT_0123; //@< Orbite composante connexe.

// @}

//****************************************************************************
// @name Ensembles d'orbites particuliers.
// @{

/// Orbite de l'ensemble des involutions d'indice inférieur strict à l'indice
/// du tableau.
const TOrbit ORBIT_INF[4] =
{
  ORBIT_SELF, ORBIT_0, ORBIT_01, ORBIT_012
};

/// Orbite cellule de dimension l'indice du tableau.
const TOrbit ORBIT_CELL[5] =
{
  ORBIT_VERTEX, ORBIT_EDGE, ORBIT_FACE, ORBIT_VOLUME, ORBIT_CC
};

/// Orbite bord de dimension l'indice du tableau.
const TOrbit ORBIT_BORDER [4] =
{
  ORBIT_BORDER_0, ORBIT_BORDER_1, ORBIT_BORDER_2, ORBIT_BORDER_3
};

/// Orbite des deux involutions d'indice égal et immédiatement inférieur à
/// l'indice du tableau.
const TOrbit ORBIT_I_IM1[4] =
{
  ORBIT_0, ORBIT_01, ORBIT_12, ORBIT_23
};

/// Orbite des deux involutions d'indice égal et immédiatement supérieur à
/// l'indice du tableau.
const TOrbit ORBIT_I_IP1[4] =
{
  ORBIT_01, ORBIT_12, ORBIT_23, ORBIT_3
};

/// Orbite qui résulte en une involution lorsqu'elle est composée avec
/// l'involution d'indice celui du tableau.
const TOrbit ORBIT_INVO[4] =
{
  ORBIT_23, ORBIT_3, ORBIT_0, ORBIT_01
};

/// Orbite qui est modifiée lors d'une couture par l'indice du tableau.
const TOrbit ORBIT_SEWS[4] =
{
  ORBIT_023, ORBIT_13, ORBIT_02, ORBIT_013
};

/// Orbite par l'involution d'indice celui du tableau.
const TOrbit ORBIT_DIM[4] =
{
  ORBIT_0, ORBIT_1, ORBIT_2, ORBIT_3
};

// @}

//******************************************************************************
// @name Opérations (interne au fonctionnement des parcours).

// @{

/// Ce type permet, lors d'un parcours, de connaître quel est le type de
/// la dernière opération
typedef int8_t TOperationState;

enum
{
  /// Le parcours débute et il n'y a pas d'opération précédente (operator++).
  OP_NONE = -1,

  /// L'opération précédente était un operator++ de type alpha0.
  OP_ALPHA0,
  /// L'opération précédente était un operator++ de type alpha1.
  OP_ALPHA1,
  /// L'opération précédente était un operator++ de type alpha2.
  OP_ALPHA2,
  /// L'opération précédente était un operator++ de type alpha3.
  OP_ALPHA3,
  /// Pour les templates de parcours, les alpha relatifs utilisés sont alphai
  /// pour le premier,
  OP_ALPHAI,
  /// alphaj pour le second.
  OP_ALPHAJ,
  /// et alphak pour le troisième.
  OP_ALPHAK,

  /// Opération de saut (pour les parcours qui utilisent une pile)
  OP_JUMP,
  /// Pour un parcours de bord de dimension i, étape qui consiste à "tourner"
  /// autour d'une cellule à l'aide de l'involution alpha(i).alpha(i-1).
  OP_TURN_AROUND,
  /// End of op ???....
  OP_END,

  // Lorsqu'on parcourt une liste de brins (static et multidynamic coverages).
  OP_UNKNOWN
};

// @}

//****************************************************************************
/**
 * @name Les masques
 *
 * Les masques sont des tableaux de bits interprétés comme en valeur
 * booléenne. L'indice du tableau correspond à l'indice de l'involution
 * associée au booléen.
 */

// @{

/// Définition du type TMask16 pour les 16 orbites possibles.
typedef std::bitset<16> TMask16;

//****************************************************************************
/**
 * Un masque est un tableau de 16 booléens utilisé principalement pour la
 * mise à jour des plongements. En effet, lors d'une couture ou d'une
 * décousure de dimension i, toutes les orbites ne contenant pas l'involution
 * alphai peuvent nécessiter une modification de leurs plongements.
 */

/// Attention, les bits de poids fort sont à gauche.

/// Masque 'faux'.
const TMask16 MASK_FALSE(std::string("0000000000000000"));
/// Mask 'vrai'.
const TMask16 MASK_TRUE(std::string("1111111111111111"));

/// Masque de toutes les orbites contenant l'involution alpha0 dans leur
/// ensemble définition.
const TMask16 MASK0(std::string("1010101010101010"));
/// Masque de toutes les orbites contenant l'involution alpha1 dans leur
/// ensemble définition.
const TMask16 MASK1(std::string("1100110011001100"));
/// Masque de toutes les orbites contenant l'involution alpha2 dans leur
/// ensemble définition.
const TMask16 MASK2(std::string("1111000011110000"));
/// Masque de toutes les orbites contenant l'involution alpha3 dans leur
/// ensemble définition.
const TMask16 MASK3(std::string("1111111100000000"));

/// Masques de toutes les orbites contenant alphai et pas alphaj :
const TMask16 MASK0M2(MASK0 & ~MASK2);
const TMask16 MASK0M3(MASK0 & ~MASK3);
const TMask16 MASK1M3(MASK1 & ~MASK3);
const TMask16 MASK2M0(MASK2 & ~MASK0);
const TMask16 MASK3M0(MASK3 & ~MASK0);
const TMask16 MASK3M1(MASK3 & ~MASK1);

// @}

//******************************************************************************
/// @name Type d'affichage éclaté.

/// @{
/// Ce type permet de spécifier quel type d'affichage éclatée on utilise.
typedef int8_t TBurstMethod;

enum
{
  FIRST_BURST_METHOD = 0,
  
  BURST_METHOD_SCALE = FIRST_BURST_METHOD,
  BURST_METHOD_SHIFT,

  LAST_BURST_METHOD
};

/// @}

//******************************************************************************
#endif // KERNEL_TYPES_HH
//******************************************************************************
