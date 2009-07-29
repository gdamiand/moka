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
#ifndef TRANSFORMATION_MATRIX_HH
#define TRANSFORMATION_MATRIX_HH
//******************************************************************************
#include "math-extension.hh"
#include "vertex.hh"
#include "inline-macro.hh"

#include <iostream>
//******************************************************************************

class CVector;

/**
 * Définit la classe permettant d'effectuer des transformations géométriques
 * sur les objets en utilisant des matrices 4x4.
 *
 * @author MOKA TEAM
 */
class CTransformationMatrix
{
private:
  TCoordinate FElements[4][4];

public:
  // @name Constantes
  // @{

  /**
   * Types de matrices particulières.
   */
  typedef enum { NullMatrix, IdentityMatrix, AnyMatrix } TMatrixType;

  // @}
  // @name Constructeurs et destructeur
  // @{

  /**
   * Constructeur par défaut
   * Construction d'une nouvelle instance de la classe, initialisée à la
   * matrice identité (voir la méthode setToIdentity).
   *
   * @param AType Le type d'initialisation à effectuer (Identity par défaut)
   * @return Une nouvelle instance de la classe
   */
  CTransformationMatrix(TMatrixType AType = IdentityMatrix);

  /**
   * Constructeur par copie
   * Construction d'une nouvelle instance de la classe, en copiant la matrice
   * passée en paramètre.
   *
   * @param AMatrix la matrice à copier
   * @return Une nouvelle instance de la classe
   */
  CTransformationMatrix(const CTransformationMatrix& AMatrix);

  /**
   * Constructeur avec initialisation
   * Construction d'une nouvelle instance de la classe, avec initialisation
   * explicite.
   *
   * @param A-- Les valeurs des cellules de la matrice.
   * @return Une nouvelle instance de la classe
   */
  CTransformationMatrix(TCoordinate A00, TCoordinate A01,
			TCoordinate A02, TCoordinate A03,

			TCoordinate A10, TCoordinate A11,
			TCoordinate A12, TCoordinate A13,

			TCoordinate A20, TCoordinate A21,
			TCoordinate A22, TCoordinate A23,

			TCoordinate A30, TCoordinate A31,
			TCoordinate A32, TCoordinate A33);

  /**
   * Constructeur avec initialisation à partir de sommets
   * Construction d'une nouvelle instance de la classe, avec initialisation.
   * Les coordonnées du sommet ALine1 sont placées sur la première ligne,
   * celles de ALine2 sur la deuxième ligne et celles de ALine3 sur la
   * troisième ligne.
   * La dernière ligne et la dernière colonne de la matrice sont initialisées
   * avec la valeur 0.
   *
   * @param ALine- Des sommets
   * @return Une nouvelle instance de la classe
   */
  CTransformationMatrix(const CVertex& ALine1,
			const CVertex& ALine2,
			const CVertex& ALine3);

  /**
   * Destructeur
   */
  ~CTransformationMatrix();

  // @}
  // @name Initialisations
  // @{

  /**
   * Réinitialise l'instance à la matrice nulle.
   * La matrice nulle est composée uniquement de 0:
   *                   [0 0 0 0]
   *                   [0 0 0 0]
   *                   [0 0 0 0]
   *                   [0 0 0 0]
   */
  void setToNull();

  /**
   * Réinitialise l'instance à la matrice identité.
   * La matrice identité est composée de 1 sur sa première diagonale et de 0
   * partout ailleurs: [1 0 0 0]
   *                   [0 1 0 0]
   *                   [0 0 1 0]
   *                   [0 0 0 1]
   */
  void setToIdentity();

  /**
   * Opérateur d'affectation
   *
   * @param AMatrix La matrice initiale
   * @return La matrice courante obtenue après affectation
   */
  CTransformationMatrix& operator=(const CTransformationMatrix& AMatrix);

  // @}
  // @name Accesseur aux éléments de la matrice
  // @{

  /**
   * Permet d'accéder (en lecture et en écriture) à une ligne de la matrice.
   *
   * @param ARow La ligne de la matrice à atteindre
   *
   * @precondition ARow est compris entre 0 et 3 inclus.
   */
  TCoordinate* operator[](int ARow) const;

  // @}
  // @name Fonctions mathématiques (produit, calcul de déterminant, ...)
  // @{

  /**
   * Calcule le produit de la matrice courante par la matrice AMatrix passée
   * en paramètre.
   *
   * @param AMatrix Une matrice quelconque
   * @return Le résultat du produit de la matrice courante par la matrice
   *   AMatrix
   */
  CTransformationMatrix operator*(const CTransformationMatrix& AMatrix) const;

  /**
   * Calcule le produit de la matrice courante par le vecteur AVector passé
   * en paramètre.
   *
   * @param AVector Un vecteur quelconque
   * @return Le résultat du produit de la matrice courante par le vecteur
   *   AVector
   */
  CVector operator*(const CVector& AVector) const;

  // @}
  // @name Méthodes correspondant aux transformations géométriques
  // @{

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une translation de vecteur AVector.
   *
   * @param AVector Le vecteur de translation
   */
  void translate(const CVertex& AVector);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une rotation de AAngle degrés autour de l'axe passant par l'origine et
   * dont le vecteur directeur est donné par le paramètre AAxeDirection.
   *
   * @param AAxeDirection Le vecteur directeur de l'axe de rotation
   * @param AAngle L'angle (en degrés) de la rotation
   *
   * @precondition ! AAxeDirection.isNull()
   */
  void rotate(const CVertex& AAxeDirection, TCoordinate AAngle);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une rotation de AAngle degrés autour de l'axe passant par le sommet
   * AAxeVertex et dont le vecteur directeur est donné par le paramètre
   * AAxeDirection.
   *
   * @param AAxeVertex Un sommet quelconque de l'axe de rotation
   * @param AAxeDirection Le vecteur directeur de l'axe de rotation
   * @param AAngle L'angle (en degrés) de la rotation
   *
   * @precondition AAxe n'est pas un vecteur nul.
   */
  void rotate(const CVertex& AAxeVertex,
	      const CVertex& AAxeDirection, TCoordinate AAngle);

  /**
   * Effectue trois rotations sur le repère de base (OX,OY,OZ):
   *  - la première  autour de l'axe OX et d'angle AAngles.getX(),
   *  - la deuxième  autour de l'axe OY et d'angle AAngles.getY(),
   *  - la troisième autour de l'axe OZ et d'angle AAngles.getZ().
   *
   * Remarque: les rotations sont faites autour des axes du repère
   * local. La deuxième rotation se fait donc autour de l'axe OY
   * qui alpha été modifié par la première rotation. La troisième rotation
   * se fait autour de l'axe OZ qui alpha été modifié par les deux
   * premières rotations.
   *
   * @param AAngles Les angles de rotation
   */
  void rotate(const CVertex& AAngles);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une rotation de 180 degrés autour de l'axe passant par l'origine et
   * dont le vecteur directeur est donné par le paramètre AAxeDirection.
   *
   * @param AVertex Le vecteur directeur de l'axe de rotation
   *
   * @precondition !AAxeDirection.isNull()
   */
  void rotate180(const CVertex& AAxeDirection);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une homothétie centrée sur l'origine dont le coefficient est donné par
   * le paramètre ACoef.
   *
   * @param ACoef Le coefficient de l'homothétie
   */
  void scale(TCoordinate ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une opération d'étirement dont les coefficients sont donnés par le
   * paramètre ACoef.
   *
   * @param ACoef Les coefficients dans les trois dimensions de l'étirement
   */
  void scale(const CVertex& ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une homothétie de centre ACenter et de coefficient ACoef.
   *
   * @param ACenter Le centre d'homothétie
   * @param ACoef Le coefficient d'homothétie
   */
  void scale(const CVertex& ACenter, TCoordinate ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une opération d'étirement de center ACenter et dont les coefficients
   * sont donnés par le paramètre ACoef.
   *
   * @param ACenter Le centre de l'étirement
   * @param ACoef Les coefficients dans les trois dimensions de l'étirement
   */
  void scale(const CVertex& ACenter, const CVertex& ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une homothétie d'axe la droite de vecteur directeur AAxeDirection
   * passant par l'origine et de coefficient ACoef.
   *
   * @param AAxeDirection Un vecteur directeur de l'axe
   * @param ACoef Le coefficient d'homothétie
   *
   * @precondition ! AAxeDirection.isNull()
   */
  void axialScale(const CVertex& AAxeDirection, TCoordinate ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une homothétie d'axe la droite de vecteur directeur AAxeDirection
   * passant par le sommet AAxeVertex et de coefficient ACoef.
   *
   * @param AAxeVertex Un sommet de l'axe
   * @param AAxeDirection Le vecteur directeur de l'axe
   * @param ACoef Le coefficient d'homothétie
   *
   * @precondition ! AAxeDirection.isNull()
   */
  void axialScale(const CVertex& AAxeVertex,
		  const CVertex& AAxeDirection, TCoordinate ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une homothétie selon le plan de vecteur normal APlaneNormal passant par
   * l'origine et de coefficient ACoef.
   *
   * @param APlaneNormal Un vecteur normal du plan
   * @param ACoef Le coefficient d'homothétie
   *
   * @precondition ! APlaneNormal.isNull()
   */
  void planarScale(const CVertex& APlaneNormal, TCoordinate ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une homothétie selon le plan de vecteur normal APlaneNormal passant par
   * le sommet APlaneVertex et de coefficient ACoef.
   *
   * @param APlaneVertex Un sommet du plan
   * @param APlaneNormal Un vecteur normal du plan
   * @param ACoef Le coefficient d'homothétie
   *
   * @precondition ! APlaneNormal.isNull()
   */
  void planarScale(const CVertex& APlaneVertex,
		   const CVertex& APlaneNormal, TCoordinate ACoef);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une rotation qui projette AVector1 sur AVector2.
   * L'axe de la rotation est donné par le vecteur AVector1*AVector2.
   *
   * La taille des vecteurs n'est pas prise en compte (pas d'étirement).
   *
   * @param AVector1 Un premier vecteur
   * @param AVector2 Un second vecteur
   *
   * @precondition Aucun des vecteurs passés en paramètre n'est le vecteur nul.
   */
  void orientate(const CVertex& AVector1, const CVertex& AVector2);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une rotation autour du sommet ACenter qui projette AVector1 sur
   * AVector2.
   * L'axe de la rotation est donné par le vecteur AVector1*AVector2.
   *
   * La taille des vecteurs n'est pas prise en compte (pas d'étirement).
   *
   * @param ACenter  Le sommet origine
   * @param AVector1 Un premier vecteur
   * @param AVector2 Un second vecteur
   *
   * @precondition Aucun des vecteurs passés en paramètre n'est le vecteur nul.
   */
  void orientate(const CVertex& ACenter,
		 const CVertex& AVector1, const CVertex& AVector2);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à une ou plusieurs rotations qui correspondent au changement de repère
   * (AVectorU1, AVectorV1) vers (AVectorU2, AVectorV2).
   *
   * Si les vecteurs AVectorUi et AVectorVi ne sont pas orthogonaux, AVectorVi
   * est projeté sur le plan orthogonal à AVectorUi et passant par l'origine.
   * Le troisième vecteur de chaque repère est déterminé par produit vectoriel
   * des deux premiers.
   *
   * La taille des vecteurs n'est pas prise en compte (pas d'étirement).
   *
   * @param AVectorU1 Le premier vecteur du premier repère
   * @param AVectorV1 Le second vecteur du premier repère
   * @param AVectorU2 Le premier vecteur du deuxième repère
   * @param AVectorV2 Le second vecteur du deuxième repère
   *
   * @precondition Aucun des vecteurs passés en paramètre n'est le vecteur nul.
   */
  void orientate(const CVertex& AVectorU1, const CVertex& AVectorV1,
		 const CVertex& AVectorU2, const CVertex& AVectorV2);

  /**
   * Modifie la matrice courante en la combinant avec la matrice correspondant
   * à un changement de repère.
   *
   * @param AOrigin1  L'origine du premier repère
   * @param AVectorU1 Le premier vecteur du premier repère
   * @param AVectorV1 Le second vecteur du premier repère
   * @param AOrigin2  L'origine du second  repère
   * @param AVectorU2 Le premier vecteur du deuxième repère
   * @param AVectorV2 Le second vecteur du deuxième repère
   * @param ACoef     Les coefficients d'étirement dans les 3 dimensions
   *
   * @precondition Aucun des vecteurs passés en paramètre n'est le vecteur nul.
   */
  void transform(const CVertex& AOrigin1 ,
		 const CVertex& AVectorU1, const CVertex& AVectorV1,
		 const CVertex& AOrigin2 ,
		 const CVertex& AVectorU2, const CVertex& AVectorV2,
		 const CVertex& ACoef = CVertex(1,1,1));

  // @}
  // @name Application de la matrice sur un sommet
  // @{

  /**
   * @param AVertex Le sommet sur lequel appliquer la transformation
   */
  void applyOn(CVertex& AVertex) const;

  /**
   * @param AVertex Le sommet sur lequel appliquer la transformation
   * @param AResult Le sommet obtenu après transformation
   */
  void applyOn(const CVertex& AVertex, CVertex& AResult) const;

  // @}
};

// @name Matrices de lissage
// @{

extern const CTransformationMatrix COONS     ;
extern const CTransformationMatrix HERMITE   ;
extern const CTransformationMatrix BEZIER    ;
extern const CTransformationMatrix OVERHAUSER;
extern const CTransformationMatrix SPLINE    ;

// @}
// @name Affichage
// @{

/**
 * @param AStream Le flot dans lequel afficher la matrice
 * @param AMatrix La matrice à afficher
 * @return Le flot utilisé pour l'affichage
 */
std::ostream& operator<<(std::ostream& AStream,
			 const CTransformationMatrix& AMatrix);

// @}
//******************************************************************************
#include INCLUDE_INLINE("transformation-matrix.icc")
//******************************************************************************
#endif // TRANSFORMATION_MATRIX_HH
//******************************************************************************
