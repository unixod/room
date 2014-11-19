#include "room/lexer.h"
#include "room/utils/make_unique.h"
#include "room/core/tokens/end.h"

namespace core = room::core;

room::Lexer::Lexer(std::unique_ptr<room::Source>)
{

}

bool room::Lexer::hasNext()
{
    return false;
}

std::unique_ptr<core::Token> room::Lexer::next()
{
    return std::make_unique<core::tokens::End>();
}
