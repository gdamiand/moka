#-------------------------------------------------------------------------------
# Moka : Un modeleur de 3-G-cartes.
# Copyright (C) 2004, Moka Team, Université de Poitiers, Laboratoire SIC
#               http://www.sic.sp2mi.univ-poitiers.fr/
# Copyright (C) 2009, Guillaume Damiand, CNRS, LIRIS,
#               guillaume.damiand@liris.cnrs.fr, http://liris.cnrs.fr/
#
# This file is part of Moka
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 2.6.2)
project(moka)

################################################################################
# Par défaut, il y a 2 types de distributions :
#   (a spécifier lorsqu'on exécute cmake, par exemple :
#	 cd debug; cmake ../src/ -DCMAKE_BUILD_TYPE=Debug)
# Debug       = non inline + assert 
# Release     = inline     + no-assert + O3

#SET(CMAKE_CXX_FLAGS_DEBUG "-g")
#SET(CMAKE_CXX_FLAGS_RELEASE "-DINLINE_FLAG -O3 -finline-functions -DNDEBUG")

#-------------------------------------------------------------------------------
# Des options que l'on peut activer ou désactiver selon les besoins.
# L'option par défaut est mise en majuscule.
#
#-------------------------------------------------------------------------------
# Pour utiliser des librairies dynamique (on) ou statique (OFF)
OPTION(BUILD_SHARED_LIBS "Build shared libraries." OFF)

#-------------------------------------------------------------------------------
# Les modules à activer ou désactiver
OPTION(MODULE_MESH "Activate mesh module" ON)
OPTION(MODULE_ROUNDING "Activate rounding module" ON)
OPTION(MODULE_COREFINEMENT "Activate corefining module" ON)
OPTION(MODULE_SPAMOD "Activate spamod module" OFF)

#-------------------------------------------------------------------------------
# Des options de compilation que l'on peut activer avec ADD_DEFINITIONS(xxx).

# -DNO_ACCENTED_CHARS pour ne pas utiliser de caractères accentués;
# -DNO_LIST_OPENGL pour ne pas utiliser les listes openGl précompilés;

ADD_DEFINITIONS("-Wall")
ADD_DEFINITIONS("-g")

################################################################################
# Pour savoir comment gérer les icc : ils sont soit inclus dans les
# .hh en mode INLINE, soit dans les .cc en mode NONINLINE.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   set_property(DIRECTORY PROPERTY
		IMPLICIT_DEPENDS_INCLUDE_TRANSFORM 
		"INCLUDE_NON_INLINE(%)=%;INCLUDE_HEADER(%)=%")
   message( STATUS "**** MODE DEBUG ****")
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
   set_property(DIRECTORY PROPERTY
		IMPLICIT_DEPENDS_INCLUDE_TRANSFORM 
   		"INCLUDE_INLINE(%)=%")   
   message( STATUS "***** MODE RELEASE ****" )
   ADD_DEFINITIONS(-DINLINE_FLAG)
endif (CMAKE_BUILD_TYPE STREQUAL  "Debug")

#------------------------------------------------------------------------------
# Les includes pour gmapkernel
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/g-map)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/g-map-generic)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/g-map-vertex)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/tools/geometry)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/tools/streams)
#------------------------------------------------------------------------------
# Les includes pour tools
include_directories (${CMAKE_SOURCE_DIR}/src/lib-tools)
#------------------------------------------------------------------------------
# Les includes pour controler
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/parameter)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/parameter/parameter-precompile)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/parameter/parameter-view)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/precompile)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/view)
#-------------------------------------------------------------------------------
# Les includes pour controler-gmap
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/operations)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/parameter)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/parameter/parameter-controler)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/parameter/parameter-precompile)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/precompile)
#-------------------------------------------------------------------------------
# Pour positionner les flags de compilation en fonction des modules
if (MODULE_COREFINEMENT)
  add_definitions(-DMODULE_COREFINEMENT)

  # Les includes pour corefinement
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-corefinement)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-corefinement/2d-propagation)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-corefinement/2d-sweeping)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-corefinement/3d)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-corefinement/common)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-corefinement/interface)

  add_subdirectory (src/lib-corefinement lib-corefinement)
endif (MODULE_COREFINEMENT)
  
if (MODULE_MESH)
  add_definitions(-DMODULE_MESH)

  # Les includes pour mesh
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-mesh)

  add_subdirectory (src/lib-mesh lib-mesh)
endif (MODULE_MESH)
#------------------------------------------------------------------------------- 
if (MODULE_ROUNDING)
  add_definitions(-DMODULE_ROUNDING)

  # Les includes pour rounding
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-rounding)

  add_subdirectory (src/lib-rounding lib-rounding)
endif (MODULE_ROUNDING)

#-------------------------------------------------------------------------------
if (MODULE_SPAMOD)
  add_definitions(-DMODULE_SPAMOD)

  # Les includes pour spamod
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod/main)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod/main/attributes)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod/main/basic)

  add_subdirectory (src/lib-spamod lib-spamod)
endif (MODULE_SPAMOD)

if ( MODULE_COREFINEMENT )
  # Les includes pour tools
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-tools)

  add_subdirectory (src/lib-tools lib-tools)
endif ( MODULE_COREFINEMENT )

#-------------------------------------------------------------------------------
add_subdirectory (src/lib-gmapkernel lib-gmapkernel)
add_subdirectory (src/lib-controler lib-controler)
add_subdirectory (src/lib-controler-gmap lib-controler-gmap)
add_subdirectory (src/mokaQtIhm mokaQtIhm)

add_subdirectory (src/example-basic example-basic EXCLUDE_FROM_ALL)

#-------------------------------------------------------------------------------

