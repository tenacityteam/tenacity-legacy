# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE.txt file for details.

#[=======================================================================[.rst:
Findsuil
--------

Finds the suil library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``suil::suil``
  The suil library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``suil_FOUND``
  True if the system has the suil library.
``suil_INCLUDE_DIRS``
  Include directories needed to use suil.
``suil_LIBRARIES``
  Libraries needed to link to suil.
``suil_DEFINITIONS``
  Compile definitions needed to use suil.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``suil_INCLUDE_DIR``
  The directory containing ``suil/all.h``.
``suil_LIBRARY``
  The path to the suil library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_suil QUIET suil-0)
endif()

find_path(suil_INCLUDE_DIR
  NAMES suil/suil.h
  PATHS ${PC_suil_INCLUDE_DIRS}
  DOC "suil include directory")
mark_as_advanced(suil_INCLUDE_DIR)

find_library(suil_LIBRARY
  NAMES suil-0
  PATHS ${PC_suil_LIBRARY_DIRS}
  DOC "suil library"
)
mark_as_advanced(suil_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  suil
  DEFAULT_MSG
  suil_LIBRARY
  suil_INCLUDE_DIR
)

if(suil_FOUND)
  set(suil_LIBRARIES "${suil_LIBRARY}")
  set(suil_INCLUDE_DIRS "${suil_INCLUDE_DIR}")
  set(suil_DEFINITIONS ${PC_suil_CFLAGS_OTHER})

  if(NOT TARGET suil::suil)
    add_library(suil::suil UNKNOWN IMPORTED)
    set_target_properties(suil::suil
      PROPERTIES
        IMPORTED_LOCATION "${suil_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_suil_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${suil_INCLUDE_DIR}"
    )
  endif()
endif()
