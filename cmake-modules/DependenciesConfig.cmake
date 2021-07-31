include( TenacityDependenciesFunctions )

add_external_lib(
   ZLIB
   zlib/1.2.11
   REQUIRED
   PKG_CONFIG "zlib >= 1.2.11"
   INTERFACE_NAME ZLIB::ZLIB
   OPTION_NAME zlib
   CONAN_OPTIONS 
      zlib:shared=True
)

add_external_lib(
   expat
   expat/2.2.9@audacity/stable
   REQUIRED
   PKG_CONFIG "expat >= 2.1.0"
   CONAN_OPTIONS 
      expat:shared=True
)

set( wx_zlib "zlib" )

set( wx_png "libpng" )
set( wx_jpeg "libjpeg-turbo")
set( wx_tiff "off" )

set( id3tag_zlib "conan" )

if ( ${_OPT}use_zlib STREQUAL "system" )
   set( wx_zlib "sys" )
   # To prevent linking conflicts - we need to use system libpng as well.
   # wxWdigets will attempt to resolve it using find_package
   find_required_package( PNG "libpng-dev" )
   set( wx_png "sys" )
   # And, for consistency
   find_required_package( JPEG "libjpeg-turbo8-dev" )
   set( wx_jpeg "sys" )

   set( id3tag_zlib "system" )
endif()

set(wx_expat "expat")

if (${_OPT}use_expat STREQUAL "system")
   set(wx_expat "sys")
endif()

add_external_lib( 
   wxWidgets 
   wxwidgets/3.1.3.1-audacity 
   REQUIRED 
   ALWAYS_ALLOW_CONAN_FALLBACK
   OPTION_NAME wxwidgets
   FIND_PACKAGE_OPTIONS COMPONENTS adv base core html qa xml net
   INTERFACE_NAME wxwidgets::wxwidgets
   CONAN_OPTIONS 
      wxwidgets:shared=True
      wxwidgets:zlib=${wx_zlib}
      wxwidgets:expat=${wx_expat}
      wxwidgets:compatibility=3.0
      wxwidgets:png=${wx_png}
      wxwidgets:jpeg=${wx_jpeg}
      wxwidgets:tiff=${wx_tiff}
      wxwidgets:secretstore=False
      wxwidgets:opengl=False
      wxwidgets:propgrid=False
      wxwidgets:ribbon=False
      wxwidgets:richtext=False
      wxwidgets:stc=False
      wxwidgets:webview=False
      wxwidgets:help=False
      wxwidgets:html_help=False
      wxwidgets:fs_inet=False
      wxwidgets:protocol=False
      # Building with SIMD requires a huge number of build dependencies
      # Probably this will be enabled in the future
      libjpeg-turbo:SIMD=False 
)

add_external_lib(
   LAME
   libmp3lame/3.100
   REQUIRED
   FIND_PACKAGE
   INTERFACE_NAME LAME::LAME
)

add_external_lib(
   libid3tag
   libid3tag/0.15.2b@audacity/stable
   OPTION_NAME libid3tag
   PKG_CONFIG "id3tag >= 0.15.0b"
   CONAN_OPTIONS
      libid3tag:zlib=${id3tag_zlib}
)

add_external_lib(
   libmad
   libmad/0.15.2b@audacity/stable
   OPTION_NAME libmad
   PKG_CONFIG "mad >= 0.15.0b"
)

if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
   set( curl_ssl "darwinssl" )
elseif( CMAKE_SYSTEM_NAME MATCHES "Windows" )
   set( curl_ssl "schannel")
else()
   set ( curl_ssl "openssl" )
endif ()

add_external_lib(
   RapidJSON
   rapidjson/1.1.0
   REQUIRED
   FIND_PACKAGE
)