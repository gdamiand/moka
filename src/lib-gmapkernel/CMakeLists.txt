#-------------------------------------------------------------------------------
add_library (gmapkernel 

  g-map/attribute.cc g-map/dart.cc g-map/embedding.cc g-map/g-map.cc
  g-map/coverage.cc g-map/dynamic-coverage.cc g-map/g-map-basic.cc 
  g-map/static-coverage.cc

  g-map-generic/gmg-border.cc g-map-generic/gmg-fictive-face.cc
  g-map-generic/gmg-selection.cc g-map-generic/gmg-contract-and-remove.cc
  g-map-generic/gmg-inline.cc g-map-generic/gmg-sew.cc  
  g-map-generic/gmg-contraction.cc g-map-generic/gmg-insertion.cc        
  g-map-generic/gmg-simplification.cc g-map-generic/gmg-degree.cc
  g-map-generic/gmg-instance.cc g-map-generic/gmg-stop-up.cc  
  g-map-generic/gmg-deletion.cc g-map-generic/gmg-merge.cc
  g-map-generic/gmg-thickening.cc g-map-generic/gmg-direct-info.cc
  g-map-generic/gmg-polyline.cc g-map-generic/gmg-topology.cc
  g-map-generic/gmg-duplication.cc g-map-generic/gmg-primitive.cc
  g-map-generic/gmg-triangulation.cc g-map-generic/gmg-exploration.cc
  g-map-generic/gmg-primitive-mesh.cc g-map-generic/gmg-unsew.cc  
  g-map-generic/gmg-extrusion.cc g-map-generic/gmg-quadrangulation.cc
  g-map-generic/gmg-fictive-edge.cc g-map-generic/gmg-removal.cc

  g-map-vertex/attribute-vertex.cc g-map-vertex/gmv-geometry.cc
  g-map-vertex/gmv-null-length-edge.cc g-map-vertex/gmv-save.cc  
  g-map-vertex/dart-vertex.cc g-map-vertex/gmv-geo-sew.cc
  g-map-vertex/gmv-off.cc g-map-vertex/gmv-stop-up.cc  
  g-map-vertex/gmv-burstview.cc g-map-vertex/gmv-inline.cc
  g-map-vertex/gmv-plating.cc g-map-vertex/gmv-thickening.cc
  g-map-vertex/gmv-deformation.cc g-map-vertex/gmv-insertion.cc
  g-map-vertex/gmv-polyline.cc g-map-vertex/gmv-triangulation.cc
  g-map-vertex/gmv-direct-info.cc g-map-vertex/gmv-instance.cc
  g-map-vertex/gmv-primitive.cc g-map-vertex/mesh-diver.cc
  g-map-vertex/gmv-dual.cc g-map-vertex/gmv-isomorphisme.cc
  g-map-vertex/gmv-primitive-mesh.cc g-map-vertex/gmv-export-pov.cc
  g-map-vertex/gmv-materialization.cc g-map-vertex/gmv-quadrangulation.cc
  g-map-vertex/gmv-extrusion.cc g-map-vertex/gmv-merge.cc
  g-map-vertex/gmv-regression.cc

  tools/geometry/geometry.cc tools/geometry/transformation-matrix.cc  
  tools/geometry/vertex.cc tools/geometry/math-extension.cc
  tools/geometry/vector.cc tools/geometry/plane.cc

  tools/streams/streams.cc
)

#-------------------------------------------------------------------------------
if (MODULE_COREFINEMENT)
  target_link_libraries (gmapkernel corefinement)
endif (MODULE_COREFINEMENT)

if (MODULE_MESH)
  target_link_libraries (gmapkernel mesh)
endif (MODULE_MESH)

if (MODULE_ROUNDING)
  target_link_libraries (gmapkernel rounding)
endif (MODULE_ROUNDING)

if (MODULE_SPAMOD)
  target_link_libraries (gmapkernel spamod)
endif (MODULE_SPAMOD)

if ( MODULE_COREFINEMENT )
  target_link_libraries (gmapkernel tools)
endif ( MODULE_COREFINEMENT )

if ( MODULE_EXTRACTION_IMAGE )
  target_link_libraries (gmapkernel extraction-images)
endif ( MODULE_EXTRACTION_IMAGE )

target_link_libraries (gmapkernel homology)
#-------------------------------------------------------------------------------