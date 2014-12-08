#include "room/lexer.h"

namespace core = room::core;

room::Lexer::Lexer(std::unique_ptr<room::Source>)
{

}

bool room::Lexer::hasNext()
{
    return false;
}

core::Token room::Lexer::next()
{
    return {};
}
