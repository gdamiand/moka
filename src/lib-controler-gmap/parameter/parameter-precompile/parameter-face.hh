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
#ifndef PARAMETER_FACE_HH
#define PARAMETER_FACE_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterAimedPoint regroupe les paramètres nécessaires au
 *  dessin des face pleines :
 *     * la couleur et un coefficient de transparence.
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

namespace GMap3d
{
  class CParameterFace : public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterFace(int ANbRef = 0);
    CParameterFace(const CParameterFace &);
    CParameter * copy() const;

    /** Destructeur
     */
    virtual ~CParameterFace();

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

    /// @name Accesseurs pour les couleurs des axes Ox, Oy et Oz
    //@{
    float         getCLFace(int AIndice) const;
    const float * getCLFace() const;
    void          setCLFace(int AIndice, float AValue);
    void          setCLFace(float AValue0, float AValue1, float AValue2);
    void          setCLFace(const float ATab[3]);
    //@}

    /// @name Accesseurs pour la transparence des faces
    //@{
    float getBLFace() const;
    void  setBLFace(float AValue);
    //@}

    /// @name Accesseurs pour les couleurs aléatoire des faces et des volumes
    //@{
    bool  getRandomCLFace() const;
    void  setRandomCLFace(bool AValue) ;
    bool  getRandomCLVolume() const;  
    void  setRandomCLVolume(bool AValue);  
    //@}

    /// @name Accesseurs pour activer le calcul des couleurs aléatoire à partir
    /// de la géométrie (vrai) ou à partir de la topologie (faux).
    //@{
    bool  getRandomColorGeometry() const;
    void  setRandomColorGeometry(bool AValue) ;
    //@}
   
    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream& operator << (std::ostream&, const CParameterFace &);
    friend std::istream& operator >> (std::istream&, CParameterFace &);

  private:

    /// Couleurs
    float FCLFace[3];

    /// Coefficient de transparence des faces
    float FBLFace;

    /// Booleens pour gerer les couleurs aléatoire des faces et des volumes
    bool FRandomCLFace;
    bool FRandomCLVolume;
    
    /// Booleens pour calculer les couleurs aléatoires à partir de la
    /// géométrie (vrai) ou à partir de la topologie (faux).
    bool FRandomColorGeometry;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_FACE_HH
//******************************************************************************
