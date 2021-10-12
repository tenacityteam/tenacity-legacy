/*!********************************************************************

Audacity: A Digital Audio Editor

@file wxWidgetsBasicUI.h
@brief Implementation of BasicUI using wxWidgets

Paul Licameli

**********************************************************************/
#ifndef __WXWIDGETS_BASIC_UI__
#define __WXWIDGETS_BASIC_UI__

#include "BasicUI.h"

class wxWindow;

//! Window placement information for wxWidgetsBasicUI can be constructed from a wxWindow pointer
struct TENACITY_DLL_API wxWidgetsWindowPlacement final
: BasicUI::WindowPlacement {
   wxWidgetsWindowPlacement() = default;
   explicit wxWidgetsWindowPlacement( wxWindow *pWindow )
      : pWindow{ pWindow }
   {}

   ~wxWidgetsWindowPlacement() override;
   wxWindow *pWindow{};
};

//! An implementation of BasicUI::Services in terms of the wxWidgets toolkit
/*! This is a singleton that doesn't need TENACITY_DLL_API visibility */
class wxWidgetsBasicUI final : public BasicUI::Services {
public:
   ~wxWidgetsBasicUI() override;

   void DoCallAfter(const BasicUI::Action &action) override;
   void DoYield() override;
};

#endif
