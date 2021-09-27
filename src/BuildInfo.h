/**********************************************************************

  Tenacity

  BuildInfo.h

**********************************************************************/

#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#include "wx/cpp.h"
#include <wx/ctrlsub.h>
#include <wx/sstream.h>
#include <wx/string.h>
#include <wx/txtstrm.h>

#include <Internat.h>

// RevisionIdent contains the GIT_DESCRIBE and REV_LONG defines from git commit information
#include "RevisionIdent.h"

// This define replaces the original that modified the macro in wxwidgets
#define CUSTOM_wxMAKE_VERSION_DOT_STRING_T(x, y, z) wxSTRINGIZE_T(x) wxT(".") wxSTRINGIZE_T(y) wxT(".") wxSTRINGIZE_T(z) wxT("(Tenacity)")

class BuildInfo {

public:
      enum class CompilerType { MSVC, MinGW, GCC, Clang, AppleClang, Unknown };

      static constexpr auto CurrentBuildCompiler =
        #if defined(_MSC_FULL_VER)
              CompilerType::MSVC;
        #elif defined(__GNUC_PATCHLEVEL__) && defined(__MINGW32__)
              CompilerType::MinGW;
        #elif defined(__GNUC_PATCHLEVEL__) && !defined(__llvm__) && !defined(__clang__)
              CompilerType::GCC;
        #elif defined(__clang_version__) && !defined(__apple_build_version__)
              CompilerType::Clang;
        #elif defined(__clang_version__) && defined(__apple_build_version__)
              CompilerType::AppleClang;
        #else
              CompilerType::Unknown;
        #endif

        static const inline wxString getCompilerVersionString(){

          switch (BuildInfo::CurrentBuildCompiler) {

          case BuildInfo::CompilerType::MSVC:
            #if !defined(_MSC_VER) || !defined(_MSC_FULL_VER) || !defined (_MSC_BUILD)
              // This should be unreachable, but it makes the compiler realize that they will always be defined
              #define _MSC_VER 0
              #define _MSC_FULL_VER 0
              #define _MSC_BUILD 0
            #endif
            return wxString::Format(wxT("MSVC %02d.%02d.%05d.%02d"), _MSC_VER / 100, _MSC_VER % 100, _MSC_FULL_VER % 100000, _MSC_BUILD);

          case BuildInfo::CompilerType::MinGW:
            #if !defined(__GNUC__) || !defined (__GNUC_MINOR__) || !defined(__GNUC_PATCHLEVEL__)
              // This should be unreachable, but it makes the compiler realize that they will always be defined
              #define __GNUC__ 0
              #define __GNUC_MINOR__ 0
              #define __GNUC_PATCHLEVEL__ 0
            #endif
            return wxString::Format(wxT("MinGW %s"), CUSTOM_wxMAKE_VERSION_DOT_STRING_T( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__));

          case BuildInfo::CompilerType::GCC:
            #if !defined(__GNUC__) || !defined (__GNUC_MINOR__) || !defined(__GNUC_PATCHLEVEL__)
              // This should be unreachable, but it makes the compiler realize that they will always be defined
              #define __GNUC__ 0
              #define __GNUC_MINOR__ 0
              #define __GNUC_PATCHLEVEL__ 0
            #endif
            return wxString::Format(wxT("GCC %s"), CUSTOM_wxMAKE_VERSION_DOT_STRING_T( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__));

          case BuildInfo::CompilerType::Clang:
            #if !defined(__clang_major__) || !defined (__clang_minor__) || !defined(__clang_patchlevel__)
              // This should be unreachable, but it makes the compiler realize that they will always be defined
              #define __clang_major__ 0
              #define __clang_minor__ 0
              #define __clang_patchlevel__ 0
            #endif
            return wxString::Format( wxT("Clang %s"), CUSTOM_wxMAKE_VERSION_DOT_STRING_T(__clang_major__, __clang_minor__, __clang_patchlevel__));

          case BuildInfo::CompilerType::AppleClang:
            #if !defined(__clang_major__) || !defined (__clang_minor__) || !defined(__clang_patchlevel__)
              // This should be unreachable, but it makes the compiler realize that they will always be defined
              #define __clang_major__ 0
              #define __clang_minor__ 0
              #define __clang_patchlevel__ 0
            #endif
            return wxString::Format( wxT("Apple Clang %s"), CUSTOM_wxMAKE_VERSION_DOT_STRING_T(__clang_major__, __clang_minor__, __clang_patchlevel__));

          case BuildInfo::CompilerType::Unknown:
          default:
            return wxT("Unknown!!!");
          }
        };

        static const inline wxString getRevisionIdentifier(){
            static wxString NoRevisionText = XO("No revision identifier was provided").Translation();

            #ifdef REV_LONG
                return wxString("[[https://github.com/tenacityteam/tenacity/commit/") + REV_LONG + "|" + wxString(GIT_DESCRIBE) + "]]";
            #else
                return NoRevisionText;
            #endif
        }

        static constexpr inline bool is64bits(){
            return (sizeof(void*) == 8);
        }

        static constexpr inline bool isCMakeBuild(){
           #ifdef CMAKE
                return true;
           #else
                return false;
           #endif
        }

        static constexpr inline bool isDebugBuild(){
            #ifdef _DEBUG
                return true;
            #else
                return false;
            #endif
        }

        static const wxString getBuildType() {

            wxStringOutputStream o;
            wxTextOutputStream buildTypeString(o);

            if constexpr (isCMakeBuild()){
                buildTypeString << Verbatim("CMake ");
            }

            if constexpr (isDebugBuild()){
                buildTypeString << XO("Debug build (debug level %d)").Format(wxDEBUG_LEVEL).Translation();
            }else{
                buildTypeString << XO("Release build (debug level %d)").Format(wxDEBUG_LEVEL).Translation();
            }

            if constexpr (is64bits()) {
                buildTypeString << XO(", 64 bits").Translation();
            }


            return o.GetString();
        }

        static const wxString getWxWidgetsVersion() {
            wxPlatformInfo info = wxPlatformInfo::Get();

            return wxString::Format("v%s (%s v%d.%d)",
                        wxVERSION_NUM_DOT_STRING_T,
                        info.GetPortIdShortName(),
                        info.GetToolkitMajorVersion(),
                        info.GetToolkitMinorVersion());
        }
};
#endif
