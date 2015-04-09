cmake_minimum_required(VERSION 3.2)

include(../../cmake/boost.cmake)
include(../../cmake/qt.cmake)
if (WIN32)
  include(../../cmake/asiosdk.cmake)
endif ()

include_directories(${Boost_INCLUDE_DIRS})

set(TestApp_SOURCES "main.cpp")

file(GLOB_RECURSE TestApp_QMLS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} qml/*.qml)
file(GLOB_RECURSE TestApp_QRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} qml/*.qrc)
qt5_add_resources(TestApp_RESOURCES ${TestApp_QRCS})

if (WIN32)
  include_directories(${ASIOSDK_INCLUDE_DIR})

  add_executable(TestApp WIN32
    ${TestApp_SOURCES}
    ${TestApp_RESOURCES}
    ${TestApp_QMLS})

  target_link_libraries(TestApp asiosdk)
elseif (APPLE)
  add_executable(TestApp MACOSX_BUNDLE
    ${TestApp_SOURCES}
    ${TestApp_RESOURCES}
    ${TestApp_QMLS})
endif ()

qt5_use_modules(TestApp Core Gui Widgets Declarative Quick)
target_link_libraries(TestApp
  ${Boost_LIBRARIES})
