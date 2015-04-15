cmake_minimum_required(VERSION 3.2)

include(manWaveform.cmake)
include(../../cmake/qt.cmake)

message(STATUS "Building manWaveform...")

file(GLOB_RECURSE manWaveform_SOURCES src/*.cpp)
file(GLOB_RECURSE manWaveform_HEADERS include/*.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(manWaveform STATIC
  ${manWaveform_SOURCES}
  ${manWaveform_HEADERS})

qt5_use_modules(manWaveform Quick)
