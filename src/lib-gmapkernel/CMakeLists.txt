#-------------------------------------------------------------------------------
file(GLOB_RECURSE source_files *.cc *.hh *.incc) #VIC cambio *.icc pot *.incc
source_group(GMap REGULAR_EXPRESSION "g-map/.*") 
source_group(GMap-generic REGULAR_EXPRESSION "g-map-generic/.*") 
source_group(GMap-vertex REGULAR_EXPRESSION "g-map-vertex/.*") 
source_group(Tools REGULAR_EXPRESSION "tools/.*") 
add_library (gmapkernel ${source_files})
#-------------------------------------------------------------------------------
#VIC
target_include_directories(gmapkernel PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../msys64/usr/include>
)
#-------------------------------------------------------------------------------
target_link_libraries (gmapkernel homology)
#-------------------------------------------------------------------------------
if (MODULE_COREFINEMENT)
  target_link_libraries (gmapkernel corefinement)
endif (MODULE_COREFINEMENT)
#-------------------------------------------------------------------------------
if (MODULE_MESH)
  target_link_libraries (gmapkernel mesh)
endif (MODULE_MESH)
#-------------------------------------------------------------------------------
if (MODULE_ROUNDING)
  target_link_libraries (gmapkernel rounding)
endif (MODULE_ROUNDING)
#-------------------------------------------------------------------------------
if (MODULE_SPAMOD)
  target_link_libraries (gmapkernel spamod)
endif (MODULE_SPAMOD)
#-------------------------------------------------------------------------------
if ( MODULE_COREFINEMENT )
  target_link_libraries (gmapkernel tools)
endif ( MODULE_COREFINEMENT )
#-------------------------------------------------------------------------------
if ( MODULE_EXTRACTION_IMAGE )
  target_link_libraries (gmapkernel extraction-images)
endif ( MODULE_EXTRACTION_IMAGE )
#-------------------------------------------------------------------------------
if ( assimp_FOUND )
  target_link_libraries (gmapkernel assimp)
endif ( assimp_FOUND )
#-------------------------------------------------------------------------------
