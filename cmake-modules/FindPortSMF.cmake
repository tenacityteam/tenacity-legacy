#[=======================================================================[.rst:
FindPortSMF
-------

Finds the PortSMF library.

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``PortSMF_FOUND``
  True if the system has the PortSMF library.
``PortSMF_INCLUDE_DIRS``
  Include directories needed to use PortSMF.
``PortSMF_LIBRARIES``
  Libraries needed to link to PortSMF.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``PortSMF_INCLUDE_DIR``
  The directory containing ``PortSMF/PortSMF.h``.
``PortSMF_LIBRARY``
  The path to the PortSMF library.

#]=======================================================================]
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(PortSMF QUIET portSMF)
endif()

# Arch installs to portsmf directory
find_path(PortSMF_INCLUDE_DIR
  NAMES portsmf/allegro.h
  PATHS ${PortSMF_INCLUDE_DIRS}
  DOC "PortSMF include directory"
)
if(NOT PortSMF_INCLUDE_DIR)
  # Debian installs to portSMF directory with capital SMF
  find_path(PortSMF_INCLUDE_DIR
    NAMES portSMF/allegro.h
    PATHS ${PortSMF_INCLUDE_DIRS}
    DOC "PortSMF include directory"
  )
  if(PortSMF_INCLUDE_DIR)
    set(PORTSMF_CAPITAL 1 CACHE INTERNAL "")
  endif()
endif()

mark_as_advanced(PortSMF_INCLUDE_DIR)

# Both Arch and Debian name the library portSMF with capital SMF
find_library(PortSMF_LIBRARY
  NAMES portSMF
  PATHS ${PortSMF_LIBRARY_DIRS}
  DOC "PortSMF library"
)
mark_as_advanced(PortSMF_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  PortSMF
  REQUIRED_VARS
  PortSMF_LIBRARY
  PortSMF_INCLUDE_DIR
)

if(PortSMF_FOUND)
  set(PortSMF_LIBRARIES ${PortSMF_LIBRARY})
  set(PortSMF_INCLUDE_DIRS ${PortSMF_INCLUDE_DIR})

  if(NOT TARGET PortSMF::PortSMF)
    add_library(PortSMF::PortSMF INTERFACE IMPORTED)
    target_link_libraries(PortSMF::PortSMF INTERFACE "${PortSMF_LIBRARY}")
    target_include_directories(PortSMF::PortSMF INTERFACE "${PortSMF_INCLUDE_DIR}")
    if(PORTSMF_CAPITAL)
      target_compile_definitions(PortSMF::PortSMF INTERFACE PORTSMF_CAPITAL)
    endif()
  endif()
endif()
