#-------------------------------------------------------------------------------
add_executable(exampleBasic example-basic.cc)
target_link_libraries (exampleBasic corefinement gmapkernel)
#-------------------------------------------------------------------------------
add_executable(submapTest submap.cc)
target_link_libraries (submapTest gmapkernel)
#-------------------------------------------------------------------------------
add_executable(compute-homology2d homology2d.cc)
target_link_libraries (compute-homology2d gmapkernel)
#-------------------------------------------------------------------------------
add_executable(compute-homology homology.cc)
target_link_libraries (compute-homology gmapkernel)
#-------------------------------------------------------------------------------
add_executable(compute-cubical-homology cubical-homology.cc)
target_link_libraries (compute-cubical-homology gmapkernel)
#-------------------------------------------------------------------------------
add_executable(convert-off-to-moka convert-off-to-moka.cc)
target_link_libraries (convert-off-to-moka gmapkernel)
#-------------------------------------------------------------------------------
