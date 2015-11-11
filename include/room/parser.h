#ifndef ROOM_PARSER_H
#define ROOM_PARSER_H

#include <functional>
#include "room/lexer/token.h"

namespace room {

namespace parser {

class Ast;

} // namespace parser

// generator whill be called until the returns End
parser::Ast parse(std::function<lexer::Token()> generator);

} // namespace room

#endif // ROOM_PARSER_H
