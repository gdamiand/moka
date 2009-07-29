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
#ifndef EMBEDDING_HH
#define EMBEDDING_HH
//******************************************************************************
#include "kernel-types.hh"
#include "inline-macro.hh"
//******************************************************************************
class CAttribute;

/**
 * Cette classe représente une liste d'attributs associés à la même orbite.
 * Les méthodes de cette classe permettent d'ajouter, supprimer, chercher...
 * un attribut particulier.
 * La plupart de ces méthodes ne font pas de contrôle car elles sont appelées
 * de CDart.
 *
 * @author MOKA TEAM
 */
class CEmbedding
{
public:
  // @name Constructeurs et destructeur
  // @{

  /**
   * Construction d'une nouvelle instance de la classe.
   * Le champ id est initialisé.
   *   La liste d'attribut est initialisée à vide.
   *
   * @param AOrbit Une orbite
   * @return Une nouvelle instance de la classe
   */
  CEmbedding(TOrbit AOrbit);

  /**
   * Construction par recopie d'une nouvelle instance de la classe.
   *
   * @param AEmbedding L'instance à recopier
   * @return Une nouvelle instance de la classe
   */
  CEmbedding(const CEmbedding& AEmbedding);

  /**
   * Destruction de l'instance.
   * Tous les attributs de la liste sont éventuellement détruits
   * par l'appel de la méthode destroy de la classe attribut.
   */
  ~CEmbedding();

  /**
   * Méthode pour recopier un CEmbedding.
   *
   * Construction d'une nouvelle instance de la classe.
   * La liste des attributs de cette instance est initialisée par l'appel
   * de la méthode copy sur chaque attribut de AEmbedding.
   * Le champ id est initialisé avec la même orbite que celle de AEmbedding
   *
   * @return Une copie de l'instance. Cette copie peut etre l'instance
   *   elle-même. (par exemple si l'on veut des embedding partages.)
   */
  CEmbedding* copy() const;

  /**
   * Méthode pour suprimer un CEmbedding.
   *
   * Méthode pendante de la méthode copy. Si la méthode copy cré une nouvelle
   * instance, la methode destroy doit la détruire. Dans le cas d'embedding
   * partagés, on peut par exemple compter le nombre de réferences.
   */
  void destroy();

  // @}

  // @name Accesseurs
  // @{

  /**
   * Accesseur en lecture de l'orbite de l'instance
   */
  TOrbit getOrbit() const;

  /**
   * Accesseur en lecture du prochain CEmbedding dans la liste
   *
   * Le chaînage des CEmbedding est effectué dans cette classe.
   * Cet accesseur permet de récuperer le prochain CEmbedding dans cette
   * liste.
   */
  CEmbedding* getNext() const;

  /**
   * Modifie le chainage des CEmbedding. L'embedding suivant de l'instance
   * est désormais AEmbedding.
   *
   * @param AEmbedding Un pointeur vers un CEmbedding
   */
  void setNext(CEmbedding* AEmbedding);

  /**
   * Accesseur en lecture du précédent CEmbedding dans la liste
   *
   * @return Un pointeur vers un CEmbedding
   */
  CEmbedding* getPrev() const;

  /**
   * Modifie le chainage des CEmbedding. L'CEmbedding précédent de
   * l'instance est désormais AEmbedding.
   *
   * @param AEmbedding Un pointeur vers un CEmbedding
   */
  void setPrev(CEmbedding* AEmbedding);

  /**
   * Accesseur en lecture du premier attribut de la liste
   */
  CAttribute* getFirstAttribute() const;

  /**
   * Accesseur en écriture du premier attribut de la liste
   * Affecte le premier attribut de la liste qui est désormais AAttribute
   *
   * @param AAttribute Un pointeur vers un CAttribute
   */
  void setFirstAttribute(CAttribute* AAttribute);

  /**
   * Accesseur pour tester si la liste des attributs est vide
   *
   * @return Un booléen indiquant si la liste est vide
   */
  bool isEmpty() const;

  // @}
  // @name Méthodes pour travailler sur les attributs
  // @{

  /**
   * Cherche s'il existe un attribut dans la liste ayant comme identifiant
   * attribType. Si oui retourne un pointeur vers cet attribut, sinon
   * retourne NULL.
   *
   * @param attribType Un TAttributeId
   * @return Un pointeur vers un CAttribute
   */
  CAttribute* getAttribute(TAttributeId AAttribType) const;

  /**
   * Ajoute l'attribut dans la liste
   *
   * @param AAttribute un pointeur vers un CAttribute
   *
   * @precondition getAttribute(AAttribute->getType()) == NULL
   * @precondition AAttribute->getPrev() == AAttribute->getNext() == NULL
   */
  void addAttribute(CAttribute* AAttribute);

  /**
   * Enlève (sans le détruire) l'attribut de la liste.
   *
   * @param AAttribute un pointeur vers un CAttribute
   * @return Le pointeur vers l'attribut enlevé de la liste
   *
   * @precondition AAttribute fait partie de la liste des attributs de
   *   l'instance.
   */
  CAttribute* removeAttribute(CAttribute* AAttribute);

  /**
   * Enlève l'attribut ayant l'identifiant attribType de la liste. Si aucun
   * attribut n'est de ce type, alors retourne NULL.
   *
   * @param attribType un TAttributeId
   * @return Le pointeur vers l'attribut enlevé de la liste, ou NULL
   */
  CAttribute* removeAttribute(TAttributeId attribType);

  /**
   * Enlève l'attribut de la liste et le détruit eventuellement en appelant
   * la méthode destroy sur cet attribut.
   *
   * @param AAttribute un pointeur vers un CAttribute
   *
   * @precondition AAttribute fait partie de la liste des atributs de
   *   l'instance
   */
  void deleteAttribute(CAttribute* AAttribute);

  /**
   * Enlève l'attribut ayant l'identifiant attribType de la liste et le
   * détruit eventuellement en appelant la méthode destroy sur cet attribut.
   * Si aucun attribut n'est de ce type, ne fait rien.
   *
   * @param attribType un TAttributeId
   */
  void deleteAttribute(TAttributeId attribType);

  /**
   * Ajoute tout les attributs de AEmbedding non présent dans la liste des
   * attributs de l'instance dans cette liste. Ces attributs sont enlevés
   * de la liste des attributs de AEmbedding.
   *
   * @param AEmbedding un pointeur vers un CEmbedding
   *
   * @precondition l'instance et AEmbedding concernent la même orbite
   */
  void mergeAttribute(CEmbedding* AEmbedding);

  // @}

private:
  // @name Champs privés
  // @{

  /// Pointeur vers le premier CAttribute de la liste
  CAttribute* FFirstAttribute;

  /// Identifiant de l'orbite de l'instance
  TOrbit FId;

  /// Pour le double-chainage des CEmbedding, utilisé par la classe CDart
  CEmbedding* FPrev;
  CEmbedding* FNext;

  // @}
};
//******************************************************************************
#include INCLUDE_INLINE("embedding.icc")
//******************************************************************************
#endif // EMBEDDING_HH
//******************************************************************************
