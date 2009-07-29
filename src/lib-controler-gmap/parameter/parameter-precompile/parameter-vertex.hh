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
#ifndef PARAMETER_VERTEX_HH
#define PARAMETER_VERTEX_HH
//******************************************************************************
#include "parameter.hh"
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

/** La classe CParameterVertex regroupe les paramètres nécessaires au dessin de
 *  la liste des sommets :
 *  - largeurs des différents éléments
 *  - les couleurs des différents éléments
 *
 *  Cette classe contient simplement les accesseurs et modificateurs
 *  des différentes variables membres.
 */

namespace GMap3d
{
  class CParameterVertex : public CParameter
  {
  public:
    /** @name Constructeur et destructeur
     */

    //@{

    /** Constructeur
     */
    CParameterVertex(int ANbRef = 0);
    CParameterVertex(const CParameterVertex &);
    CParameter * copy() const;

    /** Destructeur
     */
    virtual ~CParameterVertex();

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

    /** @name Accesseurs pour la taille des sommets
     */

    //@{
    int  getSVertex() const;
    void setSVertex(int);
    //@}

    /** @name Accesseurs pour les couleurs
     */

    //@{
    float         getCLVertex(int AIndice) const;
    const float * getCLVertex() const;
    void          setCLVertex(int AIndice, float AValue);
    void          setCLVertex(float AValue0, float AValue1, float AValue2);
    void          setCLVertex(const float ATab[3]);
    //@}

    /// Identificateur du type de paramètre.
    virtual int getType() const;

    /// Fonctions amies pour lire et écrire dans un flux.
    friend std::ostream& operator << (std::ostream&, const CParameterVertex &);
    friend std::istream& operator >> (std::istream&, CParameterVertex &);

  private:private:

    /// Taille pour les sommets
    int FSVertex;

    /// Couleurs
    float FCLVertex[3];
  };

} // namespace GMap3d
//******************************************************************************
#endif // PARAMETER_VERTEX_HH
//******************************************************************************
