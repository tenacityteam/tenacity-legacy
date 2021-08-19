#                 /\_/\
#        ________(' - ')
#  _____(              )
# /     (______________)
# '     | |          | |
#       | |          | |  hello
#       '"'          '"'


# If you want built-in precompiled header support
# then make sure you have cmake 3.16 or higher.
#
# Minimum required is 3.15 due to use of multiple values in
# generator expressions.
cmake_minimum_required( VERSION 3.15 )

find_package(Git)

if(WIN32 OR APPLE)
  option(VCPKG "Use vcpkg for dependencies" ON)
else()
  option(VCPKG "Use vcpkg for dependencies" OFF)
endif()

if(VCPKG)
  set(ENV{VCPKG_DISABLE_METRICS} true)

  if(VCPKG_ROOT)
    message(STATUS "Using dependencies from vcpkg repository at ${VCPKG_ROOT}")
    if(NOT EXISTS "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
      message(FATAL_ERROR "${VCPKG_ROOT} is not a vcpkg Git repository.")
    endif()
  else()
    message(STATUS "Using dependencies from vcpkg Git submodule")
    set(VCPKG_ROOT "${CMAKE_SOURCE_DIR}/vcpkg")

    if(NOT EXISTS "${VCPKG_ROOT}/bootstrap-vcpkg.sh")
      message(STATUS "Initializing vcpkg Git submodule")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule init WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
      execute_process(COMMAND ${GIT_EXECUTABLE} submodule update WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    endif()
  endif()

  if(NOT DEFINED VCPKG_OVERLAY_PORTS)
    set(VCPKG_OVERLAY_PORTS "${VCPKG_ROOT}/overlay/ports")
  endif()

  if(NOT DEFINED VCPKG_OVERLAY_TRIPLETS)
    set(VCPKG_OVERLAY_TRIPLETS "${VCPKG_ROOT}/overlay/triplets")
  endif()

  if(NOT DEFINED ENV{VCPKG_DEFAULT_TRIPLET} AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(APPLE)
      set(VCPKG_TARGET_TRIPLET "x64-osx-10.12min")
    endif()
  elseif(DEFINED ENV{VCPKG_DEFAULT_TRIPLET})
    set(VCPKG_TARGET_TRIPLET "$ENV{VCPKG_DEFAULT_TRIPLET}")
  endif()

  set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
  message(STATUS "Searching for dependencies from system, not using vcpkg.")
endif()

# If building with GNU compiler, then must be 4.9 or later.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
    CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
   message(FATAL_ERROR "Tenacity requires at least GCC 4.9")
endif ()

# We only do alpha builds, beta builds, and release versions.
# Most of the time we're in development, so AUDACITY_BUILD_LEVEL should be
# defined to 0.
# Its value may be more than 0 for pre-release "Beta" builds that differ only
# in the welcome screen, and hiding of some development menu commands, but
# still link to the alpha manual online.

# Set this value to 0 for alpha, 1 for beta, 2 for release builds
set( AUDACITY_BUILD_LEVEL 0 CACHE STRING "0 for alpha, 1 for beta, 2 for release builds" )

# Don't allow in-source builds...no real reason, just
# keeping those source trees nice and tidy.  :-)
# (This can be removed if it becomes an issue.)
if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}" )
   message( FATAL_ERROR
      "In-source builds not allowed.\n"
      "Create a new directory and run cmake from there, i.e.:\n"
      "  mkdir build\n"
      "  cd build\n"
      "  cmake ..\n"
      "You will need to delete CMakeCache.txt and CMakeFiles from this directory to clean up."
   )
endif()

# Default build type is Debug
if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
   set( CMAKE_BUILD_TYPE "Debug" )
endif()

# List of defines which are intended to silence C++17 warnings
# These will be added to each CMAKE target as PRIVATE compilation definitions
list( APPEND CXX_WARNINGS_SILENCE_DEFINES _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING=1 )
list( APPEND CXX_WARNINGS_SILENCE_DEFINES _SILENCE_CXX17_NEGATORS_DEPRECATION_WARNING=1 )

# Ignore COMPILE_DEFINITIONS_<Config> properties
cmake_policy( SET CMP0043 NEW )

# Link libraries by full path even in implicit directories.
cmake_policy( SET CMP0060 NEW )

# ``INTERPROCEDURAL_OPTIMIZATION`` is enforced when enabled.
cmake_policy( SET CMP0069 NEW )

# ``FindOpenGL`` prefers GLVND by default when available.
cmake_policy( SET CMP0072 NEW )

# Include file check macros honor ``CMAKE_REQUIRED_LIBRARIES``.
cmake_policy( SET CMP0075 NEW )

# Definitions that must happen before the project() command
if( APPLE )
   set( MIN_MACOS_VERSION 10.12 )
   set( TARGET_MACOS_VERSION 10.13 )
   # Generate schema files
   set( CMAKE_XCODE_GENERATE_SCHEME ON )

   # Define the OSX compatibility parameters
   set( CMAKE_OSX_ARCHITECTURES x86_64 CACHE INTERNAL "" )
   set( CMAKE_OSX_DEPLOYMENT_TARGET ${MIN_MACOS_VERSION} CACHE INTERNAL "" )
   set( CMAKE_OSX_SYSROOT macosx CACHE INTERNAL "" )
   set( CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "" CACHE INTERNAL "" )

   # This prevents a link error when building with the 10.9 or older SDKs
   set( CMAKE_XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME OFF )

   # Shouldn't cmake do this???
   string( APPEND CMAKE_CXX_FLAGS " -stdlib=libc++" )
endif()

# Add our module path
# CMAKE_BINARY_DIR is required for Conan to work
set( AUDACITY_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-proxies/cmake-modules")
set( CMAKE_MODULE_PATH
   ${AUDACITY_MODULE_PATH}
   ${CMAKE_BINARY_DIR}
   ${CMAKE_MODULE_PATH}
   "${CMAKE_SOURCE_DIR}/cmake-modules"
)

set( CMAKE_PREFIX_PATH
   ${CMAKE_BINARY_DIR}
   ${CMAKE_PREFIX_PATH}
)

# This "is a good thing" but greatly increases link time on Linux
#set( CMAKE_INTERPROCEDURAL_OPTIMIZATION ON )
#set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON )
#set( CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF )

# Set the required C++ standard
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

include( CMakeDependentOption )

project( Tenacity )

# Use ccache if available
find_program( CCACHE_PROGRAM ccache )
mark_as_advanced( FORCE CCACHE_PROGRAM )
if(NOT "${CCACHE_PROGRAM}" STREQUAL "CCACHE_PROGRAM-NOTFOUND")
  option(CCACHE "Use ccache for compiler caching to speed up rebuilds." ON)
endif()

find_program( SCCACHE_PROGRAM sccache )
mark_as_advanced( FORCE SCCACHE_PROGRAM )
if(NOT "${SCCACHE_PROGRAM}" STREQUAL "SCCACHE_PROGRAM-NOTFOUND")
  option(SCCACHE "Use sccache for compiler caching to speed up rebuilds." ON)
endif()

# Prefer sccache if both ccache and sccache are found because Windows users may have
# ccache installed with MinGW which would not work with MSVC.
if(SCCACHE)
   message( STATUS "Using sccache for compiler caching to speed up rebuilds" )
   set( CMAKE_C_COMPILER_LAUNCHER "${SCCACHE_PROGRAM}" )
   set( CMAKE_CXX_COMPILER_LAUNCHER "${SCCACHE_PROGRAM}" )
elseif(CCACHE)
   message( STATUS "Using ccache for compiler caching to speed up rebuilds" )
   set( CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" )
   set( CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" )
else()
   message( STATUS "No compiler caching enabled. Install sccache or ccache to enable compiler caching." )
endif()

# Load our functions/macros
include( AudacityFunctions )

set_from_env(AUDACITY_ARCH_LABEL) # e.g. x86_64

# Pull all the modules we'll need
include( CheckCXXCompilerFlag )
include( CheckIncludeFile )
include( CheckIncludeFiles )
include( CheckLibraryExists )
include( CheckSymbolExists )
include( CheckTypeSize )
include( CMakeDetermineASM_NASMCompiler )
include( CMakePushCheckState )
include( GNUInstallDirs )
include( TestBigEndian )
include( ProcessorCount )

# Determine total number of processors to enable us to only use total_available - 1 processors
# This helps prevent slow down of the other applications during the build process
ProcessorCount( CMAKE_BUILD_CPU_COUNT )
if( CMAKE_BUILD_CPU_COUNT GREATER 0 )
  math( EXPR CMAKE_BUILD_CPU_COUNT "${CMAKE_BUILD_CPU_COUNT} - 2" OUTPUT_FORMAT DECIMAL )
endif()

if( CMAKE_BUILD_CPU_COUNT LESS_EQUAL 0 )
  message( WARNING "Unable to optimize build CPU usage, defaulting to use all processors" )
  set( CMAKE_BUILD_CPU_COUNT 0 )
elseif(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL )
  message( STATUS "Build automatically optimized to use processor count: ${CMAKE_BUILD_CPU_COUNT}" )
  set( CMAKE_BUILD_PARALLEL_LEVEL ${CMAKE_BUILD_CPU_COUNT} )
#else()
#  message( STATUS "Using CMAKE_BUILD_PARALLEL_LEVEL value to configure processor count: ${CMAKE_BUILD_PARALLEL_LEVEL}")
#  set ( CMAKE_BUILD_CPU_COUNT ${CMAKE_BUILD_PARALLEL_LEVEL} )
endif()

# Determine 32-bit or 64-bit target
if( CMAKE_C_COMPILER_ID MATCHES "MSVC" AND CMAKE_VS_PLATFORM_NAME MATCHES "Win64|x64" )
   set( IS_64BIT ON )
elseif( NOT CMAKE_SIZEOF_VOID_P STREQUAL "4" )
   set( IS_64BIT ON )
endif()

message( STATUS "Build Info:" )
message( STATUS "  Host System: ${CMAKE_HOST_SYSTEM}" )
message( STATUS "  Host System Name: ${CMAKE_HOST_SYSTEM_NAME}" )
message( STATUS "  Host System Processor: ${CMAKE_HOST_SYSTEM_PROCESSOR}" )
message( STATUS "  Host System Version: ${CMAKE_HOST_SYSTEM_VERSION}" )

if( IS_64BIT )
   message( STATUS "  Host System Architecture: 64-bit" )
else()
   message( STATUS "  Host System Architecture: 32-bit" )
endif()

message( STATUS )
message( STATUS "  Compiler: ${CMAKE_CXX_COMPILER}" )
message( STATUS "  Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}" )
message( STATUS "  Compiler Standard: ${CMAKE_CXX_STANDARD}" )
message( STATUS "  Compiler Standard Required: ${CMAKE_CXX_STANDARD_REQUIRED}" )
message( STATUS "  Compiler Extensions: ${CMAKE_CXX_EXTENSIONS}" )
message( STATUS )

if( MSVC )
   message( STATUS "  MSVC Version: ${MSVC_VERSION}" )
   message( STATUS "  MSVC Toolset: ${MSVC_TOOLSET_VERSION}" )
   message( STATUS )
elseif( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
   if( CMAKE_GENERATOR MATCHES "Xcode" )
      message( STATUS "  Xcode Version: ${XCODE_VERSION}" )
   endif()
   message( STATUS "  MacOS SDK: ${CMAKE_OSX_SYSROOT}" )
   message( STATUS )

endif()

# Current version and commit info
set( AUDACITY_VERSION 0 )
set( AUDACITY_RELEASE 0 )
set( AUDACITY_REVISION 0 )
set( GIT_DESCRIBE "unknown" )
if( GIT_FOUND )
    execute_process(
        COMMAND
            ${GIT_EXECUTABLE} describe --abbrev=7
        WORKING_DIRECTORY
            ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE
            GIT_DESCRIBE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    if( GIT_DESCRIBE )
        # Copy to new variable for string manipulation
        set( git_output ${GIT_DESCRIBE} )

        # TODO: Remove this after first Tenacity release
        string(REPLACE "Audacity-" "" git_output "${git_output}")

        string( REGEX REPLACE "-.*" "" git_output "${git_output}" )
        string( REPLACE "." ";" git_output "${git_output}" )

        list( GET git_output 0 AUDACITY_VERSION )
        list( GET git_output 1 AUDACITY_RELEASE )
        list( GET git_output 2 AUDACITY_REVISION )
    endif()
endif()
message( STATUS "  Current Commit: ${GIT_DESCRIBE}" )
message( STATUS )

# Not sure what this even does
set( AUDACITY_MODLEVEL 0 )

# Organize subdirectories/targets into folders for the IDEs
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

if( CMAKE_GENERATOR MATCHES "Visual Studio" )
   # Make sure Tenacity is the startup project
   set_directory_properties(
      PROPERTIES
         VS_STARTUP_PROJECT "${CMAKE_PROJECT_NAME}"
   )

   # Build using multiple processors
   foreach( config ${CMAKE_CONFIGURATION_TYPES} )
      string( TOUPPER "${config}" config )
      if( NOT CMAKE_BUILD_CPU_COUNT EQUAL 0 )
        string( APPEND CMAKE_C_FLAGS_${config} " /MP${CMAKE_BUILD_CPU_COUNT}" )
        string( APPEND CMAKE_CXX_FLAGS_${config} " /MP${CMAKE_BUILD_CPU_COUNT}" )
      else()
        string( APPEND CMAKE_C_FLAGS_${config} " /MP" )
        string( APPEND CMAKE_CXX_FLAGS_${config} " /MP" )
      endif()
   endforeach()

   # Define system library information, but we'll do the install
   set( CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP YES )
   set( CMAKE_INSTALL_UCRT_LIBRARIES NO )
   set( CMAKE_INSTALL_MFC_LIBRARIES NO )
   set( CMAKE_INSTALL_OPENMP_LIBRARIES NO )
   include( InstallRequiredSystemLibraries )
endif()

set ( _SHARED_PROXY_BASE "shared" )
set ( _SHARED_PROXY_BASE_PATH "${CMAKE_BINARY_DIR}/${_SHARED_PROXY_BASE}")

# Define the non-install and executable paths and where the final product is stored
if( CMAKE_CONFIGURATION_TYPES )
   set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
   set( _DESTDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR}" )
   set( _SHARED_PROXY_PATH "${_SHARED_PROXY_BASE_PATH}/${CMAKE_CFG_INTDIR}")
else()
   set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE} )
   set( _DESTDIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/" )
   set( _SHARED_PROXY_PATH "${_SHARED_PROXY_BASE_PATH}/${CMAKE_BUILD_TYPE}")
endif()

set( _DEST "${_DESTDIR}" )
set( _PREFIX "${CMAKE_INSTALL_PREFIX}" )
set( _LIBDIR "${CMAKE_INSTALL_LIBDIR}" )
set( _DATADIR "${CMAKE_INSTALL_DATADIR}" )
set( _PKGLIB "${_LIBDIR}/tenacity" )
set( _PKGDATA "${_DATADIR}/tenacity/" )
set( _MANDIR "${CMAKE_INSTALL_MANDIR}" )
set( _MODDIR "${_DEST}/modules" )
set( _EXEDIR "${_DEST}" )

# Setup RPATH handling
set( CMAKE_BUILD_RPATH "${_DEST}/${_PKGLIB}" )
set( CMAKE_BUILD_WITH_INSTALL_RPATH FALSE )
set( CMAKE_INSTALL_RPATH "${_PREFIX}/${_PKGLIB}" )
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE )

# Adjust them for the Mac
if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
   set( _APPDIR "Tenacity.app/Contents" )
   set( _DEST "${_DESTDIR}/${_APPDIR}" )
   set( _EXEDIR "${_DEST}/MacOS" )
   set( _MODDIR "${_DEST}/modules" )
   set( _PKGLIB "${_DEST}/Frameworks" )

   set( CMAKE_MACOSX_RPATH OFF )
endif()

# Add the math library (if found) to the list of required libraries
check_library_exists( m pow "" HAVE_LIBM )
if( HAVE_LIBM )
   list( APPEND CMAKE_REQUIRED_LIBRARIES -lm )
endif()

check_library_exists( atomic __atomic_fetch_add_4 "" HAVE_LIBATOMIC )
if( HAVE_LIBATOMIC )
  list( APPEND CMAKE_REQUIRED_LIBRARIES -latomic )
endif()

# Add the dynamic linker library (if needed) to the list of required libraries
list( APPEND CMAKE_REQUIRED_LIBRARIES ${CMAKE_DL_LIBS} )

# Make sure they're used during the link steps
set( CMAKE_LINK_INTERFACE_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} )

# Various common checks whose results are used by the different targets
test_big_endian( WORDS_BIGENDIAN )

# Check for compiler flags
set( MMX_FLAG "" CACHE INTERNAL "" )
set( SSE_FLAG "" CACHE INTERNAL "" )
if( CMAKE_CXX_COMPILER_ID MATCHES "AppleClang|Clang|GNU" )
   check_cxx_compiler_flag( "-mmmx" HAVE_MMX )
   if( HAVE_MMX AND NOT IS_64BIT )
      set( MMX_FLAG "-mmmx" CACHE INTERNAL "" )
   endif()

   check_cxx_compiler_flag( "-msse" HAVE_SSE )
   if( HAVE_SSE AND NOT IS_64BIT )
      set( SSE_FLAG "-msse" CACHE INTERNAL "" )
   endif()

   check_cxx_compiler_flag( "-msse2" HAVE_SSE2 )
   if( HAVE_SSE2 AND NOT IS_64BIT )
      set( SSE_FLAG "-msse2" CACHE INTERNAL "" )
   endif()
elseif( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
   set( HAVE_MMX ON )
   set( HAVE_SSE ON )
   set( HAVE_SSE2 ON )
   if( NOT IS_64BIT )
      set( SSE_FLAG "/arch:SSE2" )
   endif()

   # required for sccache
   if(CMAKE_BUILD_TYPE STREQUAL "Debug")
      string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
      string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
      string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
      string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
      string(REPLACE "/Zi" "/Z7" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
      string(REPLACE "/Zi" "/Z7" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    endif()
endif()

check_include_files( "float.h;stdarg.h;stdlib.h;string.h" STDC_HEADERS )

check_include_file( "assert.h" HAVE_ASSERT_H )
check_include_file( "errno.h" HAVE_ERRNO_H )
check_include_file( "fcntl.h" HAVE_FCNTL_H )
check_include_file( "fenv.h" HAVE_FENV_H )
check_include_file( "inttypes.h" HAVE_INTTYPES_H )
check_include_file( "limits.h" HAVE_LIMITS_H )
if( CMAKE_SYSTEM_NAME MATCHES "FreeBSD" )
  check_include_file( "stdlib.h" HAVE_MALLOC_H )
  check_include_file( "sys/endian.h" HAVE_ENDIAN_H )
else()
  check_include_file( "malloc.h" HAVE_MALLOC_H )
  check_include_file( "byteswap.h" HAVE_BYTESWAP_H )
endif()
check_include_file( "memory.h" HAVE_MEMORY_H )
check_include_file( "stdbool.h" HAVE_STDBOOL_H )
check_include_file( "stdint.h" HAVE_STDINT_H )
check_include_file( "stdlib.h" HAVE_STDLIB_H )
check_include_file( "string.h" HAVE_STRING_H )
check_include_file( "strings.h" HAVE_STRINGS_H )
check_include_file( "unistd.h" HAVE_UNISTD_H )
check_include_file( "xmmintrin.h" HAVE_XMMINTRIN_H )
check_include_file( "sys/param.h" HAVE_SYS_PARAM_H )
check_include_file( "sys/stat.h" HAVE_SYS_STAT_H )
check_include_file( "sys/types.h" HAVE_SYS_TYPES_H )
check_include_file( "sys/wait.h" HAVE_SYS_WAIT_H )

check_symbol_exists( bcopy "strings.h" HAVE_BCOPY )
check_symbol_exists( fileno "stdio.h" HAVE_FILENO )
check_symbol_exists( flock "sys/file.h" HAVE_FLOCK )
check_symbol_exists( fork "unistd.h" HAVE_FORK )
check_symbol_exists( fsync "unistd.h" HAVE_FSYNC )
check_symbol_exists( ftruncate "unistd.h" HAVE_FTRUNCATE )
check_symbol_exists( getpagesize "unistd.h" HAVE_GETPAGESIZE )
check_symbol_exists( gettimeofday "sys/time.h" HAVE_GETTIMEOFDAY )
check_symbol_exists( gmtime "time.h" HAVE_GMTIME )
check_symbol_exists( gmtime_r "time.h" HAVE_GMTIME_R )
check_symbol_exists( lrint "math.h" HAVE_LRINT )
check_symbol_exists( lrintf "math.h" HAVE_LRINTF )
check_symbol_exists( lround "math.h" HAVE_LROUND )
check_symbol_exists( lstat "sys/stat.h" HAVE_LSTAT )
check_symbol_exists( memcpy "string.h" HAVE_MEMCPY )
check_symbol_exists( memmove "string.h" HAVE_MEMMOVE )
check_symbol_exists( mlock "sys/mman.h" HAVE_MLOCK )
check_symbol_exists( pipe "unistd.h" HAVE_PIPE )
check_symbol_exists( posix_fadvise "fcntl.h" HAVE_POSIX_FADVISE )
check_symbol_exists( posix_memalign "stdlib.h" HAVE_POSIX_MEMALIGN )
check_symbol_exists( strchr "string.h" HAVE_STRCHR )
check_symbol_exists( waitpid "sys/wait.h" HAVE_WAITPID )

check_type_size( "int8_t" SIZEOF_INT8 LANGUAGE C )
check_type_size( "int16_t" SIZEOF_INT16 LANGUAGE C )
check_type_size( "uint16_t" SIZEOF_UINT16 LANGUAGE C )
check_type_size( "u_int16_t" SIZEOF_U_INT16 LANGUAGE C )
check_type_size( "int32_t" SIZEOF_INT32 LANGUAGE C )
check_type_size( "uint32_t" SIZEOF_UINT32 LANGUAGE C )
check_type_size( "u_int32_t" SIZEOF_U_INT32 LANGUAGE C )
check_type_size( "int64_t" SIZEOF_INT64 LANGUAGE C )
check_type_size( "short" SIZEOF_SHORT LANGUAGE C )
check_type_size( "unsigned short" SIZEOF_UNSIGNED_SHORT LANGUAGE C )
check_type_size( "int" SIZEOF_INT LANGUAGE C )
check_type_size( "unsigned int" SIZEOF_UNSIGNED_INT LANGUAGE C )
check_type_size( "long" SIZEOF_LONG LANGUAGE C )
check_type_size( "unsigned long" SIZEOF_UNSIGNED_LONG LANGUAGE C )
check_type_size( "long long" SIZEOF_LONG_LONG LANGUAGE C )
check_type_size( "unsigned long long" SIZEOF_UNSIGNED_LONG_LONG LANGUAGE C )
check_type_size( "float" SIZEOF_FLOAT LANGUAGE C )
check_type_size( "double" SIZEOF_DOUBLE LANGUAGE C )
check_type_size( "long double" SIZEOF_LONG_DOUBLE LANGUAGE C )
check_type_size( "loff_t" SIZEOF_LOFF LANGUAGE C )
check_type_size( "off_t" SIZEOF_OFF LANGUAGE C )
check_type_size( "off64_t" SIZEOF_OFF64 LANGUAGE C )
check_type_size( "size_t" SIZEOF_SIZE LANGUAGE C )
check_type_size( "wchar_t" SIZEOF_WCHAR LANGUAGE C )
check_type_size( "void*" SIZEOF_POINTER LANGUAGE C )

# We'll be using it if it's available
find_package( PkgConfig QUIET )

# Mostly just to make the CMP0072 policy happy
find_package( OpenGL QUIET )

# Precreate the lib and lib64 directories so we can make then the same
if( NOT EXISTS "${CMAKE_BINARY_DIR}/lib" )
   file( MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/lib" )
endif()

# Only create on systems that need it, effectively excluding Windows where links
# may not work due to insufficient privileges
if( NOT CMAKE_INSTALL_LIBDIR STREQUAL "lib" AND NOT EXISTS "${CMAKE_BINARY_DIR}/lib64" )
   file( CREATE_LINK "${CMAKE_BINARY_DIR}/lib" "${CMAKE_BINARY_DIR}/lib64" SYMBOLIC )
endif()

# Define Audacity's name
if( CMAKE_SYSTEM_NAME MATCHES "Darwin|Windows" )
   set( AUDACITY_NAME "Tenacity" )
else()
   set( AUDACITY_NAME "tenacity" )
endif()

# Python is used for the manual and (possibly) message catalogs
find_package( Python3 )
if( Python3_FOUND )
   set( PYTHON "${Python3_EXECUTABLE}" )
elseif( CMAKE_SYSTEM_NAME MATCHES "Windows" )
   # This is an odd case now, as Conan requires Python as well
   nuget_package( pkgdir "python3" "3.7.7" )
   file( TO_NATIVE_PATH "${pkgdir}/tools/python.exe" PYTHON )
endif()

# define EXPERIMENTAL flags
# Do this before consistency checks for added third-party libraries
include( "src/Experimental.cmake" )

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(EXPAT REQUIRED)
find_package(mp3lame REQUIRED)
find_package(SndFile REQUIRED)
find_package(Soxr REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(PortAudio REQUIRED)

find_package(PortMidi)
find_package(PortSMF)
cmake_dependent_option(MIDI "MIDI support requires PortMidi and PortSMF." ON "PortMidi_FOUND;PortSMF_FOUND" OFF)
if(MIDI)
    set(USE_MIDI ON)
    message(STATUS "MIDI support enabled.")
else()
    message(STATUS "MIDI support disabled. Requires both PortMidi and PortSMF.")
endif()

find_package(id3tag)
cmake_dependent_option(ID3TAG "ID3 Tag support for MP3s." ON "id3tag_FOUND" OFF)
if(ID3TAG)
    set(USE_LIBID3TAG ON)
    message(STATUS "ID3 tag support for MP3s enabled.")
else()
    message(STATUS "ID3 tag support for MP3s disabled. Requires libid3tag.")
endif()

find_package(MAD)
cmake_dependent_option(MP3_DECODING "MP3 decoding support with libmad" ON "MAD_FOUND" OFF)
if(MP3_DECODING)
    set(USE_LIBMAD ON)
    message(STATUS "MP3 decoding support enabled.")
else()
    message(STATUS "MP3 decoding support disabled. Requires libmad.")
endif()

find_package(libtwolame)
cmake_dependent_option(MP2_ENCODING "MP2 support with Twolame" ON "libtwolame_FOUND" OFF)
if(MP2_ENCODING)
    set(USE_LIBTWOLAME ON)
    message(STATUS "MP2 encoding support enabled.")
else()
    message(STATUS "MP2 encoding support disabled. Requires Twolame library.")
endif()

find_package(Ogg)
cmake_dependent_option(OGG "OGG container format support" ON "Ogg_FOUND" OFF)
if(OGG)
    set(USE_LIBOGG ON)
    message(STATUS "OGG container format support enabled.")
else()
    message(STATUS "OGG container format support disabled. Requires libogg.")
endif()

find_package(Vorbis)
cmake_dependent_option(VORBIS "Vorbis codec support" ON "Vorbis_FOUND" OFF)
if(VORBIS)
    set(USE_LIBVORBIS ON)
    message(STATUS "Vorbis codec support enabled.")
else()
    message(STATUS "Voribs codec support disabled. Requires libvorbis.")
endif()

find_package(FLAC++)
cmake_dependent_option(FLAC "FLAC codec support" ON "FLAC++_FOUND" OFF)
if(FLAC)
    set(USE_LIBFLAC ON)
    message(STATUS "FLAC codec support enabled.")
else()
    message(STATUS "FLAC codec support disabled. Requires libflac and libflac++ C++ bindings.")
endif()

# FIXME: requires vendored fork of PortMixer which requires vendored fork of PortAudio
# https://github.com/audacity/audacity/issues/840#issuecomment-837795388
#find_package(PortMixer)
#cmake_dependent_option(PORTMIXER "PortMixer support" ON "PortMixer_FOUND" OFF)
#if(PORTMIXER)
    #set(USE_PORTMIXER ON)
    #message(STATUS "PortMixer support enabled.")
#else()
    #message(STATUS "PortMixer support disabled.")
#endif()

find_package(sbsms)
cmake_dependent_option(SBSMS "SBSMS timestretching" ON "sbsms_FOUND" OFF)
if(SBSMS)
    set(USE_SBSMS ON)
    message(STATUS "SBSMS timestretching support enabled.")
else()
    message(STATUS "SBSMS timestretching support disabled. Requires libsbsms.")
endif()

find_package(SoundTouch)
cmake_dependent_option(SOUNDTOUCH "SoundTouch timestretching" ON "SoundTouch_FOUND" OFF)
if(SOUNDTOUCH)
    set(USE_SOUNDTOUCH ON)
    message(STATUS "SoundTouch timestretching support enabled.")
else()
    message(STATUS "SoundTouch timestretching support disabled. Requires SoundTouch library.")
endif()

find_package(FFMPEG)
cmake_dependent_option(FFMPEG "FFMPEG codecs support." ON "FFMPEG_FOUND" OFF)
if(FFMPEG)
    set(USE_FFMPEG ON)
    message(STATUS "FFMPEG codecs support enabled.")
else()
    message(STATUS "FFMPEG codecs support disabled. Requires FFMPEG libraries.")
endif()

find_package(VampHostSDK)
cmake_dependent_option(VAMP "VAMP plugin hosting." ON "VampHostSDK_FOUND" OFF)
if(VAMP)
    set(USE_VAMP ON)
    message(STATUS "VAMP plugin hosting enabled.")
else()
    message(STATUS "VAMP plugin hosting disabled. Requires VAMP host SDK.")
endif()

find_package(LV2)
find_package(lilv)
find_package(suil)
cmake_dependent_option(LV2 "LV2 plugin host support" ON "LV2_FOUND;lilv_FOUND;suil_FOUND" OFF)
if(LV2)
    message(STATUS "LV2 plugin hosting enabled.")
    set(USE_LV2 ON)
else()
    message(STATUS "LV2 plugin hosting disabled. Requires LV2, lilv, and suil libraries.")
endif()

option(VST2 "VST2 plugin host support" ON)
if(VST2)
    message(STATUS "VST2 plugin host support enabled.")
else()
    message(STATUS "VST2 plugin host support disabled.")
endif()

if(NOT CMAKE_SYSTEM_NAME MATCHES "Darwin|Windows")
    find_package(GLIB REQUIRED)
    find_package(GTK 3.0 REQUIRED)

endif()

if(NOT WIN32)
  set(wxWidgets_CONFIG_OPTIONS --version=3.1)
  find_package(wxWidgets 3.1 COMPONENTS adv base core html qa xml net)
  include(${wxWidgets_USE_FILE})
  # The FindwxWidgets.cmake module does not create an IMPORT target, so hack one together.
  # This makes it easy to add the compile definitions to the lib-strings and lib-strings-utils targets.
  if(NOT TARGET wxWidgets::wxWidgets)
    add_library(wxWidgets::wxWidgets INTERFACE IMPORTED)
    target_link_libraries(wxWidgets::wxWidgets INTERFACE ${wxWidgets_LIBRARIES})
    target_compile_definitions(wxWidgets::wxWidgets INTERFACE ${CXX_WARNINGS_SILENCE_DEFINES})
    target_compile_definitions(wxWidgets::wxWidgets INTERFACE ${wxWidgets_DEFINITIONS} ${wxWidgets_DEFINITIONS_DEBUG})
  endif()
else()
  # On Windows we are using wxWidgets from an unmerged vcpkg branch in our vcpkg overlay
  # https://github.com/microsoft/vcpkg/pull/17111
  # This requires using 'unofficial-wxWidgets' rather than simply 'wxWidgets'.
  # This is required for CMAKE_BUILD_TYPE=Debug to work. Refer to
  # https://github.com/microsoft/vcpkg/issues/18066
  # for background about the bug.
  find_package(unofficial-wxWidgets COMPONENTS adv base core html qa xml net)
  if(NOT TARGET wxWidgets::wxWidgets)
    add_library(wxWidgets::wxWidgets INTERFACE IMPORTED)
    target_link_libraries(wxWidgets::wxWidgets INTERFACE wx::wxadv wx::wxbase wx::wxcore wx::wxhtml wx::wxqa wx::wxxml wx::wxnet)
  endif()
endif()

add_subdirectory(lib-src/libnyquist)
set(USE_NYQUIST ON)

add_subdirectory( "help" )
add_subdirectory( "images" )
add_subdirectory( "libraries" )
add_subdirectory( "locale" )
add_subdirectory( "src" )
add_subdirectory( "modules" )
add_subdirectory( "nyquist" )
add_subdirectory( "plug-ins" )
add_subdirectory( "scripts" )

# Generate config file
if( CMAKE_SYSTEM_NAME MATCHES "Windows" )
   configure_file( src/audacity_config.h.in src/private/configwin.h )
elseif( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
   set( HAVE_VISIBILITY 1 )
   configure_file( src/audacity_config.h.in src/private/configmac.h )
else()
   set( HAVE_VISIBILITY 1 )
   configure_file( src/audacity_config.h.in src/private/configunix.h )
endif()

# Generate a picture of module dependencies
string( JOIN "\n" GRAPH_EDGES ${GRAPH_EDGES} )
# Choose edge attributes making it easy to hover at either end of edge
# and see a tooltip describing the edge, in svg image
file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/modules.dot" "digraph {
   graph [rankdir=LR] edge [dir=both,arrowtail=inv] \n"
   "${GRAPH_EDGES}"
   "\n}\n"
)
execute_process( COMMAND
   dot -O -Tsvg "${CMAKE_CURRENT_BINARY_DIR}/modules.dot" )

#
# Code signing
#

cmd_option( ${_OPT}perform_codesign
            "Perform code signing during the install step. This only works on Windows and macOS."
            Off
)

cmake_dependent_option(
   ${_OPT}perform_notarization
   "Perform notarization during the install step. This only works on macOS."
   Off
   "${_OPT}perform_codesign;APPLE"
   Off
)

if( ${_OPT}perform_codesign )
   include( AudacityCodeSigning )
endif()

#
# Packaging
#

cmd_option( ${_OPT}package_manual
            "Package the manual along with the DMG and InnoSetup targets"
            Off
)

# Variables, that are common for all package targets

if( CMAKE_SYSTEM_NAME MATCHES "Windows" )
   include( AudacityInnoSetup )
endif()

# Uncomment what follows for symbol values.
#[[
   get_cmake_property( _variableNames VARIABLES )
   foreach( _variableName ${_variableNames} )
      message( STATUS "${_variableName}=${${_variableName}}" )
   endforeach()
#]]
#[[
   include( PrintProperties )
   print_properties( TARGET "wxWidgets" )
#]]

include( Package ) # do this last
