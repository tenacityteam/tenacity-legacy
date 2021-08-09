# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE.txt file for details.

#[=======================================================================[.rst:
Findid3tag
----------

Finds the id3tag library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``id3tag::id3tag``
  The id3tag library

Result Variables
^^^^^^^^^^^^^^^^

This will define the following variables:

``id3tag_FOUND``
  True if the system has the id3tag library.
``id3tag_INCLUDE_DIRS``
  Include directories needed to use id3tag.
``id3tag_LIBRARIES``
  Libraries needed to link to id3tag.
``id3tag_DEFINITIONS``
  Compile definitions needed to use id3tag.

Cache Variables
^^^^^^^^^^^^^^^

The following cache variables may also be set:

``id3tag_INCLUDE_DIR``
  The directory containing ``id3tag.h``.
``id3tag_LIBRARY``
  The path to the id3tag library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(id3tag id3tag)
else()
  find_path(id3tag_INCLUDEDIR
    NAMES id3tag.h
    DOC "id3tag include directory")

  find_library(id3tag_LINK_LIBRARIES
    NAMES id3tag
    DOC "id3tag library"
  )

  find_package(ZLIB REQUIRED)
  list(APPEND id3tag_LINK_LIBRARIES ZLIB::ZLIB)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  id3tag
  DEFAULT_MSG
  id3tag_LINK_LIBRARIES
  id3tag_INCLUDEDIR
)

if(id3tag_FOUND)
  if(NOT TARGET id3tag::id3tag)
    add_library(id3tag::id3tag INTERFACE IMPORTED)
    target_link_libraries(id3tag::id3tag INTERFACE "${id3tag_LINK_LIBRARIES}")
    target_include_directories(id3tag::id3tag INTERFACE "${id3tag_INCLUDEDIR}")
  endif()
endif()
