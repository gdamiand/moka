#-------------------------------------------------------------------------------
file(GLOB_RECURSE source_files *.cc *.hh *.icc)
source_group(GMap REGULAR_EXPRESSION "g-map/.*") 
source_group(GMap-generic REGULAR_EXPRESSION "g-map-generic/.*") 
source_group(GMap-vertex REGULAR_EXPRESSION "g-map-vertex/.*") 
source_group(Tools REGULAR_EXPRESSION "tools/.*") 
add_library (gmapkernel ${source_files})
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
if ( ASSIMP_FOUND )
  target_link_libraries (gmapkernel ${ASSIMP_LIBRARIES})
endif ( ASSIMP_FOUND )
#-------------------------------------------------------------------------------