def_vars()

add_library(libflac INTERFACE)

set(BUILD_PROGRAMS OFF CACHE BOOL "Build and install programs")
set(BUILD_EXAMPLES OFF CACHE BOOL "Build and install examples")
set(BUILD_DOCS OFF CACHE BOOL "Build and install doxygen documents")
set(INSTALL_MANPAGES OFF CACHE BOOL "Install MAN pages")
set(INSTALL_PKGCONFIG_MODULES OFF CACHE BOOL "Install PkgConfig modules")
set(INSTALL_CMAKE_CONFIG_MODULE OFF CACHE BOOL "Install CMake package-config module")

# FLAC's default CMakeLists doesn't support building as a part of a larger project, hence this hack.
set(WITH_OGG OFF CACHE BOOL "ogg support (default: test for libogg)")
set(OGG_PACKAGE "ogg")

add_subdirectory("${TARGET_ROOT}" "${bindir}/libflac" EXCLUDE_FROM_ALL)

get_directory_property(VERSION DIRECTORY "${TARGET_ROOT}" DEFINITION VERSION)
project(libflac VERSION ${VERSION})

target_compile_definitions( libflac INTERFACE FLAC__NO_DLL )
target_include_directories( libflac INTERFACE "${TARGET_ROOT}/include" )
target_link_libraries( libflac INTERFACE FLAC FLAC++ )
