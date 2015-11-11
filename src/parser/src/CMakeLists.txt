project(parser)
cmake_minimum_required(VERSION 2.8)

include_directories(
    ${PARSER_SOURCE_DIR}
    ${ROOM_INCLUDE_DIR})

set(PARSER_SOURCES
    parser.cpp
    alpha/parser.cpp)

add_library(${PROJECT_NAME}
    ${PARSER_SOURCES})

showFilesInIDE(
    ${ROOM_INCLUDE_DIR}/room/parser.h)
