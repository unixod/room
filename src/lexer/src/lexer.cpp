#include <istream>
#include "room/lexer.h"
#include "lexer_p.h"

namespace core = room::core;

room::Lexer::Lexer(std::istream &is)
    : d_ptr{new Private{is.rdbuf(), 256}}
{}

room::Lexer::Lexer(room::Lexer &&lexer)
    : d_ptr{std::move(lexer.d_ptr)}
{}

room::Lexer::~Lexer()
{}

room::Lexer & room::Lexer::operator >> (core::Token &token)
{
    if (not d_ptr->atEnd) {
        std::get<core::TokenClass>(token) = details::nextToken(d_ptr->re2cState, [&](std::size_t n){ d_ptr->refill(n); });
        if (std::get<core::TokenClass>(token) != core::token::Class::End) {
            // following two lines are equal to:
            // std::get<core::TokenLexeme>(token) = d_ptr->stash + std::string{re2cState.lexemeStart, re2cState.lexemeEnd};
            d_ptr->stash.append(d_ptr->re2cState.lexemeStart, d_ptr->re2cState.lexemeEnd);
            std::swap(std::get<core::TokenLexeme>(token), d_ptr->stash);
            d_ptr->stash.clear();
        } else {
            d_ptr->atEnd = true;
        }
    }

    return *this;
}

room::Lexer::operator bool() const
{
    return not d_ptr->atEnd;
}
