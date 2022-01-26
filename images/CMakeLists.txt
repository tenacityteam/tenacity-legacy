
set( TARGET images )
set( TARGET_ROOT ${CMAKE_SOURCE_DIR}/images )

message( STATUS "========== Configuring ${TARGET} ==========" )

def_vars()

# This isn't really a target...

list( APPEND PIXMAPS
   ${_SRCDIR}/gnome-mime-application-x-audacity-project.xpm
   ${_SRCDIR}/icons/16x16/tenacity16.xpm
   ${_SRCDIR}/icons/32x32/tenacity32.xpm
   ${_SRCDIR}/icons/48x48/tenacity.xpm
)

if( NOT CMAKE_SYSTEM_NAME MATCHES "Darwin" )
   if( NOT WIN32)
      install( FILES "${_SRCDIR}/tenacity.svg"
               DESTINATION "${_DATADIR}/icons/hicolor/scalable/apps" )
      install( DIRECTORY "${_SRCDIR}/icons/"
               DESTINATION "${_DATADIR}/icons/hicolor"
               FILES_MATCHING PATTERN "*.png" )
      install( FILES ${PIXMAPS}
               DESTINATION "${_DATADIR}/pixmaps" )
   endif()
endif()

