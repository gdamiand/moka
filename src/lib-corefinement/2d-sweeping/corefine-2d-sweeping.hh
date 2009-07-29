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
#ifndef COREFINE_SEGMENTS_SWEEPING_HH
#define COREFINE_SEGMENTS_SWEEPING_HH
//******************************************************************************
#include "sweeping-comparators.hh"
#include "vertex.hh"
#include <set>
//******************************************************************************
#define LEX_SET  std::multiset<CDartVertex*, CDartLexicoCompare>
#define VERT_SET std::multiset<CDartVertex*, CDartVertexerticalCompare>
#define ANG_SET  std::multiset<CDartVertex*, CDartAngularCompare>
#define LEX_IT   LEX_SET::iterator
#define VERT_IT  VERT_SET::iterator
#define ANG_IT   ANG_SET::iterator
//******************************************************************************
namespace GMap3d
{
  class  CDart;
  class  CDartVertex;
  class  CGMapVertex;

  class  CCorefineSegmentsSweeping
  {
  public:
    //@{

    /// Constructeur
    CCorefineSegmentsSweeping(CGMapVertex* AMap);

    /// Destructeur
    virtual ~CCorefineSegmentsSweeping();

    /**
     * Coraffine par balayage les maillages 2d incidents à ADart1 et ADart2.
     *
     * @param ADart1 Un brin du maillage bleu
     * @param ADart2 Un brin du maillage rouge
     * @param ANormalVector Un vecteur normal au plan des maillages
     */
    void corefine(CDart* ADart1, CDart* ADart2, const CVertex & ANormalVector);

  protected:
    /**
     * Permet de retrouver l'élément AElement dans l'ensemble de balayage.
     * Si l'ensemble n'est pas ordonné (à cause des erreurs numériques), la
     * méthode scrute l'ensemble tout entier pour retrouver AElement.
     *
     * @param ASweepingSet L'ensemble de balayage
     * @param AElement Un brin de l'ensemble de balayage
     * @return Un itérateur pointant sur AElement.
     *
     * @precondition AElement est dans ASweepingSet
     */
    VERT_IT findElementInSweepingSet(VERT_SET & ASweepingSet, CDart* AElement);

    /**
     * Gère l'intersection entre 2 arêtes lors du co-raffinement 2D par balayage.
     * Si les deux arêtes se coupent, le point d'intersection est inséré mais les
     * deux maillages ne sont pas "joints".
     * Les nouveaux évènements (extrémités entrantes et sortantes créées) sont
     * insérés dans l'ensemble AEventSet.
     * Les marques AExtremity1 et AMesh1 sont positionnées correctement sur les
     * brins créés. Les champs directInfo[ADirectVertex] sont eux-aussi
     * initialisés.
     * Les coordonnées des points d'intersection sont déterminées par la méthode
     * 'CGeometry::getSegmentsIntersection'.
     *
     * @param ADart1 Un brin entrant de la première arête
     * @param ADart2 Un brin entrant de la deuxième arête
     * @param AEventSet L'ensemble des extrémités non encore traitées
     * @param ALexComparator Un comparateur de brins pour le tri lexicographique
     * @param ASweepingSet L'ensemble de balayage
     * @param AMesh1 Un numéro de marque
     * @param AExtremity1 Un numéro de marque
     * @param ADirectVertex Un indice de champ directInfo
     * @param ANormalVector Un vecteur normal au plan de co-raffinement
     */
    void manageEdgesIntersection(CDart* ADart1, CDart* ADart2,
				 LEX_SET & AEventSet,
				 const CDartLexicoCompare & ALexComparator,
				 VERT_SET & ASweepingSet,
				 int AExtremity1,
				 int AMesh1,
				 int ADirectVertex,
				 const CVertex & ANormalVector);

  private:
    CGMapVertex* FMap;
  };

} // namespace GMap3d
//******************************************************************************
#endif // COREFINE_SEGMENTS_SWEEPING_HH
//******************************************************************************
