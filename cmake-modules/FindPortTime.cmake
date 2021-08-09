# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE.txt file for details.

#[=======================================================================[.rst:
FindPortTime
---------------

Finds the PortTime library.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``PortTime_FOUND``
  True if the system has the PortTime library.
``PortTime_INCLUDE_DIRS``
  Include directories needed to use PortTime.
``PortTime_LIBRARIES``
  Libraries needed to link to PortTime.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``PortTime_INCLUDE_DIR``
  The directory containing ``porttime.h``.
``PortTime_LIBRARY``
  The path to the PortTime library.

#]=======================================================================]

find_library(PortTime_LIBRARY
  NAMES porttime
  DOC "PortTime library"
)
mark_as_advanced(PortTime_LIBRARY)

find_path(PortTime_INCLUDE_DIR
  NAMES porttime.h
  PATH_SUFFIXES portmidi porttime
  DOC "PortTime include directory")
mark_as_advanced(PortTime_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  PortTime
  DEFAULT_MSG
  PortTime_LIBRARY
  PortTime_INCLUDE_DIR
)

if(NOT TARGET PortMidi::PortTime)
  add_library(PortMidi::PortTime UNKNOWN IMPORTED)
  set_target_properties(PortMidi::PortTime
    PROPERTIES
      IMPORTED_LOCATION "${PortTime_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${PortTime_INCLUDE_DIR}"
  )
endif()
