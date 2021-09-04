# This CMake script is invoked to build the InnoSetup installer for Tenacity
# Requiered parameters:
# BUILD_DIR - should be set to CMAKE_BINARY_DIR by the caller
# SOURCE_DIR - should be set to CMAKE_SOURCE_DIR by teh caller
# OUTPUT_DIR - directory, where installer will be built
# INNO_SETUP_COMPILER - InnoSetup compiler executable
# IS_64_BIT - Flag, that indicates that we are building a 64-bit installer
# EMBED_MANUAL - embed a fresh copy of manual
# SIGN - sign the installer
# WINDOWS_CERTIFICATE - path to PFX file. If not present, env:WINDOWS_CERTIFICATE will be used
# WINDOWS_CERTIFICATE_PASSWORD - password for the PFX file. If not present, env:WINDOWS_CERTIFICATE_PASSWORD will be used

# Allow if statements to use the new IN_LIST operator (compatibility override for CMake <3.3)
cmake_policy( SET CMP0057 NEW )

if( IS_64_BIT )
    set( INSTALLER_SUFFIX "x64" )
    set( INSTALLER_X64_MODE "ArchitecturesInstallIn64BitMode=x64")
else()
    set( INSTALLER_SUFFIX "x86" )
    set( INSTALLER_X64_MODE "ArchitecturesInstallIn64BitMode=x64")
endif()

if( SIGN )
    set( SIGN_TOOL "SignTool=byparam powershell -ExecutionPolicy Bypass -File \$q${SOURCE_DIR}/scripts/build/windows/PfxSign.ps1\$q -File $f")

    if( WINDOWS_CERTIFICATE )
        string(APPEND SIGN_TOOL " -CertFile \$q${WINDOWS_CERTIFICATE}\$q")
    endif()

    if( WINDOWS_CERTIFICATE_PASSWORD )
        message("Setting env:WINDOWS_CERTIFICATE_PASSWORD...")
        set( ENV{WINDOWS_CERTIFICATE_PASSWORD} "${WINDOWS_CERTIFICATE_PASSWORD}")
    endif()
else()
    set( SIGN_TOOL )
endif()

if( EMBED_MANUAL )
    set ( MANUAL [[Source: "Package\help\manual\*"; DestDir: "{app}\help\manual\"; Flags: ignoreversion recursesubdirs]])
else()
    set( MANUAL )
endif()

# Prepare the output directory

file(COPY "${SOURCE_DIR}/win/Inno_Setup_Wizard/" DESTINATION "${OUTPUT_DIR}")
configure_file("${OUTPUT_DIR}/tenacity.iss.in" "${OUTPUT_DIR}/tenacity.iss")

# Copy additional files

file(COPY "${SOURCE_DIR}/presets" DESTINATION "${OUTPUT_DIR}/Additional")

file(COPY
        "${SOURCE_DIR}/LICENSE.txt"
        "${SOURCE_DIR}/win/README.rtf"
        "${SOURCE_DIR}/win/tenacity.ico"
        "${SOURCE_DIR}/win/darktenacity.ico"
    DESTINATION
        "${OUTPUT_DIR}/Additional"
)

if( DEFINED CMAKE_BUILD_TYPE )
  if( EXISTS "${BUILD_DIR}/bin/${CMAKE_BUILD_TYPE}" )
    # Use CMAKE_BUILD_TYPE value if defined and previously built
    set( INNOSETUP_BUILD_CONFIGURATIONS "${CMAKE_BUILD_TYPE};" )
  else()
    message( FATAL_ERROR "You defined a value for CMAKE_BUILD_TYPE which has not already been built, please build it and try again" )
  endif()
elseif( DEFINED CMAKE_INSTALL_CONFIG_NAME )
  if( EXISTS "${BUILD_DIR}/bin/${CMAKE_INSTALL_CONFIG_NAME}" )
    # Use install --config value if defined and previously built
    set( INNOSETUP_BUILD_CONFIGURATIONS "${CMAKE_INSTALL_CONFIG_NAME};" )
  else()
    message( FATAL_ERROR "You defined a value for CMAKE_INSTALL_CONFIG_NAME which has not already been built, please build it and try again" )
  endif()
else()
  # These are listed in order of preference in case more than one of them are built
  # It must include all four configurations for this to work correctly on multi-config
  set( INNOSETUP_BUILD_CONFIGURATIONS "MinSizeRel;Release;RelWithDebInfo;Debug" )
  message( STATUS "You did not define a build type manually so we will attempt to find build types in this order: ${INNOSETUP_BUILD_CONFIGURATIONS}" )
endif()

# "Install" prebuilt package

set( VALID_BUILD_TYPE_FOUND FALSE )
foreach( config ${INNOSETUP_BUILD_CONFIGURATIONS} )
  file( GLOB VALID_BUILD
          "${BUILD_DIR}/bin/${config}/Tenacity" "${BUILD_DIR}/bin/${config}/Tenacity.*"
          "${BUILD_DIR}/bin/${config}/tenacity" "${BUILD_DIR}/bin/${config}/tenacity.*"
  )
  if( VALID_BUILD )
    message ( STATUS "Using build type ${config} to create innosetup installer" )
    execute_process(
      COMMAND
          ${CMAKE_COMMAND}
              --install ${BUILD_DIR}
              --prefix "${OUTPUT_DIR}/Package"
              --config ${config}
              # When we upgrade to CMake min version 3.19 we can use this
              # COMMAND_ERROR_IS_FATAL ANY
    )
    set( VALID_BUILD_TYPE_FOUND TRUE FORCE )
    set( VALID_BUILD_TYPE ${config} )
    break()
  endif()
endforeach()

if( NOT VALID_BUILD_TYPE_FOUND )
  message( FATAL_ERROR "You must build the project successfully before building the innosetup target" )
endif()

execute_process(
    COMMAND
        ${INNO_SETUP_COMPILER} /Sbyparam=$p "tenacity.iss" /Qp
    WORKING_DIRECTORY
        ${OUTPUT_DIR}
    # When we upgrade to CMake min version 3.19 we can use this
    # COMMAND_ERROR_IS_FATAL ANY
)

# If we enable COMMAND_ERROR_IS_FATAL then if we reach here we are sure we successfully created the installer
# message ( STATUS "Successfully created innosetup installer using build type ${VALID_BUILD_TYPE}" )

# Emulate CPack behavior
file( COPY "${OUTPUT_DIR}/Output/" DESTINATION "${BUILD_DIR}/package" )
