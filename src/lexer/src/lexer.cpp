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
        auto &tokenClass = std::get<core::TokenClass>(token);

        tokenClass = details::nextToken(d_ptr->re2cState, [&](std::size_t n){ d_ptr->refill(n); });
        if (tokenClass != core::token::Class::End) {
            d_ptr->stash.append(d_ptr->re2cState.lexemeStart, d_ptr->re2cState.lexemeEnd);

            if (tokenClass == core::token::Class::Atom) {
                details::unescapeAtomLexeme(d_ptr->stash);
            }

            std::swap(std::get<core::TokenLexeme>(token), d_ptr->stash);
            d_ptr->stash.clear();
        } else {
            d_ptr->atEnd = true;
        }
    }

    return *this;
}

std::size_t room::Lexer::currentOffset() const noexcept
{
    return d_ptr->currentOffset();
}

room::Lexer::operator bool() const
{
    return not d_ptr->atEnd;
}
