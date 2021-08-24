/**********************************************************************

  Tenacity

  FileNames.h

  James Crook

**********************************************************************/

#ifndef __AUDACITY_FILE_NAMES__
#define __AUDACITY_FILE_NAMES__



#include <wx/dir.h> // for wxDIR_FILES
#include <wx/string.h> // function return value
#include "Identifier.h"
#include "Prefs.h"
#include <memory>

// Please try to support unlimited path length instead of using PLATFORM_MAX_PATH!
// Define one constant for maximum path value, so we don't have to do
// platform-specific conditionals everywhere we want to check it.
#define PLATFORM_MAX_PATH 260 // Play it safe for default, with same value as Windows' MAX_PATH.

#ifdef __WXMAC__
#undef PLATFORM_MAX_PATH
#define PLATFORM_MAX_PATH PATH_MAX
#endif

#ifdef __WXGTK__
// Some systems do not restrict the path length and therefore PATH_MAX is undefined
#ifdef PATH_MAX
#undef PLATFORM_MAX_PATH
#define PLATFORM_MAX_PATH PATH_MAX
#endif
#endif

#ifdef __WXX11__
// wxX11 should also get the platform-specific definition of PLATFORM_MAX_PATH, so do not declare here.
#endif

#ifdef __WXMSW__
#undef PLATFORM_MAX_PATH
#define PLATFORM_MAX_PATH MAX_PATH
#endif

class wxFileName;
class wxFileNameWrapper;

namespace FileNames
{
   // A description of a type of file
   struct FileType {
      FileType() = default;

      FileType( TranslatableString d, FileExtensions e, bool a = false )
         : description{ std::move( d ) }
         , extensions( std::move( e ) )
         , appendExtensions{ a }
      {}

      TranslatableString description;
      FileExtensions extensions;
      // Whether to extend the displayed description with mention of the
      // extensions:
      bool appendExtensions = false;
   };

   // Frequently used types
   extern TENACITY_DLL_API const FileType
        AllFiles // *
      , AudacityProjects // *.aup3
      , DynamicLibraries // depends on the operating system
      , TextFiles // *.txt
      , XMLFiles; // *.xml, *.XML

   using FileTypes = std::vector< FileType >;

   // Convert fileTypes into a single string as expected by wxWidgets file
   // selection dialog
   TENACITY_DLL_API wxString FormatWildcard( const FileTypes &fileTypes );

   // This exists to compensate for bugs in wxCopyFile:
   TENACITY_DLL_API bool DoCopyFile(
      const FilePath& file1, const FilePath& file2, bool overwrite = true);

   // wxWidgets doesn't have a function to do this:  make a hard file-system
   // link if possible.  It might not be, as when the paths are on different
   // storage devices.
   TENACITY_DLL_API
   bool HardLinkFile( const FilePath& file1, const FilePath& file2);

   TENACITY_DLL_API wxString MkDir(const wxString &Str);

   TENACITY_DLL_API bool IsMidi(const FilePath &fName);

   /** \brief A list of directories that should be searched for Audacity files
    * (plug-ins, help files, etc.).
    *
    * On Unix this will include the directory Audacity was installed into,
    * plus the current user's .audacity-data/Plug-Ins directory.  Additional
    * directories can be specified using the AUDACITY_PATH environment
    * variable.  On Windows or Mac OS, this will include the directory
    * which contains the Audacity program. */
   TENACITY_DLL_API const FilePaths &AudacityPathList();
   TENACITY_DLL_API void SetAudacityPathList( FilePaths list );

   // originally an ExportMultipleDialog method. Append suffix if newName appears in otherNames.
   TENACITY_DLL_API void MakeNameUnique(
      FilePaths &otherNames, wxFileName &newName);

   TENACITY_DLL_API wxString LowerCaseAppNameInPath( const wxString & dirIn);
   /** \brief Tenacity user config directory
    *
    * Where Tenacity keeps its settigns squirreled away, by default ~/.config/tenacity/
    * on Unix, Application Data/Tenacity on Windows */
   FilePath ConfigDir();
   /** \brief Tenacity user data directory
    *
    * Where Tenacity keeps its settings and other user data squirreled away,
    * by default ~/.local/share/tenacity/ on Unix, Application Data/Tenacity on
    * windows system */
   TENACITY_DLL_API FilePath DataDir();
   TENACITY_DLL_API FilePath ResourcesDir();
   TENACITY_DLL_API FilePath HtmlHelpDir();
   TENACITY_DLL_API FilePath HtmlHelpIndexFile(bool quick);
   TENACITY_DLL_API FilePath LegacyChainDir();
   TENACITY_DLL_API FilePath MacroDir();
   TENACITY_DLL_API FilePath NRPDir();
   TENACITY_DLL_API FilePath NRPFile();
   TENACITY_DLL_API FilePath PluginRegistry();
   TENACITY_DLL_API FilePath PluginSettings();

   TENACITY_DLL_API FilePath BaseDir();
   TENACITY_DLL_API FilePath ModulesDir();

   /** \brief The user plug-in directory (not a system one)
    *
    * This returns the string path to where the user may have put plug-ins
    * if they don't have system admin rights. Under default settings, it's
    * <DataDir>/Plug-Ins/ */
   TENACITY_DLL_API FilePath PlugInDir();
   TENACITY_DLL_API FilePath ThemeDir();
   TENACITY_DLL_API FilePath ThemeComponentsDir();
   TENACITY_DLL_API FilePath ThemeCachePng();
   TENACITY_DLL_API FilePath ThemeCacheAsCee();
   TENACITY_DLL_API FilePath ThemeComponent(const wxString &Str);
   TENACITY_DLL_API FilePath ThemeCacheHtm();
   TENACITY_DLL_API FilePath ThemeImageDefsAsCee();

   // Obtain name of loaded module that contains address
   TENACITY_DLL_API FilePath PathFromAddr(void *addr);

   TENACITY_DLL_API bool IsPathAvailable( const FilePath & Path);
   TENACITY_DLL_API wxFileNameWrapper DefaultToDocumentsFolder
      (const wxString &preference);

   // If not None, determines a preference key (for the default path string) to
   // be read and updated
   enum class Operation {
      // _ on None to defeat some macro that is expanding this.
      _None,

      // These do not have a specific pathtype
      Temp,
      Presets,

      // These have default/lastused pathtypes
      Open,
      Save,
      Import,
      Export,
      MacrosOut
   };

   enum class PathType {
      // _ on None to defeat some macro that is expanding this.
      _None,
      User,
      LastUsed
   };

   TENACITY_DLL_API wxString PreferenceKey(FileNames::Operation op, FileNames::PathType type);

   TENACITY_DLL_API FilePath FindDefaultPath(Operation op);
   TENACITY_DLL_API void UpdateDefaultPath(Operation op, const FilePath &path);

   // F is a function taking a wxString, returning wxString
   template<typename F>
   FilePath WithDefaultPath
   (Operation op, const FilePath &defaultPath, F function)
   {
      auto path = gPrefs->Read(PreferenceKey(op, PathType::User), defaultPath);
      if (path.empty())
         path = FileNames::FindDefaultPath(op);
      auto result = function(path);
      FileNames::UpdateDefaultPath(op, ::wxPathOnly(result));
      return result;
   }

   TENACITY_DLL_API FilePath
   SelectFile(Operation op,   // op matters only when default_path is empty
      const TranslatableString& message,
      const FilePath& default_path,
      const FilePath& default_filename,
      const FileExtension& default_extension,
      const FileTypes& fileTypes,
      int flags,
      wxWindow *parent);

   // Useful functions for working with search paths
   TENACITY_DLL_API void AddUniquePathToPathList(const FilePath &path,
                                       FilePaths &pathList);
   TENACITY_DLL_API void AddMultiPathsToPathList(const wxString &multiPathString,
                                       FilePaths &pathList);
   TENACITY_DLL_API void FindFilesInPathList(const wxString & pattern,
                                   const FilePaths & pathList,
                                   FilePaths &results,
                                   int flags = wxDIR_FILES);

   /** \brief Protect against Unicode to multi-byte conversion failures
    * on Windows */
#if defined(__WXMSW__)
   TENACITY_DLL_API char *VerifyFilename(const wxString &s, bool input = true);
#endif

   //! Check location on writable access and return true if checked successfully.
   TENACITY_DLL_API bool WritableLocationCheck(const FilePath& path);

   // wxString compare function for sorting case, which is needed to load correctly.
   TENACITY_DLL_API int CompareNoCase(const wxString& first, const wxString& second);

   // Create a unique filename using the passed prefix and suffix
   TENACITY_DLL_API wxString CreateUniqueName(const wxString &prefix,
                             const wxString &suffix = wxEmptyString);

   // File extension used for unsaved/temporary project files
   TENACITY_DLL_API wxString UnsavedProjectExtension();

   TENACITY_DLL_API
   bool IsOnFATFileSystem(const FilePath &path);

   TENACITY_DLL_API
   //! Give enough of the path to identify the device.  (On Windows, drive letter plus ':')
   wxString AbbreviatePath(const wxFileName &fileName);
};

// Use this macro to wrap all filenames and pathnames that get
// passed directly to a system call, like opening a file, creating
// a directory, checking to see that a file exists, etc...
#if defined(__WXMSW__)
// Note, on Windows we don't define an OSFILENAME() to prevent accidental use.
// See VerifyFilename() for an explanation.
#define OSINPUT(X) FileNames::VerifyFilename(X, true)
#define OSOUTPUT(X) FileNames::VerifyFilename(X, false)
#elif defined(__WXMAC__)
#define OSFILENAME(X) ((char *) (const char *)(X).fn_str())
#define OSINPUT(X) OSFILENAME(X)
#define OSOUTPUT(X) OSFILENAME(X)
#else
#define OSFILENAME(X) ((char *) (const char *)(X).mb_str())
#define OSINPUT(X) OSFILENAME(X)
#define OSOUTPUT(X) OSFILENAME(X)
#endif

#endif
