cmake_minimum_required(VERSION 3.2)

include(manWaveform.cmake)
include(../../cmake/qt.cmake)

message(STATUS "Building manWaveform...")

file(GLOB_RECURSE manWaveform_SOURCES src/*.cpp)
file(GLOB_RECURSE manWaveform_HEADERS include/*.h)
file(GLOB manWaveform_QMLDIR qmldir)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${QT_DIR}/include/QtCore/5.4.1/QtCore)
include_directories(${QT_DIR}/include/QtGui/5.4.1)
include_directories(${QT_DIR}/include/QtGui/5.4.1/QtGui)
include_directories(${QT_DIR}/include/QtQuick/5.4.1)
include_directories(${QT_DIR}/include/QtQuick/5.4.1/QtQuick)
add_library(manWaveform SHARED
    ${manWaveform_SOURCES}
    ${manWaveform_HEADERS}
    )

qt5_use_modules(manWaveform Quick)

add_custom_command(TARGET manWaveform
    COMMAND ${CMAKE_COMMAND} -E copy
    ${manWaveform_QMLDIR} $<TARGET_FILE_DIR:manWaveform>)
