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
#ifndef GEOMETRY_HH
#define GEOMETRY_HH
//******************************************************************************
#include "inline-macro.hh"
#include "math-extension.hh"
#include "vertex.hh"
//******************************************************************************

/**
 * Cette classe fournit des fonctions permettant des calculs
 * géométriques communément utilisés.
 *
 * Conventions:
 *
 * alpha.b signifie  produit scalaire  entre alpha et b
 *     résultat: ax*bx + ay*by + az*bz
 *
 * alpha*b signifie  produit vectoriel entre alpha et b
 *     résultat: { ay*bz-az*by, az*bx-ax*bz, ax*by-ay*bx }
 *
 * @author MOKA TEAM
 */
class CGeometry
{
public:
  // @name Projections
  // @{

  /**
   * Calcule le projeté orthogonal de AVertex sur la droite de vecteur
   * directeur ALineDirection passant par ALineVertex.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param ALineVertex Un sommet quelconque de la droite
   * @param ALineDirection Un vecteur directeur de la droite
   * @return Le projeté de AVertex sur la droite donnée
   *
   * @precondition ! ADirection.isNull()
   */
  static CVertex orthoProjectVertexOnLine(const CVertex& AVertex,
					  const CVertex& ALineVertex,
					  const CVertex& ALineDirection);

  /**
   * Calcule le projeté orthogonal de AVertex sur le plan d'équation
   * cartésienne AA*x + AB*y + AC*z + AD = 0.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param A? Les paramètres de l'équation cartésienne du plan
   * @return Le projeté de AVertex sur le plan donné
   */
  static CVertex
  orthoProjectVertexOnPlane(const CVertex& AVertex,
			    const TCoordinate& AA, const TCoordinate& AB,
			    const TCoordinate& AC, const TCoordinate& AD);

  /**
   * Calcule le projeté orthogonal de AVertex sur le plan de vecteur normal
   * APlaneNormal passant par APlaneVertex.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param APlaneVertex Un sommet quelconque du plan
   * @param APlaneNormal Un vecteur normal au plan
   * @return Le projeté de AVertex sur le plan donné
   *
   * @precondition !ANormal.isNull()
   */
  static CVertex orthoProjectVertexOnPlane(const CVertex& AVertex,
					   const CVertex& APlaneVertex,
					   const CVertex& APlaneNormal);

  // @}
  // @name Homothéties
  // @{

  /**
   * Calcule l'image de AVertex par l'homothétie de centre ACenter et de
   * coefficient ACoef.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param ACenter Le centre d'homothétie
   * @param ACoef Le coefficient d'homothétie
   * @return L'image de AVertex par l'homothétie donnée
   */
  static CVertex centralHomothety(const CVertex& AVertex,
				  const CVertex& ACenter,
				  const TCoordinate& ACoef);

  /**
   * Calcule l'image de AVertex par l'homothétie de centre ACenter et de
   * coefficient ACoef.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param ACenter Le centre d'homothétie
   * @param ACoef Les coefficients d'homothétie, selon les 3 dimensions
   * @return L'image de AVertex par l'homothétie donnée
   */
  static CVertex centralHomothety(const CVertex& AVertex,
				  const CVertex& ACenter,
				  const CVertex& ACoef);

  /**
   * Calcule l'image de AVertex par l'homothétie axiale de coefficient ACoef et
   * d'axe la droite passant par ALineVertex et de vecteur directeur
   * ALineDirection.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param ALineVertex Un sommet de l'axe d'homothétie
   * @param ALineDirection Un vecteur directeur de l'axe d'homothétie
   * @param ACoef Le coefficient d'homothétie
   * @return L'image de AVertex par l'homothétie donnée
   *
   * @precondition ! ALineDirection.isNull()
   */
  static CVertex axialHomothety(const CVertex& AVertex,
				const CVertex& ALineVertex,
				const CVertex& ALineDirection,
				const TCoordinate& ACoef);

  /**
   * Calcule l'image de AVertex par l'homothétie planaire de coefficient ACoef
   * selon le plan passant par APlaneVertex et ayant APlaneNormal pour vecteur
   * normal.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param APlaneVertex Un sommet du plan d'homothétie
   * @param APlaneNormal Un vecteur normal du plan d'homothétie
   * @param ACoef Le coefficient d'homothétie
   * @return L'image de AVertex par l'homothétie donnée
   *
   * @precondition ! APlaneNormal.isNull()
   */
  static CVertex planarHomothety(const CVertex& AVertex,
				 const CVertex& APlaneVertex,
				 const CVertex& APlaneNormal,
				 const TCoordinate& ACoef);

  // @}
  // @name Calcul de la distance d'un sommet à un autre objet
  // @{

  /**
   * Calcule la distance euclidienne du sommet AVertex au sommet AOtherVertex.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param AOtherVertex Un sommet quelconque de l'espace
   * @return La distance entre les deux sommets
   */
  static TCoordinate distanceToVertex(const CVertex& AVertex,
				      const CVertex& AOtherVertex);

  /**
   * Calcule la distance euclidienne du sommet AVertex à la droite de vecteur
   * directeur ALineDirection passant par ALineVertex, c'est-à-dire la distance
   * de AVertex à son projeté orthogonal sur cette droite.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param ALineVertex Un sommet quelconque de la droite
   * @param ALineDirection Un vecteur directeur de la droite
   * @return La distance du sommet à la droite
   *
   * @precondition ! ALineDirection.isNull()
   */
  static TCoordinate distanceToLine(const CVertex& AVertex,
				    const CVertex& ALineVertex,
				    const CVertex& ALineDirection);

  /**
   * Calcule la distance euclidienne du sommet AVertex au plan de vecteur
   * normal APlaneNormal passant par APlaneVertex, c'est-à-dire la distance de
   * AVertex à son projeté orthogonal sur ce plan.
   *
   * @param AVertex Un sommet quelconque de l'espace
   * @param APlaneVertex Un sommet quelconque du plan
   * @param APlaneNormal Un vecteur normal du plan
   * @param Ka distance du sommet au plan
   *
   * @precondition ! APlaneNormal.isNull()
   */
  static TCoordinate distanceToPlane(const CVertex& AVertex,
				     const CVertex& APlaneVertex,
				     const CVertex& APlaneNormal);

  // @}
  // @name Fonctions géométriques diverses
  // @{

  /**
   * Calcule la bissectrice des vecteurs AVector1 et AVector2.
   * Le sens de la bissectrice est déterminé par le signe de la valeur
   * (AVector1 * AVector2).ANormal.
   *
   * @param AVector1 Un premier vecteur
   * @param AVector2 Un deuxième vecteur
   * @param ANormal  Un vecteur normal aux deux premiers (paramètre facultatif)
   *   permettant de déterminer le signe de l'angle.
   * @return La bissectrice des vecteurs AVector1 et AVector2
   */
  static CVertex getBissectrix(const CVertex& AVector1,
			       const CVertex& AVector2,
			       const CVertex& ANormal = ORIGIN);

  /**
   * Calcule un vecteur normal à AVector, si possible se trouvant dans le plan
   * XY.
   *
   * @param AVector Un vecteur non nul
   * @return Un vecteur normal à AVector et de même norme que celui-ci
   */
  static CVertex getNormalVector(const CVertex& AVector);

  /**
   *  Calcule un vecteur normal à AVector1 et AVector2 (en effectuant le produit
   *  vectoriel de AVector1 et AVector2).
   *
   *  L'intérêt de cette méthode est que si aucun des vecteurs n'est le vecteur
   *  nul et si les vecteurs ne sont pas colinéaires, la norme du vecteur
   *  retourné est égale à la moyenne des normes des vecteurs AVector1 et
   *  AVector2.
   *
   *  @param AVector1 Un vecteur quelconque
   *  @param AVector2 Un vecteur quelconque
   *  @return Un vecteur normal à AVector1 et AVector2
   */
  static CVertex getNormalVector(const CVertex& AVector1,
				 const CVertex& AVector2);

  /**
   *  Calcule un vecteur normal aux vecteurs (AVertex1,AVertex2) et
   * (AVertex2,AVertex3).
   */
  static CVertex getNormalVector(const CVertex& AVertex1,
				 const CVertex& AVertex2,
				 const CVertex& AVertex3);

  /**
   * Calcule l'intersection entre deux droites.
   * Chaque droite est donnée par deux sommets.
   *
   * Le résultat est imprévisible si les deux droites ne se croisent pas.
   *
   * @param ALine1Vertex1 Un premier sommet de la première droite
   * @param ALine1Vertex2 Un deuxième sommet de la première droite
   * @param ALine2Vertex1 Un premier sommet de la deuxième droite
   * @param ALine2Vertex2 Un deuximème sommet de la deuxième droite
   *
   * @precondition ALine1Vertex1 != ALine1Vertex2
   * @precondition ALine2Vertex1 != ALine2Vertex2
   */
  static CVertex getLinesIntersection(const CVertex& ALine1Vertex1,
				      const CVertex& ALine1Vertex2,
				      const CVertex& ALine2Vertex1,
				      const CVertex& ALine2Vertex2);

  /**
   * Teste si deux vecteurs sont colinéaires.
   *
   * @param AVector1 Un vecteur quelconque
   * @param AVector2 Un vecteur quelconque
   * @return Un booléen indiquant si AVector1 et AVector2 sont colinéaires
   */
  static bool areColinear(const CVertex& AVertex1, const CVertex& AVertex2);

  /**
   * Calcule l'angle entre les vecteurs AVector1 et AVector2.
   * Le signe de l'angle est donné par le signe de la valeur de
   * (AVector1*AVector2).ANormal.
   *
   * @param AVector1 Un premier vecteur
   * @param AVector2 Un deuxième vecteur
   * @param ANormal  Un vecteur normal aux deux premiers, permettant de
   *                 déterminer le signe de l'angle
   * @return Un angle
   */
  static TCoordinate getAngle(const CVertex& AVector1,
			      const CVertex& AVector2,
			      const CVertex& ANormal = ORIGIN);

  /**
   * Calcule les 3 angles de rotation permettant d'obtenir le repère
   * (AVx,AVy,AVz) en trois rotations à partir du repère (OX,OY,OZ). AVy vaut
   * AVz*AVx.  Le résultat est un triplet (alpha,beta,gamma) tel que si l'on
   * applique la méthode CTransformationMatrix::rotate(AAngles) sur le repère
   * (OX,OY,OZ), on obtient le repère (AVx,AVy,AVz) à la norme des vecteurs
   * près.
   * Si les vecteurs AVx et AVz ne sont pas orthogonaux, AVx est projeté sur
   * le plan orthogonal à AVz et passant par l'origine.
   *
   * @param AVx Un premier vecteur
   * @param AVz Un deuxième vecteur
   * @return Trois angles de rotation
   */
  static CVertex getAngles(const CVertex& AVx, const CVertex& AVz);

  // @}
  // @name Méthodes auxiliaires pour le coraffinement 2D
  // @{

  /**
   * Localise le sommet AVertex par rapport à la droite donnée.
   * La valeur retournée vaut:
   *  -1 si le sommet est à gauche de la droite,
   *   0 si le sommet est sur la droite,
   *  +1 si le sommet est à droite de la droite.
   *
   * Remarque: Les calculs tiennent compte des erreurs d'arrondi.
   *
   * @param AVertex Le sommet à localiser
   * @param ALineVertex Un sommet de la droite par rapport à laquelle localiser
   *   le sommet AVertex
   * @param ALineDirection Un vecteur directeur de la droite par rapport à
   *   laquelle localiser le sommet AVertex
   * @param APlaneNormal Un vecteur normal au plan dans lequel on travaille
   * @return Un entier
   *
   * @precondition !ALineDirection.isNull()
   * @precondition !APlaneNormal.isNull()
   */
  static int localiseVertexComparedToLine(const CVertex& AVertex,
					  const CVertex& ALineVertex,
					  const CVertex& ALineDirection,
					  const CVertex& APlaneNormal);

  /**
   * Teste si les segments AB et CD s'intersectent.
   * Les résultats sont stockés dans les paramètres AIntersection-On.
   * AIntersection-On peut prendre la valeur:
   *   0 : pas d'intersection,
   *   1 : intersection franche sur le segment AB
   *   2 : intersection franche sur le segment CD.
   *
   * Si un paramètre AIntersection-On vaut vrai, alors la méthode affecte les
   * coordonnées du point d'intersection à AIntersection-, sinon
   * AIntersection- n'est pas modifié.
   *
   * Remarque : s'il n'y alpha qu'une seule intersection, celle-ci n'est pas
   * forcément placée dans AIntersection1.
   *
   * La méthode fonctionne dans toutes les configurations (arêtes disjointes,
   * sécantes, se chevauchant, confondues, etc...)
   *
   * @param AVertexA Un sommet quelconque
   * @param AVertexB Un sommet quelconque
   * @param AVertexC Un sommet quelconque
   * @param AVertexD Un sommet quelconque
   * @param APlaneNormal Un vecteur normal au plan des segments
   * @param ACutABSegment Un booléen indiquant si l'arête AB est coupée par
   *   l'arête CD de manière franche
   * @param AIntersectionOnAB Une référence sur un sommet où sont stockées les
   *   coordonnées du point par lequel CD coupe AB
   * @param ACutCDSegment Un booléen indiquant si l'arête CD est coupée par
   *   l'arête AB de manière franche
   * @param AIntersectionOnCD Une référence sur un sommet où sont stockées les
   *   coordonnées du point par lequel AB coupe CD
   *
   * @precondition AVertexA != AVertexB
   * @precondition AVertexC != AVertexD
   * @precondition !APlaneNormal.isNull()
   */
  static void getSegmentsIntersection(const CVertex& AVertexA,
				      const CVertex& AVertexB,
				      const CVertex& AVertexC,
				      const CVertex& AVertexD,
				      const CVertex& APlaneNormal,
				      int& AIntersection1On,
				      CVertex& AIntersection1,
				      int& AIntersection2On,
				      CVertex& AIntersection2);

  /**
   *  Méthode équivalente à getSegmentsIntersection, mais qui travaille dans
   *  le plan.
   */
  static void getSegmentsIntersection2d(const CVertex& AVertexA,
					const CVertex& AVertexB,
					const CVertex& AVertexC,
					const CVertex& AVertexD,
					int& AIntersection1On,
					CVertex& AIntersection1,
					int& AIntersection2On,
					CVertex& AIntersection2);

  // @}
  // @name Méthodes pour le chanfreinage :
  // @{

  /**
   * @param AVertex_ trois sommets A,B,C
   * @return le projeté orthogonal du vecteur AC sur le vecteur normal
   *   au vecteur AB dans le plan ABC.
   */
  static CVertex getRoundingNormalVector(const CVertex& AVertex1, 
					 const CVertex& AVertex2,
					 const CVertex& AVertex3);

  // @}
};
//******************************************************************************
#include INCLUDE_INLINE("geometry.icc")
//******************************************************************************
#endif // GEOMETRY_HH
//******************************************************************************
