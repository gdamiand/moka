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
#ifndef G_MAP_BASIC_HH
#define G_MAP_BASIC_HH
//******************************************************************************
#include "kernel-types.hh"
#include "inline-macro.hh"

#include <bitset>
//******************************************************************************
namespace GMap3d
{
  class CDart;

  /**
   * Cette classe regroupe uniquement des méthodes de base :
   *  - pour la gestion des marques booléennes,
   *  - pour la gestion des champs direct-info,
   *  - pour lier/délier deux brins entre eux, sans vérification
   *    des contraintes d'intégrité.
   *
   * Les méthodes permettant d'explorer la carte et de gérer les plongements
   * sont définies dans la classe CGMap.
   *
   * @author MOKA TEAM
   */
  class CGMapBasic
  {
  public:
    // @name Constructeur et destructeur
    // @{

    /// Constructeur
    CGMapBasic();

    /// Destructeur
    virtual ~CGMapBasic();

    // @}
    // @name Réinitialisation de la carte
    // @{

    /**
     * Secoue la liste des brins pour avoir un ordre aléatoire.
     * n'a pas d'effet sur le contenue de la carte mais juste sur l'ordre
     * dans lequel seront parcouru les brins.
     */
    void randomizeDarts();

    /**
     * Vide la carte.
     * Tous les brins sont détruits.
     */
    void removeAllDarts();

    /**
     * Réinitialise la carte.
     * La méthode removeAllDarts est appelée, puis toutes les marques, champs
     * direct-info, etc, sont réinitialisés.
     */
    virtual void empty();

    // @}
    // @name Gestion des champs direct-info
    // @{

    /**
     * Réserve un champ direct info libre.
     *
     * @return L'indice du champ direct-info réservé.
     */
    int getNewDirectInfo();

    /**
     * Libère un champ direct-info préalablement réservé avec la
     * méthode getNewDirectInfo.
     *
     * @param ADirectIndex Un indice de champ direct-info.
     */
    void freeDirectInfo(int ADirectIndex);

    /**
     * Retourne la valeur du champ direct-info[ADirectIndex] du brin ADart.
     *
     * @param ADart Un brin de la carte
     * @param ADirectIndex Un indice de champ direct-info
     * @return Un pointeur
     */
    void* getDirectInfo(CDart* ADart, int ADirectIndex) const;

    /**
     * Affecte la valeur APointer au champ direct-info[ADirectIndex] du brin
     * ADart.
     *
     * @param ADart Un brin de la carte
     * @param ADirectIndex Un indice de champ direct-info
     * @param APointer La valeur à affecter
     */
    void setDirectInfo(CDart* ADart, int ADirectIndex, void* APointer);

    // @}
    // @name Accès au premier brin de liste des brins de la carte.
    // @{

    /**
     * Retourne le brin situé en tête de la liste des brins de la carte.
     *
     *  @return Le premier brin de la carte
     */
    CDart* getFirstDart() const;

    /**
     * Définit le premier brin de la carte.
     *
     * @param ADart Un brin de la carte.
     */
    void setFirstDart(CDart* ADart);

    // @}
    // @name Gestion des marques
    // @{

    /**
     * Réserve une marque inutilisée de la carte.
     *
     * @return Un numéro de marque
     */
    int getNewMark();

    /**
     * Retourne l'état de la marque.
     * Initialement, cette valeur vaut false. Elle est modifiée à chaque appel
     * à la méthode negateMaskMark().
     *
     * @param AMarkNumber Un numéro de marque
     * @return Un booléen indiquant l'état de la marque
     */
    bool getMaskMark(int AMarkNumber) const;

    /**
     * Libère la marque AMarkNumber préalablement réservée avec la méthode
     * getNewMark().
     *
     * @param AMarkNumber
     */
    void freeMark(int AMarkNumber);

    /**
     * Inverse l'état d'une marque. Cette méthode permet de changer l'état de
     * la marque AMarkNumber de tous les brins de la carte en O(1).
     * C'est utile par exemple pour démarquer tous les brins à la fin d'un
     * traitement.
     *
     * @param AMarkNumber Un numéro de marque
     */
    void negateMaskMark(int AMarkNumber);

    /**
     * Cette méthode est utilisée (en assertion) par la méthode freeMark.
     * En effet, lorqu'on libère une marque, il faut nécessairement que tous
     * les brins de la carte soient démarqués.
     *
     * @param AMarkNumber Un numéro de marque
     * @return Un booléen indiquant si tous les brins de la carte sont démarqués
     */
    bool isWholeMapUnmarked(int AMarkNumber);

    /**
     * Cette méthode permet de savoir à tout moment combien de marques sont
     * utilisées simultanément.
     * Sert au debugging.
     *
     * @return Le nombre de marques utilisées
     */
    int getNbUsedMarks() const;

    /**
     * Teste si la marque AMarkNumber du brin ADart est activée.
     * Cette méthode est redirigée sur la méthode équivalente de CDart.
     *
     * @param ADart Un brin de la carte
     * @param AMarkNumber Un numéro de marque
     * @return Un booléen indiquant l'état de la marque AMarkNumber du brin
     *   ADart.
     */
    bool isMarked(const CDart* ADart, int AMarkNumber) const;

    /**
     * Permet de modifier l'état de la marque AMarkNumber du brin ADart.
     * Cette méthode est redirigée sur la méthode équivalente de CDart.
     *
     * @param ADart Un brin de la carte
     * @param AMarkNumber Un numéro de marque
     * @param AState Un booléen indiquant s'il faut activer ou désactiver la
     *   marque
     */
    void setMarkTo(CDart* ADart, int AMarkNumber, bool AState);

    /**
     * Permet d'activer la marque AMarkNumber du brin ADart.
     * Cette méthode équivaut à setMarkTo(ADart, AMarkNumber, true).
     *
     * @param ADart Un brin de la carte
     * @param AMarkNumber Un numéro de marque
     */
    void setMark(CDart* ADart, int AMarkNumber);

    /**
     * Permet de désactiver la marque AMarkNumber du brin ADart.
     * Cette méthode équivaut à setMarkTo(ADart, AMarkNumber, false).
     *
     * @param ADart Un brin de la carte
     * @param AMarkNumber Un numéro de marque
     */
    void unsetMark(CDart* ADart, int AMarkNumber);

    /**
     * Cette méthode permet d'initialiser toutes les marques du brin ADart.
     * AMarks est modifié pour prendre en comte le masque des marques.
     *
     * @param ADart Un brin de la carte
     * @param AMarks un tableau indiquant l'état de toutes les marques
     */
    void setMarks(CDart* ADart, const std::bitset<NB_MARKS> & AMarks) const;

    /**
     * Retourne l'état des marques du brin, en tenant compte du masque des
     * marques.
     *
     * @param ADart Un brin de la carte
     * @return L'état des marques du brin
     */
    std::bitset<NB_MARKS> getMarks(CDart* ADart) const;

    // @}
    // @name Gestion des orbites utilisées pour les plongements
    // @{

    /**
     * Permet de savoir si une orbite est utilisée.
     * Cette méthode est utilisée par la classe CGMap pour la gestion des
     * plongements.
     *
     * @param AOrbit Une des 16 orbites classiques (pas les orbites de bord)
     * @return Un booléen indiquant si l'orbite est utilisée
     */
    bool isOrbitUsed(TOrbit AOrbit) const;

    /**
     * Permet d'activer l'utilisation d'une orbite.
     * Lorsqu'une orbite est activée, les plongements qui lui correspondent
     * sont mis à jour lors des opérations de couture et de décousure (celles
     * avec gestion des plongements  de la classe CGMap).
     *
     * @param AOrbit Une orbite classique (pas une orbite de bord)
     */
    void setOrbitUsed(TOrbit AOrbit);

    /**
     * Permet de désactiver l'utilistion d'une orbite.
     * Lorsqu'une orbite est désactivée, auncun plongement qui lui correspond
     * n'est mis à jour par les opérations de couture et de décousure de la
     * classe CGMap.
     * Initialement, toutes les orbites sont désactivées.
     *
     * @param AOrbit Une orbite classique (pas une orbite de bord)
     */
    void unsetOrbitUsed(TOrbit AOrbit);

    /**
     * Permet de savoir si un brin contient un plongement sans avoir à
     * parcourir la liste de ses attributs.
     *
     * @param ADart Un brin de la carte
     * @param AOrbit Une des 16 orbites classiques
     * @return Un booléen indiquant si l'orbite AOrbit du brin ADart contient
     *   un plongement
     */
    bool isOrbitUsed(CDart* ADart, TOrbit AOrbit) const;

    // @}
    // @name Création et suppression de brins
    // @{

    /**
     * Permet de créer un nouveau brin.
     * Cette méthode doit être surchargée dans les classes qui dérivent de
     * CGMapBasic (car un constructeur ne peut être virtuel).
     * N'utilisez pas cette méthode mais la méthode addMapDart() pour insérer
     * un nouveau brin dans la carte.
     *
     * @return Un nouveau brin de la carte
     */
    virtual CDart* newDart();

    /**
     * Permet de détruire un brin.
     * Cette méthode doit être surchargée dans les classes qui dérivent de
     * CGMapBasic.
     * N'utilisez pas cette méthode mais la méthode delMapDart() pour supprimer
     * un brin de la carte.
     *
     * @param ADart Un brin de la carte
     */
    virtual void delDart(CDart* ADart);

    /**
     * Permet de créer un nouveau brin (avec la méthode addDart()) et de
     * l'insérer dans la carte.
     *
     * @return Le brin inséré
     */
    CDart* addMapDart();

    /**
     * Ote un brin de la liste des brins de la carte, puis le supprime avec la
     * méthode delDart().
     *
     * @param ADart Le brin à supprimer
     */
    void delMapDart(CDart* ADart);

    // @}

    /**
     * @name Accès aux involutions
     *
     * Attention, les méthodes d'accès en écritures qui suivent ne sont pas
     * sûres car elles ne préservent pas forcément les contraintes des
     * cartes. Elles ne mettent pas non plus à jour les éventuels plongements.
     */

    /**
     * Teste si le brin ADart est libre par alpha-i, c'est-à-dire s'il est sa
     * propre image par alpha-i.
     *
     * @param ADart Un brin de la carte
     */
    // @{
    bool isFree0(const CDart* ADart) const;
    bool isFree1(const CDart* ADart) const;
    bool isFree2(const CDart* ADart) const;
    bool isFree3(const CDart* ADart) const;
    // @}

    /**
     * Teste si le brin ADart est libre en dimension ADimension, c'est-à-dire
     * s'il est sa propre image par alpha-ADimension.
     *
     * @param ADart Un brin de la carte
     * @param ADimension Une dimension comprise entre 0 et 3 inclus
     */
    bool isFree(const CDart* ADart, int ADimension) const;

    /**
     * @param ADart Un brin de la carte
     * @return L'image par alpha-i de ADart
     */
    // @{
    CDart* alpha0(const CDart* ADart) const;
    CDart* alpha1(const CDart* ADart) const;
    CDart* alpha2(const CDart* ADart) const;
    CDart* alpha3(const CDart* ADart) const;
    // @}

    /**
     * @param ADart Un brin de la carte
     * @param ADimension Une dimension comprise entre 0 et 3 inclus
     * @return l'image par alpha-ADimension de ADart.
     */
    CDart* alpha(const CDart* ADart, int ADimension) const;

    /**
     * Lie par alpha-[i] les brins ADart1 et ADart2.
     *
     * @param ADart1 Un brin de la carte
     * @param ADart2 Un brin de la carte
     */
    // @{
    void linkAlpha0(CDart* ADart1, CDart* ADart2);
    void linkAlpha1(CDart* ADart1, CDart* ADart2);
    void linkAlpha2(CDart* ADart1, CDart* ADart2);
    void linkAlpha3(CDart* ADart1, CDart* ADart2);
    // @}

    /**
     * Lie par alpha-ADimension les brins ADart1 et ADart2.
     *
     * @param ADart1 Un brin de la carte
     * @param ADart2 Un brin de la carte
     * @param ADimension Une dimension entre 0 et 3 inclus.
     */
    void linkAlpha(CDart* ADart1, CDart* ADart2, int ADimension);

    /**
     * Délie par alpha-[i] les brins ADart et alpha-[i](ADart).
     *
     * @param ADart Un brin de la carte
     */
    // @{
    void unlinkAlpha0(CDart* ADart);
    void unlinkAlpha1(CDart* ADart);
    void unlinkAlpha2(CDart* ADart);
    void unlinkAlpha3(CDart* ADart);
    // @}

    /**
     * Délie par alpha-ADimension les brins ADart et alpha-ADimension(ADart2).
     *
     * @param ADart1 Un brin de la carte
     * @param ADart2 Un brin de la carte
     * @param ADimension Une dimension entre 0 et 3 inclus.
     */
    void unlinkAlpha(CDart* ADart, int ADimension);

  protected:
    // @name Champs protégés :
    // @{

    /// Premier brin de la liste des brins de la carte
    CDart* FFirstDart;

    /// Tableau indiquant quelles sont les marques prises
    std::bitset<NB_MARKS> FUsedMarks;

    /// Masque des marques (modifié par la méthode negateMaskMark())
    std::bitset<NB_MARKS> FMaskMarks;

    /// Liste des marques libres
    int FMarks[NB_MARKS];

    /// Tableau indiquant quelles sont les orbites utilisées
    std::bitset<16> FUsedOrbits;

    /// Tableau indiquant quels sont les champs direct-info utilisés
    std::bitset<NB_DIRECT_INFO> FUsedDirectInfo;

    /// Liste des champs directInfo libres
    int FDirectInfos[NB_DIRECT_INFO];

    // @}

  protected:
    /* Add the dart ADart in the double linked list of darts.
     */
    void addDartInList(CDart* ADart);

    /* Remove the dart ADart from the double linked list of darts.
     */
    void removeDartInList(CDart* ADart);
    
    
  private:
    // @name Champs privés :
    // @{

    /// Nombre de marques utilisées
    int FNbUsedMarks;

    /// Nombre de champs directInfo utilisés
    int FNbUsedDirectInfos;

#ifndef NDEBUG
    /// Nombre maximum de marques utilisées simultanément
    int FMaxNbUsedMarks;

    /// Nombre maximum de champs directInfo utilisés simultanément
    int FMaxNbUsedDirectInfos;
#endif // NDEBUG

    // @}
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("g-map-basic.icc")
//******************************************************************************
#endif // G_MAP_BASIC_HH
//******************************************************************************
