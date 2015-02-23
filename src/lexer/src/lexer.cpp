#include <istream>
#include "room/lexer.h"
#include "lexer_p.h"

namespace lexer = room::lexer;

room::Lexer::Lexer(std::istream &is)
    : d_ptr{new Private{is.rdbuf(), 256}}
{}

room::Lexer::Lexer(room::Lexer &&lexer)
    : d_ptr{std::move(lexer.d_ptr)}
{}

room::Lexer::~Lexer()
{}

room::Lexer & room::Lexer::operator >> (lexer::Token &token)
{
    if (not d_ptr->atEnd) {
        auto &tokenClass = std::get<lexer::TokenClass>(token);

        tokenClass = details::nextToken(d_ptr->re2cState, [&](std::size_t n){ d_ptr->refill(n); });
        if (tokenClass != token::Class::End) {
            d_ptr->stash.append(d_ptr->re2cState.lexemeStart, d_ptr->re2cState.lexemeEnd);

            if (tokenClass == token::Class::Atom) {
                details::unescapeAtomLexeme(d_ptr->stash);
            }

            std::swap(std::get<lexer::TokenLexeme>(token), d_ptr->stash);
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
