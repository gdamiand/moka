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
#ifndef PRECOMPILE_PREVIEW_HH
#define PRECOMPILE_PREVIEW_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */
//******************************************************************************
#include "precompile.hh"
//******************************************************************************
class CVertex;
class CTransformationMatrix;

namespace GMap3d
{
  class CParameterPreview;
  class CParameterObjectPosition;
  class CParameterPolylinePosition;
  class CParameterCreation;

  /** La classe CPrecompilePreview permet l'affichage interactif des objets
   *  en cours de création.
   */
  class CPrecompilePreview : public CPrecompile
  {
  public:
    //@name Constructeurs et destructeur.
    //@{
    /** Constructeur
     *
     * @param APreview les paramètres de dessin (couleur...)
     * @param AObjectPosition les paramètres de position d'objets
     * @param AObjectParameters les paramètres de création d'objets
     * @param APolylineParameter les paramètres de position polyline
     *
     */
    CPrecompilePreview(CParameterPreview          * APreview,
		       CParameterObjectPosition   * AObjectPosition,
		       CParameterCreation         * AObjectParameters,
		       CParameterPolylinePosition * APolylinePosition);
    CPrecompilePreview(const CPrecompilePreview &);
    CPrecompile* copy() const;

    /// Destructeur
    virtual ~CPrecompilePreview();
    //@}

    /// Cette méthode retourne le type du precompile.
    virtual TPrecompile getType() const;

    //@{
    /**
     *  Modifie le paramètre de type AParameter->getType() si c'est un paramètre
     *  connu de ce précompile, sinon ne fait rien.
     */
    void        setParameter(CParameter* AParameter);
    CParameter* getParameter() const;
  
    /// Modifie le paramètre PARAMETER_PREVIEW associé au précompile.
    void setPreview(CParameterPreview* APreview);

    /// Modifie le paramètre PARAMETER_OBJECT_POSITION associé au précompile.
    void setPosition(CParameterObjectPosition* APosition);

    /// Modifie le paramètre PARAMETER_CREATION associé au précompile.
    void setCreation(CParameterCreation* ACreation);

    /// Modifie le paramètre PARAMETER_POLYLINE_POSITION associé au précompile.
    void setPolylinePosition(CParameterPolylinePosition* APolylinePosition);

    //@}

    //@{
    void setCurrentMode(TMode AMode);
    //@}

  protected:

    /// Cette méthode affiche l'objet en cours de création.
    virtual void drawModel();

    //@{ drawXXX
    /*
     *  Affichage de l'objet de type XXX qui est en cours de création.
     */
    void drawPolyline();
    void drawPolygon();
    void drawMesh();
    void drawSphere();
    void drawCylinder();
    void drawPyramid();
    void drawTorus();
    //@}

    //@{ Méthodes utilisées par les méthodes drawXXX

    /** drawMesh1
     *
     *  Affichage d'un segment maillé.
     */
    void drawMesh1(const CTransformationMatrix & AMatrix,
		   const CVertex & AOrigin, const CVertex & AVector, int ASx);

    /** drawMesh2
     *
     *  Affichage d'une grille.
     */
    void drawMesh2(const CTransformationMatrix & AMatrix,
		   const CVertex & AOrigin,
		   const CVertex & AVectorX, int ASx,
		   const CVertex & AVectorY, int ASy);

    /** drawMesh2
     *
     *  Affichage d'un cube maillé en dimension 3.
     */
    void drawMesh3(const CTransformationMatrix & AMatrix,
		   const CVertex & AOrigin,
		   const CVertex & AVectorX, int ASx,
		   const CVertex & AVectorY, int ASy,
		   const CVertex & AVectorZ, int ASz);

    //@}

  private:
    /// Les différents paramètres
    CParameterPreview          * FParameterPreview;
    CParameterObjectPosition   * FParameterPosition;
    CParameterCreation 	       * FParameterCreation;
    CParameterPolylinePosition * FParameterPolylinePosition;

    /// Le mode courant pour différencier
    //    MODE_CREATION_CYLINDER, MODE_CREATION_MESH, MODE_CREATION_POLYGON,
    //    MODE_CREATION_POLYLINE, MODE_CREATION_PYRAMID,
    //    MODE_CREATION_SPHERE et MODE_CREATION_TORUS
    TMode FCurrentMode;
  };

} // namespace GMap3d
//******************************************************************************
#endif // PRECOMPILE_PREVIEW_HH
//******************************************************************************
