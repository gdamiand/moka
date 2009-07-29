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
#ifndef PARAMETER_EYE_POSITION_HH
#define PARAMETER_EYE_POSITION_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterEyePosition regroupe les paramètres pour la position de
 *  l'oeil.
 *  - 2 angles de rotations pour tourner autour de la scène
 *  - une distance pour zoomer
 *  - Les pas de deplacement de l'oeil
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

  class CParameterEyePosition : public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterEyePosition(int ANbRef = 0);
    CParameterEyePosition(const CParameterEyePosition &);
    CParameter * copy() const;

    /** Destructeur
     */
    virtual ~CParameterEyePosition();

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

    /** getRotationAlpha
     *
     *  @return la valeur de FRotationAlpha.
     */
    float getRotationAlpha() const;

    /** setRotationAlpha
     *
     *  @param ARotation Le nouvel angle
     *
     *  Affecte la valeur FRotationAlpha.
     */
    void setRotationAlpha(float);

    /** getRotationBeta
     *
     *  @return la valeur de FRotationBeta.
     */
    float getRotationBeta() const;

    /** setRotationBeta
     *
     *  @param ARotation Le nouvel angle
     *
     *  Affecte la valeur FRotationBeta.
     */
    void setRotationBeta(float);

    /** getDistance
     *
     *  @return la valeur de FDistance.
     */
    float getDistance()  const;

    /** setDistance
     *
     *  @param ADistance La nouvelle distance
     *
     *  Affecte la valeur FDistance.
     */
    void setDistance(float);

    /** incRotationAlpha
     *
     *  @param ADelta Le delta
     *
     *  Incrémente la valeur FRotationAlpha de ADelta.
     */
    void incRotationAlpha(float);

    /** incRotationBeta
     *
     *  @param ADelta Le delta
     *
     *  Incrémente la valeur FRotationBeta de ADelta.
     */
    void incRotationBeta(float);

    /** incDistance
     *
     *  @param ADelta Le delta
     *
     *  Incrémente la valeur FDistance de ADelta.
     */
    void incDistance(float);
    
    
      
    
    /** getPasAvancement
     *
     *  @return la valeur de FPasAvancement.
     */
    float getPasAvancement()  const;

    
    /** setPasAvancement
     *
     *  @param ADistance La nouvelle distance
     *
     *  Affecte la valeur FPasAvancement.
     */
    void setPasAvancement(float);
    
    
    /** getAngleRotation
     *
     *  @return la valeur de FAngleRotation.
     */
    float getAngleRotation()  const;

    
    
    /** setAngleRotation
     *
     *  @param ADistance La nouvelle distance
     *
     *  Affecte la valeur FAngleRotation.
     */
    void setAngleRotation(float);

    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream&
    operator<<(std::ostream&,const CParameterEyePosition &);

    friend std::istream&
    operator>>(std::istream&, CParameterEyePosition &);
  
  private:
    /// Angle en degré pour la rotation selon l'axe [Oz)
    float FRotationAlpha;

    /// Angle en degré pour la rotation selon l'axe [Ox)
    float FRotationBeta;

    /// Distance entre l'observateur et l'origine de la scène
    float FDistance;
    
    /// Distance entre 2 deplacements du point de vue.
    float FPasAvancement;
    
    /// Angle entre 2 rotations du point de vue.
    float FAngleRotation;

  };

#endif // PARAMETER_EYE_POSITION_HH
//******************************************************************************
