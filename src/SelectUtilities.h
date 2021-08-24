/**********************************************************************
 
 Tenacity
 
 SelectUtilities.h
 
 Paul Licameli split from SelectMenus.h
 
 **********************************************************************/

#ifndef __AUDACITY_SELECT_UTILITIES__
#define __AUDACITY_SELECT_UTILITIES__

class AudacityProject;
class Track;

/// Namespace for functions for Select menu
namespace SelectUtilities {

TENACITY_DLL_API void DoSelectTimeAndTracks(
   AudacityProject &project, bool bAllTime, bool bAllTracks);
TENACITY_DLL_API void SelectAllIfNone( AudacityProject &project );
TENACITY_DLL_API bool SelectAllIfNoneAndAllowed( AudacityProject &project );
TENACITY_DLL_API void SelectNone( AudacityProject &project );
TENACITY_DLL_API void DoListSelection(
   AudacityProject &project, Track *t,
   bool shift, bool ctrl, bool modifyState );
TENACITY_DLL_API void DoSelectAll( AudacityProject &project );
TENACITY_DLL_API void DoSelectAllAudio( AudacityProject &project );
TENACITY_DLL_API void DoSelectSomething( AudacityProject &project );

}

#endif
