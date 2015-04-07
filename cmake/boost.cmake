cmake_minimum_required(VERSION 3.2)

message(STATUS "Building boost project")

set(BOOST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../external/boost")
set(BOOST_BUILD_DIR "${CMAKE_BINARY_DIR}/boost")

set(BOOST_ROOT ${BOOST_BUILD_DIR})
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost
  COMPONENTS system thread)
if (Boost_FOUND)
  #message(STATUS ${Boost_LIBRARIES})
else ()
  message(STATUS "Building from sources...")
  if (WIN32)
    set(BOOST_BOOTSTRAP ".\\bootstrap.bat")
    set(BOOST_B2 ".\\b2.exe")
  elseif (APPLE)
    set(BOOST_BOOTSTRAP "./bootstrap.sh")
    set(BOOST_B2 "./b2")
  endif ()

  set(BOOST_BOOTSTRAP_RESULT)
  set(BOOST_BOOTSTRAP_OUTPUT)
  execute_process(COMMAND ${BOOST_BOOTSTRAP}
    RESULT_VARIABLE BOOST_BOOTSTRAP_RESULT
    OUTPUT_VARIABLE BOOST_BOOTSTRAP_OUTPUT
    WORKING_DIRECTORY ${BOOST_SOURCE_DIR})

  if (${BOOST_BOOTSTRAP_RESULT})
    message(FATAL_ERROR "Failed boost bootstraping")
  else ()
    #message(STATUS ${BOOST_BOOTSTRAP_OUTPUT})

    set(B2_COMMAND_AND_OPTIONS ${BOOST_B2}
      --prefix=${BOOST_BUILD_DIR}
      #--stagedir=${BOOST_BUILD_DIR}/stage
      --builddir=${BOOST_BUILD_DIR}/build
      --with-system
      --with-thread
      toolset=msvc-12.0
      architecture=x86
      address-model=64
      link=static
      threading=multi
      install
    )

    execute_process(COMMAND ${B2_COMMAND_AND_OPTIONS}
      WORKING_DIRECTORY ${BOOST_SOURCE_DIR})
  endif ()
endif ()
