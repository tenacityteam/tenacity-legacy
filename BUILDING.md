# Building Tenacity

## Prerequisites

### Linux

Most distributions do not package all of Tenacity's dependencies (yet).
wxWidgets 3.1 is suggested for building Tenacity but many distributions only
package wxWidgets 3.0. When 3.0 is used instead of 3.1, the main user visible
known drawback is that a few user interface texts will not be localized, and
the effects of not having various wxWidgets bug fixes that have not been
backported to the stable 3.0 series.
[PortMidi](https://github.com/mixxxdj/portmidi) and
[PortSMF](https://github.com/tenacityteam/portsmf) are required for MIDI support
but some distributions do not package PortSMF (Tenacity can still build without
MIDI support). [libsbsms](https://github.com/claytonotey/libsbsms) is an
optional dependency used for time stretching that is not available in many Linux
distribution package managers either. Optionally,
[vcpkg can be used](#vcpkg-on-Linux) to build dependencies from source which
may be helpful if your distribution is missing some packages.

Installing ccache and ninja-build is highly recommended for faster builds but
not required. CMake will automatically use ccache if it is installed.

#### wxWidgets from source

Where wxWidgets is not provided as a binary package,
a possible way to build and install wxWidgets from source is given by the
following example. After [downloading](https://www.wxwidgets.org/downloads/) and extracting the
source code into a location, such as `~/Downloads/wxWidgets-3.1.5`, it can be built in a local
directory, such as `~/Downloads/wxWidgets-build`:

```
cd ~/Downloads
mkdir wxWidgets-build && cd wxWidgets-build  # create and go to a new empty build directory
cmake -G Ninja ~/Downloads/wxWidgets-3.1.5   # configure wxWidgets from the assumed download location
cmake --build .  # actual compilation
```

Then to install wxWidgets in the default location, run

```
sudo cmake --build . --target install
```

#### Debian, Ubuntu, and derived distributions

To install Tenacity's dependencies, run:

```
sudo apt-get install build-essential libavcodec-dev libavformat-dev libavutil-dev libflac++-dev libglib2.0-dev libgtk-3-dev libid3tag0-dev libjack-dev liblilv-dev libmad0-dev libmp3lame-dev libogg-dev libpng-dev portaudio19-dev libportmidi-dev libserd-dev libsndfile1-dev libsord-dev libsoundtouch-dev libsoxr-dev libsuil-dev libtwolame-dev vamp-plugin-sdk libvorbis-dev lv2-dev zlib1g-dev cmake ninja-build libjpeg-dev libtiff-dev liblzma-dev libsqlite3-dev
```

##### Notes

- The version of PortSMF included in Debian & Ubuntu distributions
(`libportsmf-dev`) is not included in the above list because it is outdated
and using it will cause Tenacity to fail to build. If you want to use MIDI, it
is recommended to use our modified version of
[PortSMF](https://github.com/tenacityteam/portsmf) that you should build
from source until the packaged version gets updated. If you are installing
PortSMF to any other location than the default location where libraries get
installed (`/usr/local/lib`, `/usr/lib`, etc.) such as your `$HOME` directory,
you should point CMake to it by adding `-DCMAKE_PREFIX_PATH=/path/to/portsmf`
to the CMake configuration step.

- Additionally, if you happen to come across a package conflict involving a
package called `libjack0`, you may need to install `libjack-jack2-dev` instead
of `libjack-dev`.

- wxWidgets 3.1 is suggested but not packaged in Debian or Ubuntu. Refer
to the
[wxWidgets documentation](https://docs.wxwidgets.org/3.1/overview_cmake.html)
for how to install it from source code, or see the [previous section](#wxwidgets-from-source). The above package list
includes wxWidgets' build dependencies. If you install wxWidgets
somewhere other than the default /usr/local, you need to set the
`WX_CONFIG` environment variable to the location of the `wx-config`
script installed by wxWidgets to get CMake to find wxWidgets 3.1. For
example, if you installed wxWidgets to /home/user/local:

```
export WX_CONFIG=/home/user/local/bin/wx-config
```

- Alternatively, you may skip installing wxWidgets 3.1 and use 3.0 instead:

```
sudo apt-get install libwxgtk3.0-dev
```

#### Fedora

First, if you want to build Tenacity with FFmpeg support, enable the
[RPM Fusion](https://rpmfusion.org/) free repository if you do not
have enabled already. The nonfree RPM Fusion repository is not
required. If you do not mind building without FFmpeg support,
RPM Fusion is not required and this first step can be skipped (also
remove `ffmpeg-devel` from the end of the package list in the second
step below).

```
sudo dnf install https://mirrors.rpmfusion.org/free/fedora/rpmfusion-free-release-$(rpm -E %fedora).noarch.rpm
```

Then install Tenacity's dependencies:

```
sudo dnf install alsa-lib-devel cmake expat-devel flac-devel gcc-g++ gettext-devel lame-devel libid3tag-devel libmad-devel libogg-devel libsndfile-devel libvorbis-devel lilv-devel lv2-devel portaudio-devel portmidi-devel serd-devel sord-devel soundtouch-devel soxr-devel sqlite-devel sratom-devel suil-devel taglib-devel twolame-devel vamp-plugin-sdk-devel wxGTK-devel zlib-devel ccache ninja-build git ffmpeg-devel
```

If you use a high DPI screen, the wxWidgets 3.1.4 package in Fedora 34
does not work well for that. You can compile wxWidgets 3.1.5 instead of
using the Fedora package. Refer to the
[wxWidgets documentation](https://docs.wxwidgets.org/3.1/overview_cmake.html)
for details. If you install wxWidgets somewhere other than the default
/usr/local, you need to set the `WX_CONFIG` environment variable to
the location of the `wx-config` script installed by wxWidgets to get
CMake to find wxWidgets 3.1. For example, if you installed wxWidgets to
/home/user/local:

```
export WX_CONFIG=/home/user/local/bin/wx-config
```

#### Arch

To use wxWidgets 3.1, install `wxgtk3-dev-light` with your AUR helper of
choice, for example:

```
paru -S wxgtk3-dev-light
```

CMake requires explicitly specifying the path to the wx-config script from
this AUR package:

```
export WX_CONFIG=/usr/bin/wx-config-gtk3-3.1
```

Alternatively, install `wxgtk3` with pacman to use wxWidgets 3.0, and set
`WX_CONFIG=/usr/bin/wx-config-gtk3`.

Install the rest of the build dependencies from the main Arch repository:

```
sudo pacman -S cmake ninja ccache expat gcc-libs gdk-pixbuf2 glibc flac gtk3 glib2 libid3tag lilv libmad libogg portaudio portmidi libsndfile libsoxr suil twolame vamp-plugin-sdk libvorbis soundtouch ffmpeg
```

TODO: add portsmf and sbsms to this package list when those packages are updated.

#### Alpine

The build dependencies for Tenacity and wxWidgets can be found on Alpine's
community repository:

```
sudo apk add cmake samurai lame-dev libsndfile-dev soxr-dev sqlite-dev portaudio-dev portmidi-dev libid3tag-dev soundtouch-dev libmad-dev ffmpeg-dev
```

wxWidgets 3.1 is suggested but not packaged in Alpine Linux. Refer to the
[wxWidgets documentation](https://github.com/wxWidgets/wxWidgets/blob/master/docs/gtk/install.md)
for how to install it from source code, and make sure to set
`--disable-xlocale` in the configuration.

To install wxWidgets' dependencies:

```
sudo apk add gtk+3.0-dev zlib-dev libpng-dev tiff-dev libjpeg-turbo-dev expat-dev
```

Alternatively, install `wxgtk3-dev` with apk to use wxWidgets 3.0, and set
`WX_CONFIG=/usr/bin/wx-config-gtk3`.

TODO: add portsmf and libsbsms to this package list when aports are accepted.


#### FreeBSD

wxWidgets 3.1.5 is packaged in FreeBSD's repositories. Install it and the rest
of Tenacity's dependencies:

```
sudo pkg install wx31-gtk3 cmake ninja pkgconf lame libsndfile libsoxr portaudio lv2 lilv suil vamp-plugin-sdk portmidi libid3tag twolame libmad soundtouch ffmpeg
```

#### vcpkg on Linux

Optionally, you can build dependencies from source using vcpkg, with the
exception of wxWidgets due to
[bugs in vcpkg's wxwidgets package](https://github.com/microsoft/vcpkg/pull/17111).
vcpkg is not set up to build GTK or GLib either. To use vcpkg for
dependencies, pass `-D VCPKG=ON` to the CMake configure command. You will need
[nasm](https://www.nasm.us/) installed to build ffmpeg from vcpkg which you can
install from your distribution's package manager. If you use vcpkg, you need to
set the `WX_CONFIG` environment variable to the path of the wx-config script
installed by wxWidgets. For example, if you installed wxWidgets to /usr/local:

```
export WX_CONFIG=/usr/local/bin/wx-config
```

If you switch between system packages and vcpkg, you may need to delete
`CMakeCache.txt` inside your CMake build directory.

### Windows

Install
[Microsoft Visual Studio](https://visualstudio.microsoft.com/vs/community/)
with the **Desktop development with C++** installation option. Microsoft's ATL/MFC components are also needed to build the application.

Installing [sccache](https://github.com/mozilla/sccache) is highly recommended
for faster builds but not required. CMake will automatically use sccache if you
have it installed. You can install it from
[Chocolatey](https://community.chocolatey.org/packages/sccache):

```
choco install sccache
```

Alternatively, if you have a [Rust toolchain
installed](https://forge.rust-lang.org/infra/other-installation-methods.html),
you can build sccache from source:

```
cargo install sccache
```

Tenacity's dependencies will be built automatically with vcpkg when configuring
CMake. You can turn off vcpkg by passing `-D VCPKG=OFF` to the CMake
configuration command, but then it is up to you to install all of Tenacity's
dependencies.

Note that building the dependencies requires 10 GB of storage space.

### macOS

Install the Clang C++ compiler and macOS SDK from the Xcode command line tools.
If you have the Xcode IDE installed, you already have the command line tools
too. To install the Xcode command line tools without the Xcode IDE, launch the
Terminal application, and type the following command:

```
xcode-select --install
```

Click "Install" on the software update popup window that will appear and wait
for the download and installation to finish.

You will also need to install a few build tools and dependencies, which can be
installed from [Homebrew](https://brew.sh/):

```
brew install cmake ccache ninja nasm mono wxwidgets
```

You must set the `WX_CONFIG` environment variable for CMake to find wxWidgets
installed from Homebrew:

```
export WX_CONFIG=/usr/local/bin/wx-config
```

The rest of the dependencies will be built automatically with vcpkg when
configuring CMake. You turn off vcpkg by passing `-D VCPKG=OFF` to the CMake
configuration command, but then it is up to you to install all of Tenacity's
dependencies.

## Building Tenacity

On Windows, run the commands below from the x64 Native Tools Command Prompt. For
other operating systems, run them from a normal shell.

First, download the Tenacity source code:

```
git clone https://github.com/tenacityteam/tenacity.git
cd tenacity
```

Then, configure CMake. This will take a long time the first time on macOS and
Windows (or if you use `-D VCPKG=ON` on Linux) because vcpkg will compile
itself, then compile Tenacity's dependencies. `-G Ninja` is recommended for
faster builds but not required. Add `-D CMAKE_INSTALL_PREFIX=/some/path` to
change the installation path from the default /usr/local:

```
cmake -G Ninja -S . -B build
```

Build Tenacity:

```
cmake --build build --config Release
```

Run Tenacity:

```
build/bin/Release/tenacity
```

Optionally, install Tenacity:

```
cmake --install build
```

## Build options

These are set with a `-D` prefix, for example `cmake -DLV2=OFF ...`

  * **VCPKG** (ON|OFF): whether to use dependencies from vcpkg. ON by default
    for Windows and macOS; OFF by default for Linux.
  * **VCPKG_ROOT** (file path): path to vcpkg Git repository, defaults to
    using the vcpkg submodule in the Tenacity repository
  * **SCCACHE** (ON|OFF): whether to use sccache for compiler caching to
    speed up rebuilds. ON by default if sccache is installed.
  * **CCACHE** (ON|OFF): whether to use ccache for compiler caching to speed
    up rebuilds. ON by default if ccache is installed. If sccache and ccache
    are both installed, sccache will be prefered.

The following feature options are enabled by default if the required libraries
are found. You may explicitly disable them if you prefer or your distribution
has outdated libraries that do not build with Tenacity.

  * **MIDI** (ON|OFF): MIDI support. Requires PortMidi and PortSMF.
  * **ID3TAG** (ON|OFF): ID3 tag support for MP3 files. Requires libid3tag.
  * **MP3_DECODING** (ON|OFF): MP3 decoding support. Requires libmad.
  * **MP2_ENCODING** (ON|OFF): MP2 encoding support. Requires Twolame library.
  * **OGG** (ON|OFF): Ogg container format support. Requires libogg.
  * **VORBIS** (ON|OFF): Vorbis codec support. Requires libvorbis.
  * **FLAC** (ON|OFF): FLAC codec support. Requires libflac and libflac++ C++
    bindings.
  * **SBSMS** (ON|OFF): SBSMS timestretching support. Requires libsbsms.
  * **SOUNDTOUCH** (ON|OFF): SoundTouch timestretching support. Requires
    SoundTouch library.
  * **FFMPEG** (ON|OFF): Support for a wide variety of codecs with FFmpeg.
    Requires FFmpeg libraries.
  * **VAMP** (ON|OFF): VAMP plugin hosting support. Requires VAMP host SDK.
  * **LV2** (ON|OFF): LV2 plugin hosting support. Requires LV2, lilv, and
    suil libraries.
  * **VST2** (ON|OFF): VST2 plugin hosting support. Requires GTK with X11
    support on non-Apple/Windows.
