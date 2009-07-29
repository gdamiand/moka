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
#ifndef CONTROLER_TYPES_HH
#define CONTROLER_TYPES_HH
//******************************************************************************
// Nombre initial de view allouées dans un vector
#define INITIAL_NB_VIEWS (6)
//******************************************************************************
//@name Types de vues
//@{
typedef char TView;

enum
{
  FIRST_VIEW_CONTROLER = 0,
  
  VIEW_XYZ = FIRST_VIEW_CONTROLER,
  VIEW_ISO,
  VIEW_XY,
  VIEW_XZ,
  VIEW_YZ,
  
  LAST_VIEW_CONTROLER
};
//@}
//******************************************************************************
typedef unsigned int TViewId;
//******************************************************************************
typedef int TMode;

enum
{
  FIRST_MODE_CONTROLER = 0,
  
  MODE_DEFAULT = FIRST_MODE_CONTROLER,
  
  MODE_SCENE_TRANSLATION,
  MODE_SCENE_ROTATION,
  MODE_SCENE_SCALE,

  MODE_SELECTION,
  
  LAST_MODE_CONTROLER
};
//******************************************************************************
typedef int TModeOperation;

enum
{
  FIRST_MODE_OPERATION_CONTROLER = 0,
  
  MODE_OPERATION_NONE = FIRST_MODE_OPERATION_CONTROLER,

  MODE_OPERATION_SCENE_TRANSLATION,
  MODE_OPERATION_SCENE_ROTATION,
  MODE_OPERATION_SCENE_SCALE,
  
  MODE_OPERATION_SELECTION,
  
  LAST_MODE_OPERATION_CONTROLER
};
//******************************************************************************
//@name Types de listes précompilées
//@{
//------------------------------------------------------------------------------
typedef int TPrecompile;

enum
{
  FIRST_PRECOMPILE_CONTROLER= 0,
  
  PRECOMPILE_AIMED_POINT = FIRST_PRECOMPILE_CONTROLER,
  PRECOMPILE_AXIS,
  PRECOMPILE_GRID,

  LAST_PRECOMPILE_CONTROLER
};
//@}
//******************************************************************************
//@name Types de paramètres
typedef int TParameter;

enum
{
  FIRST_PARAMETER_CONTROLER = 0,
  
  // Paramètres précompiles
  PARAMETER_AIMED_POINT = FIRST_PARAMETER_CONTROLER,
  PARAMETER_AXIS,
  PARAMETER_GRID,
  
  // Paramètres de vues
  PARAMETER_AIMED_POSITION,
  PARAMETER_EYE_POSITION,
  PARAMETER_DRAWING,

  LAST_PARAMETER_CONTROLER
};
//******************************************************************************
typedef int TOperation;

enum
{
  FIRST_OPERATION_CONTROLER = 0,
  
  OPERATION_NONE = FIRST_OPERATION_CONTROLER,

  OPERATION_UNDO_REDO,

  LAST_OPERATION_CONTROLER
};
//------------------------------------------------------------------------------
typedef int TSubOperation;

enum
{
  FIRST_SUB_OPERATION_CONTROLER = 0,

  SUB_OPERATION_NONE = FIRST_SUB_OPERATION_CONTROLER,

  LAST_SUB_OPERATION_CONTROLER
};
//******************************************************************************
#endif // CONTROLER_TYPES_HH
//******************************************************************************
