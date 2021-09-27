set( TARGET manual )
set( TARGET_ROOT ${CMAKE_SOURCE_DIR}/manual )

message( STATUS "========== Configuring ${TARGET} ==========" )

def_vars()

if( NOT DEFINED PYTHON )
   message( WARNING "Python not found...unable to produce manual." )
   return()
endif()

set( host "alphamanual.audacityteam.org" )
set( src "https://${host}/man" )
set( dst "${_DEST}/help/manual" )
set( script_dir "${CMAKE_SOURCE_DIR}/scripts/mw2html_audacity" )
set( script "mw2html.py" )
set( out_dir "${_INTDIR}" )
set( out "${out_dir}/${host}/index.html" )

add_custom_command(
   COMMENT
      "Downloading manual from: ${src}"
   COMMAND
      "${PYTHON}" "${script_dir}/${script}" -s "${src}" "${out_dir}"
   COMMAND
      ${CMAKE_COMMAND} -E copy_directory "${out_dir}/${host}" "${dst}"
   WORKING_DIRECTORY
      "${script_dir}"
   OUTPUT
      "${out}"
)

add_custom_target( ${TARGET} DEPENDS "${out}" )

if( ${_OPT}package_manual )
   install(CODE "                                     \
      execute_process(                                \
         COMMAND                                      \
            ${CMAKE_COMMAND}                          \
               --build ${CMAKE_BINARY_DIR}            \
               --config \${CMAKE_INSTALL_CONFIG_NAME} \
               --target manual                        \
      )                                               \
   ")

   install( DIRECTORY "${out_dir}/${host}/" DESTINATION "help/manual" )
endif()

if( NOT CMAKE_SYSTEM_NAME MATCHES "Darwin" )
   if( NOT WIN32)
      install( DIRECTORY "${dst}" OPTIONAL
               DESTINATION "${_DATADIR}/tenacity/help" )

      install( FILES "${_SRCDIR}/org.tenacityaudio.Tenacity.metainfo.xml"
               DESTINATION "${_DATADIR}/metainfo" )
   endif()
endif()

# Test if scdoc is installed
find_program(SCDOC_CMD scdoc)

cmake_dependent_option(BUILD_MANPAGE "Build manpage" ON "NOT SCDOC_CMD STREQUAL SCDOC_CMD-NOTFOUND" OFF)

if( BUILD_MANPAGE )
   add_custom_command( COMMENT "Generating manpage"
      COMMAND ${SCDOC_CMD} < "${_SRCDIR}/tenacity.1.scd" > "${CMAKE_BINARY_DIR}/help/tenacity.1"
      DEPENDS "${_SRCDIR}/tenacity.1.scd"
      OUTPUT "${CMAKE_BINARY_DIR}/help/tenacity.1" )

   add_custom_target( gen_manpage ALL DEPENDS "${CMAKE_BINARY_DIR}/help/tenacity.1")

   install( FILES "${CMAKE_BINARY_DIR}/help/tenacity.1"
            DESTINATION "${_MANDIR}/man1" )
endif()
