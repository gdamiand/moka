/*
 * lib-controler : Un contrôleur générique de scène 3D.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-controler
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
#ifndef VIEW_PRECOMPILE_PERSPECTIVE_HH
#define VIEW_PRECOMPILE_PERSPECTIVE_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 */

//******************************************************************************
#include "view-precompile.hh"
#include "vertex.hh"
#include <list>
//******************************************************************************

  class CPrecompile;
  class CPrecompileAimedPoint;
  class CParameterEyePosition;
  class CParameterAimedPosition;
  class CParameterDrawing;

  /** La classe CViewPrecompilePerspective est une implémentation de
   *  l'interface CView.
   *  Elle permet l'affichage éclatée en OpenGL d'une liste de précompilés.
   *  La position de la caméra est fixée par CParameterScenePosition, et
   *  les paramètres d'affichage (couleur du fond, de la lumière...) par
   *  CParameterDrawing.
   */
  class CViewPrecompilePerspective : public CViewPrecompile
  {
  public:
    /// Contructeur pour la vue Perspective :
    CViewPrecompilePerspective(CParameterEyePosition *,
			       CParameterAimedPosition *,
			       CParameterDrawing *);

    virtual ~CViewPrecompilePerspective();

  protected:

    /** setScenePosition
     *
     *  Cette méthode positionne les différents paramètres de visualisation
     * (position de la caméra, direction...)
     */
    virtual void setScenePosition();
  };
//******************************************************************************
#endif // VIEW_PRECOMPILE_PERSPECTIVE_HH
//******************************************************************************
