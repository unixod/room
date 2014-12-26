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
    std::get<core::TokenClass>(token) = details::nextToken(d_ptr->re2cState, [&](std::size_t n){ d_ptr->refill(n); });
//    return std::get<core::TokenClass>(d_ptr->token) == token::Class::End;
    return *this;
}

room::Lexer::operator bool() const
{
//    return d_ptr->source == d_ptr->sourceEnd;
}
