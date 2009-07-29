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
#ifndef PARAMETER_AXIS_HH
#define PARAMETER_AXIS_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterAxis regroupe les paramètres nécessaires au dessin du repère
 *  - largeurs des différents éléments (traits, longueur du repère)
 *  - les couleurs des différents éléments
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

  class CParameterAxis : public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterAxis(int ANbRef = 0);
    CParameterAxis(const CParameterAxis &);
    CParameter* copy() const;

    /** Destructeur
     */
    virtual ~CParameterAxis();

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
    int  getLWAxis() const;
    void setLWAxis(int);
    //@}

    /** @name Accesseurs pour la taille du repère
     */

    //@{
    float getLGAxis() const;
    void  setLGAxis(float);
    //@}

    /** @name Accesseurs pour les couleurs des axes Ox, Oy et Oz
     */

    //@{
    float         getCLAxisX(int AIndice) const;
    const float * getCLAxisX() const;
    void          setCLAxisX(int AIndice, float AValue);
    void          setCLAxisX(float AValue0, float AValue1, float AValue2);
    void          setCLAxisX(const float ATab[3]);

    float         getCLAxisY(int AIndice) const;
    const float * getCLAxisY() const;
    void          setCLAxisY(int AIndice, float AValue);
    void          setCLAxisY(float AValue0, float AValue1, float AValue2);
    void          setCLAxisY(const float ATab[3]);

    float         getCLAxisZ(int AIndice) const;
    const float * getCLAxisZ() const;
    void          setCLAxisZ(int AIndice, float AValue);
    void          setCLAxisZ(float AValue0, float AValue1, float AValue2);
    void          setCLAxisZ(const float ATab[3]);
    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream& operator << (std::ostream&, const CParameterAxis &);
    friend std::istream& operator >> (std::istream&, CParameterAxis &);

  private:

    /// Taille des lignes
    int FLWAxis;

    /// Taille du repère
    float FLGAxis;

    /// Couleurs
    float FCLAxisX[3];
    float FCLAxisY[3];
    float FCLAxisZ[3];
  };

//******************************************************************************
#endif // PARAMETER_AXIS_HH
//******************************************************************************
