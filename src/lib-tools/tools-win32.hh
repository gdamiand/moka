/*
 * lib-tools : Divers outils autour de lib-gmapkernel.
 * Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
 *               http://www.sic.sp2mi.univ-poitiers.fr/
 * Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
 *               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
 *
 * This file is part of lib-tools
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

#ifndef TOOLS_WIN32_HH
#define TOOLS_WIN32_HH

#ifdef WIN32

#ifndef WIN_EXPORT
#ifdef STATIC_LIB
#define WIN_EXPORT
#define WIN_IMPORT
#define WIN_EXTERN
#else // STATIC_LIB
#define WIN_EXPORT __declspec(dllexport)
#define WIN_IMPORT __declspec(dllimport)
#define WIN_EXTERN extern
#endif // STATIC_LIB
#endif // WIN_EXPORT

#ifndef DLL_TOOLS

#ifdef TOOLS_EXPORTS
#   define DLL_TOOLS WIN_EXPORT
#   define DLL_TOOLS_EXTERN  WIN_EXPORT extern
#   define DLL_TOOLS_EXTERNC extern "C" WIN_EXPORT
#else // TOOLS_EXPORTS
#   define DLL_TOOLS WIN_IMPORT
#   define DLL_TOOLS_EXTERN WIN_IMPORT extern
#   define DLL_TOOLS_EXTERNC extern "C" WIN_IMPORT
#endif // TOOLS_EXPORTS

#endif // DLL_TOOLS

#else // WIN32

#define DLL_TOOLS

#endif // WIN32

#endif // TOOLS_WIN32_HH
