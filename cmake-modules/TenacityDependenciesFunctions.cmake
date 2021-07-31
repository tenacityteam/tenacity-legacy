# Load Conan
include( conan )
include( FindPkgConfig )

if(USE_CONAN)
    conan_add_remote(NAME audacity
        URL https://artifactory.audacityteam.org/artifactory/api/conan/conan-local
        VERIFY_SSL True
    )
endif()

set( CONAN_BUILD_REQUIRES )
set( CONAN_REQUIRES )
set( CONAN_PACKAGE_OPTIONS )
set( CONAN_ONLY_DEBUG_RELEASE )
set( CONAN_CONFIG_OPTIONS )
set( CONAN_RESOLVE_LIST )
set( FIND_LIBS_RESOLVE_LIST )

# Add a external dependency
# Example usage:
# add_external_lib( 
#   wxWidgets 
#   wxwidgets/3.1.3-audacity
#   OPTION_NAME wxwidgets
#   SYMBOL WXWIDGET
#   REQUIRED 
#   FIND_PACKAGE
#   ALWAYS_ALLOW_CONAN_FALLBACK
#   PKG_CONFIG "wxwidgets >= 3.1.3"
#   FIND_PACKAGE_OPTIONS COMPONENTS adv base core html qa xml
#   INTERFACE_NAME wxwidgets::wxwidgets
#   HAS_ONLY_DEBUG_RELEASE
#   CONAN_OPTIONS 
#        wxwidgets:shared=True
# )

function( add_external_lib package conan_package_name )
    list(REMOVE_AT ARGV 0 1 )
    better_parse_args(
        . REQUIRED FIND_PACKAGE ALWAYS_ALLOW_CONAN_FALLBACK HAS_ONLY_DEBUG_RELEASE
        - OPTION_NAME SYMBOL PKG_CONFIG INTERFACE_NAME
        + CONAN_OPTIONS FIND_PACKAGE_OPTIONS
    )

    set( option_name_base ${package} )
    set( required off )
    set( no_pkg off )
    set( system_only ${${_OPT}obey_system_dependencies})
    set( interface_name "${package}::${package}")
    
    # Parse function arguments
    # TODO: Remove last leftovers from the old way of parsing args

    if (ARG_FIND_PACKAGE_OPTIONS )
        set( ARG_FIND_PACKAGE on )
    endif()
    if (ARG_OPTION_NAME )
        set( option_name_base ${ARG_OPTION_NAME})
    endif()
    if (ARG_SYMBOL )
        set( symbol ${ARG_SYMBOL})
    endif()
    if (ARG_INTERFACE_NAME )
        set( interface_name ${ARG_INTERFACE_NAME})
    endif()
    if (ARG_REQUIRED )
        set( required on )
    endif()
    if (ARG_ALWAYS_ALLOW_CONAN_FALLBACK )
        set( system_only off )
    endif()
    if (ARG_HAS_ONLY_DEBUG_RELEASE )
        set ( only_debug_release on )
    endif()

    if( NOT DEFINED symbol )
        string( TOUPPER "${option_name_base}" symbol)
    endif()

    # Generate CMake option
    set( option_name ${_OPT}use_${option_name_base} )

    set( option_desc "conan" )

    if(NOT USE_CONAN)
        set( sysopt "system" )
        set( default "system" )
        set( option_desc "system (forced), " )
        set( system_only on)
    elseif( ARG_PKG_CONFIG OR ARG_FIND_PACKAGE )
        set( sysopt "system" )
        string( PREPEND option_desc "system (if available), " )
        set( default "${${_OPT}external_lib_preference}" )
    else()
        set( default "conan" )
    endif()

    if( NOT required )
        set( reqopt "off" )
        string( APPEND option_desc ", off" )
    endif()

    cmd_option( ${option_name}
                "Use ${option_name_base} library [${option_desc}]"
                "${default}"
                STRINGS ${sysopt} "conan" ${reqopt}
    )
    
    # Early bail out
    if( ${option_name} STREQUAL "off" )

      message( STATUS "========== ${option_name_base} disabled ==========" )

      set( USE_${symbol} OFF CACHE INTERNAL "" FORCE )

      return()
    endif()

    # Let the Audacity target know that this library will be used
    set( USE_${symbol} ON CACHE INTERNAL "" FORCE )

    if ( TARGET "${package}" )
        return()
    endif()

    if( ${option_name} STREQUAL "system")
        if( ARG_PKG_CONFIG AND PKG_CONFIG_FOUND )
            pkg_check_modules( PKG_${package} ${ARG_PKG_CONFIG} )

            if( PKG_${package}_FOUND )
                message( STATUS "Using '${package}' system library (Found by pkg_config)" )
    
                # Create the target interface library
                add_library( ${interface_name} INTERFACE IMPORTED GLOBAL)
        
                # Retrieve the package information
                get_package_interface( PKG_${package} )
        
                # And add it to our target
                target_include_directories( ${interface_name} INTERFACE ${INCLUDES} )
                target_link_libraries( ${interface_name} INTERFACE ${LIBRARIES} )

                message(STATUS "Added inteface ${interface_name} ${INCLUDES} ${LIBRARIES}")
                return()
            endif()
        endif()

        if( ARG_FIND_PACKAGE OR NOT PKG_CONFIG_FOUND )
            find_package( ${package} ${find_package_options} )

            if ( ${package}_FOUND )
                message( STATUS "Using '${package}' system library (Found by find_package)" )

                # Need to refind the package later since we are in a function scope
                list( JOIN ARG_FIND_PACKAGE_OPTIONS "|" joined_package_options )
                list( APPEND FIND_LIBS_RESOLVE_LIST "${package}|${joined_package_options}" )
                lift_var( FIND_LIBS_RESOLVE_LIST )
                return()
            endif()
        endif()

        if( system_only )
            message( FATAL_ERROR "Failed to find the system package ${package}" )
        else()
            set( ${option_name} "conan" )
            set_property( CACHE ${option_name} PROPERTY VALUE "conan" )
        endif()
    endif()

    # We only want the name of the library
    string(REPLACE "/" ";" pkg_name_split ${conan_package_name})
    list(GET pkg_name_split 0 conan_package_name_only)

    list( APPEND CONAN_REQUIRES ${conan_package_name} )
    list( APPEND CONAN_PACKAGE_OPTIONS ${ARG_CONAN_OPTIONS} )
    list( APPEND CONAN_RESOLVE_LIST "${conan_package_name_only}|${interface_name}" )

    if ( only_debug_release )
        message( STATUS "${package} only has Debug and Release versions" )
        list( APPEND CONAN_ONLY_DEBUG_RELEASE ${package})
    endif()

    lift_var( CONAN_REQUIRES 
        CONAN_PACKAGE_OPTIONS
        CONAN_RESOLVE_LIST 
        CONAN_ONLY_DEBUG_RELEASE )
    
    message (STATUS "Adding Conan dependency ${package}")
endfunction()

function ( _conan_install build_type )
    conan_cmake_configure (
        REQUIRES ${CONAN_REQUIRES}
        GENERATORS cmake_multi
        BUILD_REQUIRES ${CONAN_BUILD_REQUIRES}
        ${CONAN_CONFIG_OPTIONS}
        IMPORTS "bin, *.dll -> ./${_SHARED_PROXY_BASE}/${build_type} @ keep_path=False"
        IMPORTS "lib, *.dll -> ./${_SHARED_PROXY_BASE}/${build_type} @ keep_path=False"
        IMPORTS "lib, *.dylib -> ./${_SHARED_PROXY_BASE}/${build_type} @ keep_path=False"
        IMPORTS "lib, *.so* -> ./${_SHARED_PROXY_BASE}/${build_type} @ keep_path=False"
        OPTIONS ${CONAN_PACKAGE_OPTIONS}
    )

    # Hash the conanfile.txt to check if it has changed, so we can skip running conan
    set(hash_name "${build_type}_CONAN_HASH")
    file(MD5 "${CMAKE_CURRENT_BINARY_DIR}/conanfile.txt" conanfile_hash)

    if( "_${conanfile_hash}" STREQUAL "_${${hash_name}}" AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake")
        message(STATUS "Conanfile up to date for ${build_type}, not running conan")
        return()
    endif()

    set(${hash_name} ${conanfile_hash} CACHE INTERNAL "MD5 hash of conanfile.txt for ${build_type}" FORCE)

    message(STATUS "Configuring packages for ${build_type}")

    conan_cmake_autodetect(settings BUILD_TYPE ${build_type})

    if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
        # TODO: Read the target CPU architecture from the CMake option
        # We have no AppleSilicon support yet
        list( APPEND settings "arch=x86_64" )
        list( APPEND settings "os.version=${CMAKE_OSX_DEPLOYMENT_TARGET}" )
        # This line is required to workaround the conan bug #8025
        # https://github.com/conan-io/conan/issues/8025
        # Without it, libjpeg-turbo will fail to cross-compile on AppleSilicon macs 
        list( APPEND settings ENV "CONAN_CMAKE_SYSTEM_PROCESSOR=x86_64")
    endif()

    if (build_type MATCHES "MinSizeRel|RelWithDebInfo")
        message(STATUS "Release only libraries: ${CONAN_ONLY_DEBUG_RELEASE}")

        foreach( package ${CONAN_ONLY_DEBUG_RELEASE} )
            list( APPEND settings "${package}:build_type=Release")
        endforeach()
    endif()

    message(STATUS 
    "Executing Conan: \
        REQUIRES ${CONAN_REQUIRES}
        GENERATORS cmake_multi
        BUILD_REQUIRES ${CONAN_BUILD_REQUIRES}
        ${CONAN_CONFIG_OPTIONS}
        OPTIONS ${CONAN_PACKAGE_OPTIONS}
        SETTINGS ${settings}
    ")

    conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        SETTINGS ${settings}
    )
endfunction()

macro( resolve_conan_dependencies )
    if(USE_CONAN)
        if(MSVC OR XCODE AND NOT DEFINED CMAKE_BUILD_TYPE)
            foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
                _conan_install(${TYPE})
            endforeach()
        else()
            _conan_install(${CMAKE_BUILD_TYPE})
        endif()

        list( REMOVE_DUPLICATES CONAN_REQUIRES )

        set(CONAN_CMAKE_SILENT_OUTPUT TRUE)
        include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
        conan_define_targets()

        foreach( package_raw ${CONAN_RESOLVE_LIST} )
            string(REPLACE "|" ";" package_list ${package_raw})
            list(GET package_list 0 package)
            list(GET package_list 1 target_name)

            # Alias the CONAN_PKG target to the propper target name
            set(_curr_conan_target "CONAN_PKG::${package}")
            if (TARGET ${_curr_conan_target})
                set_target_properties(${_curr_conan_target} PROPERTIES IMPORTED_GLOBAL TRUE)
                add_library( ${target_name} ALIAS ${_curr_conan_target} )
            else()
                message(WARNING "Conan target ${_curr_conan_target} not found" )
            endif()
        endforeach()

    endif(USE_CONAN)

    file(GLOB dependency_helpers "${AUDACITY_MODULE_PATH}/dependencies/*.cmake")

    foreach(f ${dependency_helpers})
        include(${f})
    endforeach()
endmacro()


# Refind the packages since some packages dont set vars outside the function scope 
macro(resolve_find_packages)
    foreach( package_raw ${FIND_LIBS_RESOLVE_LIST} )
        string(REPLACE "|" ";" package_list ${package_raw})
        list(GET package_list 0 package)
        list(GET package_list 1 find_package_options)

        find_package( ${package} QUIET ${find_package_options} )

        message("${package} ${${package}_FOUND}")
    endforeach()
endmacro()


macro ( find_required_package package_name system_package_name )
    find_package ( ${package_name} QUIET ${ARGN} )

    if ( NOT ${package_name}_FOUND )
        if (CMAKE_SYSTEM_NAME MATCHES "Darwin|Windows")
            message( FATAL_ERROR "Error: ${package_name} is required")
        else()
            message( FATAL_ERROR "Error: ${package_name} is required.\nPlease install it with using command like:\n\t\$ sudo apt install ${system_package_name}" )
        endif()
    endif()
endmacro()