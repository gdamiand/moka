#-------------------------------------------------------------------------------
file(GLOB_RECURSE source_files *.cc *.hh *.icc)
add_library (extraction-images ${source_files})
#-------------------------------------------------------------------------------
if ( ImageMagick_FOUND AND ImageMagick_Magick++_FOUND )
  TARGET_LINK_LIBRARIES(extraction-images ${ImageMagick_LIBRARIES}
    ${ImageMagick_Magick++_LIBRARIES})
endif ( ImageMagick_FOUND AND ImageMagick_Magick++_FOUND )
#-------------------------------------------------------------------------------
