/**********************************************************************

  Audacity: A Digital Audio Editor

  Warning.h

  Dominic Mazzoni

**********************************************************************/

#pragma once



#include <wx/defs.h>
class wxString;
class wxWindow;
class TranslatableString;

// "Don't show this warning again"
TENACITY_DLL_API
const TranslatableString &DefaultWarningFooter();

/// Displays a warning dialog with a check box.  If the user checks
/// the box, the internalDialogName is noted in the
/// preferences.  The internalDialogName is never seen by
/// the user; it should be unique to each message.
TENACITY_DLL_API
int ShowWarningDialog(wxWindow *parent,
                      const wxString &internalDialogName,
                      const TranslatableString &message,
                      bool showCancelButton = false,
                      // This message appears by the checkbox:
                      const TranslatableString &footer = DefaultWarningFooter());
