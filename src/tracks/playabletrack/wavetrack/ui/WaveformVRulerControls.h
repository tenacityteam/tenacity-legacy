/**********************************************************************

Audacity: A Digital Audio Editor

WaveformVRulerControls.h

Paul Licameli split from WaveTrackVRulerControls.h

**********************************************************************/

#pragma once

#include "../../../ui/TrackVRulerControls.h" // to inherit

class WaveTrack;
class WaveformVZoomHandle;

class WaveformVRulerControls final : public TrackVRulerControls
{
   WaveformVRulerControls(const WaveformVRulerControls&) = delete;
   WaveformVRulerControls &operator=(const WaveformVRulerControls&) = delete;

public:
   explicit
   WaveformVRulerControls( const std::shared_ptr<TrackView> &pTrackView )
      : TrackVRulerControls( pTrackView ) {}
   ~WaveformVRulerControls() override;

   std::vector<UIHandlePtr> HitTest(
      const TrackPanelMouseState &state,
      const AudacityProject *) override;

   unsigned HandleWheelRotation(
      const TrackPanelMouseEvent &event,
      AudacityProject *pProject) override;
   static unsigned DoHandleWheelRotation(
      const TrackPanelMouseEvent &event,
      AudacityProject *pProject, WaveTrack *wt);

private:
   // TrackPanelDrawable implementation
   void Draw(
      TrackPanelDrawingContext &context,
      const wxRect &rect, unsigned iPass ) override;

   // TrackVRulerControls implementation
   void UpdateRuler( const wxRect &rect ) override;

   static void DoUpdateVRuler( const wxRect &rect, const WaveTrack *wt );

   std::weak_ptr<WaveformVZoomHandle> mVZoomHandle;
};
