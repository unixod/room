#ifndef ROOM_CORE_TOKEN_H
#define ROOM_CORE_TOKEN_H

#include <string>
#include <cstdint>

namespace room {
namespace core {
namespace token {

enum class Class {
    Space, Atom, End
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
