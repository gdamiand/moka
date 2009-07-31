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
#ifndef PLANE_HH
#define PLANE_HH
//******************************************************************************
#include <cassert>

#include "inline-macro.hh"
#include "vertex.hh"

/// Les différents plan de projection possibles.
typedef enum {XY_Proj, YZ_Proj, XZ_Proj} TProjection;
//******************************************************************************
/**
 * Classe définissant un plan.
 */
class CPlane
{
public:

  /// @name Constructeurs et destructeur

  //@{

  /**
   * Construction d'une nouvelle instance de la classe initialisée
   * avec un plan non défini.
   *
   * @return Une nouvelle instance de la classe
   */
  CPlane();

  /**
   * Construction d'une nouvelle instance de la classe initialisée avec un
   * plan ayant pour équation Aa * x + Ab * y + Ac * z + Ad = 0.
   *
   * @param Aa Coefficient 'a' de l'équation du plan
   * @param Ab Coefficient 'b' de l'équation du plan
   * @param Ac Coefficient 'c' de l'équation du plan
   * @param Ad Coefficient 'd' de l'équation du plan
   * @return Une nouvelle instance de la classe
   */
  CPlane(TCoordinate Aa,TCoordinate Ab, TCoordinate Ac, TCoordinate Ad);

  /**
   * Construction d'une nouvelle instance de la classe initialisée avec un
   * plan passant par les 3 sommets passés en paramètre.
   *
   * @param APoint1 Un point par lequel passe le plan
   * @param APoint2 Un point par lequel passe le plan
   * @param APoint3 Un point par lequel passe le plan
   * @return Une nouvelle instance de la classe
   */
  CPlane(const CVertex & APoint1,
	 const CVertex & APoint2,
	 const CVertex & APoint3);

  /**
   *  Construction d'une nouvelle instance de la classe initialisée avec un
   *  plan ayant pour normale ANormal et qui passe par APoint.
   *
   *  @param ANormal Le vecteur normale au plan
   *  @param APoint Un point par lequel passe le plan
   *  @return Une nouvelle instance de la classe
   */
  CPlane(const CVertex & ANormal, const CVertex & APoint);

  ///Destruction de l'instance.
  virtual ~CPlane();

  //@}

  /// @name Méthodes publiques

  //@{

  /**
   * Affecte au plan l'équation Aa * x + Ab * y + Ac * z + Ad = 0.
   *
   * @param Aa Coefficient 'a' de l'équation du plan
   * @param Ab Coefficient 'b' de l'équation du plan
   * @param Ac Coefficient 'c' de l'équation du plan
   * @param Ad Coefficient 'd' de l'équation du plan
   */
  void setPlane(TCoordinate Aa,TCoordinate Ab, TCoordinate Ac, TCoordinate Ad);

  /**
   * Modifie le plan pour qu'il passe par les 3 points passé en paramètre.
   *
   * @param APoint1 Un point par lequel passe le plan
   * @param APoint2 Un point par lequel passe le plan
   * @param APoint3 Un point par lequel passe le plan
   */
  void setPlane(const CVertex & APoint1,
		const CVertex & APoint2,
		const CVertex & APoint3);

  /**
   * Modifie le plan pour qu'il ait comme normale ANormal et qu'il passe par
   * le point APoint.
   *
   *  @param ANormal Le vecteur normale au plan
   *  @param APoint Un point par lequel passe le plan
   */
  void setPlane(const CVertex & ANormal, const CVertex & APoint);
  
  /**
   * Cette méthode permet de récupérer la normale au plan.
   *
   * @return La normale au plan
   */
  const CVertex & getNormal() const;

  /**
   * Cette méthode permet de récupérer la norme de la normale au plan.
   *
   * @return La normale au plan
   */
  TCoordinate getNormalNorm() const;

  /**
   * Cette méthode permet de récupérer le coefficient "a" de l'équation du plan.
   *
   * @return le coefficient "a"
   */
  TCoordinate getA() const;

  /**
   * Cette méthode permet de récupérer le coefficient "b" de l'équation du plan.
   *
   * @return le coefficient "b"
   */
  TCoordinate getB() const;

  /**
   * Cette méthode permet de récupérer le coefficient "c" de l'équation du plan.
   *
   * @return le coefficient "c"
   */
  TCoordinate getC() const;

  /**
   * Cette méthode permet de récupérer le coefficient "d" de l'équation du plan.
   *
   * @return le coefficient "d"
   */
  TCoordinate getD() const;

  /**
   * Cette méthode permet d'inverser l'orientation du plan en multipliant
   * tous les coefficients par -1.
   */
  void reverseOrientation();

  /**
   * Cette méthode permet de tester l'appartenance d'un point au plan.
   *
   * @param APoint Un point
   * @return vrai si le point appartient au plan, faux sinon
   */
  bool isPointOnPlane(const CVertex & APoint) const;

  /**
   * Cette méthode permet de calculer la distance relative entre un point
   * et le plan courant
   *
   * @param APoint Un point
   * @return la distance relative entre APoint et le plan courant
   */
  TCoordinate pointDistance(const CVertex & APoint) const;

  /**
   * Cette méthode permet de calculer l'intersection entre une droite et le
   * plan. Si une intersection a été trouvée, elle donne le paramètre du point
   * d'intersection sur la droite par l'intermédiaire de AInterParam.
   *
   * @param APoint Un point d'un droite
   * @param ADirection Le vecteur directeur de la droite
   * @param AInterParam Le paramètre sur la droite du point d'intersection
   *                    entre la droite et le plan
   * @return vrai s'il y a une intersection, faux sinon
   */
  bool getLineIntersection(const CVertex & APoint,
			   const CVertex & ADirection,
			   TCoordinate * AInterParam) const;

  /** 
   * Cette méthode permet de calculer l'intersection entre une droite et le
   * plan. Si une intersection a été trouvée, elle donne le point
   * d'intersection par l'intermédiaire de AInter.
   *
   * @param APoint Un point d'un droite
   * @param ADirection Le vecteur directeur de la droite
   * @param AInter Le point d'intersection entre la droite et le plan
   * @return vrai s'il y a une intersection, faux sinon
   */
  bool getLineIntersection(const CVertex & APoint,
			   const CVertex & ADirection,
			   CVertex * AInter) const;

  /**
   * Cette méthode permet de donner le plan de projection le mieux adapté pour
   * la projcetion de points appartenant au plan.
   *
   * @return Le plan de projection le mieux adapté
   */
  TProjection getBestProjection() const;

  /**
   * Cette méthode permet de récupérer la projection orthogonale d'un point
   * sur le plan.
   *
   * @param APoint Un point de coordonnées (x, y, z)
   * @return L'image de APoint par projection orthogonale sur le plan
   */
  CVertex projectPoint(const CVertex & APoint) const;

  /**
   * Cette méthode permet de projeter un point du plan sur un plan de projection
   * parallèle aux axes du repère.
   *
   * @param APoint Un point de coordonnées (x, y, z)
   * @param AProj Un plan de projection
   * @return L'image de APoint par projection sur le plan AProj
   */
  CVertex projectPoint(const CVertex & APoint, TProjection AProj) const;

  /**
   * Cette méthode permet de récupérer le point d'origine du point projeté par
   * la méthode projectPoint.
   *
   * @param APoint Un point de coordonnées (x, y)
   * @param AProj Un plan de projection dans lequel se trouve APoint
   * @return Le point du plan dont l'image est APoint par projection sur AProj
   */
  CVertex unprojectPoint(const CVertex & APoint, TProjection AProj) const;

  //@}

  /**
   * Affichage du plan APlane sur AStream.
   */
  friend std::ostream & operator << (std::ostream & AStream, const CPlane & APlane);

private:
  
  /// @name Attributs privés

  //@{

  /**
   * Normale au plan. Correspond aux coefficients A, B, C dans l'équation du
   * plan qui est Ax + By + Cz + D = 0
   */
  CVertex FNormal;

  /// Norme de la normale au plan
  TCoordinate FNormalNorm;

  /// Coefficient D de l'équation du plan qui est Ax + By + Cz + D = 0
  TCoordinate FD;

  //@}
};
//******************************************************************************
#include INCLUDE_INLINE("plane.icc")
//******************************************************************************
#endif
//******************************************************************************
