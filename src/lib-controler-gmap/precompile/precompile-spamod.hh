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
#ifdef MODULE_SPAMOD
//******************************************************************************
#ifndef PRECOMPILE_SPAMOD_HH
#define PRECOMPILE_SPAMOD_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include "precompile.hh"
//******************************************************************************
namespace GMap3d
{
  class CParameterGMapVertex;
  class CParameterSpamod;
  class CSpamodAPI;
  //****************************************************************************

  /// La classe CPrecompileSpamod permet l'affichage des objets Spamod.
  class CPrecompileSpamod : public CPrecompile
  {
  public:
    //@name Constructeurs et destructeur.
    //@{
    /** Constructeur
     *
     * @param AParameterGmapV les paramètre de g-map-vertex.
     * @param AParameterSpamod les paramètres de dessin de Spamod
     */
    CPrecompileSpamod(CParameterGMapVertex*, CParameterSpamod*);
    CPrecompileSpamod(const CPrecompileSpamod &);
    CPrecompile* copy() const;

    /// Destructeur
    virtual ~CPrecompileSpamod();
    //@}

    void        setParameter(CParameter* AParameter);
    CParameter* getParameter() const;
  
    /// Cette méthode retourne le type du precompile.
    virtual TPrecompile getType() const;

    /// Modifie le paramètre PARAMETER_GMAP_VERTEX associé au précompile.
    void setGMapVertex(CParameterGMapVertex* AGMapV);

    /// Modifie le paramètre PARAMETER_SPAMOD associé au précompile.
    void setParameterSpamod(CParameterSpamod*);

  protected:

    /// Cette méthode affiche la / les grille(s).
    virtual void drawModel();

  private:
    /// Les différents paramètres
    CParameterGMapVertex * FParameterGMapV;
    CParameterSpamod     * FParameterSpamod;
    CSpamodAPI           * FSpamodAPI;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PRECOMPILE_SPAMOD_HH
//******************************************************************************
#endif // MODULE_SPAMOD
//******************************************************************************
