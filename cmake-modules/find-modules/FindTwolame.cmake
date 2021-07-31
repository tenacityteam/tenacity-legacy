#[=======================================================================[.rst:
FindTwolame
--------

Finds the Twolame library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``Twolame::Twolame``
  The Twolame library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``Twolame_FOUND``
  True if the system has the Twolame library.
``Twolame_INCLUDE_DIRS``
  Include directories needed to use Twolame.
``Twolame_LIBRARIES``
  Libraries needed to link to Twolame.
``Twolame_DEFINITIONS``
  Compile definitions needed to use Twolame.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``Twolame_INCLUDE_DIR``
  The directory containing ``twolame.h``.
``Twolame_LIBRARY``
  The path to the Twolame library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_Twolame QUIET flac)
endif()

find_path(Twolame_INCLUDE_DIR
  NAMES twolame.h
  PATHS ${PC_Twolame_INCLUDE_DIRS}
  DOC "Twolame include directory")
mark_as_advanced(Twolame_INCLUDE_DIR)

find_library(Twolame_LIBRARY
  NAMES twolame
  PATHS ${PC_Twolame_LIBRARY_DIRS}
  DOC "Twolame library"
)
mark_as_advanced(Twolame_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  Twolame
  DEFAULT_MSG
  Twolame_LIBRARY
  Twolame_INCLUDE_DIR
)

if(Twolame_FOUND)
  set(Twolame_LIBRARIES "${Twolame_LIBRARY}")
  set(Twolame_INCLUDE_DIRS "${Twolame_INCLUDE_DIR}")
  set(Twolame_DEFINITIONS ${PC_Twolame_CFLAGS_OTHER})

  if(NOT TARGET Twolame::Twolame)
    add_library(Twolame::Twolame UNKNOWN IMPORTED)
    set_target_properties(Twolame::Twolame
      PROPERTIES
        IMPORTED_LOCATION "${Twolame_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_Twolame_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${Twolame_INCLUDE_DIR}"
    )
  endif()
endif()
