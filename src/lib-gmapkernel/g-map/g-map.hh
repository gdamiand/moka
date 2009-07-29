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
#ifndef G_MAP_HH
#define G_MAP_HH
//******************************************************************************
#include "g-map-basic.hh"
#include "inline-macro.hh"
// Includes facultatifs :
#include "dart.hh"
#include "attribute.hh"
#include "embedding.hh"
#include "coverage.hh"
#include "dynamic-coverage.hh"
#include "static-coverage.hh"
//******************************************************************************
namespace GMap3d
{
  /**
   * Cette classe fournit tous les outils de base pour manipuler une G-carte,
   * et notamment :
   *  - les coutures et décousures (topologiques et géométrique),
   *  - les méthodes pour gérer les attributs (ajouter, enlever...),
   *  - des méthodes utilisées en interne pour gérer les attributs.
   *
   *  @author MOKA TEAM
   */
  class CGMap : public CGMapBasic
  {
  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CGMap();
  
    /// Destructeur
    virtual ~CGMap();

    // @}
  
    // @name Méthodes "sûres" pour la couture et décousure.
  
    /**
     * @param ADart1 et ADart2 Deux brins
     * @return vrai si la [i]-couture de ces deux brins est possible,
     *   faux sinon.
     */
    // @{
    bool canSew0(CDart* ADart1, CDart* ADart2);
    bool canSew1(CDart* ADart1, CDart* ADart2);
    bool canSew2(CDart* ADart1, CDart* ADart2);
    bool canSew3(CDart* ADart1, CDart* ADart2);
    // @}

    /**
     * @param ADart1 et ADart2 Deux brins
     * @param ADimension Une dimension
     * @return si la [ADimension]-couture de ces deux brins est possible.
     */  
    bool canSew(CDart* ADart1, CDart* ADart2, int ADimension);

    /**
     * [i]-couture géométrique des 2 brins. Les attributs sont mis à jour
     * lorsque c'est nécessaire (quand deux orbites sont fusionnées et que
     * chacune avait un attribut). De plus, la couture est effectuée sur
     * l'orbite concernée afin de garantir la validité de la G-carte obtenue.
     *
     * @param ADart1 et ADart2 Deux brins
     * @precondition canSew[i](ADart1, ADart2)
     */
    // @{
    void sew0(CDart* ADart1, CDart* ADart2);
    void sew1(CDart* ADart1, CDart* ADart2);
    void sew2(CDart* ADart1, CDart* ADart2);
    void sew3(CDart* ADart1, CDart* ADart2);
    // @}
  
    /**
     * Couture géométrique des deux brins pour la dimension donnée (cf. sew[i])
     *
     * @param ADart1 et ADart2 Deux brins
     * @param ADimension Une dimension
     * @precondition canSew(ADart1, ADart2, ADimension)
     */
    void sew(CDart* ADart1, CDart* ADart2, int ADimension);

    /**
     * [i]-décousure géométrique. Les attributs sont mis à jour lorsque c'est
     * nécessaire (quand on coupe une orbites en deux). De plus, la décousure
     * est effectuée sur l'orbite concernée afin de garantir la validité
     * de la G-carte obtenue.
     *
     * @param ADart Un brin
     */
    // @{
    void unsew0(CDart* ADart);
    void unsew1(CDart* ADart);
    void unsew2(CDart* ADart);
    void unsew3(CDart* ADart);
    // @}

    /**
     * Décousure géométrique du brin pour la dimension donnée. (cf. unsew[i])
     *
     * @param ADart Un brin
     * @param ADimension Une dimension
     */
    void unsew(CDart* ADart,int ADimension);

    /**
     * @name Méthodes pour récupérer des parcours étant donné un brin et une
     *   orbite. Voir les différents types de parcours dans
     *   dynamic-coverage.hh et static-coverage.hh.
     */
    // @{

    CCoverage* getDynamicCoverage     (const CDart* ADart, TOrbit AOrbit);
    CCoverage* getStaticCoverage      (const CDart* ADart, TOrbit AOrbit);
    CCoverage* getMultiDynamicCoverage(const CDart* ADart, TOrbit AOrbit);
    CCoverage* getBasicDynamicCoverage(const CDart* ADart, TOrbit AOrbit,
				       int AMarkNumber);

    // @}
    // @name Méthodes de gestion des attributs.
    // @{

    /**
     * @return Un pointeur vers l'attribut de type donné appartenant à l'orbite
     *   donné du brin ADart. NULL si un tel attribut n'existe pas.
     */
    CAttribute* getAttribute(const CDart* ADart, TOrbit, TAttributeId);

    /**
     * Ajoute l'attribut donné à l'orbite donné du brin ADart.
     * ATTENTION : cette méthode ne doit pas être appelé si un tel attribut
     * existe déjà. Cette précondition doit être assurée par l'appelant.
     * À UTILISER AVEC ATTENTION.
     */  
    void addAttribute(CDart* ADart, TOrbit, CAttribute*);

    /**
     * @return Un pointeur vers l'attribut de type donné appartenant à l'orbite
     *   donné du brin ADart et enlève cet attribut de cette orbite.
     *   Retourne NULL si un tel attribut n'existe pas.
     */  
    CAttribute* removeAttribute(CDart* ADart, TOrbit, TAttributeId);

    /**
     * Détruit l'attribut de type donné appartenant à l'orbite donné du brin
     * ADart et enlève cet attribut de cette orbite. 
     * Rien ne se passe si un tel attribut n'existe pas.
     */  
    void deleteAttribute(CDart* ADart, TOrbit, TAttributeId);
  
    /**
     * @param ADart1 et ADart2 Deux brins
     * @param AOrbit Une orbite 
     *
     * @return vrai si ADart1 et ADart2 appartiennent à la même orbite <AOrbit>
     *   faux sinon
     */
    bool isSameOrbit(CDart* ADart1, CDart* ADart2, TOrbit AOrbit);
  
    /**
     * @param ADart  Un brin
     * @param AOrbit Une orbite
     *
     * @return un pointeur vers le brin appartenant à l'orbite <AOrbit> de
     *   ADart contenant l'embedding pour cette orbite. 
     *   Retourne NULL si un tel brin n'existe pas (pas d'embedding).
     */
    CDart* getEmbeddingOwner(const CDart* ADart, TOrbit AOrbit);

    // @}
    /**
     * @name Méthodes "non-sûres" pour la coutures. Le résultat de ces
     *   méthodes entraîne l'obtention d'un objet éventuellement non valide
     *   (une 3-G-carte ne vérifiant pas les contraintes sur les alpha, ou
     *   plusieurs attributs sur une même orbite...). La responsabilité de la
     *   validité est laissé à l'appelant. Ces méthodes servent malgré tout pour
     *   optimiser des traitements particulier. A UTILISER AVEC ATTENTION.
     */

    /**
     * [i]-couture géométrique des 2 brins uniquement (et pas de toute l'orbite
     * contrairement à sew[i]). Les attributs sont mis à jour lorsque c'est
     * nécessaire (quand deux orbites sont fusionnées et que
     * chacune avait un attribut). 
     *
     * @param ADart1 et ADart2 Deux brins
     */
    // @{
    void dartSew0(CDart*, CDart* ADart2);
    void dartSew1(CDart*, CDart* ADart2);
    void dartSew2(CDart*, CDart* ADart2);
    void dartSew3(CDart*, CDart* ADart2);
    void dartSew (CDart*, CDart* ADart2, int ADimension);
    // @}

    /**
     * [i]-décousure géométrique uniquement du brin (et pas de toute l'orbite
     * contrairement à unsew[i]). Les attributs sont mis à jour lorsque c'est
     * nécessaire (quand on coupe une orbites en deux). 
     *
     * @param ADart Un brin
     */
    // @{
    void dartUnsew0(CDart* ADart);
    void dartUnsew1(CDart* ADart);
    void dartUnsew2(CDart* ADart);
    void dartUnsew3(CDart* ADart);
    void dartUnsew (CDart* ADart, int ADimension);
    // @}

    /**
     * [i]-couture topologique des 2 brins (les attributs ne sont pas mis à
     * jour contrairement à sew[i]). La couture est effectuée sur l'orbite
     * concernée afin de garantir la validité de la G-carte obtenue.
     *
     * @param ADart1 et ADart2 Deux brins
     * @precondition canSew[i](ADart1, ADart2)
     */
    // @{
    void topoSew0(CDart* ADart1, CDart* ADart2);
    void topoSew1(CDart* ADart1, CDart* ADart2);
    void topoSew2(CDart* ADart1, CDart* ADart2);
    void topoSew3(CDart* ADart1, CDart* ADart2);
    void topoSew (CDart* ADart1, CDart* ADart2, int ADimension);
    // @}

    /**
     * [i]-décousure topologique (les attributs ne sont pas mis à jour
     * contrairement à unsew[i]). La décousure est effectuée sur l'orbite
     * concernée afin de garantir la validité de la G-carte obtenue.
     *
     * @param ADart Un brin
     */
    // @{
    void topoUnsew0(CDart* ADart);
    void topoUnsew1(CDart* ADart);
    void topoUnsew2(CDart* ADart);
    void topoUnsew3(CDart* ADart);
    void topoUnsew (CDart* ADart, int ADimension);
    // @}

  private:
    // @name Méthodes utilisées par sew et unsew pour gérer les attributs.
    // @{
  
    /**
     * Teste si l'orbite <AOrbit>(ADart) est localement valide, c'est-à-dire
     * si les compositions d'involution contenues dans AOrbit sont vérifiées
     * pour le brin ADart (attention, la vérification n'est pas faite pour
     * toute l'orbite).
     *
     * @param ADart un brin
     * @param AOrbit une orbite
     * @return un booléen indiquant si l'orbite est localement valide 
     */
    bool isValidOrbit(CDart* ADart, TOrbit AOrbit);

    /**
     * Met à jour tout les attributs des deux brins lorsque c'est nécessaire.
     * Cette méthode est appelée avant une opération de couture. Elle teste
     * pour chaque orbite qui vont être regroupé par la couture, s'il existe
     * un embedding à la fois pour l'orbite contenant ADart1 et celle contenant
     * ADart2. Si c'est le cas, l'embedding associé à ADart1 est supprimée, et
     * l'embedding de ADart2 récupère les éventuels attributs qu'il n'avait
     * pas déjà. Les autres attributs sont détruits.
     *
     * @param ADart1 et ADart2 Deux brins
     * @param AMask un masque de 16 booléens.
     */
    void groupAttributes(CDart* ADart1, CDart* ADart2, TMask16);

    /**
     * Met à jour tout les attributs des deux brins lorsque c'est nécessaire.
     * Cette méthode est appelée après une opération de décousure. Elle teste
     * pour chaque orbite qui ont été concernés par la décousure, s'il existe
     * un embedding d'un coté et pas de l'autre. Si c'est le cas, l'embedding
     * existant est dupliqué (méthode copy de la classe CEmbedding) et ajouté
     * du coté qui n'en avait pas.
     *
     * @param ADart1 et ADart2 Deux brins
     * @param AMask un masque de 16 booléens.
     */
    void duplicateAttributes(CDart* ADart1, CDart* ADart2, TMask16);
  
    /**
     * Optimisation pour les méthodes {duplicate/group}Attributes
     *
     * Méthode équivalente à isSameOrbit.
     * Le paramètre ADartEmbeddingOwner1 contient le propriétaire de l'embedding
     * si celui-ci est trouvé lors du parcours, sinon NULL.
     * Méthode équivalente à  :
     *      if (isSameOrbit(ADart1,ADart2))
     *             ADartEmbeddingOwner1=getEmbeddingOwner(ADart1,AOrbit);
     * mais en un seul parcours de l'orbite.
     */
    bool isSameOrbit(CDart* ADart1, CDart* ADart2, TOrbit AOrbit,
		     CDart*& ADartEmbeddingOwner1);
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("g-map.icc")
//******************************************************************************
#endif // G_MAP_HH
//******************************************************************************
