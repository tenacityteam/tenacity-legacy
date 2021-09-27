set( TARGET Tenacity )
set( TARGET_ROOT ${CMAKE_SOURCE_DIR}/src )

message( STATUS "========== Configuring ${TARGET} ==========" )

# Allow modules to link against the Tenacity executable
set( CMAKE_ENABLE_EXPORTS ON )

add_executable( ${TARGET} )
add_dependencies( ${TARGET} locale )
add_dependencies( ${TARGET} nyquist )
add_dependencies( ${TARGET} plug-ins )

def_vars()

if( NOT GIT_FOUND )
  find_package( Git )
endif()

# ~~~
# Add a target that will provide the git revision info whenever it changes.
# Must be done at build time, not configuration time.
# ~~~
if( GIT_FOUND )
  add_custom_target( version
    COMMAND
      ${CMAKE_COMMAND}
      -D GIT="${GIT_EXECUTABLE}"
      -D GIT_DESCRIBE="${GIT_DESCRIBE}"
      -D "_PRVDIR=${_PRVDIR}"
      -P "cmake-proxies/cmake-modules/Version.cmake"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )

  add_dependencies( ${TARGET} version )
endif()

# Handle Audio Units option
if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
  cmd_option( use_audio_units "Use Audio Units plug-in support [ON, OFF]" ON )
  set( USE_AUDIO_UNITS ${use_audio_units} CACHE INTERNAL "" )
endif()

# ~~~
# Detect, which version of FileDialog to use
# Assume that GTK is used on platforms other than Windows or macOS
# ~~~
if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
  set( wxIS_MAC ON )
elseif( CMAKE_SYSTEM_NAME MATCHES "Windows" )
  set( wxIS_WIN ON )
else()
  set( wxIS_GTK ON )
endif()

# Handle Ladspa option
cmd_option( use_ladspa "Use LADSPA plug-in support [ON, OFF]" ON )
set( USE_LADSPA ${use_ladspa} CACHE INTERNAL "" )

# ~~~
# General source files
# ~~~
list( APPEND SOURCES PRIVATE
  AColor.cpp
  AColor.h
  AColorResources.h
  ActiveProjects.cpp
  ActiveProjects.h
  AboutDialog.cpp
  AboutDialog.h
  AdornedRulerPanel.cpp
  AdornedRulerPanel.h
  AllThemeResources.cpp
  AllThemeResources.h
  AttachedVirtualFunction.h
  AudacityApp.cpp
  AudacityApp.h
  $<$<BOOL:${wxIS_MAC}>:AudacityApp.mm>
  AudacityException.cpp
  AudacityException.h
  AudacityFileConfig.cpp
  AudacityFileConfig.h
  AudacityHeaders.cpp
  AudacityHeaders.h
  AudacityLogger.cpp
  AudacityLogger.h
  AudioIO.cpp
  AudioIO.h
  AudioIOBase.cpp
  AudioIOBase.h
  AudioIOBufferHelper.h
  AudioIOListener.h
  AutoRecoveryDialog.cpp
  AutoRecoveryDialog.h
  BatchCommandDialog.cpp
  BatchCommandDialog.h
  BatchCommands.cpp
  BatchCommands.h
  BatchProcessDialog.cpp
  BatchProcessDialog.h
  Benchmark.cpp
  Benchmark.h
  BuildInfo.h
  CellularPanel.cpp
  CellularPanel.h
  ClassicThemeAsCeeCode.h
  ClientData.h
  ClientDataHelpers.h
  Clipboard.cpp
  Clipboard.h
  CommonCommandFlags.cpp
  CommonCommandFlags.h
  DarkThemeAsCeeCode.h
  DBConnection.cpp
  DBConnection.h
  DeviceChange.cpp
  DeviceChange.h
  DeviceManager.cpp
  DeviceManager.h
  Diags.cpp
  Diags.h
  Dither.cpp
  Dither.h
  Envelope.cpp
  Envelope.h
  EnvelopeEditor.cpp
  EnvelopeEditor.h
  FFT.cpp
  FFT.h
  FFmpeg.cpp
  FFmpeg.h
  FileException.cpp
  FileException.h
  FileFormats.cpp
  FileFormats.h
  FileIO.cpp
  FileIO.h
  FileNames.cpp
  FileNames.h
  FreqWindow.cpp
  FreqWindow.h
  HelpText.cpp
  HelpText.h
  HiContrastThemeAsCeeCode.h
  HistoryWindow.cpp
  HistoryWindow.h
  HitTestResult.h
  ImageManipulation.cpp
  ImageManipulation.h
  InconsistencyException.cpp
  InconsistencyException.h
  InterpolateAudio.cpp
  InterpolateAudio.h
  KeyboardCapture.cpp
  KeyboardCapture.h
  LabelDialog.cpp
  LabelDialog.h
  LabelTrack.cpp
  LabelTrack.h
  LangChoice.cpp
  LangChoice.h
  Legacy.cpp
  Legacy.h
  LightThemeAsCeeCode.h
  Lyrics.cpp
  Lyrics.h
  LyricsWindow.cpp
  LyricsWindow.h
  MacroMagic.h
  Matrix.cpp
  Matrix.h
  Menus.cpp
  Menus.h
  Mix.cpp
  Mix.h
  MixerBoard.cpp
  MixerBoard.h
  ModuleManager.cpp
  ModuleManager.h
  ModuleSettings.cpp
  ModuleSettings.h
  NoteTrack.cpp
  NoteTrack.h
  NumberScale.h
  PitchName.cpp
  PitchName.h
  PlatformCompatibility.cpp
  PlatformCompatibility.h
  PlaybackSchedule.cpp
  PlaybackSchedule.h
  PluginManager.cpp
  PluginManager.h
  PluginRegistrationDialog.cpp
  PluginRegistrationDialog.h
  Prefs.cpp
  Prefs.h
  Printing.cpp
  Printing.h
  Profiler.cpp
  Profiler.h
  Project.cpp
  Project.h
  ProjectAudioIO.cpp
  ProjectAudioIO.h
  ProjectAudioManager.cpp
  ProjectAudioManager.h
  ProjectFileIO.cpp
  ProjectFileIO.h
  ProjectFileIORegistry.cpp
  ProjectFileIORegistry.h
  ProjectFileManager.cpp
  ProjectFileManager.h
  ProjectHistory.cpp
  ProjectHistory.h
  ProjectManager.cpp
  ProjectManager.h
  ProjectSelectionManager.cpp
  ProjectSelectionManager.h
  ProjectSerializer.cpp
  ProjectSerializer.h
  ProjectSettings.cpp
  ProjectSettings.h
  ProjectStatus.cpp
  ProjectStatus.h
  ProjectWindow.cpp
  ProjectWindow.h
  ProjectWindowBase.cpp
  ProjectWindowBase.h
  RealFFTf.cpp
  RealFFTf.h
  RealFFTf48x.cpp
  RealFFTf48x.h
  RefreshCode.h
  Registrar.h
  Registry.cpp
  Registry.h
  Resample.cpp
  Resample.h
  RingBuffer.cpp
  RingBuffer.h
  SampleBlock.cpp
  SampleBlock.h
  SampleFormat.cpp
  SampleFormat.h
  Screenshot.cpp
  Screenshot.h
  SelectUtilities.cpp
  SelectUtilities.h
  SelectedRegion.cpp
  SelectedRegion.h
  SelectionState.cpp
  SelectionState.h
  Sequence.cpp
  Sequence.h
  Shuttle.cpp
  Shuttle.h
  ShuttleGetDefinition.cpp
  ShuttleGetDefinition.h
  ShuttleGui.cpp
  ShuttleGui.h
  ShuttlePrefs.cpp
  ShuttlePrefs.h
  Snap.cpp
  Snap.h
  SoundActivatedRecord.cpp
  SoundActivatedRecord.h
  Spectrum.cpp
  Spectrum.h
  SpectrumAnalyst.cpp
  SpectrumAnalyst.h
  SplashDialog.cpp
  SplashDialog.h
  SqliteSampleBlock.cpp
  SseMathFuncs.cpp
  SseMathFuncs.h
  Tags.cpp
  Tags.h
  TempDirectory.cpp
  TempDirectory.h
  Theme.cpp
  Theme.h
  ThemeAsCeeCode.h
  TimeDialog.cpp
  TimeDialog.h
  TimeTrack.cpp
  TimeTrack.h
  TimerRecordDialog.cpp
  TimerRecordDialog.h
  Track.cpp
  Track.h
  TrackArtist.cpp
  TrackArtist.h
  TrackInfo.cpp
  TrackInfo.h
  TrackPanel.cpp
  TrackPanel.h
  TrackPanelAx.cpp
  TrackPanelAx.h
  TrackPanelCell.cpp
  TrackPanelCell.h
  TrackPanelDrawable.cpp
  TrackPanelDrawable.h
  TrackPanelDrawingContext.h
  TrackPanelListener.h
  TrackPanelMouseEvent.h
  TrackPanelResizeHandle.cpp
  TrackPanelResizeHandle.h
  TrackPanelResizerCell.cpp
  TrackPanelResizerCell.h
  TrackUtilities.cpp
  TrackUtilities.h
  UIHandle.cpp
  UIHandle.h
  UndoManager.cpp
  UndoManager.h
  UserException.cpp
  UserException.h
  ViewInfo.cpp
  ViewInfo.h
  VoiceKey.cpp
  VoiceKey.h
  WaveClip.cpp
  WaveClip.h
  WaveTrack.cpp
  WaveTrack.h
  WaveTrackLocation.h
  WrappedType.cpp
  WrappedType.h
  ZoomInfo.cpp
  ZoomInfo.h
  float_cast.h
  wxFileNameWrapper.h
)

# ~~~
# Commands source files
# ~~~
list( APPEND SOURCES PRIVATE
  commands/AppCommandEvent.cpp
  commands/AppCommandEvent.h
  commands/AudacityCommand.cpp
  commands/AudacityCommand.h
  commands/BatchEvalCommand.cpp
  commands/BatchEvalCommand.h
  commands/Command.cpp
  commands/Command.h
  commands/CommandBuilder.cpp
  commands/CommandBuilder.h
  commands/CommandContext.cpp
  commands/CommandContext.h
  commands/CommandDirectory.cpp
  commands/CommandDirectory.h
  commands/CommandFlag.h
  commands/CommandFunctors.h
  commands/CommandHandler.cpp
  commands/CommandHandler.h
  commands/CommandManager.cpp
  commands/CommandManager.h
  commands/CommandManagerWindowClasses.h
  commands/CommandMisc.h
  commands/CommandSignature.cpp
  commands/CommandSignature.h
  commands/CommandTargets.cpp
  commands/CommandTargets.h
  commands/CommandType.cpp
  commands/CommandType.h
  commands/CompareAudioCommand.cpp
  commands/CompareAudioCommand.h
  commands/Demo.cpp
  commands/Demo.h
  commands/DragCommand.cpp
  commands/DragCommand.h
  commands/GetInfoCommand.cpp
  commands/GetInfoCommand.h
  commands/GetTrackInfoCommand.cpp
  commands/GetTrackInfoCommand.h
  commands/HelpCommand.cpp
  commands/HelpCommand.h
  commands/ImportExportCommands.cpp
  commands/ImportExportCommands.h
  commands/Keyboard.cpp
  commands/Keyboard.h
  commands/LoadCommands.cpp
  commands/LoadCommands.h
  commands/MessageCommand.cpp
  commands/MessageCommand.h
  commands/OpenSaveCommands.cpp
  commands/OpenSaveCommands.h
  commands/PreferenceCommands.cpp
  commands/PreferenceCommands.h
  commands/ResponseQueue.cpp
  commands/ResponseQueue.h
  commands/ScreenshotCommand.cpp
  commands/ScreenshotCommand.h
  commands/ScriptCommandRelay.cpp
  commands/ScriptCommandRelay.h
  commands/SelectCommand.cpp
  commands/SelectCommand.h
  commands/SetClipCommand.cpp
  commands/SetClipCommand.h
  commands/SetEnvelopeCommand.cpp
  commands/SetEnvelopeCommand.h
  commands/SetLabelCommand.cpp
  commands/SetLabelCommand.h
  commands/SetProjectCommand.cpp
  commands/SetProjectCommand.h
  commands/SetTrackInfoCommand.cpp
  commands/SetTrackInfoCommand.h
  commands/Validators.h
)

# ~~~
# Effects source files
# ~~~
list( APPEND SOURCES PRIVATE
  # Built-in effects
  effects/Amplify.cpp
  effects/Amplify.h
  effects/AutoDuck.cpp
  effects/AutoDuck.h
  effects/BassTreble.cpp
  effects/BassTreble.h
  effects/Biquad.cpp
  effects/Biquad.h
  effects/ChangePitch.cpp
  effects/ChangePitch.h
  effects/ChangeSpeed.cpp
  effects/ChangeSpeed.h
  effects/ChangeTempo.cpp
  effects/ChangeTempo.h
  effects/ClickRemoval.cpp
  effects/ClickRemoval.h
  effects/Compressor.cpp
  effects/Compressor.h
  effects/Contrast.cpp
  effects/Contrast.h
  effects/Distortion.cpp
  effects/Distortion.h
  effects/DtmfGen.cpp
  effects/DtmfGen.h
  effects/EBUR128.cpp
  effects/EBUR128.h
  effects/Echo.cpp
  effects/Echo.h
  effects/Effect.cpp
  effects/Effect.h
  effects/EffectManager.cpp
  effects/EffectManager.h
  effects/EffectUI.cpp
  effects/EffectUI.h
  effects/Equalization.cpp
  effects/Equalization.h
  effects/Equalization48x.cpp
  effects/Equalization48x.h
  effects/Fade.cpp
  effects/Fade.h
  effects/FindClipping.cpp
  effects/FindClipping.h
  effects/Generator.cpp
  effects/Generator.h
  effects/Invert.cpp
  effects/Invert.h
  effects/LoadEffects.cpp
  effects/LoadEffects.h
  effects/Loudness.cpp
  effects/Loudness.h
  effects/Noise.cpp
  effects/Noise.h
  effects/NoiseReduction.cpp
  effects/NoiseReduction.h
  effects/NoiseRemoval.cpp
  effects/NoiseRemoval.h
  effects/Normalize.cpp
  effects/Normalize.h
  effects/Paulstretch.cpp
  effects/Paulstretch.h
  effects/Phaser.cpp
  effects/Phaser.h
  effects/RealtimeEffectManager.cpp
  effects/RealtimeEffectManager.h
  effects/Repair.cpp
  effects/Repair.h
  effects/Repeat.cpp
  effects/Repeat.h
  effects/Reverb.cpp
  effects/Reverb.h
  effects/Reverb_libSoX.h
  effects/Reverse.cpp
  effects/Reverse.h
  effects/SBSMSEffect.cpp
  effects/SBSMSEffect.h
  effects/ScienFilter.cpp
  effects/ScienFilter.h
  effects/ScoreAlignDialog.cpp
  effects/ScoreAlignDialog.h
  effects/Silence.cpp
  effects/Silence.h
  effects/SimpleMono.cpp
  effects/SimpleMono.h
  effects/SoundTouchEffect.cpp
  effects/SoundTouchEffect.h
  effects/StereoToMono.cpp
  effects/StereoToMono.h
  effects/TimeScale.cpp
  effects/TimeScale.h
  effects/TimeWarper.cpp
  effects/TimeWarper.h
  effects/ToneGen.cpp
  effects/ToneGen.h
  effects/TruncSilence.cpp
  effects/TruncSilence.h
  effects/TwoPassSimpleMono.cpp
  effects/TwoPassSimpleMono.h
  effects/Wahwah.cpp
  effects/Wahwah.h
  # VST effects
  $<$<BOOL:${USE_VST}>:
    effects/VST/VSTControl.h
    effects/VST/VSTEffect.cpp
    effects/VST/VSTEffect.h
    effects/VST/aeffectx.h
  >
  $<$<AND:$<BOOL:${USE_VST}>,$<BOOL:${wxIS_GTK}>>:
    effects/VST/VSTControlGTK.cpp
    effects/VST/VSTControlGTK.h
  >
  $<$<AND:$<BOOL:${USE_VST}>,$<BOOL:${wxIS_WIN}>>:
    effects/VST/VSTControlMSW.cpp
    effects/VST/VSTControlMSW.h
  >
  $<$<AND:$<BOOL:${USE_VST}>,$<BOOL:${wxIS_MAC}>>:
    effects/VST/VSTControlOSX.h
    effects/VST/VSTControlOSX.mm
  >
  # Audio Unit effects
  $<$<AND:$<BOOL:${USE_AUDIO_UNITS}>,$<BOOL:${wxIS_MAC}>>:
    effects/audiounits/AUControl.h
    effects/audiounits/AUControl.mm
    effects/audiounits/AudioUnitEffect.cpp
    effects/audiounits/AudioUnitEffect.h
  >
  # Ladspa effects
  $<$<BOOL:${USE_LADSPA}>:
    effects/ladspa/LadspaEffect.cpp
    effects/ladspa/LadspaEffect.h
    effects/ladspa/ladspa.h
  >
  # LV2 effects
  $<$<BOOL:${USE_LV2}>:
    effects/lv2/LV2Effect.cpp
    effects/lv2/LV2Effect.h
    effects/lv2/LoadLV2.cpp
    effects/lv2/LoadLV2.h
    effects/lv2/NativeWindow.h
    effects/lv2/lv2_external_ui.h
    effects/lv2/zix/common.h
    effects/lv2/zix/ring.cpp
    effects/lv2/zix/ring.h
  >
  # Nyquist effects
  $<$<BOOL:${USE_NYQUIST}>:
    effects/nyquist/LoadNyquist.cpp
    effects/nyquist/LoadNyquist.h
    effects/nyquist/Nyquist.cpp
    effects/nyquist/Nyquist.h
  >
  # VAMP effects
  $<$<BOOL:${USE_VAMP}>:
    effects/vamp/LoadVamp.cpp
    effects/vamp/LoadVamp.h
    effects/vamp/VampEffect.cpp
    effects/vamp/VampEffect.h
  >
)

# ~~~
# Exports source files
# ~~~
list( APPEND SOURCES PRIVATE
  export/Export.cpp
  export/Export.h
  # Standard exporters
  export/ExportCL.cpp
  export/ExportMP3.cpp
  export/ExportMP3.h
  export/ExportMultiple.cpp
  export/ExportMultiple.h
  export/ExportPCM.cpp
  # Optional exporters
  $<$<BOOL:${USE_FFMPEG}>:
    export/ExportFFmpeg.cpp
    export/ExportFFmpegDialogs.cpp
    export/ExportFFmpegDialogs.h
  >
  $<$<BOOL:${USE_LIBFLAC}>:
    export/ExportFLAC.cpp
  >
  $<$<BOOL:${USE_LIBTWOLAME}>:
    export/ExportMP2.cpp
  >
  $<$<AND:$<BOOL:${USE_LIBOGG}>,$<BOOL:${USE_LIBVORBIS}>>:
    export/ExportOGG.cpp
  >
)

# ~~~
# Imports source files
# ~~~
list( APPEND SOURCES PRIVATE
  import/FormatClassifier.cpp
  import/FormatClassifier.h
  import/Import.cpp
  import/Import.h
  import/ImportForwards.h
  import/MultiFormatReader.cpp
  import/MultiFormatReader.h
  import/RawAudioGuess.cpp
  import/RawAudioGuess.h
  import/SpecPowerMeter.cpp
  import/SpecPowerMeter.h
  # Standard importers
  import/ImportAUP.cpp
  import/ImportLOF.cpp
  import/ImportPCM.cpp
  import/ImportPlugin.h
  import/ImportPlugin.cpp
  import/ImportRaw.cpp
  import/ImportRaw.h
  # Optional importers
  $<$<BOOL:${USE_FFMPEG}>:
    import/ImportFFmpeg.cpp
  >
  $<$<BOOL:${USE_LIBFLAC}>:
    import/ImportFLAC.cpp
  >
  $<$<BOOL:${USE_MIDI}>:
    import/ImportMIDI.cpp
    import/ImportMIDI.h
  >
  $<$<BOOL:${USE_LIBMAD}>:
    import/ImportMP3.cpp
  >
  $<$<AND:$<BOOL:${USE_LIBOGG}>,$<BOOL:${USE_LIBVORBIS}>>:
    import/ImportOGG.cpp
  >
)

# ~~~
# Menus source files
# ~~~
list( APPEND SOURCES PRIVATE
  menus/ClipMenus.cpp
  menus/EditMenus.cpp
  menus/ExtraMenus.cpp
  menus/FileMenus.cpp
  menus/HelpMenus.cpp
  menus/LabelMenus.cpp
  menus/NavigationMenus.cpp
  menus/PluginMenus.cpp
  menus/SelectMenus.cpp
  menus/ToolbarMenus.cpp
  menus/TrackMenus.cpp
  menus/TransportMenus.cpp
  menus/ViewMenus.cpp
  menus/WindowMenus.cpp
)

# ~~~
# Preferences source files
# ~~~
list( APPEND SOURCES PRIVATE
  prefs/BatchPrefs.cpp
  prefs/BatchPrefs.h
  prefs/DevicePrefs.cpp
  prefs/DevicePrefs.h
  prefs/DirectoriesPrefs.cpp
  prefs/DirectoriesPrefs.h
  prefs/EffectsPrefs.cpp
  prefs/EffectsPrefs.h
  prefs/ExtImportPrefs.cpp
  prefs/ExtImportPrefs.h
  prefs/GUIPrefs.cpp
  prefs/GUIPrefs.h
  prefs/GUISettings.h
  prefs/ImportExportPrefs.cpp
  prefs/ImportExportPrefs.h
  prefs/KeyConfigPrefs.cpp
  prefs/KeyConfigPrefs.h
  prefs/LibraryPrefs.cpp
  prefs/LibraryPrefs.h
  prefs/MidiIOPrefs.cpp
  prefs/MidiIOPrefs.h
  prefs/ModulePrefs.cpp
  prefs/ModulePrefs.h
  prefs/MousePrefs.cpp
  prefs/MousePrefs.h
  prefs/PlaybackPrefs.cpp
  prefs/PlaybackPrefs.h
  prefs/PrefsDialog.cpp
  prefs/PrefsDialog.h
  prefs/PrefsPanel.cpp
  prefs/PrefsPanel.h
  prefs/QualityPrefs.cpp
  prefs/QualityPrefs.h
  prefs/QualitySettings.cpp
  prefs/QualitySettings.h
  prefs/RecordingPrefs.cpp
  prefs/RecordingPrefs.h
  prefs/SpectrogramSettings.cpp
  prefs/SpectrogramSettings.h
  prefs/SpectrumPrefs.cpp
  prefs/SpectrumPrefs.h
  prefs/ThemePrefs.cpp
  prefs/ThemePrefs.h
  prefs/TracksBehaviorsPrefs.cpp
  prefs/TracksBehaviorsPrefs.h
  prefs/TracksPrefs.cpp
  prefs/TracksPrefs.h
  prefs/WarningsPrefs.cpp
  prefs/WarningsPrefs.h
  prefs/WaveformPrefs.cpp
  prefs/WaveformPrefs.h
  prefs/WaveformSettings.cpp
  prefs/WaveformSettings.h
)

# ~~~
# Toolbars source files
# ~~~
list( APPEND SOURCES PRIVATE
  toolbars/ControlToolBar.cpp
  toolbars/ControlToolBar.h
  toolbars/DeviceToolBar.cpp
  toolbars/DeviceToolBar.h
  toolbars/EditToolBar.cpp
  toolbars/EditToolBar.h
  toolbars/MeterToolBar.cpp
  toolbars/MeterToolBar.h
  toolbars/ScrubbingToolBar.cpp
  toolbars/ScrubbingToolBar.h
  toolbars/SelectionBar.cpp
  toolbars/SelectionBar.h
  toolbars/SelectionBarListener.h
  toolbars/SpectralSelectionBar.cpp
  toolbars/SpectralSelectionBar.h
  toolbars/SpectralSelectionBarListener.h
  toolbars/TimeToolBar.cpp
  toolbars/TimeToolBar.h
  toolbars/ToolBar.cpp
  toolbars/ToolBar.h
  toolbars/ToolDock.cpp
  toolbars/ToolDock.h
  toolbars/ToolManager.cpp
  toolbars/ToolManager.h
  toolbars/ToolsToolBar.cpp
  toolbars/ToolsToolBar.h
  toolbars/TranscriptionToolBar.cpp
  toolbars/TranscriptionToolBar.h
)

# ~~~
# Tracks UI source files
# ~~~
list( APPEND SOURCES PRIVATE
  # Variant UIs
  tracks/labeltrack/ui/LabelDefaultClickHandle.cpp
  tracks/labeltrack/ui/LabelDefaultClickHandle.h
  tracks/labeltrack/ui/LabelGlyphHandle.cpp
  tracks/labeltrack/ui/LabelGlyphHandle.h
  tracks/labeltrack/ui/LabelTextHandle.cpp
  tracks/labeltrack/ui/LabelTextHandle.h
  tracks/labeltrack/ui/LabelTrackControls.cpp
  tracks/labeltrack/ui/LabelTrackControls.h
  tracks/labeltrack/ui/LabelTrackShifter.cpp
  tracks/labeltrack/ui/LabelTrackVRulerControls.cpp
  tracks/labeltrack/ui/LabelTrackVRulerControls.h
  tracks/labeltrack/ui/LabelTrackView.cpp
  tracks/labeltrack/ui/LabelTrackView.h
  tracks/playabletrack/notetrack/ui/NoteTrackAffordanceControls.h
  tracks/playabletrack/notetrack/ui/NoteTrackAffordanceControls.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackButtonHandle.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackButtonHandle.h
  tracks/playabletrack/notetrack/ui/NoteTrackControls.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackControls.h
  tracks/playabletrack/notetrack/ui/NoteTrackShifter.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackSliderHandles.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackSliderHandles.h
  tracks/playabletrack/notetrack/ui/NoteTrackVRulerControls.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackVRulerControls.h
  tracks/playabletrack/notetrack/ui/NoteTrackVZoomHandle.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackVZoomHandle.h
  tracks/playabletrack/notetrack/ui/NoteTrackView.cpp
  tracks/playabletrack/notetrack/ui/NoteTrackView.h
  tracks/playabletrack/notetrack/ui/StretchHandle.cpp
  tracks/playabletrack/notetrack/ui/StretchHandle.h
  tracks/playabletrack/ui/PlayableTrackButtonHandles.cpp
  tracks/playabletrack/ui/PlayableTrackButtonHandles.h
  tracks/playabletrack/ui/PlayableTrackControls.cpp
  tracks/playabletrack/ui/PlayableTrackControls.h
  tracks/playabletrack/wavetrack/ui/CutlineHandle.cpp
  tracks/playabletrack/wavetrack/ui/CutlineHandle.h
  tracks/playabletrack/wavetrack/ui/SampleHandle.cpp
  tracks/playabletrack/wavetrack/ui/SampleHandle.h
  tracks/playabletrack/wavetrack/ui/SpectrumVRulerControls.cpp
  tracks/playabletrack/wavetrack/ui/SpectrumVRulerControls.h
  tracks/playabletrack/wavetrack/ui/SpectrumVZoomHandle.cpp
  tracks/playabletrack/wavetrack/ui/SpectrumVZoomHandle.h
  tracks/playabletrack/wavetrack/ui/SpectrumView.cpp
  tracks/playabletrack/wavetrack/ui/SpectrumView.h
  tracks/playabletrack/wavetrack/ui/WaveTrackAffordanceControls.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackAffordanceControls.h
  tracks/playabletrack/wavetrack/ui/WaveTrackControls.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackControls.h
  tracks/playabletrack/wavetrack/ui/WaveTrackShifter.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackSliderHandles.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackSliderHandles.h
  tracks/playabletrack/wavetrack/ui/WaveTrackVRulerControls.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackVRulerControls.h
  tracks/playabletrack/wavetrack/ui/WaveTrackVZoomHandle.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackVZoomHandle.h
  tracks/playabletrack/wavetrack/ui/WaveTrackView.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackView.h
  tracks/playabletrack/wavetrack/ui/WaveTrackViewConstants.cpp
  tracks/playabletrack/wavetrack/ui/WaveTrackViewConstants.h
  tracks/playabletrack/wavetrack/ui/WaveformVRulerControls.cpp
  tracks/playabletrack/wavetrack/ui/WaveformVRulerControls.h
  tracks/playabletrack/wavetrack/ui/WaveformVZoomHandle.cpp
  tracks/playabletrack/wavetrack/ui/WaveformVZoomHandle.h
  tracks/playabletrack/wavetrack/ui/WaveformView.cpp
  tracks/playabletrack/wavetrack/ui/WaveformView.h
  tracks/timetrack/ui/TimeTrackControls.cpp
  tracks/timetrack/ui/TimeTrackControls.h
  tracks/timetrack/ui/TimeTrackVRulerControls.cpp
  tracks/timetrack/ui/TimeTrackVRulerControls.h
  tracks/timetrack/ui/TimeTrackVZoomHandle.cpp
  tracks/timetrack/ui/TimeTrackVZoomHandle.h
  tracks/timetrack/ui/TimeTrackView.cpp
  tracks/timetrack/ui/TimeTrackView.h
  # Main UI
  tracks/ui/AffordanceHandle.cpp
  tracks/ui/AffordanceHandle.h
  tracks/ui/BackgroundCell.cpp
  tracks/ui/BackgroundCell.h
  tracks/ui/ButtonHandle.cpp
  tracks/ui/ButtonHandle.h
  tracks/ui/CommonTrackControls.cpp
  tracks/ui/CommonTrackControls.h
  tracks/ui/CommonTrackPanelCell.cpp
  tracks/ui/CommonTrackPanelCell.h
  tracks/ui/CommonTrackView.cpp
  tracks/ui/CommonTrackView.h
  tracks/ui/EditCursorOverlay.cpp
  tracks/ui/EditCursorOverlay.h
  tracks/ui/EnvelopeHandle.cpp
  tracks/ui/EnvelopeHandle.h
  tracks/ui/PlayIndicatorOverlay.cpp
  tracks/ui/PlayIndicatorOverlay.h
  tracks/ui/ScrubUI.cpp
  tracks/ui/ScrubUI.h
  tracks/ui/Scrubbing.cpp
  tracks/ui/Scrubbing.h
  tracks/ui/SelectHandle.cpp
  tracks/ui/SelectHandle.h
  tracks/ui/SliderHandle.cpp
  tracks/ui/SliderHandle.h
  tracks/ui/TimeShiftHandle.cpp
  tracks/ui/TimeShiftHandle.h
  tracks/ui/TrackButtonHandles.cpp
  tracks/ui/TrackButtonHandles.h
  tracks/ui/TrackControls.cpp
  tracks/ui/TrackControls.h
  tracks/ui/TrackSelectHandle.cpp
  tracks/ui/TrackSelectHandle.h
  tracks/ui/TrackVRulerControls.cpp
  tracks/ui/TrackVRulerControls.h
  tracks/ui/TrackView.cpp
  tracks/ui/TrackView.h
  tracks/ui/ZoomHandle.cpp
  tracks/ui/ZoomHandle.h
)

# ~~~
# UI helpers source files
# ~~~
list( APPEND SOURCES PRIVATE
  ui/AccessibleLinksFormatter.h
  ui/AccessibleLinksFormatter.cpp
)

# ~~~
# Widgets source files
# ~~~
list( APPEND SOURCES PRIVATE
  widgets/AButton.cpp
  widgets/AButton.h
  widgets/ASlider.cpp
  widgets/ASlider.h
  widgets/AttachableScrollBar.cpp
  widgets/AttachableScrollBar.h
  widgets/AudacityMessageBox.cpp
  widgets/AudacityMessageBox.h
  widgets/BackedPanel.cpp
  widgets/BackedPanel.h
  widgets/ErrorDialog.cpp
  widgets/ErrorDialog.h
  widgets/ExpandingToolBar.cpp
  widgets/ExpandingToolBar.h
  widgets/FileConfig.cpp
  widgets/FileConfig.h
  widgets/FileDialog/FileDialog.cpp
  widgets/FileDialog/FileDialog.h
  $<$<BOOL:${wxIS_WIN}>:
    widgets/FileDialog/win/FileDialogPrivate.cpp
    widgets/FileDialog/win/FileDialogPrivate.h
  >
  $<$<BOOL:${wxIS_MAC}>:
    widgets/FileDialog/mac/FileDialogPrivate.mm
    widgets/FileDialog/mac/FileDialogPrivate.h
  >
  $<$<BOOL:${wxIS_GTK}>:
    widgets/FileDialog/gtk/FileDialogPrivate.cpp
    widgets/FileDialog/gtk/FileDialogPrivate.h
  >
  widgets/FileHistory.cpp
  widgets/FileHistory.h
  widgets/Grabber.cpp
  widgets/Grabber.h
  widgets/Grid.cpp
  widgets/Grid.h
  widgets/HelpSystem.cpp
  widgets/HelpSystem.h
  widgets/HtmlWindow.cpp
  widgets/HtmlWindow.h
  widgets/ImageRoll.cpp
  widgets/ImageRoll.h
  widgets/KeyView.cpp
  widgets/KeyView.h
  widgets/Meter.cpp
  widgets/Meter.h
  widgets/MeterPanelBase.cpp
  widgets/MeterPanelBase.h
  widgets/MultiDialog.cpp
  widgets/MultiDialog.h
  widgets/NumericTextCtrl.cpp
  widgets/NumericTextCtrl.h
  widgets/Overlay.cpp
  widgets/Overlay.h
  widgets/OverlayPanel.cpp
  widgets/OverlayPanel.h
  widgets/PopupMenuTable.cpp
  widgets/PopupMenuTable.h
  widgets/ProgressDialog.cpp
  widgets/ProgressDialog.h
  widgets/ReadOnlyText.h
  widgets/Ruler.cpp
  widgets/Ruler.h
  widgets/UnwritableLocationErrorDialog.cpp
  widgets/UnwritableLocationErrorDialog.h
  widgets/Warning.cpp
  widgets/Warning.h
  widgets/WindowAccessible.cpp
  widgets/WindowAccessible.h
  widgets/numformatter.cpp
  widgets/numformatter.h
  widgets/valnum.cpp
  widgets/valnum.h
  widgets/wxPanelWrapper.cpp
  widgets/wxPanelWrapper.h
  widgets/wxTextCtrlWrapper.h
)

# ~~~
# XML source files
# ~~~
list( APPEND SOURCES PRIVATE
  xml/XMLFileReader.cpp
  xml/XMLFileReader.h
  xml/XMLTagHandler.cpp
  xml/XMLTagHandler.h
  xml/XMLWriter.cpp
  xml/XMLWriter.h
  xml/audacityproject.dtd
)

# ~~~
# Experimental module
# ~~~
list( APPEND SOURCES PRIVATE Experimental.cmake )

# ~~~
# General headers
# ~~~
list( APPEND HEADERS
  ../include/tenacity/EffectInterface.h
  ../include/tenacity/Types.h
  ../include/tenacity/ConfigInterface.h
  ../include/tenacity/ModuleInterface.h
  ../include/tenacity/PluginInterface.h
  ../include/tenacity/ComponentInterface.h
  ../include/tenacity/EffectAutomationParameters.h
)

# ~~~
# General includes
# ~~~
list( APPEND INCLUDES PUBLIC
  "${_PRVDIR}"
  "${CMAKE_SOURCE_DIR}/include"
  "${TARGET_ROOT}"
)

# ~~~
# General resources
# ~~~
list( APPEND RESOURCES ../presets/EQDefaultCurves.xml )

# ~~~
# General definitions
# ~~~
list( APPEND DEFINES PRIVATE
  CMAKE
  $<$<BOOL:${HAVE_LRINT}>:
    HAVE_LRINT
  >
  $<$<BOOL:${HAVE_LRINTF}>:
    HAVE_LRINTF
  >
  $<$<BOOL:${HAVE_MLOCK}>:
    HAVE_MLOCK
  >
  $<$<PLATFORM_ID:Windows>:
    _CRT_SECURE_NO_WARNINGS
    __STDC_CONSTANT_MACROS
    STRICT
  >
)

# Default to enabling PCH on supported CMake versions if compiler caching is unavailable
if( CMAKE_VERSION VERSION_GREATER_EQUAL "3.16" AND NOT CCACHE_PROGRAM AND NOT SCCACHE_PROGRAM )
  cmd_option( use_pch "Use precompiled headers [yes, no]" YES )
  set(USE_PCH ${use_pch} CACHE INTERNAL "")
endif()

audacity_append_common_compiler_options( OPTIONS "${USE_PCH}" )

# ~~~
# Linker flags
# ~~~
list( APPEND LDFLAGS PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/MANIFEST:NO>
  $<$<CXX_COMPILER_ID:GNU>:-Wl,--disable-new-dtags>
)

# ~~~
# Publicly linked libraries
# ~~~
list( APPEND LIBRARIES PUBLIC
  ${CMAKE_REQUIRED_LIBRARIES}
  ZLIB::ZLIB
  wxWidgets::wxWidgets
  EXPAT::EXPAT
  mp3lame::mp3lame
  SndFile::sndfile
  Soxr::soxr
  PortAudio::PortAudio
  ${SQLite3_LIBRARIES}
  $<$<BOOL:${USE_FFMPEG}>:FFMPEG::FFMPEG>
  $<$<BOOL:${USE_LIBID3TAG}>:id3tag::id3tag>
  $<$<BOOL:${USE_LIBFLAC}>:FLAC++::FLAC++>
  $<$<BOOL:${USE_LIBMAD}>:MAD::MAD>
  $<$<BOOL:${USE_LIBOGG}>:Ogg::ogg>
  $<$<BOOL:${USE_LIBVORBIS}>:Vorbis::vorbis>
  $<$<BOOL:${USE_LIBVORBIS}>:Vorbis::vorbisenc>
  $<$<BOOL:${USE_LIBVORBIS}>:Vorbis::vorbisfile>
  $<$<BOOL:${USE_LIBTWOLAME}>:libtwolame::libtwolame>
  $<$<BOOL:${USE_LV2}>:lilv::lilv>
  $<$<BOOL:${USE_LV2}>:suil::suil>
  $<$<BOOL:${USE_MIDI}>:PortMidi::PortMidi>
  $<$<BOOL:${USE_MIDI}>:PortSMF::PortSMF>
  libnyquist
  $<$<BOOL:${USE_SBSMS}>:sbsms::sbsms>
  $<$<BOOL:${USE_SOUNDTOUCH}>:SoundTouch::SoundTouch>
  $<$<BOOL:${USE_VAMP}>:VampHostSDK::VampHostSDK>
  $<$<PLATFORM_ID:Linux,FreeBSD,OpenBSD,NetBSD,CYGWIN>:${GLIB_LIBRARIES}>
  $<$<PLATFORM_ID:Linux,FreeBSD,OpenBSD,NetBSD,CYGWIN>:GTK::GTK>
  $<$<PLATFORM_ID:Linux,FreeBSD,OpenBSD,NetBSD,CYGWIN>:Threads::Threads>
)

set( INSTALL_PREFIX "${_PREFIX}" )
set( PKGLIBDIR "${_PKGLIBDIR}" )
set( LIBDIR "${CMAKE_INSTALL_FULL_LIBDIR}" )
set( HAVE_GTK ${GTK_FOUND} )

# ~~~
# Platform specific configurations
# ~~~

if( CMAKE_SYSTEM_NAME MATCHES "Windows" )
  set_target_properties(
              ${TARGET}
              PROPERTIES
                         WIN32_EXECUTABLE ON # Make sure we're a GUI application
  )

  # Define the Windows specific resources
  list( APPEND WIN_RESOURCES ../win/tenacity.rc )

  # Copy the required wxWidgets libs into the bundle
  add_custom_command(
    TARGET ${TARGET}
    COMMAND ${CMAKE_COMMAND}
            -D SRC="${_EXEDIR}/${AUDACITY_NAME}.exe"
            -D DST="${_EXEDIR}"
            -D WXWIN="${_SHARED_PROXY_BASE_PATH}/$<CONFIG>/"
            -P "${AUDACITY_MODULE_PATH}/CopyLibs.cmake"
            -parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    POST_BUILD
  )

  # Copy the MSVC runtime libraries as well
  if( NOT "_X${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}" STREQUAL "_X" )
    add_custom_command(
      TARGET ${TARGET}
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} ${_DEST}
      POST_BUILD
    )
  endif()
elseif( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
  check_for_platform_version()
  if( PLATFORM_VERSION_SUPPORTED )
    list( APPEND LDFLAGS PRIVATE -Wl,-platform_version,macos,${MIN_MACOS_VERSION},${TARGET_MACOS_VERSION} )
  else()
    list( APPEND LDFLAGS PRIVATE -Wl,-sdk_version, ${TARGET_MACOS_VERSION} )
  endif()

   # Define Mac specific resources
   #TODO Remove Audacity from Mac resources
   list( APPEND MAC_RESOURCES
      ../mac/Resources/Tenacity.icns
      ../mac/Resources/TenacityAIFF.icns
      ../mac/Resources/TenacityAU.icns
      ../mac/Resources/TenacityMP3.icns
      ../mac/Resources/TenacityOGG.icns
      ../mac/Resources/TenacityProject.icns
      ../mac/Resources/TenacityWAV.icns
   )

  set_target_properties(
    ${TARGET}
    PROPERTIES # Build a bundle
               MACOSX_BUNDLE ON # Add the resources
               RESOURCE "${MAC_RESOURCES};${RESOURCES}"
  )
  set_target_property_all( ${TARGET} RUNTIME_OUTPUT_DIRECTORY "${_DESTDIR}" )

  # These sources must be compiled as Objective C++ and excluded from precompiled header usage
  set_source_files_properties(
    AudacityApp.mm
    KeyboardCapture.cpp
    effects/EffectUI.cpp
    effects/VST/VSTControlOSX.mm
    effects/audiounits/AUControl.mm
    menus/WindowMenus.cpp
    widgets/FileDialog/mac/FileDialogPrivate.mm
    PROPERTIES
               COMPILE_FLAGS "-ObjC++"
               SKIP_PRECOMPILE_HEADERS YES
  )

  list( APPEND LIBRARIES PRIVATE
    "-framework Foundation"
    "-framework Cocoa"
    "-framework Carbon"
  )

  if(USE_AUDIO_UNITS)
    list( APPEND LIBRARIES PRIVATE
      "-framework AudioUnit"
      "-framework CoreAudioKit"
    )
  endif()

  # Use the Aqua theme
  set( USE_AQUA_THEME 1 )

  # Copy the required wxWidgets libs into the bundle
  add_custom_command(
    TARGET ${TARGET}
    COMMAND ${CMAKE_COMMAND}
      -D SRC="${_EXEDIR}/${AUDACITY_NAME}"
      -D DST="${_PKGLIB}"
      -D WXWIN="${_SHARED_PROXY_BASE_PATH}/$<CONFIG>"
      -P "${AUDACITY_MODULE_PATH}/CopyLibs.cmake"
      -parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    POST_BUILD
  )

  # Define the Wrapper target
  set( WRAPPER_ROOT "${TARGET_ROOT}/../mac" )
  set( WRAPPER_SOURCES "${WRAPPER_ROOT}/Wrapper.c" )

  add_executable( Wrapper "${WRAPPER_SOURCES}" )
  add_dependencies( "${TARGET}" Wrapper )

  set_target_property_all( "Wrapper" RUNTIME_OUTPUT_DIRECTORY "${_EXEDIR}" )
  organize_source( "${WRAPPER_ROOT}" "mac" "${WRAPPER_SOURCES}" )
else()
  # TODO: Test ExportMP3.cpp more to allow us to do this on more OSes
  if( LAME_FOUND )
    set( DISABLE_DYNAMIC_LOADING_LAME YES )
  endif()

  if( FFMPEG_FOUND )
    set( DISABLE_DYNAMIC_LOADING_FFMPEG YES )
  endif()

  set_target_property_all( ${TARGET} RUNTIME_OUTPUT_DIRECTORY "${_DEST}" )

  # Build the list of mimetypes (we can't use generator expressions)
  set( MIMETYPES "" )

  list( APPEND MIMETYPES
    application/x-tenacity-project
  )

  if(USE_FFMPEG)
    list( APPEND MIMETYPES
      audio/aac
      audio/ac3
      audio/mp4
      audio/x-ms-wma
      video/mpeg
    )
  endif()


  if(USE_LIBFLAC)
    list( APPEND MIMETYPES
      audio/flac
      audio/x-flac
    )
  endif()

  if(USE_LIBMAD)
    list( APPEND MIMETYPES
      audio/mpeg
    )
  endif()

  if(USE_SNDFILE)
    list( APPEND MIMETYPES
      audio/basic
      audio/x-aiff
      audio/x-wav
    )
  endif()

  if(USE_LIBOGG AND USE_LIBVORBIS)
    list( APPEND MIMETYPES
      application/ogg
      audio/x-vorbis+ogg
    )
  endif()

  # Create the desktop file
  configure_file( org.tenacityaudio.Tenacity.desktop.in ${_INTDIR}/org.tenacityaudio.Tenacity.desktop )

  # Copy the required wxWidgets libs into the bundle
  add_custom_command(
    TARGET ${TARGET}
    COMMAND ${CMAKE_COMMAND}
           -D SRC="${_EXEDIR}/${AUDACITY_NAME}"
           -D DST="${_DEST}/${_PKGLIB}"
           -D WXWIN="${_SHARED_PROXY_BASE_PATH}/$<CONFIG>"
           -P "${AUDACITY_MODULE_PATH}/CopyLibs.cmake"
           -parallel ${CMAKE_BUILD_PARALLEL_LEVEL}
    POST_BUILD
  )
endif()

set_target_property_all( ${TARGET} RUNTIME_OUTPUT_NAME ${AUDACITY_NAME} )

organize_source( "${TARGET_ROOT}/.." "include" "${HEADERS}" )
organize_source( "${TARGET_ROOT}/../presets" "presets" "${RESOURCES}" )
organize_source( "${TARGET_ROOT}" "src" "${SOURCES}" )
organize_source( "${TARGET_ROOT}/../mac/Resources" "mac" "${MAC_RESOURCES}" )
organize_source( "${TARGET_ROOT}/../win" "win" "${WIN_RESOURCES}" )

#
# Get those pesky CMake files out of the way (on Windows)
#
source_group(
  TREE "${_INTDIR}"
  PREFIX "CMake"
  FILES
    ${_INTDIR}/CMakeFiles/${CMAKE_PROJECT_NAME}.dir/cmake_pch.cxx
    ${_INTDIR}/CMakeFiles/${CMAKE_PROJECT_NAME}.dir/cmake_pch.hxx
)

# Define TENACITY_DLL_API
import_symbol_define( import_symbol TENACITY_DLL )
export_symbol_define( export_symbol TENACITY_DLL )
list( APPEND DEFINES
 PRIVATE
   "${export_symbol}"
 INTERFACE
   "${import_symbol}"
)

target_sources( ${TARGET} PRIVATE ${HEADERS} ${SOURCES} ${RESOURCES} ${MAC_RESOURCES} ${WIN_RESOURCES} )
target_compile_definitions( ${TARGET} PRIVATE ${DEFINES} )
target_compile_definitions( ${TARGET} PRIVATE ${CXX_WARNINGS_SILENCE_DEFINES} )
target_compile_options( ${TARGET} PRIVATE ${OPTIONS} )
target_include_directories( ${TARGET} PRIVATE ${INCLUDES} )
target_link_options( ${TARGET} PRIVATE ${LDFLAGS} )
# Somehow the "version" target gets added to the LIBRARIES list? But only on Windows??
list( REMOVE_ITEM LIBRARIES version )
target_link_libraries( ${TARGET} ${LIBRARIES} )
target_link_libraries( ${TARGET} PUBLIC ${AUDACITY_LIBRARIES} )

if( USE_PCH )
  message( STATUS "Using precompiled headers" )
  target_precompile_headers(
    ${TARGET} PRIVATE $<$<PLATFORM_ID:Windows>:${CMAKE_BINARY_DIR}/src/private/configwin.h>
    $<$<PLATFORM_ID:Darwin>:${CMAKE_BINARY_DIR}/src/private/configmac.h>
    $<$<NOT:$<PLATFORM_ID:Windows,Darwin>>:${CMAKE_BINARY_DIR}/src/private/configunix.h>
    AudacityHeaders.h
  )
else()
  message( STATUS "Not using precompiled headers" )
endif()

if( CMAKE_GENERATOR MATCHES "Xcode|Visual Studio*" )
  install(
    DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/"
    DESTINATION "."
    USE_SOURCE_PERMISSIONS
    PATTERN "*.pdb" EXCLUDE
    PATTERN "*.ilk" EXCLUDE
  )
else()
  if( APPLE )
    install(
      TARGETS ${TARGET}
      DESTINATION "."
      RESOURCE DESTINATION "${_APPDIR}/Resources"
    )

    set_target_properties(${TARGET} PROPERTIES
        MACOSX_BUNDLE true
    )
    install(CODE "
        include(BundleUtilities)
        fixup_bundle( \${CMAKE_INSTALL_PREFIX}/Tenacity.app \"\" \"${CMAKE_PREFIX_PATH}/lib\" )")
  elseif( WIN32 )
    install(
      DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
      DESTINATION "."
      USE_SOURCE_PERMISSIONS
      PATTERN "*.pdb" EXCLUDE
      PATTERN "*.ilk" EXCLUDE
    )
  else()
    install( TARGETS ${TARGET} RUNTIME )
    install(
      DIRECTORY "${_DEST}/${_LIBDIR}/"
      DESTINATION "${_LIBDIR}"
      USE_SOURCE_PERMISSIONS FILES_MATCHING
      PATTERN "*.so*"
    )
    install(
      FILES "${_INTDIR}/org.tenacityaudio.Tenacity.desktop"
      DESTINATION "${_DATADIR}/applications"
    )
    install(
      FILES "${CMAKE_SOURCE_DIR}/LICENSE.txt" "${CMAKE_SOURCE_DIR}/README.md"
      DESTINATION "${_DATADIR}/doc/${AUDACITY_NAME}"
    )
    install(
      FILES "${_SRCDIR}/org.tenacityaudio.Tenacity.xml"
      DESTINATION "${_DATADIR}/mime/packages"
    )
    install(
      FILES "${CMAKE_SOURCE_DIR}/presets/EQDefaultCurves.xml"
      DESTINATION "${_PKGDATA}"
    )
    install(
      PROGRAMS "${PROJECT_SOURCE_DIR}/linux/tenacity.sh"
      DESTINATION "."
      RENAME "tenacity"
    )
  endif()
endif()

#[[
# collect dependency information for third party libraries
list( APPEND GRAPH_EDGES "Tenacity [shape=house]" )
foreach( LIBRARY ${LIBRARIES} )
   if( NOT LIBRARY MATCHES "PUBLIC|PRIVATE|INTERFACE" )
      canonicalize_node_name(LIBRARY "${LIBRARY}" )
      list( APPEND GRAPH_EDGES "\"${TARGET}\" -> \"${LIBRARY}\"" )
   endif()
endforeach()

#propagate collected edgets to root CMakeLists.txt
set( GRAPH_EDGES "${GRAPH_EDGES}" PARENT_SCOPE )
#]]
