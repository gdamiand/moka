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
#ifndef CONTROLER_HH
#define CONTROLER_HH
//******************************************************************************
#include "vertex.hh"
#include "controler-types.hh"
#include "operations.hh"

#include "parameters-default-values.hh"
#include "parameter-aimed-point.hh"
#include "parameter-drawing.hh"
#include "parameter-eye-position.hh"

#include "parameter-aimed-position.hh"
#include "parameter-axis.hh"
#include "parameter-grid.hh"

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
class CTransformationMatrix;

class CViewPrecompile;

class CControler
{
public:
  /* @name Constructeur et destructeur
   */

  //@{
  
  CControler(int ANb, const std::string & AName,
	     const std::string & ADirectory = "");
  virtual ~CControler();

  //@}

  /// @name Méthodes auxiliaires
  //@{
#include "controler-protected-methods.hh"
  //@}

  /// @name Méthodes de paramétrage et d'accès aux vues
  //@{
#include "controler-views.hh"
  //@}
  
  /// @name Récupération des paramètres
  //@{
#include "controler-parameter.hh"
  //@}

  /// @name Évènements clavier et souris
  //@{
#include "controler-input-events.hh"
  //@}

  /// @name Gestion générique du mode sélection
  //@{
#include "controler-selection.hh"
    //@}

  /// @name Méthodes undo/redo
  //@{
#include "controler-undo-redo.hh"
  //@}

  /// @name Opérations
  //@{
#include "controler-operations.hh"
  //@}

  /// @name Modification de la scène
  //@{
#include "controler-scene-transformation.hh"
  //@}  
};

//******************************************************************************
#endif // CONTROLER_HH
//******************************************************************************
