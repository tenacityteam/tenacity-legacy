# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE.txt file for details.

#[=======================================================================[.rst:
FindPortAudio
--------

Finds the PortAudio library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``PortAudio::PortAudio``
  The PortAudio library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``PortAudio_FOUND``
  True if the system has the PortAudio library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PortAudio portaudio-2.0)
else()
  find_path(PortAudio_INCLUDEDIR
    NAMES portaudio.h
    DOC "PortAudio include directory")

  find_library(PortAudio_LINK_LIBRARIES
    NAMES portaudio
    DOC "PortAudio library"
  )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  PortAudio
  DEFAULT_MSG
  PortAudio_LINK_LIBRARIES
  PortAudio_INCLUDEDIR
)

if(PortAudio_FOUND)
  if(NOT TARGET PortAudio::PortAudio)
    add_library(PortAudio::PortAudio INTERFACE IMPORTED)
    target_link_libraries(PortAudio::PortAudio INTERFACE "${PortAudio_LINK_LIBRARIES}")
    target_include_directories(PortAudio::PortAudio INTERFACE "${PortAudio_INCLUDEDIR}")
  endif()
endif()
