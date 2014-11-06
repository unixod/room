project(lexer)
cmake_minimum_required(VERSION 2.8)

include_directories(
    ${ROOM_INCLUDE_DIR})

set(LEXER_SOURCES
    lexer.cpp)

add_library(${PROJECT_NAME} ${LEXER_SOURCES})

showFilesInIDE(
    lexer.re2c)
