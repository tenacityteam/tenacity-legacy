/**********************************************************************
 
 Audacity: A Digital Audio Editor
 
 QualitySettings.h
 
 Paul Licameli split from QualityPrefs.h
 
 **********************************************************************/

#ifndef __AUDACITY_QUALITY_SETTINGS__
#define __AUDACITY_QUALITY_SETTINGS__

#include "Prefs.h" // for EnumSetting

class IntSetting;

namespace QualitySettings {
extern TENACITY_DLL_API IntSetting DefaultSampleRate;
extern TENACITY_DLL_API EnumSetting< sampleFormat > SampleFormatSetting;
extern TENACITY_DLL_API sampleFormat SampleFormatChoice();
}

#endif
