#-------------------------------------------------------------------------------
file(GLOB_RECURSE source_files *.cc *.hh *.icc)
source_group(Parameter REGULAR_EXPRESSION "parameter/.*") 
source_group(Precompile REGULAR_EXPRESSION "precompile/.*") 
source_group(View REGULAR_EXPRESSION "view/.*") 
add_library (controler ${source_files})
#-------------------------------------------------------------------------------
# Check under windows find_package(GLUT)
find_package(OpenGL)
INCLUDE(FindOpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(controler ${OPENGL_LIBRARIES})
#-------------------------------------------------------------------------------
