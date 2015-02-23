cmake_minimum_required(VERSION 2.8)

include(addTest)
include(precompileCatch)

include_directories(
    ${3RDPARTY_LIB_DIR}/Catch/single_include
    ${ROOM_INCLUDE_DIR})

precompileCatch(catch)

addTest("Lexer::tokens::atom" atom_token
        OBJECT_LIBS ${catch}
        LIBS lexer)
