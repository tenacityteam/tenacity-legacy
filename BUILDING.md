# Building Tenacity

## Prerequisites

* **python3** >= 3.5
* **conan** >= 1.32.0
* **cmake** >= 3.16
* A working C++ 14 compiler

### Conan

[The best way to install Conan is `pip`.](https://docs.conan.io/en/latest/installation.html)

To install Conan on Windows:

```
$ pip install conan
```

To install Conan on macOS and Linux:

```
$ sudo pip3 install conan
```

Alternatively, on macOS, Conan is available from `brew`.

### CMake

On Windows, please use the [prebuilt binaries](https://cmake.org/download/).

On macOS, the easiest way to install CMake is `brew install cmake`.

On Linux, `cmake` is usually available from the system package manager.

### Windows

We build Tenacity using [Microsoft Visual Studio 2019](https://visualstudio.microsoft.com/vs/community/). In order to build Tenacity **Desktop development with C++** workload is required.

As we require only C++14 - MSVC 2017 should work just fine too.

### MacOS

We build Tenacity using XCode 12. However, it is likely possible to build it with XCode 7.

### Linux

We use GCC 9, but any C++14 compliant compiler should work.

On Debian or Ubuntu, you can install everything required using the following commands:

```
$ sudo apt-get update
$ sudo apt-get install -y build-essential cmake git python3-pip
$ sudo pip3 install conan
$ sudo apt-get install libgtk2.0-dev libasound2-dev libavformat-dev libjack-jackd2-dev uuid-dev
```

## Building on Windows

1. Clone Tenacity from the Tenacity GitHub project. 
  
   For example, in the **git-bash** run:

    ```
    $ git clone https://github.com/tenacityteam/tenacity/
    ```

2. Open CMake GUI. 
   
   Set the **Where is the source code** to the location where Tenacity was cloned. 
   
   Set **Where to build the binaries** to the location you want to place your build in. It is preferred that this location is not within the directory with the source code.

3. Press **Configure**. You can choose which version of Visual Studio to use and the platform to build for in the pop-up. We support **x64** and **Win32** platforms. The **x64** platform is a default option. Press **Finish** to start the configuration process.

4. After successful configuration, you will see `Configuring done` in the last line of the log. Press **Generate** to generate the Visual Studio project. 

5. After you see "Generating done", press **Open Project** to open the project in Visual Studio.
   
6. Select "Build -> Build Solution".
   
7. You can now run and debug Tenacity!
      
Generally, steps 1-5 are only needed the first-time you configure. Then, after you've generated the solution, you can open it in Visual Studio next time. If the project configuration has changed, the IDE will invoke CMake internally. 

> Conan Center provides prebuilt binaries only for **x64**. Configuring the project for Win32 will take much longer, as all the 3rd party libraries will be built during the configuration.

## macOS

1. Clone Tenacity from the Tenacity GitHub project. 
  
    ```
    $ git clone https://github.com/tenacityteam/tenacity/
    ```

2. Configure Tenacity using CMake:
   ```
   $ mkdir build && cd build
   $ cmake -GXcode -T buildsystem=1 ../tenacity
   ```

3. Open the created XCode project:
   ```
   $ open Audacity.xcodeproj
   ```
   and build using the XCode IDE. 

Steps 1 and 2 are only required for first-time builds. 

Alternatively, you can use **CLion**. If you chose to do so, open the directory where you have cloned Tenacity using CLion and you are good to go.

At the moment we only support **x86_64** builds. It is possible to build using AppleSilicon hardware but **mad** and **id3tag** should be disabled:

```
cmake -GXCode -T buildsystem=1 -Duse_mad="off" -Duse_id3tag=off ../tenacity
```

## Linux & Other OS

1. Clone Tenacity from the Tenacity GitHub project. 
  
    ```
    $ git clone https://github.com/tenacityteam/tenacity/
    ```

2. Configure Tenacity using CMake:
   ```bash
   $ mkdir build && cd build
   $ cmake -G "Unix Makefiles" -Duse_ffmpeg=loaded ..
   ```
   By default, Debug build will be configured. To change that, pass `-DCMAKE_BUILD_TYPE=Release` to CMake.

3. Build Tenacity:
   ```bash
   $ make -j`nproc`
   ```
   Note that this may slow your computer down quite a bit. To avoid this, you can use the alternate command:
   ```bash
   $ make -j`let numcores=$(nproc)-2 ; echo $numcores`
   ```
   Or, you can manually specify the number of CPU cores to use:
   ```bash
   $ make -j2
   # Uses only 2 cores
   ```

4. Testing the build:
   Adding a "Portable Settings" folder allows Tenacity to ignore the settings of any existing Tenacity installation.
   ```
   $ cd bin/Debug
   $ mkdir "Portable Settings"
   $ ./audacity
   ```

At the moment, you are unable to install tenacity system-wide due conflits with libraries. You have to run Step 4 to use Tenacity. We are trying to fix that for the first stable release.

## Advanced

### CMake options

You can use `cmake -LH` to get a list of the options available (or use CMake GUI or `ccmake`). The list will include documentation about each option. For convenience, [here is a list](CMAKE_OPTIONS.md) of the most notable options.

### Building using system libraries

On Linux it is possible to build Tenacity using (almost) only the libraries provided by the package manager. Please, see the list of required libraries [here](linux/required_libraries.md).

```
$ mkdir build && cd build
$ cmake -G "Unix Makefiles" \
        -Duse_ffmpeg=loaded \
        -Dlib_preference=system \
        -Dobey_system_dependencies=On \
         ../tenacity
```

There are a few cases when the local library build is preferred:

1. **wxWidgets**: While Tenacity on **Linux** uses vanilla version of wxWidgets, we **require** that version **3.1.3** is used. This version is not available in most of the distributions.
2. **portaudio-v19**: Tenacity currently uses [some private APIs](https://github.com/audacity/audacity/issues/871), so using system portaudio is not yet possible.
3. **vamp-host-sdk**: Development packages are not available in Ubuntu 20.04.
4. **libnyquist** & **portmixer**: Libraries are not available in Ubuntu 20.04.
5. **sqlite3** & **libsmbs**: Libraries are very outdated in Ubuntu 20.04.

It is not advised to mix system and local libraries, except for the list above. `ZLib` is a very common dependency; it is possible to mix system and local libraries in one build. However, we advise against doing so.

There is a [`Dockerfile`](linux/build-environment/Dockerfile) that can be used as an example of how to build Tenacity using system libraries: 

```
$ docker build -t tenacity_linux_env .\linux\build-environment\
$ docker run --rm -v ${pwd}:/tenacity/tenacity/ -v ${pwd}/../build/linux-system:/tenacity/build -it tenacity_linux_env
```

To find system packages, we rely on `pkg-config`. There are several packages that have broken `*.pc` or do not use `pkg-config` at all. For the docker image - we handle this issue by installing the correct [`pc` files](linux/build-environment/pkgconfig/).
