project(lexer)
cmake_minimum_required(VERSION 2.8)

if(USE_RE2C)
    if(NOT IS_ABSOLUTE ${USE_RE2C})
        get_filename_component(USE_RE2C ${CMAKE_BINARY_DIR}/${USE_RE2C} ABSOLUTE)
    endif()

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

    set(LEXER_RE2C_CPP lexer.re2c.cpp)

    add_custom_command(OUTPUT ${LEXER_RE2C_CPP}
        COMMAND ${RE2C_EXECUTABLE} -c -o ${LEXER_RE2C_CPP} ${CMAKE_CURRENT_LIST_DIR}/lexer.re2c
        DEPENDS lexer.re2c
    )

    set(LEXER_RE2C_CPP ${CMAKE_CURRENT_BINARY_DIR}/${LEXER_RE2C_CPP})
else()
    # use pre-generated re2c-lexer
    set(LEXER_RE2C_CPP lexer.re2c.cpp)
endif()

include_directories(
    ${ROOM_INCLUDE_DIR})

set(LEXER_SOURCES
    lexer.cpp
    ${LEXER_RE2C_CPP})

add_library(${PROJECT_NAME} ${LEXER_SOURCES})

showFilesInIDE(
    lexer.re2c)
