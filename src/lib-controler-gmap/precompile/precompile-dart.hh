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
#ifndef PRECOMPILE_DART_HH
#define PRECOMPILE_DART_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include "precompile.hh"
//******************************************************************************
namespace GMap3d
{
  class CDart;
  class CParameterGMapVertex;
  class CParameterDart;
  class CParameterSelection;

  /** La classe CPrecompileDart permet l'affichage de tout les brins d'une
   *  carte. Si le paramètre CParameterSelection n'est pas NULL, alors affiche
   *  différemment les brins sélectionné et le last de la sélection courante
   *  sinon tout les brins sont considérés non-sélectionnés.
   */
  class CPrecompileDart : public CPrecompile
  {
  public:
    //@name Constructeurs et destructeur.
    //@{
    /** Constructeur
     *
     * @param AParameterGmapV les paramètre de g-map-vertex.
     * @param AParameterDart les paramètres de dessin des brins
     * @param AParameterSelection les paramètres de sélection.
     *
     * Les paramètres AParameterGmapV, AParameterDart sont obligatoires,
     * tandis que AParameterSelection peut-être NULL.
     * Dans ce cas, tout les brins sont considéres non sélectionné
     *
     */
    CPrecompileDart(CParameterGMapVertex *,
		    CParameterDart       *,
		    CParameterSelection  *);
    
    CPrecompileDart(const CPrecompileDart &);
    CPrecompile* copy() const;

    /// Destructeur
    virtual ~CPrecompileDart();
    //@}

    void        setParameter(CParameter* AParameter);
    CParameter* getParameter() const;

    /// Cette méthode retourne le type du precompile.
    virtual TPrecompile getType() const;

    /// Modifie le paramètre PARAMETER_GMAP_VERTEX associé au précompile.
    void setGMapVertex(CParameterGMapVertex* AParameterGMapV);

    /// Modifie le paramètre PARAMETER_DART associé au précompile.
    void setDart(CParameterDart* ADart);

    /// Modifie le paramètre PARAMETER_SELECTION associé au précompile.
    void setSelection(CParameterSelection* ASelection);

    /// Cette méthode est appelée pour la sélection d'un brin à la position x,y.
    virtual void pick(int, int, CView*);

  protected:

    /// Used in simplification mode: return the incident dart for ADim
    /// by using connecting pathes.
    CDart* nextDartAlpha(CDart* ADart, unsigned int ADim);
    
    /** drawOneDart
     *
     *  @param ADart le brin à afficher
     *
     *  Cette méthode affiche tout le brin de la carte passé en paramètre,
     *  sans aucune modification de couleur, largeur de trait...
     */
    void drawOneDart(CDart*);

    /** Draw the edge associated with ADart. In simplification mode this is
     *  the connecting path between ADart and its alpha0.
     *  Otherwise, this is the dart.
     */
    void drawOneEdge(CDart * ADart);
    
    /// Cette méthode affiche tout les brins de la carte.
    virtual void drawModel();

  private:
    /// Les différents paramètres
    CParameterGMapVertex * FParameterGMapV;
    CParameterDart       * FParameterDart;
    CParameterSelection  * FParameterSelection;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PRECOMPILE_DART_HH
//******************************************************************************
