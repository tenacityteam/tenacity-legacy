# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE.txt file for details.

#[=======================================================================[.rst:
FindPortMidi
---------------

Finds the PortMidi library.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``PortMidi_FOUND``
  True if the system has the PortMidi library.
``PortMidi_INCLUDE_DIRS``
  Include directories needed to use PortMidi.
``PortMidi_LIBRARIES``
  Libraries needed to link to PortMidi.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``PortMidi_INCLUDE_DIR``
  The directory containing ``portmidi.h``.
``PortTime_INCLUDE_DIR``
  The directory containing ``porttime.h``.
``PortMidi_LIBRARY``
  The path to the PortMidi library.
``PortTime_LIBRARY``
  The path to the PortTime library.

#]=======================================================================]

find_path(PortMidi_INCLUDE_DIR
  NAMES portmidi.h
  PATH_SUFFIXES portmidi
  DOC "PortMidi include directory")
mark_as_advanced(PortMidi_INCLUDE_DIR)

find_library(PortMidi_LIBRARY
  NAMES portmidi portmidi_s
  DOC "PortMidi library"
)
mark_as_advanced(PortMidi_LIBRARY)

find_package_handle_standard_args(
  PortMidi
  DEFAULT_MSG
  PortMidi_LIBRARY
  PortMidi_INCLUDE_DIR
)

find_package(PortTime)

if(PortMidi_FOUND)
  if(NOT TARGET PortMidi::PortMidi)
    add_library(PortMidi::PortMidi UNKNOWN IMPORTED)
    set_target_properties(PortMidi::PortMidi
      PROPERTIES
        IMPORTED_LOCATION "${PortMidi_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${PortMidi_INCLUDE_DIR}"
    )
  endif()

  if(PortTime_FOUND)
    target_link_libraries(PortMidi::PortMidi INTERFACE PortMidi::PortTime)
  endif()
endif()
