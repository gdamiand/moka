/*
 * lib-corefinement : Opérations de corafinement.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-corefinement
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
#ifndef SWEEPING_COMPARATORS_HH
#define SWEEPING_COMPARATORS_HH
//******************************************************************************
#include "math-extension.hh"
#include "vertex.hh"

//******************************************************************************
namespace GMap3d
{
  class  CDartVertex;
  class  CGMapVertex;

  /**
   * Les classes CDart---Compare servent à trier des extrémités d'arêtes.
   * Ce tri est effectué pendant l'opération de coraffinement 2d.
   *
   * @author MOKA TEAM
   */
  // @{

  class  CDartCompare
  {
  public:
    // @name Constructeur
    // @{

    /**
     * Construction d'une nouvelle instance de la classe.
     * Les plongements des brins doivent être accessibles via le champ
     * directInfo dont l'indice est donné (voir méthode
     * 'CGMapVertex::pointDirectInfoToAttributeVertex').
     * Le paramètre AExtremity1 correspond à un numéro de marque.
     * Pour plus d'informations, se reporter à la méthode 'operator()'.
     *
     * @param AMap La carte sur laquelle s'applique le comparateur
     * @param ADirectVertex Un indice de champ directInfo
     * @param AExtremity1 Un numéro de marque
     * @return Une nouvelle instance de la classe
     */
    CDartCompare(CGMapVertex* AMap,
		 int ADirectVertex, int AExtremity1, const CVertex& AZVector);

    // @}
    // @name Méthode de projection
    // @{

    /**
     * Transforme les coordonnées 3d du sommet AVertex en coordonnées 2d, qui
     * sont placées dans les paramètres AX et AY.
     * La méthode projette simplement le sommet sur l'un des plans OXY, OXZ ou
     * OYZ. Ce plan est déterminé à partir du vecteur normal AZVector passé en
     * paramètre au constructeur de l'instance.
     *
     * @param AVertex Un sommet quelconque
     * @param AX L'abscisse du sommet AVertex
     * @param AY L'odonnée du sommet AVertex
     */

    void project(const CVertex& AVertex,
		 TCoordinate& AX, TCoordinate& AY) const;

    // @}

  protected:
    // @name Champs protégés
    // @{

    CGMapVertex* FMap;

    int FDirectVertex;
    int FExtremity1;

    int FCoord1, FCoord2;

    // @}
  };
  //----------------------------------------------------------------------------
  class  CDartLexicoCompare : public CDartCompare
  {
  public:
    // @name Constructeur
    // @{

    /**
     * Construction d'une nouvelle instance de la classe.
     * Les plongements des brins doivent être accessibles via le champ
     * directInfo dont l'indice est donné (voir méthode
     * 'CGMapVertex::pointDirectInfoToAttributeVertex').
     * Le paramètre AExtremity1 correspond à un numéro de marque.
     * Pour plus d'informations, se reporter à la méthode 'operator()'.
     *
     * @param AMap La carte sur laquelle s'applique le comparateur
     * @param ADirectVertex Un indice de champ directInfo
     * @param AExtremity1 Un numéro de marque
     * @param AExactComparison Un booléen
     * @return Une nouvelle instance de la classe
     */
    CDartLexicoCompare(CGMapVertex* AMap,
		       int ADirectVertex, int AExtremity1,
		       const CVertex& AZVector, bool AExactComparison);

    // @}
    // @name Opérateur de comparaison
    // @{

    /**
     * Si FExactComparison est vrai, la comparaison faite entre les coordonnées
     * des sommets incidents aux brins sont faites en utilisant l'opérateur ==.
     * Si FExactComparison est faux, la comparaison faite entre les coordonnées
     * des sommets incidents aux brins sont faites en utilisant la fonction
     * areEqual.
     *
     * @param ADart1 Un brin
     * @param ADart2 Un brin
     * @return Un booléen vrai si ADart1 < ADart2
     */
    bool operator()(CDartVertex* ADart1, CDartVertex* ADart2) const;

    // @}

    // @name Champs Privés
    // @{

    bool FExactComparison;

    // @}
    };

  class  CDartVertexerticalCompare : public CDartCompare
  {
  public:
    // @name Constructeur
    // @{

    /**
     * Construction d'une nouvelle instance de la classe.
     * Les plongements des brins doivent être accessibles via le champ
     * directInfo dont l'indice est donné (voir méthode
     * 'CGMapVertex::pointDirectInfoToAttributeVertex').
     * Le paramètre AExtremity1 correspond à un numéro de marque.
     * Pour plus d'informations, se reporter à la méthode 'operator()'.
     *
     * @param AMap La carte sur laquelle s'applique le comparateur
     * @param ADirectVertex Un indice de champ directInfo
     * @param AExtremity1 Un numéro de marque
     * @return Une nouvelle instance de la classe
     */

    CDartVertexerticalCompare(CGMapVertex* AMap,
			      int ADirectVertex, int AExtremity1,
			      const CVertex& AZVector);

    // @}
    // @name Opérateur de comparaison
    // @{

    /**
     * @param ADart1 Un brin
     * @param ADart2 Un brin
     * @return Un booléen vrai si ADart1 < ADart2
     */
    bool operator()(CDartVertex* ADart1, CDartVertex* ADart2) const;

    /**
     * Positionne la ligne de balayage de manière à ce qu'elle passe par
     * AVertex.
     *
     * @param AVertex Un sommet
     */
    void setCurrentPoint(const CVertex& AVertex);

    // @}

  private:
    // @name Champs privés
    // @{

    /// Abscisse de balayage.
    static TCoordinate FX;

    // @}
  };
  //****************************************************************************
  class  CDartAngularCompare : public CDartCompare
  {
  public:
    // @name Constructeur
    // @{

    /**
     * Construction d'une nouvelle instance de la classe.
     * Les plongements des brins doivent être accessibles via le champ
     * directInfo dont l'indice est donné (voir méthode
     * 'CGMapVertex::pointDirectInfoToAttributeVertex').
     * Pour plus d'informations, se reporter à la méthode 'operator()'.
     *
     * @param AMap La carte sur laquelle s'applique le comparateur
     * @param ADirectVertex Un indice de champ directInfo
     * @return Une nouvelle instance de la classe
     */
    CDartAngularCompare(CGMapVertex* AMap, int ADirectVertex,
			const CVertex& AZVector);

    // @}
    // @name Opérateur de comparaison
    // @{

    /**
     * @param ADart1 Un brin
     * @param ADart2 Un brin
     * @return Un booléen vrai si ADart1 < ADart2
     */
    bool operator()(CDartVertex* ADart1, CDartVertex* ADart2) const;

    // @}
    // @name Champs privés
    // @{

    CVertex FZVector;

    // @}
    };
  //****************************************************************************
  class  CIntersection
  {
  public:
    // @name Attributs publics
    // @{

    /**
     * Position du point d'intersection sur la droite courante
     * (pas la droite d'indice lineId).
     */
    TCoordinate FLambda;

    /// Indice de la droite à laquelle correspond le point d'intersection.
    int FIdentity;

    // @}
    // @name Constructeur
    // @{

    /**
     * Constructeur avec initialisation
     *
     * Crée une nouvelle instance initialisée de la classe.
     *
     * @param ALambda Une valeur réelle quelconque
     * @param AIdentity Un numéro de droite
     * @return Une nouvelle instance de la classe
     */
    CIntersection(const TCoordinate& ALambda, int AIdentity);

    // @}
    };
  //****************************************************************************
  class  CDartLinearCompare
  {
  public:
    // @name Constructeur
    // @{

    /**
     * Construction d'une nouvelle instance de la classe.
     * Pour plus d'informations, se reporter à la méthode 'operator()'.
     */
    CDartLinearCompare();

    // @}
    // @name Opérateur de comparaison
    // @{

    /**
     * @param AIntersection1 Un couple (lambda , id)
     * @param AIntersection2 Un couple (lambda , id)
     * @return Un booléen vrai si AIntersection1 < AIntersection2
     */
    bool operator()(CIntersection* AIntersection1,
		    CIntersection* AIntersection2) const;

    // @}
    };

  // @}

} // namespace GMap3d
//******************************************************************************
#endif // SWEEPING_COMPARATORS_HH
//******************************************************************************
