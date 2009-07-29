/*
 * lib-gmapkernel : Un noyau de 3-G-cartes et des opérations.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-gmapkernel
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
#ifndef INLINE_MACRO_HH
#define INLINE_MACRO_HH
//******************************************************************************

#ifdef INLINE_FLAG

// Compilation inline : les .icc sont inclus dans les .hh et pas dans les .cc
#define INLINE inline
#define INCLUDE_HEADER(file) "inline-macro.hh"
#define INCLUDE_INLINE(file) file
#define INCLUDE_NON_INLINE(file) "inline-macro.hh"

#else

// Compilation non-inline : c'est l'inverse
#define INLINE
#define INCLUDE_HEADER(file) file
#define INCLUDE_INLINE(file) "inline-macro.hh"
#define INCLUDE_NON_INLINE(file) file

#endif // INLINE_FLAG

//******************************************************************************
#endif // INLINE_MACRO_HH
//******************************************************************************
