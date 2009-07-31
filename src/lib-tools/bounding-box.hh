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

#ifndef BOUNDING_BOX_HH
#define BOUNDING_BOX_HH

#include "vertex.hh"

/** Classe définissant une boîte englobante.
 *
 * @author Nicolas Guiard
 */

class CBoundingBox
{
public:

  /** @name Constructeurs
   */

  //@{

  /** Constructeur par défaut
   *
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle boîte englobante vide.
   */
  CBoundingBox();

  /** Constructeur avec initialisation
   *
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle boîte englobante contenant un unique point.
   */
  CBoundingBox(const CVertex & APoint);

  /** Constructeur avec initialisation
   *
   * @return Une nouvelle instance de la classe
   *
   * Construction d'une nouvelle boîte englobante définit par deux points.
   */
  CBoundingBox(const CVertex & APoint1, const CVertex & APoint2);

  //@}

  /** @name Méthodes publiques
   */

  //@{

  /** getEpsilon
   *
   * @return La valeur d'Epsilon
   *
   * Cette méthode permet de récupérer la valeur de l'Epsilon utilisé lors des
   * tests de comparaisons.
   */
  static TCoordinate getEpsilon();

  /** setEpsilon
   *
   * @param AEpsilon La valeur d'Epsilon
   *
   * Cette méthode permet de définir la valeur de l'Epsilon à utiliser lors des
   * comparaisons. Cette valeur est la même pour toutes les boîtes englobantes.
   */
  static void setEpsilon(TCoordinate AEpsilon);

  /** addPoint
   *
   * @param APoint Un point
   *
   * Cette méthode permet d'ajouter un point à la boîte englobante. Si ce point
   * est déjà dans la boîte englobante, celle-ci est inchangée, sinon elle est
   * agrandie.
   */
  void addPoint(const CVertex & APoint);

  /** clear
   *
   * Cette méthode permet de vider la boîte englobante.
   */
  void clear();

  /** isEmpty
   *
   * @return Vrai si la boîte et vide et faux sinon
   *
   * Cette méthode permet de savoir si la boîte est vide ou non.
   */
  bool isEmpty() const;

  /** isInBox
   *
   * @param APoint Un point
   * @return vrai si le point se trouve dans la boîte englobante, faux sinon
   *
   * Cette méthode permet de tester l'appartenance d'un point à une boîte
   * englobante.
   */
  bool isInBox(const CVertex & APoint) const;

  /** isInIntersectionWith
   *
   * @param ABB Une boîte englobante
   * @return vrai si la boîte englobante courante coupe celle passée en
   *         paramètre et faux sinon
   *
   * Cette méthode permet de tester si les domaines de deux boîtes englobantes
   * se chevauchent.
   */
  bool isInIntersectionWith(const CBoundingBox & ABB) const;

  /** getVolume
   *
   * @return Le volume de la boîte englobante
   *
   * Cette permet de calculer le volume de la boîte englobante courante.
   */
  TCoordinate getVolume() const;

  /** getSurface
   *
   * @return La surface de la boîte englobante
   *
   * Cette méthode permet de calculer la surface de la boîte englobante.
   */
  TCoordinate getSurface() const;

  /** getMinBound
   *
   * @return Le point de la boîte englobante ayant les coordonnées les plus
   *         petites
   *
   * Cette méthode permet de connaître le point de la boîte englobante ayant les
   * coordonnées (x, y, z) les plus petites.
   */
  const CVertex & getMinBound() const;

  /** getMaxBound
   *
   * @return Le point de la boîte englobante ayant les coordonnées les plus
   *         grandes
   *
   * Cette méthode permet de connaître le point de la boîte englobante ayant les
   * coordonnées (x, y, z) les plus grandes.
   */
  const CVertex & getMaxBound() const;

  /** getEpsMinBound
   *
   * @return Le point de la boîte englobante ayant les coordonnées les plus
   *         petites
   *
   * Cette méthode permet de connaître le point de la boîte englobante ayant les
   * coordonnées (x, y, z) les plus petites en prenant en compte la valeur de
   * l'incertitude.
   */
  CVertex getEpsMinBound() const;

  /** getMaxBound
   *
   * @return Le point de la boîte englobante ayant les coordonnées les plus
   *         grandes
   *
   * Cette méthode permet de connaître le point de la boîte englobante ayant les
   * coordonnées (x, y, z) les plus grandes en prenant en compte la valeur de
   * l'incertitude.
   */
  CVertex getEpsMaxBound() const;

  /** getCenter
   *
   * @return Le centre de la boîte englobante
   *
   * Cette méthode permet de calculer le centre de la boîte englobante.
   */
  CVertex getCenter() const;

  /** operator *
   *
   * @param AOther Une boîte englobante
   * @return vrai si la boîte englobante courante coupe celle passée en
   *         paramètre et faux sinon
   *
   * Cet opérateur permet de tester si les domaines de deux boîtes englobantes
   * se chevauchent. Cet opérateur fait juste appel à la fonction
   * isInIntersectionWith.
   */
  bool operator * (const CBoundingBox & AOther) const;

  /** operator +
   *
   * @param AOther Une boîte englobante
   * @return La boîte englobant la boîte courante et celle passée en paramètre
   *
   * Cet opérateur permet de calculer la boîte englobante de deux boîtes 
   * englobantes.
   */
  CBoundingBox operator + (const CBoundingBox & AOther) const;

  //@}

private:

  /** @name Attributs privés
   */

  //@{

  /** FMin, FMax
   * Les bornes inférieures et supérieures de la boîte englobante.
   */
  CVertex FMin, FMax;


  /** FIsEmpty
   * Indique si la boîte est vide ou non.
   */
  bool FIsEmpty;

  //@}
};

/** operator <<
 *
 * @param AStream Un flux dans lequel écrire
 * @param ABB Une boîte englobante
 * @return Le flux AStream modifié
 *
 * Cet opérateur permet d'afficher les coordonnées de la boîte englobante.
 */
std::ostream & operator << (std::ostream & AStream, const CBoundingBox & ABB);

#endif
