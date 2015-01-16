#-------------------------------------------------------------------------------
file(GLOB_RECURSE source_files *.cc *.hh *.icc)
add_library (spamod ${source_files})
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
