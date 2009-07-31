/*
 * lib-tools : Divers outils autour de lib-gmapkernel.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-tools
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

#ifndef ATTRIBUTE_BOUNDING_BOX_HH
#define ATTRIBUTE_BOUNDING_BOX_HH

#include "attribute.hh"
#include "bounding-box.hh"

/** Classe définissant un attribut boîte englobante qui contient les limites
 * spatiales d'un objet.
 *
 * @author Nicolas Guiard
 */

class CAttributeBoundingBox : public CAttribute, public CBoundingBox
{
public:

  /** @name Constructeurs et destructeur
   */

  //@{

  /** Constructeur par défaut
   *
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe
   * initialisée avec une boîte englobante vide.
   */

  CAttributeBoundingBox();

  /** Constructeur avec initialisation
   *
   * @param AOther la boîte englobante à utiliser
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe, en utilisant la boîte
   * englobante passée en paramètre.
   */

  CAttributeBoundingBox(const CBoundingBox & ABB);

  /** Constructeur par recopie
   *
   * @param AOther la boîte englobante à copier
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle instance de la classe, en copiant la boîte
   * englobante passée en paramètre.
   */

  CAttributeBoundingBox(const CAttributeBoundingBox & AOther);

  /** Destructeur
   *
   * Destruction de l'instance.
   */

  virtual ~CAttributeBoundingBox();

  //@}

  /** @name Implémentation des méthodes virtuelles pures héritées de la classe
   *  Attribute.
   */

  //@{

  /** getType
   *
   * @return Le type de l'attribut
   *
   * Retourne le type de l'attribut.
   */

  TAttributeId getType() const;
  
  /** copy
   *
   * @return Une nouvelle instance de l'attribut
   *
   * Crée une nouvelle instance de CAttributeBoundingBox, clone de l'instance
   * courante.
   */

  CAttribute * copy() const;

  /** destroy
   *
   * Détruit l'objet (appel au destructeur).
   */

  void destroy();
  
  /** save
   *
   * @param AStream Un flot dans lequel écrire l'attribut
   *
   * Écrit l'attribut dans le flot donné, en vue d'une sauvegarde.
   */

  void save(std::ostream & AStream) const;

  /** load
   *
   * @param AStream Un flot dans lequel lire l'attribut
   *
   * Lit l'attribut à partir du flot donné.
   */

  void load(std::istream & AStream);

  //@}

private:
};

#endif
