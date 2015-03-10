project(sst-parser)
cmake_minimum_required(VERSION 2.8)

 include_directories(
     ${SST_PARSER_SOURCE_DIR}
     ${ROOM_INCLUDE_DIR})

 set(SST_PARSER_SOURCES
     parser.cpp)

 add_library(${PROJECT_NAME} ${SST_PARSER_SOURCES})

 showFilesInIDE(
    ${ROOM_INCLUDE_DIR}/room/ast/perspective.h
    ${ROOM_INCLUDE_DIR}/room/ast/space.h
    ${ROOM_INCLUDE_DIR}/room/ast/subject.h
    ${ROOM_INCLUDE_DIR}/room/ast/symbol.h
    ${ROOM_INCLUDE_DIR}/room/sst/parser.h)
