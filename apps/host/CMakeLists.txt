cmake_minimum_required(VERSION 3.2)

include(../../cmake/boost.cmake)

set(TestApp_SOURCES "main.cpp")
if (WIN32)
  add_executable(TestApp ${TestApp_SOURCES})
elseif (APPLE)
  add_executable(TestApp ${TestApp_SOURCES})
endif ()

target_link_libraries(TestApp ${Boost_LIBRARIES})
