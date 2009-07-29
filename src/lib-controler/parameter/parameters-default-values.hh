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
#ifndef PARAMETERS_DEFAULT_VALUES_HH
#define PARAMETERS_DEFAULT_VALUES_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 *  Ces constantes sont utilisées comme valeurs par defaut à la construction
 *  d'un paramètre ou lors de l'appel à la méthode réinit.
 */
//******************************************************************************

//
// Paramètres de vues
//

//------------------------------------------------------------------------------
// CParameterAimedPosition
#define DEFAULT_AIMED_POSITION_ROTATION  0
#define DEFAULT_AIMED_POSITION_LOOK_AT_X 0
#define DEFAULT_AIMED_POSITION_LOOK_AT_Y 0
#define DEFAULT_AIMED_POSITION_LOOK_AT_Z 0
//------------------------------------------------------------------------------
// CParameterDrawing
#define DEFAULT_DRAWING_LIGHT_COLOR_0       1.0
#define DEFAULT_DRAWING_LIGHT_COLOR_1       1.0
#define DEFAULT_DRAWING_LIGHT_COLOR_2       1.0
#define DEFAULT_DRAWING_BACKGROUND_COLOR_0  0.8
#define DEFAULT_DRAWING_BACKGROUND_COLOR_1  0.8
#define DEFAULT_DRAWING_BACKGROUND_COLOR_2  0.8
#define DEFAULT_DRAWING_DISPLAY_LIGHT       true
//------------------------------------------------------------------------------
// CParameterEyePosition
#define DEFAULT_EYE_POSITION_ROTATION_ALPHA  35
#define DEFAULT_EYE_POSITION_ROTATION_BETA  -45
#define DEFAULT_EYE_POSITION_DISTANCE        15
#define DEFAULT_EYE_POSITION_PAS_AVANCEMENT  1
#define DEFAULT_EYE_POSITION_ANGLE_ROTATION  4
//------------------------------------------------------------------------------

//
// Paramètres précompiles
//

//------------------------------------------------------------------------------
// CParameterAimedPoint
#define DEFAULT_AIMED_POINT_LINE_WIDTH 1
#define DEFAULT_AIMED_POINT_LENGTH     0.2
#define DEFAULT_AIMED_POINT_COLOR_0    0.5
#define DEFAULT_AIMED_POINT_COLOR_1    0.5
#define DEFAULT_AIMED_POINT_COLOR_2    0.5
//------------------------------------------------------------------------------
// CParameterAxis
#define DEFAULT_AXIS_LINE_WIDTH 2
#define DEFAULT_AXIS_LENGTH     1.0
#define DEFAULT_AXIS_COLOR_X_0  1.0
#define DEFAULT_AXIS_COLOR_X_1  0.0
#define DEFAULT_AXIS_COLOR_X_2  0.0
#define DEFAULT_AXIS_COLOR_Y_0  0.0
#define DEFAULT_AXIS_COLOR_Y_1  0.6
#define DEFAULT_AXIS_COLOR_Y_2  0.0
#define DEFAULT_AXIS_COLOR_Z_0  0.0
#define DEFAULT_AXIS_COLOR_Z_1  0.0
#define DEFAULT_AXIS_COLOR_Z_2  1.0
//------------------------------------------------------------------------------
// CParameterGrid
#define DEFAULT_GRID_LINE_WIDTH  1
#define DEFAULT_GRID_SIZE        20
#define DEFAULT_GRID_DISPLAY_XY  true
#define DEFAULT_GRID_DISPLAY_YZ  false
#define DEFAULT_GRID_DISPLAY_XZ  false
#define DEFAULT_GRID_POSITIVE_X  false
#define DEFAULT_GRID_POSITIVE_Y  false
#define DEFAULT_GRID_POSITIVE_Z  false
#define DEFAULT_GRID_COLOR_0     0.7
#define DEFAULT_GRID_COLOR_1     0.7
#define DEFAULT_GRID_COLOR_2     0.7

//******************************************************************************
#endif // PARAMETERS_DEFAULT_VALUES_HH
//******************************************************************************
