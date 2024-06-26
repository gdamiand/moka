#-------------------------------------------------------------------------------
# Moka : A 3D Topological Modeller.
#
cmake_minimum_required(VERSION 3.5)
project(moka)

################################################################################
# By default, there are two possible build type:
#   (fix when executing cmake, for example:
#	 cd debug; cmake ../src/ -DCMAKE_BUILD_TYPE=Debug)
# Debug       = non inline + assert 
# Release     = inline     + no-assert + O3

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

# For profilling with gprof
# ADD_DEFINITIONS("-pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

# To enable c++11
ADD_DEFINITIONS("-std=c++11")

# Decide which opengl driver use.
set(OpenGL_GL_PREFERENCE GLVND)
 
#-------------------------------------------------------------------------------
# Some modules that we can enable or disable.
OPTION(MODULE_MESH "Activate mesh module" ON)
OPTION(MODULE_ROUNDING "Activate rounding module" ON)
OPTION(MODULE_COREFINEMENT "Activate corefining module" ON)
OPTION(MODULE_SPAMOD "Activate spamod module" OFF)
OPTION(MODULE_EXTRACTION_IMAGE "Activate extraction image module (require ImageMagick++)" ON)
OPTION(DISABLE_SELECTION_BOX "Disable the selection box on a 3D view" OFF)

#-------------------------------------------------------------------------------
# Some compiler options that we can enable with some ADD_DEFINITIONS(xxx).

# -DNO_ACCENTED_CHARS to avoid accented char.
# -DNO_LIST_OPENGL to not use precompile openGl list.

ENABLE_TESTING()
INCLUDE(CTest)

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
# Includes for ASSIMP
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
find_package(assimp)

if ( assimp_FOUND )
   ADD_DEFINITIONS(-DWITH_ASSIMP)
   include_directories(${assimp_INCLUDE_DIR})
endif ( assimp_FOUND )

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
#  Includes for homology
include_directories (${CMAKE_SOURCE_DIR}/src/lib-homology)
include_directories (${CMAKE_SOURCE_DIR}/src/lib-homology/matrices-pmq)
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
#-------------------------------------------------------------------------------
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
#-------------------------------------------------------------------------------
if ( MODULE_COREFINEMENT )
  # Includes for tools
  include_directories (${CMAKE_SOURCE_DIR}/src/lib-tools)

  add_subdirectory (src/lib-tools lib-tools)
endif ( MODULE_COREFINEMENT )
#-------------------------------------------------------------------------------
if ( MODULE_EXTRACTION_IMAGE )

  FIND_PACKAGE(ImageMagick COMPONENTS Magick++)

  if ( NOT (ImageMagick_Magick++_FOUND ) )
    add_definitions(-DMODULE_EXTRACTION_IMAGE_WITHOUT_MAGICK)
    message(STATUS "Warning: Magick++ not installed, module extraction image does not able to load images.")
  else ( NOT (ImageMagick_Magick++_FOUND ) )
     # We need to use pkg_check_modules in order to retreive the compiler options
     # required for ImageMagick (I think it is bug that it is not done by find_package)
     find_package(PkgConfig)
     pkg_check_modules(MAGICKXX Magick++)
     add_definitions(${MAGICKXX_CFLAGS})
  endif ( NOT (ImageMagick_Magick++_FOUND ) )

  add_definitions(-DMODULE_EXTRACTION_IMAGE)
  include_directories(${CMAKE_SOURCE_DIR}/src/lib-extraction-images)
  add_subdirectory   (src/lib-extraction-images lib-extraction-images)
endif ( MODULE_EXTRACTION_IMAGE )
#-------------------------------------------------------------------------------
if (DISABLE_SELECTION_BOX)
  add_definitions(-DDISABLE_SELECTION_BOX)
endif (DISABLE_SELECTION_BOX)
#-------------------------------------------------------------------------------
add_subdirectory (src/lib-gmapkernel lib-gmapkernel)
add_subdirectory (src/lib-controler lib-controler)
add_subdirectory (src/lib-controler-gmap lib-controler-gmap)
add_subdirectory (src/lib-homology lib-homology)
add_subdirectory (src/mokaQtIhm mokaQtIhm)

add_subdirectory (src/tools tools EXCLUDE_FROM_ALL)
#-------------------------------------------------------------------------------

