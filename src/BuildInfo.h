/**********************************************************************

  Tenacity

  BuildInfo.h

**********************************************************************/

#ifndef BUILD_INFO_H
#define BUILD_INFO_H

#include <wx/string.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>

#include <Internat.h>

// RevisionIdent contains the REV_TIME and REV_LONG defines from git commit information
#include "RevisionIdent.h"

// This define replaces the original that modified the macro in wxwidgets
#define CUSTOM_wxMAKE_VERSION_DOT_STRING_T(x, y, z) wxSTRINGIZE_T(x) wxT(".") wxSTRINGIZE_T(y) wxT(".") wxSTRINGIZE_T(z) wxT("(Tenacity)")

class BuildInfo {
    
public:
      const enum class CompilerType { MSVC, MinGW, GCC, Clang, Unknown };

      static constexpr auto CurrentBuildCompiler =
        #if defined(_MSC_FULL_VER)
              CompilerType::MSVC;
        #elif defined(__GNUC_PATCHLEVEL__) && defined(__MINGW32__)
              CompilerType::MinGW;
        #elif defined(__GNUC_PATCHLEVEL__)
              CompilerType::GCC;
        #elif defined(__clang_version__)
              CompilerType::Clang;
        #else
              CompilerType::Unknown;
        #endif

        static const inline wxString getCompilerVersionString(){

          switch (BuildInfo::CurrentBuildCompiler) {

          case BuildInfo::CompilerType::MSVC:
            return wxString::Format(wxT("MSVC %02d.%02d.%05d.%02d"), _MSC_VER / 100, _MSC_VER % 100, _MSC_FULL_VER % 100000, _MSC_BUILD);

          case BuildInfo::CompilerType::MinGW:
            return wxString::Format(wxT("MinGW %s"), CUSTOM_wxMAKE_VERSION_DOT_STRING_T( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__));

          case BuildInfo::CompilerType::GCC:
            return wxString::Format(wxT("GCC %s"), CUSTOM_wxMAKE_VERSION_DOT_STRING_T( __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__));

          case BuildInfo::CompilerType::Clang:
            return wxString::Format( wxT("clang %s"), CUSTOM_wxMAKE_VERSION_DOT_STRING_T(__clang_major__, __clang_minor__, __clang_patchlevel__));

          case BuildInfo::CompilerType::Unknown:
          default:
            return wxT("Unknown!!!");
          }
        };

        static const inline wxString getRevisionIdentifier(){
        
            static wxString NoRevisionText = XO("No revision identifier was provided").Translation();

            #ifdef REV_LONG
                return wxString( "[[https://github.com/tenacityteam/tenacity/commit/" ) + REV_LONG + "|" + wxString( REV_LONG ).Left(6) + "]] of " +  getRevisionDateTime();
            #else
                return NoRevisionText;
            #endif
        }

        static const inline wxString getRevisionDateTime(){

            
            //This needs to be outside the #ifdef or it won't end up in the POT file consistently
            static wxString NoDateTimeText = XO("Unknown date and time").Translation();

            #ifdef REV_TIME
                return wxString(REV_TIME);
            #else
                return NoDateTimeText;
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
};
#endif
