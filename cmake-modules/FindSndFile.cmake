# Copyright (C) 2001-2020 Mixxx Development Team
# Distributed under the GNU General Public Licence (GPL) version 2 or any later
# later version. See the LICENSE.txt file for details.

#[=======================================================================[.rst:
FindSndFile
-----------

Finds the SndFile library.

Imported Targets
^^^^^^^^^^^^^^^^

This module provides the following imported targets, if found:

``SndFile::SndFile``
  The SndFile library

This will define the following variables:

``SndFile_FOUND``
  True if the system has the SndFile library.

#]=======================================================================]

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(SndFile QUIET sndfile)
else()
  find_path(SndFile_INCLUDEDIR
    NAMES sndfile.h
    PATH_SUFFIXES sndfile
    DOC "SndFile include directory")

  find_library(SndFile_LINK_LIBRARIES
    NAMES sndfile sndfile-1
    DOC "SndFile library"
  )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  SndFile
  DEFAULT_MSG
  SndFile_LINK_LIBRARIES
  SndFile_INCLUDEDIR
)

file(STRINGS "${SndFile_INCLUDEDIR}/sndfile.h" SndFile_SUPPORTS_SET_COMPRESSION_LEVEL REGEX ".*SFC_SET_COMPRESSION_LEVEL.*")
if(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL)
  set(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL ON)
else()
  set(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL OFF)
endif()
mark_as_advanced(SndFile_SUPPORTS_SET_COMPRESSION_LEVEL)

if(SndFile_FOUND)
  if(NOT TARGET SndFile::sndfile)
    add_library(SndFile::sndfile INTERFACE IMPORTED)
    target_link_libraries(SndFile::sndfile INTERFACE "${SndFile_LINK_LIBRARIES}")
    target_include_directories(SndFile::sndfile INTERFACE "${SndFile_INCLUDEDIR}")
  endif()
endif()
