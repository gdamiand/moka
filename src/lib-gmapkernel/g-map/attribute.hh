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
#ifndef ATTRIBUTE_HH
#define ATTRIBUTE_HH
//******************************************************************************
#include "kernel-types.hh"
#include "inline-macro.hh"

#include <iostream>
//******************************************************************************
/**
 * Définition de l'interface attribut.
 * Toute classe concrète implémentant cette classe abstraite peut être
 * associée à une orbite de la G-Carte.
 *
 * Cinq méthodes virtuelles pures sont à implémenter :
 *  - getType() : l'identificateur de la classe de l'attribut,
 *  - copy()     : le constructeur de recopie pour la propagation de l'attribut,
 *  - destroy()  : le destructeur de l'attribut,
 *  - save()     : sauvegarde de l'attribut dans un flux,
 *  - load()     : lecture de l'attribut depuis un flux.
 *
 * @author MOKA TEAM
 */
class CAttribute
{
public:
  // @name Constructeurs et destructeur.
  // @{

  /// Constructeur par défaut
  CAttribute();

  /// Constructeur par copie
  CAttribute(const CAttribute& CAttribute);

  /// Destructeur
  virtual ~CAttribute();

  // @}
  // @name Méthodes virtuelles pures à implémenter dans les classes filles.
  // @{

  /**
   * @return L'identificateur de la classe d'attribut.
   */
  virtual TAttributeId getType() const = 0;

  /**
   * Cette méthode permet de spécifier le comportement de l'attribut
   * lorsqu'il est propagé.
   *
   * @return Un clone de l'attribut
   */
  virtual CAttribute* copy() const = 0;

  /**
   * Méthode de destruction de l'attribut.
   * Lorsque l'attribut n'est plus présent dans le modèle, cette méthode de
   * destruction est déclenchée.
   */
  virtual void destroy() = 0;

  /**
   * Sauvegarde de l'attribut.
   * Les méthodes de sauvegarde des attributs sont appelées lors de la
   * sauvegarde du modèle.
   *
   * @param AStream Le flux de sortie.
   */
  virtual void save(std::ostream& AStream) const = 0;

  /**
   * Lecture de l'attribut.
   *
   * Les méthodes de lecture des attributs sont appelées lors du chargement
   * du modèle.
   *
   * @param AStream Le flux d'entrée.
   */
  virtual void load(std::istream& AStream) = 0;

  // @}
  // @name Opérateurs de flux.
  // @{

  /**
   * @param AOS Le flux de sortie.
   * @param AAttribute L'attribut à envoyer dans le flux.
   */
  friend std::ostream& operator<<(std::ostream& AOS,
				  const CAttribute& AAttribute);

  /**
   * @param AIS Le flux d'entrée.
   * @param AAttribute L'attribut à lire depuis le flux.
   */
  friend std::istream& operator>>(std::istream& AIS,
				  CAttribute& AAttribute);

  // @}
  // @name Méthodes de manipulation de la liste des attributs.
  // @{

  /**
   * Récupération de l'attribut successeur.
   *
   * @return Un pointeur sur l'attribut suivant, ou NULL.
   */
  CAttribute* getNext() const;

  /**
   * Chaînage de l'attribut successeur.
   *
   * @param ANext Un pointeur sur l'attribut qui va devenir le suivant.
   */
  void setNext(CAttribute* ANext);

  /**
   * Récupération de l'attribut prédécesseur.
   *
   * @return Un pointeur sur l'attribut précédent, ou NULL.
   */
  CAttribute* getPrev() const;

  /**
   * Chaînage de l'attribut prédécesseur.
   *
   * @param APrev Un pointeur sur l'attribut qui va devenir le précédent.
   */
  void setPrev(CAttribute* APrev);

  // @}

private:
  // @name Champs privés (chaînage des attributs)
  // @{

  /// Attribut précédent.
  CAttribute* FPrev;

  /// Attribut suivant.
  CAttribute* FNext;

  // @}
};
//******************************************************************************
#include INCLUDE_INLINE("attribute.icc")
//******************************************************************************
#endif // ATTRIBUTE_HH
//******************************************************************************
