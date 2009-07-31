#-------------------------------------------------------------------------------
add_library (spamod
  spamod-api.cc main/analytic-view.cc main/gmap-ops.cc main/k2-view.cc
  main/voxel-view.cc main/euclidian-view.cc main/ineq-op.cc
  main/topologic-view.cc
)

#-------------------------------------------------------------------------------
find_package(GLUT)
find_package(OpenGL)
INCLUDE(FindOpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(spamod ${OPENGL_LIBRARIES})

INCLUDE(FindGLUT REQUIRED)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(spamod ${GLUT_LIBRARIES})
#-------------------------------------------------------------------------------
