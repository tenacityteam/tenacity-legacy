/**********************************************************************
 
 Tenacity
 
 TempDirectory.h
 
 Paul Licameli split from FileNames.h
 
 **********************************************************************/

#ifndef __AUDACITY_TEMP_DIRECTORY__
#define __AUDACITY_TEMP_DIRECTORY__


#include "Identifier.h"
class TranslatableString;
class wxWindow;

namespace TempDirectory
{
   TENACITY_DLL_API wxString TempDir();
   TENACITY_DLL_API void ResetTempDir();

   TENACITY_DLL_API const FilePath &DefaultTempDir();
   TENACITY_DLL_API void SetDefaultTempDir( const FilePath &tempDir );
   TENACITY_DLL_API bool IsTempDirectoryNameOK( const FilePath & Name );

   // Create a filename for an unsaved/temporary project file
   TENACITY_DLL_API wxString UnsavedProjectFileName();

   TENACITY_DLL_API bool FATFilesystemDenied(const FilePath &path,
                            const TranslatableString &msg,
                            wxWindow *window = nullptr);
};

#endif
