cmake_minimum_required(VERSION 2.8)

#===============================================================================
# CMake configuration
#===============================================================================
option(WITH_TESTS  "Enable testing" OFF)

if(WITH_TESTS)
    enable_testing()
endif()

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#===============================================================================
# Setup compiler flags
#===============================================================================
include(configCXX)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${COMPILER_WALL_FLAG} ${COMPILER_CXX11_FLAG}")

#===============================================================================
# Setup project
#===============================================================================
include(showFilesInIDE)

set(ROOM_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/include")

set(3RDPARTY_LIB_DIR
    "${CMAKE_SOURCE_DIR}/3rd_party")

add_subdirectory(src)

