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
#ifndef PRECOMPILE_NORMAL_VECTOR_HH
#define PRECOMPILE_NORMAL_VECTOR_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include "precompile.hh"
//******************************************************************************
namespace GMap3d
{
  class CParameterGMapVertex;
  class CParameterSelection;
  class CParameterNormalVector;
  class CDart;

  /** La classe CPrecompileNormalVector permet l'affichage des vecteurs normaux
   *  des faces de la carte.
   */
  class CPrecompileNormalVector : public CPrecompile
  {
  public:
    //@name Constructeurs et destructeur.
    //@{
    /** Constructeur
     *
     * @param AParameterGmapV les paramètre de g-map-vertex.
     * @param AParameterNormalVector les paramètres de dessin des normales
     * @param AParameterSelection les paramètres permettant d'accéder au brin
     * last
     */
    CPrecompileNormalVector(CParameterGMapVertex*,
			    CParameterNormalVector*,
			    CParameterSelection*);
    CPrecompileNormalVector(const CPrecompileNormalVector &);
    CPrecompile* copy() const;

    /// Destructeur
    virtual ~CPrecompileNormalVector();
    //@}

    void        setParameter(CParameter* AParameter);
    CParameter* getParameter() const;
  
    /// Cette méthode retourne le type du precompile.
    virtual TPrecompile getType() const;

    /// Modifie le paramètre PARAMETER_GMAP_VERTEX associé au précompile.
    void setGMapVertex(CParameterGMapVertex* AGMapV);

    /// Modifie le paramètre PARAMETER_NORMAL_VECTOR associé au précompile.
    void setNormalVector(CParameterNormalVector* ANormalVector);

    /// Modifie le paramètre PARAMETER_SELECTION associé au précompile.
    void setSelection(CParameterSelection* ASelection);

  protected:

    /// Cette méthode affiche tout les sommets de la carte.
    virtual void drawModel();

  private:
    void drawNormal(CDart*, bool ATwoSides);

    /// Les différents paramètres
    CParameterGMapVertex   * FParameterGMapV;
    CParameterNormalVector * FParameterNormalVector;
    CParameterSelection    * FParameterSelection;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PRECOMPILE_NORMAL_VECTOR_HH
//******************************************************************************
