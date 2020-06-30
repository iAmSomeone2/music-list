cmake_minimum_required(VERSION 3.16)

project(MusicList VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

option(ENABLE_TESTS "Enable the Google Test framework for the project." on)

include_directories(src/core)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Disabling tests for Release build")
    set(ENABLE_TESTS off)
endif()

find_package(SQLite3 REQUIRED)
find_package(JsonCpp REQUIRED)

configure_file(src/config.hpp.in src/config.hpp @ONLY)

add_subdirectory(src)

if(ENABLE_TESTS)
    find_package(GTest)
    if (GTest_FOUND)
        enable_testing()
        add_subdirectory(test)

        file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}/test/)
    endif()
endif()
