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
#ifndef ATTRIBUTE_VERTEX_HH
#define ATTRIBUTE_VERTEX_HH
//******************************************************************************
#include "attribute.hh"
#include "vertex.hh"
#include "inline-macro.hh"

#include <iostream>
//******************************************************************************

/**
 * Les différents formats de fichiers supportés pour la sauvegarde et le
 * chargement.
 */
typedef enum { UnknownFormat=-1, AsciiFormat, BinaryFormat } TSaveFormat;

/**
 * Définit la classe définissant un attribut sommet, pouvant être utilisé par
 * une carte plongée sommets.
 *
 * @author MOKA TEAM
 */
class CAttributeVertex: public CAttribute, public CVertex
{
public:
  // @name Constructeurs et destructeur
  // @{

  /**
   * Constructeur par défaut
   *
   * Construction d'une nouvelle instance de la classe, initialisée
   * au sommet origine (0,0,0).
   *
   * @return Une nouvelle instance de la classe
   */
  CAttributeVertex();

  /**
   * Constructeur avec initialisation
   *
   * Construction d'une nouvelle instance de la classe, initialisée au
   * sommet de coordonnées (Ax,Ay,Az).
   *
   * @param Ax La première  coordonnée du sommet
   * @param Ay La deuxième  coordonnée du sommet
   * @param Az La troisième coordonnée du sommet
   * @return Une nouvelle instance de la classe
   */
  CAttributeVertex(TCoordinate Ax, TCoordinate Ay, TCoordinate Az);

  /**
   * Constructeur par copie (CVertex)
   *
   * Construction d'une nouvelle instance de la classe, en copiant le sommet
   * passé en paramètre.
   *
   * @param AVertex le sommet à copier
   * @return Une nouvelle instance de la classe
   */
  CAttributeVertex(const CVertex& AVertex);

  /**
   * Constructeur par copie (CAttributeVertex)
   *
   * Construction d'une nouvelle instance de la classe, en copiant le sommet
   * passé en paramètre.
   *
   * @param AAttributeVertex le sommet à copier
   * @return Une nouvelle instance de la classe
   */
  CAttributeVertex(const CAttributeVertex& AAttributeVertex);

  /// Destructeur
  ~CAttributeVertex();

  // @}
  /**
   * @name Implémentation des méthodes virtuelles pures héritées de la classe
   *   CAttribute.
   */
  // @{

  /**
   * Retourne le type de l'attribut, c'est-à-dire la valeur de la variable de
   * classe 'TYPE'.
   *
   * @return Le type de l'attribut
   */
  TAttributeId getType() const;

  /**
   * Crée une nouvelle instance de CAttributeVertex, clone de l'instance
   * courante.
   *
   * @return Une nouvelle instance de l'attribut
   */
  CAttribute* copy() const;

  /// Détruit l'objet (appel au destructeur).
  void destroy();

  /**
   * Écrit l'attribut dans le flot donné, en vue d'une sauvegarde.
   *
   * @param AStream Un flot dans lequel écrire l'attribut
   */
  void save(std::ostream& AStream) const;

  /**
   * Lit l'attribut à partir du flot donné.
   *
   * @param AStream Un flot dans lequel lire l'attribut
   */
  void load(std::istream& AStream);

  // @}
};

// @name Affichage
// @{

/**
 * @param AStream Le flot dans lequel afficher le sommet
 * @param AVertex Le sommet à afficher
 @ @return Le flot utilisé pour l'affichage
 */
std::ostream& operator<<(std::ostream& AStream,
			 const CAttributeVertex& AVertex);

// @}
//******************************************************************************
#include INCLUDE_INLINE("attribute-vertex.icc")
//******************************************************************************
#endif // ATTRIBUTE_VERTEX_HH
//******************************************************************************
