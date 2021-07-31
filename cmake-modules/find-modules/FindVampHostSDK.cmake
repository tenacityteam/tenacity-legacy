#[=======================================================================[.rst:
FindVampHostSDK
--------

Finds the VampHostSDK library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``VampHostSDK::VampHostSDK``
  The VampHostSDK library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``VampHostSDK_FOUND``
  True if the system has the VampHostSDK library.
``VampHostSDK_INCLUDE_DIRS``
  Include directories needed to use VampHostSDK.
``VampHostSDK_LIBRARIES``
  Libraries needed to link to VampHostSDK.
``VampHostSDK_DEFINITIONS``
  Compile definitions needed to use VampHostSDK.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``VampHostSDK_INCLUDE_DIR``
  The directory containing ``vamp-hostsdk/vamp-hostsdk.h``.
``VampHostSDK_LIBRARY``
  The path to the VampHostSDK library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_VampHostSDK QUIET vamp-hostsdk)
endif()

find_path(VampHostSDK_INCLUDE_DIR
  NAMES vamp-hostsdk/vamp-hostsdk.h
  PATHS ${PC_VampHostSDK_INCLUDE_DIRS}
  DOC "VampHostSDK include directory")
mark_as_advanced(VampHostSDK_INCLUDE_DIR)

find_library(VampHostSDK_LIBRARY
  NAMES vamp-hostsdk
  PATHS ${PC_VampHostSDK_LIBRARY_DIRS}
  DOC "VampHostSDK library"
)
mark_as_advanced(VampHostSDK_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  VampHostSDK
  DEFAULT_MSG
  VampHostSDK_LIBRARY
  VampHostSDK_INCLUDE_DIR
)

if(VampHostSDK_FOUND)
  set(VampHostSDK_LIBRARIES "${VampHostSDK_LIBRARY}")
  set(VampHostSDK_INCLUDE_DIRS "${VampHostSDK_INCLUDE_DIR}")
  set(VampHostSDK_DEFINITIONS ${PC_VampHostSDK_CFLAGS_OTHER})

  if(NOT TARGET VampHostSDK::VampHostSDK)
    add_library(VampHostSDK::VampHostSDK UNKNOWN IMPORTED)
    set_target_properties(VampHostSDK::VampHostSDK
      PROPERTIES
        IMPORTED_LOCATION "${VampHostSDK_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_VampHostSDK_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${VampHostSDK_INCLUDE_DIR}"
    )
  endif()
endif()
