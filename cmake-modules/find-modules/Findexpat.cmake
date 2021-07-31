#[=======================================================================[.rst:
Findexpat
--------------

Finds the expat library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``expat::expat``
  The expat library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``expat_FOUND``
  True if the system has the expat library.
``expat_INCLUDE_DIRS``
  Include directories needed to use expat.
``expat_LIBRARIES``
  Libraries needed to link to expat.
``expat_DEFINITIONS``
  Compile definitions needed to use expat.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``expat_INCLUDE_DIR``
  The directory containing ``expat.h``.
``expat_LIBRARY``
  The path to the expat library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_expat QUIET expat)
endif()

find_path(expat_INCLUDE_DIR
  NAMES expat.h
  PATHS ${PC_expat_INCLUDE_DIRS}
  DOC "expat include directory")
mark_as_advanced(expat_INCLUDE_DIR)

find_library(expat_LIBRARY
  NAMES expat
  PATHS ${PC_expat_LIBRARY_DIRS}
  DOC "expat library"
)
mark_as_advanced(expat_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  expat
  DEFAULT_MSG
  expat_LIBRARY
  expat_INCLUDE_DIR
)

if(expat_FOUND)
  set(expat_LIBRARIES "${expat_LIBRARY}")
  set(expat_INCLUDE_DIRS "${expat_INCLUDE_DIR}")
  set(expat_DEFINITIONS ${PC_expat_CFLAGS_OTHER})

  if(NOT TARGET expat::expat)
    add_library(expat::expat UNKNOWN IMPORTED)
    set_target_properties(expat::expat
      PROPERTIES
        IMPORTED_LOCATION "${expat_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_expat_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${expat_INCLUDE_DIR}"
    )
  endif()
endif()
