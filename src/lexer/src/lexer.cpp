#include "room/lexer.h"
#include "lexer_p.h"

namespace core = room::core;

room::Lexer::Lexer(std::unique_ptr<room::Source>)
{}

room::Lexer::Lexer(room::Lexer &&lexer)
    : d_ptr{std::move(lexer.d_ptr)}
{}

room::Lexer::~Lexer()
{}

bool room::Lexer::hasNext()
{
    return false;
}

core::Token room::Lexer::next()
{
    return {};
}
