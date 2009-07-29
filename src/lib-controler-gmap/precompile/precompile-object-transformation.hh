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
#ifndef PRECOMPILE_OBJECT_TRANSFORMATION_HH
#define PRECOMPILE_OBJECT_TRANSFORMATION_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include "precompile.hh"
//******************************************************************************
class CVertex;

namespace GMap3d
{
  class CParameterObjectTransformation;
  class CParameterObjectTranslation;
  class CParameterObjectRotation;
  class CParameterObjectScale;
  class CParameterObjectPonderation;

  /** La classe CPrecompileObjectTransformation
   */
  class CPrecompileObjectTransformation : public CPrecompile
  {
  public:
    //@name Constructeurs et destructeur.
    //@{
    /** Constructeur
     *
     * @param AParameterObjectTransformation les paramètres de dessin
     * @param AParameterObjectTranslation les paramètres de translation
     * @param AParameterObjectRotation les paramètres de rotation
     * @param AParameterObjectScale les paramètres de scaling
     * @param AParameterObjectPonderation les paramètres de pondération
     *
     */
    CPrecompileObjectTransformation(CParameterObjectTransformation*,
				    CParameterObjectTranslation*,
				    CParameterObjectRotation*,
				    CParameterObjectScale*,
				    CParameterObjectPonderation*);
    
    CPrecompileObjectTransformation(const CPrecompileObjectTransformation &);
    CPrecompile* copy() const;

    /// Destructeur
    virtual ~CPrecompileObjectTransformation();
    //@}

    //@{
    void        setParameter(CParameter* AParameter);
    CParameter* getParameter() const;
  
    /// Cette méthode retourne le type du precompile.
    virtual TPrecompile getType() const;

    /// Modifie le paramètre PARAMETER_OBJECT_TRANSFORMATION associé au
    ///  précompile.
    void setObjectTransformation(CParameterObjectTransformation*
				 AObjectTransformation);

    /// Modifie le paramètre PARAMETER_OBJECT_TRANSLATION associé au précompile.
    void setObjectTranslation(CParameterObjectTranslation* AObjectTranslation);

    /// Modifie le paramètre PARAMETER_OBJECT_ROTATION associé au précompile.
    void setObjectRotation(CParameterObjectRotation* AObjectRotation);

    /// Modifie le paramètre PARAMETER_OBJECT_SCALE associé au précompile.
    void setObjectScale(CParameterObjectScale* AObjectScale);

    /// Modifie le paramètre PARAMETER_OBJECT_PONDERATION associé au précompile.
    void setObjectPonderation(CParameterObjectPonderation* AObjectPonderation);
    //@}

    //@{
    void setCurrentMode(TMode AMode);
    //@}

  protected:

    /// Cette méthode affiche la / les grille(s).
    virtual void drawModel();

    /**
     * Pour afficher les pondérations (point, axe ou plan) quand c'est
     * nécessaire
     */
    void drawInteractivePonderation();

    /**
     * Pour afficher un axe donné par un sommet C, un vecteur V, un nombre
     * d'arêtes
     */
    void drawInteractiveAxe(const CVertex & C,
			    const CVertex & V,
			    int circleNbEdges,
			    bool ponderation=false);


  private:
    /// Les différents paramètres
    CParameterObjectTransformation * FParameterObjectTransformation;
    CParameterObjectTranslation    * FParameterObjectTranslation;
    CParameterObjectRotation 	   * FParameterObjectRotation;
    CParameterObjectScale 	   * FParameterObjectScale;
    CParameterObjectPonderation    * FParameterObjectPonderation;

    // Le mode courant pour différencier
    //    MODE_SCENE_TRANSLATION, MODE_SCENE_ROTATION ou MODE_SCENE_SCALE
    TMode FCurrentMode;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PRECOMPILE_OBJECT_TRANSFORMATION_HH
//******************************************************************************
