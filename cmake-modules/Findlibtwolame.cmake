#[=======================================================================[.rst:
Findlibtwolame
--------

Finds the libtwolame library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``libtwolame::libtwolame``
  The libtwolame library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``libtwolame_FOUND``
  True if the system has the libtwolame library.
``libtwolame_INCLUDE_DIRS``
  Include directories needed to use libtwolame.
``libtwolame_LIBRARIES``
  Libraries needed to link to libtwolame.
``libtwolame_DEFINITIONS``
  Compile definitions needed to use libtwolame.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``libtwolame_INCLUDE_DIR``
  The directory containing ``twolame.h``.
``libtwolame_LIBRARY``
  The path to the libtwolame library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_libtwolame QUIET twolame)
endif()

find_path(libtwolame_INCLUDE_DIR
  NAMES twolame.h
  PATHS ${PC_libtwolame_INCLUDE_DIRS}
  DOC "libtwolame include directory")
mark_as_advanced(libtwolame_INCLUDE_DIR)

find_library(libtwolame_LIBRARY
  NAMES twolame
  PATHS ${PC_libtwolame_LIBRARY_DIRS}
  DOC "libtwolame library"
)
mark_as_advanced(libtwolame_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  libtwolame
  DEFAULT_MSG
  libtwolame_LIBRARY
  libtwolame_INCLUDE_DIR
)

if(libtwolame_FOUND)
  set(libtwolame_LIBRARIES "${libtwolame_LIBRARY}")
  set(libtwolame_INCLUDE_DIRS "${libtwolame_INCLUDE_DIR}")
  set(libtwolame_DEFINITIONS ${PC_libtwolame_CFLAGS_OTHER})

  if(NOT TARGET libtwolame::libtwolame)
    add_library(libtwolame::libtwolame UNKNOWN IMPORTED)
    set_target_properties(libtwolame::libtwolame
      PROPERTIES
        IMPORTED_LOCATION "${libtwolame_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_libtwolame_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${libtwolame_INCLUDE_DIR}"
    )
  endif()
endif()
