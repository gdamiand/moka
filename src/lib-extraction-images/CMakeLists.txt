#-------------------------------------------------------------------------------
add_library (extraction-images
  extraction-images.cc image-3d.cc image-3d-cea.cc image-3d-magick.cc
  )

if ( ImageMagick_FOUND AND ImageMagick_Magick++_FOUND )
  TARGET_LINK_LIBRARIES(extraction-images ${ImageMagick_LIBRARIES}
    ${ImageMagick_Magick++_LIBRARIES})
endif ( ImageMagick_FOUND AND ImageMagick_Magick++_FOUND )
#-------------------------------------------------------------------------------
