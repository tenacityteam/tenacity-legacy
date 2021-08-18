#[=======================================================================[.rst:
Findsbsms
--------

Finds the sbsms library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``sbsms::sbsms``
  The sbsms library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``sbsms_FOUND``
  True if the system has the sbsms library.
``sbsms_INCLUDE_DIRS``
  Include directories needed to use sbsms.
``sbsms_LIBRARIES``
  Libraries needed to link to sbsms.
``sbsms_DEFINITIONS``
  Compile definitions needed to use sbsms.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``sbsms_INCLUDE_DIR``
  The directory containing ``sbsms.h``.
``sbsms_LIBRARY``
  The path to the sbsms library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_sbsms QUIET flac)
endif()

find_path(sbsms_INCLUDE_DIR
  NAMES sbsms.h
  PATHS ${PC_sbsms_INCLUDE_DIRS}
  DOC "sbsms include directory")
mark_as_advanced(sbsms_INCLUDE_DIR)

find_library(sbsms_LIBRARY
  NAMES sbsms
  PATHS ${PC_sbsms_LIBRARY_DIRS}
  DOC "sbsms library"
)
mark_as_advanced(sbsms_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  sbsms
  DEFAULT_MSG
  sbsms_LIBRARY
  sbsms_INCLUDE_DIR
)

if(sbsms_FOUND)
  if(NOT TARGET sbsms::sbsms)
    add_library(sbsms::sbsms UNKNOWN IMPORTED)
    set_target_properties(sbsms::sbsms
      PROPERTIES
        IMPORTED_LOCATION "${sbsms_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_sbsms_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${sbsms_INCLUDE_DIR}"
    )
  endif()
endif()
