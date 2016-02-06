cmake_minimum_required(VERSION 2.8)

include(addTest)
include(precompileCatch)

include_directories(
    ${PARSER_SOURCE_DIR}
    ${ROOM_INCLUDE_DIR}
    ${3RDPARTY_LIB_DIR}/Catch/single_include)

precompileCatch(catch)

addTest("detail:AtomSet" detail/atom-set
        OBJECT_LIBS ${catch}
        LIBS parser)

addTest("detail:alpha-parser" detail/alpha-parser
        OBJECT_LIBS ${catch}
        LIBS parser)
