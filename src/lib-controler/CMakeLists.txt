#-------------------------------------------------------------------------------
add_library (controler 

  controler.cc controler-operations.cc controler-protected-methods.cc
  controler-selection.cc controler-views.cc controler-input-events.cc
  controler-parameter.cc controler-scene-transformation.cc 
  controler-undo-redo.cc operations.cc operations-eye-positioning.cc

  parameter/parameter.cc

  parameter/parameter-precompile/parameter-aimed-point.cc
  parameter/parameter-precompile/parameter-grid.cc
  parameter/parameter-precompile/parameter-axis.cc

  parameter/parameter-view/parameter-aimed-position.cc
  parameter/parameter-view/parameter-eye-position.cc
  parameter/parameter-view/parameter-drawing.cc

  precompile/precompile-aimed-point.cc precompile/precompile-axis.cc  
  precompile/precompile.cc precompile/precompile-grid.cc

  view/view.cc view/view-precompile.cc view/view-precompile-ortho.cc
  view/view-precompile-perspective.cc
 )

#-------------------------------------------------------------------------------
# Check under windows find_package(GLUT)
find_package(OpenGL)
INCLUDE(FindOpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(controler ${OPENGL_LIBRARIES})
#-------------------------------------------------------------------------------
