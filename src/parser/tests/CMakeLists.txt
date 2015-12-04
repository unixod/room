cmake_minimum_required(VERSION 2.8)

include(addTest)
include(precompileCatch)

include_directories(
    ${PARSER_SOURCE_DIR}
    ${ROOM_INCLUDE_DIR}
    ${3RDPARTY_LIB_DIR}/Catch/single_include)

precompileCatch(catch)

#addTest("LST-parser:empty_source" detail/empty_source
#        OBJECT_LIBS ${catch}
#        LIBS lexer lst-parser)

#addTest("LST-parser:correct_source" detail/correct_source
#        OBJECT_LIBS ${catch}
#        LIBS lexer lst-parser)

#addTest("LST-parser:incorrect_source" detail/incorrect_source
#        OBJECT_LIBS ${catch}
#        LIBS lexer lst-parser)

addTest("detail:AtomSet" detail/atom-set
        OBJECT_LIBS ${catch}
        LIBS parser)

addTest("detail:AtomSet" detail/alpha-parser
        OBJECT_LIBS ${catch}
        LIBS parser)

showFilesInIDE(
    ${ROOM_INCLUDE_DIR}/testing/facilities.h)
