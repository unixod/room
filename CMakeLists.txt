cmake_minimum_required(VERSION 2.8)

#===============================================================================
# CMake configuration
#===============================================================================
option(WITH_TESTS               "Enable testing" OFF)
option(USE_RE2C                 "Use re2c instead using preprocessed sources (full path needed) to regenerate lexer")
option(RE2C_OVERRIDE_SOURCES    "Generate files in source dir (useful during development)")

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
    "${CMAKE_CXX_FLAGS} ${COMPILER_WALL_FLAG} ${COMPILER_CXX14_FLAG}")

#===============================================================================
# Setup project
#===============================================================================
include(showFilesInIDE)

set(ROOM_INCLUDE_DIR
    "${CMAKE_SOURCE_DIR}/include")

set(3RDPARTY_LIB_DIR
    "${CMAKE_SOURCE_DIR}/3rd_party")

#add_library(catch.precompiled OBJECT ${CMAKE_BINARY_DIR}/catch.precompiled.cpp)
#add_custom_command(gen.catch.precompiled
#    COMMAND ${CMAKE_COMMAND} echo -E "#define CATCH_CONFIG_MAIN\n#include <catch>" > ${CMAKE_BINARY_DIR}/catch.precompiled.cpp)
#add_dependencies(catch.precompiled gen.catch.precompiled)

add_subdirectory(src)

