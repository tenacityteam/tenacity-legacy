/**********************************************************************

Audacity: A Digital Audio Editor

WaveTrackVRulerControls.h

Paul Licameli split from TrackPanel.cpp

**********************************************************************/

#pragma once

#include "../../../ui/TrackVRulerControls.h"

class Ruler;
class WaveTrack;

namespace WaveTrackVRulerControls
{
   TENACITY_DLL_API Ruler &ScratchRuler();

   TENACITY_DLL_API void DoDraw( TrackVRulerControls &controls,
      TrackPanelDrawingContext &context,
      const wxRect &rect, unsigned iPass );
};
