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
#ifndef PARAMETER_SEW_HH
#define PARAMETER_SEW_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterSew regroupe les paramètres nécessaires au dessin des
 *  coutures :
 *  - largeur, couleur, position sur les brins
 *  - des flags pour afficher seulement certaines coutures
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

namespace GMap3d
{
  class CParameterSew: public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterSew(int ANbRef = 0);
    CParameterSew(const CParameterSew &);
    CParameter * copy() const;

    /** Destructeur
     */
    virtual ~CParameterSew();

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

    int  getLWSew(int ADimension) const;
    void setLWSew(int ADimension, int);

    //@}

    /** @name Accesseurs pour la position des coutures sur les brins
     */

    //@{

    float getSewPosCoef(int ADimension) const;
    void  setSewPosCoef(int ADimension, float AValue);

    //@}

    /** @name Accesseurs pour les couleurs
     */

    //@{
    float         getCLSew(int ADimension, int AIndice) const;
    const float * getCLSew(int ADimension) const;
    void          setCLSew(int ADimension, int AIndice, float AValue);
    void          setCLSew(int ADimension,float AValue0,
			   float AValue1, float AValue2);
    void          setCLSew(int ADimension,const float ATab[3]);
    //@}

    /** @name Accesseurs pour les flags
     *       bool getXXX() const; pour récupérer la valeur du flag
     *       void setXXX(bool);   pour fixer la valeur
     *       void invXXX();       pour inverser sa valeur
     */

    //@{

    bool getDisplayAlpha(int AIndex) const; /// affichage de certain alpha
    void setDisplayAlpha(int AIndex,bool AValue);
    void invDisplayAlpha(int AIndex);

    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream& operator << (std::ostream&, const CParameterSew &);
    friend std::istream& operator >> (std::istream&, CParameterSew &);

  private:
    /// Largeur des lignes
    int FLWSew[4];

    /// Position des coutures
    float FSewPosCoef[4];

    /// Couleurs
    float FCLSew[4][3];

    /// Flags
    bool FDisplayAlpha[4];
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_SEW_HH
//******************************************************************************
