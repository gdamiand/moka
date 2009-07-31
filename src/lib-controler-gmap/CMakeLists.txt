#-------------------------------------------------------------------------------
add_library (controler-gmap 

  controler-gmap.cc controler-gmap-object-transformation.cc
  controler-gmap-rounding.cc controler-gmap-creations.cc
  controler-gmap-operations.cc controler-gmap-selection.cc
  controler-gmap-export-xfig.cc controler-gmap-parameter.cc
  controler-gmap-undo-redo.cc controler-gmap-input-events.cc
  controler-gmap-protected-methods.cc controler-gmap-views.cc

  operations/operations-corefinement.cc operations/operations-plating.cc
  operations/operations-creation.cc operations/operations-positioning.cc
  operations/operations-dual.cc operations/operations-rounding.cc
  operations/operations-duplication-deletion.cc 
  operations/operations-save-load-export.cc
  operations/operations-extrusion.cc operations/operations-selection.cc
  operations/operations-geology.cc operations/operations-sew-unsew.cc
  operations/operations-materialization.cc operations/operations-topology.cc
  operations/operations-merging-insertion-contraction.cc 
  operations/operations-triangulation.cc
  operations/operations-mesh.cc
  operations/operations-triangulation-quadrangulation.cc
  operations/operations-move-selection.cc

  parameter/parameter-controler/parameter-creation.cc            
  parameter/parameter-controler/parameter-object-scale.cc
  parameter/parameter-controler/parameter-gmap-vertex.cc
  parameter/parameter-controler/parameter-object-translation.cc
  parameter/parameter-controler/parameter-object-ponderation.cc
  parameter/parameter-controler/parameter-operations.cc
  parameter/parameter-controler/parameter-object-position.cc
  parameter/parameter-controler/parameter-polyline-position.cc
  parameter/parameter-controler/parameter-object-rotation.cc
  parameter/parameter-controler/parameter-selection.cc

  parameter/parameter-precompile/parameter-dart.cc
  parameter/parameter-precompile/parameter-preview.cc
  parameter/parameter-precompile/parameter-face.cc
  parameter/parameter-precompile/parameter-sew.cc
  parameter/parameter-precompile/parameter-normal-vector.cc
  parameter/parameter-precompile/parameter-spamod.cc
  parameter/parameter-precompile/parameter-object-transformation.cc
  parameter/parameter-precompile/parameter-vertex.cc

  precompile/precompile-dart.cc precompile/precompile-object-transformation.cc
  precompile/precompile-spamod.cc precompile/precompile-face.cc
  precompile/precompile-preview.cc precompile/precompile-vertex.cc
  precompile/precompile-normal-vector.cc precompile/precompile-sew.cc
  )
#-------------------------------------------------------------------------------
find_package(GLUT)
find_package(OpenGL)
INCLUDE(FindGLUT REQUIRED)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(controler-gmap ${GLUT_LIBRARIES})
#-------------------------------------------------------------------------------
