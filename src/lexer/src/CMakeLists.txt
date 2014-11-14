project(lexer)
cmake_minimum_required(VERSION 2.8)

set(LEXER_RE2C_FILE             lexer.re2c)
set(LEXER_RE2C_GEN_H            lexer.re2c.h)
set(LEXER_RE2C_GEN_CPP          lexer.re2c.cpp)
set(RE2C_GEN_FILES_LOCATION     pregenerated)

if(USE_RE2C)
    # where to store the generated files
    if(RE2C_OVERRIDE_SOURCES)
        set(RE2C_GEN_FILES_LOCATION ${CMAKE_CURRENT_LIST_DIR}/pregenerated)
    else()
        set(RE2C_GEN_FILES_LOCATION ${CMAKE_CURRENT_BINARY_DIR})
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

    add_custom_command(OUTPUT ${RE2C_GEN_FILES_LOCATION}/${LEXER_RE2C_GEN_CPP}
        COMMAND ${RE2C_EXECUTABLE} -c -o ${LEXER_RE2C_GEN_CPP} -t ${LEXER_RE2C_GEN_H} ${CMAKE_CURRENT_LIST_DIR}/${LEXER_RE2C_FILE}
        WORKING_DIRECTORY ${RE2C_GEN_FILES_LOCATION}
        DEPENDS ${LEXER_RE2C_FILE}
    )
endif()

include_directories(
    ${LEXER_SOURCE_DIR}
    ${RE2C_GEN_FILES_LOCATION}
    ${ROOM_INCLUDE_DIR})

set(LEXER_SOURCES
    lexer.cpp
    utils/source_string.cpp
    ${RE2C_GEN_FILES_LOCATION}/${LEXER_RE2C_GEN_CPP})

add_library(${PROJECT_NAME} ${LEXER_SOURCES})

showFilesInIDE(
    ${ROOM_INCLUDE_DIR}/room/lexer.h
    ${ROOM_INCLUDE_DIR}/room/source.h
    ${ROOM_INCLUDE_DIR}/room/utils/source_string.h
#    ${ROOM_INCLUDE_DIR}/room/utils/source_file.h
    ${LEXER_RE2C_FILE})
