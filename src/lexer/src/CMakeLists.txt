project(lexer)
cmake_minimum_required(VERSION 2.8)

set(LEXER_RE2C_CPP lexer.re2c.cpp)
set(LEXER_RE2C_H lexer.re2c.h)

if(USE_RE2C)
    # where to store generated files
    if(RE2C_OVERRIDE_SOURCES)
        set(RE2C_OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/pregenerated)
    else()
        set(RE2C_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()

    if(NOT IS_ABSOLUTE ${USE_RE2C})
        get_filename_component(USE_RE2C ${CMAKE_BINARY_DIR}/${USE_RE2C} ABSOLUTE)
    endif()

    # check that specified re2c binary is work
    message(STATUS "Check for working re2c using: ${USE_RE2C}")
    execute_process(COMMAND ${USE_RE2C} --version
                    OUTPUT_VARIABLE RE2C_VERSION
                    ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)

    if(RE2C_VERSION MATCHES "^re2c [0-9]")
        message(STATUS "Check for working re2c using: ${USE_RE2C} - works")
        set(RE2C_EXECUTABLE "${USE_RE2C}")
    else()
        message(FATAL_ERROR "Check for working re2c using: ${USE_RE2C} - broken")
    endif()
    unset(RE2C_VERSION)

    add_custom_command(OUTPUT ${LEXER_RE2C_CPP}
        COMMAND ${RE2C_EXECUTABLE} -c -o ${LEXER_RE2C_CPP} -t ${LEXER_RE2C_H} ${CMAKE_CURRENT_LIST_DIR}/lexer.re2c
        WORKING_DIRECTORY ${RE2C_OUTPUT_DIR}
        DEPENDS lexer.re2c
    )

    set(LEXER_RE2C_CPP ${RE2C_OUTPUT_DIR}/${LEXER_RE2C_CPP})
    include_directories(
        ${RE2C_OUTPUT_DIR})
else()
    # use pregenerated sources
    set(LEXER_RE2C_CPP ${RE2C_OUTPUT_DIR}/${LEXER_RE2C_CPP})
    include_directories(
        pregenerated)
endif()

include_directories(
    ${LEXER_SOURCE_DIR}
    ${ROOM_INCLUDE_DIR})

set(LEXER_SOURCES
    lexer.cpp
    ${LEXER_RE2C_CPP})

add_library(${PROJECT_NAME} ${LEXER_SOURCES})

showFilesInIDE(
    lexer.re2c)
