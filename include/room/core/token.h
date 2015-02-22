#ifndef ROOM_CORE_TOKEN_H
#define ROOM_CORE_TOKEN_H

#include <string>

namespace room {
namespace core {
namespace token {

enum class Class {
    SpaceBegin,
    SpaceEnd,
    Quotation,
    Atom,
    End,
    Error
};

typedef std::string Lexeme;

} // namespace token

enum {
    TokenClass, TokenLexeme
};

typedef std::pair<token::Class, token::Lexeme> Token;

} // namespace core
} // namespace room

#endif // ROOM_CORE_TOKEN_H
