/**********************************************************************

  Audacity: A Digital Audio Editor

  ImportRaw.h

  Dominic Mazzoni

**********************************************************************/

#pragma once

#include <memory>

class AudacityProject;
class WaveTrackFactory;
class WaveTrack;
class wxString;
class wxWindow;

#include <vector>

// Newly constructed WaveTracks that are not yet owned by a TrackList
// are held in unique_ptr not shared_ptr
using NewChannelGroup = std::vector< std::shared_ptr<WaveTrack> >;
using TrackHolders = std::vector< NewChannelGroup >;


void ImportRaw(const AudacityProject &project, wxWindow *parent, const wxString &fileName,
   WaveTrackFactory *trackFactory, TrackHolders &outTracks);
