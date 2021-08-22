/**********************************************************************

  Tenacity

  Printing.h

  Dominic Mazzoni

**********************************************************************/

#pragma once

#include <wx/defs.h>

class wxString;
class wxWindow;
class TrackList;
class TrackPanel;

void HandlePageSetup(wxWindow *parent);
void HandlePrint(
   wxWindow *parent, const wxString &name, TrackList *tracks,
   TrackPanel &panel);
