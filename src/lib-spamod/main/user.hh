/*
 * lib-spamod : Visualisation des objets en discret.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-spamod
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
// file <user.h>
// for defining special types
//
// Date : 06.19.2000
//


#ifndef _USER_H_
#define _USER_H_




//******************************************************************************
// User defined types identificator, which must be returned
// All the used id sed by the program must be defined here
// The given value cannot be greater than 255 (TAttributeId length)
//
#define    VERTEX_ATTRIBUTE_ID      1
#define    COLOR_ATTRIBUTE_ID       15
#define    VECTOR2D_ATTRIBUTE_ID    17
#define    INEQ2_ATTRIBUTE_ID      18
#define    INEQ6_ATTRIBUTE_ID      19
#define    PT_LIST_ATTRIBUTE_ID     20
#define    USE_ATTRIBUTE_ID         21
#define    INT_ATTRIBUTE_ID         22
#define    VOXEL_LIST_ATTRIBUTE_ID  23
#define    MATRIX_ATTRIBUTE_ID      24
#define    COLOR_TABLE_ATTRIBUTE_ID 25


#endif // _USER_H_
