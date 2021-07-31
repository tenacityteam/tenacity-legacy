#[=======================================================================[.rst:
FindUUID
-----------

Finds the UUID library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``UUID::UUID``
  The UUID library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``UUID_FOUND``
  True if the system has the UUID library.
``UUID_INCLUDE_DIRS``
  Include directories needed to use UUID.
``UUID_LIBRARIES``
  Libraries needed to link to UUID.
``UUID_DEFINITIONS``
  Compile definitions needed to use UUID.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``UUID_INCLUDE_DIR``
  The directory containing ``UUID.h``.
``UUID_LIBRARY``
  The path to the UUID library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PC_UUID QUIET UUID)
endif()

find_path(UUID_INCLUDE_DIR
  NAMES uuid/uuid.h
  PATHS ${PC_UUID_INCLUDE_DIRS}
  PATH_SUFFIXES UUID
  DOC "UUID include directory")
mark_as_advanced(UUID_INCLUDE_DIR)

find_library(UUID_LIBRARY
  NAMES uuid
  PATHS ${PC_UUID_LIBRARY_DIRS}
  DOC "UUID library"
)
mark_as_advanced(UUID_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  UUID
  DEFAULT_MSG
  UUID_LIBRARY
  UUID_INCLUDE_DIR
)

if(UUID_FOUND)
  set(UUID_LIBRARIES "${UUID_LIBRARY}")
  set(UUID_INCLUDE_DIRS "${UUID_INCLUDE_DIR}")
  set(UUID_DEFINITIONS ${PC_UUID_CFLAGS_OTHER})

  if(NOT TARGET UUID::UUID)
    add_library(UUID::UUID UNKNOWN IMPORTED)
    set_target_properties(UUID::UUID
      PROPERTIES
        IMPORTED_LOCATION "${UUID_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${PC_UUID_CFLAGS_OTHER}"
        INTERFACE_INCLUDE_DIRECTORIES "${UUID_INCLUDE_DIR}"
    )
  endif()
endif()
