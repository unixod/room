cmake_minimum_required(VERSION 2.8)

include(addTest)
include(precompileCatch)

include_directories(
    ${3RDPARTY_LIB_DIR}/Catch/single_include
    ${ROOM_INCLUDE_DIR})

precompileCatch(catch)

addTest("LST-parser:empty_source" empty_source
        OBJECT_LIBS ${catch}
        LIBS lexer lst-parser)

addTest("LST-parser:correct_source" correct_source
        OBJECT_LIBS ${catch}
        LIBS lexer lst-parser)

addTest("LST-parser:incorrect_source" incorrect_source
        OBJECT_LIBS ${catch}
        LIBS lexer lst-parser)

showFilesInIDE(
    ${ROOM_INCLUDE_DIR}/testing/facilities.h)
