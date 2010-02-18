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
#ifndef PARAMETERS_GMAP_DEFAULT_VALUES_HH
#define PARAMETERS_GMAP_DEFAULT_VALUES_HH
//******************************************************************************
/** @author Guillaume Damiand, Frédéric Vidil
 *  Ces constantes sont utilisées comme valeurs par defaut à la construction
 *  d'un paramètre ou lors de l'appel à la méthode réinit.
 */
//******************************************************************************

#include "parameters-default-values.hh"

// Répertoire par defaut pour le controleur.
#define CONTROLER_GMAP_DIRECTORY ".moka"

//
// Paramètres précompiles
//

//------------------------------------------------------------------------------
// CParameterDart
#define DEFAULT_DART_LINE_WIDTH    2
#define DEFAULT_DART_SIZE_ALONE    9
#define DEFAULT_DART_COUL_1UNSEL_0 0.0
#define DEFAULT_DART_COUL_1UNSEL_1 0.0
#define DEFAULT_DART_COUL_1UNSEL_2 0.0
#define DEFAULT_DART_COUL_2UNSEL_0 0.0
#define DEFAULT_DART_COUL_2UNSEL_1 0.0
#define DEFAULT_DART_COUL_2UNSEL_2 0.0
#define DEFAULT_DART_COUL_3UNSEL_0 0.0
#define DEFAULT_DART_COUL_3UNSEL_1 0.0
#define DEFAULT_DART_COUL_3UNSEL_2 0.0
#define DEFAULT_DART_COUL_nUNSEL_0 0.0
#define DEFAULT_DART_COUL_nUNSEL_1 0.0
#define DEFAULT_DART_COUL_nUNSEL_2 0.0
#define DEFAULT_DART_COUL_1SEL_0   1.0
#define DEFAULT_DART_COUL_1SEL_1   0.3
#define DEFAULT_DART_COUL_1SEL_2   0.0
#define DEFAULT_DART_COUL_2SEL_0   0.0
#define DEFAULT_DART_COUL_2SEL_1   0.6
#define DEFAULT_DART_COUL_2SEL_2   0.0
#define DEFAULT_DART_COUL_3SEL_0   1.0
#define DEFAULT_DART_COUL_3SEL_1   0.0
#define DEFAULT_DART_COUL_3SEL_2   1.0
#define DEFAULT_DART_COUL_nSEL_0   1.0
#define DEFAULT_DART_COUL_nSEL_1   0.0
#define DEFAULT_DART_COUL_nSEL_2   1.0
#define DEFAULT_DART_COUL_1LAST_0  1.0
#define DEFAULT_DART_COUL_1LAST_1  1.0
#define DEFAULT_DART_COUL_1LAST_2  0.0
#define DEFAULT_DART_COUL_2LAST_0  1.0
#define DEFAULT_DART_COUL_2LAST_1  1.0
#define DEFAULT_DART_COUL_2LAST_2  0.0
#define DEFAULT_DART_COUL_3LAST_0  1.0
#define DEFAULT_DART_COUL_3LAST_1  1.0
#define DEFAULT_DART_COUL_3LAST_2  0.0
#define DEFAULT_DART_COUL_nLAST_0  1.0
#define DEFAULT_DART_COUL_nLAST_1  1.0
#define DEFAULT_DART_COUL_nLAST_2  0.0
#define DEFAULT_DART_COUL_0REMOVE1_0  0.4
#define DEFAULT_DART_COUL_0REMOVE1_1  0.4
#define DEFAULT_DART_COUL_0REMOVE1_2  0.8
#define DEFAULT_DART_COUL_0REMOVE2_0  0.4
#define DEFAULT_DART_COUL_0REMOVE2_1  0.4
#define DEFAULT_DART_COUL_0REMOVE2_2  0.8
#define DEFAULT_DART_COUL_0REMOVE3_0  0.4
#define DEFAULT_DART_COUL_0REMOVE3_1  0.4
#define DEFAULT_DART_COUL_0REMOVE3_2  0.8
#define DEFAULT_DART_COUL_0REMOVEn_0  0.4
#define DEFAULT_DART_COUL_0REMOVEn_1  0.4
#define DEFAULT_DART_COUL_0REMOVEn_2  0.8
#define DEFAULT_DART_COUL_1REMOVE1_0  0.65
#define DEFAULT_DART_COUL_1REMOVE1_1  0.65
#define DEFAULT_DART_COUL_1REMOVE1_2  0.65
#define DEFAULT_DART_COUL_1REMOVE2_0  0.65
#define DEFAULT_DART_COUL_1REMOVE2_1  0.65
#define DEFAULT_DART_COUL_1REMOVE2_2  0.65
#define DEFAULT_DART_COUL_1REMOVE3_0  0.65
#define DEFAULT_DART_COUL_1REMOVE3_1  0.65
#define DEFAULT_DART_COUL_1REMOVE3_2  0.65
#define DEFAULT_DART_COUL_1REMOVEn_0  0.65
#define DEFAULT_DART_COUL_1REMOVEn_1  0.65
#define DEFAULT_DART_COUL_1REMOVEn_2  0.65
//------------------------------------------------------------------------------
// CParameterFace
#define DEFAULT_FACE_COLOR_0      0.5
#define DEFAULT_FACE_COLOR_1      0.7
#define DEFAULT_FACE_COLOR_2      0.9 
#define DEFAULT_FACE_TRANSPARENCY 0.9
#define DEFAULT_FACE_RANDOM_CL_FACE   false
#define DEFAULT_FACE_RANDOM_CL_VOLUME true
#define DEFAULT_FACE_RANDOM_GEOMETRY  false
//------------------------------------------------------------------------------
// CParameterNormalVector
#define DEFAULT_NORMAL_VECTOR_ALL        false
#define DEFAULT_NORMAL_VECTOR_LINE_WIDTH 2
#define DEFAULT_NORMAL_VECTOR_LENGTH     1.0
#define DEFAULT_NORMAL_VECTOR_COLOR_0    1.0
#define DEFAULT_NORMAL_VECTOR_COLOR_1    0.5
#define DEFAULT_NORMAL_VECTOR_COLOR_2    0.0
//------------------------------------------------------------------------------
//CParameterObjectTransformation
#define DEFAULT_OBJECT_TRANSFORMATION_LINE_WIDTH 2
#define DEFAULT_OBJECT_TRANSFORMATION_COLOR_0    1.0
#define DEFAULT_OBJECT_TRANSFORMATION_COLOR_1    1.0
#define DEFAULT_OBJECT_TRANSFORMATION_COLOR_2    1.0
//------------------------------------------------------------------------------
// CParameterPreview
#define DEFAULT_PREVIEW_LINE_WIDTH       2
#define DEFAULT_PREVIEW_LINE_WIDTH_POINT 9
#define DEFAULT_PREVIEW_COLOR_0          1.0
#define DEFAULT_PREVIEW_COLOR_1          1.0
#define DEFAULT_PREVIEW_COLOR_2          1.0
#define DEFAULT_PREVIEW_COLOR_BAD_0      0.3
#define DEFAULT_PREVIEW_COLOR_BAD_1      0.3
#define DEFAULT_PREVIEW_COLOR_BAD_2      0.3
//------------------------------------------------------------------------------
// CParameterSew
#define DEFAULT_SEW0_LINE_WIDTH 1
#define DEFAULT_SEW1_LINE_WIDTH 1
#define DEFAULT_SEW2_LINE_WIDTH 1
#define DEFAULT_SEW3_LINE_WIDTH 1
#define DEFAULT_SEW0_POSITION 1.0
#define DEFAULT_SEW1_POSITION 0.2
#define DEFAULT_SEW2_POSITION 0.9
#define DEFAULT_SEW3_POSITION 0.0
#define DEFAULT_SEW0_DISPLAY false
#define DEFAULT_SEW1_DISPLAY false
#define DEFAULT_SEW2_DISPLAY true
#define DEFAULT_SEW3_DISPLAY true
#define DEFAULT_SEW0_COLOR_0 1.0
#define DEFAULT_SEW0_COLOR_1 1.0
#define DEFAULT_SEW0_COLOR_2 1.0
#define DEFAULT_SEW1_COLOR_0 1.0
#define DEFAULT_SEW1_COLOR_1 0.0
#define DEFAULT_SEW1_COLOR_2 0.0
#define DEFAULT_SEW2_COLOR_0 0.0
#define DEFAULT_SEW2_COLOR_1 0.5
#define DEFAULT_SEW2_COLOR_2 0.0
#define DEFAULT_SEW3_COLOR_0 0.0
#define DEFAULT_SEW3_COLOR_1 0.0
#define DEFAULT_SEW3_COLOR_2 1.0
//------------------------------------------------------------------------------
// CParameterVertex
#define DEFAULT_VERTEX_SIZE  5
#define DEFAULT_VERTEX_COUL0 1.0
#define DEFAULT_VERTEX_COUL1 1.0
#define DEFAULT_VERTEX_COUL2 1.0
//------------------------------------------------------------------------------

//******************************************************************************
#endif // PARAMETERS_GMAP_DEFAULT_VALUES_HH
//******************************************************************************
