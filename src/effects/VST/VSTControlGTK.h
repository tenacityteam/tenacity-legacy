/**********************************************************************

  Audacity: A Digital Audio Editor

  VSTControlGTK.h

  Leland Lucius

**********************************************************************/

#pragma once


// Must include after ours since we have a lot of name collisions
#define Region XRegion     // Conflicts with Audacity's Region structure
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#undef Success
#undef Region

#include "VSTControl.h"

class VSTControl final : public VSTControlBase
{
public:
   VSTControl();
   ~VSTControl();

   bool Create(wxWindow *parent, VSTEffectLink *link);

private:
   Display *mXdisp;
   Window mXwin;
};
