[![Tenacity](https://raw.githubusercontent.com/tenacityteam/assets/master/PNG/tenacity-logo-dark-readme.png)](https://tenacityaudio.org)

[![Chat on IRC](https://badgen.net/badge/irc/%23tenacity/green)](https://web.libera.chat/gamja/?channels=#tenacity)
[![License](https://badgen.net/badge/license/GPLv2/blue)](LICENSE.txt)
[![Open issues](https://badgen.net/github/open-issues/tenacityteam/tenacity)](https://github.com/tenacityteam/tenacity/issues)
[![GitHub builds](https://badgen.net/github/status/tenacityteam/tenacity)](https://github.com/tenacityteam/tenacity/actions?query=branch%3Amaster+event%3Apush)
[![builds.sr.ht](https://builds.sr.ht/~tenacity/tenacity/commits/.svg)](https://builds.sr.ht/~tenacity/tenacity/commits/?)
[![Translation status](https://hosted.weblate.org/widgets/tenacity/-/tenacity/svg-badge.svg)](https://hosted.weblate.org/engage/tenacity/)
[![Donators](https://opencollective.com/tenacity/tiers/badge.svg)](https://opencollective.com/tenacity#category-CONTRIBUTE)

Tenacity is an easy-to-use multi-track audio editor and recorder for Windows, macOS, Linux and other operating systems. It is built on top of the widely popular [Audacity](https://audacityteam.org/) and is being developed by a wide, diverse group of volunteers.

- **Recording** from audio devices (real or virtual)
- **Export / Import** a wide range of audio formats (extensible with FFmpeg)
- **High quality** including up to 32-bit float audio support
- **Plug-ins** providing support for VST, LV2, and AU plugins
- **Scripting** in the built-in scripting language Nyquist, or in Python, Perl and other languages with named pipes
- **Editing** arbitrary sampling and multi-track timeline
- **Accessibility** including editing via keyboard, screen reader support and narration support
- **Tools** useful in the analysis of signals, including audio

## Motivation

Our project initially started as a fork of [Audacity](https://audacityteam.org) as a result of multiple controversies and public relation crises, which you can find out more about here:

- [**Privacy policy which may violate the original project's GPL license**](https://github.com/audacity/audacity/issues/1213)
- [**Contributor's License Agreement (CLA) which may violate the same GPL license**](https://github.com/audacity/audacity/discussions/932)
- [**Attempts at adding telemetry using Google services for data collection**](https://github.com/audacity/audacity/pull/835)

Nevertheless, the goal of this project is to pick up what the original developers of Audacity the decades-long work by the original creators of Audacity and create an audio editor that is fresh, more modern, convenient and practical to use, with the help and the guidance of our users and our community.

## Download

### Tenacity

We have not released the first version of Tenacity yet, as it is still in the early stages of development.

Nevertheless, as a result of popular demand by the community, we decided to provide "nightly" downloads for Windows, macOS and Ubuntu Linux. These downloads are based on the latest version of Tenacity's code, and are not representative of our final product. Some features may be added, removed, or changed.

- [Nightly Downloads](https://tenacityaudio.org/very-early-builds)
- [Nightly Flatpak builds](https://github.com/tenacityteam/tenacity-flatpak-nightly/blob/main/README.md)
- [Unofficial Arch Linux (AUR) Package](https://aur.archlinux.org/packages/tenacity-git/)

### Audacity

The latest official version of Audacity that does not implement telemetry is `3.0.2`. Some Linux-based operating systems also ship Audacity with telemetry and networking features disabled by default.

Downloads for these versions can be found on the [Audacity website](https://www.audacityteam.org/download/). If you're looking for support regarding these versions, it may be worth consulting [Audacity's forum](https://forum.audacityteam.org/) or [Audacity's wiki](https://wiki.audacityteam.org/).

## Getting Started

Build instructions for Tenacity are available in the [BUILDING.md file](BUILDING.md).

## Contributing

To start contributing, please consult the [CONTRIBUTING.md file](CONTRIBUTING.md).

If you are planning to make a big change or if you are otherwise hesitant as to whether we want to incorporate something you want to work on in Tenacity itself, then just ask us about it in our [development mailing list](https://lists.sr.ht/~tenacity/tenacity-dev). We can discuss it and work together, so that neither our time or your time and hard effort goes to waste.

### Translating

You can help us translate Tenacity and make it more accessible on [Weblate](https://hosted.weblate.org/projects/tenacity).
