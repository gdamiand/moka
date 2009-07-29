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
#ifndef PARAMETER_AIMED_POSITION_HH
#define PARAMETER_AIMED_POSITION_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterAimedPosition regroupe les paramètres pour la position
 *  du point que l'on regarde.
 *  - ses coordonnées
 *  - un angle afin de fixer "le haut"
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

  class CParameterAimedPosition : public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterAimedPosition(int ANbRef = 0);
    CParameterAimedPosition(const CParameterAimedPosition &);
    CParameter* copy() const;

    /** Destructeur
     */
    virtual ~CParameterAimedPosition();

    //@}

    /** @name Sauvegarde et chargement des paramètres
     */

    //@{

    /** save
     *
     *  Méthode permettant de sauver les valeurs des variables membres.
     */
    virtual void save(std::ostream &);

    /** load
     *
     *  Méthode permettant de récupérer les valeurs des variables membres.
     */
    virtual void load(std::istream &);

    /** reinit
     *
     * Méthode pour réinitialiser avec nos valeurs par defaut.
     */
    virtual void reinit();

    //@}

    /** @name Accesseurs et modificateurs des paramètres
     */

    //@{

    /** getRotation
     *
     *  @return la valeur de FRotation.
     */
    float getRotation() const;

    /** setRotation
     *
     *  @param ARotation Le nouvel angle
     *
     *  Affecte la valeur FRotation.
     */
    void setRotation(float);

    /** incRotation
     *
     *  @param ADelta Le delta
     *
     *  Incrémente la valeur FRotation de ADelta.
     */
    void incRotation(float);

    /** getLookAt
     *
     *  @param AIndice Un indice entre [0,1,2] de dimension
     *  @return la valeur de FLookAt[AIndice]
     */
    float getLookAt(int) const;

    /** setLookAt
     *
     *  @param AIndice Un indice entre [0,1,2] de dimension
     *  @param AValue La nouvelle coordonnée
     *
     *  Affecte la valeur FLookAt[AIndice] avec AValue
     */
    void  setLookAt(int, float);

    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream&
    operator<<(std::ostream&, const CParameterAimedPosition &);

    friend std::istream&
    operator>>(std::istream&, CParameterAimedPosition &);
			       
  private:
    /// Angle en degré pour la rotation dans le plan perpendiculaire à
    /// l'axe allant de l'oeil ou point regarde.
    float FRotation;

    /// Le point que l'on regarde (dans l'espace 3D)
    float FLookAt[3];
  };
//******************************************************************************
#endif // PARAMETER_AIMED_POSITION_HH
//******************************************************************************
