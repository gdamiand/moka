#-------------------------------------------------------------------------------
add_executable(exampleBasic example-basic.cc)
target_link_libraries (exampleBasic corefinement gmapkernel)
#-------------------------------------------------------------------------------
add_executable(submapTest submap.cc)
target_link_libraries (submapTest gmapkernel)
#-------------------------------------------------------------------------------
add_executable(compute-homology homology.cc)
target_link_libraries (compute-homology gmapkernel)
#-------------------------------------------------------------------------------
add_executable(compute-cubical-homology cubical-homology.cc)
target_link_libraries (compute-cubical-homology gmapkernel)
#-------------------------------------------------------------------------------
