cmake_minimum_required(VERSION 3.2)

include(../../cmake/boost.cmake)
include(../../cmake/qt.cmake)

add_subdirectory(../../libs/manWaveform manWaveform)
add_subdirectory(../../libs/manAudioDevice manAudioDevice)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${manAudioDevice_INCLUDE_DIR})

set(TestApp_SOURCES "main.cpp")

file(GLOB_RECURSE TestApp_QMLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} qml/*.qml)
file(GLOB_RECURSE TestApp_QRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} qml/*.qrc)
qt5_add_resources(TestApp_RESOURCES ${TestApp_QRCS})

if (WIN32)
    add_executable(TestApp WIN32
    ${TestApp_SOURCES}
    ${TestApp_RESOURCES}
    ${TestApp_QMLS})
elseif (APPLE)
    add_executable(TestApp MACOSX_BUNDLE
    ${TestApp_SOURCES}
    ${TestApp_RESOURCES}
    ${TestApp_QMLS})
endif ()

qt5_use_modules(TestApp Core Gui Widgets Declarative Quick)
target_link_libraries(TestApp
    ${Boost_LIBRARIES}
    manAudioDevice
    manWaveform)
