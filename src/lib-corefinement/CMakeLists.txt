#-------------------------------------------------------------------------------
add_library (corefinement
  
  2d-propagation/boolean-operations-2d.cc
  2d-propagation/corefine-2d-propagation.cc

  2d-sweeping/corefine-2d-sweeping.cc
  2d-sweeping/sweeping-comparators.cc

  3d/boolean-operations-3d.cc
  3d/corefine-3d.cc
  3d/corefine-3d-face-face.cc 
  3d/face-intersection-tools.cc  

  common/boolean-operations.cc
  common/edge-intersection.cc
  common/corefine.cc
  common/corefine-2d-tools.cc
  common/corefine-3d-tools.cc

  interface/corefinement-api.cc
)
#-------------------------------------------------------------------------------
