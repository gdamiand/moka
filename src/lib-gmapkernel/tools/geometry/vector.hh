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
#ifndef VECTOR_HH
#define VECTOR_HH
//******************************************************************************
#include "math-extension.hh"
#include "inline-macro.hh"

#include <iostream>
//******************************************************************************
class CTransformationMatrix;

/**
 * Définit la classe permettant de manipuler des vecteurs de dimension 4.
 *
 * @author MOKA TEAM
 */
class CVector
{
private:
  TCoordinate* FElements;

public:
  // @name Constantes
  // @{

  /**
   * Types de vecteurs particuliers.
   */
  typedef enum { NullVector, AnyVector } TVectorType;

  // @}

  // @name Constructeurs et destructeur
  // @{

  /**
   * Constructeur par défaut
   * Construction d'une nouvelle instance de la classe, NON INITIALISÉE.
   *
   * @param AType Le type d'initialisation à effectuer (AnyVector par défaut)
   * @return Une nouvelle instance de la classe
   */
  CVector(TVectorType AType = AnyVector);

  /**
   * Constructeur par copie
   * Construction d'une nouvelle instance de la classe, en copiant le vecteur
   * passé en paramètre.
   *
   * @param AVector le vecteur à copier
   * @return Une nouvelle instance de la classe
   */
  CVector(const CVector& AVector);

  /**
   * Constructeur avec initialisation
   * Construction d'une nouvelle instance de la classe, avec initialisation
   * explicite.
   *
   * @param A-- Les valeurs des cellules du vecteur.
   * @return Une nouvelle instance de la classe
   */
  CVector(TCoordinate A0, TCoordinate A1, TCoordinate A2, TCoordinate A3);

  /**
   * Destructeur
   */
  ~CVector();

  // @}
  // @name Initialisations
  // @{

  /**
   * Réinitialise l'instance au vecteur nul.
   */
  void setToNull();

  /**
   * @param AVector Le vecteur initial
   * @return La matrice courante obtenue après affectation
   */
  CVector& operator=(const CVector& AVector);

  // @}
  // @name Accesseur aux éléments du vecteur
  // @{

  /**
   * Permet d'accéder (en lecture et en écriture) à un élément du vecteur.
   *
   * @param AIndex L'indice de l'élément à atteindre
   *
   * @precondition 0 <= AIndex <= 3
   */
  TCoordinate& operator[](int AIndex) const;

  // @}
  // @name Produit de matrices
  // @{

  /**
   * Calcule le produit du vecteur courant par la matrice AMatrix passée
   * en paramètre.
   *
   * @param AMatrix Une matrice quelconque
   * @return Le résultat du produit du vecteur courant par la matrice
   *   AMatrix
   */
  CVector operator*(const CTransformationMatrix& AMatrix) const;

  /**
   * Calcule le produit du vecteur courant par le vecteur AVector passé
   * en paramètre.
   *
   * @param AVector Un vecteur quelconque
   * @return Le résultat du produit du vecteur courant par le vecteur
   *   AVector
   */
  TCoordinate operator*(const CVector& AVector) const;

  // @}
};

// @name Affichage
// @{

/**
 * @param AStream Le flot dans lequel afficher le vecteur
 * @param AVector Le vecteur à afficher
 * @return Le flot utilisé pour l'affichage
 */
std::ostream& operator<<(std::ostream& AStream, const CVector& AVector);

// @}
//******************************************************************************
#include INCLUDE_INLINE("vector.icc")
//******************************************************************************
#endif // VECTOR_HH
//******************************************************************************
