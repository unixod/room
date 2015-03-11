project(lst-parser)
cmake_minimum_required(VERSION 2.8)

include_directories(
    ${LST_PARSER_SOURCE_DIR}
    ${ROOM_INCLUDE_DIR})

set(LST_PARSER_SOURCES
    parser.cpp)

add_library(${PROJECT_NAME} ${LST_PARSER_SOURCES})

showFilesInIDE(
    ${ROOM_INCLUDE_DIR}/room/ast/atom.h
    ${ROOM_INCLUDE_DIR}/room/ast/set.h
    ${ROOM_INCLUDE_DIR}/room/lst/symbol.h
    ${ROOM_INCLUDE_DIR}/room/lst/parser.h)
