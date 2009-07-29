/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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
#ifndef PARAMETER_AIMED_POINT_HH
#define PARAMETER_AIMED_POINT_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterAimedPoint regroupe les paramètres nécessaires au dessin du point que l'on regarde
 *  - largeurs des différents éléments (traits, longueur de la croix)
 *  - les couleurs des différents éléments
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

  class CParameterAimedPoint : public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterAimedPoint(int ANbRef = 0);
    CParameterAimedPoint(const CParameterAimedPoint &);
    CParameter * copy() const;

    /** Destructeur
     */
    virtual ~CParameterAimedPoint();

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
    int  getLWAimedPoint() const;
    void setLWAimedPoint(int);
    //@}

    /** @name Accesseurs pour la taille de la croix
     */

    //@{
    float getLGAimedPoint() const;
    void  setLGAimedPoint(float);
    //@}

    /** @name Accesseurs pour les couleurs des axes Ox, Oy et Oz
     */

    //@{
    float         getCLAimedPoint(int AIndice) const;
    void          setCLAimedPoint(int AIndice, float AValue);
    const float * getCLAimedPoint() const;
    void          setCLAimedPoint(float AValue0, float AValue1, float AValue2);
    void          setCLAimedPoint(const float ATab[3]);
    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream&
    operator<<(std::ostream&, const CParameterAimedPoint &);

    friend std::istream&
    operator>>(std::istream&, CParameterAimedPoint &);

  private:

    /// Taille des lignes
    int FLWAimedPoint;

    /// Taille de la croix
    float FLGAimedPoint;

    /// Couleurs
    float FCLAimedPoint[3];
  };

//******************************************************************************
#endif // PARAMETER_AIMED_POINT_HH
//******************************************************************************
