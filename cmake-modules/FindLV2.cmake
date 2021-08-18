#[=======================================================================[.rst:
FindLV2
-----------

Finds the LV2 library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``LV2::LV2``
  The LV2 library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``LV2_FOUND``
  True if the system has the LV2 library.
``LV2_INCLUDE_DIRS``
  Include directories needed to use LV2.
``LV2_LIBRARIES``
  Libraries needed to link to LV2.
``LV2_DEFINITIONS``
  Compile definitions needed to use LV2.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``LV2_INCLUDE_DIR``
  The directory containing ``lv2.h``.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_LV2 QUIET lv2)
endif()

# There is no library; only a header.
find_path(LV2_INCLUDE_DIR
  NAMES lv2.h
  PATHS ${PC_LV2_INCLUDE_DIRS}
  PATH_SUFFIXES LV2
  DOC "LV2 include directory")
mark_as_advanced(LV2_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  LV2
  DEFAULT_MSG
  LV2_INCLUDE_DIR
)

if(LV2_FOUND)
  if(NOT TARGET LV2::LV2)
    add_library(LV2::LV2 INTERFACE IMPORTED)
    set_target_properties(LV2::LV2
      PROPERTIES
        INTERFACE_COMPILE_OPTIONS "${PC_LV2_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${LV2_INCLUDE_DIR}"
    )
  endif()
endif()
