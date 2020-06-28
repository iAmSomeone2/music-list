# Music List

GUI and CLI app for locating music files and verifying album completeness.

## Table of Contents

- [Goals](#goals)
- [Build Instructions](#build-instructions)
  * [Required Packages](#required-packages)
     - [Fedora](#fedora)
     - [Manjaro Linux](#manjaro-linux)
  * [Set up](#set-up)
  * [Build](#build)

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

- cmake
- g++ (or clang)
- git
- jsoncpp-devel
- flac-devel
- opus-devel
- opusfile-devel
- (Optional) gtest-devel

All-in-one install command:

``` bash
    sudo dnf install cmake g++ git jsoncpp-devel flac-devel opus-devel opusfile-devel gtest-devel
```

#### Manjaro Linux

- cmake
- g++ (or clang)
- git
- jsoncpp
- flac
- opus
- opusfile
- (Optional) gtest

All-in-one install command:

``` bash
    sudo pacman -S cmake g++ git jsoncpp flac opus opusfile gtest
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

3. Configure the cmake build directory:
    - Set up build directory
    
    ``` bash
      mkdir cmake-build-debug && cd cmake-build-debug
    ```

   - Basic dev build:
   
   ``` bash
     cmake ..
   ```

   - Build without tests:

   ``` bash
     cmake build -DENABLE_TESTS=off ..
   ```

   - Basic release build:

   ``` bash
     cmake build -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTS=off ..
   ```

### Build

1. Run the build using make

   ``` bash
    cd cmake-build-<build-type> && make -j$(nproc)
   ```

2. Locate built executables under `src/cli`.

3. ***Optional*** Run tests.

   ``` bash
    make test
   ```

   **Note:** Given that the tests are reliant on copyrighted media, they must be configured exclusively for each test
   environment. Adjust the search location and expected file count constants in each of the tests to match your
   environment.
