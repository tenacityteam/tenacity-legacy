#[=======================================================================[.rst:
FindSoxr
--------

Finds the Soxr library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Soxr::Soxr``
  The Soxr library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Soxr_FOUND``
  True if the system has the Soxr library.
``Soxr_INCLUDE_DIRS``
  Include directories needed to use Soxr.
``Soxr_LIBRARIES``
  Libraries needed to link to Soxr.
``Soxr_DEFINITIONS``
  Compile definitions needed to use Soxr.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Soxr_INCLUDE_DIR``
  The directory containing ``soxr.h``.
``Soxr_LIBRARY``
  The path to the Soxr library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_Soxr QUIET soxr)
endif()

find_path(Soxr_INCLUDE_DIR
  NAMES soxr.h
  PATHS ${PC_Soxr_INCLUDE_DIRS}
  DOC "Soxr include directory")
mark_as_advanced(Soxr_INCLUDE_DIR)

find_library(Soxr_LIBRARY
  NAMES soxr
  PATHS ${PC_Soxr_LIBRARY_DIRS}
  DOC "Soxr library"
)
mark_as_advanced(Soxr_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Soxr
  DEFAULT_MSG
  Soxr_LIBRARY
  Soxr_INCLUDE_DIR
)

if(Soxr_FOUND)
  set(Soxr_LIBRARIES "${Soxr_LIBRARY}")
  set(Soxr_INCLUDE_DIRS "${Soxr_INCLUDE_DIR}")
  set(Soxr_DEFINITIONS ${PC_Soxr_CFLAGS_OTHER})

  if(NOT TARGET Soxr::soxr)
    add_library(Soxr::soxr UNKNOWN IMPORTED)
    set_target_properties(Soxr::soxr
      PROPERTIES
        IMPORTED_LOCATION "${Soxr_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_Soxr_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${Soxr_INCLUDE_DIR}"
    )
  endif()
endif()
