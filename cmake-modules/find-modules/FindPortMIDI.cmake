# This file is part of Mixxx, Digital DJ'ing software.
# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE file for details.

#[=======================================================================[.rst:
FindPortMIDI
---------------

Finds the PortMIDI library.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``PortMIDI_FOUND``
  True if the system has the PortMIDI library.
``PortMIDI_INCLUDE_DIRS``
  Include directories needed to use PortMIDI.
``PortMIDI_LIBRARIES``
  Libraries needed to link to PortMIDI.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``PortMIDI_INCLUDE_DIR``
  The directory containing ``portmidi.h``.
``PortTime_INCLUDE_DIR``
  The directory containing ``porttime.h``.
``PortMIDI_LIBRARY``
  The path to the PortMIDI library.
``PortTime_LIBRARY``
  The path to the PortTime library.

#]=======================================================================]

find_path(PortMIDI_INCLUDE_DIR
  NAMES portmidi.h
  PATH_SUFFIXES portmidi
  DOC "PortMIDI include directory")
mark_as_advanced(PortMIDI_INCLUDE_DIR)

find_library(PortMIDI_LIBRARY
  NAMES portmidi portmidi_s
  DOC "PortMIDI library"
)
mark_as_advanced(PortMIDI_LIBRARY)

find_package_handle_standard_args(
  PortMIDI
  DEFAULT_MSG
  PortMIDI_LIBRARY
  PortMIDI_INCLUDE_DIR
)

find_package(PortTime)

if(PortMIDI_FOUND)
  if(NOT TARGET PortMIDI::PortMIDI)
    add_library(PortMIDI::PortMIDI UNKNOWN IMPORTED)
    set_target_properties(PortMIDI::PortMIDI
      PROPERTIES
        IMPORTED_LOCATION "${PortMIDI_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${PortMIDI_INCLUDE_DIR}"
    )
  endif()

  if(PortTime_FOUND)
    target_link_libraries(PortMIDI::PortMIDI INTERFACE PortMIDI::PortTime)
  endif()
endif()
