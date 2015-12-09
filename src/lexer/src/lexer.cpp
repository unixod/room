#include <istream>
#include "room/common.h"
#include "room/lexer.h"
#include "lexer_p.h"

namespace lexer = room::lexer;
namespace detail = lexer::detail;

using lexer::Token;

room::Lexer::Lexer(std::istream &is)
    : _impl{new Private{is.rdbuf(), 256}}
{}

room::Lexer::Lexer(room::Lexer &&lexer)
    : _impl{std::move(lexer._impl)}
{}

room::Lexer::~Lexer()
{}

Token room::Lexer::nextToken()
{
    if (_impl->atEnd) {
        throw std::runtime_error {
            ERROR_MSG "Internal error: end of input"
        };
    }
    
    auto category = detail::nextToken(_impl->re2cState, [&](std::size_t n){ _impl->refill(n); });

    Token::Lexeme lexeme;

    switch (category) {
    case Token::Category::End:
    case Token::Category::Error:
        _impl->atEnd = true;
        _impl->stash.clear();
        break;
    default:
        _impl->stash.append(_impl->re2cState.lexemeStart, _impl->re2cState.lexemeEnd);

        if (category == Token::Category::Atom) {
            detail::unescapeAtomLexeme(_impl->stash);
        }

        std::swap(lexeme, _impl->stash);
    }

    return {category, lexeme};
}

std::size_t room::Lexer::currentOffset() const noexcept
{
    return _impl->currentOffset();
}

