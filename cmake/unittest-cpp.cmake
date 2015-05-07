cmake_minimum_required(VERSION 3.2)
project(UnitTest++)

option(UTPP_USE_PLUS_SIGN
  "Set this to OFF is you with to use '-cpp' instead of '++' in lib/include paths" ON)

# get the main sources
file(GLOB headers_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/UnitTest++/*.h)
file(GLOB sources_ RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/UnitTest++/*.cpp)
source_group("" FILES ${headers_} ${sources_})

set(UNITTESTCPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src CACHE STRING "Unittest++ headers path")

# get platform specific sources
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    set(platformDir_ Win32)
else()
    set(platformDir_ Posix)
endif(WIN32)

file(GLOB platformHeaders_ RELATIVE
  ${CMAKE_CURRENT_SOURCE_DIR} src/UnitTest++/${platformDir_}/*.h)
file(GLOB platformSources_ RELATIVE
  ${CMAKE_CURRENT_SOURCE_DIR} src/UnitTest++/${platformDir_}/*.cpp)
source_group(${platformDir_} FILES ${platformHeaders_} ${platformSources_})

# create the lib
add_library(UnitTest++ STATIC
  ${headers_}
  ${sources_}
  ${platformHeaders_}
  ${platformSources_})

if(${UTPP_USE_PLUS_SIGN})
	set_target_properties(UnitTest++ PROPERTIES OUTPUT_NAME UnitTest++)
endif()
