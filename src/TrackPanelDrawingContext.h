/**********************************************************************

 Tenacity

 TrackPanelDrawingContext.h

 Paul Licameli

 **********************************************************************/

#pragma once

#include <memory>

class UIHandle;
using UIHandlePtr = std::shared_ptr<UIHandle>;
class wxDC;

#include <wx/mousestate.h> // member variable

struct TrackPanelDrawingContext {
   wxDC &dc;
   UIHandlePtr target;
   wxMouseState lastState;

   void *pUserData;

   // This redundancy fixes an MSVC compiler warning:
   TrackPanelDrawingContext() = delete;
};
