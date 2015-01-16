#-------------------------------------------------------------------------------
file(GLOB_RECURSE source_files *.cc *.hh *.icc)
source_group(Parameter REGULAR_EXPRESSION "parameter/.*") 
source_group(Precompile REGULAR_EXPRESSION "precompile/.*") 
source_group(Operations REGULAR_EXPRESSION "operations/.*")
add_library (controler-gmap ${source_files})
#-------------------------------------------------------------------------------
find_package(GLUT)
find_package(OpenGL)
INCLUDE(FindGLUT REQUIRED)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(controler-gmap ${GLUT_LIBRARIES})
#-------------------------------------------------------------------------------
