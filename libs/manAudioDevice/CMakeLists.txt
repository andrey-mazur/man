cmake_minimum_required(VERSION 3.2)

message(STATUS "Building manAudioDevice...")

include(../../cmake/boost.cmake)
include_directories(${Boost_INCLUDE_DIRS})

if (WIN32)
    include(../../cmake/asiosdk.cmake)
    include_directories(${ASIOSDK_INCLUDE_DIR})

    set(platformDir_ pc)
elseif (APPLE)
    set(platformDir_ mac)
endif ()

file(GLOB_RECURSE headers_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    include/*.h)
file(GLOB sources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/*.cpp)
file(GLOB platformSources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/${platformDir_}/*.cpp src/${platformDir_}/*.mm)
file(GLOB platformHeaders_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/${platformDir_}/*.h)

set(manAudioDevice_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE STRING "manAudioDevice include dir")

include_directories(${manAudioDevice_INCLUDE_DIR})

add_definitions(-DNOMINMAX)

add_library(manAudioDevice STATIC
    ${sources_}
    ${headers_}
    ${platformSources_}
    ${platformHeaders_})
