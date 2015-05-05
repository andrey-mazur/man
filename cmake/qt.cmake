cmake_minimum_required(VERSION 3.2)

if (NOT DEFINED QT_DIR)
    message(FATAL_ERROR "QT_DIR should be defined explicitely")
else ()
    if (NOT Qt5_FOUND)
        message(STATUS "Building Qt from: ${QT_DIR}")

        if (NOT DEFINED CMAKE_PREFIX_PATH
        OR NOT ${CMAKE_PREFIX_PATH} MATCHES "${QT_DIR}")
            set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_DIR})
            set(CMAKE_AUTOMOC ON)
        endif ()

        find_package(Qt5 REQUIRED Core Gui Declarative Quick)

        if (Qt5_FOUND)
            #message(STATUS "Qt5 found")
        endif ()
    endif ()
endif ()
