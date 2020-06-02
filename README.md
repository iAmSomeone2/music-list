# Music List

GUI and CLI app for locating music files and verifying album completeness.

## Table of Contents

- [Goals](#goals)
- [Build Instructions](#build-instructions)
  * [Required Libraries](#required-libraries)

## Goals

- [x] Quickly identify audio files in a given directory.
- [x] Utilize MusicBrainz data to group audio files based on album.
- [ ] Support most common audio file types
  * [x] FLAC
  * [x] Opus
  * [ ] Vorbis
  * [ ] AAC
  * [ ] MP3
- [x] Export organized data as JSON document.
- [ ] Provide GUI for easy use and viewing of data.
- [ ] Highlight tracks that are missing from a compilation/album.
- [ ] Optionally flag tracks that are not lossless.
- [ ] Configure build to work with distribution packages.
- [ ] Set up a changelog for first release.

## Build Instructions

### Required Packages

#### Fedora

- meson
- g++ (or clang)
- git
- jsoncpp-devel
- flac-devel
- opus-devel
- opusfile-devel
- (Optional) gtkmm30-devel
- (Optional) glibmm24-devel
- (Optional) gtest-devel

All-in-one install command:

``` bash
    sudo dnf install meson g++ git jsoncpp-devel flac-devel opus-devel opusfile-devel gtkmm30-devel glibmm24-devel gtest-devel
```

#### Manjaro Linux

- meson
- g++ (or clang)
- git
- jsoncpp
- flac
- opus
- opusfile
- (Optional) gtkmm30
- (Optional) glibmm24
- (Optional) gtest

All-in-one install command:

``` bash
    sudo pacman -S meson g++ git jsoncpp flac opus opusfile gtkmm30 glibmm24 gtest
```

### Set up

1. Clone repo:

``` bash
    git clone https://github.com/iAmSomeone2/music-list.git
```

2. Enter cloned directory:

``` bash
    cd music-list
```

3. Configure the Meson build directory:
   - Basic dev build:
   
   ``` bash
    meson build
   ```

   - Build with tests:

   ``` bash
    meson build -Denable_tests=true
   ```

   - Disable building GUI:

   ``` bash
    meson build -Denable_gui=false
   ```

   - Basic release build:

   ``` bash
    meson build -Dbuildtype=release
   ```

### Build

1. Run the build using Ninja

   ``` bash
    ninja -C build
   ```

   or 

   ``` bash
    cd build && ninja
   ```

   **Note:** Ninja should have automatically been installed with Meson.

2. Locate built executables under `build/src/cli` and `build/src/gui`.

3. ***Optional*** Run tests.

   ``` bash
    ninja test -C build
   ```

   or

   ``` bash
    cd build && ninja test
   ```

   **Note:** Given that the tests are reliant on copyrighted media, they must be configured exclusively for each test environment. Adjust the search location and expected file count constants in each of the tests to match your environment.
