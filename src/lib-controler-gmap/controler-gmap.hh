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
#ifndef CONTROLER_GMAP_HH
#define CONTROLER_GMAP_HH
//******************************************************************************
#include "kernel-types.hh"
#include "g-map-vertex.hh"
#include "vertex.hh"
#include "controler.hh"
#include "controler-types.hh"
#include "operations.hh"

#include "parameters-gmap-default-values.hh"
#include "parameter-gmap-vertex.hh"
#include "parameter-creation.hh"
#include "parameter-object-ponderation.hh"
#include "parameter-object-position.hh"
#include "parameter-object-rotation.hh"
#include "parameter-object-scale.hh"
#include "parameter-object-translation.hh"
#include "parameter-operations.hh"
#include "parameter-polyline-position.hh"
#include "parameter-selection.hh"

#include "parameter-aimed-position.hh"
#include "parameter-axis.hh"
#include "parameter-dart.hh"
#include "parameter-face.hh"
#include "parameter-grid.hh"
#include "parameter-preview.hh"
#include "parameter-normal-vector.hh"
#include "parameter-object-transformation.hh"
#include "parameter-sew.hh"
#include "parameter-vertex.hh"

#ifdef MODULE_SPAMOD
#include "parameter-spamod.hh"
#include "spamod-api.hh"
#endif // MODULE_SPAMOD

#ifdef MODULE_ROUNDING
#include "rounding-interface.hh"
#endif // MODULE_ROUNDING

#ifdef _WINDOWS
#include <windows.h>
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <cstdio>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

//******************************************************************************
#define NB_SELECTION_LEVELS (3)
//******************************************************************************

//******************************************************************************
class CTransformationMatrix;

class CViewPrecompile;

namespace GMap3d
{
  class CDart;
  class CGMapVertex;

#ifdef MODULE_SPAMOD
  class CParameterSpamod;
#endif // MODULE_SPAMOD

  class CControlerGMap : public CControler
  {
  public:
    /* @name Constructeur et destructeur
     */

    //@{
  
    CControlerGMap(CGMapVertex* AGMapVertex = NULL,
		   const std::string & ADirectory = "");
    virtual ~CControlerGMap();

    CGMapVertex* getMap();
    //@}

    /// @name Création d'objets
    //@{
#include "controler-gmap-creations.hh"
    //@}

    /// @name Modifications d'objets
    //@{
#include "controler-gmap-object-transformation.hh"
    //@}

    /// @name Mode chanfreinage
    //@{
#include "controler-gmap-rounding.hh"
    //@}

    /// @name Récupération des paramètres
    //@{
#include "controler-gmap-parameter.hh"
    //@}

    /// @name Opérations
    //@{
#include "controler-gmap-operations.hh"
    //@}

    /// @name Évènements clavier et souris
    //@{
#include "controler-gmap-input-events.hh"
    //@}

    /* @name Méthodes de paramétrage et d'accès aux vues
     */
    //@{
#include "controler-gmap-views.hh"
    //@}

    /* @name Méthodes pour l'export Xfig
     */
    //@{
#include "controler-gmap-export-xfig.hh"
    //@}

    /// @name Méthodes undo/redo
    //@{
#include "controler-gmap-undo-redo.hh"
    //@}

    /* @name Sélection de brins
     */
    //@{
#include "controler-gmap-selection.hh"
    //@}

    /// @name Méthodes auxiliaires
    //@{
#include "controler-gmap-protected-methods.hh"
    //@}

    /// @name Champs privés
    //@{

  protected:
    /// The generalized map.
    CGMapVertex* FMap;

  private:
    /// Le paramètre contenant la carte.
    CParameterGMapVertex       * FParameterGMapVertex;

    CParameterSelection        * FParameterSelection;
    CParameterCreation	       * FParameterCreation;
    CParameterPolylinePosition * FParameterPolylinePosition;
    CParameterOperations       * FParameterOperations;
    CParameterObjectPosition   * FParameterObjectPosition[NB_PREDEFINED_OBJECTS];
    CParameterObjectTranslation* FParameterTranslation;
    CParameterObjectRotation   * FParameterRotation;
    CParameterObjectScale      * FParameterScale;
    CParameterObjectPonderation* FParameterPonderation;

    //@}
  };

} // namespace GMap3d
//******************************************************************************
#endif // CONTROLER_HH
//******************************************************************************
