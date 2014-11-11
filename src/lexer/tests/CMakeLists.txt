cmake_minimum_required(VERSION 2.8)

include(addTest)

include_directories(
    ${3RDPARTY_LIB_DIR}/Catch/single_include
    ${ROOM_INCLUDE_DIR})

addTest("Lexer::tokens::atom" atom_token lexer)

