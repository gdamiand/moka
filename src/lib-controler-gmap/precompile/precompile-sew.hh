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
#ifndef PRECOMPILE_SEW_HH
#define PRECOMPILE_SEW_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include "precompile.hh"
//******************************************************************************
namespace GMap3d
{
  class CParameterGMapVertex;
  class CParameterSew;
  
  /** La classe CPrecompileSew permet l'affichage des coutures.
   */
  class CPrecompileSew : public CPrecompile
  {
  public:
    //@name Constructeurs et destructeur.
    //@{
    /** Constructeur
     *
     * @param AParameterGmapV les paramètre de g-map-vertex.
     * @param AParameterSew les paramètres de dessin des coutures
     */
    CPrecompileSew(CParameterGMapVertex*, CParameterSew*);
    CPrecompileSew(const CPrecompileSew &);
    CPrecompile* copy() const;

    /// Destructeur
    virtual ~CPrecompileSew();
    //@}

    void        setParameter(CParameter* AParameter);
    CParameter* getParameter() const;
  
    /// Cette méthode retourne le type du precompile.
    virtual TPrecompile getType() const;

     /// Modifie le paramètre PARAMETER_GMAP_VERTEX associé au précompile.
    void setGMapVertex(CParameterGMapVertex* AGMapV);

    /// Modifie le paramètre PARAMETER_SEW associé au précompile.
    void setSew(CParameterSew* ASew);

  protected:

    /** drawSewsI
     *
     *  @param AIndice la dimension des coutures à afficher
     *  @param AMark la marque utilisée durant le parcours de la carte
     *
     *  Cette méthode affiche toute les coutures pour la dimension AIndice.
     */
    virtual void drawSewsI(int AIndice, int AMark);

    /// Cette méthode affiche toute les coutures.
    virtual void drawModel();

  private:
    /// Les différents paramètres
    CParameterGMapVertex * FParameterGMapV;
    CParameterSew        * FParameterSew;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PRECOMPILE_SEW_HH
//******************************************************************************
