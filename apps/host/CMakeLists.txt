cmake_minimum_required(VERSION 3.2)

include(../../cmake/boost.cmake)
include(../../cmake/asiosdk.cmake)

message(STATUS ${Boost_INCLUDE_DIRS})
message(STATUS ${ASIOSDK_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIRS} ${ASIOSDK_INCLUDE_DIR})

set(TestApp_SOURCES "main.cpp")
if (WIN32)
  add_executable(TestApp ${TestApp_SOURCES})
elseif (APPLE)
  add_executable(TestApp ${TestApp_SOURCES})
endif ()

target_link_libraries(TestApp ${Boost_LIBRARIES} asiosdk)
