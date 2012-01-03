#-------------------------------------------------------------------------------
add_executable(exampleBasic example-basic.cc)
target_link_libraries (exampleBasic corefinement gmapkernel)
#-------------------------------------------------------------------------------
add_executable(submapTest submap.cc)
target_link_libraries (submapTest gmapkernel)
#-------------------------------------------------------------------------------
add_executable(homologyTest homology.cc)
target_link_libraries (homologyTest gmapkernel)
#-------------------------------------------------------------------------------
