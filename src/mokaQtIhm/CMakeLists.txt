set(CMAKE_CXX_STANDARD 14)

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
# We need QT5 library
find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

QT5_WRAP_CPP(MOC_SRCS ${NEED_MOCS})
QT5_ADD_RESOURCES(RESOURCES_SRCS ${QT_RESOURCES})
#-------------------------------------------------------------------------------
add_executable(mokaQt ${IHM_SRCS} ${MOC_SRCS} ${RESOURCES_SRCS} ${NEED_MOCS})

target_link_libraries(mokaQt controler-gmap controler gmapkernel)
target_link_libraries(mokaQt Qt5::Core Qt5::Widgets Qt5::OpenGL)
#-------------------------------------------------------------------------------
