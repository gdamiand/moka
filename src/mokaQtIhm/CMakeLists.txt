#-------------------------------------------------------------------------------
include_directories (object-creations/ object-mesh/ object-operations/
  object-options/ utils/ windows/)
#-------------------------------------------------------------------------------
set (IHM_SRCS main.cc

object-creations/creation-boutons.qt.cc   object-creations/creation-options.qt.cc
object-creations/creation-brin.qt.cc      object-creations/creation-polygone.qt.cc
object-creations/creation-cylindre.qt.cc  object-creations/creation-pyramide.qt.cc
object-creations/creation-maillage.qt.cc  object-creations/creation-sphere.qt.cc
object-creations/creation-objet.qt.cc     object-creations/creation-tore.qt.cc

object-operations/dialog-homothetie.qt.cc object-operations/dialog-moveto.qt.cc
object-operations/dialog-operations.qt.cc object-operations/dialog-translation.qt.cc
object-operations/dialog-rotation.qt.cc   object-operations/operation-chanfreinage.qt.cc

object-options/options-affichage.qt.cc  object-options/options-frame.qt.cc
object-options/options-carac.qt.cc      object-options/options-interpolation.qt.cc
object-options/options-couleurs.qt.cc   object-options/options-ponderation.qt.cc
object-options/options-divers.qt.cc     object-options/options-vue.qt.cc
object-options/options-extrusion.qt.cc  
object-options/options-surfacic-homology.qt.cc
object-options/options-volumic-homology.qt.cc

utils/floatSpinBox.qt.cc  utils/HtmlEntities.cc  
utils/IdWidgets.qt.cc     utils/SpaceWidget.qt.cc

windows/dialog-do.qt.cc windows/gl-multi-window.qt.cc windows/menu-bar.qt.cc
windows/window.qt.cc    windows/gl-window.qt.cc       windows/select-bar.qt.cc
)

set (NEED_MOCS 
object-creations/creation-boutons.qt.hh    object-operations/operation-chanfreinage.qt.hh
object-creations/creation-brin.qt.hh       object-options/options-affichage.qt.hh
object-creations/creation-cylindre.qt.hh   object-options/options-carac.qt.hh
object-creations/creation-maillage.qt.hh   object-options/options-couleurs.qt.hh
object-creations/creation-objet.qt.hh      object-options/options-divers.qt.hh
object-creations/creation-options.qt.hh    object-options/options-extrusion.qt.hh
object-creations/creation-polygone.qt.hh   object-options/options-frame.qt.hh
object-creations/creation-pyramide.qt.hh   object-options/options-interpolation.qt.hh
object-creations/creation-sphere.qt.hh     object-options/options-ponderation.qt.hh
object-creations/creation-tore.qt.hh       object-options/options-vue.qt.hh
utils/floatSpinBox.qt.hh                   utils/IdWidgets.qt.hh
utils/SpaceWidget.qt.hh                    windows/dialog-do.qt.hh
object-operations/dialog-homothetie.qt.hh  windows/gl-multi-window.qt.hh
object-operations/dialog-operations.qt.hh  windows/gl-window.qt.hh
object-operations/dialog-rotation.qt.hh    windows/menu-bar.qt.hh
object-operations/dialog-moveto.qt.hh      windows/select-bar.qt.hh
object-operations/dialog-translation.qt.hh windows/window.qt.hh
object-options/options-surfacic-homology.qt.hh
object-options/options-volumic-homology.qt.hh)

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
add_executable(mokaQt ${IHM_SRCS} ${MOC_SRCS} ${RESOURCES_SRCS})

target_link_libraries (mokaQt controler-gmap controler gmapkernel)

target_link_libraries(mokaQt ${QT_LIBRARIES} ${QT_QT3SUPPORT_LIBRARY})
target_link_libraries(mokaQt ${QT_QTOPENGL_LIBRARY} ${QGLVIEWER_LIBRARIES})
#-------------------------------------------------------------------------------
