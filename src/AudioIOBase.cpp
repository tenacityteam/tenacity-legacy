/**********************************************************************

Tenacity

AudioIOBase.cpp

Paul Licameli split from AudioIO.cpp

**********************************************************************/


#include "AudioIOBase.h"



#include <wx/log.h>
#include <wx/sstream.h>
#include <wx/txtstrm.h>

#include "MemoryX.h"
#include "Prefs.h"
#include "widgets/MeterPanelBase.h"

#include <portaudio.h>

#ifdef EXPERIMENTAL_MIDI_OUT
#include "../lib-src/portmidi/pm_common/portmidi.h"
#endif

int AudioIOBase::mCachedPlaybackIndex = -1;
std::vector<long> AudioIOBase::mCachedPlaybackRates;
int AudioIOBase::mCachedCaptureIndex = -1;
std::vector<long> AudioIOBase::mCachedCaptureRates;
std::vector<long> AudioIOBase::mCachedSampleRates;
double AudioIOBase::mCachedBestRateIn = 0.0;

const int AudioIOBase::StandardRates[] = {
   8000,
   11025,
   16000,
   22050,
   32000,
   44100,
   48000,
   88200,
   96000,
   176400,
   192000,
   352800,
   384000
};

const int AudioIOBase::NumStandardRates = WXSIZEOF(AudioIOBase::StandardRates);

const int AudioIOBase::RatesToTry[] = {
   8000,
   9600,
   11025,
   12000,
   15000,
   16000,
   22050,
   24000,
   32000,
   44100,
   48000,
   88200,
   96000,
   176400,
   192000,
   352800,
   384000
};
const int AudioIOBase::NumRatesToTry = WXSIZEOF(AudioIOBase::RatesToTry);

wxString AudioIOBase::DeviceName(const PaDeviceInfo* info)
{
   wxString infoName = wxSafeConvertMB2WX(info->name);

   return infoName;
}

wxString AudioIOBase::HostName(const PaDeviceInfo* info)
{
   wxString hostapiName = wxSafeConvertMB2WX(Pa_GetHostApiInfo(info->hostApi)->name);

   return hostapiName;
}

std::unique_ptr<AudioIOBase> AudioIOBase::ugAudioIO;

AudioIOBase *AudioIOBase::Get()
{
   return ugAudioIO.get();
}

AudioIOBase::~AudioIOBase() = default;

void AudioIOBase::HandleDeviceChange()
{
   // This should not happen, but it would screw things up if it did.
   // Vaughan, 2010-10-08: But it *did* happen, due to a bug, and nobody
   // caught it because this method just returned. Added wxASSERT().
   wxASSERT(!IsStreamActive());
   if (IsStreamActive())
      return;

   // get the selected record and playback devices
   const int playDeviceNum = getPlayDevIndex();
   const int recDeviceNum = getRecordDevIndex();

   // If no change needed, return
   if (mCachedPlaybackIndex == playDeviceNum &&
       mCachedCaptureIndex == recDeviceNum)
       return;

   // cache playback/capture rates
   mCachedPlaybackRates = GetSupportedPlaybackRates(playDeviceNum);
   mCachedCaptureRates = GetSupportedCaptureRates(recDeviceNum);
   mCachedSampleRates = GetSupportedSampleRates(playDeviceNum, recDeviceNum);
   mCachedPlaybackIndex = playDeviceNum;
   mCachedCaptureIndex = recDeviceNum;
   mCachedBestRateIn = 0.0;

}

void AudioIOBase::SetCaptureMeter(AudacityProject *project, MeterPanelBase *meter)
{
   if (( mOwningProject ) && ( mOwningProject != project))
      return;

   if (meter)
   {
      mInputMeter = meter;
      mInputMeter->Reset(mRate, true);
   }
   else
      mInputMeter.Release();
}

void AudioIOBase::SetPlaybackMeter(AudacityProject *project, MeterPanelBase *meter)
{
   if (( mOwningProject ) && ( mOwningProject != project))
      return;

   if (meter)
   {
      mOutputMeter = meter;
      mOutputMeter->Reset(mRate, true);
   }
   else
      mOutputMeter.Release();
}

bool AudioIOBase::IsPaused() const
{
   return mPaused;
}

bool AudioIOBase::IsBusy() const
{
   if (mStreamToken != 0)
      return true;

   return false;
}

bool AudioIOBase::IsStreamActive() const
{
   bool isActive = false;
   // JKC: Not reporting any Pa error, but that looks OK.
   if( mPortStreamV19 )
      isActive = (Pa_IsStreamActive( mPortStreamV19 ) > 0);

#ifdef EXPERIMENTAL_MIDI_OUT
   if( mMidiStreamActive && !mMidiOutputComplete )
      isActive = true;
#endif
   return isActive;
}

bool AudioIOBase::IsStreamActive(int token) const
{
   return (this->IsStreamActive() && this->IsAudioTokenActive(token));
}

bool AudioIOBase::IsAudioTokenActive(int token) const
{
   return ( token > 0 && token == mStreamToken );
}

bool AudioIOBase::IsMonitoring() const
{
   return ( mPortStreamV19 && mStreamToken==0 );
}

std::vector<long> AudioIOBase::GetSupportedPlaybackRates(int devIndex, double rate)
{
   if (devIndex == -1)
   {  // weren't given a device index, get the prefs / default one
      devIndex = getPlayDevIndex();
   }

   // Check if we can use the cached rates
   if (mCachedPlaybackIndex != -1 && devIndex == mCachedPlaybackIndex
         && (rate == 0.0 || make_iterator_range(mCachedPlaybackRates).contains(rate)))
   {
      return mCachedPlaybackRates;
   }

   std::vector<long> supported;
   int irate = (int)rate;
   const PaDeviceInfo* devInfo = NULL;
   int i;

   devInfo = Pa_GetDeviceInfo(devIndex);

   if (!devInfo)
   {
      wxLogDebug(wxT("GetSupportedPlaybackRates() Could not get device info!"));
      return supported;
   }

   // LLL: Remove when a proper method of determining actual supported
   //      DirectSound rate is devised.
   const PaHostApiInfo* hostInfo = Pa_GetHostApiInfo(devInfo->hostApi);
   bool isDirectSound = (hostInfo && hostInfo->type == paDirectSound);

   PaStreamParameters pars;

   pars.device = devIndex;
   pars.channelCount = 1;
   pars.sampleFormat = paFloat32;
   pars.suggestedLatency = devInfo->defaultHighOutputLatency;
   pars.hostApiSpecificStreamInfo = NULL;

   // JKC: PortAudio Errors handled OK here.  No need to report them
   for (i = 0; i < NumRatesToTry; i++)
   {
      // LLL: Remove when a proper method of determining actual supported
      //      DirectSound rate is devised.
      if (!(isDirectSound && RatesToTry[i] > 200000)){
         if (Pa_IsFormatSupported(NULL, &pars, RatesToTry[i]) == 0)
            supported.push_back(RatesToTry[i]);
         Pa_Sleep( 10 );// There are ALSA drivers that don't like being probed
         // too quickly.
      }
   }

   if (irate != 0 && !make_iterator_range(supported).contains(irate))
   {
      // LLL: Remove when a proper method of determining actual supported
      //      DirectSound rate is devised.
      if (!(isDirectSound && RatesToTry[i] > 200000))
         if (Pa_IsFormatSupported(NULL, &pars, irate) == 0)
            supported.push_back(irate);
   }

   return supported;
}

std::vector<long> AudioIOBase::GetSupportedCaptureRates(int devIndex, double rate)
{
   if (devIndex == -1)
   {  // not given a device, look up in prefs / default
      devIndex = getRecordDevIndex();
   }

   // Check if we can use the cached rates
   if (mCachedCaptureIndex != -1 && devIndex == mCachedCaptureIndex
         && (rate == 0.0 || make_iterator_range(mCachedCaptureRates).contains(rate)))
   {
      return mCachedCaptureRates;
   }

   std::vector<long> supported;
   int irate = (int)rate;
   const PaDeviceInfo* devInfo = NULL;
   int i;

   devInfo = Pa_GetDeviceInfo(devIndex);

   if (!devInfo)
   {
      wxLogDebug(wxT("GetSupportedCaptureRates() Could not get device info!"));
      return supported;
   }

   auto latencyDuration = AudioIOLatencyDuration.Read();
   // Why not defaulting to 2 as elsewhere?
   auto recordChannels = AudioIORecordChannels.ReadWithDefault(1);

   // LLL: Remove when a proper method of determining actual supported
   //      DirectSound rate is devised.
   const PaHostApiInfo* hostInfo = Pa_GetHostApiInfo(devInfo->hostApi);
   bool isDirectSound = (hostInfo && hostInfo->type == paDirectSound);

   PaStreamParameters pars;

   pars.device = devIndex;
   pars.channelCount = recordChannels;
   pars.sampleFormat = paFloat32;
   pars.suggestedLatency = latencyDuration / 1000.0;
   pars.hostApiSpecificStreamInfo = NULL;

   for (i = 0; i < NumRatesToTry; i++)
   {
      // LLL: Remove when a proper method of determining actual supported
      //      DirectSound rate is devised.
      if (!(isDirectSound && RatesToTry[i] > 200000))
      {
         if (Pa_IsFormatSupported(&pars, NULL, RatesToTry[i]) == 0)
            supported.push_back(RatesToTry[i]);
         Pa_Sleep( 10 );// There are ALSA drivers that don't like being probed
         // too quickly.
      }
   }

   if (irate != 0 && !make_iterator_range(supported).contains(irate))
   {
      // LLL: Remove when a proper method of determining actual supported
      //      DirectSound rate is devised.
      if (!(isDirectSound && RatesToTry[i] > 200000))
         if (Pa_IsFormatSupported(&pars, NULL, irate) == 0)
            supported.push_back(irate);
   }

   return supported;
}

std::vector<long> AudioIOBase::GetSupportedSampleRates(
   int playDevice, int recDevice, double rate)
{
   // Not given device indices, look up prefs
   if (playDevice == -1) {
      playDevice = getPlayDevIndex();
   }
   if (recDevice == -1) {
      recDevice = getRecordDevIndex();
   }

   // Check if we can use the cached rates
   if (mCachedPlaybackIndex != -1 && mCachedCaptureIndex != -1 &&
         playDevice == mCachedPlaybackIndex &&
         recDevice == mCachedCaptureIndex &&
         (rate == 0.0 || make_iterator_range(mCachedSampleRates).contains(rate)))
   {
      return mCachedSampleRates;
   }

   auto playback = GetSupportedPlaybackRates(playDevice, rate);
   auto capture = GetSupportedCaptureRates(recDevice, rate);
   int i;

   // Return only sample rates which are in both arrays
   std::vector<long> result;

   for (i = 0; i < (int)playback.size(); i++)
      if (make_iterator_range(capture).contains(playback[i]))
         result.push_back(playback[i]);

   // If this yields no results, use the default sample rates nevertheless
/*   if (result.empty())
   {
      for (i = 0; i < NumStandardRates; i++)
         result.push_back(StandardRates[i]);
   }*/

   return result;
}

/** \todo: should this take into account PortAudio's value for
 * PaDeviceInfo::defaultSampleRate? In principal this should let us work out
 * which rates are "real" and which resampled in the drivers, and so prefer
 * the real rates. */
int AudioIOBase::GetOptimalSupportedSampleRate()
{
   auto rates = GetSupportedSampleRates();

   if (make_iterator_range(rates).contains(44100))
      return 44100;

   if (make_iterator_range(rates).contains(48000))
      return 48000;

   // if there are no supported rates, the next bit crashes. So check first,
   // and give them a "sensible" value if there are no valid values. They
   // will still get an error later, but with any luck may have changed
   // something by then. It's no worse than having an invalid default rate
   // stored in the preferences, which we don't check for
   if (rates.empty()) return 44100;

   return rates.back();
}

int AudioIOBase::getPlayDevIndex(const wxString &devNameArg)
{
   wxString devName(devNameArg);
   // if we don't get given a device, look up the preferences
   if (devName.empty())
      devName = AudioIOPlaybackDevice.Read();

   auto hostName = AudioIOHost.Read();
   PaHostApiIndex hostCnt = Pa_GetHostApiCount();
   PaHostApiIndex hostNum;
   for (hostNum = 0; hostNum < hostCnt; hostNum++)
   {
      const PaHostApiInfo *hinfo = Pa_GetHostApiInfo(hostNum);
      if (hinfo && wxString(wxSafeConvertMB2WX(hinfo->name)) == hostName)
      {
         for (PaDeviceIndex hostDevice = 0; hostDevice < hinfo->deviceCount; hostDevice++)
         {
            PaDeviceIndex deviceNum = Pa_HostApiDeviceIndexToDeviceIndex(hostNum, hostDevice);

            const PaDeviceInfo *dinfo = Pa_GetDeviceInfo(deviceNum);
            if (dinfo && DeviceName(dinfo) == devName && dinfo->maxOutputChannels > 0 )
            {
               // this device name matches the stored one, and works.
               // So we say this is the answer and return it
               return deviceNum;
            }
         }

         // The device wasn't found so use the default for this host.
         // LL:  At this point, preferences and active no longer match.
         return hinfo->defaultOutputDevice;
      }
   }

   // The host wasn't found, so use the default output device.
   // FIXME: TRAP_ERR PaErrorCode not handled well (this code is similar to input code
   // and the input side has more comments.)

   PaDeviceIndex deviceNum = Pa_GetDefaultOutputDevice();

   // Sometimes PortAudio returns -1 if it cannot find a suitable default
   // device, so we just use the first one available
   //
   // LL:  At this point, preferences and active no longer match
   //
   //      And I can't imagine how far we'll get specifying an "invalid" index later
   //      on...are we certain "0" even exists?
   if (deviceNum < 0) {
      wxASSERT(false);
      deviceNum = 0;
   }

   return deviceNum;
}

int AudioIOBase::getRecordDevIndex(const wxString &devNameArg)
{
   wxString devName(devNameArg);
   // if we don't get given a device, look up the preferences
   if (devName.empty())
      devName = AudioIORecordingDevice.Read();

   auto hostName = AudioIOHost.Read();
   PaHostApiIndex hostCnt = Pa_GetHostApiCount();
   PaHostApiIndex hostNum;
   for (hostNum = 0; hostNum < hostCnt; hostNum++)
   {
      const PaHostApiInfo *hinfo = Pa_GetHostApiInfo(hostNum);
      if (hinfo && wxString(wxSafeConvertMB2WX(hinfo->name)) == hostName)
      {
         for (PaDeviceIndex hostDevice = 0; hostDevice < hinfo->deviceCount; hostDevice++)
         {
            PaDeviceIndex deviceNum = Pa_HostApiDeviceIndexToDeviceIndex(hostNum, hostDevice);

            const PaDeviceInfo *dinfo = Pa_GetDeviceInfo(deviceNum);
            if (dinfo && DeviceName(dinfo) == devName && dinfo->maxInputChannels > 0 )
            {
               // this device name matches the stored one, and works.
               // So we say this is the answer and return it
               return deviceNum;
            }
         }

         // The device wasn't found so use the default for this host.
         // LL:  At this point, preferences and active no longer match.
         return hinfo->defaultInputDevice;
      }
   }

   // The host wasn't found, so use the default input device.
   // FIXME: TRAP_ERR PaErrorCode not handled well in getRecordDevIndex()
   PaDeviceIndex deviceNum = Pa_GetDefaultInputDevice();

   // Sometimes PortAudio returns -1 if it cannot find a suitable default
   // device, so we just use the first one available
   // PortAudio has an error reporting function.  We should log/report the error?
   //
   // LL:  At this point, preferences and active no longer match
   //
   //      And I can't imagine how far we'll get specifying an "invalid" index later
   //      on...are we certain "0" even exists?
   if (deviceNum < 0) {
      // JKC: This ASSERT will happen if you run with no config file
      // This happens once.  Config file will exist on the next run.
      // TODO: Look into this a bit more.  Could be relevant to blank Device Toolbar.
      wxASSERT(false);
      deviceNum = 0;
   }

   return deviceNum;
}

wxString AudioIOBase::GetDeviceInfo()
{
   wxStringOutputStream o;
   wxTextOutputStream s(o, wxEOL_UNIX);

   if (IsStreamActive()) {
      return XO("Stream is active ... unable to gather information.\n")
         .Translation();
   }


   // FIXME: TRAP_ERR PaErrorCode not handled.  3 instances in GetDeviceInfo().
   int recDeviceNum = Pa_GetDefaultInputDevice();
   int playDeviceNum = Pa_GetDefaultOutputDevice();
   int cnt = Pa_GetDeviceCount();

   // PRL:  why only into the log?
   wxLogDebug(wxT("Portaudio reports %d audio devices"),cnt);
   
   s << wxT("==============================\n");
   s << XO("Default recording device number: %d\n").Format( recDeviceNum );
   s << XO("Default playback device number: %d\n").Format( playDeviceNum);

   auto recDevice = AudioIORecordingDevice.Read();
   auto playDevice = AudioIOPlaybackDevice.Read();
   int j;

   // This gets info on all available audio devices (input and output)
   if (cnt <= 0) {
      s << XO("No devices found\n");
      return o.GetString();
   }

   const PaDeviceInfo* info;

   for (j = 0; j < cnt; j++) {
      s << wxT("==============================\n");

      info = Pa_GetDeviceInfo(j);
      if (!info) {
         s << XO("Device info unavailable for: %d\n").Format( j );
         continue;
      }

      wxString name = DeviceName(info);
      s << XO("Device ID: %d\n").Format( j );
      s << XO("Device name: %s\n").Format( name );
      s << XO("Host name: %s\n").Format( HostName(info) );
      s << XO("Recording channels: %d\n").Format( info->maxInputChannels );
      s << XO("Playback channels: %d\n").Format( info->maxOutputChannels );
      s << XO("Low Recording Latency: %g\n").Format( info->defaultLowInputLatency );
      s << XO("Low Playback Latency: %g\n").Format( info->defaultLowOutputLatency );
      s << XO("High Recording Latency: %g\n").Format( info->defaultHighInputLatency );
      s << XO("High Playback Latency: %g\n").Format( info->defaultHighOutputLatency );

      auto rates = GetSupportedPlaybackRates(j, 0.0);

      /* i18n-hint: Supported, meaning made available by the system */
      s << XO("Supported Rates:\n");
      for (int k = 0; k < (int) rates.size(); k++) {
         s << wxT("    ") << (int)rates[k] << wxT("\n");
      }

      if (name == playDevice && info->maxOutputChannels > 0)
         playDeviceNum = j;

      if (name == recDevice && info->maxInputChannels > 0)
         recDeviceNum = j;

      // Sometimes PortAudio returns -1 if it cannot find a suitable default
      // device, so we just use the first one available
      if (recDeviceNum < 0 && info->maxInputChannels > 0){
         recDeviceNum = j;
      }
      if (playDeviceNum < 0 && info->maxOutputChannels > 0){
         playDeviceNum = j;
      }
   }

   bool haveRecDevice = (recDeviceNum >= 0);
   bool havePlayDevice = (playDeviceNum >= 0);

   s << wxT("==============================\n");
   if (haveRecDevice)
      s << XO("Selected recording device: %d - %s\n").Format( recDeviceNum, recDevice );
   else
      s << XO("No recording device found for '%s'.\n").Format( recDevice );

   if (havePlayDevice)
      s << XO("Selected playback device: %d - %s\n").Format( playDeviceNum, playDevice );
   else
      s << XO("No playback device found for '%s'.\n").Format( playDevice );

   std::vector<long> supportedSampleRates;

   if (havePlayDevice && haveRecDevice) {
      supportedSampleRates = GetSupportedSampleRates(playDeviceNum, recDeviceNum);

      s << XO("Supported Rates:\n");
      for (int k = 0; k < (int) supportedSampleRates.size(); k++) {
         s << wxT("    ") << (int)supportedSampleRates[k] << wxT("\n");
      }
   }
   else {
      s << XO("Cannot check mutual sample rates without both devices.\n");
      return o.GetString();
   }

   return o.GetString();
}

#ifdef EXPERIMENTAL_MIDI_OUT
// FIXME: When EXPERIMENTAL_MIDI_IN is added (eventually) this should also be enabled -- Poke
wxString AudioIOBase::GetMidiDeviceInfo()
{
   wxStringOutputStream o;
   wxTextOutputStream s(o, wxEOL_UNIX);

   if (IsStreamActive()) {
      return XO("Stream is active ... unable to gather information.\n")
         .Translation();
   }


   // XXX: May need to trap errors as with the normal device info
   int recDeviceNum = Pm_GetDefaultInputDeviceID();
   int playDeviceNum = Pm_GetDefaultOutputDeviceID();
   int cnt = Pm_CountDevices();

   // PRL:  why only into the log?
   wxLogDebug(wxT("PortMidi reports %d MIDI devices"), cnt);

   s << wxT("==============================\n");
   s << XO("Default recording device number: %d\n").Format( recDeviceNum );
   s << XO("Default playback device number: %d\n").Format( playDeviceNum );

   wxString recDevice = gPrefs->Read(wxT("/MidiIO/RecordingDevice"), wxT(""));
   wxString playDevice = gPrefs->Read(wxT("/MidiIO/PlaybackDevice"), wxT(""));

   // This gets info on all available audio devices (input and output)
   if (cnt <= 0) {
      s << XO("No devices found\n");
      return o.GetString();
   }

   for (int i = 0; i < cnt; i++) {
      s << wxT("==============================\n");

      const PmDeviceInfo* info = Pm_GetDeviceInfo(i);
      if (!info) {
         s << XO("Device info unavailable for: %d\n").Format( i );
         continue;
      }

      wxString name = wxSafeConvertMB2WX(info->name);
      wxString hostName = wxSafeConvertMB2WX(info->interf);

      s << XO("Device ID: %d\n").Format( i );
      s << XO("Device name: %s\n").Format( name );
      s << XO("Host name: %s\n").Format( hostName );
      /* i18n-hint: Supported, meaning made available by the system */
      s << XO("Supports output: %d\n").Format( info->output );
      /* i18n-hint: Supported, meaning made available by the system */
      s << XO("Supports input: %d\n").Format( info->input );
      s << XO("Opened: %d\n").Format( info->opened );

      if (name == playDevice && info->output)
         playDeviceNum = i;

      if (name == recDevice && info->input)
         recDeviceNum = i;

      // XXX: This is only done because the same was applied with PortAudio
      // If PortMidi returns -1 for the default device, use the first one
      if (recDeviceNum < 0 && info->input){
         recDeviceNum = i;
      }
      if (playDeviceNum < 0 && info->output){
         playDeviceNum = i;
      }
   }

   bool haveRecDevice = (recDeviceNum >= 0);
   bool havePlayDevice = (playDeviceNum >= 0);

   s << wxT("==============================\n");
   if (haveRecDevice)
      s << XO("Selected MIDI recording device: %d - %s\n").Format( recDeviceNum, recDevice );
   else
      s << XO("No MIDI recording device found for '%s'.\n").Format( recDevice );

   if (havePlayDevice)
      s << XO("Selected MIDI playback device: %d - %s\n").Format( playDeviceNum, playDevice );
   else
      s << XO("No MIDI playback device found for '%s'.\n").Format( playDevice );

   // Mention our conditional compilation flags for Alpha only
#ifdef IS_ALPHA

   // Not internationalizing these alpha-only messages
   s << wxT("==============================\n");
#ifdef EXPERIMENTAL_MIDI_OUT
   s << wxT("EXPERIMENTAL_MIDI_OUT is enabled\n");
#else
   s << wxT("EXPERIMENTAL_MIDI_OUT is NOT enabled\n");
#endif
#ifdef EXPERIMENTAL_MIDI_IN
   s << wxT("EXPERIMENTAL_MIDI_IN is enabled\n");
#else
   s << wxT("EXPERIMENTAL_MIDI_IN is NOT enabled\n");
#endif

#endif

   return o.GetString();
}
#endif

StringSetting AudioIOHost{
   L"/AudioIO/Host", L"" };
DoubleSetting AudioIOLatencyCorrection{
   L"/AudioIO/LatencyCorrection", -130.0 };
DoubleSetting AudioIOLatencyDuration{
   L"/AudioIO/LatencyDuration", 100.0 };
StringSetting AudioIOPlaybackDevice{
   L"/AudioIO/PlaybackDevice", L"" };
IntSetting AudioIORecordChannels{
   L"/AudioIO/RecordChannels", 2 };
StringSetting AudioIORecordingDevice{
   L"/AudioIO/RecordingDevice", L"" };
StringSetting AudioIORecordingSource{
   L"/AudioIO/RecordingSource", L"" };
IntSetting AudioIORecordingSourceIndex{
   L"/AudioIO/RecordingSourceIndex", -1 };
