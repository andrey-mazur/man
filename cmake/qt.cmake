cmake_minimum_required(VERSION 3.2)

if (Qt5_FOUND)
    # everything is cool, we've already found Qt before
else ()
    if (NOT DEFINED QT_DIR)
        if (NOT DEFINED _3RDPARTY_LIBS_DIR)
            message(FATAL_ERROR "QT_DIR should be defined explicitely")
        else ()
            message(STATUS "Searching for Qt in 3rd-party libs folder: "
                "${_3RDPARTY_LIBS_DIR}")

            if (WIN32)
                find_file(qmake_ NAMES qmake.exe PATHS ${_3RDPARTY_LIBS_DIR})
            elseif (APPLE)
                find_file(qmake_ NAMES qmake PATHS ${_3RDPARTY_LIBS_DIR})
            endif ()

            if (NOT qmake_)
                message(FATAL_ERROR "Could not find qmake")
            endif ()

            get_filename_component(binFolder_ ${qmake_} DIRECTORY)
            get_filename_component(rootFolder_ "${binFolder_}/../" ABSOLUTE)
            set(QT_DIR ${rootFolder_})
        endif ()
    endif ()

    if (NOT Qt5_FOUND)
        message(STATUS "Searching for Qt in: ${QT_DIR}")

        if (NOT DEFINED CMAKE_PREFIX_PATH
            OR NOT ${CMAKE_PREFIX_PATH} MATCHES "${QT_DIR}")
            set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${QT_DIR})
            set(CMAKE_AUTOMOC ON)
        endif ()

        find_package(Qt5 REQUIRED Core Gui Declarative Quick)

        if (Qt5_FOUND)
            # message(STATUS "Qt5 found")
        endif ()
    endif ()
endif ()
