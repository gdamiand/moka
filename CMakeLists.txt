#-------------------------------------------------------------------------------
# Moka : Un modeleur de 3-G-cartes.
#

cmake_minimum_required(VERSION 2.6.2)
project(moka)

################################################################################
# By default, there are two possible build type:
#   (fix when executing cmake, for example:
#	 cd debug; cmake ../src/ -DCMAKE_BUILD_TYPE=Debug)
# Debug       = non inline + assert 
# Release     = inline     + no-assert + O3

#SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
#SET(CMAKE_CXX_FLAGS_RELEASE "-DINLINE_FLAG -O3 -finline-functions -DNDEBUG")

# If like me you have some problems under windows, set these variable to
# the correct pathes and file.
IF (WIN32)
  SET(GLUT_INCLUDE_DIR "C:/Program Files/Microsoft SDKs/Windows/v6.1/Include/")
  SET(QTDIR "C:/Qt/qt")
  SET(QT_QMAKE_EXECUTABLE "C:/Qt/qt/bin/qmake")
ENDIF (WIN32)

#-------------------------------------------------------------------------------
# Some options that we can enable or disable. The default is in uppercase.
#
#-------------------------------------------------------------------------------
# To use dynamic libraires (on) or static (OFF)
OPTION(BUILD_SHARED_LIBS "Build shared libraries." OFF)

#-------------------------------------------------------------------------------
# Some modules that we can enable or disable.
OPTION(MODULE_MESH "Activate mesh module" ON)
OPTION(MODULE_ROUNDING "Activate rounding module" ON)
OPTION(MODULE_COREFINEMENT "Activate corefining module" ON)
OPTION(MODULE_SPAMOD "Activate spamod module" OFF)

#-------------------------------------------------------------------------------
# Some compiler options that we can enable with some ADD_DEFINITIONS(xxx).

# -DNO_ACCENTED_CHARS to avoid accented char.
# -DNO_LIST_OPENGL to not use precompile openGl list.

################################################################################
# To manage correctlty icc files : they are either included in .hh files
# in INLINE mode, or included in .cc file in NONINLINE mode.
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
# Includes for gmapkernel
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/g-map)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/g-map-generic)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/g-map-vertex)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/tools/geometry)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-gmapkernel/tools/streams)
#------------------------------------------------------------------------------
# Includes for tools
include_directories (${CMAKE_SOURCE_DIR}/src/lib-tools)
#------------------------------------------------------------------------------
# Includes for controler
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/parameter)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/parameter/parameter-precompile)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/parameter/parameter-view)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/precompile)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler/view)
#-------------------------------------------------------------------------------
# Includes for controler-gmap
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/operations)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/parameter)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/parameter/parameter-controler)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/parameter/parameter-precompile)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-controler-gmap/precompile)
#-------------------------------------------------------------------------------
# Some specific options depending on the enabled modules.
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

  # Includes for mesh
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-mesh)

  add_subdirectory (src/lib-mesh lib-mesh)
endif (MODULE_MESH)
#------------------------------------------------------------------------------- 
if (MODULE_ROUNDING)
  add_definitions(-DMODULE_ROUNDING)

  # Includes for rounding
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-rounding)

  add_subdirectory (src/lib-rounding lib-rounding)
endif (MODULE_ROUNDING)

#-------------------------------------------------------------------------------
if (MODULE_SPAMOD)
  add_definitions(-DMODULE_SPAMOD)

  # Includes for spamod
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod/main)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod/main/attributes)
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-spamod/main/basic)

  add_subdirectory (src/lib-spamod lib-spamod)
endif (MODULE_SPAMOD)

if ( MODULE_COREFINEMENT )
  # Includes for tools
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

