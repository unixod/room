#ifndef ROOM_CORE_TOKEN_H
#define ROOM_CORE_TOKEN_H

#include <string>

namespace room {
namespace lexer {

class Token {
public:
    enum class Category {
        SpaceBegin,
        SpaceEnd,
        Quotation,
        Atom,
        End,
        Error
    };

    using Lexeme = std::string;

    Token(Category c, Lexeme l)
        : category{std::move(c)}, lexeme{std::move(l)}
    {}

    Category category;
    Lexeme lexeme;
};


inline bool operator == (const Token &t1, const Token &t2)
{
    return (t1.category == t2.category) &&
             ((t1.category != Token::Category::Atom) || (t1.lexeme == t2.lexeme));  // currently, lexeme comparision is sensible for Atoms only
}

inline bool operator != (const Token &t1, const Token &t2)
{
    return !(t1 == t2);
}

} // namespace lexer
} // namespace room

#endif // ROOM_CORE_TOKEN_H
