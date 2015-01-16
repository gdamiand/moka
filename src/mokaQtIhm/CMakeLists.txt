#-------------------------------------------------------------------------------
include_directories(object-creations/ 
                    object-mesh/ 
                    object-operations/
                    object-options/ 
                    utils/ 
                    windows/)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE IHM_SRCS *.cc)
file(GLOB_RECURSE NEED_MOCS *.qt.hh)
set (QT_RESOURCES Icones/icones.qrc)
#-------------------------------------------------------------------------------
# We need QT4 library
FIND_PACKAGE(Qt4 COMPONENTS QtOpenGL Qt3Support REQUIRED)
INCLUDE(${QT_USE_FILE})

add_definitions(${QT_DEFINITIONS})
include_directories (${QT_QTOPENGL_INCLUDE_DIR})

QT4_WRAP_CPP(MOC_SRCS ${NEED_MOCS})
QT4_ADD_RESOURCES(RESOURCES_SRCS ${QT_RESOURCES})
#-------------------------------------------------------------------------------
# QGLViewer
# find_package(QGLViewer)
# include_directories (${QGLVIEWER_INCLUDE_DIR})

#-------------------------------------------------------------------------------
add_executable(mokaQt ${IHM_SRCS} ${MOC_SRCS} ${RESOURCES_SRCS} ${NEED_MOCS})

target_link_libraries (mokaQt controler-gmap controler gmapkernel)

target_link_libraries(mokaQt ${QT_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY})
target_link_libraries(mokaQt ${QT_QTOPENGL_LIBRARY} ${QGLVIEWER_LIBRARIES})
#-------------------------------------------------------------------------------
