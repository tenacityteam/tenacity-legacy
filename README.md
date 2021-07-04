[![Audacity](https://forum.audacityteam.org/styles/prosilver/theme/images/Audacity-logo_75px_trans_forum.png)](https://www.audacityteam.org) 
=========================

[**Audacity**](https://www.audacityteam.org) is an easy-to-use, multi-track audio editor and recorder for Windows, Mac OS X, GNU/Linux and other operating systems. Developed by a group of volunteers as open source.

- **Recording** from any real, or virtual audio device that is available to the host system.
- **Export / Import** a wide range of audio formats, extendible with FFmpeg.
- **High quality** using 32-bit float audio processing.
- **Plug-ins** Support for multiple audio plug-in formats, including VST, LV2, AU.
- **Macros** for chaining commands and batch processing.
- **Scripting** in Python, Perl, or any language that supports named pipes.
- **Nyquist** Very powerful built-in scripting language that may also be used to create plug-ins.
- **Editing** multi-track editing with sample accuracy and arbitrary sample rates.
- **Accessibility** for VI users.
- **Analysis and visualization** tools to analyze audio, or other signal data.

## This repository

This repository is a fork of Audacity that tries to revert all the sketchy changes made to the software in recent months, mostly related to data collection. You can find more informations on what happened in these Github issues on the original Audacity repository :

- [**Privacy policy that doesn't respect the GPL**](https://github.com/audacity/audacity/issues/1213)
- [**Discussion on the Contributer's License Agreement (CLA), goes against the GPL**](https://github.com/audacity/audacity/discussions/932)
- [**Pull request trying to implement telemetry using Google's services (did not pass)**](https://github.com/audacity/audacity/pull/835)

## Getting Started

For end users, the latest Windows and macOS release of the *original* version of Audacity is available from the *original* [Audacity website](https://www.audacityteam.org/download/). Note that this is the unmodified version which still contains some sketchy code.
Help with using Audacity is available from the original [Audacity Forum](https://forum.audacityteam.org/).

Build instructions are available [here](BUILDING.md).

More information for developers is available from the original [Audacity Wiki](https://wiki.audacityteam.org/wiki/For_Developers).
