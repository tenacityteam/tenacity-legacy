# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE.txt file for details.

#[=======================================================================[.rst:
FindFLAC++
--------

Finds the FLAC++ library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``FLAC++::FLAC++``
  The FLAC++ library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``FLAC++_FOUND``
  True if the system has the FLAC++ library.
``FLAC++_INCLUDE_DIRS``
  Include directories needed to use FLAC++.
``FLAC++_LIBRARIES``
  Libraries needed to link to FLAC++.
``FLAC++_DEFINITIONS``
  Compile definitions needed to use FLAC++.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``FLAC++_INCLUDE_DIR``
  The directory containing ``FLAC++/all.h``.
``FLAC++_LIBRARY``
  The path to the FLAC++ library.

#]=======================================================================]

find_package(FLAC REQUIRED)

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_FLAC++ QUIET flac++)
endif()

find_path(FLAC++_INCLUDE_DIR
  NAMES FLAC++/all.h
  PATHS ${PC_FLAC++_INCLUDE_DIRS}
  DOC "FLAC++ include directory")
mark_as_advanced(FLAC++_INCLUDE_DIR)

find_library(FLAC++_LIBRARY
  NAMES FLAC++
  PATHS ${PC_FLAC++_LIBRARY_DIRS}
  DOC "FLAC++ library"
)
mark_as_advanced(FLAC++_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  FLAC++
  DEFAULT_MSG
  FLAC++_LIBRARY
  FLAC++_INCLUDE_DIR
)

if(FLAC++_FOUND)
  set(FLAC++_LIBRARIES "${FLAC++_LIBRARY}")
  set(FLAC++_INCLUDE_DIRS "${FLAC++_INCLUDE_DIR}")
  set(FLAC++_DEFINITIONS ${PC_FLAC++_CFLAGS_OTHER})

  if(NOT TARGET FLAC++::FLAC++)
    add_library(FLAC++::FLAC++ UNKNOWN IMPORTED)
    set_target_properties(FLAC++::FLAC++
      PROPERTIES
        IMPORTED_LOCATION "${FLAC++_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_FLAC++_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${FLAC++_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES FLAC::FLAC
    )
  endif()
endif()
