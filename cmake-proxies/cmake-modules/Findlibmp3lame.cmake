# This file is part of Mixxx, Digital DJ'ing software.
# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE file for details.

#[=======================================================================[.rst:
Findlibmp3lame
-----------

Finds the LAME library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``libmp3lame::libmp3lame``
  The LAME library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``libmp3lame_FOUND``
  True if the system has the LAME library.
``libmp3lame_INCLUDE_DIRS``
  Include directories needed to use LAME.
``libmp3lame_LIBRARIES``
  Libraries needed to link to LAME.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``libmp3lame_INCLUDE_DIR``
  The directory containing ``lame/lame.h``.
``libmp3lame_LIBRARY``
  The path to the LAME library.

#]=======================================================================]

find_path(libmp3lame_INCLUDE_DIR
  NAMES lame/lame.h
  DOC "LAME include directory")
mark_as_advanced(libmp3lame_INCLUDE_DIR)

find_library(libmp3lame_LIBRARY
  NAMES mp3lame mp3lame-static
  DOC "LAME library"
)
mark_as_advanced(libmp3lame_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  libmp3lame
  DEFAULT_MSG
  libmp3lame_LIBRARY
  libmp3lame_INCLUDE_DIR
)

if(libmp3lame_FOUND)
  set(libmp3lame_LIBRARIES "${libmp3lame_LIBRARY}")
  set(libmp3lame_INCLUDE_DIRS "${libmp3lame_INCLUDE_DIR}")

  if(NOT TARGET libmp3lame::libmp3lame)
    add_library(libmp3lame::libmp3lame INTERFACE IMPORTED GLOBAL)
    set_target_properties(libmp3lame::libmp3lame
      PROPERTIES
        INTERFACE_LINK_LIBRARIES "${libmp3lame_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${libmp3lame_INCLUDE_DIRS}"
    )
  endif()
  message("Found libmp3lame: ${libmp3lame_INCLUDE_DIRS} | ${libmp3lame_LIBRARIES}")
endif()
