#[=======================================================================[.rst:
FindSBSMS
--------

Finds the SBSMS library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``SBSMS::SBSMS``
  The SBSMS library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``SBSMS_FOUND``
  True if the system has the SBSMS library.
``SBSMS_INCLUDE_DIRS``
  Include directories needed to use SBSMS.
``SBSMS_LIBRARIES``
  Libraries needed to link to SBSMS.
``SBSMS_DEFINITIONS``
  Compile definitions needed to use SBSMS.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``SBSMS_INCLUDE_DIR``
  The directory containing ``sbsms.h``.
``SBSMS_LIBRARY``
  The path to the SBSMS library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_SBSMS QUIET flac)
endif()

find_path(SBSMS_INCLUDE_DIR
  NAMES sbsms.h
  PATHS ${PC_SBSMS_INCLUDE_DIRS}
  DOC "SBSMS include directory")
mark_as_advanced(SBSMS_INCLUDE_DIR)

find_library(SBSMS_LIBRARY
  NAMES sbsms
  PATHS ${PC_SBSMS_LIBRARY_DIRS}
  DOC "SBSMS library"
)
mark_as_advanced(SBSMS_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  SBSMS
  DEFAULT_MSG
  SBSMS_LIBRARY
  SBSMS_INCLUDE_DIR
)

if(SBSMS_FOUND)
  set(SBSMS_LIBRARIES "${SBSMS_LIBRARY}")
  set(SBSMS_INCLUDE_DIRS "${SBSMS_INCLUDE_DIR}")
  set(SBSMS_DEFINITIONS ${PC_SBSMS_CFLAGS_OTHER})

  if(NOT TARGET SBSMS::SBSMS)
    add_library(SBSMS::SBSMS UNKNOWN IMPORTED)
    set_target_properties(SBSMS::SBSMS
      PROPERTIES
        IMPORTED_LOCATION "${SBSMS_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_SBSMS_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${SBSMS_INCLUDE_DIR}"
    )
  endif()
endif()
