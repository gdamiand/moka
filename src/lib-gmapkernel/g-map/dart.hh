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
#ifndef DART_HH
#define DART_HH
//******************************************************************************
#include "kernel-types.hh"
#include "inline-macro.hh"

#include <bitset>
//******************************************************************************
class CAttribute;
class CEmbedding;

namespace GMap3d
{
  /**
   * Brin d'une 3-G-Carte.
   *
   * Cette classe contient entre autres les fonctions de manipulation bas
   * niveau des alphas, ainsi que celles de gestion des attributs (classe
   * CEmbedding).
   * Ses méthodes ne doivent pas être directement appelées par un utilisateur
   * lambda : la manipulation des brins se fait toujours à l'aide de la classe
   * de la carte généralisée (CGMapBasic / CGMap).
   *
   * @author MOKA TEAM
   */
  class CDart
  {
  public:
    // @name Constructeurs et destructeur
    // @{

    /**
     * Constructeur par défaut
     *
     * Construit un nouveau brin non marqué dont les alphas sont initialisés à
     * l'identité. Ce constructeur ne doit être appelé que par des utilisateurs
     * avertis (préférer la méthode d'ajout de brin de la classe CGMap).
     *
     * @return Une nouvelle instance de la classe CDart
     */
    CDart();

    /**
     * Constructeur par recopie
     *
     * Construit un nouveau brin non marqué dont les alphas sont initialisés à
     * l'identité. Seules les marques du brin passé en paramètre sont
     * recopiées.
     *
     * @return Une nouvelle instance de la classe CDart
     */
    CDart(const CDart& ADart);

    /**
     * Constructeur avec marquage
     *
     * Construit un nouveau brin marqué selon la suite de marques AMarks, et
     * dont les alphas sont initialisés à l'identité. Ce constructeur ne doit
     * être appelé que par des utilisateurs avertis (préférer la méthode
     * d'ajout de brin de la classe CGMap).
     *
     * @param AMarks Un tableau de marques à appliquer au brin créé
     *
     * @return Une nouvelle instance de la classe CDart
     */
    CDart(const std::bitset<NB_MARKS>& AMarks);

    /**
     * Destructeur
     *
     * Détruit une instance de la classe CDart.
     * Cette méthode est à surcharger lors de la création d'une classe dérivée
     * de CDart.
     */
    ~CDart();

    // @}
    /**
     * @name Utilisation des champs direct-info
     *
     * Une information directe est un pointeur vers une structure de données
     * quelconque. Le nombre d'informations par brin est fixé par la macro
     * NB_DIRECT_INFO (voir fichier kernel-types.hh). Ceci permet un accès aux
     * informations plus rapide qu'en passant par les attributs et les
     * embeddings mais plus coûteux en place mémoire.
     */
    // @{

    /**
     * @param AIndice Le numéro de l'information directe à récupérer.
     *   L'information directe permet de simuler une orbite vide
     *   (ORBIT_SELF) de manière plus rapide mais il n'y a aucune
     *   vérification.
     * @return Un pointeur sur l'information recherchée.
     *
     * @precondition 0 <= AIndice < NB_DIRECT_INFO
     */
    void* getDirectInfo(int ADirectIndex) const;

    /**
     * @param AIndice Le numéro de champ à modifier
     * @param APointer La valeur à affecter
     *
     * @precondition 0 <= AIndice < NB_DIRECT_INFO
     */
    void setDirectInfo(int ADirectIndex, void* AValue);

    // @}
    // @name Gestion des plongements
    // @{

    /**
     * Récupération du premier élément de la liste des "CEmbedding"
     *
     * Un "CEmbedding" est une liste d'attributs appartenant à une même orbite,
     * dont le brin est propriétaire (voir de plus amples explications avec la
     * clase CEmbedding).
     * Chaque liste d'attributs d'une même orbite est elle-même chaînée avec
     * les autres listes d'un même brin. Cette méthode retourne le premier
     * élément (un "CEmbedding") de la liste chaînée des listes d'attributs.
     *
     * @return La première liste d'attributs
     */
    CEmbedding* getFirstEmbedding() const;

    /**
     * Définition du premier élément de la liste des "CEmbedding"
     *
     * Cette méthode pose un "CEmbedding" comme étant le premier élément de la
     * liste des "CEmbedding", sans aucune vérification d'existence du premier
     * "CEmbedding". Cette méthode ne doit être utilisée que par la carte
     * généralisée.
     *
     * @param AEmbedding la liste d'attributs à placer en première position.
     */
    void setFirstEmbedding(CEmbedding* AEmbedding);

    /**
     * Récupération de la liste d'attributs plongés sur une orbite donnée
     *
     * Cette méthode recherche dans la liste des "CEmbedding" celui qui
     * correspond à l'orbite orbit.
     *
     * @param AOrbit l'orbite correspondant à la liste d'attributs recherchés
     * @return la liste d'attributs si elle est trouvée
     */
    CEmbedding* getEmbedding(TOrbit AOrbit) const;

    /**
     * Ajout d'une liste d'attributs à l'instance
     *
     * Cette méthode ajoute la liste AEmbedding à la liste des "CEmbedding".
     *
     * @param AEmbedding une liste d'attributs
     * @precondition La liste AEmbedding n'est pas vide et aucune liste de même
     *   orbite n'existe sur le brin instancié.
     * @precondition les champs prev et next de AEmbedding sont a NULL
     */
    void addEmbedding(CEmbedding* AEmbedding);

    /**
     * Suppression d'une liste d'attributs donnée dans la liste des
     * "CEmbedding" de l'instance
     *
     * Cette méthode ote la liste d'attributs E de la liste des "CEmbedding".
     *
     * @param AEmbedding la liste d'attributs à enlever
     * @return la liste AEmbedding
     *
     * @precondition AEmbedding n'est pas vide
     */
    CEmbedding* removeEmbedding(CEmbedding* AEmbedding);

    /**
     * Suppression de la liste d'attributs correspondant à une orbite donnée
     * dans les "CEmbedding"
     *
     * Cette méthode recherche la liste d'attributs correspondant à l'orbite
     * orbit, et la supprime de la liste des "CEmbedding".
     *
     * @param AOrbit l'orbite de la liste à enlever
     * @return la liste correspondante
     */
    CEmbedding* removeEmbedding(TOrbit AOrbit);

    /**
     * Suppression puis destruction d'une liste d'attributs donnée dans la
     * liste des "CEmbedding"
     *
     * Cette méthode ote la liste d'attributs E de la liste des "CEmbedding"
     * avant de la détruire.
     *
     * @param AEmbedding la liste d'attributs à détruire
     * @precondition E n'est pas vide
     */
    void deleteEmbedding(CEmbedding* AEmbedding);

    /**
     * Suppression puis destruction de la liste d'attributs correspondant à une
     * orbite donnée
     *
     * Cette méthode recherche la liste d'attributs correspondant à l'orbite
     * AOrbit, la retire de la liste des "CEmbedding", et enfin la détruit.
     *
     * @param AOrbit l'orbite de la liste à détruire
     */
    void deleteEmbedding(TOrbit AOrbit);

    /**
     * Ajout, avec fusion le cas échéant, d'une liste d'attributs à l'instance
     *
     * Cette méthode vérifie si une liste d'attributs de même orbite existe
     * déjà sur le brin instancié. Si ce n'est pas le cas, la liste E est
     * simplement ajoutée. Sinon les attributs de E sont ajoutés à la liste
     * déjà présente.
     *
     * @precondition AEmbedding n'est pas vide
     */
    void mergeEmbedding(CEmbedding* AEmbedding);

    /**
     * Récupération d'un attribut plongé sur une orbite donnée
     *
     * Cette méthode trouve dans la liste des "CEmbedding" la liste d'attributs
     * qui correspond à l'orbite orbit, puis y recherche l'attribut de type
     * AAttribType.
     *
     * @param AOrbit l'orbite de plongement de l'attribut recherché
     * @param AAttribType le numéro d'identifiant de son type
     * @return l'attribut si il est trouvé
     */
    CAttribute* getAttribute (TOrbit AOrbit, TAttributeId AAttribType) const;

    /**
     * Ajout d'un attribut sur une orbite donnée
     *
     * Cette méthode recherche dans la liste des "CEmbedding" la liste
     * d'attributs qui correspond à l'orbite orbit. Si elle est trouvée,
     * l'attribut data y est ajouté. Sinon une liste d'attributs contenant
     * data est créé pour cette orbite.
     *
     * @param orbit l'orbite de plongement de l'attribut à ajouter
     * @param data l'attribut
     * @precondition l'attribut data ne peut être nul
     */
    void addAttribute(TOrbit AOrbit, CAttribute* AAttribute);

    /**
     * Suppression d'un attribut de la liste des attributs d'une orbite donnée
     *
     * Cette méthode recherche la liste d'attributs correspondant à l'orbite
     * orbit, et si elle existe, enlève l'attribut de type typeAttr qu'elle
     * contient. Si la liste d'attributs est alors vide, elle est supprimée de
     * la liste des "CEmbedding".
     *
     * @param orbit l'orbite de plongement de l'attribut à supprimer
     * @param attrType le numéro d'identifiant de son type
     * @return l'attribut enlevé
     */
    CAttribute* removeAttribute(TOrbit AOrbit, TAttributeId AAttribType);

    /**
     * Suppression et destruction d'un attribut sur une orbite donnée
     *
     * Cette méthode supprime l'attribut, de type AAttrType, de la liste des
     * attributs de l'orbite AOrbit, puis le détruit.
     *
     * @param AOrbit l'orbite de plongement de l'attribut à supprimer
     * @param AAttrType le numéro d'identifiant de son type
     */
    void deleteAttribute(TOrbit AOrbit, TAttributeId AAttribType);

    /**
     * Méthode précisant que la présence d'une orbite donnée dans la liste des
     * "CEmbedding" de l'instance.
     *
     * Cette méthode définit comme utilisée l'orbite orbit sur le brin. Aucune
     * vérification supplémentaire n'est effectué sur la cohérence des
     * plongements, cette méthode étant une méthode de premier niveau.
     *
     * @param AOrbit l'orbite à utiliser
     * @precondition ORBIT_SELF <= AOrbit <= ORBIT0123
     */
    void setOrbitUsed(TOrbit AOrbit);

    /**
     * Méthode testant si une orbite est utilisée sur le brin
     *
     * @return le booléen répondant au test
     * @precondition ORBIT_SELF <= AOrbit <= ORBIT0123
     */
    bool isOrbitUsed(TOrbit AOrbit) const;

    /**
     * Méthode libérant l'utilisation d'une orbite donnée sur le brin instancié
     *
     * Aucune vérification supplémentaire n'est effectué sur la cohérence des
     * plongements.
     *
     * @precondition ORBIT_SELF <= AOrbit && AOrbit <= ORBIT0123
     */
    void unsetOrbitUsed(TOrbit AOrbit);

    // @}
    // @name Chaînage des brins dans la carte généralisée
    // @{

    /**
     * Récupération du brin suivant dans la liste
     *
     * La liste chaînée de brins permet à la carte généralisée de passer en
     * revue l'ensemble des brins qu'elle contient. Cette méthode permet de se
     * déplacer au prochain brin. Elle ne doit pas être utilisé par un
     * utilisateur non averti (préférer un parcours : dynamic ou static
     * CCoverageAll).
     *
     * @return le brin de la liste chaînée qui suit le brin instancié.
     */
    CDart* getNext() const;

    /**
     * Récupération du brin précédent dans la liste
     *
     * Même remarque que précédemment.
     *
     * @return le brin de la liste chaînée qui précéde le brin instancié.
     */
    CDart* getPrev() const;

    /**
     * Définition du brin suivant dans la liste
     *
     * Cette méthode permet de définir quel est le brin suivant l'instance dans
     * la liste. Cette méthode ne doit pas être directement utilisée, elle
     * permet à la carte généralisée d'ajouter un brin dans la liste (préférer
     * addMapDart de la classe CGMapBasic).
     *
     * @param le brin de la liste chaînée qui suit l'instance.
     */
    void setNext(CDart* ADart);

    /**
     * Définition du brin précédent dans la liste
     *
     * Même remarque que précédemment.
     *
     * @param le brin de la liste chaînée qui précéde l'instance.
     */
    void setPrev(CDart* ADart);

    // @}
    // @name Accès aux involutions
    // @{

    /**
     * Accesseur de l'involution alpha0
     *
     * Cette méthode renvoie le brin qui est cousu par alpha0 à l'instance.
     * Si cette instance n'est pas cousue à un autre brin, la méthode rentourne
     * l'instance elle-même. Préférer tout de même la méthode de même nom de la
     * classe CGMapBasic/CGMap.
     *
     * @return le brin cousu par alpha0.
     */
    CDart* getAlpha0() const;

    /**
     * Accesseur de l'involution alpha1
     *
     * Même remarque qu'à la méthode précédente.
     *
     * @return le brin cousu par alpha1.
     */
    CDart* getAlpha1() const;

    /**
     * Accesseur de l'involution alpha2
     *
     * Même remarque qu'à la méthode précédente.
     *
     * @return le brin cousu par alpha2.
     */
    CDart* getAlpha2() const;

    /**
     * Accesseur de l'involution alpha3
     *
     * Même remarque qu'à la méthode précédente.
     *
     * @return le brin cousu par alpha3.
     */
    CDart* getAlpha3() const;

    /**
     * Accesseur de l'involution alphaI
     *
     * Cette méthode renvoie le brin qui est cousu par alphaI à l'instance,
     * avec AIndex un numéro donné par l'utilisateur.
     *
     * @param ADimension le numéro de la liaison à regarder (entre 0 et 3).
     * @return le brin cousu par alphaI.
     */
    CDart* getAlpha(int ADimension) const;

    /**
     * Définition de l'involution alpha0
     *
     * Cette méthode permet de lier un brin à l'instance par alpha0. Seule la
     * classe CGMapBasic/CGMap peut l'utiliser (préférer pour les coutures les
     * méthodes *  de la classe CGMapBasic/CGMap telles que sew0, voire
     * dartSew0 ou topoSew0, et en dernière solution linkAlpha0).
     *
     * @param le brin à lier par alpha0 à l'instance.
     */
    void setAlpha0(CDart* ADart);

    /**
     * Définition de l'involution alpha1
     *
     * Même remarque que précédemment.
     *
     * @param D le brin à lier par alpha1 à l'instance.
     */
    void setAlpha1(CDart* ADart);

    /**
     * Définition de l'involution alpha2
     *
     * Même remarque que précédemment.
     *
     * @param D le brin à lier par alpha2 à l'instance.
     */
    void setAlpha2(CDart* ADart);

    /**
     * Définition de l'involution alpha3
     *
     * Même remarque que précédemment.
     *
     * @param D le brin à lier par alpha3 à l'instance.
     */
    void setAlpha3(CDart* ADart);

    /**
     * Définition de l'involution alphaI
     *
     * Même remarque que précédemment (préférer sew, voire dartSew ou topoSew,
     * et en dernier recours linkAlpha).
     *
     * @param ADart le brin à lier par alphaI à l'instance.
     * @param ADimension le dimension de la liaison alpha à considérer.
     */
    void setAlpha (CDart* ADart, int ADimension);

    /**
     * Test permettant de savoir si un brin est libre (non cousu) par alpha0.
     *
     * Cette méthode teste si l'instance est cousu à lui-même par alpha0. Elle
     * renvoie VRAI si c'est le cas, FAUX si le brin instancié est cousu à un
     * autre brin.
     *
     * @return un booléen signifiant si l'instance est libre par alpha0.
     */
    bool isFree0() const;

    /**
     * Test permettant de savoir si un brin est libre (non cousu) par alpha1.
     *
     * Même remarque que précédemment.
     *
     * @return un booléen signifiant si le brin instancié est libre par alpha1.
     */
    bool isFree1() const;

    /**
     * Test permettant de savoir si un brin est libre (non cousu) par alpha2.
     *
     * Même remarque que précédemment.
     *
     * @return un booléen signifiant si le brin instancié est libre par alpha2.
     */
    bool isFree2() const;

    /**
     * Test permettant de savoir si un brin est libre (non cousu) par alpha3.
     *
     * Même remarque que précédemment.
     *
     * @return un booléen signifiant si le brin instancié est libre par alpha3.
     */
    bool isFree3() const;

    /**
     * Test permettant de savoir si un brin est libre (non cousu) par alphaI.
     *
     * Cette méthode teste si l'instance est cousu à lui-même par alphaI, avec
     * i la dimension de la liaison à regarder. Elle renvoie VRAI si c'est le
     * cas, FAUX si l'instance est cousu à un autre brin.
     *
     * @param ADimension la dimension de la liaison à tester.
     * @return un booléen signifiant si l'instance est libre par alpha3.
     */
    bool isFree(int ADimension) const;

    /**
     * Libération de l'involution alpha0 de l'instance.
     *
     * Cette méthode coud pour alpha0 l'instance à elle-même. Le brin
     * devient libre par alpha0, mais aucune vérification de la cohérence
     * de la carte généralisée n'est faite. Préférer donc les méthodes de
     * la classe CGMapBasic/CGMap telles que unsew0, voire dartUnsew0 ou
     * topoUnsew0, et en dernier ressort unlinkAlpha0.
     */
    void setFree0();

    /**
     * Libération de l'involution alpha1 de l'instance.
     *
     * Même remarque que précédemment.
     */
    void setFree1();

    /**
     * Libération de l'involution alpha2 de l'instance.
     *
     * Même remarque que précédemment.
     */
    void setFree2();

    /**
     * Libération de l'involution alpha3 de l'instance.
     *
     * Même remarque que précédemment.
     */
    void setFree3();

    /**
     * Libération pour le brin instancié de son involution alphaI
     *
     * Cette méthode coud pour alphaI l'instance à elle-même. Le brin
     * devient libre par alphaI, mais aucune vérification de la cohérence
     * de la carte généralisée n'est faite. Préférer donc les méthodes de
     * la classe CGMapBasic/CGMap telles que unsew, voire dartUnsew ou
     * topoUnsew, et en dernier ressort unlinkAlpha.
     *
     * @param ADimension le dimension de la liaison alphaI à considérer.
     */
    void setFree(int ADimension);

    // @}
    // @name Gestion des marques
    // @{

    /**
     * Test permettant de savoir si un brin est marqué.
     *
     * Cette méthode renvoie VRAI si le brin possède la marque i.
     * Le nombre de marques par brin est fixé par la variable
     * NB_MARKS du fichier dart.hpp.
     *
     * @param i Le numéro de la marque à tester (entre 0 et NB_MARKS-1).
     * @return booléen signifiant si le brin est marqué.
     * @precondition 0 <= i < NB_MARKS
     */
    bool getMark(int AMarkNumber) const;

    /**
     * Méthode définissant si un brin est marqué ou non.
     *
     * Cette méthode définit la i-ème marque selon le booléen value.
     * Si value est à VRAI, le brin sera marqué par la marque i.
     *
     * @param i Le numéro de la marque à changer (entre 0 et NB_MARKS-1).
     * @param value La valeur booléenne de la marque.
     * @precondition 0 <= i < NB_MARKS
     */
    void setMark(int AMarkNumber, bool AValue);

    /**
     * Méthode d'accès à l'ensemble des marques d'un brin.
     *
     * Cette méthode récupère toutes les marques d'un brin.
     *
     * @return l'ensemble de marques (de type bitset)
     */
    std::bitset<NB_MARKS> getMarks() const;

    /**
     * Méthode définissant en une seule fois l'ensemble des marques d'un brin.
     *
     * Cette méthode définit chacune des marques d'un brin suivant la suite de
     * valeurs données par marks.
     *
     * @param marks l'ensemble des valeurs à prendre par les marques du brin
     */
    void setMarks(const std::bitset<NB_MARKS> & AMarks);

    // @}

  private:
    // @name Champs privés
    // @{

    /// Tableau des liaisons alpha (suite de pointeurs sur un brin)
    CDart* FAlpha[4];

    /// Début de la liste chaînée des plongements
    CEmbedding* FFirstEmbedding;

    /// Valeur des marques booléennes du brin
    std::bitset<NB_MARKS> FMarks;

    /**
     * Tableau des orbites plongées sur le brin, seules les plongements dont
     * il est propriétaire sont signalés dans cette suite (voir la notion de
     * propriété d'un plongement dans la classe CGMap).
     */
    std::bitset<16> FUsedOrbits;

    /// Tableau des informations directes
    void* FDirectInfo[NB_DIRECT_INFO];

    /**
     * Variables permettant de définir les brins suivant et précédent dans la
     * liste chaînée des brins de la carte généralisée.
     */
    CDart* FPrev;
    CDart* FNext;

    // @}
  };

} // namespace GMap3d
//******************************************************************************
#include INCLUDE_INLINE("dart.icc")
//******************************************************************************
#endif // DART_HH
//******************************************************************************
