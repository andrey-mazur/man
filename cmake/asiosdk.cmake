cmake_minimum_required(VERSION 3.2)

if (NOT DEFINED ASIOSDK_DIR)
    message(FATAL_ERROR "ASIOSDK_DIR should be defined explicitely")
else ()
    message(STATUS "Building asiosdk from: ${ASIOSDK_DIR}")

    if (WIN32)
        string(REPLACE "\\" "/" ASIOSDK_DIR "${ASIOSDK_DIR}")
    endif ()
endif ()

set(headers_
    ${ASIOSDK_DIR}/common/asio.h
    ${ASIOSDK_DIR}/common/asiosys.h
    ${ASIOSDK_DIR}/host/asiodrivers.h
    ${ASIOSDK_DIR}/host/ginclude.h
    ${ASIOSDK_DIR}/host/ASIOConvertSamples.h)

set(sources_
	${ASIOSDK_DIR}/common/asio.cpp
	${ASIOSDK_DIR}/host/asiodrivers.cpp
    ${ASIOSDK_DIR}/host/ASIOConvertSamples.cpp)

source_group("" FILES ${headers_} ${sources_})

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267 /wd4996")
    set(platformDir_ pc)
else()
    set(platformDir_ mac)
endif()

file(GLOB platformHeaders_
    ${ASIOSDK_DIR}/host/${platformDir_}/*.h)
file(GLOB platformSources_
    ${ASIOSDK_DIR}/host/${platformDir_}/*.cpp)
source_group(${platformDir_} FILES ${platformHeaders_} ${platformSources_})

include_directories(${ASIOSDK_DIR}/common
    ${ASIOSDK_DIR}/host
    ${ASIOSDK_DIR}/host/${platformDir_})

get_filename_component(ASIOSDK_INCLUDE_DIR "${ASIOSDK_DIR}/../" ABSOLUTE CACHE)

add_library(asiosdk STATIC
    ${headers_}
    ${sources_}
    ${platformSources_}
    ${platformHeaders_})
