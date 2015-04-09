cmake_minimum_required(VERSION 3.2)

include(../../cmake/boost.cmake)
include(../../cmake/qt.cmake)
if (WIN32)
  include(../../cmake/asiosdk.cmake)
endif ()

include_directories(${Boost_INCLUDE_DIRS})

set(TestApp_SOURCES "main.cpp")
if (WIN32)
  include_directories(${ASIOSDK_INCLUDE_DIR})

  add_executable(TestApp WIN32 ${TestApp_SOURCES})

  target_link_libraries(TestApp asiosdk)
elseif (APPLE)
  add_executable(TestApp MACOSX_BUNDLE ${TestApp_SOURCES})
endif ()

target_link_libraries(TestApp
  ${Boost_LIBRARIES}
  Qt5::Core Qt5::Gui Qt5::Declarative)
