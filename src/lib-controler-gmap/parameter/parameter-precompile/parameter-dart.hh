/*
 * lib-controler-gmap : Le contrôleur de 3-G-cartes, surcouche de lib-controler.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler-gmap
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
#ifndef PARAMETER_DART_HH
#define PARAMETER_DART_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterDart regroupe les paramètres nécessaires au dessin de la liste des brins
 *  - largeurs des différents éléments (traits, points pour les brins 0-libre)
 *  - les couleurs des différents éléments (brin sélectionné ou non, dernier brin)
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

namespace GMap3d
{
  class CParameterDart : public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterDart(int ANbSelectionLevels, int ANbRef = 0);
    CParameterDart(const CParameterDart &);
    CParameter * copy() const;

    /** Destructeur
     */
    virtual ~CParameterDart();

    //@}

    /** @name Sauvegarde et chargement des paramètres
     */

    //@{

    /** save
     *
     *  Méthode permettant de sauver les valeurs des variables membres.
     */
    virtual void save(std::ostream&);

    /** load
     *
     *  Méthode permettant de récupérer les valeurs des variables membres.
     */
    virtual void load(std::istream&);

    /** reinit
     *
     *    Reinitialise le paramètre.
     */
    virtual void reinit();

    //@}

    /** @name Accesseurs pour la taille des lignes
     */

    //@{
    int  getLWDarts() const;
    void setLWDarts(int);

    //@}

    /** @name Accesseurs pour différentes mesures
     */

    //@{
    int  getSAloneDarts() const; // taille des brin 0-libre
    void setSAloneDarts(int);
    //@}

    /** @name Accesseurs pour les couleurs
     */

    //@{
    /// Brins non sélectionnés
    float         getCLUnsel(int ALevel, int AIndice) const;
    void          setCLUnsel(int ALevel, int AIndice, float AValue);
    const float * getCLUnsel(int ALevel) const;
    void          setCLUnsel(int ALevel,
			     float AValue0, float AValue1, float AValue2);
    void          setCLUnsel(int ALevel,const float ATab[3]);

    /// Brins sélectionnés
    float         getCLSel(int ALevel, int AIndice) const;
    void          setCLSel(int ALevel, int AIndice, float AValue);
    const float * getCLSel(int ALevel) const;
    void          setCLSel(int ALevel,
			   float AValue0, float AValue1, float AValue2);
    void          setCLSel(int ALevel,const float ATab[3]);

    /// Dernier brins sélectionnés
    float         getCLLastSel(int ALevel, int AIndice) const;
    void          setCLLastSel(int ALevel, int AIndice, float AValue);
    const float * getCLLastSel(int ALevel) const;
    void          setCLLastSel(int ALevel,
			       float AValue0, float AValue1, float AValue2);
    void          setCLLastSel(int ALevel,const float ATab[3]);

    //@}

    /// Brins 0-supprimés
    float         getCL0Remove(int ALevel, int AIndice) const;
    void          setCL0Remove(int ALevel, int AIndice, float AValue);
    const float * getCL0Remove(int ALevel) const;
    void          setCL0Remove(int ALevel,
			     float AValue0, float AValue1, float AValue2);
    void          setCL0Remove(int ALevel,const float ATab[3]);

    /// Brins 1-supprimés
    float         getCL1Remove(int ALevel, int AIndice) const;
    void          setCL1Remove(int ALevel, int AIndice, float AValue);
    const float * getCL1Remove(int ALevel) const;
    void          setCL1Remove(int ALevel,
			     float AValue0, float AValue1, float AValue2);
    void          setCL1Remove(int ALevel,const float ATab[3]);

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream& operator << (std::ostream&, const CParameterDart &);
    friend std::istream& operator >> (std::istream&, CParameterDart &);

  private:
    /// Nombre de niveaux de sélections.
    /// Nécessaire pour allouer les tableaux de couleurs.
    int FNbSelectionLevels;

    /// Largeur des lignes
    int FLWDarts;

    /// Taille pour les points représentant les brins 0-libre
    int FSAloneDarts;

    /// The different possible colors
    /// (unselected darts, selected, last, 0removed, 1removed)
    float ** FCLUnsel;

    float ** FCLSel;

    float ** FCLLastSel;

    float ** FCL0Remove;

    float ** FCL1Remove;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_DART_HH
//******************************************************************************
